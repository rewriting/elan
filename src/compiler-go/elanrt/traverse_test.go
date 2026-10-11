package elanrt

import (
	"strings"
	"testing"
)

// Tests of the traversals (S7d task 6), on the symbols of termops_test.go:
// constants a, b, c, d, free f(@,@), g(@) (g(g(x)) => x by normG), the AC
// symbol U (c U c => d by normG).

// rulesOf is a strategy giving, for a term, the results listed for it in
// order (none for the others).
func rulesOf(m map[Term][]Term) Strat[Term] {
	return func(t Term, k K[Term]) bool {
		for _, r := range m[t] {
			if k(r) {
				return true
			}
		}
		return false
	}
}

// everywhere selects s for every argument of every symbol.
func everywhere(s Strat[Term]) Sel { return func(int32, int) Strat[Term] { return s } }

func formats(ts []Term) string {
	var v []string
	for _, t := range ts {
		v = append(v, Format(t))
	}
	return strings.Join(v, " | ")
}

func travResults(t Term, s Strat[Term]) string { return formats(AllResults(t, s)) }

// sAB: a gives b then c; nothing else applies.
var sAB = rulesOf(map[Term][]Term{ta: {tb, tc}})

func TestTAll(t *testing.T) {
	all := func(s Strat[Term]) Strat[Term] {
		return func(x Term, k K[Term]) bool { return TAll(x, k, normG, everywhere(s)) }
	}
	cases := []struct {
		name string
		t    Term
		s    Strat[Term]
		want string
	}{
		// every combination, the last argument varying fastest (REM)
		{"product", tf(ta, ta), sAB, "f(b,b) | f(b,c) | f(c,b) | f(c,c)"},
		{"fails on one argument", tf(ta, td), sAB, ""},
		{"constant", ta, sAB, "a"},
		{"int", MkInt(3), sAB, "3"},
		// the rebuilt term is normalised: g(g(c)) => c
		{"renormalised", tg(ta), rulesOf(map[Term][]Term{ta: {tg(tc)}}), "c"},
		// AC: each distinct element once, its result for all its
		// occurrences (REM: genericGetMult); flattened, sorted, normalised
		// (c U c => d)
		{"AC", tu(ta, ta, tb), rulesOf(map[Term][]Term{ta: {tb, tc}, tb: {ta}}), "a U b U b | a U d"},
		{"AC normalised", tu(ta, ta, tb), rulesOf(map[Term][]Term{ta: {tc}, tb: {tb}}), "b U d"},
		{"AC flattened", tu(ta, tb), rulesOf(map[Term][]Term{ta: {tu(tc, td)}, tb: {tb}}), "b U c U d"},
	}
	for _, c := range cases {
		if got := travResults(c.t, all(c.s)); got != c.want {
			t.Errorf("%s: tall on %s = [%s], want [%s]", c.name, Format(c.t), got, c.want)
		}
	}
}

func TestTOne(t *testing.T) {
	one := func(s Strat[Term]) Strat[Term] {
		return func(x Term, k K[Term]) bool { return TOne(x, k, normG, everywhere(s)) }
	}
	cases := []struct {
		name string
		t    Term
		s    Strat[Term]
		want string
	}{
		// each result of s on each argument, arguments in order (REM)
		{"each argument", tf(ta, ta), sAB, "f(b,a) | f(c,a) | f(a,b) | f(a,c)"},
		{"none", tf(td, td), sAB, ""},
		{"constant", ta, sAB, ""},
		{"renormalised", tg(ta), rulesOf(map[Term][]Term{ta: {tg(tc)}}), "c"},
		// AC: one occurrence of each distinct element, in order
		{"AC", tu(ta, ta, tb), rulesOf(map[Term][]Term{ta: {tb, tc}, tb: {tc}}), "a U b U b | a U b U c | a U a U c"},
		{"AC normalised", tu(ta, tc), rulesOf(map[Term][]Term{ta: {tc}}), "d"},
	}
	for _, c := range cases {
		if got := travResults(c.t, one(c.s)); got != c.want {
			t.Errorf("%s: tone on %s = [%s], want [%s]", c.name, Format(c.t), got, c.want)
		}
	}
	// the search stops when k does
	n := 0
	TOne(tf(ta, ta), func(Term) bool { n++; return true }, normG, everywhere(sAB))
	if n != 1 {
		t.Errorf("tone went on after its continuation stopped: %d results", n)
	}
}

func TestTSome(t *testing.T) {
	some := func(s Strat[Term]) Strat[Term] {
		return func(x Term, k K[Term]) bool { return TSome(x, k, normG, everywhere(s)) }
	}
	cases := []struct {
		name string
		t    Term
		s    Strat[Term]
		want string
	}{
		// per argument: each result of s, then the argument unchanged; not
		// all unchanged (REM)
		{"each subset", tf(ta, ta), sAB, "f(b,b) | f(b,c) | f(b,a) | f(c,b) | f(c,c) | f(c,a) | f(a,b) | f(a,c)"},
		{"one argument", tf(ta, td), sAB, "f(b,d) | f(c,d)"},
		{"none", tf(td, td), sAB, ""},
		{"constant", ta, sAB, ""},
		// AC: all the occurrences of a distinct element, or none of them
		{"AC", tu(ta, ta, tb), rulesOf(map[Term][]Term{ta: {tb, tc}}), "b U b U b | b U d"},
	}
	for _, c := range cases {
		if got := travResults(c.t, some(c.s)); got != c.want {
			t.Errorf("%s: tsome on %s = [%s], want [%s]", c.name, Format(c.t), got, c.want)
		}
	}
}

func TestNormInOut(t *testing.T) {
	// step: g(a) => b, a => c, d => a
	step := rulesOf(map[Term][]Term{tg(ta): {tb}, ta: {tc}, td: {ta}})
	var in, outStep Strat[Term]
	in = func(x Term, k K[Term]) bool { return k(NormIn(x, normG, everywhere(in), step)) }
	outStep = func(x Term, k K[Term]) bool { return OuterStep(x, k, normG, everywhere(outStep), step) }
	out := func(x Term) Term { return NormTop(x, outStep) }
	cases := []struct {
		t       Term
		in, out string
	}{
		// innermost: a => c first; outermost: g(a) => b first
		{tg(ta), "g(c)", "b"},
		{tf(ta, tg(td)), "f(c,g(c))", "f(c,b)"},
		// outermost: d => a below g, then g(a) at the top
		{tg(td), "g(c)", "b"},
		{tb, "b", "b"},
		// AC elements: d => a => c, then c U c => d => a => c
		{tu(td, tc), "c", "c"},
	}
	for _, c := range cases {
		if got := Format(NormIn(c.t, normG, everywhere(in), step)); got != c.in {
			t.Errorf("normin %s = %s, want %s", Format(c.t), got, c.in)
		}
		if got := Format(out(c.t)); got != c.out {
			t.Errorf("normout %s = %s, want %s", Format(c.t), got, c.out)
		}
	}
	// a native sort: the steps at the top only
	inc := func(n int32, k K[int32]) bool {
		if n < 5 {
			return k(n + 1)
		}
		return false
	}
	if got := NormTop(int32(2), inc); got != 5 {
		t.Errorf("NormTop(2) = %d, want 5", got)
	}
}
