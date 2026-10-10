//! From the `.ref` model to the [IR](crate::ir) (S6a: free operators,
//! unlabelled rules, builtins, the start term).
//!
//! Steps:
//!
//! 1. **Operators.** Every grammar rule declares a symbol with a profile
//!    (argument sorts from its `Type(n)` lexems, result sort from its
//!    `GrammarForSort` block). Builtin codes below 300 are overloaded
//!    (`==` is symbol 8 on `builtinInt` and on `bool`), so an operator is a
//!    (symbol, arguments, result) triple. Its printing form is the **last**
//!    grammar rule of the symbol in the file, as the compiled C runtime
//!    chooses it (`findTextForm`, `src/compiler/runtime/termOut.c`).
//! 2. **Typing** of terms: a symbol is resolved to the profile that fits
//!    the sorts of its arguments and the expected sort.
//! 3. **Reachability** from the start term: the operators of the start
//!    term, then those of the rules (left-hand sides, conditions,
//!    right-hand sides) of every reachable operator. Only reachable sorts
//!    and operators are compiled; constructs that S6a does not support are
//!    refused only when reachable ([`Error::Unsupported`]).
//! 4. **Representation** of the sorts ([`Repr`]): builtin families, enums.
//! 5. **Totality** fixpoint: an operator is total when its unconditional
//!    rules cover every normal form of its arguments, a normal form being a
//!    constructor or a non-total operator at the top (greatest fixpoint:
//!    all total at first, until nothing changes). Non-total operators get
//!    a variant for their stuck terms.
//! 6. **Decision trees** of the defined operators ([`crate::matching`]).

use std::collections::{BTreeMap, HashMap, HashSet};
use std::fmt;

use elan_ref::{DefStrat, Lexem, Program, Rule, RuleBody, SortIndex, SymbolCode, Term, Where};

use crate::ir::*;
use crate::matching;
use crate::names;

/// Why a program cannot be compiled.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Error {
    /// A construct of a later stage: `stage` is `S6b`, `S6c` or `S6d`.
    Unsupported { stage: &'static str, what: String },
    /// The `.ref` file is not what the compiler expects (a bug or a
    /// misread field).
    Invalid(String),
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter) -> fmt::Result {
        match self {
            Error::Unsupported { stage, what } => {
                write!(f, "not supported yet ({stage}): {what}")
            }
            Error::Invalid(m) => write!(f, "invalid program: {m}"),
        }
    }
}

fn unsupported(stage: &'static str, what: impl Into<String>) -> Error {
    Error::Unsupported {
        stage,
        what: what.into(),
    }
}

/// The symbol code of a strategy call in a term (`Call`, `term/codes.h`).
const CALL_SYMBOL: SymbolCode = 144;

/// An operator profile of the `.ref` file (a candidate when typing).
#[derive(Clone, Debug)]
struct Cand {
    symbol: SymbolCode,
    args: Vec<SortIndex>,
    result: SortIndex,
    semantic: i32,
    infos: i32,
    defstrat: DefStrat,
    /// The lexems of the first grammar rule of this profile (declaration).
    decl: Vec<Lexem>,
}

/// A typed term: operators resolved to candidates.
#[derive(Clone, Debug)]
enum TT {
    Op(usize, Vec<TT>),
    Var(u32, SortIndex),
    EVar,
    Int(i64),
    Str(Vec<u8>),
}

/// Typing failure: a type mismatch (try another profile) or a hard error.
enum TypeErr {
    Mismatch(String),
    Hard(Error),
}

impl From<Error> for TypeErr {
    fn from(e: Error) -> TypeErr {
        TypeErr::Hard(e)
    }
}

/// The builtin kind of a sort, from its name.
fn builtin_kind(name: &str) -> Option<BuiltinKind> {
    match name {
        "builtinInt" => Some(BuiltinKind::Int),
        "bool" => Some(BuiltinKind::Bool),
        "builtinString" => Some(BuiltinKind::Str),
        _ => None,
    }
}

/// Decode a `STRING(...)` term: C `char`s (signed bytes), kept as bytes
/// (a string is a C string: it stops at a NUL byte).
fn decode_string(cs: &[i32]) -> Vec<u8> {
    cs.iter()
        .map(|&c| c as u8)
        .take_while(|&c| c != 0)
        .collect()
}

/// Lower a program; `name` is the program name (for the crate).
pub fn lower(p: &Program, name: &str) -> Result<Ir, Error> {
    Lowerer::new(p).run(name)
}

struct Lowerer<'p> {
    p: &'p Program,
    cands: Vec<Cand>,
    by_symbol: HashMap<SymbolCode, Vec<usize>>,
    /// Printing form of each symbol: its last grammar rule.
    print_form: HashMap<SymbolCode, Vec<Lexem>>,
    /// Unlabelled rules by head symbol, in file order.
    rules_by_symbol: HashMap<SymbolCode, Vec<&'p Rule>>,
    int_sort: Option<SortIndex>,
    str_sort: Option<SortIndex>,
    bool_sort: Option<SortIndex>,
}

impl<'p> Lowerer<'p> {
    fn new(p: &'p Program) -> Lowerer<'p> {
        let mut cands: Vec<Cand> = Vec::new();
        let mut by_symbol: HashMap<SymbolCode, Vec<usize>> = HashMap::new();
        let mut print_form = HashMap::new();
        for (g, o) in p.operators() {
            let args: Vec<SortIndex> = o
                .rhs
                .iter()
                .filter_map(|l| match l {
                    Lexem::Type(s) => Some(*s),
                    _ => None,
                })
                .collect();
            print_form.insert(o.symbol, o.rhs.clone());
            let ids = by_symbol.entry(o.symbol).or_default();
            if ids
                .iter()
                .any(|&i| cands[i].args == args && cands[i].result == g.sort)
            {
                continue;
            }
            ids.push(cands.len());
            cands.push(Cand {
                symbol: o.symbol,
                args,
                result: g.sort,
                semantic: o.semantic,
                infos: o.infos,
                defstrat: o.defstrat,
                decl: o.rhs.clone(),
            });
        }
        let mut rules_by_symbol: HashMap<SymbolCode, Vec<&Rule>> = HashMap::new();
        for r in &p.rules {
            // labelled rules: strategies only (S6b); the visibility
            // (`infos`, global or local) is ignored for normalisation, as REM
            // does
            if r.name.is_some() {
                continue;
            }
            if let Term::Fsym { symbol, .. } = r.lhs {
                rules_by_symbol.entry(symbol).or_default().push(r);
            }
        }
        let sort_named = |n: &str| p.sorts.iter().find(|s| s.name == n).map(|s| s.index);
        Lowerer {
            p,
            cands,
            by_symbol,
            print_form,
            rules_by_symbol,
            int_sort: sort_named("builtinInt"),
            str_sort: sort_named("builtinString"),
            bool_sort: sort_named("bool"),
        }
    }

    fn sort_name(&self, s: SortIndex) -> String {
        self.p
            .sort(s)
            .map(|s| s.name.clone())
            .unwrap_or_else(|| format!("sort{s}"))
    }

    fn ident(&self, i: u32) -> String {
        self.p
            .identifier(i)
            .map(str::to_string)
            .unwrap_or_else(|| format!("ident{i}"))
    }

    /// `name(@,@) : (s1 s2) s` for messages and comments.
    fn cand_decl(&self, c: &Cand) -> String {
        let mut s = String::new();
        for l in &c.decl {
            match l {
                Lexem::Type(_) => s.push('@'),
                Lexem::Char(ch) => s.push(char::from_u32(*ch as u32).unwrap_or('?')),
                Lexem::Num(n) | Lexem::String(n) => s.push_str(&n.to_string()),
                Lexem::Ident(i) => {
                    if s.ends_with(|c: char| c.is_alphanumeric()) {
                        s.push(' ');
                    }
                    s.push_str(&self.ident(*i))
                }
                Lexem::Blank => s.push(' '),
            }
        }
        let args: Vec<String> = c.args.iter().map(|&a| self.sort_name(a)).collect();
        if args.is_empty() {
            format!("{} : {}", s.trim(), self.sort_name(c.result))
        } else {
            format!(
                "{} : ({}) {}",
                s.trim(),
                args.join(" "),
                self.sort_name(c.result)
            )
        }
    }

    // ------------------------------------------------------------------
    // Typing
    // ------------------------------------------------------------------

    fn type_term(&self, t: &Term, expected: Option<SortIndex>) -> Result<TT, TypeErr> {
        let check = |s: SortIndex, what: &str| -> Result<(), TypeErr> {
            match expected {
                Some(e) if e != s => Err(TypeErr::Mismatch(format!(
                    "{what} of sort {} where {} is expected",
                    self.sort_name(s),
                    self.sort_name(e)
                ))),
                _ => Ok(()),
            }
        };
        match t {
            Term::Var { index, sort } => {
                check(*sort, "variable")?;
                Ok(TT::Var(*index, *sort))
            }
            Term::EVar { .. } => Ok(TT::EVar),
            Term::Int(n) => {
                let s = self
                    .int_sort
                    .ok_or_else(|| Error::Invalid("integer without builtinInt".into()))?;
                check(s, "integer")?;
                Ok(TT::Int(*n as i32 as i64))
            }
            Term::Str(cs) => {
                let s = self
                    .str_sort
                    .ok_or_else(|| Error::Invalid("string without builtinString".into()))?;
                check(s, "string")?;
                Ok(TT::Str(decode_string(cs)))
            }
            Term::Ident(_) => Err(unsupported("S6d", "builtin identifiers (sort ident)").into()),
            Term::Fsym { args, symbol } => {
                if *symbol == CALL_SYMBOL && matches!(args.as_slice(), [Term::Int(_)]) {
                    let name = match &args[0] {
                        Term::Int(s) => self.p.strategy_name(*s as u32).unwrap_or("?").to_string(),
                        _ => unreachable!(),
                    };
                    return Err(
                        unsupported("S6b", format!("strategy call {name} in a term")).into(),
                    );
                }
                let cands = self.by_symbol.get(symbol).map(Vec::as_slice).unwrap_or(&[]);
                let mut why = format!("symbol {symbol} has no profile");
                for &c in cands {
                    let cand = &self.cands[c];
                    if cand.args.len() != args.len() {
                        why = format!("symbol {symbol}: arity");
                        continue;
                    }
                    if let Some(e) = expected {
                        if e != cand.result {
                            why = format!(
                                "symbol {symbol} of sort {} where {} is expected",
                                self.sort_name(cand.result),
                                self.sort_name(e)
                            );
                            continue;
                        }
                    }
                    let mut typed = Vec::new();
                    let mut ok = true;
                    for (a, &s) in args.iter().zip(&cand.args) {
                        match self.type_term(a, Some(s)) {
                            Ok(t) => typed.push(t),
                            Err(TypeErr::Mismatch(m)) => {
                                why = m;
                                ok = false;
                                break;
                            }
                            Err(h) => return Err(h),
                        }
                    }
                    if ok {
                        return Ok(TT::Op(c, typed));
                    }
                }
                Err(TypeErr::Mismatch(why))
            }
        }
    }

    fn type_top(&self, t: &Term, expected: Option<SortIndex>, ctx: &str) -> Result<TT, Error> {
        match self.type_term(t, expected) {
            Ok(t) => Ok(t),
            Err(TypeErr::Hard(e)) => Err(e),
            Err(TypeErr::Mismatch(m)) => Err(Error::Invalid(format!("{ctx}: cannot type: {m}"))),
        }
    }

    fn sort_of(&self, t: &TT) -> SortIndex {
        match t {
            TT::Op(c, _) => self.cands[*c].result,
            TT::Var(_, s) => *s,
            TT::Int(_) => self.int_sort.unwrap(),
            TT::Str(_) => self.str_sort.unwrap(),
            TT::EVar => unreachable!("sort of an extension variable"),
        }
    }

    // ------------------------------------------------------------------
    // The whole program
    // ------------------------------------------------------------------

    fn run(&self, name: &str) -> Result<Ir, Error> {
        let q = &self.p.query;
        // `start with (S) t`: only normalisation (or the identity) in S6a
        if let Some(s) = q.strategy {
            let identity = self
                .p
                .strategy(s)
                .map_or(false, |d| d.body == vec![elan_ref::StrategyExpr::Id]);
            if !identity {
                let n = self.p.strategy_name(s).unwrap_or("?");
                return Err(unsupported("S6b", format!("strategies: start with ({n})")));
            }
        }
        let start = self.type_top(&q.start_with, Some(q.result_sort), "start term")?;
        if contains_var(&start) {
            return Err(unsupported(
                "S6d",
                "query input: the start term uses the query",
            ));
        }

        // Reachability
        let mut reach = Reach::default();
        self.visit(&start, &mut reach);
        while let Some(c) = reach.todo.pop() {
            self.expand(c, &mut reach)?;
        }
        for &s in &reach.sorts {
            let n = self.sort_name(s);
            if n == "ident" || n.starts_with("intern ") {
                return Err(unsupported("S6d", format!("builtin sort {n}")));
            }
        }

        Builder::new(self, reach).build(name, &start, q)
    }

    fn visit(&self, t: &TT, r: &mut Reach) {
        match t {
            TT::Op(c, args) => {
                r.reach(*c);
                for a in args {
                    self.visit(a, r);
                }
            }
            TT::Var(_, s) => {
                r.sorts.insert(*s);
            }
            TT::Int(_) => {
                r.sorts.insert(self.int_sort.unwrap());
            }
            TT::Str(_) => {
                r.sorts.insert(self.str_sort.unwrap());
            }
            TT::EVar => {}
        }
    }

    /// A newly reached operator: check it, then reach what its rules use.
    fn expand(&self, c: usize, r: &mut Reach) -> Result<(), Error> {
        let cand = &self.cands[c];
        r.sorts.insert(cand.result);
        r.sorts.extend(cand.args.iter().copied());
        let decl = || self.cand_decl(cand);
        match cand.infos {
            0 => {}
            2 => return Err(unsupported("S6c", format!("AC operator {}", decl()))),
            _ => {
                return Err(unsupported(
                    "S6c",
                    format!("commutative operator {}", decl()),
                ))
            }
        }
        if cand.defstrat != DefStrat::None {
            return Err(unsupported(
                "S6b",
                format!("operator with a default strategy {}", decl()),
            ));
        }
        if Some(cand.result) == self.bool_sort {
            // `true` and `false` are values of every bool computation
            for sym in [0, 1] {
                if let Some(t) = self.by_symbol.get(&sym).and_then(|v| {
                    v.iter().copied().find(|&i| {
                        self.cands[i].result == cand.result && self.cands[i].args.is_empty()
                    })
                }) {
                    r.reach(t);
                }
            }
        }
        let rules = self.rules_of(c)?;
        if cand.semantic != 0 {
            let code = cand.semantic.abs();
            if BuiltinOp::from_code(code).is_none() {
                return Err(unsupported(
                    "S6d",
                    format!("builtin operation code {code} ({})", decl()),
                ));
            }
            if !rules.is_empty() {
                return Err(unsupported(
                    "S6d",
                    format!("rules for the builtin operation {}", decl()),
                ));
            }
            return Ok(());
        }
        for (rule, lhs) in rules {
            self.check_rule(rule)?;
            self.visit(&lhs, r);
            if contains_evar(&lhs) {
                continue;
            }
            let RuleBody::Plain { rhs, wheres } = &rule.body else {
                unreachable!("checked")
            };
            for w in wheres {
                for t in self.where_terms(w, cand)? {
                    self.visit(&t, r);
                }
            }
            let rhs = self.type_top(rhs, Some(rule.sort), &format!("rule for {}", decl()))?;
            if contains_evar(&rhs) {
                return Err(Error::Invalid(format!(
                    "extension variable in a right-hand side ({})",
                    decl()
                )));
            }
            self.visit(&rhs, r);
        }
        Ok(())
    }

    fn check_rule(&self, rule: &Rule) -> Result<(), Error> {
        if rule.which_match == 0 {
            return Err(unsupported("S6c", "rule with AC matching"));
        }
        if let RuleBody::Switch(_) = rule.body {
            return Err(unsupported(
                "S6d",
                "rule with a switch/case right-hand side",
            ));
        }
        Ok(())
    }

    /// The typed terms of a condition (patterns included).
    fn where_terms(&self, w: &Where, cand: &Cand) -> Result<Vec<TT>, Error> {
        let ctx = format!("condition of a rule for {}", self.cand_decl(cand));
        Ok(match w {
            Where::If(t) => vec![self.type_top(t, self.bool_sort, &ctx)?],
            Where::Assign { lhs, strategy, rhs } => {
                if let Some(s) = strategy {
                    let n = self.p.strategy_name(*s).unwrap_or("?");
                    return Err(unsupported(
                        "S6b",
                        format!("where with a strategy ({n}) in {ctx}"),
                    ));
                }
                let r = self.type_top(rhs, var_sort(lhs), &ctx)?;
                let l = self.type_top(lhs, Some(self.sort_of(&r)), &ctx)?;
                vec![l, r]
            }
            Where::Match {
                pattern,
                sort,
                strategy,
                rhs,
            } => {
                if let Some(s) = strategy {
                    let n = self.p.strategy_name(*s).unwrap_or("?");
                    return Err(unsupported(
                        "S6b",
                        format!("where with a strategy ({n}) in {ctx}"),
                    ));
                }
                vec![
                    self.type_top(pattern, Some(*sort), &ctx)?,
                    self.type_top(rhs, Some(*sort), &ctx)?,
                ]
            }
            Where::Try(_) => {
                return Err(unsupported(
                    "S6d",
                    format!("choose/try conditions in {ctx}"),
                ))
            }
        })
    }

    /// The unlabelled rules whose left-hand side has operator `c` at the
    /// top, in file order, with their typed left-hand side.
    fn rules_of(&self, c: usize) -> Result<Vec<(&'p Rule, TT)>, Error> {
        let cand = &self.cands[c];
        let mut v = Vec::new();
        for &r in self
            .rules_by_symbol
            .get(&cand.symbol)
            .map(Vec::as_slice)
            .unwrap_or(&[])
        {
            if r.sort != cand.result {
                continue;
            }
            match self.type_term(&r.lhs, Some(r.sort)) {
                Ok(t) if matches!(t, TT::Op(h, _) if h == c) => v.push((r, t)),
                // a rule of another profile of the symbol (overloading)
                Ok(_) => {}
                // a rule with the symbol at the top that fits no profile:
                // refused rather than silently ignored
                Err(TypeErr::Mismatch(m)) => {
                    return Err(Error::Invalid(format!(
                        "a rule of sort {} for symbol {} ({}): cannot type its left-hand side: {m}",
                        self.sort_name(r.sort),
                        cand.symbol,
                        self.cand_decl(cand)
                    )))
                }
                // a rule of this operator that S6a cannot compile
                Err(TypeErr::Hard(e)) => return Err(e),
            }
        }
        Ok(v)
    }
}

/// The sort of a variable term (the expected sort of `where x := () t`).
fn var_sort(t: &Term) -> Option<SortIndex> {
    match t {
        Term::Var { sort, .. } => Some(*sort),
        _ => None,
    }
}

fn contains_var(t: &TT) -> bool {
    match t {
        TT::Var(..) => true,
        TT::Op(_, a) => a.iter().any(contains_var),
        _ => false,
    }
}

fn contains_evar(t: &TT) -> bool {
    match t {
        TT::EVar => true,
        TT::Op(_, a) => a.iter().any(contains_evar),
        _ => false,
    }
}

#[derive(Default)]
struct Reach {
    cands: Vec<usize>,
    seen: HashSet<usize>,
    todo: Vec<usize>,
    sorts: HashSet<SortIndex>,
}

impl Reach {
    fn reach(&mut self, c: usize) {
        if self.seen.insert(c) {
            self.cands.push(c);
            self.todo.push(c);
        }
    }
}

// ----------------------------------------------------------------------
// Building the IR from the reachable part
// ----------------------------------------------------------------------

/// The kind of an operator before the totality analysis.
#[derive(Clone, Debug)]
enum PreKind {
    Constructor,
    Injection,
    BoolConst(bool),
    Builtin(BuiltinOp),
    Defined,
}

struct Builder<'a, 'p> {
    l: &'a Lowerer<'p>,
    reach: Reach,
    /// candidate -> op id
    op_of: HashMap<usize, OpId>,
    /// sort index -> sort id
    sort_of: BTreeMap<SortIndex, SortId>,
}

impl<'a, 'p> Builder<'a, 'p> {
    fn new(l: &'a Lowerer<'p>, reach: Reach) -> Self {
        Builder {
            l,
            reach,
            op_of: HashMap::new(),
            sort_of: BTreeMap::new(),
        }
    }

    fn build(mut self, name: &str, start: &TT, q: &elan_ref::Query) -> Result<Ir, Error> {
        let l = self.l;
        // sorts, in table order
        let mut sort_indices: Vec<SortIndex> = self.reach.sorts.iter().copied().collect();
        sort_indices.sort_unstable();
        let mut used_types = HashSet::new();
        let mut sorts: Vec<SortIr> = Vec::new();
        for &s in &sort_indices {
            self.sort_of.insert(s, sorts.len());
            let n = l.sort_name(s);
            sorts.push(SortIr {
                index: s,
                type_name: names::unique(&mut used_types, names::type_name(&n)),
                name: n,
                repr: Repr::Enum { variants: vec![] },
            });
        }
        // operators, by symbol code then profile
        let mut cs = self.reach.cands.clone();
        cs.sort_by_key(|&c| {
            let k = &l.cands[c];
            (k.symbol, k.result, k.args.clone())
        });
        let mut ops: Vec<OpIr> = Vec::new();
        let mut used_funs = HashSet::new();
        for &c in &cs {
            self.op_of.insert(c, ops.len());
            let k = &l.cands[c];
            let base = names::op_base_name(&self.lexem_texts(&k.decl));
            let print = self.print_items(k);
            ops.push(OpIr {
                symbol: k.symbol,
                fun: names::unique(&mut used_funs, format!("{}_{}", base, k.symbol)),
                name: base,
                variant: None,
                args: k.args.iter().map(|a| self.sort_of[a]).collect(),
                result: self.sort_of[&k.result],
                print,
                decl: l.cand_decl(k),
                kind: OpKind::Constructor,
            });
        }

        // pre-kinds
        let builtin_of = |sid: SortId| builtin_kind(&sorts[sid].name);
        let mut pre: Vec<PreKind> = Vec::with_capacity(ops.len());
        let mut has_rules: Vec<bool> = Vec::new();
        for (i, &c) in cs.iter().enumerate() {
            let k = &l.cands[c];
            let rules = l.rules_of(c)?;
            has_rules.push(!rules.is_empty());
            let p = if k.semantic != 0 {
                PreKind::Builtin(BuiltinOp::from_code(k.semantic.abs()).unwrap())
            } else if !rules.is_empty() {
                PreKind::Defined
            } else if builtin_of(ops[i].result) == Some(BuiltinKind::Bool)
                && k.args.is_empty()
                && (k.symbol == 0 || k.symbol == 1)
            {
                PreKind::BoolConst(k.symbol == 1)
            } else {
                PreKind::Constructor
            };
            pre.push(p);
        }

        // families: builtin sorts, and the sorts whose only constructor is
        // a transparent injection of a builtin sort
        let mut families: Vec<Family> = Vec::new();
        let mut family_of: HashMap<SortId, FamilyId> = HashMap::new();
        for sid in 0..sorts.len() {
            if let Some(kind) = builtin_of(sid) {
                family_of.insert(sid, families.len());
                families.push(Family {
                    kind,
                    base: sid,
                    members: vec![sid],
                    stuck: None,
                });
            }
        }
        for sid in 0..sorts.len() {
            if family_of.contains_key(&sid) {
                continue;
            }
            let ctors: Vec<OpId> = (0..ops.len())
                .filter(|&o| ops[o].result == sid && matches!(pre[o], PreKind::Constructor))
                .collect();
            if let [c] = ctors.as_slice() {
                let op = &ops[*c];
                if op.args.len() == 1
                    && builtin_of(op.args[0]).is_some()
                    && op.print == vec![PrintItem::Arg(0)]
                {
                    let f = family_of[&op.args[0]];
                    families[f].members.push(sid);
                    family_of.insert(sid, f);
                    pre[*c] = PreKind::Injection;
                }
            }
        }

        // rules
        let mut rules: Vec<Vec<RuleIr>> = vec![vec![]; ops.len()];
        let mut skipped: Vec<Vec<String>> = vec![vec![]; ops.len()];
        for (i, &c) in cs.iter().enumerate() {
            if !matches!(pre[i], PreKind::Defined) {
                continue;
            }
            for (r, lhs) in l.rules_of(c)? {
                let mut rl = RuleLowerer::new(&self, r);
                match rl.rule(&lhs)? {
                    Some(ir) => rules[i].push(ir),
                    None => skipped[i].push(format!(
                        "{} (extension variable in the left-hand side: not compiled, as REM)",
                        rl.render_tt(&lhs)
                    )),
                }
            }
        }

        // totality fixpoint
        let mut total: Vec<bool> = vec![true; ops.len()];
        loop {
            let sig = Signatures::new(&sorts, &ops, &pre, &total, &family_of, &families);
            let new: Vec<bool> = (0..ops.len())
                .map(|o| match &pre[o] {
                    PreKind::Defined => matching::exhaustive(
                        &rules[o]
                            .iter()
                            .filter(|r| r.unconditional())
                            .map(|r| r.lhs.clone())
                            .collect::<Vec<_>>(),
                        &ops[o].args,
                        &sig,
                    ),
                    PreKind::Builtin(b) => {
                        !b.partial()
                            && (!b.needs_values()
                                || ops[o].args.iter().all(|&a| sig.values_only(a)))
                    }
                    _ => true,
                })
                .collect();
            if new == total {
                break;
            }
            total = new;
        }

        // variants (constructors and non-total operators), decision trees
        let mut enum_variants: Vec<Vec<OpId>> = vec![vec![]; sorts.len()];
        let mut stuck_variants: Vec<Vec<OpId>> = vec![vec![]; families.len()];
        let mut trees: Vec<Option<(Decision, Vec<RuleId>)>> = vec![None; ops.len()];
        {
            let sig = Signatures::new(&sorts, &ops, &pre, &total, &family_of, &families);
            for o in 0..ops.len() {
                if sig.has_variant(o) {
                    let s = ops[o].result;
                    match family_of.get(&s) {
                        Some(&f) => stuck_variants[f].push(o),
                        None => enum_variants[s].push(o),
                    }
                }
                if let PreKind::Defined = pre[o] {
                    trees[o] = Some(matching::decision_tree(&rules[o], &ops[o].args, &sig));
                }
            }
        }
        let mut used_variant_names: Vec<HashSet<String>> =
            vec![HashSet::new(); sorts.len() + families.len()];
        for (s, vs) in enum_variants.iter().enumerate() {
            for &o in vs {
                let n = names::unique(
                    &mut used_variant_names[s],
                    names::variant_name(&ops[o].name, ops[o].symbol),
                );
                ops[o].variant = Some(n);
            }
        }
        for (f, vs) in stuck_variants.iter().enumerate() {
            for &o in vs {
                let n = names::unique(
                    &mut used_variant_names[sorts.len() + f],
                    names::variant_name(&ops[o].name, ops[o].symbol),
                );
                ops[o].variant = Some(n);
            }
        }
        for (f, fam) in families.iter_mut().enumerate() {
            if !stuck_variants[f].is_empty() {
                let base_name = &sorts[fam.base].type_name;
                fam.stuck = Some(StuckEnum {
                    type_name: names::unique(&mut used_types, format!("{base_name}Stuck")),
                    variants: stuck_variants[f].clone(),
                    alias: names::unique(&mut used_types, format!("{base_name}Term")),
                });
            }
        }
        for (s, sort) in sorts.iter_mut().enumerate() {
            sort.repr = match family_of.get(&s) {
                Some(&f) => Repr::Family(f),
                None => Repr::Enum {
                    variants: enum_variants[s].clone(),
                },
            };
        }

        // kinds and decision trees
        for o in 0..ops.len() {
            ops[o].kind = match &pre[o] {
                PreKind::Constructor => OpKind::Constructor,
                PreKind::Injection => OpKind::Injection,
                PreKind::BoolConst(b) => OpKind::BoolConst(*b),
                PreKind::Builtin(b) => OpKind::Builtin {
                    op: *b,
                    total: total[o],
                },
                PreKind::Defined => {
                    let rs = std::mem::take(&mut rules[o]);
                    let mut sk = std::mem::take(&mut skipped[o]);
                    let (tree, never) = trees[o].take().expect("tree");
                    for r in never {
                        sk.push(format!(
                            "{} (never applies: an operator of its left-hand side is always reduced)",
                            rs[r].text
                        ));
                    }
                    OpKind::Defined {
                        rules: rs,
                        skipped: sk,
                        tree,
                        total: total[o],
                    }
                }
            };
        }

        let start_sort = self.sort_of[&l.sort_of(start)];
        let start = expr_of(&self, start, &mut |_, _| unreachable!("no variable"));
        Ok(Ir {
            program: name.to_string(),
            sorts,
            families,
            ops,
            start: Start {
                query_sort: l.sort_name(q.sort),
                sort: start_sort,
                term: start,
            },
        })
    }

    fn lexem_texts(&self, ls: &[Lexem]) -> Vec<String> {
        ls.iter()
            .filter_map(|l| match l {
                Lexem::Ident(i) => Some(self.l.ident(*i)),
                Lexem::Char(c) => Some(char::from_u32(*c as u32).unwrap_or('?').to_string()),
                Lexem::Num(n) => Some(n.to_string()),
                _ => None,
            })
            .collect()
    }

    /// The printing form: the last grammar rule of the symbol (C runtime);
    /// the operator's own declaration if that rule has another arity.
    fn print_items(&self, k: &Cand) -> Vec<PrintItem> {
        let arity = |ls: &[Lexem]| ls.iter().filter(|l| matches!(l, Lexem::Type(_))).count();
        let ls = match self.l.print_form.get(&k.symbol) {
            Some(ls) if arity(ls) == k.args.len() => ls,
            _ => &k.decl,
        };
        let mut items = Vec::new();
        let mut i = 0;
        for l in ls {
            match l {
                Lexem::Type(_) => {
                    items.push(PrintItem::Arg(i));
                    i += 1;
                }
                Lexem::Char(c) => {
                    items.push(PrintItem::Char(char::from_u32(*c as u32).unwrap_or('?')))
                }
                Lexem::Num(n) => items.push(PrintItem::Num(*n as i64)),
                Lexem::Ident(id) => items.push(PrintItem::Ident(self.l.ident(*id))),
                Lexem::String(_) | Lexem::Blank => {}
            }
        }
        items
    }
}

/// The possible heads of the normal forms of each sort, during and after
/// the totality analysis (see [`crate::matching`]).
pub struct Signatures<'a> {
    ops: &'a [OpIr],
    pre: &'a [PreKind],
    total: &'a [bool],
    family_of: &'a HashMap<SortId, FamilyId>,
    families: &'a [Family],
}

impl<'a> Signatures<'a> {
    fn new(
        sorts: &'a [SortIr],
        ops: &'a [OpIr],
        pre: &'a [PreKind],
        total: &'a [bool],
        family_of: &'a HashMap<SortId, FamilyId>,
        families: &'a [Family],
    ) -> Self {
        let _ = sorts;
        Signatures {
            ops,
            pre,
            total,
            family_of,
            families,
        }
    }

    /// Does a normal form with `o` at the top exist (a constructor, or an
    /// operator that can be stuck)? Injections and booleans are values.
    pub fn has_variant(&self, o: OpId) -> bool {
        match self.pre[o] {
            PreKind::Constructor => true,
            PreKind::Injection | PreKind::BoolConst(_) => false,
            PreKind::Builtin(_) | PreKind::Defined => !self.total[o],
        }
    }

    /// Can a head `o` be at the top of a normal form?
    pub fn possible(&self, o: OpId) -> bool {
        match self.pre[o] {
            PreKind::Injection | PreKind::BoolConst(_) => true,
            _ => self.has_variant(o),
        }
    }

    /// The heads of the normal forms of sort `s`, and whether this list is
    /// complete (false for the literals of builtinInt and builtinString).
    pub fn heads(&self, s: SortId) -> (Vec<OpId>, bool) {
        let mut v: Vec<OpId> = (0..self.ops.len())
            .filter(|&o| self.ops[o].result == s && self.possible(o))
            .collect();
        v.sort_unstable();
        let finite = match self.family_of.get(&s) {
            Some(&f) => {
                let fam = &self.families[f];
                if fam.base == s {
                    // literals: infinitely many values; bool: true, false
                    fam.kind == BuiltinKind::Bool
                } else {
                    true
                }
            }
            None => true,
        };
        (v, finite)
    }

    /// The argument sorts of an operator.
    pub fn args(&self, o: OpId) -> &[SortId] {
        &self.ops[o].args
    }

    /// Are the normal forms of sort `s` values only (no stuck term)?
    pub fn values_only(&self, s: SortId) -> bool {
        let no_variant =
            (0..self.ops.len()).all(|o| self.ops[o].result != s || !self.has_variant(o));
        match self.family_of.get(&s) {
            Some(&f) if self.families[f].base != s => {
                no_variant && self.values_only(self.families[f].base)
            }
            _ => no_variant,
        }
    }
}

/// Lowering of one rule: variables, non-linearity, conditions.
struct RuleLowerer<'b, 'a, 'p> {
    b: &'b Builder<'a, 'p>,
    rule: &'p Rule,
    vars: Vec<VarIr>,
    /// model variable index -> IR variable (first occurrence)
    var_of: HashMap<u32, VarId>,
    conds: Vec<Cond>,
}

impl<'b, 'a, 'p> RuleLowerer<'b, 'a, 'p> {
    fn new(b: &'b Builder<'a, 'p>, rule: &'p Rule) -> Self {
        RuleLowerer {
            b,
            rule,
            vars: vec![],
            var_of: HashMap::new(),
            conds: vec![],
        }
    }

    /// A new variable for model variable `i` (named `x<i>` as in the rule
    /// comments, `x<i>_<n>` for its other occurrences).
    fn new_var(&mut self, i: u32, sort: SortIndex) -> VarId {
        let id = self.vars.len();
        let mut name = format!("x{i}");
        let mut n = 1;
        while self.vars.iter().any(|v| v.name == name) {
            n += 1;
            name = format!("x{i}_{n}");
        }
        self.vars.push(VarIr {
            name,
            sort: self.b.sort_of[&sort],
        });
        id
    }

    /// A pattern; a variable already bound gives a fresh variable and an
    /// equality condition (`same`).
    fn pattern(&mut self, t: &TT, same: &mut Vec<Cond>) -> Pattern {
        match t {
            TT::Var(i, s) => match self.var_of.get(i) {
                None => {
                    let v = self.new_var(*i, *s);
                    self.var_of.insert(*i, v);
                    Pattern::Var(v)
                }
                Some(&first) => {
                    let v = self.new_var(*i, *s);
                    same.push(Cond::Same(first, v));
                    Pattern::Var(v)
                }
            },
            TT::Op(c, args) => Pattern::Op {
                op: self.b.op_of[c],
                args: args.iter().map(|a| self.pattern(a, same)).collect(),
            },
            TT::Int(n) => Pattern::Int(*n),
            TT::Str(s) => Pattern::Str(s.clone()),
            TT::EVar => unreachable!("rules with extension variables are skipped"),
        }
    }

    fn expr(&mut self, t: &TT) -> Result<Expr, Error> {
        let mut unbound = None;
        let e = expr_of(self.b, t, &mut |i, s| match self.var_of.get(&i) {
            Some(&v) => v,
            None => {
                unbound = Some((i, s));
                0
            }
        });
        match unbound {
            None => Ok(e),
            Some((i, _)) => Err(Error::Invalid(format!(
                "variable {i} used before being bound in a rule of sort {}",
                self.b.l.sort_name(self.rule.sort)
            ))),
        }
    }

    /// The rule, or `None` when it is not compiled (extension variables).
    fn rule(&mut self, lhs: &TT) -> Result<Option<RuleIr>, Error> {
        if contains_evar(lhs) {
            return Ok(None);
        }
        let l = self.b.l;
        let TT::Op(_, args) = lhs else { unreachable!() };
        let mut same = vec![];
        let pats: Vec<Pattern> = args.iter().map(|a| self.pattern(a, &mut same)).collect();
        self.conds.append(&mut same);
        let RuleBody::Plain { rhs, wheres } = &self.rule.body else {
            unreachable!("checked")
        };
        let mut text_conds = Vec::new();
        for w in wheres {
            match w {
                Where::If(t) => {
                    let t = l.type_top(t, l.bool_sort, "condition")?;
                    text_conds.push(format!("if {}", self.render_tt(&t)));
                    let e = self.expr(&t)?;
                    self.conds.push(Cond::If(e));
                }
                Where::Assign { lhs, rhs, .. } => {
                    let r = l.type_top(rhs, var_sort(lhs), "where")?;
                    let lt = l.type_top(lhs, Some(l.sort_of(&r)), "where")?;
                    let e = self.expr(&r)?;
                    text_conds.push(format!(
                        "where {} := () {}",
                        self.render_tt(&lt),
                        self.render_tt(&r)
                    ));
                    match lt {
                        TT::Var(i, s) if !self.var_of.contains_key(&i) => {
                            let v = self.new_var(i, s);
                            self.var_of.insert(i, v);
                            self.conds.push(Cond::Let(v, e));
                        }
                        _ => {
                            let mut same = vec![];
                            let p = self.pattern(&lt, &mut same);
                            self.conds.push(Cond::Match(p, e));
                            self.conds.append(&mut same);
                        }
                    }
                }
                Where::Match {
                    pattern, sort, rhs, ..
                } => {
                    let r = l.type_top(rhs, Some(*sort), "where")?;
                    let pt = l.type_top(pattern, Some(*sort), "where")?;
                    let e = self.expr(&r)?;
                    text_conds.push(format!(
                        "where {} := () {}",
                        self.render_tt(&pt),
                        self.render_tt(&r)
                    ));
                    let mut same = vec![];
                    let p = self.pattern(&pt, &mut same);
                    self.conds.push(Cond::Match(p, e));
                    self.conds.append(&mut same);
                }
                Where::Try(_) => unreachable!("refused during reachability"),
            }
        }
        let rt = l.type_top(rhs, Some(self.rule.sort), "right-hand side")?;
        let rhs_e = self.expr(&rt)?;
        let mut text = format!("[] {} => {}", self.render_tt(lhs), self.render_tt(&rt));
        for c in text_conds {
            text.push(' ');
            text.push_str(&c);
        }
        Ok(Some(RuleIr {
            text,
            vars: std::mem::take(&mut self.vars),
            lhs: pats,
            conds: std::mem::take(&mut self.conds),
            rhs: rhs_e,
        }))
    }

    /// ELAN text of a typed term (variables `x<model index>`).
    fn render_tt(&self, t: &TT) -> String {
        let mut out = String::new();
        render(self.b, t, &mut out);
        out
    }
}

fn render(b: &Builder, t: &TT, out: &mut String) {
    let alnum_end = |s: &str| s.ends_with(|c: char| c.is_ascii_alphanumeric());
    match t {
        TT::Var(i, _) => {
            if alnum_end(out) {
                out.push(' ');
            }
            out.push_str(&format!("x{i}"));
        }
        TT::EVar => out.push_str("<evar>"),
        TT::Int(n) => {
            if alnum_end(out) {
                out.push(' ');
            }
            out.push_str(&n.to_string())
        }
        TT::Str(s) => out.push_str(&string_text(s)),
        TT::Op(c, args) => {
            // the declaration's syntax (the printing form may be an alias)
            let mut i = 0;
            for l in &b.l.cands[*c].decl {
                match l {
                    Lexem::Type(_) => {
                        render(b, &args[i], out);
                        i += 1;
                    }
                    Lexem::Ident(id) => {
                        let s = b.l.ident(*id);
                        if alnum_end(out) && s.starts_with(|c: char| c.is_ascii_alphanumeric()) {
                            out.push(' ');
                        }
                        out.push_str(&s)
                    }
                    Lexem::Char(ch) => {
                        let ch = char::from_u32(*ch as u32).unwrap_or('?');
                        if matches!(
                            ch,
                            '+' | '-' | '*' | '/' | '=' | '<' | '>' | '&' | '|' | '%'
                        ) {
                            out.push(' ');
                            out.push(ch);
                            out.push(' ');
                        } else {
                            out.push(ch)
                        }
                    }
                    Lexem::Num(n) => out.push_str(&n.to_string()),
                    Lexem::String(_) | Lexem::Blank => {}
                }
            }
        }
    }
}

/// An expression from a typed term; `var` maps model variables.
fn expr_of(b: &Builder, t: &TT, var: &mut dyn FnMut(u32, SortIndex) -> VarId) -> Expr {
    match t {
        TT::Var(i, s) => Expr::Var(var(*i, *s)),
        TT::Op(c, args) => Expr::Op {
            op: b.op_of[c],
            args: args.iter().map(|a| expr_of(b, a, var)).collect(),
        },
        TT::Int(n) => Expr::Int(*n),
        TT::Str(s) => Expr::Str(s.clone()),
        TT::EVar => unreachable!("extension variable in an expression"),
    }
}
