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

// The order is a fixed function of the structure: the same in every run
// and every build (the hash uses the symbols' names, not their codes).
func TestCanonicalOrderIsFixed(t *testing.T) {
	var e []Term
	for i := range 8 {
		e = append(e, atom(i))
	}
	e = append(e, nat(2), MkStr("x"), MkInt(1<<20), True)
	x := MkAC(symU, Flatten(symU, e...))
	// (a change of the hash function changes this order: outputs of AC
	// programs change with it)
	const want = "p(3)U true U1048576 U p(1)U p(7)U p(0)U p(4)U s(s(o))U p(2)U p(6)U p(5)Ux"
	if got := Format(x); got != want {
		t.Fatalf("got  %s\nwant %s", got, want)
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

// Compare is a total order consistent with equality, also on terms whose
// hashes are equal (compared by structure).
func TestCompareOnEqualHashes(t *testing.T) {
	a, b := atom(1), atom(2)
	// as if the two terms had the same hash: compareStructure
	if c := compareStructure(a, b); c == 0 || c != -compareStructure(b, a) {
		t.Fatal("p(1), p(2) by structure")
	}
	if compareStructure(MkStr("a"), MkStr("b")) >= 0 || compareStructure(Mk0(symO), Mk0(symNil)) <= 0 ||
		compareStructure(MkInt(1), MkInt(2)) >= 0 || compareStructure(Mk0(symNil), atom(1)) >= 0 {
		t.Fatal("strings by bytes, ints by value, symbols by name")
	}
	if Compare(a, a) != 0 || Compare(a, b) == 0 || Compare(a, b) != -Compare(b, a) {
		t.Fatal("Compare")
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
