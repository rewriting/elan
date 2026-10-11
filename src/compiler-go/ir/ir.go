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
// # Strategy terms (S7b task 2, as elanc-rs's S6b task 2)
//
// Strategies built at run time ([S]t, `where x := [S]t`, defined
// strategies with parameters `[] app(s) => s`, if-then-else, the labels as
// strategy constants) are terms of the strategy sorts <X> (<X->X>) and
// StrList[X,X] of strsig[X,Y]/strat[X] (elanlib/strategy). The
// interpreter has no builtin evaluation of them: the codes of their
// operators (DS_*, -179..-190, -205..-208, term/codes.h) are never used by
// its rewriting (standardreduction, rewrite/rtmisc.cc); the defstrat word
// of their grammar rules is only exported for REM. Their semantics is
// ordinary ELAN code, which the .ref file contains in full: the strategy
// eval:X/strat[X] (a `where x := [S]t` is exported as `where x := (eval)
// [S]t`), the labelled rules of strat[X] ([DK], [DC], [IF1], [CONST]...)
// and the rules the interpreter generates when it loads the program
// (load/msemact.cc): [LAB] per label used as a strategy constant, [DSTR]
// per defined strategy, [FSYM] congruences. So the strategy sorts are
// ordinary sorts of terms, their operators constructors ([@]@ included: a
// normal form keeps [S]t), and eval an ordinary named strategy: the
// results and their order are the interpreter's by construction. The
// labelled rules whose left-hand side has an operator that no term of the
// program builds are not compiled (the traversal [TONE] when no tone(S)
// is built, the [FSYM] congruences of operators never applied to
// strategies).
//
// `call "name":X` (Meta_strat[X]) has a builtin evaluation: the META
// strategy of set_of (Meta_apply[X], used by setof of strapply[X])
// interprets a meta_apply term ([MetaOp]), whose first argument, a term of
// sort Strategy[X], denotes a strategy (term2strategy, meta/meta.cc). Its
// sorts are ordinary sorts too; the function of META on a sort
// ([MetaApply]) and the evaluator of the Strategy[X] terms ([MetaEval])
// are generated. The strategy of a call is chosen as the interpreter does
// it when it evaluates the term (Strategyname_to_ref_index, commit
// 93ec493): by its name and the sort of the term, known at compile time
// (X), not by the index of the export (any sort: the 2004 behaviour that
// REM keeps). Labels named by strings in Strategy[X] terms are S7d.
//
// # Extension points
//
//   - S7d: AC operators on builtin sorts.
//
// # AC operators (S7c tasks 1 and 2)
//
// An AC operator ([Op.AC]) builds canonical terms: its function flattens
// and sorts its arguments (rt.MkAC2: REM's order, see the S7c plan) and
// applies its unlabelled rules to the result, which is the only column of
// its decision tree. An AC pattern ([PatAC]) in a left-hand side or in a
// where is replaced, for the decision tree, by a fresh variable; a
// [CondAC] condition then matches the term bound to it (each solution in
// turn, the conditions after it backtracking into the matching): the
// element patterns one element each, in source order, each over the
// subject's elements in canonical order (each distinct choice once), then
// the variables of the AC level ([ACVar]). An unlabelled rule with its AC
// operator at the top is applied modulo an extension ([Pattern.Ext]), as
// the interpreter does with the extension rules it exports.
//
// Task 2: the patterns REM matches with Eker's AC matcher (the patterns
// of wheres that contain an AC operator, and the variables of an AC level
// of three items or more with several variables, [ACAbs]) are matched by
// rt.Eker ([CondEker], [EkerPat]) in the oracles' order; x^n U y and
// x^n U y^m as REM's Diophantine code ([ACVar]).
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
	// MetaApplies are the functions of the META strategy, one per sort it
	// is applied to.
	MetaApplies []MetaApply
	// MetaEvals are the evaluators of the Strategy[X] terms, one per X.
	MetaEvals []MetaEval
	Start     Start
	// Idents is the program's table of identifiers (the Identifiers
	// table of the .ref file) when its terms can have identifiers (sort
	// ident reached): rt.InitIdents.
	Idents []Ident
}

// Ident is an entry of the table of identifiers.
type Ident struct {
	Index int32
	Name  string
}

// MetaOp is what the META strategy does with an operator of Meta_apply[X]
// (strbuiltins, rewrite/stateofexecution.cc); elsewhere the operator is a
// constructor.
type MetaOp int

const (
	// MetaNone: not a meta_apply operator.
	MetaNone MetaOp = iota
	// MetaList is meta_apply(s, t.l, from, n) (NEW_META_APPLY, code 128,
	// set_of): one result, the list of the results of s on t, from number
	// from (0 first), n of them (all when n is 0).
	MetaList
	// MetaAll is meta_apply(s, t) (META_APPL, 129): every result of s on t.
	MetaAll
	// MetaNth is meta_apply(s, t, n) (META_APPLY, 130): result number n of
	// s on t (0 first), if any.
	MetaNth
)

// MetaApply is the function of the META strategy on terms of one sort:
// `func(t T, k rt.K[T]) bool` over the meta_apply terms of that sort whose
// Strategy[X] has an evaluator; any other term is the interpreter's fatal
// error "[META-built] META strategy over wrong term".
type MetaApply struct {
	Fun  string
	Sort SortID
}

// MetaEval is the evaluation of the terms of sort Strategy[X]
// (Meta_strat[X]) by META: term2strategy of meta/meta.cc, compiled to
// `func(s rt.Term, t T, typed bool, k rt.K[T]) bool` applying the strategy
// denoted by s to the term t of sort X: a sequence (@ 141, @ @ 142) of id
// (143), call "name":X (144), dc/dk of strategies (137, 138 over 131,
// 132), repeat (139) and iterate (140). typed: the sort of the term is
// known (false for a number or a string, which the interpreter cannot
// type); it chooses the strategy of a call.
type MetaEval struct {
	// Fun evaluates the Strategy[X] terms, StrategFun the Strateg[X] terms
	// (its elements), ListFun the Strategies[X] terms (the strategies of a
	// dc or dk).
	Fun, StrategFun, ListFun string
	// Strategy is Strategy[X]; Strateg and Strategies are Strateg[X] and
	// Strategies[X], -1 when not reached.
	Strategy, Strateg, Strategies SortID
	// Term is X.
	Term SortID
	// Calls are the call "name":X terms of the program, by name.
	Calls []MetaCall
}

// MetaCall is a call "name":X and the strategies it applies.
type MetaCall struct {
	Name string
	// Typed is the strategy for a term whose sort is known (X).
	Typed StratID
	// Untyped is, for a term whose sort is unknown (X builtinInt or
	// builtinString, a number or a string), the strategy; -1 when there is
	// none (X has a symbol) or when it is refused (UntypedWhy: a strategy
	// of another sort; the program stops with status 2 if it happens).
	Untyped    StratID
	UntypedWhy string
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
	// StMeta is META (the strategy of `where x := (META) t`): t is a
	// meta_apply term ([MetaOp]); its results.
	StMeta
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
	// Meta is the function of StMeta (an index in [IR.MetaApplies]).
	Meta int
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
	// KindIdent is ident: int32, the index of the identifier in the
	// program's table of identifiers (rt.IdentIndex, S7d task 3).
	KindIdent
)

func (k BuiltinKind) String() string {
	return [...]string{"int", "bool", "string", "ident"}[k]
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
	// ACFun is, for an AC operator, the name of its normalisation
	// function over a multiset (Fun_ac, unique).
	ACFun string
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
	// Meta is the evaluation of a meta_apply operator by META.
	Meta MetaOp
	// AC: the operator is associative-commutative (infos 2): its terms are
	// flattened and canonically sorted multisets of two or more elements.
	AC bool
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
	IntAdd        BuiltinOp = iota // + on builtinInt (code 3)
	IntSub                         // - (4)
	IntMul                         // * (5)
	IntDiv                         // / (6)
	IntRem                         // % (27)
	IntAnd                         // & (28)
	IntOr                          // | (29)
	IntNeg                         // unary - (20)
	CmpEq                          // == on builtinInt or bool values (8)
	CmpNe                          // != (9)
	CmpLt                          // < (10)
	CmpLe                          // <= (11)
	CmpGt                          // > (12)
	CmpGe                          // >= (13)
	BoolAnd                        // and (21): a stuck argument counts as false
	BoolOr                         // or (22)
	BoolNot                        // not (24)
	TermEq                         // == on any sort (18): syntactic equality of normal forms
	TermNeq                        // != on any sort (19)
	StrLen                         // string_strlen (150)
	StrCat                         // string_strcat (151)
	StrIndex                       // s[i] (152)
	StrModif                       // s[i <- c] (153)
	StrSubstr                      // string_substr (154)
	StrSpn                         // string_strspn (156)
	StrCmp                         // string_strcmp (157)
	StrOfChar                      // string_string (158)
	BoolToInt                      // btoi (25): 1 or 0 (no oracle: the S7d plan, Task 1)
	IntToBool                      // itob (26): n != 0
	TermReplace                    // replace t1 by t2 in t3 on any sorts (16): rt.Replace
	TermOccurs                     // occurs t1 in t2 on any sorts (17): rt.Occurs
	TermMatch                      // builtinSyntacticMatching (191): rt.SyntacticMatch
	TermUnify                      // builtinSyntacticUnification (175): rt.SyntacticUnify
	IdentEq                        // == on ident (14)
	IdentNe                        // != on ident (15)
	IdentToString                  // ident2string (177): the name
)

var builtinNames = [...]string{
	"IntAdd", "IntSub", "IntMul", "IntDiv", "IntRem", "IntAnd", "IntOr", "IntNeg",
	"CmpEq", "CmpNe", "CmpLt", "CmpLe", "CmpGt", "CmpGe",
	"BoolAnd", "BoolOr", "BoolNot", "TermEq", "TermNeq",
	"StrLen", "StrCat", "StrIndex", "StrModif", "StrSubstr", "StrSpn", "StrCmp", "StrOfChar",
	"BoolToInt", "IntToBool", "TermReplace", "TermOccurs", "TermMatch", "TermUnify",
	"IdentEq", "IdentNe", "IdentToString",
}

func (b BuiltinOp) String() string { return builtinNames[b] }

var builtinCodes = map[int32]BuiltinOp{
	3: IntAdd, 4: IntSub, 5: IntMul, 6: IntDiv, 27: IntRem, 28: IntAnd, 29: IntOr, 20: IntNeg,
	8: CmpEq, 9: CmpNe, 10: CmpLt, 11: CmpLe, 12: CmpGt, 13: CmpGe,
	21: BoolAnd, 22: BoolOr, 24: BoolNot, 18: TermEq, 19: TermNeq,
	150: StrLen, 151: StrCat, 152: StrIndex, 153: StrModif, 154: StrSubstr,
	156: StrSpn, 157: StrCmp, 158: StrOfChar,
	25: BoolToInt, 26: IntToBool, 16: TermReplace, 17: TermOccurs, 191: TermMatch, 175: TermUnify,
	14: IdentEq, 15: IdentNe, 177: IdentToString,
}

// BuiltinFromCode is the operation of a semantic code; false when it is not
// supported.
func BuiltinFromCode(code int32) (BuiltinOp, bool) {
	b, ok := builtinCodes[code]
	return b, ok
}

// NeedsValues tells whether the operation needs values (not stuck terms) as
// arguments; and, or, not, the term equalities and the operations on
// terms ([BuiltinOp.OnTerms]) accept any normal form.
func (b BuiltinOp) NeedsValues() bool {
	switch b {
	case BoolAnd, BoolOr, BoolNot, TermEq, TermNeq:
		return false
	}
	return !b.OnTerms()
}

// OnTerms tells whether the operation works on terms of any sorts, its
// result too (replace, occurs, syntactic matching and unification):
// declared on sorts of terms, it is compiled (a value operation on sorts
// of terms is a constructor).
func (b BuiltinOp) OnTerms() bool {
	switch b {
	case TermReplace, TermOccurs, TermMatch, TermUnify:
		return true
	}
	return false
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
	// PatIdent is an identifier literal (Int: its index).
	PatIdent
	// PatAC is an AC pattern (only in [CondAC]): the AC operator Op, the
	// element patterns Args (nested patterns of the same operator
	// flattened; each matches one element), the variables of the AC level
	// Vars, and Ext.
	PatAC
)

// ACVar is a variable of the AC level of an AC pattern: it takes a
// non-empty sub-multiset, Mult times (x U x: Mult 2). With two variables
// (the shapes REM matches with its Diophantine code, Term.java
// genDioMatching), the first is REM's "list_x": both of multiplicity 1, it
// takes a counter over the elements ([rt.Split]); x^n U y (n > 1), x takes
// the elements from the largest multiset down ([rt.Down] with n), y the
// rest (non-empty); x^n U y^m (n, m > 1: REM abstracts the variable that
// comes first in its order, here the second, `x^n :=() X`), the first
// takes them from the largest down (Down with m), the rest X (non-empty)
// is n copies of the second.
type ACVar struct {
	Var  VarID
	Mult int
}

// ACAbs is the part of an AC pattern that REM matches with Eker's matcher
// (RewriteRule.java transformation3_1: several variables at an AC level
// of three items or more): a fresh variable Var takes the elements left to
// it (the last variable of the AC level), then a [CondEker] condition
// matches it against the abstracted items (EkerVars: the variables it
// binds, to be bound before the conditions that read them).
type ACAbs struct {
	Var      VarID
	EkerVars []VarID
}

// Pattern is a pattern of a left-hand side or of a where.
type Pattern struct {
	Kind PatternKind
	Var  VarID
	Op   OpID
	Args []Pattern
	Int  int32
	Str  string
	// Vars are the variables of the AC level of a PatAC (at most two;
	// with Abs, at most one, before Abs.Var).
	Vars []ACVar
	// Abs is the abstracted part of a PatAC (nil if none).
	Abs *ACAbs
	// Ext: the pattern is the left-hand side of an unlabelled rule of its
	// AC operator, applied modulo an extension E (the elements left over,
	// possibly none): the result is then op(rhs, E) normalised.
	Ext bool
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
	// CondAC: the term of variable A matches the AC pattern Pat; each
	// solution in turn (the conditions after it backtrack into the
	// matching).
	CondAC
	// CondEker: the term of variable A matches the pattern Eker with
	// Eker's AC matcher (rt.Eker), each solution in turn, in the order of
	// the oracles; EkerVars[i] is the variable of the pattern's variable
	// number i.
	CondEker
	// CondChoose is `choose try b1 try b2 ... end` (S7d task 2): the
	// branches (Branches, lists of conditions) are alternatives of the
	// rest of the rule, as both oracles run them (the interpreter's
	// TRYCHOICE, rewrite/stateofexecution.cc; REM's Choice.java): branch 1
	// then the conditions after end and the right-hand side, each of its
	// solutions in turn; when they are exhausted (or fail), branch 2 and
	// the rest, and so on. So a condition after end that fails tries the
	// next branch, a labelled rule gives the results of every branch in
	// order (dk), and a normalisation keeps the first. A variable bound
	// by a branch is the same variable in every branch (one VarID); after
	// end, only those bound by every branch are bound. Under dc, REM stops
	// a labelled rule after the first branch that gave a result; the
	// interpreter, followed here, does not (the S7d plan, Task 2).
	CondChoose
)

// Cond is a condition or local assignment of a rule.
type Cond struct {
	Kind CondKind
	// A, B are the variables of CondSame; A is the variable of CondLet
	// and the subject of CondAC.
	A, B VarID
	// Expr is the term of CondIf, CondLet, CondMatch, CondApply.
	Expr Expr
	// Pat is the pattern of CondMatch, CondApply and CondAC.
	Pat Pattern
	// Strategy is the strategy of CondApply.
	Strategy StratID
	// Eker, EkerVars: the pattern of CondEker and its variables.
	Eker     *EkerPat
	EkerVars []VarID
	// Branches are the branches of CondChoose.
	Branches [][]Cond
}

// Walk calls f on c and on the conditions of its branches (CondChoose),
// depth first.
func (c *Cond) Walk(f func(*Cond)) {
	f(c)
	for _, b := range c.Branches {
		for i := range b {
			b[i].Walk(f)
		}
	}
}

// EkerPat is a pattern for Eker's matcher ([CondEker]), in REM's form:
// the elements of an AC pattern are its distinct items in REM's order
// (Term.java orderedNormalForm: ints, then variables by number, then
// symbols by code, then their arguments), with their multiplicities; the
// variables are numbered depth first in that order
// (rgenTermForACMatcher), from 0.
type EkerPat struct {
	Kind PatternKind // PatVar (Var: its number), PatOp, PatAC, PatInt, PatStr
	Var  int
	Op   OpID
	Args []EkerPat
	// Mults are the multiplicities of the elements (Args) of a PatAC.
	Mults []int
	Int   int32
	Str   string
}

// ExprKind is the kind of an [Expr].
type ExprKind int

// The kinds of expressions.
const (
	ExprVar ExprKind = iota
	ExprOp
	ExprInt
	ExprStr
	// ExprIdent is an identifier literal (Int: its index).
	ExprIdent
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
	// HeadIdent is an identifier literal (Int: its index).
	HeadIdent
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
	// Term is the start term; when it uses the query (UsesQuery), Vars
	// has one variable, the query (ExprVar 0).
	Term      Expr
	UsesQuery bool
	Vars      []Var
	// Strategy is the strategy of `start with (S) t`, applied to the
	// normal form of the term, each result printed; -1 for `()`.
	Strategy StratID
	// Query is the query parser: the query is read from the standard
	// input (without -noInput, S7d task 5).
	Query *Query
}

// Query are the tables of the query parser (elanrt/earley): the grammar
// and, per grammar rule, how the term of its operator is built.
type Query struct {
	// Sort is the sort of the query (the .ref sort index: the start
	// symbol of the parser); SortID its sort in the IR.
	Sort   int32
	SortID SortID
	// Grammar are the grammar rules (REM's gram[]), Rules how each one
	// is built.
	Grammar []GrammarRule
	Rules   []QueryRule
	// SortNames are the names of the sorts of the grammar (warnings).
	SortNames map[int32]string
	// InternIdent, InternInt, InternString are the sorts of the standard
	// rules (identifier, number and string lexems).
	InternIdent, InternInt, InternString int32
}

// QueryKind says how the term of a grammar rule is built.
type QueryKind uint8

const (
	// QueryNone: the parser never builds it (a rule of a sort that a
	// query cannot contain, or a number, identifier or string: built by
	// the parser).
	QueryNone QueryKind = iota
	// QueryOp: the function of the operator Op.
	QueryOp
	// QueryUmin: -n of the number read (INTCONSTUMIN, code 79).
	QueryUmin
	// QueryUnsupported: the operator is not compiled (Why); a query that
	// contains it stops the program (status 2).
	QueryUnsupported
)

// QueryRule is how the term of a grammar rule is built.
type QueryRule struct {
	Kind QueryKind
	Op   OpID
	Why  string
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
	case ExprIdent:
		return ir.builtinBase(KindIdent)
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
