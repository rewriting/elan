package elanrt

import (
	"cmp"
	"slices"
)

// AC multisets (spec G3, G5). A term whose top symbol is AC holds its
// flattened arguments (no argument has the same AC symbol on top), at least
// two, duplicates kept, sorted by [Compare]. Matching code works on these
// element slices ("multisets"); the helpers below never modify their
// arguments and return fresh slices.
//
// The order is history-independent: it depends only on the structure of
// the terms (their stored structural hash, then a structural comparison
// for the rare equal hashes), never on creation order or addresses. With a
// garbage collector, a term that is collected and rebuilt is a new node; an
// order by creation would make outputs and step counts depend on GC timing.

// Compare is the canonical total order of terms: by structural hash, then
// (two different terms with the same hash) by structure. Compare(a, b) == 0
// if and only if a == b.
func Compare(a, b Term) int {
	if a == b {
		return 0
	}
	if a.Hash != b.Hash {
		return cmp.Compare(a.Hash, b.Hash)
	}
	return compareStructure(a, b)
}

// compareStructure orders two different terms with the same hash: symbol
// name and arity, payload (the bytes for strings), then the arguments from
// left to right. Recursion only follows equal hashes, which differ almost
// always at the first level.
func compareStructure(a, b Term) int {
	sa, sb := &Syms[a.Sym], &Syms[b.Sym]
	if c := cmp.Compare(sa.Name, sb.Name); c != 0 {
		return c
	}
	if c := cmp.Compare(len(a.Args), len(b.Args)); c != 0 {
		return c
	}
	if a.Sym == SymString && b.Sym == SymString {
		if c := cmp.Compare(StrOf(a), StrOf(b)); c != 0 {
			return c
		}
	} else if c := cmp.Compare(a.Int, b.Int); c != 0 {
		return c
	}
	for i := range a.Args {
		if c := Compare(a.Args[i], b.Args[i]); c != 0 {
			return c
		}
	}
	// two symbols with the same name and arity (different sorts)
	return cmp.Compare(a.Sym, b.Sym)
}

// SortAC sorts elements in canonical order, in place.
func SortAC(e []Term) { slices.SortFunc(e, Compare) }

// Elems returns the multiset of t seen as an argument of the AC symbol
// sym: its elements if t has sym on top, else the one element t.
func Elems(sym int32, t Term) []Term {
	if t.Sym == sym {
		return t.Args
	}
	return []Term{t}
}

// Flatten returns the sorted multiset of sym(xs...): the elements of the
// arguments that have sym on top, and the other arguments.
func Flatten(sym int32, xs ...Term) []Term {
	n := 0
	for _, x := range xs {
		if x.Sym == sym {
			n += len(x.Args)
		} else {
			n++
		}
	}
	e := make([]Term, 0, n)
	for _, x := range xs {
		if x.Sym == sym {
			e = append(e, x.Args...)
		} else {
			e = append(e, x)
		}
	}
	SortAC(e)
	return e
}

// Merge returns the union of two sorted multisets (sorted, duplicates
// kept).
func Merge(a, b []Term) []Term {
	out := make([]Term, 0, len(a)+len(b))
	i, j := 0, 0
	for i < len(a) && j < len(b) {
		if Compare(a[i], b[j]) <= 0 {
			out = append(out, a[i])
			i++
		} else {
			out = append(out, b[j])
			j++
		}
	}
	out = append(out, a[i:]...)
	return append(out, b[j:]...)
}

// Insert returns the sorted multiset e with one more occurrence of x.
func Insert(e []Term, x Term) []Term { return Merge(e, []Term{x}) }

// MkAC returns the AC term sym of the sorted multiset e: the element
// itself if there is one, the shared node sym(e...) if there are more, nil
// if e is empty (the caller decides: a neutral element, or no term). e is
// not kept.
func MkAC(sym int32, e []Term) Term {
	switch len(e) {
	case 0:
		return nil
	case 1:
		return e[0]
	}
	return mk(sym, 0, e)
}

// MkAC2 returns the canonical AC term sym(a, b) (flattened, sorted).
func MkAC2(sym int32, a, b Term) Term { return MkAC(sym, Merge(Elems(sym, a), Elems(sym, b))) }

// Find returns the position of the first occurrence of x in the sorted
// multiset e, or -1.
func Find(e []Term, x Term) int {
	i, found := slices.BinarySearchFunc(e, x, Compare)
	if !found {
		return -1
	}
	return i
}

// Mult returns the multiplicity of the element at position i, counted
// from i (its full multiplicity when i is its first occurrence). The
// distinct elements of e are visited with
//
//	for i := 0; i < len(e); i += Mult(e, i) { ... e[i] ... }
func Mult(e []Term, i int) int {
	m := 1
	for i+m < len(e) && e[i+m] == e[i] {
		m++
	}
	return m
}

// Count returns the multiplicity of x in e.
func Count(e []Term, x Term) int {
	if i := Find(e, x); i >= 0 {
		return Mult(e, i)
	}
	return 0
}

// FirstOcc tells whether position j is the first occurrence of its
// element among the positions not in removed (to enumerate the distinct
// remaining elements when the positions removed are taken by a pattern).
func FirstOcc(e []Term, j int, removed []int) bool {
	if slices.Contains(removed, j) {
		return false
	}
	for p := j - 1; p >= 0 && e[p] == e[j]; p-- {
		if !slices.Contains(removed, p) {
			return false
		}
	}
	return true
}

// Rest returns the elements of e not at the positions removed (sorted).
func Rest(e []Term, removed ...int) []Term {
	r := make([]Term, 0, len(e))
	for i, x := range e {
		if !slices.Contains(removed, i) {
			r = append(r, x)
		}
	}
	return r
}

// RemoveOne returns e without one occurrence of x, and false if x is not
// in e.
func RemoveOne(e []Term, x Term) ([]Term, bool) {
	i := Find(e, x)
	if i < 0 {
		return nil, false
	}
	return Rest(e, i), true
}
