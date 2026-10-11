// Package ir is the intermediate representation of elanc-go, between the
// .ref model (package elan/ref) and the Go emission (package elan/gen), and
// the analysis that builds it ([Lower]).
//
// The design is the IR of the Rust compiler (src/compiler-rs/elanc-rs/src/
// ir.rs, S6a), adapted to the Go runtime (spec S7, decision G2): terms are
// generic shared nodes (elanrt.Term), so a sort needs no type of its own;
// the IR only says, per sort, whether its terms are native Go values or
// terms, and, per operator, whether a term with it at the top can exist
// (it then gets a runtime symbol).
//
// # What it contains (S7a: normalisation)
//
//   - The sorts reachable from the start term. A builtin sort (builtinInt,
//     bool, builtinString) and the sorts whose only constructor is an
//     injection of it (`@ : (builtinInt) int`) form a [Family]: they share
//     one representation, the injection being the identity. A family is
//     native (int32, bool, string) when none of its terms can be stuck,
//     else boxed (an elanrt.Term: a boxed value or a stuck term). Every
//     other sort is represented by terms.
//   - The reachable operators ([Op]), keyed by (symbol, profile): the
//     builtin codes below 300 are overloaded (`==` on builtinInt and on
//     bool). Each has a printing form (the last grammar rule of its
//     symbol, as findTextForm in src/compiler/runtime/termOut.c) and a
//     kind: free constructor, injection (identity), boolean constant,
//     builtin operation, or defined by unlabelled rules, with a decision
//     tree ([Decision]).
//   - The start term of the query.
//
// # Normalisation scheme
//
// Every operator f is compiled to a function taking its arguments in normal
// form and returning the normal form of f(args): the decision tree selects
// the rules whose left-hand side matches, in source order; the first one
// whose conditions hold is applied (its right-hand side is built bottom-up
// by calling the functions of its operators, so it is in normal form); when
// none applies, the term f(args) itself is the result (a stuck term), which
// needs a runtime symbol ([Op.Variant]). An operator is total when its
// unconditional rules cover every normal form of its arguments (a greatest
// fixpoint over the sorts, [Lower]); a total operator has no stuck term.
//
// # Strategies (S7b, as elanc-rs's S6b)
//
//   - The labels ([Label]): the labelled rules of one localised rule name
//     (`label:sort/module!GL`), in source order, with the decision tree of
//     their whole left-hand sides (one column: the term itself). A label is
//     compiled to a function in continuation-passing style over the Go
//     type of its sort, `func(t T, k rt.K[T], first bool) bool`: each
//     result of each rule that applies, in order, is passed to k (true from
//     k means stop); with first, only the results of the first rule that
//     has one (dc). Labelled rules never take part in normalisation; the
//     right-hand side is built by the normalisation functions, so every
//     result is in normal form (the interpreter reduces each result).
//   - The named strategies ([Strategy]): the STRATEGY definitions reachable
//     from the query and from the wheres, a body made of [Strat] elements,
//     each compiled to `func(t T, k rt.K[T]) bool`.
//   - [CondApply]: `where p := (S) t`, each result of S in turn (the
//     conditions after it backtrack into S); in normalisation, the first
//     result for which the rest of the conditions hold.
//   - The analyses: which strategies give at most one result ([Det],
//     greatest fixpoint), which always give one when they terminate
//     ([IR.TotalStrategies], least fixpoint), and the calls of a strategy
//     to itself in tail position ([IR.TailCalls]): deterministic repeat*,
//     iterate* and tail-recursive strategies are loops.
//
// Strategy terms built at run time (strat[X], defined strategies with
// parameters, call "name":X, META) are refused as "S7b task 2".
//
// # Extension points
//
//   - S7c (AC): [Pattern] gets an AC case, [Decision] an AC matching node.
//   - S7d (query): [Start] gets the query variable and the parser tables.
package ir

import (
	"fmt"
	"strings"

	"elan/ref"
)

// SortID is an index in [IR.Sorts].
type SortID = int

// OpID is an index in [IR.Ops].
type OpID = int

// FamilyID is an index in [IR.Families].
type FamilyID = int

// RuleID is an index in [Op.Rules].
type RuleID = int

// VarID is an index in [Rule.Vars].
type VarID = int

// LabelID is an index in [IR.Labels].
type LabelID = int

// StratID is an index in [IR.Strategies].
type StratID = int

// IR is a whole program, restricted to what the start term needs.
type IR struct {
	// Program is the name of the program (the .lgi file without extension).
	Program  string
	Sorts    []Sort
	Families []Family
	Ops      []Op
	// Labels are the reachable labels, in the order of the rule names of
	// the .ref file.
	Labels []Label
	// Strategies are the reachable named strategies, in the order of the
	// strategy names of the .ref file.
	Strategies []Strategy
	Start      Start
}

// Label is the labelled rules of one label (a localised rule name of the
// .ref file), in source order.
type Label struct {
	// Name is the localised name (r1:nat/m!GL).
	Name string
	// Fun is the name of the function.
	Fun string
	// Sort is the sort of the rules.
	Sort SortID
	// Rules are the rules; each has one left-hand side pattern, the whole
	// term.
	Rules []Rule
	// Skipped are the rules of the source not compiled, with the reason
	// (comments).
	Skipped []string
	// Tree is the matching automaton (one column: the term).
	Tree *Decision
	// RulesDet: every rule gives at most one result (its wheres use
	// deterministic strategies only).
	RulesDet bool
}

// Strategy is a named strategy (a STRATEGY definition).
type Strategy struct {
	// Name is the name in the .ref file (go:nat/m).
	Name string
	// Fun is the name of the function.
	Fun string
	// Sort is the sort of the terms it applies to (and of its results).
	Sort SortID
	// Body is the sequence s1 ; s2 ; ...
	Body []Strat
	// Det: it gives at most one result ([IR.SeqDet]).
	Det bool
}

// Choice is how a choice keeps the results of its alternatives.
type Choice int

const (
	// Dk (dk, DK): all results of all alternatives, in order.
	Dk Choice = iota
	// Dc (dc, DC, first): all results of the first alternative that has
	// one (for rules: the first rule that has one).
	Dc
	// One (one, ONE, first one, dc one): the first result.
	One
)

func (c Choice) String() string { return [...]string{"dk", "dc", "one"}[c] }

// StratKind is the kind of a [Strat].
type StratKind int

const (
	// StId is id.
	StId StratKind = iota
	// StFail is fail.
	StFail
	// StRules is dk/dc/one(l1, ..., ln) over labels.
	StRules
	// StStrats is DK/DC/ONE(s1, ..., sn) over sequences.
	StStrats
	// StRepeat is repeat*(s): the terms reached by s on which s fails,
	// depth first.
	StRepeat
	// StIterate is iterate*(s): the term, then iterate*(s) of each result
	// of s.
	StIterate
	// StCall is a call of a named strategy.
	StCall
)

// Strat is an elementary strategy (one element of a sequence).
type Strat struct {
	Kind StratKind
	// Choice of StRules and StStrats.
	Choice Choice
	// Labels of StRules.
	Labels []LabelID
	// Alts of StStrats.
	Alts [][]Strat
	// Body of StRepeat and StIterate.
	Body []Strat
	// Call is the strategy of StCall.
	Call StratID
}

// Sort is a reachable sort.
type Sort struct {
	// Index is the index in the Sorts table of the .ref file.
	Index ref.SortIndex
	// Name is the ELAN name (nat, list[int], builtinInt).
	Name string
	// Family is the builtin family of the sort, -1 for a sort of terms.
	Family FamilyID
	// Variants are, for a sort of terms, the operators that can be at the
	// top of its normal forms: its constructors and its defined operators
	// that can be stuck, in operator order.
	Variants []OpID
}

// BuiltinKind is the native type of a builtin family.
type BuiltinKind int

const (
	// KindInt is builtinInt: int32 (32-bit wrap).
	KindInt BuiltinKind = iota
	// KindBool is bool.
	KindBool
	// KindStr is builtinString: a Go string of bytes (C strings).
	KindStr
)

func (k BuiltinKind) String() string {
	return [...]string{"int", "bool", "string"}[k]
}

// Family is a builtin sort and the sorts whose only constructor is a
// transparent injection of it.
type Family struct {
	Kind BuiltinKind
	// Base is the builtin sort.
	Base SortID
	// Members are all the sorts of the family, the base first.
	Members []SortID
	// Stuck are the operators whose stuck terms are terms of a member (a
	// defined operator without a rule for some values, a builtin operation
	// on a stuck argument). When there is one, the family is boxed: its
	// values are elanrt.Terms (boxed values or stuck terms); else native.
	Stuck []OpID
}

// Boxed tells whether the terms of the family are elanrt.Terms.
func (f *Family) Boxed() bool { return len(f.Stuck) > 0 }

// OpKind is what an operator does.
type OpKind int

const (
	// Constructor is a free constructor (no rule): it builds the term.
	Constructor OpKind = iota
	// Injection is the injection of a family member's builtin sort
	// (`@ : (builtinInt) int`): the identity.
	Injection
	// BoolConst is true or false (symbols 1 and 0).
	BoolConst
	// Builtin is a builtin operation (the `code n` of a declaration).
	Builtin
	// Defined is an operator with unlabelled rules.
	Defined
)

func (k OpKind) String() string {
	return [...]string{"constructor", "injection", "boolean constant", "builtin", "defined"}[k]
}

// Op is an operator: a function symbol with one profile.
type Op struct {
	Symbol ref.SymbolCode
	// Name is a readable base name (from the mixfix form): plus, s, inj.
	Name string
	// Fun is the name of the normalisation function (Name_Symbol, unique).
	Fun string
	// Variant tells whether a term with this operator at the top can be a
	// normal form (a constructor, or an operator that can be stuck): the
	// operator then has a runtime symbol.
	Variant bool
	Args    []SortID
	Result  SortID
	// Print is the printing form: the last grammar rule of the symbol (the
	// operator's own declaration if that rule has another arity).
	Print []PrintItem
	// Decl is the ELAN declaration, for comments: `plus(@,@) : (nat nat) nat`.
	Decl string
	Kind OpKind
	// Bool is the value of a BoolConst.
	Bool bool
	// Builtin is the operation of a Builtin.
	Builtin BuiltinOp
	// Total: no term with this operator at the top is a normal form (always
	// true for constructors, injections and booleans: they are values).
	Total bool
	// Rules are the rules of a Defined operator, in source order.
	Rules []Rule
	// Skipped are the rules of the source not compiled, with the reason
	// (for comments).
	Skipped []string
	// Tree is the decision tree of a Defined operator.
	Tree *Decision
	// Recursive: the function is on a cycle of the call graph (it calls
	// itself, directly or not), so it can recurse as deep as the data: the
	// generated function guards its depth.
	Recursive bool
}

// PrintItemKind is the kind of a [PrintItem].
type PrintItemKind int

const (
	// PArg is an argument.
	PArg PrintItemKind = iota
	// PIdent is an identifier token.
	PIdent
	// PChar is a one-character token.
	PChar
	// PNum is a number token.
	PNum
)

// PrintItem is a token of a printing form.
type PrintItem struct {
	Kind PrintItemKind
	// Arg is the argument position (PArg).
	Arg int
	// Text is the identifier (raw bytes of the .ref file).
	Text string
	// Char is the character (one byte, Latin-1).
	Char byte
	// Num is the number.
	Num int64
}

// BuiltinOp is a builtin operation (codes of src/interpreter/term/codes.h,
// semantics of rewrite/rtmisc.cc).
type BuiltinOp int

// The builtin operations supported.
const (
	IntAdd    BuiltinOp = iota // + on builtinInt (code 3)
	IntSub                     // - (4)
	IntMul                     // * (5)
	IntDiv                     // / (6)
	IntRem                     // % (27)
	IntAnd                     // & (28)
	IntOr                      // | (29)
	IntNeg                     // unary - (20)
	CmpEq                      // == on builtinInt or bool values (8)
	CmpNe                      // != (9)
	CmpLt                      // < (10)
	CmpLe                      // <= (11)
	CmpGt                      // > (12)
	CmpGe                      // >= (13)
	BoolAnd                    // and (21): a stuck argument counts as false
	BoolOr                     // or (22)
	BoolNot                    // not (24)
	TermEq                     // == on any sort (18): syntactic equality of normal forms
	TermNeq                    // != on any sort (19)
	StrLen                     // string_strlen (150)
	StrCat                     // string_strcat (151)
	StrIndex                   // s[i] (152)
	StrModif                   // s[i <- c] (153)
	StrSubstr                  // string_substr (154)
	StrSpn                     // string_strspn (156)
	StrCmp                     // string_strcmp (157)
	StrOfChar                  // string_string (158)
)

var builtinNames = [...]string{
	"IntAdd", "IntSub", "IntMul", "IntDiv", "IntRem", "IntAnd", "IntOr", "IntNeg",
	"CmpEq", "CmpNe", "CmpLt", "CmpLe", "CmpGt", "CmpGe",
	"BoolAnd", "BoolOr", "BoolNot", "TermEq", "TermNeq",
	"StrLen", "StrCat", "StrIndex", "StrModif", "StrSubstr", "StrSpn", "StrCmp", "StrOfChar",
}

func (b BuiltinOp) String() string { return builtinNames[b] }

var builtinCodes = map[int32]BuiltinOp{
	3: IntAdd, 4: IntSub, 5: IntMul, 6: IntDiv, 27: IntRem, 28: IntAnd, 29: IntOr, 20: IntNeg,
	8: CmpEq, 9: CmpNe, 10: CmpLt, 11: CmpLe, 12: CmpGt, 13: CmpGe,
	21: BoolAnd, 22: BoolOr, 24: BoolNot, 18: TermEq, 19: TermNeq,
	150: StrLen, 151: StrCat, 152: StrIndex, 153: StrModif, 154: StrSubstr,
	156: StrSpn, 157: StrCmp, 158: StrOfChar,
}

// BuiltinFromCode is the operation of a semantic code; false when it is not
// supported.
func BuiltinFromCode(code int32) (BuiltinOp, bool) {
	b, ok := builtinCodes[code]
	return b, ok
}

// NeedsValues tells whether the operation needs values (not stuck terms) as
// arguments; and, or, not and the term equalities accept any normal form.
func (b BuiltinOp) NeedsValues() bool {
	switch b {
	case BoolAnd, BoolOr, BoolNot, TermEq, TermNeq:
		return false
	}
	return true
}

// Partial tells whether the operation is undefined on some values (the
// term stays).
func (b BuiltinOp) Partial() bool {
	return b == StrIndex || b == StrModif || b == StrSubstr
}

// Rule is an unlabelled rule of a defined operator, ready for compilation:
// variables numbered, non-linear occurrences turned into conditions.
type Rule struct {
	// Text is the rule in ELAN syntax (variables named x0, x1, ...), for
	// comments.
	Text string
	Vars []Var
	// Lhs are the patterns of the arguments of the left-hand side.
	Lhs []Pattern
	// Conds are the conditions and local assignments, in order (the
	// non-linearity checks of the left-hand side first).
	Conds []Cond
	Rhs   Expr
}

// Unconditional tells whether the rule applies whenever its left-hand side
// matches (a local assignment `where x := () t` always succeeds).
func (r *Rule) Unconditional() bool {
	for _, c := range r.Conds {
		if c.Kind != CondLet {
			return false
		}
	}
	return true
}

// Var is a rule variable.
type Var struct {
	Name string
	Sort SortID
}

// PatternKind is the kind of a [Pattern].
type PatternKind int

const (
	// PatVar is a variable (each variable occurs once in the patterns of a
	// rule; repeated occurrences are CondSame conditions).
	PatVar PatternKind = iota
	// PatOp is an operator applied to patterns.
	PatOp
	// PatInt is a builtinInt literal.
	PatInt
	// PatStr is a builtinString literal.
	PatStr
)

// Pattern is a pattern of a left-hand side or of a where.
type Pattern struct {
	Kind PatternKind
	Var  VarID
	Op   OpID
	Args []Pattern
	Int  int32
	Str  string
}

// CondKind is the kind of a [Cond].
type CondKind int

const (
	// CondSame: non-linear left-hand side, the two variables are equal
	// (pointer equality of shared terms, value equality of natives).
	CondSame CondKind = iota
	// CondIf is `if t`: t normalises to true.
	CondIf
	// CondLet is `where x := () t`.
	CondLet
	// CondMatch is `where p := () t`: the normal form of t matches p.
	CondMatch
	// CondApply is `where p := (S) t` (p a variable for `where x := (S)
	// t`): each result of strategy S applied to the normal form of t that
	// matches p, in order (backtracking).
	CondApply
)

// Cond is a condition or local assignment of a rule.
type Cond struct {
	Kind CondKind
	// A, B are the variables of CondSame; A is the variable of CondLet.
	A, B VarID
	// Expr is the term of CondIf, CondLet, CondMatch, CondApply.
	Expr Expr
	// Pat is the pattern of CondMatch and CondApply.
	Pat Pattern
	// Strategy is the strategy of CondApply.
	Strategy StratID
}

// ExprKind is the kind of an [Expr].
type ExprKind int

// The kinds of expressions.
const (
	ExprVar ExprKind = iota
	ExprOp
	ExprInt
	ExprStr
)

// Expr is a term to build (right-hand side, condition, start term).
// Building an operator calls its normalisation function on the arguments'
// normal forms (innermost).
type Expr struct {
	Kind ExprKind
	Var  VarID
	Op   OpID
	Args []Expr
	Int  int32
	Str  string
}

// Occ is a position under the arguments of the function: the argument
// number, then the argument numbers of the subterms (an injection has one
// child, its argument, which is the same value).
type Occ []int

// DecisionKind is the kind of a [Decision] node.
type DecisionKind int

const (
	// Fail: no rule applies, the term is stuck.
	Fail DecisionKind = iota
	// Try: the left-hand side of Rule matches (its variables are at the
	// Bindings positions): check its conditions; apply it, or continue with
	// Next.
	Try
	// Switch tests the head of the term at Occ (of sort Sort).
	Switch
)

// Decision is the matching automaton of a defined operator (a decision
// tree, Maranget-style: a node tests the head of one position; each case
// keeps the rules compatible with its head in source order, so the first
// rule that matches and whose conditions hold is the first in source
// order).
type Decision struct {
	Kind     DecisionKind
	Rule     RuleID
	Bindings []Binding
	Next     *Decision
	Occ      Occ
	Sort     SortID
	Cases    []Case
	// Default is for the heads not listed; nil when the cases cover every
	// possible head of the sort.
	Default *Decision
}

// Binding is a variable of a rule and its position.
type Binding struct {
	Var VarID
	Occ Occ
}

// Case is a case of a Switch.
type Case struct {
	Head Head
	Then *Decision
}

// HeadKind is the kind of a [Head].
type HeadKind int

const (
	// HeadOp is an operator (a constructor, a stuck operator, an injection,
	// true/false); its children are at Occ ++ [i].
	HeadOp HeadKind = iota
	// HeadInt is a builtinInt literal.
	HeadInt
	// HeadStr is a builtinString literal.
	HeadStr
)

// Head is a head tested by a Switch.
type Head struct {
	Kind HeadKind
	Op   OpID
	Int  int32
	Str  string
}

// Start is the query's start term (`start with () t`, run by -noInput).
type Start struct {
	// QuerySort is the sort of the query (printed by the prompt).
	QuerySort string
	Sort      SortID
	Term      Expr
	// Strategy is the strategy of `start with (S) t`, applied to the
	// normal form of the term, each result printed; -1 for `()`.
	Strategy StratID
}

// FamilyOf is the family of sort s, nil for a sort of terms.
func (ir *IR) FamilyOf(s SortID) *Family {
	if f := ir.Sorts[s].Family; f >= 0 {
		return &ir.Families[f]
	}
	return nil
}

// Native returns the native kind of the terms of sort s, and false when
// they are elanrt.Terms (a sort of terms or a boxed family).
func (ir *IR) Native(s SortID) (BuiltinKind, bool) {
	f := ir.FamilyOf(s)
	if f == nil || f.Boxed() {
		return 0, false
	}
	return f.Kind, true
}

// ExprSort is the sort of an expression of a rule with variables vars.
func (ir *IR) ExprSort(e *Expr, vars []Var) SortID {
	switch e.Kind {
	case ExprVar:
		return vars[e.Var].Sort
	case ExprOp:
		return ir.Ops[e.Op].Result
	case ExprInt:
		return ir.builtinBase(KindInt)
	default:
		return ir.builtinBase(KindStr)
	}
}

func (ir *IR) builtinBase(k BuiltinKind) SortID {
	for _, f := range ir.Families {
		if f.Kind == k {
			return f.Base
		}
	}
	panic(fmt.Sprintf("no builtin sort for %v literals", k))
}

// StringText is a builtin string for the reader (comments, messages):
// quoted, its bytes as Latin-1 characters (ELAN sources are Latin-1),
// control bytes, " and \ escaped.
func StringText(s string) string {
	var b strings.Builder
	b.WriteByte('"')
	for i := 0; i < len(s); i++ {
		c := s[i]
		switch {
		case c == '"' || c == '\\':
			b.WriteByte('\\')
			b.WriteByte(c)
		case c < 0x20 || c == 0x7f:
			fmt.Fprintf(&b, "\\x%02x", c)
		case c >= 0x80:
			b.WriteRune(rune(c))
		default:
			b.WriteByte(c)
		}
	}
	b.WriteByte('"')
	return b.String()
}
