// Code "generated" from programs/nqueens.eln + nqueens.lgi
// (start with (queens_strat) queens(query)).
package main

import "elanspike/rt"

// ---- symbols
var (
	sym_a             = rt.NewSym("a", 0, false, "")
	sym_nil           = rt.NewSym("nil", 0, false, "")
	sym_cons          = rt.NewSym(".", 2, false, "@.@")
	sym_generate_list = rt.NewSym("generate_list", 1, false, "")
	sym_queens2       = rt.NewSym("queens", 2, false, "")
	sym_queens1       = rt.NewSym("queens", 1, false, "")
	sym_noattack      = rt.NewSym("noattack", 3, false, "")
	sym_q2i           = rt.NewSym("q2i", 1, false, "")
)

// ---- constants of the rules
var (
	con_nil = rt.Mk0(sym_nil)
	int_0   = rt.MkInt(0)
	int_1   = rt.MkInt(1)
)

// ---- symbol functions (normalisation, unlabelled rules, innermost)

// q2i(@): [] q2i(n) => n
func fun_q2i(v1 rt.Term) rt.Term {
	rt.Steps++
	return v1
}

// generate_list(@): [] generate_list(0) => nil ; [] generate_list(n) => n . generate_list(n-1)
func fun_generate_list(v1 rt.Term) rt.Term {
	if v1 == int_0 {
		rt.Steps++
		return con_nil
	}
	rt.Steps++
	return rt.Mk2(sym_cons, v1, fun_generate_list(rt.Minus(v1, int_1)))
}

// queens(@): [] queens(n) => queens(n,n)
func fun_queens1(v1 rt.Term) rt.Term {
	rt.Steps++
	return fun_queens2(v1, v1)
}

// queens(@,@): only labelled rules (queens_0, queens_n): constructor
func fun_queens2(v1, v2 rt.Term) rt.Term {
	return rt.Mk2(sym_queens2, v1, v2)
}

// noattack(@,@,@):
//
//	[] noattack(diff,d,nil) => true
//	[] noattack(diff,d,p.l) => d!=p and d-p!=diff and p-d!=diff and noattack(diff+1,d,l)
func fun_noattack(v1, v2, v3 rt.Term) rt.Term {
	if v3 == con_nil {
		rt.Steps++
		return rt.True
	}
	if v3.Sym == sym_cons {
		p, l := v3.Args[0], v3.Args[1]
		s0 := rt.NeqInt(v2, p)
		s1 := rt.NeqInt(rt.Minus(v2, p), v1)
		s1 = rt.And(s0, s1)
		s0 = rt.NeqInt(rt.Minus(p, v2), v1)
		s0 = rt.And(s1, s0)
		s1 = fun_noattack(rt.Plus(v1, int_1), v2, l)
		rt.Steps++
		return rt.And(s0, s1)
	}
	return rt.Mk3(sym_noattack, v1, v2, v3)
}

// ---- labelled rules: match, conditions, wheres, then k(rhs)

// [range_rule] x => x-1 if x > 1        (sort qint)
func rule_range_rule(t rt.Term, k rt.Cont) bool {
	x := t
	if rt.Gt(x, int_1) != rt.True {
		return false
	}
	rt.Steps++
	return k(rt.Minus(x, int_1))
}

// [queens_0] queens(0,size) => nil
func rule_queens_0(t rt.Term, k rt.Cont) bool {
	if t.Sym != sym_queens2 || t.Args[0] != int_0 {
		return false
	}
	rt.Steps++
	return k(con_nil)
}

// [queens_n] queens(n,size) => x . ql
//
//	if n>0
//	where ql:=(queens_strat) queens(n-1,size)
//	where xx:=(range) size
//	where x:=()q2i(xx)
//	if noattack(1,x,ql)
func rule_queens_n(t rt.Term, k rt.Cont) bool {
	if t.Sym != sym_queens2 {
		return false
	}
	n, size := t.Args[0], t.Args[1]
	if rt.Gt(n, int_0) != rt.True {
		return false
	}
	return strat_queens_strat(fun_queens2(rt.Minus(n, int_1), size), func(ql rt.Term) bool {
		return strat_range(size, func(xx rt.Term) bool {
			x := fun_q2i(xx)
			if fun_noattack(int_1, x, ql) != rt.True {
				return false
			}
			rt.Steps++
			return k(rt.Mk2(sym_cons, x, ql))
		})
	})
}

// ---- strategies

// dc(range_rule)
func strat_range_1(t rt.Term, k rt.Cont) bool {
	got := false
	stop := rule_range_rule(t, func(r rt.Term) bool { got = true; return k(r) })
	if got {
		return stop
	}
	return false
}

// [] range => iterate*(dc(range_rule))
func strat_range(t rt.Term, k rt.Cont) bool {
	if k(t) {
		return true
	}
	return strat_range_1(t, func(r rt.Term) bool { return strat_range(r, k) })
}

// [] queens_strat => dk(queens_0, queens_n)
func strat_queens_strat(t rt.Term, k rt.Cont) bool {
	if rule_queens_0(t, k) {
		return true
	}
	return rule_queens_n(t, k)
}

// [] det_queens_strat => first one(queens_0, queens_n)
func strat_det_queens_strat(t rt.Term, k rt.Cont) bool {
	var res rt.Term
	one := func(r rt.Term) bool { res = r; return true }
	if rule_queens_0(t, one) || rule_queens_n(t, one) {
		return k(res)
	}
	return false
}

var _ = strat_det_queens_strat

// ---- logic description: start with (queens_strat) queens(query)
func main() {
	query := rt.MkInt(rt.QueryInt())
	strat_queens_strat(fun_queens1(query), func(r rt.Term) bool {
		rt.PrintResult(r)
		return false // all results
	})
	rt.PrintSteps()
}
