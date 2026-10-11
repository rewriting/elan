package elanrt

// Traversals tall, tone, tsome, rewrite and normin/normout (S7d task 6).
//
// The interpreter does not interpret them ("Traversal is not intepreted"):
// they follow REM's compiled code (src/compiler/rem/compiler/
// StrategyTraversalAll.java, ...One, ...Some, termCommon.c
// term_rewriteStep, trace.c norm_in/norm_out), except where REM is wrong
// (the S7d plan, Task 6 rulings):
//   - the term a traversal rebuilds is normalised (REM's specialApply; also
//     after rewrite, which REM does not renormalise), and the strategy after
//     a traversal in a sequence gets that term (REM's generated code gives
//     it the last argument's result);
//   - the arguments of a term are visited in order; an AC term's distinct
//     elements in canonical order, each with its multiplicity (REM's
//     genericGetMult): tall and tsome apply the strategy once to an
//     element and put its result for all its occurrences, tone replaces
//     one occurrence; each result is rebuilt from the term (REM adds the
//     new elements to one term allocated before the loop: on backtracking
//     the elements of the previous result stay);
//   - the strategy applied to an argument is the one of the argument's sort
//     ([Sel]): the labelled rules of a sort apply to its terms only (REM
//     is untyped: it applies x => s(x) to an int).

// Sel gives the strategy applied to argument i of a term of symbol sym
// (for an AC symbol: to its elements), compiled for the sort of that
// argument: the generated code selects it from the operator's profile.
type Sel func(sym int32, i int) Strat[Term]

// TAll is `tall(s)`: s applied to every argument of t, each combination
// of their results (the last argument varying fastest), the term rebuilt
// and normalised by norm; a constant (an int, a string) gives itself.
func TAll(t Term, k K[Term], norm Norm, sel Sel) bool {
	if len(t.Args) == 0 {
		return k(t)
	}
	Guard()
	defer Unguard()
	if Syms[t.Sym].AC {
		e := t.Args
		s := sel(t.Sym, 0)
		var out []Term
		var next func(i int) bool
		next = func(i int) bool {
			if i == len(e) {
				return k(rebuildAC(t.Sym, out, norm))
			}
			m := Mult(e, i)
			return s(e[i], func(r Term) bool {
				n := len(out)
				for range m {
					out = append(out, Elems(t.Sym, r)...)
				}
				stop := next(i + m)
				out = out[:n]
				return stop
			})
		}
		return next(0)
	}
	args := make([]Term, len(t.Args))
	var next func(i int) bool
	next = func(i int) bool {
		if i == len(args) {
			return k(norm(t.Sym, append([]Term(nil), args...)))
		}
		return sel(t.Sym, i)(t.Args[i], func(r Term) bool {
			args[i] = r
			return next(i + 1)
		})
	}
	return next(0)
}

// TOne is `tone(s)`: for each argument of t in order, each result of s
// on it replacing it, the term rebuilt and normalised by norm (for an AC
// term: one occurrence of each distinct element); no result on a
// constant.
func TOne(t Term, k K[Term], norm Norm, sel Sel) bool {
	if len(t.Args) == 0 {
		return false
	}
	Guard()
	defer Unguard()
	if Syms[t.Sym].AC {
		e := t.Args
		s := sel(t.Sym, 0)
		for i := 0; i < len(e); i += Mult(e, i) {
			rest := Rest(e, i)
			if s(e[i], func(r Term) bool {
				return k(rebuildAC(t.Sym, append(append([]Term(nil), rest...), Elems(t.Sym, r)...), norm))
			}) {
				return true
			}
		}
		return false
	}
	for i, a := range t.Args {
		if sel(t.Sym, i)(a, func(r Term) bool {
			args := append([]Term(nil), t.Args...)
			args[i] = r
			return k(norm(t.Sym, args))
		}) {
			return true
		}
	}
	return false
}

// TSome is `tsome(s)`: for each argument in order, each result of s on
// it, then the argument itself (after s's results: REM's choice point),
// the combinations where s applied to one argument at least, the term
// rebuilt and normalised by norm (an AC term: all the occurrences of a
// distinct element, or none); no result on a constant.
func TSome(t Term, k K[Term], norm Norm, sel Sel) bool {
	if len(t.Args) == 0 {
		return false
	}
	Guard()
	defer Unguard()
	if Syms[t.Sym].AC {
		e := t.Args
		s := sel(t.Sym, 0)
		var out []Term
		var next func(i int, applied bool) bool
		next = func(i int, applied bool) bool {
			if i == len(e) {
				return applied && k(rebuildAC(t.Sym, out, norm))
			}
			m := Mult(e, i)
			n := len(out)
			if s(e[i], func(r Term) bool {
				for range m {
					out = append(out, Elems(t.Sym, r)...)
				}
				stop := next(i+m, true)
				out = out[:n]
				return stop
			}) {
				return true
			}
			out = append(out, e[i:i+m]...)
			stop := next(i+m, applied)
			out = out[:n]
			return stop
		}
		return next(0, false)
	}
	args := make([]Term, len(t.Args))
	var next func(i int, applied bool) bool
	next = func(i int, applied bool) bool {
		if i == len(args) {
			return applied && k(norm(t.Sym, append([]Term(nil), args...)))
		}
		if sel(t.Sym, i)(t.Args[i], func(r Term) bool {
			args[i] = r
			return next(i+1, true)
		}) {
			return true
		}
		args[i] = t.Args[i]
		return next(i+1, applied)
	}
	return next(0, false)
}

// rebuildAC is the normal form of the AC term sym over the elements e
// (unsorted, two or more: a traversal keeps the number of occurrences).
func rebuildAC(sym int32, e []Term, norm Norm) Term {
	e = append([]Term(nil), e...)
	SortAC(e)
	return normElems(sym, e, norm)
}

// NormIn is `normin(l1, ..., ln)` on t: its normal form, innermost (REM's
// norm_in, norm_1): the arguments first, each by the strategy sel gives
// (normin on the argument's sort, one result), then step at the top (the
// first result of the first rule of the labels that applies, REM's step
// function), its result normalised again; t itself when no rule applies.
func NormIn(t Term, norm Norm, sel Sel, step Strat[Term]) Term {
	Guard()
	defer Unguard()
	for {
		t = mapArgs(t, norm, sel)
		r, ok := FirstResult(t, step)
		if !ok {
			return t
		}
		t = r
	}
}

// mapArgs is t with each argument replaced by the first result of the
// strategy sel gives (the term itself if none), rebuilt and normalised by
// norm when one changed.
func mapArgs(t Term, norm Norm, sel Sel) Term {
	if len(t.Args) == 0 {
		return t
	}
	var args []Term
	ac := Syms[t.Sym].AC
	for i, a := range t.Args {
		j := i
		if ac {
			j = 0
		}
		r, ok := FirstResult(a, sel(t.Sym, j))
		if !ok {
			r = a
		}
		if args == nil && !Eq(r, a) {
			args = make([]Term, 0, len(t.Args))
			args = append(args, t.Args[:i]...)
		}
		if args != nil {
			if ac {
				args = append(args, Elems(t.Sym, r)...)
			} else {
				args = append(args, r)
			}
		}
	}
	if args == nil {
		return t
	}
	if ac {
		return rebuildAC(t.Sym, args, norm)
	}
	return norm(t.Sym, args)
}

// OuterStep is one step of `normout(l1, ..., ln)` on t (REM's norm_4):
// step at the top if it applies, else the first argument (an AC term: the
// first distinct element) where the strategy sel gives (this function on
// the argument's sort) has a result, replaced by it, the term rebuilt
// and normalised by norm; no result when no rule applies anywhere.
func OuterStep(t Term, k K[Term], norm Norm, sel Sel, step Strat[Term]) bool {
	if r, ok := FirstResult(t, step); ok {
		return k(r)
	}
	if len(t.Args) == 0 {
		return false
	}
	Guard()
	defer Unguard()
	if Syms[t.Sym].AC {
		e := t.Args
		s := sel(t.Sym, 0)
		for i := 0; i < len(e); i += Mult(e, i) {
			if r, ok := FirstResult(e[i], s); ok {
				return k(rebuildAC(t.Sym, append(Rest(e, i), Elems(t.Sym, r)...), norm))
			}
		}
		return false
	}
	for i, a := range t.Args {
		if r, ok := FirstResult(a, sel(t.Sym, i)); ok {
			args := append([]Term(nil), t.Args...)
			args[i] = r
			return k(norm(t.Sym, args))
		}
	}
	return false
}

// NormTop applies step to t until it has no result: the last term
// (normout: step is [OuterStep], leftmost-outermost, from the top again
// after each step; normin on a sort of native values, whose terms have no
// arguments).
func NormTop[T any](t T, step Strat[T]) T {
	for {
		r, ok := FirstResult(t, step)
		if !ok {
			return t
		}
		t = r
	}
}
