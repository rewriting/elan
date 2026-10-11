package elanrt

import "slices"

// The builtin operations on terms of any sort (S7d task 1): replace
// (code 16), occurs (17), syntactic matching (191) and unification (175).
// Terms are compared with [Eq] (maximal sharing: equality of normal
// forms). Rulings (the S7d plan, Task 1 findings):
//   - replace and occurs: REM's term_rec_replace and term_occur
//     (src/compiler/runtime/termCommon.c), and, inside an AC term, the
//     interpreter's stermreplace and occursin (src/interpreter/term/
//     term.cc): an AC term whose elements include those of the old term
//     (same AC symbol) contains it (a U b occurs in a U b U c); REM looks at
//     the elements one by one only.
//   - matching: REM's term_syntacticMatching (builtinMatching.c), the
//     interpreter's being broken (it fails on every function symbol); a
//     variable takes any subject (as the interpreter: REM refuses ints and
//     AC terms there), strings match equal strings (REM's test is
//     inverted), AC terms match only equal terms (empty theories).
//   - unification: the interpreter's equalUnify and buildMGUResult (REM
//     has none).

// MkAny returns the term sym(args...) (args in normal form; kept only by
// an AC symbol's node): [MkAC] for an AC symbol (args its sorted elements,
// two or more), else the shared node.
func MkAny(sym int32, args []Term) Term {
	if Syms[sym].AC {
		return MkAC(sym, args)
	}
	return mk(sym, 0, args)
}

// Norm is a normalisation dispatcher: the normal form of sym(args...),
// its arguments in normal form (for an AC symbol, its elements: flattened,
// sorted, two or more; the slice is the callee's). The generated program
// passes its normTerm.
type Norm func(sym int32, args []Term) Term

// Replace returns t with every occurrence of old replaced by new (replace
// old by new in t), the copied path renormalised bottom-up by norm, as
// REM's term_rec_replace: t itself when t is old; an int, a string or a
// constant is kept; each argument of a free symbol is replaced, the node
// rebuilt and renormalised when one changed (the others shared); an AC
// term whose elements include old's (old with the same AC symbol) gets
// new for them, the rest replaced in turn (the interpreter's
// stermreplace); else each of its elements is replaced, with its
// multiplicity, and the term flattened again (a new element with the AC
// symbol) and renormalised. The new term is not searched.
func Replace(t, old, new Term, norm Norm) Term {
	if Eq(t, old) {
		return new
	}
	if t.Sym == SymInt || t.Sym == SymString || len(t.Args) == 0 {
		return t
	}
	Guard()
	defer Unguard()
	if Syms[t.Sym].AC {
		e, changed := replaceElems(t.Sym, t.Args, old, new, norm)
		if !changed {
			return t
		}
		return normElems(t.Sym, e, norm)
	}
	var args []Term
	for i, a := range t.Args {
		r := Replace(a, old, new, norm)
		if args == nil && r != a {
			args = make([]Term, len(t.Args))
			copy(args, t.Args[:i])
		}
		if args != nil {
			args[i] = r
		}
	}
	if args == nil {
		return t
	}
	return norm(t.Sym, args)
}

// replaceElems replaces old by new in the sorted elements e of an AC term
// of symbol sym; the elements of the result (sorted) and whether one
// changed.
func replaceElems(sym int32, e []Term, old, new Term, norm Norm) ([]Term, bool) {
	if old.Sym == sym && slices.EqualFunc(e, old.Args, Eq) {
		return Elems(sym, new), true
	}
	if old.Sym == sym {
		if rest, ok := subtract(e, old.Args); ok {
			r := rest
			if len(rest) >= 2 {
				r, _ = replaceElems(sym, rest, old, new, norm)
			} else if len(rest) == 1 {
				r = Elems(sym, Replace(rest[0], old, new, norm))
			}
			return Merge(r, Elems(sym, new)), true
		}
	}
	out := make([]Term, 0, len(e))
	changed := false
	for _, x := range e {
		y := Replace(x, old, new, norm)
		if y != x {
			changed = true
		}
		out = append(out, Elems(sym, y)...)
	}
	if changed {
		SortAC(out)
	}
	return out, changed
}

// normElems is the normal form of the AC term sym of elements e (sorted,
// one or more: an element alone is itself).
func normElems(sym int32, e []Term, norm Norm) Term {
	if len(e) == 1 {
		return e[0]
	}
	return norm(sym, e)
}

// subtract returns the elements of e less those of sub (both sorted), and
// whether sub is included in e, strictly (a sub-multiset that is not all
// of e).
func subtract(e, sub []Term) ([]Term, bool) {
	if len(sub) >= len(e) {
		return nil, false
	}
	rest := make([]Term, 0, len(e)-len(sub))
	j := 0
	for _, x := range e {
		if j < len(sub) && Eq(x, sub[j]) {
			j++
			continue
		}
		if j < len(sub) && Compare(sub[j], x) < 0 {
			return nil, false // sub[j] is not in e
		}
		rest = append(rest, x)
	}
	return rest, j == len(sub)
}

// Occurs tells whether a occurs in b (occurs a in b): b is a, or a occurs
// in an argument of b; an AC term whose elements include a's (a with the
// same AC symbol) contains it, as the interpreter's occursin; else one of
// its elements must contain it. REM's term_occur, which the interpreter
// follows except for AC terms (and for a string b, in which nothing
// occurs for the interpreter, the string itself included; a string is a
// term like the others here, as for REM: occur[X,Y] wants sorts of
// terms).
func Occurs(a, b Term) bool {
	if Eq(a, b) {
		return true
	}
	if b.Sym == SymInt || b.Sym == SymString || len(b.Args) == 0 {
		return false
	}
	Guard()
	defer Unguard()
	if Syms[b.Sym].AC && a.Sym == b.Sym {
		if _, ok := subtract(b.Args, a.Args); ok {
			return true
		}
	}
	for _, x := range b.Args {
		if Occurs(a, x) {
			return true
		}
	}
	return false
}

// listVars returns the variables of a list of variables (cells of arity
// 2, the variable first, ended by a constant), as REM's
// term_storeVariable; a cell of another arity is the oracles' fatal error.
func listVars(vars Term) []Term {
	var vs []Term
	for l := vars; ; l = l.Args[1] {
		switch {
		case l.Sym != SymInt && l.Sym != SymString && len(l.Args) == 0:
			return vs
		case l.Sym != SymInt && l.Sym != SymString && len(l.Args) == 2 && !Syms[l.Sym].AC:
			vs = append(vs, l.Args[0])
		default:
			Fatal("builtinSyntacticMatching: the list of variables is not correct: " + Format(vars))
		}
	}
}

// varIndex is the position of t in the variables vs, or -1.
func varIndex(t Term, vs []Term) int {
	for i, v := range vs {
		if Eq(t, v) {
			return i
		}
	}
	return -1
}

// buildList returns the list vars with its variables replaced by vals
// (nil: the variable itself), its cells rebuilt (not normalised, as REM's
// term_buildResult), its last constant kept.
func buildList(vars Term, vals []Term) Term {
	cells := make([]Term, 0, len(vals))
	l := vars
	for range vals {
		cells = append(cells, l)
		l = l.Args[1]
	}
	for i := len(cells) - 1; i >= 0; i-- {
		v := vals[i]
		if v == nil {
			v = cells[i].Args[0]
		}
		l = Mk2(cells[i].Sym, v, l)
	}
	return l
}

// SyntacticMatch matches pat against subj (builtinSyntacticMatching(pat,
// subj, vars, fail), code 191), vars being the list of the terms of pat
// that are variables: the list of their values (vars with each variable
// replaced by its value; the variables that pat does not contain stay),
// or fail when pat does not match.
func SyntacticMatch(pat, subj, vars, fail Term) Term {
	vs := listVars(vars)
	vals := make([]Term, len(vs))
	if !matchSyn(pat, subj, vs, vals) {
		return fail
	}
	return buildList(vars, vals)
}

func matchSyn(p, s Term, vs, vals []Term) bool {
	if i := varIndex(p, vs); i >= 0 {
		if vals[i] == nil {
			vals[i] = s
			return true
		}
		return Eq(vals[i], s)
	}
	if Eq(p, s) {
		return true
	}
	switch {
	case p.Sym == SymInt || s.Sym == SymInt || p.Sym == SymString || s.Sym == SymString:
		return false
	case Syms[p.Sym].AC || Syms[s.Sym].AC:
		return false // not equal (empty theories)
	case p.Sym != s.Sym || len(p.Args) != len(s.Args):
		return false
	}
	Guard()
	defer Unguard()
	for i := range p.Args {
		if !matchSyn(p.Args[i], s.Args[i], vs, vals) {
			return false
		}
	}
	return true
}

// SyntacticUnify unifies pat and subj (builtinSyntacticUnification(pat,
// subj, vars, fail), code 175) as the interpreter does (term::equalUnify,
// buildMGUResult): the list vars with each variable replaced by the term
// it is bound to, followed through bound variables but not substituted
// inside (v1 bound to f(v2), v2 to a: f(v2)), an unbound variable kept;
// fail when they do not unify. Two variables: the one listed first is
// bound to the other; no occurs check.
func SyntacticUnify(pat, subj, vars, fail Term) Term {
	vs := listVars(vars)
	vals := make([]Term, len(vs))
	if !unifySyn(pat, subj, vs, vals) {
		return fail
	}
	for i, v := range vals {
		if v != nil {
			vals[i], _ = deref(v, vs, vals)
		}
	}
	return buildList(vars, vals)
}

// deref follows the bindings of t: the first term met that is not a bound
// variable, and its index among the variables (-1: not a variable).
func deref(t Term, vs, vals []Term) (Term, int) {
	for {
		i := varIndex(t, vs)
		if i < 0 || vals[i] == nil {
			return t, i
		}
		t = vals[i]
	}
}

func unifySyn(x, y Term, vs, vals []Term) bool {
	tx, ix := deref(x, vs, vals)
	ty, iy := deref(y, vs, vals)
	switch {
	case ix >= 0 && iy >= 0:
		if ix < iy {
			vals[ix] = ty
		} else if iy < ix {
			vals[iy] = tx
		}
		return true
	case ix >= 0:
		vals[ix] = ty
		return true
	case iy >= 0:
		vals[iy] = tx
		return true
	}
	if Eq(tx, ty) {
		return true
	}
	switch {
	case tx.Sym == SymInt || ty.Sym == SymInt || tx.Sym == SymString || ty.Sym == SymString:
		return false
	case Syms[tx.Sym].AC || Syms[ty.Sym].AC:
		return false // not equal (empty theories)
	case tx.Sym != ty.Sym || len(tx.Args) != len(ty.Args):
		return false
	}
	Guard()
	defer Unguard()
	for i := range tx.Args {
		if !unifySyn(tx.Args[i], ty.Args[i], vs, vals) {
			return false
		}
	}
	return true
}
