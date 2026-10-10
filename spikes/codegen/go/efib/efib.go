// Code "generated" from programs/efib.eln + efib.lgi (start with () query,
// query = go(N)). The injections builtinInt->eInt, Fib->Object,
// Compute->Object, Object->Space are invisible (no node).
package main

import "elanspike/rt"

// ---- symbols
var (
	sym_U         = rt.NewSym("U", 2, true, "@ U @")
	sym_empty     = rt.NewSym("empty", 0, false, "")
	sym_Fib       = rt.NewSym("Fib", 2, false, "Fib[farg=@,val=@]")
	sym_Compute   = rt.NewSym("Compute", 2, false, "Compute[question=@,answer=@]")
	sym_UNDEF     = rt.NewSym("UNDEF", 0, false, "")
	sym_occursFib = rt.NewSym("occursFib", 2, false, "")
	sym_go        = rt.NewSym("go", 1, false, "")
	sym_result    = rt.NewSym("result", 2, false, "")
)

// ---- constants of the rules
var (
	con_empty   = rt.Mk0(sym_empty)
	con_UNDEF   = rt.Mk0(sym_UNDEF)
	int_0       = rt.MkInt(0)
	int_1       = rt.MkInt(1)
	int_2       = rt.MkInt(2)
	int_3       = rt.MkInt(3)
	int_1000000 = rt.MkInt(1000000)
)

// ---- symbol functions

// @ U @ (AC): flatten, sort; no unlabelled rules for U.
func fun_U(xs ...rt.Term) rt.Term {
	e := rt.ACFlatten(sym_U, xs)
	return rt.ACMake(sym_U, e, con_empty)
}

// Fib[farg=@,val=@]: constructor
func fun_Fib(v1, v2 rt.Term) rt.Term { return rt.Mk2(sym_Fib, v1, v2) }

// occursFib(@,@):
//
//	[] occursFib(S U Fib[farg=n,val=v],n) => true
//	[] occursFib(S,n)                     => false
func fun_occursFib(v1, v2 rt.Term) rt.Term {
	// rule 1: AC pattern S U Fib[farg=n,val=v], n non-linear (S, v unused)
	e := rt.ACElems(sym_U, v1)
	for i := range e {
		if rt.Skip(e, nil, i) {
			continue
		}
		p := e[i]
		if p.Sym != sym_Fib || p.Args[0] != v2 {
			continue
		}
		rt.Steps++
		return rt.True
	}
	// rule 2
	rt.Steps++
	return rt.False
}

// go(@): [] go(n) => result(S,n)
//
//	where S:=(loop) empty U Fib[farg=0,val=1] U Fib[farg=1,val=1] U Fib[farg=n,val=UNDEF]
func fun_go(v1 rt.Term) rt.Term {
	var res rt.Term
	ok := strat_loop(fun_U(con_empty, fun_Fib(int_0, int_1), fun_Fib(int_1, int_1), fun_Fib(v1, con_UNDEF)),
		func(S rt.Term) bool {
			res = fun_result(S, v1)
			return true // unlabelled rule: first result of the where
		})
	if ok {
		rt.Steps++
		return res
	}
	return rt.Mk1(sym_go, v1)
}

// result(@,@): [] result(S U Fib[farg=n,val=v],n) => v
func fun_result(v1, v2 rt.Term) rt.Term {
	e := rt.ACElems(sym_U, v1)
	for i := range e {
		if rt.Skip(e, nil, i) {
			continue
		}
		p := e[i]
		if p.Sym != sym_Fib || p.Args[0] != v2 {
			continue
		}
		rt.Steps++
		return p.Args[1]
	}
	return rt.Mk2(sym_result, v1, v2)
}

// ---- labelled rules (for Space)

// [init1] S U Fib[farg=0,val=UNDEF] => S U Fib[farg=0,val=1]
func rule_init1(t rt.Term, k rt.Cont) bool {
	e := rt.ACElems(sym_U, t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p := e[i]
		if p.Sym != sym_Fib || p.Args[0] != int_0 || p.Args[1] != con_UNDEF {
			continue
		}
		used[i] = true
		S := rt.ACMake(sym_U, rt.Rest(e, used), con_empty)
		rt.Steps++
		if k(fun_U(S, fun_Fib(int_0, int_1))) {
			return true
		}
		used[i] = false
	}
	return false
}

// [init2] S U Fib[farg=1,val=UNDEF] => S U Fib[farg=1,val=1]
func rule_init2(t rt.Term, k rt.Cont) bool {
	e := rt.ACElems(sym_U, t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p := e[i]
		if p.Sym != sym_Fib || p.Args[0] != int_1 || p.Args[1] != con_UNDEF {
			continue
		}
		used[i] = true
		S := rt.ACMake(sym_U, rt.Rest(e, used), con_empty)
		rt.Steps++
		if k(fun_U(S, fun_Fib(int_1, int_1))) {
			return true
		}
		used[i] = false
	}
	return false
}

var _, _ = rule_init1, rule_init2

// [rec1] S U Fib[farg=n,val=UNDEF] => S U Fib[farg=n,val=UNDEF] U Fib[farg=n-1,val=UNDEF]
//
//	if n > 2
//	if not(occursFib(S,n-1))
func rule_rec1(t rt.Term, k rt.Cont) bool {
	e := rt.ACElems(sym_U, t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p := e[i]
		if p.Sym != sym_Fib || p.Args[1] != con_UNDEF {
			continue
		}
		n := p.Args[0]
		used[i] = true
		S := rt.ACMake(sym_U, rt.Rest(e, used), con_empty)
		if rt.Gt(n, int_2) == rt.True && rt.Not(fun_occursFib(S, rt.Minus(n, int_1))) == rt.True {
			rt.Steps++
			if k(fun_U(S, fun_Fib(n, con_UNDEF), fun_Fib(rt.Minus(n, int_1), con_UNDEF))) {
				return true
			}
		}
		used[i] = false
	}
	return false
}

// [rec2] S U Fib[farg=n,val=UNDEF] => S U Fib[farg=n,val=UNDEF] U Fib[farg=n-2,val=UNDEF]
//
//	if n > 3
//	if not(occursFib(S,n-2))
func rule_rec2(t rt.Term, k rt.Cont) bool {
	e := rt.ACElems(sym_U, t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p := e[i]
		if p.Sym != sym_Fib || p.Args[1] != con_UNDEF {
			continue
		}
		n := p.Args[0]
		used[i] = true
		S := rt.ACMake(sym_U, rt.Rest(e, used), con_empty)
		if rt.Gt(n, int_3) == rt.True && rt.Not(fun_occursFib(S, rt.Minus(n, int_2))) == rt.True {
			rt.Steps++
			if k(fun_U(S, fun_Fib(n, con_UNDEF), fun_Fib(rt.Minus(n, int_2), con_UNDEF))) {
				return true
			}
		}
		used[i] = false
	}
	return false
}

// [compute] S U Fib[farg=n1,val=v1] U Fib[farg=n2,val=v2] U Fib[farg=n,val=UNDEF]
//
//	=> S U Fib[farg=n1,val=v1] U Fib[farg=n2,val=v2] U Fib[farg=n,val=v1+v2 % 1000000]
//	if n1 == n2 + 1
//	if n == n1 + 1
//
// Each condition is tested as soon as its variables are bound.
func rule_compute(t rt.Term, k rt.Cont) bool {
	e := rt.ACElems(sym_U, t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p1 := e[i]
		if p1.Sym != sym_Fib || !rt.IsInt(p1.Args[1]) {
			continue
		}
		n1, v1 := p1.Args[0], p1.Args[1]
		used[i] = true
		for j := range e {
			if rt.Skip(e, used, j) {
				continue
			}
			p2 := e[j]
			if p2.Sym != sym_Fib || !rt.IsInt(p2.Args[1]) {
				continue
			}
			n2, v2 := p2.Args[0], p2.Args[1]
			if rt.EqInt(n1, rt.Plus(n2, int_1)) != rt.True {
				continue
			}
			used[j] = true
			for l := range e {
				if rt.Skip(e, used, l) {
					continue
				}
				p3 := e[l]
				if p3.Sym != sym_Fib || p3.Args[1] != con_UNDEF {
					continue
				}
				n := p3.Args[0]
				if rt.EqInt(n, rt.Plus(n1, int_1)) != rt.True {
					continue
				}
				used[l] = true
				S := rt.ACMake(sym_U, rt.Rest(e, used), con_empty)
				rt.Steps++
				if k(fun_U(S, fun_Fib(n1, v1), fun_Fib(n2, v2),
					fun_Fib(n, rt.Plus(v1, rt.Mod(v2, int_1000000))))) {
					return true
				}
				used[l] = false
			}
			used[j] = false
		}
		used[i] = false
	}
	return false
}

// ---- strategies

// first one(rec1, rec2, compute)
func strat_loop_1(t rt.Term, k rt.Cont) bool {
	var res rt.Term
	one := func(r rt.Term) bool { res = r; return true }
	if rule_rec1(t, one) || rule_rec2(t, one) || rule_compute(t, one) {
		return k(res)
	}
	return false
}

// [] loop => repeat*(first one(rec1, rec2, compute))
// (the body is deterministic: repeat* is compiled as a loop)
func strat_loop(t rt.Term, k rt.Cont) bool {
	for {
		var next rt.Term
		if !strat_loop_1(t, func(r rt.Term) bool { next = r; return true }) {
			return k(t)
		}
		t = next
	}
}

// ---- logic description: start with () go(query)
func main() {
	query := rt.MkInt(rt.QueryInt())
	rt.PrintResult(fun_go(query))
	rt.PrintSteps()
}
