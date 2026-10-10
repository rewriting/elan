//! Typed model of a `.ref` file (`docs/ref-format.md`).
//!
//! A `.ref` file uses **numbers, not names**: every index below is a hash
//! position in one of the interpreter's tables (see the table of
//! `docs/ref-format.md`). The model keeps the numbers exactly as written, so
//! that [`crate::print`] reproduces the file; the `Program` methods resolve
//! them to names.
//!
//! Field meanings come from the two readers of the format: REM's grammar
//! (`src/compiler/rem/parser/REFParser.jj`) and the interpreter's writer and
//! reader (`src/interpreter/ref/ref.cc`, `parse/grammars/aterm.orig`,
//! `driver/ldmain.cc`).

/// Index in the identifier table `tabofident` (`MAXNOFIDENT` = 3000 entries).
pub type IdentIndex = u32;
/// Index of a sort in `typet` (`NTYPES` = 500 entries).
pub type SortIndex = u32;
/// Index of a module in `import` (`MAXNOFIMPORTS` = 200 entries).
pub type ModuleIndex = i32;
/// Index of a rule name in `RuleNames` (`MAXNOFTRN` = 2000 entries).
pub type RuleNameIndex = u32;
/// Index of a strategy name in `StrategyNames` (`MAXNOFSTRAT` = 500 entries).
pub type StrategyNameIndex = u32;
/// Function symbol code: built-ins below `FSYMCODESBEG` = 300
/// (`term/codes.h`), user symbols from 300.
pub type SymbolCode = i32;

/// A whole `.ref` file, in the order of its sections.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Program {
    /// `Identifiers` section: the identifier table (`tabofident`), used by
    /// `Ident(n)` lexems and `IDENT(n)` terms.
    pub identifiers: Vec<TableEntry>,
    /// `Sorts` section: the sort table (`typet`). `builtin` is not written in
    /// this section: it is the flag `b` of the `GrammarForSort s:b:` blocks
    /// (true when one of them says 1), filled in by the parser.
    pub sorts: Vec<Sort>,
    /// `Modules` section: the module table (`import`), names like `"int"`,
    /// `"choice1.lgi"`.
    pub modules: Vec<TableEntry>,
    /// `RuleNames` section. After the export's localisation
    /// (`trsystem::localize1`), the names have the form
    /// `label:sort/module!GL` (or `!LO`): one entry per rule label, sort,
    /// module and visibility. Older entries `label:sort` remain in the table
    /// (only referenced by the comments of the rules section).
    pub rule_names: Vec<TableEntry>,
    /// `StrategyNames` section (`strategynames_defs`), names like
    /// `s0:term/choice1`; `WHEREn:sort/module` are the anonymous strategies
    /// of `where x := (s) t` conditions.
    pub strategy_names: Vec<TableEntry>,
    /// The three grammar dumps, in file order: rules of both grammars, of
    /// the term grammar only, of the query grammar only.
    pub grammars: Grammars,
    /// Rewrite rules (`RULE`/`SWRULE`), labelled rules first (in rule-name
    /// table order), then unlabelled rules (by head symbol).
    pub rules: Vec<Rule>,
    /// Strategy definitions (`STRATEGY`), in strategy-name table order.
    pub strategies: Vec<StrategyDef>,
    /// The query (`QUERY`).
    pub query: Query,
}

/// One `n:"name".` line of a table section.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct TableEntry {
    /// Hash position in the table.
    pub index: u32,
    /// The name, as written between the quotes (no escape sequences: the
    /// writer prints the C string verbatim). Bytes are decoded as Latin-1, one
    /// `char` per byte, so printing gives back the same bytes.
    pub name: String,
}

/// An entry of the `Sorts` section.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Sort {
    /// Hash position in `typet`.
    pub index: SortIndex,
    /// Sort name, e.g. `"int"`, `"list[int]"`, `"intern ident"` (Latin-1
    /// decoded like [`TableEntry::name`]).
    pub name: String,
    /// The sort is one of the built-in lexical sorts (`bool`, `ident`,
    /// `builtinInt`, `builtinString`): derived from `GrammarForSort s:1:`.
    pub builtin: bool,
}

/// The three grammar dumps (`driver/ldmain.cc`: `globtermgr.Adump(3)`,
/// `globtermgr.Adump(2)`, `topgrammar->Adump(1)`).
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Grammars {
    /// Grammar rules present in both the term grammar and the query grammar
    /// (flag `INGLOBGRAM|INTOPGRAM` = 3).
    pub both: Vec<SortGrammar>,
    /// Rules of the term grammar `globtermgr` only (flag `INGLOBGRAM` = 2).
    pub global_only: Vec<SortGrammar>,
    /// Rules of the query grammar `topgrammar` only (flag `INTOPGRAM` = 1).
    pub top_only: Vec<SortGrammar>,
}

/// A `GrammarForSort s:b: ... nil end` block: the grammar rules whose
/// left-hand side is one sort. A sort has a block in a dump only when it has
/// grammar rules at all (possibly none with that dump's flag: then the block
/// is empty).
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct SortGrammar {
    /// The sort (left-hand side of the grammar rules).
    pub sort: SortIndex,
    /// `b`: 1 when the sort is built-in (`bool`, `ident`, `builtinInt`,
    /// `builtinString`), else 0.
    pub builtin: bool,
    /// The grammar rules, i.e. the operator declarations.
    pub operators: Vec<Operator>,
}

/// One grammar rule `flag:symbol:priority:syntax:semantic:infos:defstrat:lexems.nil:nil.`
/// (`grammar::Adump`). REM names the fields
/// `intGrammar:intSym:intArity:intInfo:intSemantic:intTheory:intStrategy`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Operator {
    /// Grammar flag of the rule: 1 = query grammar only, 2 = term grammar
    /// only, 3 = both; always the flag of the dump the rule is in.
    pub flag: i32,
    /// Function symbol code of the operator (`gr->r->rulenumber`).
    pub symbol: SymbolCode,
    /// Parsing priority (`priority & RPRIORITYMSK`, 0..4095).
    pub priority: i32,
    /// Syntactic bits: printable form, associativity, built-in strategy.
    pub syntax: SyntaxFlags,
    /// Built-in semantic action (`gr->r->semantic`): 0 = none (a
    /// constructor), else a built-in code of `term/codes.h` (e.g.
    /// `INTGREATER` = 12); negative = the same action `-semantic` for an
    /// instance whose name gets the sort appended (`fsym::add_sort`), REM
    /// then also declares the symbol `-semantic`.
    pub semantic: i32,
    /// Equational theory of the symbol (`fsymtab[symbol].infos()`):
    /// 0 = free (`FSNOINFO`), 1 = commutative (`FSCOMM`), 2 = AC
    /// (`FSASSOCCOM`).
    pub infos: i32,
    /// Default strategy word of the operator (`gr->r->defstrat`), decoded.
    pub defstrat: DefStrat,
    /// The right-hand side of the grammar rule (the mixfix form of the
    /// operator): one lexem per token or argument.
    pub rhs: Vec<Lexem>,
    /// Local strategies; never exported ("LOCAL STRATEGIES ARE NOT EXPORTED
    /// YET"), always empty in practice; REM ignores them.
    pub local_strategies: Vec<i32>,
}

/// The fourth field of a grammar rule: `(printable ? 8 : 0) + priority /
/// (RPRIORITYMSK+1)`, i.e. the high bits of the interpreter's priority word.
/// REM rebuilds the Earley `info` word as `priority + bits * 4096`.
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct SyntaxFlags(pub i32);

impl SyntaxFlags {
    /// `RLEFTASSOC` (octal 010000).
    pub const LEFT_ASSOC: i32 = 1;
    /// `RRIGHTASSOC` (octal 020000).
    pub const RIGHT_ASSOC: i32 = 2;
    /// `RBINSTR` (octal 040000): built-in strategy operator.
    pub const BUILTIN_STRATEGY: i32 = 4;
    /// `PRINTABLE` (`ref.cc`): this grammar rule is the textual form used to
    /// print the symbol (`fsymtab[symbol].textform()`); the interpreter's
    /// importer only (re)defines `fsymtab[symbol]` from such rules.
    pub const PRINTABLE: i32 = 8;

    /// The raw value as written.
    pub fn bits(self) -> i32 {
        self.0
    }
    /// Is this the printing form of the symbol?
    pub fn printable(self) -> bool {
        self.0 & Self::PRINTABLE != 0
    }
    /// Declared `assocLeft`.
    pub fn left_assoc(self) -> bool {
        self.0 & Self::LEFT_ASSOC != 0
    }
    /// Declared `assocRight`.
    pub fn right_assoc(self) -> bool {
        self.0 & Self::RIGHT_ASSOC != 0
    }
    /// Built-in strategy operator.
    pub fn builtin_strategy(self) -> bool {
        self.0 & Self::BUILTIN_STRATEGY != 0
    }
}

/// The `defstrat` word of an operator (`term/codes.h`): a kind in bits 24-27
/// and two 12-bit symbol codes. Decoded only when re-encoding gives the same
/// word back; any other value (e.g. one built from a code -1, whose shifted
/// bits overwrite the kind) is kept as [`DefStrat::Raw`].
#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum DefStrat {
    /// 0: no default strategy (the usual case).
    None,
    /// `FSYM_FLAG(f1,f2)` = `0x1000000 | f1<<12 | f2` (`DS_FSYM`): two symbol
    /// codes.
    Fsym { f1: u32, f2: u32 },
    /// `LAB_FLAG(f,lab)` = `0x2000000 | f<<12 | lab` (`DS_LAB`): a symbol
    /// code and a rule label index.
    Lab { f: u32, lab: u32 },
    /// `DSTR_FLAG(f,lab)` = `0x4000000 | f<<12 | lab` (`DS_DSTR`).
    Dstr { f: u32, lab: u32 },
    /// `APPLY_FLAG(x)` = `0x8000000 | x` (`DS_APPL`), `x` < 2^24.
    Apply { x: u32 },
    /// Any other word, kept verbatim.
    Raw(i64),
}

impl DefStrat {
    /// Kind bit of [`DefStrat::Fsym`].
    pub const DS_FSYM: i64 = 0x100_0000;
    /// Kind bit of [`DefStrat::Lab`].
    pub const DS_LAB: i64 = 0x200_0000;
    /// Kind bit of [`DefStrat::Dstr`].
    pub const DS_DSTR: i64 = 0x400_0000;
    /// Kind bit of [`DefStrat::Apply`].
    pub const DS_APPL: i64 = 0x800_0000;

    /// Decodes a word as written in the file.
    pub fn decode(word: i64) -> DefStrat {
        let lo = (word & 0xfff) as u32;
        let hi = ((word & 0xfff000) >> 12) as u32;
        let d = match word & 0xf00_0000 {
            _ if word == 0 => DefStrat::None,
            Self::DS_FSYM => DefStrat::Fsym { f1: hi, f2: lo },
            Self::DS_LAB => DefStrat::Lab { f: hi, lab: lo },
            Self::DS_DSTR => DefStrat::Dstr { f: hi, lab: lo },
            Self::DS_APPL => DefStrat::Apply {
                x: (word & 0xff_ffff) as u32,
            },
            _ => DefStrat::Raw(word),
        };
        if d.encode() == word {
            d
        } else {
            DefStrat::Raw(word)
        }
    }

    /// The word written in the file.
    pub fn encode(self) -> i64 {
        match self {
            DefStrat::None => 0,
            DefStrat::Fsym { f1, f2 } => Self::DS_FSYM | (f1 as i64) << 12 | f2 as i64,
            DefStrat::Lab { f, lab } => Self::DS_LAB | (f as i64) << 12 | lab as i64,
            DefStrat::Dstr { f, lab } => Self::DS_DSTR | (f as i64) << 12 | lab as i64,
            DefStrat::Apply { x } => Self::DS_APPL | x as i64,
            DefStrat::Raw(w) => w,
        }
    }
}

/// A lexem of the right-hand side of a grammar rule (`lexem::Adump`).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Lexem {
    /// `Type(n)`: an argument (non-terminal) of sort `n`.
    Type(SortIndex),
    /// `Char(c)`: a one-character token, `c` its character code.
    Char(i32),
    /// `Num(n)`: a number token.
    Num(i32),
    /// `String(n)`: a string token (written by the interpreter, never seen in
    /// practice; REM does not read it).
    String(i32),
    /// `Ident(n)`: an identifier token, `n` an index in `Identifiers`.
    Ident(IdentIndex),
    /// `Blank`: a blank required between two tokens; not a token for the
    /// Earley parser (REM skips it in its tables).
    Blank,
}

/// A term (`term::Awriterec`).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Term {
    /// `FSYM(a1.a2. ... nil, f)`: application of symbol `f` (its arity is the
    /// number of arguments; the importer takes it from `fsymtab`).
    ///
    /// Special case of `--cexport`: a call of a strategy `call` (symbol
    /// `Call` = 144) is written `FSYM(/**/INT(s).nil,144)`, `s` being the
    /// index of the called strategy in `StrategyNames`.
    Fsym { args: Vec<Term>, symbol: SymbolCode },
    /// `VAR(i,s)`: variable number `i` of the rule (0..nvars-1), of sort `s`.
    Var { index: u32, sort: SortIndex },
    /// `EVAR(i,s)`: an extension variable (only with `--cexport`); REM does
    /// not compile rules whose left-hand side contains one.
    EVar { index: u32, sort: SortIndex },
    /// `INT(n)`: a built-in integer.
    Int(i64),
    /// `IDENT(n)`: a built-in identifier, `n` an index in `Identifiers`.
    Ident(IdentIndex),
    /// `STRING(c1.c2. ... nil)`: a built-in string, as character codes (the
    /// C `char`s, signed: bytes above 127 are negative).
    Str(Vec<i32>),
}

/// A rewrite rule.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Rule {
    /// Rule label, an index in `RuleNames`; `None` for an unlabelled rule
    /// (written `-1`).
    pub name: Option<RuleNameIndex>,
    /// Sort of the rule (of both sides).
    pub sort: SortIndex,
    /// Module that declares the rule.
    pub module: ModuleIndex,
    /// Visibility: `RGLOP` = 1 global, `RLOCOOP` = 2 local.
    pub infos: i32,
    /// Matching algorithm of the left-hand side (`whichmatch`):
    /// `ACMATCH` = 0 (the pattern contains AC symbols), `NORMMATCH` = 1.
    pub which_match: i32,
    /// Number of variables of the rule (`VAR(i,_)` has `i < nvars`).
    pub nvars: u32,
    /// Left-hand side.
    pub lhs: Term,
    /// Right-hand side and conditions.
    pub body: RuleBody,
}

/// The right part of a rule.
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum RuleBody {
    /// `RULE(...,lhs,rhs,wheres)`: conditions and local assignments, evaluated
    /// in order, then `rhs`.
    Plain { rhs: Term, wheres: Vec<Where> },
    /// `SWRULE(...,lhs,switch)`: a rule with a `switch`/`case` right-hand
    /// side (written by the interpreter; REM does not read it).
    Switch(Switch),
}

/// A right-hand side with `case` branches (`struct tseq`).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Switch {
    /// `NOSWITCH(wheres, result)`: the conditions then a result term.
    NoSwitch { wheres: Vec<Where>, result: Term },
    /// `SWITCH(wheres, branches)`: the conditions then the first branch whose
    /// test holds.
    Switch {
        wheres: Vec<Where>,
        branches: Vec<Branch>,
    },
}

/// One `test, switch` branch of [`Switch::Switch`].
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Branch {
    /// The branch condition.
    pub test: Term,
    /// What follows when the test holds.
    pub body: Switch,
}

/// A condition or local assignment of a rule (`struct wherelist`,
/// `Awherelidump`).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum Where {
    /// `IFF(t)`: `if t` (a boolean term).
    If(Term),
    /// `WHERE(VAR(i,s),strategy,t)`: `where x := (strategy) t`; the
    /// interpreter writes a variable on the left, REM accepts any term.
    /// `strategy` is an index in `StrategyNames`, `None` (`-1`) for `()`
    /// (plain normalisation).
    Assign {
        lhs: Term,
        strategy: Option<StrategyNameIndex>,
        rhs: Term,
    },
    /// `PWHERE(pattern,sort,strategy,t)`: `where pattern := (strategy) t`,
    /// matching against a pattern of sort `sort`.
    Match {
        pattern: Term,
        sort: SortIndex,
        strategy: Option<StrategyNameIndex>,
        rhs: Term,
    },
    /// `TRY(b1.b2. ... nil)`: `choose try b1 try b2 ... end`, each branch a
    /// list of conditions (the first branch that succeeds is taken).
    Try(Vec<Vec<Where>>),
}

/// `STRATEGY(name,sort,module,body)`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct StrategyDef {
    /// Index in `StrategyNames`.
    pub name: StrategyNameIndex,
    /// Sort the strategy applies to.
    pub sort: SortIndex,
    /// Module that declares the strategy.
    pub module: ModuleIndex,
    /// The body: a sequence `s1 ; s2 ; ...` (at least one element).
    pub body: StrategySeq,
}

/// A sequential composition `s1 ; s2 ; ...` (`StrategyBody`): one element or
/// more.
pub type StrategySeq = Vec<StrategyExpr>;

/// An elementary strategy (`strategy::Asimpledump`, REM `StrategyTerm`).
#[derive(Clone, Debug, PartialEq, Eq)]
pub enum StrategyExpr {
    /// `id`.
    Id,
    /// `fail`.
    Fail,
    /// `META`: the meta strategy.
    Meta,
    /// `one(r1.r2. ... nil)`: one result of the first applicable labelled
    /// rule. The indices are `RuleNames` entries of the localised form
    /// `label:sort/module!GL` (one per module and visibility that the rule
    /// label resolves to).
    OneRules(Vec<RuleNameIndex>),
    /// `dc(r1. ... nil)`: dont care choose over rules.
    DcRules(Vec<RuleNameIndex>),
    /// `dk(r1. ... nil)`: dont know choose over rules (all results).
    DkRules(Vec<RuleNameIndex>),
    /// `normin(r1. ... nil)`: normalise (innermost) with these rules.
    NormIn(Vec<RuleNameIndex>),
    /// `normout(r1. ... nil)`: normalise (outermost) with these rules.
    NormOut(Vec<RuleNameIndex>),
    /// `ONE(s1 , s2 , ...)`: one result of the first strategy that succeeds.
    OneStrats(Vec<StrategySeq>),
    /// `DC(s1 , s2 , ...)`: dont care choose over strategies.
    DcStrats(Vec<StrategySeq>),
    /// `DK(s1 , s2 , ...)`: dont know choose over strategies.
    DkStrats(Vec<StrategySeq>),
    /// `repeat*(s)`.
    Repeat(StrategySeq),
    /// `iterate*(s)`.
    Iterate(StrategySeq),
    /// `call(n)`: call of the strategy `n` (index in `StrategyNames`).
    Call(StrategyNameIndex),
    /// `tall(s)`: traversal, `s` on all subterms.
    TAll(StrategySeq),
    /// `tone(s)`: traversal, `s` on one subterm.
    TOne(StrategySeq),
    /// `tsome(s)`: traversal, `s` on some subterms.
    TSome(StrategySeq),
    /// `rewrite(s)`: one rewrite step.
    Rewrite(StrategySeq),
    /// `dccall(name, maxn, sort)`: dont care concurrent call of an external
    /// process (written by the interpreter; REM does not read it). `name` is
    /// the process name as character codes.
    DcCall {
        name: Vec<i32>,
        max: i32,
        sort: SortIndex,
    },
    /// `dkcall(name, maxn, sort)`: dont know concurrent call.
    DkCall {
        name: Vec<i32>,
        max: i32,
        sort: SortIndex,
    },
}

/// `QUERY(sort,resultsort,strategy,startwith,checkwith)`.
#[derive(Clone, Debug, PartialEq, Eq)]
pub struct Query {
    /// Sort of the query term.
    pub sort: SortIndex,
    /// Sort of the results.
    pub result_sort: SortIndex,
    /// Strategy applied to the query, index in `StrategyNames`; `None`
    /// (`-1`) for normalisation only.
    pub strategy: Option<StrategyNameIndex>,
    /// The `start with` term (the query run by `-noInput`).
    pub start_with: Term,
    /// The `check with` term (a boolean condition on results).
    pub check_with: Term,
}

fn lookup(t: &[TableEntry], i: u32) -> Option<&str> {
    t.iter().find(|e| e.index == i).map(|e| e.name.as_str())
}

impl Program {
    /// Name of identifier `i`.
    pub fn identifier(&self, i: IdentIndex) -> Option<&str> {
        lookup(&self.identifiers, i)
    }
    /// The sort of index `i`.
    pub fn sort(&self, i: SortIndex) -> Option<&Sort> {
        self.sorts.iter().find(|s| s.index == i)
    }
    /// Name of module `i`.
    pub fn module(&self, i: ModuleIndex) -> Option<&str> {
        u32::try_from(i).ok().and_then(|i| lookup(&self.modules, i))
    }
    /// Name of rule label `i`.
    pub fn rule_name(&self, i: RuleNameIndex) -> Option<&str> {
        lookup(&self.rule_names, i)
    }
    /// Name of strategy `i`.
    pub fn strategy_name(&self, i: StrategyNameIndex) -> Option<&str> {
        lookup(&self.strategy_names, i)
    }
    /// The definition of strategy `i`.
    pub fn strategy(&self, i: StrategyNameIndex) -> Option<&StrategyDef> {
        self.strategies.iter().find(|s| s.name == i)
    }
    /// All operators of the three grammar dumps.
    pub fn operators(&self) -> impl Iterator<Item = (&SortGrammar, &Operator)> {
        self.grammars
            .both
            .iter()
            .chain(&self.grammars.global_only)
            .chain(&self.grammars.top_only)
            .flat_map(|g| g.operators.iter().map(move |o| (g, o)))
    }
    /// The printing form of a symbol: its operator whose
    /// [`SyntaxFlags::printable`] is set, with the sort it belongs to.
    pub fn printing_form(&self, symbol: SymbolCode) -> Option<(&SortGrammar, &Operator)> {
        self.operators()
            .find(|(_, o)| o.symbol == symbol && o.syntax.printable())
    }
}
