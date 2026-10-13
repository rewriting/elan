//! The intermediate representation between the `.ref` model
//! ([`elan_ref::Program`]) and the emitted Rust code (spec S6, §3).
//!
//! The IR is the contract between the analysis ([`crate::lower`]) and the
//! back ends ([`crate::emit`] for Rust). It is independent of the target
//! language: names are given as plain identifiers, types as sort ids and
//! representations, code as typed trees.
//!
//! # What it contains (S6a)
//!
//! * The **sorts** reachable from the start term, each with a
//!   [`Repr`]: a hash-consed enum (one variant per constructor and per
//!   defined operator that can be stuck), or a member of a builtin
//!   [`Family`] (`builtinInt` and the sorts that only inject it, such as
//!   `int`; `bool`; `builtinString` and `string`), represented by a native
//!   value, wrapped in the runtime's `Builtin<V, S>` when a term of the
//!   family can be stuck.
//! * The reachable **operators** ([`OpIr`]) with their profile, printing
//!   form and [`OpKind`]: free constructor, transparent injection, boolean
//!   constant, builtin operation, or **defined** by unlabelled rules
//!   (normalisation functions: innermost, rules in source order) with a
//!   matching automaton ([`Decision`]).
//! * The **start term** of the query.
//!
//! # Strategies (S6b)
//!
//! * The **labels** ([`LabelIr`]): the labelled rules of one localised rule
//!   name (`label:sort/module!GL`), in source order, with the decision tree
//!   of their whole left-hand sides (one column: the term itself). A label
//!   is compiled to a function in continuation-passing style over its
//!   sort, `fn(t, k, first) -> bool`: each result of each rule that
//!   applies, in order, is passed to `k` (`true` from `k` means stop);
//!   with `first`, only the results of the first rule that has one (`dc`).
//!   Labelled rules never take part in normalisation; the right-hand side
//!   is built by the normalisation functions, so every result is in normal
//!   form (the interpreter reduces each result).
//! * The **strategies** ([`StrategyIr`]): the `STRATEGY` definitions
//!   reachable from the query and from the `where`s, a body made of
//!   [`Strategy`] elements, each compiled to `fn(t, k) -> bool`.
//! * [`Cond::Apply`]: `where p := (S) t`, each result of `S` in turn (the
//!   conditions after it backtrack into `S`); in normalisation, the first
//!   result for which the rest of the conditions hold.
//!
//! # Strategy terms (S6b task 2)
//!
//! Strategies built at run time (`[S]t`, `where x := [S]t`, defined
//! strategies with parameters `[] app(s) => s`, `if b then S1 else S2 fi`,
//! `if S then S1 orelse S2 fi`, the labels as strategy constants) are
//! terms of the strategy sorts `<X>` (`<X->X>`) and `StrList[X,X]` of
//! `strsig[X,Y]`/`strat[X]` (`elanlib/strategy`). The interpreter has no
//! builtin evaluation of them: the codes `-180` (`[@]@`) .. `-190`, `-205`
//! .. `-208` of their operators (`DS_*`, `term/codes.h`) are never used by
//! its rewriting (`standardreduction`, `rewrite/rtmisc.cc`, has no case for
//! them; the `defstrat` word of the grammar rules, `FSYM_FLAG`, `LAB_FLAG`,
//! `DSTR_FLAG`, `APPLY_FLAG`, is only written to the `.ref` file,
//! `ref/ref.cc`, for REM). Their semantics is the one of ordinary ELAN
//! code, which the `.ref` file contains in full:
//! * the strategy `eval:X/strat[X]` (`first(dk(FSYM), dk(DSTR), dk(DK),
//!   ..., dc(dk(IFO1), dk(IFO2)) ; dk(IFO3))`) and `eval_DK`, `eval_DC`,
//!   `eval_ONE`, which a `where x := [S]t` calls (the interpreter exports it
//!   as `where x := (eval) [S]t`);
//! * the labelled rules of `strat[X]` (`[DK] [dk(SS)]a => b where b :=
//!   (eval_DK)((SS))a`, `[IF1]`, `[IFO1]`, `[CONST]`, ...) and the rules
//!   the interpreter generates when loading (`load/msemact.cc`,
//!   `msemact3.cc`): `[LAB] [l]x => y` for every label `l` used as a
//!   strategy constant (`[LAB] [inc1]x => x+1`), `[DSTR]` for every
//!   defined strategy (`[DSTR] [app(s)]x => y where y := (eval)[s]x`),
//!   `[FSYM]` congruences (`[s1 + s2](a + b) => ...`).
//!
//! So the strategy sorts are ordinary sorts here (D2: an enum per sort, a
//! variant per strategy constructor: `dk`, `dc`, `one`, `id`, `;`, the
//! labels, the defined strategies, `[@]@`...), and `eval` is an ordinary
//! named strategy compiled with the combinators of task 1: the results and
//! their order are the interpreter's by construction. The `[@]@`
//! application is a constructor of the sort `X` (the interpreter leaves
//! `[S]t` unevaluated in normal forms, only `eval` applies it). Labelled
//! rules whose left-hand side has an operator that no term of the program
//! contains are not compiled (`lower::Lowerer::lhs_possible`): e.g. the
//! `[TONE]` rule and its traversal `eval_TONE` when no `tone(S)` is built.
//!
//! `call "name":X` (`Meta_strat[X]`) is the one construct with a builtin
//! evaluation: the `META` strategy of `set_of`/`setof` (`Meta_apply[X]`,
//! `strapply[X]`) interprets a `meta_apply` term ([`MetaOp`]), whose
//! first argument, a term of sort `Strategy[X]`, denotes a strategy
//! (`term2strategy`, `meta/meta.cc`). Its sorts are ordinary sorts too;
//! `META` and the evaluator of the `Strategy[X]` terms ([`MetaIr`]) are
//! generated. The strategy of a call is chosen as the interpreter does it
//! when it evaluates the term (`Strategyname_to_ref_index`, commit
//! 93ec493): by name and by the sort of the term, known at compile time
//! (X), not with the index that the export wrote (any sort, the 2004
//! behaviour that REM keeps).
//!
//! # Normalisation scheme
//!
//! Every operator `f` is compiled to a function taking its arguments in
//! normal form and returning the normal form of `f(args)`: the decision
//! tree selects the rules whose left-hand side matches, in source order;
//! the first one whose conditions hold is applied (its right-hand side is
//! built bottom-up by calling the functions of its operators, so it is in
//! normal form); when none applies, the term `f(args)` itself is the
//! result (a *stuck* term), which needs a variant ([`OpIr::variant`]).
//! An operator is **total** when its unconditional rules cover every
//! normal form of its arguments (computed by [`crate::lower`] as a fixpoint
//! over the sorts); a total operator has no variant.
//!
//! # Extension points
//!
//! * S6c (AC): [`Repr::Enum`] variants get an AC multiset field, [`Pattern`]
//!   an AC case, [`Decision`] an AC matching node.
//! * S6d (query): [`Start`] gets the query variable and the parser tables.

use elan_ref::{SortIndex, SymbolCode};

/// Index in [`Ir::sorts`].
pub type SortId = usize;
/// Index in [`Ir::ops`].
pub type OpId = usize;
/// Index in [`Ir::families`].
pub type FamilyId = usize;
/// Index of a rule in [`OpKind::Defined::rules`].
pub type RuleId = usize;
/// Index in [`RuleIr::vars`].
pub type VarId = usize;
/// Index in [`Ir::labels`].
pub type LabelId = usize;
/// Index in [`Ir::strategies`].
pub type StratId = usize;

/// A whole program, restricted to what the start term needs.
#[derive(Clone, Debug)]
pub struct Ir {
    /// Name of the program (the `.lgi` file without extension).
    pub program: String,
    pub sorts: Vec<SortIr>,
    pub families: Vec<Family>,
    pub ops: Vec<OpIr>,
    pub labels: Vec<LabelIr>,
    pub strategies: Vec<StrategyIr>,
    /// The evaluators of the `Strategy[X]` terms of `META` (one per X).
    pub metas: Vec<MetaIr>,
    pub start: Start,
    /// The table of identifiers of the `.ref` file (index, name) when the
    /// sort `ident` is reached (`elan_runtime::init_idents`), else empty.
    pub idents: Vec<(i32, String)>,
}

/// The evaluation of the terms of sort `Strategy[X]` (`Meta_strat[X]`) by
/// the `META` strategy: `term2strategy` of `meta/meta.cc`, compiled to
/// `fn(s, t, typed, k) -> bool` applying the strategy denoted by `s` to the
/// term `t` of sort X: a sequence (`@` 141, `@ @` 142) of `id` (143),
/// `call "name":X` (144), `dc`/`dk` of strategies (137, 138 over 131,
/// 132), `repeat` (139) and `iterate` (140). `typed`: the sort of the term
/// is known (false for a number or a string, which the interpreter cannot
/// type), it chooses the strategy of a call.
#[derive(Clone, Debug)]
pub struct MetaIr {
    /// Name of the function.
    pub fun: String,
    /// `Strategy[X]`.
    pub strategy: SortId,
    /// `Strateg[X]`, `Strategies[X]` when reachable.
    pub strateg: Option<SortId>,
    pub strategies: Option<SortId>,
    /// X.
    pub term: SortId,
    /// The `call "name":X` terms of the program.
    pub calls: Vec<MetaCall>,
}

/// A `call "name":X` and the strategies it applies.
#[derive(Clone, Debug)]
pub struct MetaCall {
    pub name: Vec<u8>,
    /// For a term whose sort is known (X).
    pub typed: StratId,
    /// For a term whose sort is unknown (X builtinInt or builtinString, a
    /// number or a string): the strategy, or why it is refused (a strategy
    /// of another sort; the program stops with status 2 if it happens).
    pub untyped: Option<Result<StratId, String>>,
}

/// An operator evaluated by the `META` strategy (`strbuiltins`,
/// `rewrite/stateofexecution.cc`); elsewhere it is a constructor.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum MetaOp {
    /// `meta_apply(s, t.l, from, n)` (`NEW_META_APPLY`, code 128,
    /// `set_of`): one result, the list of the results of `s` on `t`, from
    /// number `from` (0 first), `n` of them (all when `n` is 0).
    List,
    /// `meta_apply(s, t)` (`META_APPL`, 129): every result of `s` on `t`.
    All,
    /// `meta_apply(s, t, n)` (`META_APPLY`, 130): result number `n` of `s`
    /// on `t` (0 first), if any.
    Nth,
}

/// The labelled rules of one label (a localised rule name of the `.ref`
/// file), in source order.
#[derive(Clone, Debug)]
pub struct LabelIr {
    /// The localised name (`r1:nat/m!GL`).
    pub name: String,
    /// Name of the function.
    pub fun: String,
    /// Sort of the rules.
    pub sort: SortId,
    /// The rules; each has one left-hand side pattern, the whole term.
    pub rules: Vec<RuleIr>,
    /// Rules of the source not compiled, with the reason (comments).
    pub skipped: Vec<String>,
    /// The matching automaton (one column: the term).
    pub tree: Decision,
    /// Every rule gives at most one result (its `where`s use
    /// deterministic strategies only).
    pub rules_det: bool,
}

/// A named strategy (`STRATEGY` definition).
#[derive(Clone, Debug)]
pub struct StrategyIr {
    /// Name in the `.ref` file (`go:nat/m`).
    pub name: String,
    /// Name of the function.
    pub fun: String,
    /// Sort of the terms it applies to (and of its results).
    pub sort: SortId,
    /// The sequence `s1 ; s2 ; ...`.
    pub body: Vec<Strategy>,
    /// It gives at most one result (see [`Ir::seq_det`]).
    pub det: bool,
}

/// How a choice keeps the results of its alternatives.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Choice {
    /// `dk`, `DK`: all results of all alternatives, in order.
    Dk,
    /// `dc`, `DC` (`first`): all results of the first alternative that has
    /// one (for rules: the first **rule** that has one).
    Dc,
    /// `one`, `ONE` (`first one`, `dc one`): the first result.
    One,
}

/// An elementary strategy (one element of a sequence).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Strategy {
    Id,
    Fail,
    /// `dk/dc/one(l1, ..., ln)` over labels.
    Rules {
        choice: Choice,
        labels: Vec<LabelId>,
    },
    /// `DK/DC/ONE(s1, ..., sn)` over sequences.
    Strats {
        choice: Choice,
        alts: Vec<Vec<Strategy>>,
    },
    /// `repeat*(s)`: the terms reached by `s` on which `s` fails, depth
    /// first.
    Repeat(Vec<Strategy>),
    /// `iterate*(s)`: the term, then `iterate*(s)` of each result of `s`.
    Iterate(Vec<Strategy>),
    /// A named strategy.
    Call(StratId),
    /// `META` (the strategy of `where x := (META) t`): `t` is a
    /// `meta_apply` term ([`MetaOp`]); its results.
    Meta,
}

/// A sort.
#[derive(Clone, Debug)]
pub struct SortIr {
    /// Index in the `Sorts` table of the `.ref` file.
    pub index: SortIndex,
    /// ELAN name (`nat`, `list[int]`, `builtinInt`).
    pub name: String,
    /// Name of the enum for [`Repr::Enum`] (a valid identifier).
    pub type_name: String,
    pub repr: Repr,
}

/// How the terms of a sort are represented.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Repr {
    /// A hash-consed enum (`H<type_name>`) with these variants, in order:
    /// the constructors of the sort and its defined operators that can be
    /// stuck.
    Enum { variants: Vec<OpId> },
    /// A member of a builtin family: the family's native type, or
    /// `Builtin<V, H<stuck enum>>` when the family can be stuck.
    Family(FamilyId),
}

/// The builtin value types.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub enum BuiltinKind {
    /// `builtinInt`: an `Int` (`i64` holding a 32-bit value).
    Int,
    /// `bool`: a `bool`.
    Bool,
    /// `builtinString`: a `Str` (`Rc<[u8]>`: bytes, as C strings).
    Str,
    /// `ident`: an `Ident`, the index of the identifier in the table of
    /// identifiers of the program (`elan_runtime::ident`).
    Ident,
}

/// A builtin sort and the sorts whose only constructor is an injection of
/// it (`@ : (builtinInt) int`): they share one representation, the
/// injection being the identity.
#[derive(Clone, Debug)]
pub struct Family {
    pub kind: BuiltinKind,
    /// The builtin sort.
    pub base: SortId,
    /// All members, the base first.
    pub members: Vec<SortId>,
    /// `Some` when a term of a member can be stuck: the enum of the stuck
    /// terms of all members (a variant per operator that can be stuck),
    /// and the values are `Builtin<V, H<enum>>`.
    pub stuck: Option<StuckEnum>,
}

/// The enum of the stuck terms of a family.
#[derive(Clone, Debug)]
pub struct StuckEnum {
    pub type_name: String,
    pub variants: Vec<OpId>,
    /// Name of the type alias of the family's values,
    /// `Builtin<V, H<type_name>>`.
    pub alias: String,
}

/// An operator (a function symbol with one profile; the builtin codes
/// below 300 are overloaded, e.g. `==` on `builtinInt` and on `bool`).
#[derive(Clone, Debug)]
pub struct OpIr {
    pub symbol: SymbolCode,
    /// Readable base name (from the mixfix form).
    pub name: String,
    /// Name of the normalisation function.
    pub fun: String,
    /// Variant name in the enum of the result sort (or in the stuck enum of
    /// its family); `None` when no normal form has this operator at the
    /// top (a total defined operator, an injection, a boolean constant).
    pub variant: Option<String>,
    pub args: Vec<SortId>,
    pub result: SortId,
    /// How the compiled programs print a term with this operator at the
    /// top (the last grammar rule of the symbol, as `findTextForm` in
    /// `src/compiler/runtime/termOut.c`).
    pub print: Vec<PrintItem>,
    /// ELAN declaration, for comments: `plus(@,@) : (nat nat) nat`.
    pub decl: String,
    pub kind: OpKind,
    /// A `meta_apply` operator, which the `META` strategy evaluates (a
    /// constructor otherwise).
    pub meta: Option<MetaOp>,
}

/// A token of a printing form.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum PrintItem {
    /// The i-th argument.
    Arg(usize),
    /// An identifier token.
    Ident(String),
    /// A one-character token.
    Char(char),
    /// A number token.
    Num(i64),
}

/// What an operator does.
#[derive(Clone, Debug)]
pub enum OpKind {
    /// A free constructor (no rule): builds the term.
    Constructor,
    /// The injection of a family member's builtin sort (`@ : (builtinInt)
    /// int`): the identity.
    Injection,
    /// `true` / `false` (symbols 1 and 0).
    BoolConst(bool),
    /// A builtin operation (the `code n` of a declaration).
    Builtin { op: BuiltinOp, total: bool },
    /// An operator with unlabelled rules.
    Defined {
        rules: Vec<RuleIr>,
        /// Rules of the source not compiled, with the reason (comments).
        skipped: Vec<String>,
        tree: Decision,
        total: bool,
    },
}

impl OpKind {
    /// Can a term with this operator at the top be a normal form?
    pub fn total(&self) -> bool {
        match self {
            OpKind::Builtin { total, .. } | OpKind::Defined { total, .. } => *total,
            _ => true,
        }
    }
}

/// The builtin operations supported (codes of `src/interpreter/term/codes.h`,
/// semantics of `rewrite/rtmisc.cc`).
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub enum BuiltinOp {
    /// `+ - * / % & |` on builtinInt (codes 3, 4, 5, 6, 27, 28, 29).
    IntAdd,
    IntSub,
    IntMul,
    IntDiv,
    IntRem,
    IntAnd,
    IntOr,
    /// unary `-` (20).
    IntNeg,
    /// `== != < <= > >=` on builtinInt or bool values (8..13).
    Cmp(CmpOp),
    /// `and`, `or`, `not` (21, 22, 24): a stuck argument counts as false.
    BoolAnd,
    BoolOr,
    BoolNot,
    /// `==`, `!=` on any sort (18, 19): syntactic equality of normal forms.
    TermEq,
    TermNeq,
    /// string primitives (150..158 but 155).
    StrLen,
    StrCat,
    StrIndex,
    StrModif,
    StrSubstr,
    StrSpn,
    StrCmp,
    StrOfChar,
    /// `==`, `!=` on ident (14, 15).
    IdentEq,
    IdentNe,
    /// `ident2string` (177): the name of the identifier.
    IdentToString,
    /// `replace t1 by t2 in t3` (16) and `occurs t1 in t2` (17) on sorts
    /// of terms (`replace[X,Y]`, `occur[X,Y]`): REM's term_rec_replace and
    /// term_occur, the copied path of a replacement renormalised.
    TermReplace,
    TermOccurs,
}

/// A comparison.
#[derive(Clone, Copy, Debug, PartialEq, Eq, Hash)]
pub enum CmpOp {
    Eq,
    Ne,
    Lt,
    Le,
    Gt,
    Ge,
}

impl BuiltinOp {
    /// The operation from a semantic code, `None` when not supported.
    pub fn from_code(code: i32) -> Option<BuiltinOp> {
        use BuiltinOp::*;
        Some(match code {
            3 => IntAdd,
            4 => IntSub,
            5 => IntMul,
            6 => IntDiv,
            27 => IntRem,
            28 => IntAnd,
            29 => IntOr,
            20 => IntNeg,
            8 => Cmp(CmpOp::Eq),
            9 => Cmp(CmpOp::Ne),
            10 => Cmp(CmpOp::Lt),
            11 => Cmp(CmpOp::Le),
            12 => Cmp(CmpOp::Gt),
            13 => Cmp(CmpOp::Ge),
            21 => BoolAnd,
            22 => BoolOr,
            24 => BoolNot,
            18 => TermEq,
            19 => TermNeq,
            150 => StrLen,
            151 => StrCat,
            152 => StrIndex,
            153 => StrModif,
            154 => StrSubstr,
            156 => StrSpn,
            157 => StrCmp,
            158 => StrOfChar,
            14 => IdentEq,
            15 => IdentNe,
            177 => IdentToString,
            16 => TermReplace,
            17 => TermOccurs,
            _ => return None,
        })
    }

    /// Does the operation need values (not stuck terms) as arguments?
    /// `and`/`or`/`not` and the operations on terms accept any normal
    /// form.
    pub fn needs_values(self) -> bool {
        !matches!(
            self,
            BuiltinOp::BoolAnd
                | BuiltinOp::BoolOr
                | BuiltinOp::BoolNot
                | BuiltinOp::TermEq
                | BuiltinOp::TermNeq
                | BuiltinOp::TermReplace
                | BuiltinOp::TermOccurs
        )
    }

    /// Is the operation undefined on some values (the term stays)?
    pub fn partial(self) -> bool {
        matches!(
            self,
            BuiltinOp::StrIndex | BuiltinOp::StrModif | BuiltinOp::StrSubstr
        )
    }
}

/// An unlabelled rule of a defined operator, ready for compilation:
/// variables numbered, non-linear occurrences turned into conditions.
#[derive(Clone, Debug)]
pub struct RuleIr {
    /// The rule in ELAN syntax (variables named `x0`, `x1`, ...), for
    /// comments.
    pub text: String,
    pub vars: Vec<VarIr>,
    /// The patterns of the arguments of the left-hand side.
    pub lhs: Vec<Pattern>,
    /// Conditions and local assignments, in order (the non-linearity
    /// checks of the left-hand side first).
    pub conds: Vec<Cond>,
    pub rhs: Expr,
}

impl RuleIr {
    /// A rule without conditions applies whenever its left-hand side
    /// matches (a local assignment `where x := () t` always succeeds).
    pub fn unconditional(&self) -> bool {
        self.conds.iter().all(|c| matches!(c, Cond::Let(..)))
    }
}

/// A rule variable.
#[derive(Clone, Debug)]
pub struct VarIr {
    pub name: String,
    pub sort: SortId,
}

/// A pattern of a left-hand side or of a `where`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Pattern {
    /// A variable (each variable occurs once in the patterns of a rule;
    /// repeated occurrences are [`Cond::Same`]).
    Var(VarId),
    /// An operator applied to patterns.
    Op { op: OpId, args: Vec<Pattern> },
    /// A builtinInt literal (already wrapped to 32 bits).
    Int(i64),
    /// A builtinString literal.
    Str(Vec<u8>),
    /// An identifier literal (its index).
    Ident(i32),
}

/// A condition or local assignment of a rule.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Cond {
    /// Non-linear left-hand side: the two variables are equal (pointer
    /// equality of shared terms, value equality of builtins).
    Same(VarId, VarId),
    /// `if t`: `t` normalises to `true`.
    If(Expr),
    /// `where x := () t`.
    Let(VarId, Expr),
    /// `where p := () t`: the normal form of `t` matches `p`.
    Match(Pattern, Expr),
    /// `where p := (S) t` (`p` a variable for `where x := (S) t`): each
    /// result of strategy `S` applied to the normal form of `t` that
    /// matches `p`, in order (backtracking).
    Apply {
        pattern: Pattern,
        strategy: StratId,
        expr: Expr,
    },
}

/// A term to build (right-hand side, condition, start term). Building an
/// operator calls its normalisation function on the arguments' normal
/// forms (innermost).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Expr {
    Var(VarId),
    Op { op: OpId, args: Vec<Expr> },
    Int(i64),
    Str(Vec<u8>),
    /// An identifier literal (its index).
    Ident(i32),
}

/// A position under the arguments of the function: the argument number,
/// then the argument numbers of the subterms (an injection has one child,
/// its argument, which is the same value).
pub type Occ = Vec<usize>;

/// The matching automaton of a defined operator (a decision tree,
/// Maranget-style: a node tests the head of one position; each case keeps
/// the rules compatible with its head in source order, so the first rule
/// that matches and whose conditions hold is the first in source order).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Decision {
    /// No rule applies: the term is stuck.
    Fail,
    /// The left-hand side of `rule` matches (its variables are at these
    /// positions): check its conditions; apply it, or continue with `next`.
    Try {
        rule: RuleId,
        bindings: Vec<(VarId, Occ)>,
        next: Box<Decision>,
    },
    /// Test the head of the term at `occ` (of sort `sort`).
    Switch {
        occ: Occ,
        sort: SortId,
        cases: Vec<(Head, Decision)>,
        /// For the heads not listed; `None` when the cases cover every
        /// possible head of the sort.
        default: Option<Box<Decision>>,
    },
}

/// A head tested by a [`Decision::Switch`].
#[derive(Clone, Debug, PartialEq, Eq, Hash)]
pub enum Head {
    /// An operator (a variant, an injection, `true`/`false`); its children
    /// are at `occ ++ [i]`.
    Op(OpId),
    Int(i64),
    Str(Vec<u8>),
    Ident(i32),
}

/// The query's start term (`start with () t`, run by `-noInput`).
#[derive(Clone, Debug)]
pub struct Start {
    /// Sort of the query (printed by the prompt).
    pub query_sort: String,
    pub sort: SortId,
    pub term: Expr,
    /// `start with (S) t`: the strategy applied to the normal form of the
    /// term, each result printed; `None` for `()`.
    pub strategy: Option<StratId>,
}

impl Ir {
    /// Does the sequence give at most one result? (`one`, `ONE`, `id`,
    /// `fail`, `dc`/`DC` of deterministic alternatives, a choice of one
    /// alternative that is, `repeat*` of a deterministic strategy, a call
    /// of a deterministic strategy, sequences of these.)
    pub fn seq_det(&self, seq: &[Strategy]) -> bool {
        seq.iter().all(|s| self.det(s))
    }

    /// See [`Ir::seq_det`].
    pub fn det(&self, s: &Strategy) -> bool {
        strategy_det(
            s,
            &|l| self.labels[l].rules_det,
            &|l| self.labels[l].rules.len(),
            &|c| self.strategies[c].det,
        )
    }

    pub fn family_of(&self, s: SortId) -> Option<&Family> {
        match self.sorts[s].repr {
            Repr::Family(f) => Some(&self.families[f]),
            Repr::Enum { .. } => None,
        }
    }
}

impl Ir {
    /// The defined operators whose normalisation can call itself again
    /// through other operators (a cycle of the graph "the rules of `f` build
    /// a term with `g`"): their functions check the depth of the stack
    /// (`elan_runtime::guard`, spec D3). The others are not checked: their
    /// frames are bounded between two checked calls, and a check in a small
    /// hot function (`x + y` of the int module) costs (nqueens 12: 0.40 s
    /// to 0.67 s). Recursion through a strategy (`where y := (S) t`) is
    /// checked by the strategy functions.
    pub fn recursive_ops(&self) -> Vec<bool> {
        let n = self.ops.len();
        let mut succ: Vec<Vec<OpId>> = vec![Vec::new(); n];
        fn ops_of(e: &Expr, out: &mut Vec<OpId>) {
            if let Expr::Op { op, args } = e {
                out.push(*op);
                for a in args {
                    ops_of(a, out);
                }
            }
        }
        for (o, op) in self.ops.iter().enumerate() {
            if let OpKind::Defined { rules, .. } = &op.kind {
                for r in rules {
                    ops_of(&r.rhs, &mut succ[o]);
                    for c in &r.conds {
                        match c {
                            Cond::Same(..) => {}
                            Cond::If(e) | Cond::Let(_, e) | Cond::Match(_, e) => {
                                ops_of(e, &mut succ[o])
                            }
                            Cond::Apply { expr, .. } => ops_of(expr, &mut succ[o]),
                        }
                    }
                }
                succ[o].sort_unstable();
                succ[o].dedup();
            }
        }
        // strongly connected components (Tarjan, iterative)
        let mut index = vec![usize::MAX; n];
        let mut low = vec![0usize; n];
        let mut on_stack = vec![false; n];
        let mut stack: Vec<OpId> = Vec::new();
        let mut rec = vec![false; n];
        let mut next = 0usize;
        for root in 0..n {
            if index[root] != usize::MAX {
                continue;
            }
            // (node, position in its successors)
            let mut work: Vec<(OpId, usize)> = vec![(root, 0)];
            index[root] = next;
            low[root] = next;
            next += 1;
            stack.push(root);
            on_stack[root] = true;
            while let Some(&(v, i)) = work.last() {
                if i < succ[v].len() {
                    let w = succ[v][i];
                    work.last_mut().unwrap().1 += 1;
                    if index[w] == usize::MAX {
                        index[w] = next;
                        low[w] = next;
                        next += 1;
                        stack.push(w);
                        on_stack[w] = true;
                        work.push((w, 0));
                    } else if on_stack[w] {
                        low[v] = low[v].min(index[w]);
                    }
                    continue;
                }
                work.pop();
                if let Some(&(u, _)) = work.last() {
                    low[u] = low[u].min(low[v]);
                }
                if low[v] == index[v] {
                    let mut comp = Vec::new();
                    loop {
                        let w = stack.pop().unwrap();
                        on_stack[w] = false;
                        comp.push(w);
                        if w == v {
                            break;
                        }
                    }
                    let cyclic = comp.len() > 1 || succ[v].contains(&v);
                    for w in comp {
                        rec[w] = cyclic;
                    }
                }
            }
        }
        rec
    }
}

impl Ir {
    /// The named strategies that give at least one result whenever they
    /// terminate (`id`, `repeat*`, `iterate*`, a choice with such an
    /// alternative, sequences of these; least fixpoint over the calls).
    pub fn total_strategies(&self) -> Vec<bool> {
        fn total(s: &Strategy, tot: &[bool]) -> bool {
            match s {
                Strategy::Id | Strategy::Repeat(_) | Strategy::Iterate(_) => true,
                Strategy::Fail | Strategy::Rules { .. } | Strategy::Meta => false,
                Strategy::Strats { alts, .. } => {
                    alts.iter().any(|a| a.iter().all(|s| total(s, tot)))
                }
                Strategy::Call(c) => tot[*c],
            }
        }
        let mut tot = vec![false; self.strategies.len()];
        loop {
            let mut changed = false;
            for (i, st) in self.strategies.iter().enumerate() {
                if !tot[i] && st.body.iter().all(|s| total(s, &tot)) {
                    tot[i] = true;
                    changed = true;
                }
            }
            if !changed {
                return tot;
            }
        }
    }

    /// The calls of strategy `me` to itself in tail position, when `me` is
    /// deterministic: its function can then be a loop (spec D3) where such
    /// a call on `r` replaces the term by `r` and starts again, the stack
    /// staying flat (`s => first(step ; s, id)` run 10^6 times). A call is
    /// in tail position when the result of the body is then the result of
    /// the call: it ends the body, or the last sequence of a `DC`/`ONE`
    /// alternative (or of a `DK` of one alternative) in tail position,
    /// what comes before it in its sequence giving at most one result; in
    /// an alternative that is not the last one, only if `me` always has a
    /// result (`total`, [`Ir::total_strategies`]): else its failure would
    /// let the next alternatives run. The nodes are returned by address
    /// (the emitter recognises them while it walks the same body).
    ///
    /// The loop gives the same results in the same order as the
    /// interpreter, but it may terminate where the interpreter does not:
    /// it stops the search after the single result, so the alternatives
    /// that would run after it (and could loop for ever without a result)
    /// are never started. With `me => first(dc(dec) ; me, iterate*(spin ;
    /// fail))` and `spin => dc(inc) ; spin` on 3, elanc-rs and elanc-go
    /// print 0 and exit; `elan -b` prints 0, then searches the `iterate*`
    /// of the outer calls for ever (docs/followups.md).
    pub fn tail_calls(&self, me: StratId, total: &[bool]) -> Vec<*const Strategy> {
        let mut out = Vec::new();
        if self.strategies[me].det {
            self.tail_seq(me, total[me], &self.strategies[me].body, &mut out);
        }
        out
    }

    fn tail_seq(
        &self,
        me: StratId,
        me_total: bool,
        seq: &[Strategy],
        out: &mut Vec<*const Strategy>,
    ) {
        let Some((last, prefix)) = seq.split_last() else {
            return;
        };
        if !self.seq_det(prefix) {
            return;
        }
        match last {
            Strategy::Call(c) if *c == me => out.push(last as *const Strategy),
            Strategy::Strats {
                choice: Choice::Dc | Choice::One,
                alts,
            } => {
                for (i, a) in alts.iter().enumerate() {
                    if i + 1 == alts.len() || me_total {
                        self.tail_seq(me, me_total, a, out);
                    }
                }
            }
            Strategy::Strats {
                choice: Choice::Dk,
                alts,
            } if alts.len() == 1 => self.tail_seq(me, me_total, &alts[0], out),
            _ => {}
        }
    }
}

/// Does `s` give at most one result, given the determinism of the rules
/// of each label, their number and the determinism of the named
/// strategies? (Used by [`crate::lower`] during its fixpoint and by
/// [`Ir::det`].)
pub fn strategy_det(
    s: &Strategy,
    rules_det: &dyn Fn(LabelId) -> bool,
    n_rules: &dyn Fn(LabelId) -> usize,
    strat_det: &dyn Fn(StratId) -> bool,
) -> bool {
    let seq = |v: &[Strategy]| {
        v.iter()
            .all(|s| strategy_det(s, rules_det, n_rules, strat_det))
    };
    match s {
        Strategy::Id | Strategy::Fail => true,
        Strategy::Rules { choice, labels } => match choice {
            Choice::One => true,
            Choice::Dc => labels.iter().all(|&l| rules_det(l)),
            Choice::Dk => {
                labels.iter().all(|&l| rules_det(l))
                    && labels.iter().map(|&l| n_rules(l)).sum::<usize>() <= 1
            }
        },
        Strategy::Strats { choice, alts } => match choice {
            Choice::One => true,
            Choice::Dc => alts.iter().all(|a| seq(a)),
            Choice::Dk => alts.len() <= 1 && alts.iter().all(|a| seq(a)),
        },
        Strategy::Repeat(b) => seq(b),
        Strategy::Iterate(b) => b.iter().any(|s| matches!(s, Strategy::Fail)),
        Strategy::Call(c) => strat_det(*c),
        // META_APPL gives every result (conservative)
        Strategy::Meta => false,
    }
}

/// A builtin string for the reader (comments, messages): quoted, its bytes
/// as Latin-1 characters (ELAN sources are Latin-1), control bytes, `"`
/// and `\` escaped.
pub fn string_text(b: &[u8]) -> String {
    let mut s = String::from("\"");
    for &c in b {
        match c {
            b'"' | b'\\' => {
                s.push('\\');
                s.push(c as char);
            }
            0..=0x1f | 0x7f => s.push_str(&format!("\\x{c:02x}")),
            _ => s.push(c as char),
        }
    }
    s.push('"');
    s
}
