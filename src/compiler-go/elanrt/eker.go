package elanrt

import (
	"cmp"
	"slices"
)

// Eker's AC matcher (S7c task 2): the matcher that both oracles use for
// the AC patterns of wheres (the interpreter for every AC pattern), and
// that REM uses for the AC patterns it rewrites into wheres (three
// variables or more at an AC level, RewriteRule.java transformation3_1).
// It is a port of Steven Eker's C code (1992) as REM runs it
// (src/compiler/runtime: build_match.c, build_free.c, solve_free.c,
// build_pure.c, solve_pure.c, flatten.c; the interpreter's copy in
// src/interpreter/acmatcher/matcher is the same algorithm), so that the
// solutions come in the oracles' order:
//
//   - terms are put in Eker's form as REM's toEkerForm does
//     (src/compiler/runtime/eker_tools.c): the arguments of a free symbol
//     in reverse order, the elements of an AC symbol as a list of distinct
//     elements with multiplicities, sorted in descending order of
//     eker_term_cmp: the kind of term first (variable < AC < function <
//     constant < int), then the symbol's code in the .ref file
//     ([Symbol.Code], REM's code), an int by its tagged value (n<<1|1 as
//     an unsigned word: 0, 1, 2, ..., then the negative ints), then the
//     arguments;
//   - the pattern is built by the compiler with REM's numbering of its
//     variables ([EkVar]) and sorted the same way;
//   - the free part (simplify, build_free, solve_free: the skeleton of
//     free symbols, the element patterns of each AC level matched against
//     the subject's elements with the same top symbol, by brute force
//     backtracking) gives a stack of pure equations (the variables of each
//     AC level against the elements left over); the pure system
//     (build_pure, solve_pure) gives each variable but the last a
//     sub-multiset, from the smallest size up, each size in the order of
//     REM's "selection from multiset" (advance_select), the last variable
//     the rest. Variables are tried in decreasing multiplicity, the
//     variables shared between AC symbols first (qsort in build_assign:
//     a stable sort here, as BSD's qsort is below 7 elements and glibc's
//     merge sort always).
//
// Deviations: strings are constants ordered by their bytes (in REM all
// strings are the same constant for the matcher: a string pattern matches
// any string); a constant and an int never match each other (REM compares
// their codes: the int 3 and a constant of code 7 match). Both concern
// terms that REM handles wrongly.

// Kinds of Eker terms, in the order of eker_term_cmp (term_types.h).
const (
	ekVariable = iota
	ekACNormal // (unused: terms are compressed)
	ekAC
	ekFunction
	ekConstant
	ekBuiltin
)

// codeString is REM's code of strings (CODE_STRING, builtin.h).
const codeString = 249

// ekTerm is a term in Eker's form.
type ekTerm struct {
	kind int
	sym  uint64 // the symbol's REM code, the variable's number, or an int's tagged value
	str  string // a string
	rsym int32  // the runtime symbol (an AC symbol: to build values)
	args []*ekTerm
	ac   []ekElem // AC: distinct elements, descending
	orig Term     // a subject term: the term it comes from
	n    int      // AC: the number of elements, multiplicities included
}

type ekElem struct {
	t    *ekTerm
	mult int
}

// ekCmp is eker_term_cmp (flatten.c).
func ekCmp(t, u *ekTerm) int {
	if t == u {
		return 0
	}
	if c := cmp.Compare(t.kind, u.kind); c != 0 {
		return c
	}
	if c := cmp.Compare(t.sym, u.sym); c != 0 {
		return c
	}
	switch t.kind {
	case ekVariable, ekBuiltin:
		return 0
	case ekConstant:
		return cmp.Compare(t.str, u.str)
	case ekFunction:
		for i := 0; ; i++ {
			if i == len(t.args) {
				if i == len(u.args) {
					return 0
				}
				return -1
			}
			if i == len(u.args) {
				return 1
			}
			if c := ekCmp(t.args[i], u.args[i]); c != 0 {
				return c
			}
		}
	}
	for i := 0; ; i++ {
		if i == len(t.ac) {
			if i == len(u.ac) {
				return 0
			}
			return -1
		}
		if i == len(u.ac) {
			return 1
		}
		if c := ekCmp(t.ac[i].t, u.ac[i].t); c != 0 {
			return c
		}
		if c := cmp.Compare(t.ac[i].mult, u.ac[i].mult); c != 0 {
			return c
		}
	}
}

// ekSort sorts AC elements in descending order (ac_sort's merge sort is
// stable; equal elements do not occur in a compressed list).
func ekSort(e []ekElem) {
	slices.SortStableFunc(e, func(a, b ekElem) int { return ekCmp(b.t, a.t) })
}

func intTag(v int32) uint64 { return uint64(int64(v))<<1 | 1 }

// toEker is REM's toEkerForm followed by ac_sort.
func toEker(t Term) *ekTerm {
	switch t.Sym {
	case SymInt:
		return &ekTerm{kind: ekBuiltin, sym: intTag(t.Int), orig: t}
	case SymString:
		return &ekTerm{kind: ekConstant, sym: codeString, str: StrOf(t), orig: t}
	}
	s := &Syms[t.Sym]
	code := uint64(uint32(s.Code))
	if len(t.Args) == 0 {
		return &ekTerm{kind: ekConstant, sym: code, orig: t}
	}
	if !s.AC {
		e := &ekTerm{kind: ekFunction, sym: code, orig: t, args: make([]*ekTerm, len(t.Args))}
		for i, a := range t.Args {
			e.args[len(t.Args)-1-i] = toEker(a)
		}
		return e
	}
	e := &ekTerm{kind: ekAC, sym: code, rsym: t.Sym, orig: t, n: len(t.Args)}
	for i := 0; i < len(t.Args); i += Mult(t.Args, i) {
		e.ac = append(e.ac, ekElem{toEker(t.Args[i]), Mult(t.Args, i)})
	}
	ekSort(e.ac)
	return e
}

// EkPat is a pattern for [Eker], built by the generated program: the
// variables are numbered from 0 as REM numbers them (rgenTermForACMatcher,
// Term.java: depth first, the elements of an AC pattern in REM's order).
type EkPat struct {
	kind  int
	sym   int32 // runtime symbol
	v     int   // variable number
	i     int32
	s     string
	args  []*EkPat
	items []EkItem
	ek    *ekTerm // Eker's form, made once
}

// EkItem is an element of an AC pattern with its multiplicity.
type EkItem struct {
	P    *EkPat
	Mult int
}

// EkVar is variable number n.
func EkVar(n int) *EkPat { return &EkPat{kind: ekVariable, v: n} }

// EkInt is an int literal.
func EkInt(v int32) *EkPat { return &EkPat{kind: ekBuiltin, i: v} }

// EkStr is a string literal.
func EkStr(s string) *EkPat { return &EkPat{kind: ekConstant, sym: SymString, s: s} }

// EkOp is the free symbol sym applied to args (a constant without).
func EkOp(sym int32, args ...*EkPat) *EkPat {
	if len(args) == 0 {
		return &EkPat{kind: ekConstant, sym: sym}
	}
	return &EkPat{kind: ekFunction, sym: sym, args: args}
}

// EkAC is the AC symbol sym applied to its elements.
func EkAC(sym int32, items ...EkItem) *EkPat { return &EkPat{kind: ekAC, sym: sym, items: items} }

func (p *EkPat) eker() *ekTerm {
	if p.ek != nil {
		return p.ek
	}
	e := &ekTerm{kind: p.kind}
	switch p.kind {
	case ekVariable:
		e.sym = uint64(p.v)
	case ekBuiltin:
		e.sym = intTag(p.i)
	case ekConstant:
		if p.sym == SymString {
			e.sym, e.str = codeString, p.s
		} else {
			e.sym = uint64(uint32(Syms[p.sym].Code))
		}
	case ekFunction:
		e.sym = uint64(uint32(Syms[p.sym].Code))
		e.args = make([]*ekTerm, len(p.args))
		for i, a := range p.args {
			e.args[len(p.args)-1-i] = a.eker()
		}
	case ekAC:
		e.sym, e.rsym = uint64(uint32(Syms[p.sym].Code)), p.sym
		for _, it := range p.items {
			e.ac = append(e.ac, ekElem{it.P.eker(), it.Mult})
			e.n += it.Mult
		}
		ekSort(e.ac)
	}
	p.ek = e
	return e
}

// ---------------------------------------------------------------------------
// The free problem (free_types.h, build_free.c, solve_free.c)

type ekBinding struct {
	value  *ekTerm
	nTimes int
}

type ekBind struct {
	v int
	t *ekTerm
}

type ekFree struct {
	bindings []ekBind // sorted by variable
	probs    []*ekACProb
}

type ekVarMult struct{ v, mult int }

type ekACProb struct {
	sym    uint64
	rsym   int32
	unused []ekElem
	vars   []ekVarMult
	graphs []*ekGraph
}

type ekGraph struct {
	pats []ekPatNode
	subs []ekElem // the subject nodes; mult is the multiplicity left
}

type ekPatNode struct {
	pat      *ekTerm
	pmult    int
	matching []ekMsub
	cur      int
}

type ekMsub struct {
	sub    int
	result *ekFree // nil: nothing left to solve
}

type ekPure struct {
	sym    uint64
	rsym   int32
	vars   []ekVarMult
	unused []ekElem
}

// Eker enumerates the solutions of an AC matching problem:
//
//	for m := rt.NewEker(pattern, nvars, subject); m.Next(); { x := m.Value(0, norm) ... }
type Eker struct {
	bind   []ekBinding
	free   *ekFree
	failed bool
	stack  []*ekPure
	pure   *ekPureProb
	first  bool
}

// EkPin gives variable V the value T before the matching: T is the value
// of another occurrence of V, already bound (the rule compares them
// after the matching).
type EkPin struct {
	V int
	T Term
}

// NewEker starts the matching of subject against p, whose variables are
// numbered 0 to nvars-1 (build_match). The pinned variables are bound to
// their values: the solutions are those of the matching where each one
// has that value, in the same order (a bound variable is matched by the
// free part as a constant, and removed from the pure part with its value:
// it took exactly one value at its place in the enumeration of the
// variables, the others' values being enumerated as without it), without
// enumerating the others.
func NewEker(p *EkPat, nvars int, subject Term, pins ...EkPin) *Eker {
	m := &Eker{bind: make([]ekBinding, nvars), first: true}
	for _, pin := range pins {
		// (never retracted)
		m.bind[pin.V] = ekBinding{toEker(pin.T), 1 << 30}
	}
	fp, ok := m.buildFree(p.eker(), toEker(subject))
	if !ok {
		m.failed = true
	}
	m.free = fp
	return m
}

// buildFree is build_free: nil and true when nothing is left to solve.
func (m *Eker) buildFree(p, s *ekTerm) (*ekFree, bool) {
	fp := &ekFree{}
	if !m.simplify(fp, p, s) {
		return nil, false
	}
	m.assert(fp.bindings)
	defer m.retract(fp.bindings)
	for _, a := range fp.probs {
		for _, g := range a.graphs {
			for i := range g.pats {
				pn := &g.pats[i]
				for j, sn := range g.subs {
					if sn.mult >= pn.pmult {
						if r, ok := m.buildFree(pn.pat, sn.t); ok {
							pn.matching = append(pn.matching, ekMsub{j, r})
						}
					}
				}
				if len(pn.matching) == 0 {
					return nil, false
				}
			}
		}
	}
	if len(fp.bindings) == 0 && len(fp.probs) == 0 {
		return nil, true
	}
	return fp, true
}

func (m *Eker) assert(b []ekBind) {
	for _, x := range b {
		if m.bind[x.v].value == nil {
			m.bind[x.v] = ekBinding{x.t, 1}
		} else {
			m.bind[x.v].nTimes++
		}
	}
}

func (m *Eker) retract(b []ekBind) {
	for _, x := range b {
		if m.bind[x.v].nTimes--; m.bind[x.v].nTimes == 0 {
			m.bind[x.v].value = nil
		}
	}
}

func sameSym(p, s *ekTerm) bool { return p.sym == s.sym && (p.kind == s.kind || p.kind == ekVariable) }

// simplify (build_free.c).
func (m *Eker) simplify(fp *ekFree, p, s *ekTerm) bool {
	switch p.kind {
	case ekConstant, ekBuiltin:
		return sameSym(p, s) && p.str == s.str
	case ekVariable:
		v := int(p.sym)
		if b := m.bind[v].value; b != nil {
			return ekCmp(b, s) == 0
		}
		i := 0
		for ; i < len(fp.bindings); i++ {
			if fp.bindings[i].v == v {
				return ekCmp(fp.bindings[i].t, s) == 0
			}
			if fp.bindings[i].v > v {
				break
			}
		}
		fp.bindings = slices.Insert(fp.bindings, i, ekBind{v, s})
	case ekFunction:
		if !sameSym(p, s) {
			return false
		}
		for i := range p.args {
			if !m.simplify(fp, p.args[i], s.args[i]) {
				return false
			}
		}
	case ekAC:
		if !sameSym(p, s) || p.n > s.n {
			return false
		}
		return m.simplifyAC(fp, p, s.ac)
	}
	return true
}

// simplifyAC (build_free.c simplify_ac).
func (m *Eker) simplifyAC(fp *ekFree, top *ekTerm, ss []ekElem) bool {
	pp := top.ac
	var unused []ekElem
	var graphs []*ekGraph
	var vars []ekVarMult
	leftOvers := 0
	for len(pp) > 0 {
		p := pp[0].t
		switch p.kind {
		case ekConstant, ekBuiltin:
			for {
				if len(ss) == 0 {
					return false
				}
				if sameSym(p, ss[0].t) && p.str == ss[0].t.str {
					break
				}
				unused = append(unused, ss[0])
				ss = ss[1:]
			}
			diff := ss[0].mult - pp[0].mult
			if diff < 0 {
				return false
			}
			if diff > 0 {
				unused = append(unused, ekElem{ss[0].t, diff})
			}
			ss, pp = ss[1:], pp[1:]
		case ekFunction, ekAC:
			for {
				if len(ss) == 0 {
					return false
				}
				if sameSym(p, ss[0].t) {
					break
				}
				unused = append(unused, ss[0])
				ss = ss[1:]
			}
			g := &ekGraph{}
			diff := 0
			for n := countSame(pp); n > 0; n-- {
				g.pats = append(g.pats, ekPatNode{pat: pp[0].t, pmult: pp[0].mult})
				diff -= pp[0].mult
				pp = pp[1:]
			}
			for n := countSame(ss); n > 0; n-- {
				g.subs = append(g.subs, ss[0])
				diff += ss[0].mult
				ss = ss[1:]
			}
			graphs = append(graphs, g)
			if diff < 0 {
				return false
			}
			leftOvers += diff
		case ekVariable:
			vars = append(vars, ekVarMult{int(p.sym), pp[0].mult})
			pp = pp[1:]
		}
	}
	unused = append(unused, ss...)
	if (len(unused) > 0 || leftOvers > 0) && len(vars) == 0 {
		return false
	}
	if len(unused) == 0 && len(vars) == 0 && len(graphs) == 0 {
		return true
	}
	fp.probs = append(fp.probs, &ekACProb{sym: top.sym, rsym: top.rsym, unused: unused, vars: vars, graphs: graphs})
	return true
}

// countSame is the number of elements with the symbol of the first.
func countSame(e []ekElem) int {
	n := 1
	for n < len(e) && e[n].t.sym == e[0].t.sym && e[n].t.kind == e[0].t.kind {
		n++
	}
	return n
}

func (m *Eker) clash(b []ekBind) bool {
	for _, x := range b {
		if t := m.bind[x.v].value; t != nil && ekCmp(t, x.t) != 0 {
			return true
		}
	}
	return false
}

// solveFree is solve_free.
func (m *Eker) solveFree(fp *ekFree, reset bool) bool {
	var i int
	if reset {
		if len(fp.bindings) > 0 {
			if m.clash(fp.bindings) {
				return false
			}
			m.assert(fp.bindings)
		}
		i = 0
	} else {
		i = len(fp.probs) - 1
		goto backtrack
	}
forward:
	for ; i < len(fp.probs); i++ {
		if !m.advanceProb(fp.probs[i], true) {
			i--
			goto backtrack
		}
	}
	return true
backtrack:
	for ; i >= 0; i-- {
		if m.advanceProb(fp.probs[i], false) {
			i++
			goto forward
		}
	}
	if len(fp.bindings) > 0 {
		m.retract(fp.bindings)
	}
	return false
}

// advanceProb is advance_prob: after the graphs, the pure equation of the
// level (if it has variables) is pushed.
func (m *Eker) advanceProb(p *ekACProb, reset bool) bool {
	var g int
	if reset {
		g = 0
	} else {
		g = len(p.graphs) - 1
		if len(p.vars) > 0 {
			m.stack = m.stack[:len(m.stack)-1]
		}
		goto backtrack
	}
forward:
	for ; g < len(p.graphs); g++ {
		if !m.advanceGraph(p.graphs[g], true) {
			g--
			goto backtrack
		}
	}
	if len(p.vars) > 0 {
		m.stack = append(m.stack, &ekPure{sym: p.sym, rsym: p.rsym, vars: p.vars, unused: findUnused(p)})
	}
	return true
backtrack:
	for ; g >= 0; g-- {
		if m.advanceGraph(p.graphs[g], false) {
			g++
			goto forward
		}
	}
	return false
}

// findUnused is find_unused: the subjects left over by the graphs (pushed
// in front, so in reverse order), then the unmatched subjects.
func findUnused(p *ekACProb) []ekElem {
	var left []ekElem
	for _, g := range p.graphs {
		for _, s := range g.subs {
			if s.mult > 0 {
				left = append(left, s)
			}
		}
	}
	slices.Reverse(left)
	return append(left, p.unused...)
}

// advanceGraph is advance_graph (brute force backtracking over the
// pattern nodes, each over its matching subjects in order).
func (m *Eker) advanceGraph(g *ekGraph, reset bool) bool {
	np := len(g.pats)
	var i int
	if reset {
		i = 0
	} else {
		i = np - 1
		goto backtrack
	}
forward:
	for ; i < np; i++ {
		pn := &g.pats[i]
		found := false
		for k, ms := range pn.matching {
			if g.subs[ms.sub].mult >= pn.pmult && (ms.result == nil || m.solveFree(ms.result, true)) {
				pn.cur = k
				g.subs[ms.sub].mult -= pn.pmult
				found = true
				break
			}
		}
		if !found {
			i--
			goto backtrack
		}
	}
	return true
backtrack:
	for ; i >= 0; i-- {
		pn := &g.pats[i]
		ms := pn.matching[pn.cur]
		if ms.result != nil && m.solveFree(ms.result, false) {
			i++
			goto forward
		}
		g.subs[ms.sub].mult += pn.pmult
		for k := pn.cur + 1; k < len(pn.matching); k++ {
			ms := pn.matching[k]
			if g.subs[ms.sub].mult >= pn.pmult && (ms.result == nil || m.solveFree(ms.result, true)) {
				pn.cur = k
				g.subs[ms.sub].mult -= pn.pmult
				i++
				goto forward
			}
		}
	}
	return false
}

// ---------------------------------------------------------------------------
// The pure problem (pure_types.h, build_pure.c, solve_pure.c)

const (
	ekShared = -2
	ekUnused = -1
	ekDead   = -1
	ekUndef  = -1
)

type ekAssign struct{ cur, max, sumPrev int }

type ekVarHead struct {
	v       int
	vmult   []int
	maxMult int
	owner   int64 // the owning AC symbol's code, or ekShared
	rsym    int32
	// owned
	curSize, maxSize int
	assign           []ekAssign
	// shared
	curAss int
}

type ekIndex struct{ index, smult int }

type ekTermHead struct {
	t        *ekTerm
	tmult    []int
	subterms []ekIndex // nil: none (or not all in term_tab)
}

type ekPureProb struct {
	nPure  int
	topSym []uint64
	vars   []ekVarHead
	terms  []ekTermHead
}

// buildPure is build_pure: false when the bound variables leave no
// solution.
func (m *Eker) buildPure() bool {
	n := len(m.stack)
	if n == 0 {
		m.pure = nil
		return true
	}
	prob := &ekPureProb{nPure: n, topSym: make([]uint64, n)}
	rsyms := make([]int32, n)
	// the stack from its top
	for i := 0; i < n; i++ {
		r := m.stack[n-1-i]
		prob.topSym[i], rsyms[i] = r.sym, r.rsym
		for _, v := range r.vars {
			j := slices.IndexFunc(prob.vars, func(h ekVarHead) bool { return h.v == v.v })
			if j < 0 {
				prob.vars = append(prob.vars, ekVarHead{v: v.v, vmult: make([]int, n), owner: ekShared})
				j = len(prob.vars) - 1
			}
			prob.vars[j].vmult[i] = v.mult
		}
		for _, t := range r.unused {
			j := slices.IndexFunc(prob.terms, func(h ekTermHead) bool { return ekCmp(h.t, t.t) == 0 })
			if j < 0 {
				prob.terms = append(prob.terms, ekTermHead{t: t.t, tmult: make([]int, n)})
				j = len(prob.terms) - 1
			}
			prob.terms[j].tmult[i] = t.mult
		}
	}
	if !m.deleteBound(prob) {
		return false
	}
	prob.buildAssign(rsyms)
	m.pure = prob
	return true
}

func (m *Eker) deleteBound(prob *ekPureProb) bool {
	for i := range prob.vars {
		if b := m.bind[prob.vars[i].v].value; b != nil {
			if !prob.deleteBinding(prob.vars[i].vmult, b) {
				return false
			}
			prob.vars[i].v = ekDead
		}
	}
	prob.vars = slices.DeleteFunc(prob.vars, func(h ekVarHead) bool { return h.v == ekDead })
	prob.terms = slices.DeleteFunc(prob.terms, func(h ekTermHead) bool {
		return !slices.ContainsFunc(h.tmult, func(k int) bool { return k != 0 })
	})
	if len(prob.vars) == 0 {
		return len(prob.terms) == 0
	}
	return len(prob.terms) != 0
}

func (prob *ekPureProb) deleteBinding(vmult []int, b *ekTerm) bool {
	acBinding := b.kind == ekAC
	j := slices.IndexFunc(prob.terms, func(h ekTermHead) bool { return ekCmp(h.t, b) == 0 })
	if j < 0 && !acBinding {
		return false
	}
	for k := 0; k < prob.nPure; k++ {
		m := vmult[k]
		if m == 0 {
			continue
		}
		if acBinding {
			if b.sym == prob.topSym[k] { // AC nesting
				for _, p := range b.ac {
					l := slices.IndexFunc(prob.terms, func(h ekTermHead) bool { return ekCmp(h.t, p.t) == 0 })
					if l < 0 {
						return false
					}
					t := prob.terms[l].tmult[k] - m*p.mult
					if t < 0 {
						return false
					}
					prob.terms[l].tmult[k] = t
				}
				continue
			}
			if j < 0 {
				return false
			}
		}
		t := prob.terms[j].tmult[k] - m
		if t < 0 {
			return false
		}
		prob.terms[j].tmult[k] = t
	}
	return true
}

// buildAssign is build_assign.
func (prob *ekPureProb) buildAssign(rsyms []int32) {
	shared := false
	for i := range prob.vars {
		h := &prob.vars[i]
		k := int64(ekUnused)
		for j := 0; j < prob.nPure; j++ {
			if h.vmult[j] != 0 {
				if k == ekUnused {
					k, h.rsym = int64(prob.topSym[j]), rsyms[j]
				} else if k != int64(prob.topSym[j]) {
					k = ekShared
					shared = true
					break
				}
			}
		}
		h.owner = k
		h.maxMult = slices.Max(h.vmult)
	}
	// shared variables first, then by decreasing multiplicity (qsort:
	// stable here, see above)
	slices.SortStableFunc(prob.vars, func(a, b ekVarHead) int {
		as, bs := a.owner == ekShared, b.owner == ekShared
		switch {
		case as && !bs:
			return -1
		case !as && bs:
			return 1
		}
		return b.maxMult - a.maxMult
	})
	for i := range prob.vars {
		if prob.vars[i].owner != ekShared {
			prob.vars[i].assign = make([]ekAssign, len(prob.terms))
		}
	}
	if shared {
		prob.computeIndexLists()
	}
}

func (prob *ekPureProb) computeIndexLists() {
	for i := range prob.terms {
		t := prob.terms[i].t
		if t.kind != ekAC {
			continue
		}
		var list []ekIndex
		for _, p := range t.ac {
			j := -1
			for k := range prob.terms {
				if k != i && ekCmp(prob.terms[k].t, p.t) == 0 {
					j = k
					break
				}
			}
			if j < 0 {
				list = nil
				break
			}
			list = append([]ekIndex{{j, p.mult}}, list...)
		}
		prob.terms[i].subterms = list
	}
}

// solvePure is solve_pure.
func (prob *ekPureProb) solvePure(reset bool) bool {
	n := len(prob.vars)
	advance := func(i int, reset bool) bool {
		if prob.vars[i].owner == ekShared {
			return prob.advanceShared(i, reset)
		}
		return prob.advanceOwned(i, reset)
	}
	var i int
	if reset {
		i = 0
	} else {
		i = n - 1
		goto backtrack
	}
forward:
	for ; i < n; i++ {
		if !advance(i, true) {
			i--
			goto backtrack
		}
	}
	return true
backtrack:
	for ; i >= 0; i-- {
		if advance(i, false) {
			i++
			goto forward
		}
	}
	return false
}

func (prob *ekPureProb) advanceShared(v int, reset bool) bool {
	h := &prob.vars[v]
	if v == len(prob.vars)-1 {
		return reset && prob.sharedLast(v)
	}
	cur := -1
	if !reset {
		cur = h.curAss
		prob.updateShared(v, cur, 1)
	}
next:
	for cur++; cur < len(prob.terms); cur++ {
		th := &prob.terms[cur]
		for i := 0; i < prob.nPure; i++ {
			m := h.vmult[i]
			if m == 0 {
				continue
			}
			if prob.topSym[i] == th.t.sym {
				if th.subterms == nil {
					continue next
				}
				for _, p := range th.subterms {
					if prob.terms[p.index].tmult[i] < m*p.smult {
						continue next
					}
				}
			} else if th.tmult[i] < m {
				continue next
			}
		}
		prob.updateShared(v, cur, -1)
		h.curAss = cur
		return true
	}
	return false
}

func (prob *ekPureProb) updateShared(v, t, d int) {
	vmult := prob.vars[v].vmult
	th := &prob.terms[t]
	for i := 0; i < prob.nPure; i++ {
		m := vmult[i]
		if m == 0 {
			continue
		}
		if d < 0 {
			m = -m
		}
		if prob.topSym[i] == th.t.sym {
			for _, p := range th.subterms {
				prob.terms[p.index].tmult[i] += m * p.smult
			}
		} else {
			th.tmult[i] += m
		}
	}
}

func (prob *ekPureProb) sharedLast(v int) bool {
	h := &prob.vars[v]
	for i := 0; i < prob.nPure; i++ {
		if h.vmult[i] == 0 {
			for j := range prob.terms {
				if prob.terms[j].tmult[i] != 0 {
					return false
				}
			}
		}
	}
next:
	for cur := range prob.terms {
		th := &prob.terms[cur]
		for i := 0; i < prob.nPure; i++ {
			m := h.vmult[i]
			if m == 0 {
				continue
			}
			if prob.topSym[i] == th.t.sym {
				if th.subterms == nil {
					continue next
				}
				for _, p := range th.subterms {
					if prob.terms[p.index].tmult[i] != m*p.smult {
						continue next
					}
				}
				for j := range prob.terms {
					if !slices.ContainsFunc(th.subterms, func(x ekIndex) bool { return x.index == j }) && prob.terms[j].tmult[i] != 0 {
						continue next
					}
				}
			} else {
				if th.tmult[i] != m {
					continue next
				}
				for j := range prob.terms {
					if j != cur && prob.terms[j].tmult[i] != 0 {
						continue next
					}
				}
			}
		}
		h.curAss = cur
		return true
	}
	return false
}

func (prob *ekPureProb) advanceOwned(v int, reset bool) bool {
	h := &prob.vars[v]
	if v == len(prob.vars)-1 {
		return reset && prob.ownedLast(v)
	}
	var size, curSize int
	if reset {
		for j := range prob.terms {
			ass := ekUndef
			for i := 0; i < prob.nPure; i++ {
				if m := h.vmult[i]; m != 0 {
					t := prob.terms[j].tmult[i] / m
					if t == 0 {
						ass = 0
						break
					}
					if ass == ekUndef || ass > t {
						ass = t
					}
				}
			}
			h.assign[j].sumPrev = size
			h.assign[j].max = ass
			size += ass
		}
		if size == 0 {
			return false
		}
		h.maxSize = size
	} else {
		curSize = h.curSize
		if prob.advanceSelect(v, curSize, false) {
			return true
		}
		size = h.maxSize
	}
	for curSize++; curSize <= size; curSize++ {
		if prob.advanceSelect(v, curSize, true) {
			h.curSize = curSize
			return true
		}
	}
	return false
}

func (prob *ekPureProb) update(v, i, d int) {
	for j := 0; j < prob.nPure; j++ {
		prob.terms[i].tmult[j] += d * prob.vars[v].vmult[j]
	}
}

// advanceSelect is advance_select: the next sub-multiset of size r.
func (prob *ekPureProb) advanceSelect(v, r int, reset bool) bool {
	a := prob.vars[v].assign
	n := len(prob.terms)
	var i int
	if reset {
		i = n
	} else {
		r = a[0].cur
		prob.update(v, 0, r)
		for i = 1; i < n; i++ {
			t := a[i].cur
			if r > 0 && t < a[i].max {
				a[i].cur = t + 1
				prob.update(v, i, -1)
				r--
				goto forward
			}
			r += t
			prob.update(v, i, t)
		}
		return false
	}
forward:
	for i--; i > 0 && a[i].sumPrev >= r; i-- {
		a[i].cur = 0
	}
	a[i].cur = r - a[i].sumPrev
	prob.update(v, i, -a[i].cur)
	for i--; i >= 0; i-- {
		a[i].cur = a[i].max
		prob.update(v, i, -a[i].cur)
	}
	return true
}

func (prob *ekPureProb) ownedLast(v int) bool {
	h := &prob.vars[v]
	size := 0
	for j := range prob.terms {
		a := ekUndef
		for i := 0; i < prob.nPure; i++ {
			m, t := h.vmult[i], prob.terms[j].tmult[i]
			if m == 0 {
				if t != 0 {
					return false
				}
				continue
			}
			if t%m != 0 {
				return false
			}
			t /= m
			if a == ekUndef {
				a = t
			} else if a != t {
				return false
			}
		}
		h.assign[j].cur = a
		size += a
	}
	if size == 0 {
		return false
	}
	h.curSize = size
	return true
}

// ---------------------------------------------------------------------------
// Solutions (build_match.c)

// Next moves to the next solution (find_match); false at the end.
func (m *Eker) Next() bool {
	if m.failed {
		return false
	}
	if m.first {
		m.first = false
		if m.free == nil {
			return true
		}
		if !m.solveFree(m.free, true) {
			m.failed = true
			return false
		}
		if m.buildPure() && (m.pure == nil || m.pure.solvePure(true)) {
			return true
		}
	} else {
		if m.free == nil {
			m.failed = true
			return false
		}
		if m.pure != nil && m.pure.solvePure(false) {
			return true
		}
	}
	for m.solveFree(m.free, false) {
		if m.buildPure() && (m.pure == nil || m.pure.solvePure(true)) {
			return true
		}
	}
	m.pure = nil
	m.failed = true
	return false
}

// Value is the value of variable v in the current solution (extract_match):
// a term of the subject, or the sub-multiset of elements of an AC symbol
// given by norm(sym, elements) (elements sorted, two or more; norm builds
// the normal form of the AC term). nil if v is not bound.
func (m *Eker) Value(v int, norm func(sym int32, e []Term) Term) Term {
	if m.pure != nil {
		for i := range m.pure.vars {
			h := &m.pure.vars[i]
			if h.v != v {
				continue
			}
			if h.owner == ekShared {
				return m.pure.terms[h.curAss].t.orig
			}
			var e []Term
			for j, a := range h.assign {
				for range a.cur {
					e = append(e, m.pure.terms[j].t.orig)
				}
			}
			if len(e) == 1 {
				return e[0]
			}
			SortAC(e)
			return norm(h.rsym, e)
		}
	}
	if b := m.bind[v].value; b != nil {
		return b.orig
	}
	return nil
}
