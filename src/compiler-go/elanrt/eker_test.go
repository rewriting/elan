package elanrt

import (
	"fmt"
	"slices"
	"strings"
	"testing"
)

// Symbols of the Eker tests, codes in declaration order: a < b < c < d < g.
var (
	ekA = NewSym("a", 0)
	ekB = NewSym("b", 0)
	ekC = NewSym("c", 0)
	ekD = NewSym("d", 0)
	ekG = NewSym("g", 1)
	ekH = NewSym("h", 2)
)

func ekU(xs ...Term) Term { return MkAC(symU, Flatten(symU, xs...)) }

func ekNorm(sym int32, e []Term) Term { return MkAC(sym, e) }

// ekSolutions runs the matcher and formats each solution as the values of
// the variables 0..n-1 separated by "; ".
func ekSolutions(p *EkPat, n int, subject Term) []string {
	var out []string
	for m := NewEker(p, n, subject); m.Next(); {
		var vs []string
		for v := range n {
			x := m.Value(v, ekNorm)
			if x == nil {
				vs = append(vs, "?")
			} else {
				vs = append(vs, Format(x))
			}
		}
		out = append(out, strings.Join(vs, "; "))
	}
	return out
}

func checkSolutions(t *testing.T, name string, got []string, want ...string) {
	t.Helper()
	if !slices.Equal(got, want) {
		t.Errorf("%s:\ngot  %q\nwant %q", name, got, want)
	}
	seen := map[string]bool{}
	for _, s := range got {
		if seen[s] {
			t.Errorf("%s: solution %s twice", name, s)
		}
		seen[s] = true
	}
}

func uItems(ps ...*EkPat) []EkItem {
	var it []EkItem
	for _, p := range ps {
		it = append(it, EkItem{p, 1})
	}
	return it
}

// The orders observed with both oracles (the S7c plan, Task 2 findings):
// where F(x, y) :=() a U b U c (variables numbered as REM does: x 0, y 1).
func TestEkerTwoVariables(t *testing.T) {
	a, b, c := Mk0(ekA), Mk0(ekB), Mk0(ekC)
	p := EkAC(symU, uItems(EkVar(0), EkVar(1))...)
	checkSolutions(t, "x U y", ekSolutions(p, 2, ekU(a, b, c)),
		"a U b; c", "a U c; b", "b U c; a", "a; b U c", "b; a U c", "c; a U b")
	// multiplicities: each distinct solution once
	checkSolutions(t, "x U y on a U a U b", ekSolutions(p, 2, ekU(a, a, b)),
		"a U a; b", "a U b; a", "a; a U b", "b; a U a")
	if s := ekSolutions(p, 2, a); len(s) != 0 {
		t.Errorf("one element: %q", s)
	}
}

func TestEkerThreeVariables(t *testing.T) {
	a, b, c, d := Mk0(ekA), Mk0(ekB), Mk0(ekC), Mk0(ekD)
	p := EkAC(symU, uItems(EkVar(0), EkVar(1), EkVar(2))...)
	got := ekSolutions(p, 3, ekU(a, b, c, d))
	// the oracles' order (pw3 in the findings): the last variable first,
	// by increasing size, each size in advance_select's order
	want := []string{
		"a U b; c; d", "a U c; b; d", "b U c; a; d", "a; b U c; d", "b; a U c; d", "c; a U b; d",
		"a U b; d; c", "a U d; b; c", "b U d; a; c", "a; b U d; c", "b; a U d; c", "d; a U b; c",
		"a U c; d; b", "a U d; c; b", "c U d; a; b", "a; c U d; b", "c; a U d; b", "d; a U c; b",
		"b U c; d; a", "b U d; c; a", "c U d; b; a", "b; c U d; a", "c; b U d; a", "d; b U c; a",
		"a; b; c U d", "b; a; c U d", "a; c; b U d", "c; a; b U d", "a; d; b U c", "d; a; b U c",
		"b; c; a U d", "c; b; a U d", "b; d; a U c", "d; b; a U c", "c; d; a U b", "d; c; a U b",
	}
	checkSolutions(t, "x U y U z", got, want...)
}

func TestEkerElementsAndRepeated(t *testing.T) {
	a, b, c := Mk0(ekA), Mk0(ekB), Mk0(ekC)
	ga, gb := Mk1(ekG, a), Mk1(ekG, b)
	// g(x) U y on g(a) U g(b) U g(a) U c (pwg)
	p := EkAC(symU, uItems(EkOp(ekG, EkVar(0)), EkVar(1))...)
	checkSolutions(t, "g(x) U y", ekSolutions(p, 2, ekU(ga, gb, ga, c)),
		"b; c U g(a)U g(a)", "a; c U g(a)U g(b)")
	// x U x U y on a U a U b U b U c (pwxx)
	q := EkAC(symU, EkItem{EkVar(0), 2}, EkItem{EkVar(1), 1})
	checkSolutions(t, "x U x U y", ekSolutions(q, 2, ekU(a, a, b, b, c)),
		"b; a U a U c", "a; b U b U c", "a U b; c")
	// x U x alone: the whole multiset halved, once
	r := EkAC(symU, EkItem{EkVar(0), 2})
	checkSolutions(t, "x U x", ekSolutions(r, 1, ekU(a, a, b, b)), "a U b")
	checkSolutions(t, "x U x odd", ekSolutions(r, 1, ekU(a, a, b)))
	// non-linear: h(x, x U y) on h(a, a U b)
	s := EkOp(ekH, EkVar(0), EkAC(symU, uItems(EkVar(0), EkVar(1))...))
	checkSolutions(t, "h(x, x U y)", ekSolutions(s, 2, Mk2(ekH, a, ekU(a, b))), "a; b")
	checkSolutions(t, "h(x, x U y) none", ekSolutions(s, 2, Mk2(ekH, c, ekU(a, b))))
	// a constant and a rest
	u := EkAC(symU, uItems(EkOp(ekB), EkVar(0))...)
	checkSolutions(t, "b U x", ekSolutions(u, 1, ekU(a, b, b, c)), "a U b U c")
	// no variable: exact match only
	w := EkAC(symU, uItems(EkOp(ekA), EkOp(ekB))...)
	checkSolutions(t, "a U b", ekSolutions(w, 0, ekU(a, b)), "")
	checkSolutions(t, "a U b vs a U b U c", ekSolutions(w, 0, ekU(a, b, c)))
}

// Ints are ordered by their tagged value (0, 1, 2, ..., then the negative
// ints): wpatternAC4 (g(y) U g(y) on g(1) U g(2) U g(3) U g(1)).
func TestEkerInts(t *testing.T) {
	g := func(i int32) Term { return Mk1(ekG, MkInt(i)) }
	p := EkAC(symU, EkItem{EkOp(ekG, EkVar(0)), 2}, EkItem{EkVar(1), 1})
	checkSolutions(t, "g(y) U g(y) U z", ekSolutions(p, 2, ekU(g(1), g(2), g(3), g(1))), "1; g(2)U g(3)")
	if c := ekCmp(toEker(MkInt(-1)), toEker(MkInt(5))); c <= 0 {
		t.Error("a negative int comes after the positive ones")
	}
	q := EkAC(symU, uItems(EkVar(0), EkVar(1))...)
	checkSolutions(t, "ints", ekSolutions(q, 2, ekU(MkInt(-1), MkInt(2))), "2; -1", "-1; 2")
}

// Every solution once, on random multisets: the number of solutions of
// x U y is the number of non-empty proper sub-multisets.
func TestEkerCountsSolutions(t *testing.T) {
	elems := []Term{Mk0(ekA), Mk0(ekB), Mk0(ekC), Mk0(ekD)}
	p := EkAC(symU, uItems(EkVar(0), EkVar(1))...)
	for mask := 0; mask < 81; mask++ {
		var xs []Term
		prod, m := 1, mask
		for _, e := range elems {
			k := m % 3
			m /= 3
			for range k {
				xs = append(xs, e)
			}
			prod *= k + 1
		}
		if len(xs) < 2 {
			continue
		}
		got := ekSolutions(p, 2, ekU(xs...))
		if len(got) != prod-2 {
			t.Errorf("%s: %d solutions, want %d", fmt.Sprint(Format(ekU(xs...))), len(got), prod-2)
		}
		checkSolutions(t, Format(ekU(xs...)), got, got...)
	}
}

// A pinned variable (EkPin): the solutions are those of the matching
// without it where the variable has the pinned value, in the same order.
func TestEkerPinnedSameOrder(t *testing.T) {
	a, b, c, d := Mk0(ekA), Mk0(ekB), Mk0(ekC), Mk0(ekD)
	ga, gb := Mk1(ekG, a), Mk1(ekG, b)
	cases := []struct {
		name    string
		p       *EkPat
		n       int
		subject Term
	}{
		{"x U y", EkAC(symU, uItems(EkVar(0), EkVar(1))...), 2, ekU(a, b, c, d)},
		{"x U y U z", EkAC(symU, uItems(EkVar(0), EkVar(1), EkVar(2))...), 3, ekU(a, b, c, d)},
		{"x U y U z, repeated", EkAC(symU, uItems(EkVar(0), EkVar(1), EkVar(2))...), 3, ekU(a, a, b, c, c)},
		{"x^2 U y U z", EkAC(symU, EkItem{EkVar(0), 2}, EkItem{EkVar(1), 1}, EkItem{EkVar(2), 1}), 3, ekU(a, a, b, b, c, d)},
		{"g(x) U y U z", EkAC(symU, uItems(EkOp(ekG, EkVar(0)), EkVar(1), EkVar(2))...), 3, ekU(ga, gb, ga, c, d)},
		{"x U y U z U w", EkAC(symU, uItems(EkVar(0), EkVar(1), EkVar(2), EkVar(3))...), 4, ekU(a, b, c, d, ga)},
	}
	for _, k := range cases {
		var full [][]Term
		for m := NewEker(k.p, k.n, k.subject); m.Next(); {
			var vs []Term
			for v := range k.n {
				vs = append(vs, m.Value(v, ekNorm))
			}
			full = append(full, vs)
		}
		if len(full) == 0 {
			t.Fatalf("%s: no solution", k.name)
		}
		format := func(vs []Term) string {
			var s []string
			for _, x := range vs {
				s = append(s, Format(x))
			}
			return strings.Join(s, "; ")
		}
		for pv := range k.n {
			for pv2 := pv; pv2 < k.n; pv2++ {
				seen := map[string]bool{}
				for _, sol := range full {
					key := Format(sol[pv]) + "|" + Format(sol[pv2])
					if seen[key] {
						continue
					}
					seen[key] = true
					var want, got []string
					for _, s := range full {
						if Format(s[pv]) == Format(sol[pv]) && Format(s[pv2]) == Format(sol[pv2]) {
							want = append(want, format(s))
						}
					}
					pins := []EkPin{{pv, sol[pv]}}
					if pv2 != pv {
						pins = append(pins, EkPin{pv2, sol[pv2]})
					}
					for m := NewEker(k.p, k.n, k.subject, pins...); m.Next(); {
						var vs []Term
						for v := range k.n {
							vs = append(vs, m.Value(v, ekNorm))
						}
						got = append(got, format(vs))
					}
					if !slices.Equal(got, want) {
						t.Errorf("%s, pinned %v:\ngot  %q\nwant %q", k.name, pins, got, want)
					}
				}
			}
		}
	}
}
