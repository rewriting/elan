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
// The order is REM's (Gterm_cmp, src/compiler/runtime/termBase.c): the
// order of the AC arguments is visible in the output (an AC term prints as
// its elements in that order, right-nested) and in the order of the
// solutions of AC matching, so elanc-go keeps the order of the compiled
// programs of the reference (spec S7, G3 as amended by S7c). It is
// history-independent: it depends only on the structure of the terms (the
// symbols' codes in the .ref file, ints by value, strings by bytes), never
// on creation order or addresses (with a garbage collector, a term that is
// collected and rebuilt is a new node).

// Compare is the canonical total order of terms (REM's Gterm_cmp): ints by
// value, then strings by bytes, before the other terms; those by the code
// of their symbol in the .ref file ([Symbol.Code]), then their arguments
// from left to right, the elements of an AC term as a multiset (each
// distinct element, then its multiplicity; a prefix first). Compare(a, b)
// == 0 if and only if Eq(a, b).
func Compare(a, b Term) int {
	if a == b {
		return 0
	}
	ka, kb := kindRank(a), kindRank(b)
	if ka != kb {
		return cmp.Compare(ka, kb)
	}
	switch ka {
	case 0:
		return cmp.Compare(a.Int, b.Int)
	case 1:
		return cmp.Compare(StrOf(a), StrOf(b))
	}
	sa, sb := &Syms[a.Sym], &Syms[b.Sym]
	if sa.Code != sb.Code {
		return cmp.Compare(sa.Code, sb.Code)
	}
	if sa.AC && sb.AC {
		if c := compareMultisets(a.Args, b.Args); c != 0 {
			return c
		}
	} else {
		for i := 0; i < len(a.Args) && i < len(b.Args); i++ {
			if c := Compare(a.Args[i], b.Args[i]); c != 0 {
				return c
			}
		}
		if c := cmp.Compare(len(a.Args), len(b.Args)); c != 0 {
			return c
		}
	}
	// two symbols with the same code (a builtin code shared by several
	// profiles): by runtime code
	return cmp.Compare(a.Sym, b.Sym)
}

// kindRank orders ints, then strings, then the other terms.
func kindRank(t Term) int {
	switch t.Sym {
	case SymInt:
		return 0
	case SymString:
		return 1
	}
	return 2
}

// compareMultisets compares two sorted multisets as REM compares its AC
// lists of (element, multiplicity) pairs: the first different distinct
// element, else the first different multiplicity; a prefix first.
func compareMultisets(a, b []Term) int {
	i, j := 0, 0
	for {
		if i == len(a) {
			if j == len(b) {
				return 0
			}
			return -1
		}
		if j == len(b) {
			return 1
		}
		if c := Compare(a[i], b[j]); c != 0 {
			return c
		}
		ma, mb := Mult(a, i), Mult(b, j)
		if ma != mb {
			return cmp.Compare(ma, mb)
		}
		i, j = i+ma, j+mb
	}
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
func Insert(e []Term, x Term) []Term {
	// after the elements <= x, as Merge(e, {x})
	i, _ := slices.BinarySearchFunc(e, x, func(a, x Term) int {
		if Compare(a, x) <= 0 {
			return -1
		}
		return 1
	})
	out := make([]Term, len(e)+1)
	copy(out, e[:i])
	out[i] = x
	copy(out[i+1:], e[i:])
	return out
}

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
func MkAC2(sym int32, a, b Term) Term { return MkAC(sym, MergeAC(sym, a, b)) }

// MergeAC returns the sorted multiset of sym(a, b): the union of the
// elements of a and b (seen as arguments of sym), a fresh slice.
func MergeAC(sym int32, a, b Term) []Term {
	if a.Sym == sym {
		if b.Sym == sym {
			return Merge(a.Args, b.Args)
		}
		return Insert(a.Args, b)
	}
	if b.Sym == sym {
		return Insert(b.Args, a)
	}
	if Compare(a, b) <= 0 {
		return []Term{a, b}
	}
	return []Term{b, a}
}

// ACTerm returns the AC term sym(e...) (e sorted, two elements or more;
// kept): its shared node if it exists, else a transient node, outside the
// unique table. The normalisation function of an AC symbol matches its
// rules on it, then returns [Intern] of it if no rule applies: a term
// that rewrites never pays for a node of the table (a weak pointer, ~0.25
// us). A transient node never leaves that function.
func ACTerm(sym int32, e []Term) Term {
	h := nodeHash(sym, 0, e)
	if n := lookup(h, sym, 0, e); n != nil {
		Hits++
		return n
	}
	return &Node{Sym: sym, Hash: h, Args: e, tmp: true}
}

// Normal tells whether t is an AC term known to be a normal form: its
// normalisation function, given it, returns it at once.
func Normal(t Term) bool { return t.nf }

// NormalForm returns the shared node of the AC term t that no rule of its
// symbol rewrites ([Intern]); steps is [Steps] when its normalisation
// function started on it. If no step was counted since then (no
// condition or value of a variable rewrote anything: the attempts are
// pure, and give the same answer the next time), the node is marked
// ([Normal]): the next normalisations of the term skip the rules, with
// the same results and the same count of steps.
func NormalForm(t Term, steps int64) Term {
	t = Intern(t)
	if Steps == steps {
		t.nf = true
	}
	return t
}

// Intern returns the shared node of t: t itself unless it is a transient
// node of [ACTerm], which then joins the unique table (or gives way to
// the equal node built meanwhile).
func Intern(t Term) Term {
	if !t.tmp {
		return t
	}
	if n := lookup(t.Hash, t.Sym, t.Int, t.Args); n != nil {
		Hits++
		return n
	}
	t.tmp = false
	Created++
	insert(t)
	return t
}

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
	for i+m < len(e) && Eq(e[i+m], e[i]) {
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
	for p := j - 1; p >= 0 && Eq(e[p], e[j]); p-- {
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

// ---------------------------------------------------------------------------
// AC-level variables (the elements not taken by the element patterns of an
// AC pattern, "the rest"; spec S7c task 1)

// distinct returns the distinct elements of the sorted multiset e and
// their multiplicities, with room for extra ints after the
// multiplicities (ints[:len(d)] are the multiplicities, ints holds
// (1+extra) x len(d) ints: one allocation for the counters of the
// enumerators).
func distinct(e []Term, extra int) (d []Term, ints []int) {
	k := 0
	for i := range e {
		if i == 0 || !Eq(e[i], e[i-1]) {
			k++
		}
	}
	d = make([]Term, 0, k)
	ints = make([]int, k*(1+extra))
	for i := 0; i < len(e); i++ {
		if i == 0 || !Eq(e[i], e[i-1]) {
			d = append(d, e[i])
		}
		ints[len(d)-1]++
	}
	return d, ints
}

// DivAC returns the multiset x such that m copies of x make e (a variable
// of multiplicity m, x U ... U x, takes the whole rest e), and false when
// there is none (e empty, or a multiplicity not divisible by m).
func DivAC(e []Term, m int) ([]Term, bool) {
	if len(e) == 0 || len(e)%m != 0 {
		return nil, false
	}
	if m == 1 {
		return e, true
	}
	x := make([]Term, 0, len(e)/m)
	for i := 0; i < len(e); {
		k := Mult(e, i)
		if k%m != 0 {
			return nil, false
		}
		for range k / m {
			x = append(x, e[i])
		}
		i += k
	}
	return x, true
}

// Split enumerates the solutions of two variables x U y (each once, both
// non-empty) against a multiset, in REM's order (next_minimal_extract,
// src/compiler/runtime/ac_tools.c): x takes a counter over the distinct
// elements in canonical order, each digit up to its multiplicity, the
// last element least significant, from 1 up to the whole multiset minus
// one; y takes the complement. On a U b U c: x = c, b, b U c, a, a U c,
// a U b.
//
//	for s := rt.NewSplit(e); s.Next(); { x, y := s.X(), s.Y() ... }
//
// The first solution (x the last element, y the others) is given without
// counting the distinct elements: a rule without conditions takes it.
type Split struct {
	e     []Term
	first bool // at the first solution, the counters not made
	d     []Term
	max   []int
	count []int
	n     int // elements in count
	total int
}

// NewSplit starts the enumeration over e (sorted).
func NewSplit(e []Term) Split { return Split{e: e, total: len(e)} }

// Next moves to the next solution; false at the end.
func (s *Split) Next() bool {
	if s.d == nil {
		if !s.first {
			s.first = true
			return s.total >= 2
		}
		// the counters, at the first solution
		s.first = false
		d, ints := distinct(s.e, 1)
		s.d, s.max, s.count = d, ints[:len(d)], ints[len(d):]
		s.count[len(d)-1], s.n = 1, 1
	}
	for i := len(s.count) - 1; i >= 0; i-- {
		if s.count[i] < s.max[i] {
			s.count[i]++
			s.n++
			return s.n < s.total
		}
		s.n -= s.count[i]
		s.count[i] = 0
	}
	return false
}

// X is the counter's part (sorted, non-empty; it may share e's array).
func (s *Split) X() []Term {
	if s.first {
		return s.e[len(s.e)-1:]
	}
	return s.part(false)
}

// Y is the complement (sorted, non-empty; it may share e's array).
func (s *Split) Y() []Term {
	if s.first {
		return s.e[:len(s.e)-1]
	}
	return s.part(true)
}

func (s *Split) part(complement bool) []Term {
	n := s.n
	if complement {
		n = s.total - s.n
	}
	out := make([]Term, 0, n)
	for i, x := range s.d {
		k := s.count[i]
		if complement {
			k = s.max[i] - k
		}
		for range k {
			out = append(out, x)
		}
	}
	return out
}

// Down enumerates the solutions of a variable of multiplicity m with an
// extension (x U ... U x U E against e, x non-empty, E possibly empty):
// from the largest x (each element's multiplicity divided by m) down, a
// counter over the distinct elements, the last one least significant (as
// REM's next_maximal_extract), so the first solution takes the most (with
// m = 1: x is the whole multiset, E empty).
//
//	for s := rt.NewDown(e, m); s.Next(); { x, rest := s.X(), s.Rest() ... }
//
// With m = 1, the first solution is given without counting the distinct
// elements (x is e): a rule without conditions takes it.
type Down struct {
	e       []Term
	first   bool // m = 1, at the first solution, the counters not made
	d       []Term
	mult    []int
	max     []int
	count   []int
	m       int
	started bool
}

// NewDown starts the enumeration over e (sorted).
func NewDown(e []Term, m int) Down {
	if m == 1 {
		return Down{e: e, m: 1}
	}
	if !hasMult(e, m) {
		return Down{started: true} // no solution
	}
	s := Down{m: m}
	s.counters(e)
	return s
}

// counters makes the distinct elements and the counters of e.
func (s *Down) counters(e []Term) {
	d, ints := distinct(e, 2)
	k := len(d)
	s.d, s.mult, s.max, s.count = d, ints[:k], ints[k:2*k], ints[2*k:]
	for i, n := range s.mult {
		s.max[i] = n / s.m
	}
}

// hasMult tells whether an element of the sorted multiset e occurs m
// times or more.
func hasMult(e []Term, m int) bool {
	run := 0
	for i := range e {
		if i > 0 && Eq(e[i], e[i-1]) {
			run++
		} else {
			run = 1
		}
		if run >= m {
			return true
		}
	}
	return false
}

// Next moves to the next solution; false at the end.
func (s *Down) Next() bool {
	if !s.started {
		s.started = true
		if s.m == 1 {
			s.first = true
			return len(s.e) > 0
		}
		copy(s.count, s.max)
	} else {
		if s.first {
			// the counters, at the first solution
			s.first = false
			s.counters(s.e)
			copy(s.count, s.max)
		}
		i := len(s.count) - 1
		for ; i >= 0; i-- {
			if s.count[i] > 0 {
				s.count[i]--
				break
			}
			s.count[i] = s.max[i]
		}
		if i < 0 {
			return false
		}
	}
	for _, k := range s.count {
		if k > 0 {
			return true
		}
	}
	return false // x would be empty: the end
}

// X is the variable's value (sorted, non-empty; it may be e itself).
func (s *Down) X() []Term {
	if s.first {
		return s.e
	}
	n := 0
	for _, k := range s.count {
		n += k
	}
	out := make([]Term, 0, n)
	for i, x := range s.d {
		for range s.count[i] {
			out = append(out, x)
		}
	}
	return out
}

// Rest is the extension E (sorted, possibly empty).
func (s *Down) Rest() []Term {
	if s.first {
		return nil
	}
	n := 0
	for i, k := range s.mult {
		n += k - s.m*s.count[i]
	}
	if n == 0 {
		return nil
	}
	out := make([]Term, 0, n)
	for i, x := range s.d {
		for range s.mult[i] - s.m*s.count[i] {
			out = append(out, x)
		}
	}
	return out
}

// NormAC returns the normal form of the AC term over the multiset e
// (non-empty, sorted; kept), f being the normalisation function of its
// symbol over a multiset of two elements or more (it applies the rules
// of the symbol): the element itself if there is one, else f(e). The
// elements left over by an AC match can make a reducible term (and(x, t)
// on and(a, a, t) with and(x, x) => x).
func NormAC(e []Term, f func(e []Term) Term) Term {
	if len(e) == 1 {
		return e[0]
	}
	return f(e)
}

// ApplyAC returns the normal form of the AC term sym(args...) (the args
// in normal form), normalised as a whole: f is the normalisation
// function of sym over a multiset, nil for a constructor.
func ApplyAC(sym int32, f func(e []Term) Term, args ...Term) Term {
	if f == nil {
		return MkAC(sym, Flatten(sym, args...))
	}
	return NormAC(Flatten(sym, args...), f)
}

// ExtendAC returns the result of a rule applied modulo an extension:
// sym(args..., rest...) normalised as a whole (args: the right-hand side,
// or its arguments when sym is at its top), the right-hand side itself
// when rest is empty and args is one term; f as for [ApplyAC].
func ExtendAC(sym int32, f func(e []Term) Term, rest []Term, args ...Term) Term {
	if len(rest) == 0 && len(args) == 1 {
		return args[0]
	}
	if f == nil {
		return MkAC(sym, Merge(Flatten(sym, args...), rest))
	}
	return NormAC(Merge(Flatten(sym, args...), rest), f)
}
