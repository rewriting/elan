// Code "generated" from programs/nqueens.eln + nqueens.lgi, HYBRID with
// unique.Handle nodes (rtu) instead of the weak table (rtg); otherwise
// identical to ../../hyb/nqueens. Original header:
// // representation: generic shared nodes (rtg) for the user sorts, but the
// generator uses the sorts it knows: values of builtin sorts (int, bool) and
// of unboxed sorts (qint: only the injection @ : (int) qint) are native Go
// values in the generated functions; an int is boxed into an (cached,
// shared) int node only when it is stored as an argument of a node.
// (start with (queens_strat) queens(query)).
package main

import rt "gorepr/rtu"

// ---- symbols
var (
	sym_a             = rt.NewSym("a", 0, "")
	sym_nil           = rt.NewSym("nil", 0, "")
	sym_cons          = rt.NewSym(".", 2, "@.@")
	sym_generate_list = rt.NewSym("generate_list", 1, "")
	sym_queens2       = rt.NewSym("queens", 2, "")
	sym_queens1       = rt.NewSym("queens", 1, "")
	sym_noattack      = rt.NewSym("noattack", 3, "")
	sym_q2i           = rt.NewSym("q2i", 1, "")
)

// ---- constants of the rules
var con_nil = rt.Mk0(sym_nil)

// ---- symbol functions (normalisation, unlabelled rules, innermost)

// q2i(@): [] q2i(n) => n
func fun_q2i(v1 int32) int32 {
	rt.Step()
	return v1
}

// generate_list(@): [] generate_list(0) => nil ; [] generate_list(n) => n . generate_list(n-1)
func fun_generate_list(v1 int32) rt.Term {
	if v1 == 0 {
		rt.Step()
		return con_nil
	}
	rt.Step()
	return rt.Mk2(sym_cons, rt.MkInt(v1), fun_generate_list(v1-1))
}

// queens(@): [] queens(n) => queens(n,n)
func fun_queens1(v1 int32) rt.Term {
	rt.Step()
	return fun_queens2(v1, v1)
}

// queens(@,@): only labelled rules (queens_0, queens_n): constructor
func fun_queens2(v1, v2 int32) rt.Term {
	return rt.Mk2(sym_queens2, rt.MkInt(v1), rt.MkInt(v2))
}

// noattack(@,@,@):
//
//	[] noattack(diff,d,nil) => true
//	[] noattack(diff,d,p.l) => d!=p and d-p!=diff and p-d!=diff and noattack(diff+1,d,l)
//
// bool is a builtin: a stuck noattack(...) has no representation (panic).
func fun_noattack(v1, v2 int32, v3 rt.Term) bool {
	if v3 == con_nil {
		rt.Step()
		return true
	}
	if v3.Sym() == sym_cons {
		p, l := v3.Arg(0).Int(), v3.Arg(1)
		// builtin and is strict (innermost): the recursive call is always made
		rec := fun_noattack(v1+1, v2, l)
		rt.Step()
		return v2 != p && v2-p != v1 && p-v2 != v1 && rec
	}
	panic("noattack: no rule applies")
}

// ---- labelled rules: match, conditions, wheres, then k(rhs)

// [range_rule] x => x-1 if x > 1        (sort qint, unboxed: int32)
func rule_range_rule(t int32, k func(int32) bool) bool {
	x := t
	if !(x > 1) {
		return false
	}
	rt.Step()
	return k(x - 1)
}

// [queens_0] queens(0,size) => nil
func rule_queens_0(t rt.Term, k rt.Cont) bool {
	if t.Sym() != sym_queens2 || t.Arg(0).Int() != 0 {
		return false
	}
	rt.Step()
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
	if t.Sym() != sym_queens2 {
		return false
	}
	n, size := t.Arg(0).Int(), t.Arg(1).Int()
	if !(n > 0) {
		return false
	}
	return strat_queens_strat(fun_queens2(n-1, size), func(ql rt.Term) bool {
		return strat_range(size, func(xx int32) bool {
			x := fun_q2i(xx)
			if !fun_noattack(1, x, ql) {
				return false
			}
			rt.Step()
			return k(rt.Mk2(sym_cons, rt.MkInt(x), ql))
		})
	})
}

// ---- strategies

// dc(range_rule)
func strat_range_1(t int32, k func(int32) bool) bool {
	got := false
	stop := rule_range_rule(t, func(r int32) bool { got = true; return k(r) })
	if got {
		return stop
	}
	return false
}

// [] range => iterate*(dc(range_rule))
func strat_range(t int32, k func(int32) bool) bool {
	if k(t) {
		return true
	}
	return strat_range_1(t, func(r int32) bool { return strat_range(r, k) })
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

var _, _ = strat_det_queens_strat, fun_generate_list

// ---- logic description: start with (queens_strat) queens(query)
func main() {
	query := rt.QueryInt()
	strat_queens_strat(fun_queens1(query), func(r rt.Term) bool {
		rt.PrintResult(r)
		return false // all results
	})
	rt.PrintSteps()
}
