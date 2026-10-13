package earley

import (
	"slices"
	"strconv"
	"strings"
)

// The term of a derivation: REM's semantic actions (esemact,
// earley/runtimeInit.cc) record the derivation in reverse order in
// earleyRes/earleyKind/earleyString, which computeTerm
// (runtime/termIn.c) reads back from the end.

// Kinds of the entries (earleyKind).
const (
	defaultRule = 0 // DEFAULTRULE: an operator (Res: its rule)
)

// EarleySize is EARLEYSIZE, the size of the derivation.
const EarleySize = 100000

// Tree is a derivation (earleyRes, earleyKind, earleyString, earleyPos),
// the rule of each operator entry kept (REM keeps its symbol only).
type Tree struct {
	res   []int32
	kind  []int32
	str   []string
	rule  []*grule
	pos   int
	last  struct{ ident, num int32 }
	lastS string
}

// at makes entry pos exist (the arrays of REM have EarleySize entries).
func (tr *Tree) at() {
	for len(tr.res) <= tr.pos {
		tr.res = append(tr.res, 0)
		tr.kind = append(tr.kind, 0)
		tr.str = append(tr.str, "")
		tr.rule = append(tr.rule, nil)
	}
}

// esemact records the semantic action of rule, lex being the lexem at
// the end of its derivation.
func (g *grammar) esemact(rule *grule, lex Lexem) {
	tr := g.tree
	if tr.pos == EarleySize {
		panic(&Fatal{Msg: "[esemact] Not enough memory...\n", Status: 255})
	}
	tr.at()
	num := rule.rulenumber
	if !rule.action {
		num = -1 // an operator
	}
	switch num {
	case identRule:
		tr.last.ident = lex.idval()
		tr.kind[tr.pos] = identRule
	case numRule:
		tr.last.num = lex.numval()
		tr.kind[tr.pos] = numRule
	case numToTerm:
		tr.res[tr.pos] = tr.last.num
		tr.pos++
	case identToTerm:
		tr.res[tr.pos] = tr.last.ident
		tr.pos++
	case stringRule:
		tr.lastS = g.f.stringval(lex)
		tr.kind[tr.pos] = stringRule
		tr.str[tr.pos] = tr.lastS
	case stringToTerm:
		if lex.isRealString() {
			tr.lastS = g.f.stringval(lex)
		}
		tr.str[tr.pos] = tr.lastS
		tr.pos++
	default:
		tr.kind[tr.pos] = defaultRule
		tr.res[tr.pos] = rule.rulenumber
		tr.rule[tr.pos] = rule
		tr.pos++
	}
}

// Builder builds the terms of a query: Op is the normal form of the
// operator of grammar rule r (an index in Tables.Rules) applied to args;
// AC that of the AC operator of rule r applied to the elements elems
// (flattened); Int, Ident and Str the values read.
type Builder[T any] interface {
	Op(r int, args []T) T
	AC(r int, elems []T) T
	Int(n int32) T
	Ident(i int32) T
	Str(s string) T
	// IsAC tells whether the operator of rule r is AC.
	IsAC(r int) bool
}

// raw is a term as computeTerm builds it, before its normalisation.
type raw struct {
	rule *grule
	kind int32
	res  int32
	str  string
	args []*raw
}

// computeTerm reads the term ending at *ppos.
func (tr *Tree) computeTerm(ppos *int) *raw {
	*ppos--
	p := *ppos
	if p < 0 {
		panic(&Fatal{Msg: "computeTerm: the derivation is empty\n", Status: 1})
	}
	t := &raw{rule: tr.rule[p], kind: tr.kind[p], res: tr.res[p], str: tr.str[p]}
	switch t.kind {
	case defaultRule:
		if t.rule == nil {
			panic(&Fatal{Msg: "computeTerm: an operator without its rule\n", Status: 1})
		}
		for i := 0; i < t.rule.arity; i++ {
			t.args = append(t.args, tr.computeTerm(ppos))
		}
	case numRule, identRule, stringRule:
	default:
		panic(&Fatal{Msg: "computeTerm: case not matched\n", Status: 1})
	}
	return t
}

// Build returns the normal form of the term of the derivation tr (REM:
// computeTerm, then normalise): built bottom-up, each operator by b, an
// AC operator applied to its arguments flattened (term_add_onf_term: an
// argument of the same symbol gives its arguments).
func Build[T any](tr *Tree, b Builder[T]) T {
	pos := tr.pos
	return build(tr.computeTerm(&pos), b)
}

func build[T any](t *raw, b Builder[T]) T {
	switch t.kind {
	case numRule:
		return b.Int(t.res)
	case identRule:
		return b.Ident(t.res)
	case stringRule:
		return b.Str(t.str)
	}
	if t.rule.index >= 0 && b.IsAC(t.rule.index) {
		// the distinct elements (modulo AC), each built once and given
		// as many times as it occurs: REM's term_add_onf_term merges
		// equal elements (a multiplicity), whose normalisation then
		// counts its rewrite steps once
		var elems []T
		built := map[string]T{}
		for _, a := range flatAC(t) {
			k := rawKey(a, b.IsAC)
			x, ok := built[k]
			if !ok {
				x = build(a, b)
				built[k] = x
			}
			elems = append(elems, x)
		}
		return b.AC(t.rule.index, elems)
	}
	args := make([]T, len(t.args))
	for i, a := range t.args {
		args[i] = build(a, b)
	}
	return b.Op(t.rule.index, args)
}

// flatAC returns the arguments of the AC term t, those of the same
// operator flattened.
func flatAC(t *raw) []*raw {
	var elems []*raw
	var flatten func(x *raw)
	flatten = func(x *raw) {
		for _, a := range x.args {
			if a.kind == defaultRule && a.rule.rulenumber == x.rule.rulenumber {
				flatten(a)
			} else {
				elems = append(elems, a)
			}
		}
	}
	flatten(t)
	return elems
}

// rawKey is a key of the term t, the same for terms equal modulo AC (the
// elements of an AC operator sorted); isAC tells whether the operator of
// a rule is AC.
func rawKey(t *raw, isAC func(r int) bool) string {
	switch t.kind {
	case numRule:
		return "n" + strconv.Itoa(int(t.res))
	case identRule:
		return "i" + strconv.Itoa(int(t.res))
	case stringRule:
		return "s" + strconv.Quote(t.str)
	}
	head := "o" + strconv.Itoa(int(t.rule.rulenumber)) + "/" + strconv.Itoa(t.rule.index)
	var keys []string
	if t.rule.index >= 0 && isAC(t.rule.index) {
		for _, a := range flatAC(t) {
			keys = append(keys, rawKey(a, isAC))
		}
		slices.Sort(keys)
		return head + "[" + strings.Join(keys, ",") + "]"
	}
	for _, a := range t.args {
		keys = append(keys, rawKey(a, isAC))
	}
	return head + "(" + strings.Join(keys, ",") + ")"
}
