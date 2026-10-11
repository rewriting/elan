package elanrt

import (
	"fmt"
	"math/rand"
	"runtime"
	"slices"
	"strings"
	"testing"
)

func TestCanonicalFormIsIndependentOfGroupingAndOrder(t *testing.T) {
	a, b, c := atom(1), atom(2), atom(3)
	x := and(and(a, b), c)
	if x != and(a, and(b, c)) || x != and(c, and(b, a)) {
		t.Fatal("AC terms built in different ways are different nodes")
	}
	if len(x.Args) != 3 || !slices.IsSortedFunc(x.Args, Compare) {
		t.Fatalf("not flattened and sorted: %s", Format(x))
	}
	// flattening only applies to the AC symbol itself
	if n := and(Mk1(symNot, and(a, b)), c); len(n.Args) != 2 {
		t.Fatal("flattened through not")
	}
	if MkAC(symAnd, nil) != nil || MkAC(symAnd, []Term{a}) != a {
		t.Fatal("MkAC of 0 or 1 element")
	}
}

// The order is REM's (Gterm_cmp): ints by value, strings, then by symbol
// code (declaration order here), then the arguments; an AC term as a
// multiset (distinct elements, then multiplicities).
func TestCanonicalOrderIsREMs(t *testing.T) {
	var e []Term
	for i := range 8 {
		e = append(e, atom(7-i))
	}
	e = append(e, nat(2), MkStr("x"), MkInt(1<<20), True, Mk0(symO), Mk0(symNil))
	x := MkAC(symU, Flatten(symU, e...))
	const want = "1048576Ux U true U o U s(s(o))U nil U p(0)U p(1)U p(2)U p(3)U p(4)U p(5)U p(6)U p(7)"
	if got := Format(x); got != want {
		t.Fatalf("got  %s\nwant %s", got, want)
	}
	// multisets: a prefix first, then by the first different distinct
	// element, then by its multiplicity
	a, b := atom(1), atom(2)
	ab, aab, abb, abc := and(a, b), and(and(a, a), b), and(and(a, b), b), and(and(a, b), atom(3))
	for _, c := range [][2]Term{{ab, abc}, {abb, aab}, {abc, aab}, {ab, and(a, a)}} {
		if Compare(c[0], c[1]) >= 0 || Compare(c[1], c[0]) <= 0 {
			t.Errorf("%s < %s expected", Format(c[0]), Format(c[1]))
		}
	}
}

func TestDuplicatesAndHelpers(t *testing.T) {
	a, b, c := atom(10), atom(20), atom(30)
	x := and(and(a, b), and(c, a))
	e := x.Args
	if len(e) != 4 || Count(e, a) != 2 || Count(e, c) != 1 || Count(e, atom(99)) != 0 {
		t.Fatalf("multiplicities of %s", Format(x))
	}
	ia := Find(e, a)
	if ia < 0 || e[ia] != a || (ia > 0 && e[ia-1] == a) || Mult(e, ia) != 2 {
		t.Fatal("Find returns the first occurrence")
	}
	var distinct []Term
	for i := 0; i < len(e); i += Mult(e, i) {
		distinct = append(distinct, e[i])
	}
	if len(distinct) != 3 {
		t.Fatalf("distinct elements: %d", len(distinct))
	}
	// Flatten and Merge agree with the term
	if f := Flatten(symAnd, and(a, b), c, a); !slices.Equal(f, e) {
		t.Fatal("Flatten")
	}
	if m := Merge(Elems(symAnd, and(a, b)), Elems(symAnd, and(c, a))); !slices.Equal(m, e) {
		t.Fatal("Merge")
	}
	if i := Insert(Insert(Flatten(symAnd, c, b), a), a); !slices.Equal(i, e) {
		t.Fatal("Insert")
	}
	// removing elements
	r, ok := RemoveOne(e, a)
	if !ok || len(r) != 3 || Count(r, a) != 1 || MkAC(symAnd, r) != and(and(a, b), c) {
		t.Fatal("RemoveOne")
	}
	if _, ok := RemoveOne(e, atom(99)); ok {
		t.Fatal("RemoveOne of an absent element")
	}
	if rest := Rest(e, 0, 1, 2); len(rest) != 1 {
		t.Fatal("Rest")
	}
	// first occurrences among the positions not removed
	if !FirstOcc(e, ia, nil) || FirstOcc(e, ia+1, nil) || !FirstOcc(e, ia+1, []int{ia}) || FirstOcc(e, ia, []int{ia}) {
		t.Fatal("FirstOcc")
	}
}

// Compare is a total order consistent with equality.
func TestCompareIsTotal(t *testing.T) {
	ts := []Term{atom(1), atom(2), MkStr("a"), MkStr("b"), Mk0(symO), Mk0(symNil), MkInt(1), MkInt(2),
		nat(3), and(atom(1), atom(2)), and(atom(1), atom(1)), True, False}
	for _, a := range ts {
		for _, b := range ts {
			if c := Compare(a, b); (c == 0) != (a == b) || c != -Compare(b, a) {
				t.Fatalf("Compare(%s, %s) = %d", Format(a), Format(b), c)
			}
		}
	}
	if Compare(MkStr("a"), MkStr("b")) >= 0 || Compare(Mk0(symO), Mk0(symNil)) >= 0 ||
		Compare(MkInt(1), MkInt(2)) >= 0 || Compare(MkInt(9), MkStr("a")) >= 0 || Compare(False, True) >= 0 {
		t.Fatal("strings by bytes, ints by value, symbols by code")
	}
}

// acOrder builds the AC term U(p(0), ..., p(n-1), s^i(o) ...) in an order
// given by seed, with garbage and collections in between (so that elements
// are collected and rebuilt), and returns it printed.
func acOrder(seed int64, n int) string {
	r := rand.New(rand.NewSource(seed))
	idx := r.Perm(n)
	var acc Term
	for k, i := range idx {
		var x Term
		if i%2 == 0 {
			x = atom(i)
		} else {
			x = nat(i % 7)
		}
		if acc == nil {
			acc = x
		} else {
			acc = MkAC2(symU, acc, x)
		}
		if k%50 == 0 {
			_ = list(1000) // garbage
			runtime.GC()
		}
	}
	return Format(acc)
}

func init() {
	helpers["acorder"] = func() {
		// same seeds in every process
		for seed := int64(1); seed <= 3; seed++ {
			fmt.Println(acOrder(seed, 300))
		}
	}
}

// The AC order is the same whatever the creation order, the collections in
// between and the GC setting (separate processes, GOGC=off, 1, 100, 400).
func TestACOrderIsHistoryIndependent(t *testing.T) {
	want := acOrder(1, 300)
	for seed := int64(2); seed <= 4; seed++ {
		if got := acOrder(seed, 300); got != want {
			t.Fatalf("creation order %d changes the AC order", seed)
		}
	}
	var outs []string
	for _, gogc := range []string{"off", "1", "100", "400"} {
		out, errs, st := runHelper(t, "acorder", []string{"GOGC=" + gogc})
		if st != 0 {
			t.Fatalf("GOGC=%s: status %d: %s", gogc, st, errs)
		}
		outs = append(outs, out)
	}
	wantAll := strings.Repeat(want+"\n", 3)
	for i, o := range outs {
		if o != wantAll {
			t.Fatalf("run %d: AC order differs between processes or GC settings", i)
		}
	}
}

func fmtMS(e []Term) string {
	if len(e) == 0 {
		return "-"
	}
	return Format(MkAC(symU, slices.Clone(e)))
}

// The variables of an AC pattern: x U y in REM's order (observed: on
// a U b U c, x = c, b, b U c, a, a U c, a U b; on a U a U b: b, a, a U b,
// a U a), each solution once.
func TestSplitOrder(t *testing.T) {
	a, b, c := Mk0(symO), Mk0(symNil), nat(1) // codes: o < s < nil
	var got []string
	for s := NewSplit(Flatten(symU, c, b, a)); s.Next(); {
		got = append(got, fmtMS(s.X())+"|"+fmtMS(s.Y()))
	}
	want := "nil|o U s(o),s(o)|o U nil,s(o)U nil|o,o|s(o)U nil,o U nil|s(o),o U s(o)|nil"
	if g := strings.Join(got, ","); g != want {
		t.Errorf("got  %s\nwant %s", g, want)
	}
	got = nil
	for s := NewSplit(Flatten(symU, a, b, a)); s.Next(); {
		got = append(got, fmtMS(s.X()))
	}
	if g := strings.Join(got, ","); g != "nil,o,o U nil,o U o" {
		t.Errorf("multiplicities: %s", g)
	}
	if s := NewSplit([]Term{a}); s.Next() {
		t.Error("one element: no solution")
	}
}

func TestDownAndDiv(t *testing.T) {
	a, b := Mk0(symO), Mk0(symNil)
	if s := NewDown(Flatten(symU, a, b), 2); s.Next() {
		t.Error("x U x U E on a U b: no solution")
	}
	var got []string
	for s := NewDown(Flatten(symU, a, a, b), 1); s.Next(); {
		got = append(got, fmtMS(s.X())+"|"+fmtMS(s.Rest()))
	}
	if g := strings.Join(got, ","); g != "o U o U nil|-,o U o|nil,o U nil|o,o|o U nil,nil|o U o" {
		t.Errorf("m=1: %s", g)
	}
	got = nil
	for s := NewDown(Flatten(symU, a, a, a, b, b), 2); s.Next(); {
		got = append(got, fmtMS(s.X())+"|"+fmtMS(s.Rest()))
	}
	if g := strings.Join(got, ","); g != "o U nil|o,o|o U nil U nil,nil|o U o U o" {
		t.Errorf("m=2: %s", g)
	}
	if x, ok := DivAC(Flatten(symU, a, a, b, b), 2); !ok || fmtMS(x) != "o U nil" {
		t.Error("DivAC")
	}
	if _, ok := DivAC(Flatten(symU, a, a, b), 2); ok {
		t.Error("DivAC of an odd multiplicity")
	}
}
