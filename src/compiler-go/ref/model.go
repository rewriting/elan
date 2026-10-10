// Typed model of a .ref file (docs/ref-format.md).
//
// A .ref file uses numbers, not names: every index below is a hash position
// in one of the interpreter's tables (see the table of docs/ref-format.md).
// The model keeps the numbers exactly as written, so that Print reproduces
// the file; the Program methods resolve them to names.
//
// Field meanings come from the two readers of the format: REM's grammar
// (src/compiler/rem/parser/REFParser.jj) and the interpreter's writer and
// reader (src/interpreter/ref/ref.cc, parse/grammars/aterm.orig,
// driver/ldmain.cc).
//
// Sum types are either small sealed interfaces (Term, RuleBody, Switch,
// Where), whose variants are plain structs, or tagged structs (DefStrat,
// Lexem, StrategyExpr) whose Kind/Op field says which fields are used.
// Two models are equal when reflect.DeepEqual says so; the parser builds
// empty lists as nil.

package ref

// IdentIndex is an index in the identifier table tabofident (MAXNOFIDENT =
// 3000 entries).
type IdentIndex = uint32

// SortIndex is the index of a sort in typet (NTYPES = 500 entries).
type SortIndex = uint32

// ModuleIndex is the index of a module in import (MAXNOFIMPORTS = 200
// entries).
type ModuleIndex = int32

// RuleNameIndex is the index of a rule name in RuleNames (MAXNOFTRN = 2000
// entries).
type RuleNameIndex = uint32

// StrategyNameIndex is the index of a strategy name in StrategyNames
// (MAXNOFSTRAT = 500 entries).
type StrategyNameIndex = uint32

// SymbolCode is a function symbol code: built-ins below FSYMCODESBEG = 300
// (term/codes.h), user symbols from 300.
type SymbolCode = int32

// OptIndex is an optional index, written -1 when absent.
type OptIndex struct {
	// Index is the index when Valid.
	Index uint32
	// Valid is false for -1.
	Valid bool
}

// Some is the present index i.
func Some(i uint32) OptIndex { return OptIndex{Index: i, Valid: true} }

// None is the absent index (-1).
var None = OptIndex{}

// Value is the number written in the file: the index, or -1.
func (o OptIndex) Value() int64 {
	if o.Valid {
		return int64(o.Index)
	}
	return -1
}

// Program is a whole .ref file, in the order of its sections.
type Program struct {
	// Identifiers section: the identifier table (tabofident), used by
	// Ident(n) lexems and IDENT(n) terms.
	Identifiers []TableEntry
	// Sorts section: the sort table (typet). Builtin is not written in this
	// section: it is the flag b of the `GrammarForSort s:b:` blocks (true
	// when one of them says 1), filled in by the parser.
	Sorts []Sort
	// Modules section: the module table (import), names like "int",
	// "choice1.lgi".
	Modules []TableEntry
	// RuleNames section. After the export's localisation
	// (trsystem::localize1), the names have the form label:sort/module!GL
	// (or !LO): one entry per rule label, sort, module and visibility. Older
	// entries label:sort remain in the table (only referenced by the comments
	// of the rules section).
	RuleNames []TableEntry
	// StrategyNames section (strategynames_defs), names like s0:term/choice1;
	// WHEREn:sort/module are the anonymous strategies of `where x := (s) t`
	// conditions.
	StrategyNames []TableEntry
	// Grammars are the three grammar dumps, in file order: rules of both
	// grammars, of the term grammar only, of the query grammar only.
	Grammars Grammars
	// Rules are the rewrite rules (RULE/SWRULE), labelled rules first (in
	// rule-name table order), then unlabelled rules (by head symbol).
	Rules []Rule
	// Strategies are the strategy definitions (STRATEGY), in strategy-name
	// table order.
	Strategies []StrategyDef
	// Query is the query (QUERY).
	Query Query
}

// TableEntry is one `n:"name".` line of a table section.
type TableEntry struct {
	// Index is the hash position in the table.
	Index uint32
	// Name is the name as written between the quotes (no escape sequences:
	// the writer prints the C string verbatim), raw bytes of the file.
	Name string
}

// Sort is an entry of the Sorts section.
type Sort struct {
	// Index is the hash position in typet.
	Index SortIndex
	// Name is the sort name, e.g. "int", "list[int]", "intern ident" (raw
	// bytes like TableEntry.Name).
	Name string
	// Builtin: the sort is one of the built-in lexical sorts (bool, ident,
	// builtinInt, builtinString), derived from `GrammarForSort s:1:`.
	Builtin bool
}

// Grammars are the three grammar dumps (driver/ldmain.cc:
// globtermgr.Adump(3), globtermgr.Adump(2), topgrammar->Adump(1)).
type Grammars struct {
	// Both: grammar rules present in both the term grammar and the query
	// grammar (flag INGLOBGRAM|INTOPGRAM = 3).
	Both []SortGrammar
	// GlobalOnly: rules of the term grammar globtermgr only (flag
	// INGLOBGRAM = 2).
	GlobalOnly []SortGrammar
	// TopOnly: rules of the query grammar topgrammar only (flag INTOPGRAM =
	// 1).
	TopOnly []SortGrammar
}

// Dumps returns the three dumps in file order.
func (g *Grammars) Dumps() [3][]SortGrammar {
	return [3][]SortGrammar{g.Both, g.GlobalOnly, g.TopOnly}
}

// SortGrammar is a `GrammarForSort s:b: ... nil end` block: the grammar
// rules whose left-hand side is one sort. A sort has a block in a dump only
// when it has grammar rules at all (possibly none with that dump's flag: then
// the block is empty).
type SortGrammar struct {
	// Sort is the left-hand side of the grammar rules.
	Sort SortIndex
	// Builtin is b: 1 when the sort is built-in (bool, ident, builtinInt,
	// builtinString), else 0.
	Builtin bool
	// Operators are the grammar rules, i.e. the operator declarations.
	Operators []Operator
}

// Operator is one grammar rule
// `flag:symbol:priority:syntax:semantic:infos:defstrat:lexems.nil:nil.`
// (grammar::Adump). REM names the fields
// intGrammar:intSym:intArity:intInfo:intSemantic:intTheory:intStrategy.
type Operator struct {
	// Flag is the grammar flag of the rule: 1 = query grammar only, 2 = term
	// grammar only, 3 = both; always the flag of the dump the rule is in.
	Flag int32
	// Symbol is the function symbol code of the operator
	// (gr->r->rulenumber).
	Symbol SymbolCode
	// Priority is the parsing priority (priority & RPRIORITYMSK, 0..4095).
	Priority int32
	// Syntax holds the syntactic bits: printable form, associativity,
	// built-in strategy.
	Syntax SyntaxFlags
	// Semantic is the built-in semantic action (gr->r->semantic): 0 = none
	// (a constructor), else a built-in code of term/codes.h (e.g.
	// INTGREATER = 12); negative = the same action -semantic for an instance
	// whose name gets the sort appended (fsym::add_sort), REM then also
	// declares the symbol -semantic.
	Semantic int32
	// Infos is the equational theory of the symbol
	// (fsymtab[symbol].infos()): 0 = free (FSNOINFO), 1 = commutative
	// (FSCOMM), 2 = AC (FSASSOCCOM).
	Infos int32
	// Defstrat is the default strategy word of the operator
	// (gr->r->defstrat), decoded.
	Defstrat DefStrat
	// Rhs is the right-hand side of the grammar rule (the mixfix form of the
	// operator): one lexem per token or argument.
	Rhs []Lexem
	// LocalStrategies are never exported ("LOCAL STRATEGIES ARE NOT
	// EXPORTED YET"), always empty in practice; REM ignores them.
	LocalStrategies []int32
}

// SyntaxFlags is the fourth field of a grammar rule: (printable ? 8 : 0) +
// priority / (RPRIORITYMSK+1), i.e. the high bits of the interpreter's
// priority word. REM rebuilds the Earley info word as priority + bits *
// 4096.
type SyntaxFlags int32

const (
	// LeftAssoc is RLEFTASSOC (octal 010000).
	LeftAssoc SyntaxFlags = 1
	// RightAssoc is RRIGHTASSOC (octal 020000).
	RightAssoc SyntaxFlags = 2
	// BuiltinStrategy is RBINSTR (octal 040000): built-in strategy operator.
	BuiltinStrategy SyntaxFlags = 4
	// Printable is PRINTABLE (ref.cc): this grammar rule is the textual form
	// used to print the symbol (fsymtab[symbol].textform()); the
	// interpreter's importer only (re)defines fsymtab[symbol] from such
	// rules.
	Printable SyntaxFlags = 8
)

// Printable: is this the printing form of the symbol?
func (f SyntaxFlags) Printable() bool { return f&Printable != 0 }

// LeftAssoc: declared assocLeft.
func (f SyntaxFlags) LeftAssoc() bool { return f&LeftAssoc != 0 }

// RightAssoc: declared assocRight.
func (f SyntaxFlags) RightAssoc() bool { return f&RightAssoc != 0 }

// BuiltinStrategy: built-in strategy operator.
func (f SyntaxFlags) BuiltinStrategy() bool { return f&BuiltinStrategy != 0 }

// DefStratKind is the kind of a DefStrat word.
type DefStratKind uint8

const (
	// DSNone is 0: no default strategy (the usual case).
	DSNone DefStratKind = iota
	// DSFsym is FSYM_FLAG(f1,f2) = 0x1000000 | f1<<12 | f2: two symbol
	// codes (Hi = f1, Lo = f2).
	DSFsym
	// DSLab is LAB_FLAG(f,lab) = 0x2000000 | f<<12 | lab: a symbol code
	// (Hi) and a rule label index (Lo).
	DSLab
	// DSDstr is DSTR_FLAG(f,lab) = 0x4000000 | f<<12 | lab (Hi = f, Lo =
	// lab).
	DSDstr
	// DSApply is APPLY_FLAG(x) = 0x8000000 | x, x < 2^24 (in X).
	DSApply
	// DSRaw is any other word, kept verbatim (in Word).
	DSRaw
)

// Kind bits of the defstrat word (term/codes.h).
const (
	DSFsymBit  int64 = 0x100_0000
	DSLabBit   int64 = 0x200_0000
	DSDstrBit  int64 = 0x400_0000
	DSApplyBit int64 = 0x800_0000
)

// DefStrat is the defstrat word of an operator (term/codes.h): a kind in
// bits 24-27 and two 12-bit symbol codes. Decoded only when re-encoding gives
// the same word back; any other value (e.g. one built from a code -1, whose
// shifted bits overwrite the kind) is kept as DSRaw.
type DefStrat struct {
	// Kind says which fields are used.
	Kind DefStratKind
	// Hi is f1 (DSFsym) or f (DSLab, DSDstr): bits 12-23.
	Hi uint32
	// Lo is f2 (DSFsym) or lab (DSLab, DSDstr): bits 0-11.
	Lo uint32
	// X is the operand of DSApply.
	X uint32
	// Word is the verbatim word of DSRaw.
	Word int64
}

// DecodeDefStrat decodes a word as written in the file.
func DecodeDefStrat(word int64) DefStrat {
	lo := uint32(word & 0xfff)
	hi := uint32((word & 0xfff000) >> 12)
	var d DefStrat
	switch {
	case word == 0:
		d = DefStrat{Kind: DSNone}
	case word&0xf00_0000 == DSFsymBit:
		d = DefStrat{Kind: DSFsym, Hi: hi, Lo: lo}
	case word&0xf00_0000 == DSLabBit:
		d = DefStrat{Kind: DSLab, Hi: hi, Lo: lo}
	case word&0xf00_0000 == DSDstrBit:
		d = DefStrat{Kind: DSDstr, Hi: hi, Lo: lo}
	case word&0xf00_0000 == DSApplyBit:
		d = DefStrat{Kind: DSApply, X: uint32(word & 0xff_ffff)}
	default:
		return DefStrat{Kind: DSRaw, Word: word}
	}
	if d.Encode() != word {
		return DefStrat{Kind: DSRaw, Word: word}
	}
	return d
}

// Encode gives the word written in the file.
func (d DefStrat) Encode() int64 {
	switch d.Kind {
	case DSFsym:
		return DSFsymBit | int64(d.Hi)<<12 | int64(d.Lo)
	case DSLab:
		return DSLabBit | int64(d.Hi)<<12 | int64(d.Lo)
	case DSDstr:
		return DSDstrBit | int64(d.Hi)<<12 | int64(d.Lo)
	case DSApply:
		return DSApplyBit | int64(d.X)
	case DSRaw:
		return d.Word
	}
	return 0
}

// LexemKind is the kind of a Lexem.
type LexemKind uint8

const (
	// LexType is Type(n): an argument (non-terminal) of sort n.
	LexType LexemKind = iota
	// LexChar is Char(c): a one-character token, c its character code.
	LexChar
	// LexNum is Num(n): a number token.
	LexNum
	// LexString is String(n): a string token (written by the interpreter,
	// never seen in practice; REM does not read it).
	LexString
	// LexIdent is Ident(n): an identifier token, n an index in Identifiers.
	LexIdent
	// LexBlank is Blank: a blank required between two tokens; not a token
	// for the Earley parser (REM skips it in its tables).
	LexBlank
)

var lexemNames = [...]string{"Type", "Char", "Num", "String", "Ident", "Blank"}

// String is the keyword of the kind in the file.
func (k LexemKind) String() string { return lexemNames[k] }

// Lexem is a lexem of the right-hand side of a grammar rule (lexem::Adump).
type Lexem struct {
	// Kind says what the lexem is.
	Kind LexemKind
	// N is the value: a SortIndex (LexType), an IdentIndex (LexIdent), an
	// int32 (LexChar, LexNum, LexString); 0 for LexBlank.
	N int64
}

// Term is a term (term::Awriterec): Fsym, Var, EVar, Int, Ident or Str.
type Term interface{ isTerm() }

// Fsym is FSYM(a1.a2. ... nil, f): application of symbol f (its arity is
// the number of arguments; the importer takes it from fsymtab).
//
// Special case of --cexport: a call of a strategy `call` (symbol Call =
// 144) is written FSYM(/**/INT(s).nil,144), s being the index of the called
// strategy in StrategyNames.
type Fsym struct {
	// Args are the arguments.
	Args []Term
	// Symbol is the function symbol code.
	Symbol SymbolCode
}

// Var is VAR(i,s): variable number i of the rule (0..nvars-1), of sort s.
type Var struct {
	// Index is the variable number.
	Index uint32
	// Sort is the sort of the variable.
	Sort SortIndex
}

// EVar is EVAR(i,s): an extension variable (only with --cexport); REM does
// not compile rules whose left-hand side contains one.
type EVar struct {
	// Index is the variable number.
	Index uint32
	// Sort is the sort of the variable.
	Sort SortIndex
}

// Int is INT(n): a built-in integer.
type Int struct {
	// Value is n.
	Value int64
}

// Ident is IDENT(n): a built-in identifier, n an index in Identifiers.
type Ident struct {
	// Index is n.
	Index IdentIndex
}

// Str is STRING(c1.c2. ... nil): a built-in string, as character codes (the
// C chars, signed: bytes above 127 are negative).
type Str struct {
	// Codes are the character codes.
	Codes []int32
}

func (Fsym) isTerm()  {}
func (Var) isTerm()   {}
func (EVar) isTerm()  {}
func (Int) isTerm()   {}
func (Ident) isTerm() {}
func (Str) isTerm()   {}

// Rule is a rewrite rule.
type Rule struct {
	// Name is the rule label, an index in RuleNames; None for an unlabelled
	// rule (written -1).
	Name OptIndex
	// Sort is the sort of the rule (of both sides).
	Sort SortIndex
	// Module is the module that declares the rule.
	Module ModuleIndex
	// Infos is the visibility: RGLOP = 1 global, RLOCOOP = 2 local.
	Infos int32
	// WhichMatch is the matching algorithm of the left-hand side
	// (whichmatch): ACMATCH = 0 (the pattern contains AC symbols),
	// NORMMATCH = 1.
	WhichMatch int32
	// Nvars is the number of variables of the rule (VAR(i,_) has i <
	// nvars).
	Nvars uint32
	// Lhs is the left-hand side.
	Lhs Term
	// Body is the right-hand side and conditions.
	Body RuleBody
}

// RuleBody is the right part of a rule: Plain (RULE) or a Switch (SWRULE:
// NoSwitch or SwitchCases).
type RuleBody interface{ isRuleBody() }

// Plain is RULE(...,lhs,rhs,wheres): conditions and local assignments,
// evaluated in order, then rhs.
type Plain struct {
	// Rhs is the right-hand side.
	Rhs Term
	// Wheres are the conditions and local assignments.
	Wheres []Where
}

// Switch is a right-hand side with case branches (struct tseq), the body of
// SWRULE(...,lhs,switch) (written by the interpreter; REM does not read it):
// NoSwitch or SwitchCases.
type Switch interface {
	RuleBody
	isSwitch()
}

// NoSwitch is NOSWITCH(wheres, result): the conditions then a result term.
type NoSwitch struct {
	// Wheres are the conditions.
	Wheres []Where
	// Result is the result term.
	Result Term
}

// SwitchCases is SWITCH(wheres, branches): the conditions then the first
// branch whose test holds.
type SwitchCases struct {
	// Wheres are the conditions.
	Wheres []Where
	// Branches are the `test, switch` branches.
	Branches []Branch
}

// Branch is one `test, switch` branch of SwitchCases.
type Branch struct {
	// Test is the branch condition.
	Test Term
	// Body is what follows when the test holds.
	Body Switch
}

func (Plain) isRuleBody()       {}
func (NoSwitch) isRuleBody()    {}
func (SwitchCases) isRuleBody() {}
func (NoSwitch) isSwitch()      {}
func (SwitchCases) isSwitch()   {}

// Where is a condition or local assignment of a rule (struct wherelist,
// Awherelidump): If, Assign, Match or Try.
type Where interface{ isWhere() }

// If is IFF(t): `if t` (a boolean term).
type If struct {
	// Cond is t.
	Cond Term
}

// Assign is WHERE(VAR(i,s),strategy,t): `where x := (strategy) t`; the
// interpreter writes a variable on the left, REM accepts any term.
type Assign struct {
	// Lhs is the assigned term (a variable).
	Lhs Term
	// Strategy is an index in StrategyNames, None (-1) for `()` (plain
	// normalisation).
	Strategy OptIndex
	// Rhs is t.
	Rhs Term
}

// Match is PWHERE(pattern,sort,strategy,t): `where pattern := (strategy)
// t`, matching against a pattern of sort sort.
type Match struct {
	// Pattern is the pattern.
	Pattern Term
	// Sort is the sort of the pattern.
	Sort SortIndex
	// Strategy is as in Assign.
	Strategy OptIndex
	// Rhs is t.
	Rhs Term
}

// Try is TRY(b1.b2. ... nil): `choose try b1 try b2 ... end`, each branch a
// list of conditions (the first branch that succeeds is taken).
type Try struct {
	// Branches are the branches.
	Branches [][]Where
}

func (If) isWhere()     {}
func (Assign) isWhere() {}
func (Match) isWhere()  {}
func (Try) isWhere()    {}

// StrategyDef is STRATEGY(name,sort,module,body).
type StrategyDef struct {
	// Name is the index in StrategyNames.
	Name StrategyNameIndex
	// Sort is the sort the strategy applies to.
	Sort SortIndex
	// Module is the module that declares the strategy.
	Module ModuleIndex
	// Body is a sequence s1 ; s2 ; ... (at least one element).
	Body StrategySeq
}

// StrategySeq is a sequential composition s1 ; s2 ; ... (StrategyBody): one
// element or more.
type StrategySeq = []StrategyExpr

// StrategyOp is the kind of an elementary strategy.
type StrategyOp uint8

const (
	// SId is id.
	SId StrategyOp = iota
	// SFail is fail.
	SFail
	// SMeta is META: the meta strategy.
	SMeta
	// SOneRules is one(r1.r2. ... nil): one result of the first applicable
	// labelled rule. The indices are RuleNames entries of the localised form
	// label:sort/module!GL (one per module and visibility that the rule
	// label resolves to).
	SOneRules
	// SDcRules is dc(r1. ... nil): dont care choose over rules.
	SDcRules
	// SDkRules is dk(r1. ... nil): dont know choose over rules (all
	// results).
	SDkRules
	// SNormIn is normin(r1. ... nil): normalise (innermost) with these
	// rules.
	SNormIn
	// SNormOut is normout(r1. ... nil): normalise (outermost) with these
	// rules.
	SNormOut
	// SOneStrats is ONE(s1 , s2 , ...): one result of the first strategy
	// that succeeds.
	SOneStrats
	// SDcStrats is DC(s1 , s2 , ...): dont care choose over strategies.
	SDcStrats
	// SDkStrats is DK(s1 , s2 , ...): dont know choose over strategies.
	SDkStrats
	// SRepeat is repeat*(s).
	SRepeat
	// SIterate is iterate*(s).
	SIterate
	// SCall is call(n): call of the strategy n (index in StrategyNames).
	SCall
	// STAll is tall(s): traversal, s on all subterms.
	STAll
	// STOne is tone(s): traversal, s on one subterm.
	STOne
	// STSome is tsome(s): traversal, s on some subterms.
	STSome
	// SRewrite is rewrite(s): one rewrite step.
	SRewrite
	// SDcCall is dccall(name, maxn, sort): dont care concurrent call of an
	// external process (written by the interpreter; REM does not read it).
	SDcCall
	// SDkCall is dkcall(name, maxn, sort): dont know concurrent call.
	SDkCall
)

var strategyKeywords = [...]string{
	"id", "fail", "META", "one", "dc", "dk", "normin", "normout",
	"ONE", "DC", "DK", "repeat*", "iterate*", "call", "tall", "tone",
	"tsome", "rewrite", "dccall", "dkcall",
}

// String is the keyword of the strategy in the file.
func (op StrategyOp) String() string { return strategyKeywords[op] }

// IsRules: the operand is a rule list (Rules).
func (op StrategyOp) IsRules() bool { return op >= SOneRules && op <= SNormOut }

// IsStrats: the operand is a list of alternatives (Alts).
func (op StrategyOp) IsStrats() bool { return op >= SOneStrats && op <= SDkStrats }

// IsUnary: the operand is one sequence (Body).
func (op StrategyOp) IsUnary() bool {
	return op == SRepeat || op == SIterate || (op >= STAll && op <= SRewrite)
}

// StrategyExpr is an elementary strategy (strategy::Asimpledump, REM
// StrategyTerm), a tagged struct: Op says which fields are used.
type StrategyExpr struct {
	// Op is the kind of strategy.
	Op StrategyOp
	// Rules are the rule labels of SOneRules, SDcRules, SDkRules, SNormIn,
	// SNormOut.
	Rules []RuleNameIndex
	// Alts are the alternatives of SOneStrats, SDcStrats, SDkStrats (at
	// least one).
	Alts []StrategySeq
	// Body is the operand of SRepeat, SIterate, STAll, STOne, STSome,
	// SRewrite.
	Body StrategySeq
	// Call is the called strategy of SCall (index in StrategyNames).
	Call StrategyNameIndex
	// Name is the process name of SDcCall, SDkCall, as character codes.
	Name []int32
	// Max is maxn of SDcCall, SDkCall.
	Max int32
	// Sort is the sort of SDcCall, SDkCall.
	Sort SortIndex
}

// Query is QUERY(sort,resultsort,strategy,startwith,checkwith).
type Query struct {
	// Sort is the sort of the query term.
	Sort SortIndex
	// ResultSort is the sort of the results.
	ResultSort SortIndex
	// Strategy is the strategy applied to the query, index in
	// StrategyNames; None (-1) for normalisation only.
	Strategy OptIndex
	// StartWith is the `start with` term (the query run by -noInput).
	StartWith Term
	// CheckWith is the `check with` term (a boolean condition on results).
	CheckWith Term
}

func lookup(t []TableEntry, i uint32) (string, bool) {
	for _, e := range t {
		if e.Index == i {
			return e.Name, true
		}
	}
	return "", false
}

// Identifier is the name of identifier i.
func (p *Program) Identifier(i IdentIndex) (string, bool) { return lookup(p.Identifiers, i) }

// Sort is the sort of index i, nil when there is none.
func (p *Program) Sort(i SortIndex) *Sort {
	for k := range p.Sorts {
		if p.Sorts[k].Index == i {
			return &p.Sorts[k]
		}
	}
	return nil
}

// Module is the name of module i.
func (p *Program) Module(i ModuleIndex) (string, bool) {
	if i < 0 {
		return "", false
	}
	return lookup(p.Modules, uint32(i))
}

// RuleName is the name of rule label i.
func (p *Program) RuleName(i RuleNameIndex) (string, bool) { return lookup(p.RuleNames, i) }

// StrategyName is the name of strategy i.
func (p *Program) StrategyName(i StrategyNameIndex) (string, bool) {
	return lookup(p.StrategyNames, i)
}

// Strategy is the definition of strategy i, nil when there is none.
func (p *Program) Strategy(i StrategyNameIndex) *StrategyDef {
	for k := range p.Strategies {
		if p.Strategies[k].Name == i {
			return &p.Strategies[k]
		}
	}
	return nil
}

// OperatorRef is an operator of a grammar dump with its sort block.
type OperatorRef struct {
	// Grammar is the block the operator is in.
	Grammar *SortGrammar
	// Op is the operator.
	Op *Operator
}

// Operators lists all operators of the three grammar dumps, in file order.
func (p *Program) Operators() []OperatorRef {
	var v []OperatorRef
	for _, d := range [][]SortGrammar{p.Grammars.Both, p.Grammars.GlobalOnly, p.Grammars.TopOnly} {
		for gi := range d {
			for oi := range d[gi].Operators {
				v = append(v, OperatorRef{&d[gi], &d[gi].Operators[oi]})
			}
		}
	}
	return v
}

// PrintingForm is the printing form of a symbol: its operator whose
// Syntax.Printable() is set, with the sort block it belongs to.
func (p *Program) PrintingForm(symbol SymbolCode) (OperatorRef, bool) {
	for _, r := range p.Operators() {
		if r.Op.Symbol == symbol && r.Op.Syntax.Printable() {
			return r, true
		}
	}
	return OperatorRef{}, false
}
