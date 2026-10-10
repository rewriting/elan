// Code "generated" from programs/propc.eln + propc{1,2,3}.lgi
// (start with () q1 / q2 / q3; the query on the command line selects it).
package main

import (
	"os"

	"elanspike/rt"
)

// ---- symbols
var (
	sym_t       = rt.NewSym("t", 0, false, "")
	sym_f       = rt.NewSym("f", 0, false, "")
	sym_and     = rt.NewSym("and", 2, true, "")
	sym_xor     = rt.NewSym("xor", 2, true, "")
	sym_or      = rt.NewSym("or", 2, false, "")
	sym_iff     = rt.NewSym("iff", 2, false, "")
	sym_not     = rt.NewSym("not", 1, false, "")
	sym_implies = rt.NewSym("implies", 2, false, "")
	sym_a1      = rt.NewSym("a1", 0, false, "")
	sym_a2      = rt.NewSym("a2", 0, false, "")
	sym_a3      = rt.NewSym("a3", 0, false, "")
	sym_a4      = rt.NewSym("a4", 0, false, "")
	sym_a5      = rt.NewSym("a5", 0, false, "")
	sym_a6      = rt.NewSym("a6", 0, false, "")
	sym_a7      = rt.NewSym("a7", 0, false, "")
	sym_a8      = rt.NewSym("a8", 0, false, "")
	sym_a9      = rt.NewSym("a9", 0, false, "")
	sym_a10     = rt.NewSym("a10", 0, false, "")
	sym_a11     = rt.NewSym("a11", 0, false, "")
	sym_a12     = rt.NewSym("a12", 0, false, "")
	sym_a13     = rt.NewSym("a13", 0, false, "")
	sym_a14     = rt.NewSym("a14", 0, false, "")
	sym_a15     = rt.NewSym("a15", 0, false, "")
	sym_a16     = rt.NewSym("a16", 0, false, "")
	sym_a17     = rt.NewSym("a17", 0, false, "")
	sym_a18     = rt.NewSym("a18", 0, false, "")
	sym_q1      = rt.NewSym("q1", 0, false, "")
	sym_q2      = rt.NewSym("q2", 0, false, "")
	sym_q3      = rt.NewSym("q3", 0, false, "")
)

// ---- constants (constructors of arity 0)
var (
	con_t   = rt.Mk0(sym_t)
	con_f   = rt.Mk0(sym_f)
	con_a1  = rt.Mk0(sym_a1)
	con_a2  = rt.Mk0(sym_a2)
	con_a3  = rt.Mk0(sym_a3)
	con_a4  = rt.Mk0(sym_a4)
	con_a5  = rt.Mk0(sym_a5)
	con_a6  = rt.Mk0(sym_a6)
	con_a7  = rt.Mk0(sym_a7)
	con_a8  = rt.Mk0(sym_a8)
	con_a9  = rt.Mk0(sym_a9)
	con_a10 = rt.Mk0(sym_a10)
	con_a11 = rt.Mk0(sym_a11)
	con_a12 = rt.Mk0(sym_a12)
	con_a13 = rt.Mk0(sym_a13)
	con_a14 = rt.Mk0(sym_a14)
	con_a15 = rt.Mk0(sym_a15)
	con_a16 = rt.Mk0(sym_a16)
	con_a17 = rt.Mk0(sym_a17)
	con_a18 = rt.Mk0(sym_a18)
)

// ---- symbol functions (AC rules are applied with an extension: the
// variable that is alone with a constant takes the whole rest of the
// multiset; a pattern with only non-variable context gets an implicit
// extension, as REM does). A variable bound to an AC rest is rebuilt with
// the symbol's normalising function before use.

// and(@,@) (AC):
//
//	[] and(x, x) => x                 (and(x,x,R...) => and(x,R...))
//	[] and(x, t) => x
//	[] and(x, f) => f
//	[] and(x, xor(y, z)) => xor(and(x, y), and(x, z))
func fun_and(xs ...rt.Term) rt.Term {
	e := rt.ACFlatten(sym_and, xs)
	if len(e) == 1 {
		return e[0]
	}
	// rule 1: and(x, x) with extension
	for i := 1; i < len(e); i++ {
		if e[i] == e[i-1] {
			x := e[i]
			R := rt.Without(e, i-1, i)
			rt.Steps++
			return fun_and(append(R, x)...)
		}
	}
	// rule 2: and(x, t)
	for i := range e {
		if e[i] == con_t {
			x := fun_and(rt.Without(e, i, -1)...)
			rt.Steps++
			return x
		}
	}
	// rule 3: and(x, f)
	for i := range e {
		if e[i] == con_f {
			rt.Steps++
			return con_f
		}
	}
	// rule 4: and(x, xor(y, z)), y one element of the xor, z the rest
	for i := range e {
		if rt.Skip(e, nil, i) || e[i].Sym != sym_xor {
			continue
		}
		x := fun_and(rt.Without(e, i, -1)...)
		ex := e[i].Args
		y := ex[0]
		z := fun_xor(rt.Without(ex, 0, -1)...)
		rt.Steps++
		return fun_xor(fun_and(x, y), fun_and(x, z))
	}
	return rt.ACMake(sym_and, e, nil)
}

// xor(@,@) (AC):
//
//	[] xor(x, x) => f                 (xor(x,x,R...) => xor(R...,f))
//	[] xor(x, f) => x
func fun_xor(xs ...rt.Term) rt.Term {
	e := rt.ACFlatten(sym_xor, xs)
	if len(e) == 1 {
		return e[0]
	}
	// rule 1: xor(x, x) with extension
	for i := 1; i < len(e); i++ {
		if e[i] == e[i-1] {
			R := rt.Without(e, i-1, i)
			rt.Steps++
			return fun_xor(append(R, con_f)...)
		}
	}
	// rule 2: xor(x, f)
	for i := range e {
		if e[i] == con_f {
			x := fun_xor(rt.Without(e, i, -1)...)
			rt.Steps++
			return x
		}
	}
	return rt.ACMake(sym_xor, e, nil)
}

// not(@): [] not(x) => xor(x, t)
func fun_not(v1 rt.Term) rt.Term {
	rt.Steps++
	return fun_xor(v1, con_t)
}

// implies(@,@): [] implies(x, y) => not(xor(x, and(x, y)))
func fun_implies(v1, v2 rt.Term) rt.Term {
	rt.Steps++
	return fun_not(fun_xor(v1, fun_and(v1, v2)))
}

// or(@,@): [] or(x, y) => xor(and(x, y), xor(x, y))
func fun_or(v1, v2 rt.Term) rt.Term {
	rt.Steps++
	return fun_xor(fun_and(v1, v2), fun_xor(v1, v2))
}

// iff(@,@): [] iff(x, y) => not(xor(x, y))
func fun_iff(v1, v2 rt.Term) rt.Term {
	rt.Steps++
	return fun_not(fun_xor(v1, v2))
}

// q1: [] q1 => ... (see propc.eln)
func fun_q1() rt.Term {
	rt.Steps++
	return fun_implies(fun_and(fun_iff(fun_iff(fun_or(con_a1, con_a2), fun_or(fun_not(con_a3), fun_iff(fun_xor(con_a4, con_a5), fun_not(fun_not(fun_not(con_a6)))))), fun_not(fun_and(fun_and(con_a7, con_a8), fun_not(fun_xor(fun_xor(fun_or(con_a9, fun_and(con_a10, con_a11)), con_a2), fun_and(fun_and(con_a11, fun_xor(con_a2, fun_iff(con_a5, con_a5))), fun_xor(fun_xor(con_a7, con_a7), fun_iff(con_a9, con_a4)))))))), fun_implies(fun_iff(fun_iff(fun_or(con_a1, con_a2), fun_or(fun_not(con_a3), fun_iff(fun_xor(con_a4, con_a5), fun_not(fun_not(fun_not(con_a6)))))), fun_not(fun_and(fun_and(con_a7, con_a8), fun_not(fun_xor(fun_xor(fun_or(con_a9, fun_and(con_a10, con_a11)), con_a2), fun_and(fun_and(con_a11, fun_xor(con_a2, fun_iff(con_a5, con_a5))), fun_xor(fun_xor(con_a7, con_a7), fun_iff(con_a9, con_a4)))))))), fun_not(fun_and(fun_implies(fun_and(con_a1, con_a2), fun_not(fun_xor(fun_or(fun_or(fun_xor(fun_implies(fun_and(con_a3, con_a4), fun_implies(con_a5, con_a6)), fun_or(con_a7, con_a8)), fun_xor(fun_iff(con_a9, con_a10), con_a11)), fun_xor(fun_xor(con_a2, con_a2), con_a7)), fun_iff(fun_or(con_a4, con_a9), fun_xor(fun_not(con_a6), con_a6))))), fun_not(fun_iff(fun_not(con_a11), fun_not(con_a9))))))), fun_not(fun_and(fun_implies(fun_and(con_a1, con_a2), fun_not(fun_xor(fun_or(fun_or(fun_xor(fun_implies(fun_and(con_a3, con_a4), fun_implies(con_a5, con_a6)), fun_or(con_a7, con_a8)), fun_xor(fun_iff(con_a9, con_a10), con_a11)), fun_xor(fun_xor(con_a2, con_a2), con_a7)), fun_iff(fun_or(con_a4, con_a9), fun_xor(fun_not(con_a6), con_a6))))), fun_not(fun_iff(fun_not(con_a11), fun_not(con_a9))))))
}

// q2: [] q2 => ... (see propc.eln)
func fun_q2() rt.Term {
	rt.Steps++
	return fun_implies(fun_and(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a9))), fun_iff(fun_not(fun_not(con_a2)), fun_implies(fun_or(con_a9, con_a6), fun_or(con_a10, con_a5)))), fun_not(fun_or(con_a9, fun_implies(fun_not(con_a8), fun_or(con_a4, con_a9))))))), fun_implies(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a9))), fun_iff(fun_not(fun_not(con_a2)), fun_implies(fun_or(con_a9, con_a6), fun_or(con_a10, con_a5)))), fun_not(fun_or(con_a9, fun_implies(fun_not(con_a8), fun_or(con_a4, con_a9))))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a4, fun_or(con_a4, con_a1)), con_a2))), fun_or(fun_or(fun_xor(fun_or(con_a4, con_a7), con_a2), fun_and(con_a8, con_a1)), fun_not(fun_not(fun_not(con_a6)))))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a4, fun_or(con_a4, con_a1)), con_a2))), fun_or(fun_or(fun_xor(fun_or(con_a4, con_a7), con_a2), fun_and(con_a8, con_a1)), fun_not(fun_not(fun_not(con_a6)))))))
}

// q3: [] q3 => ... (see propc.eln)
func fun_q3() rt.Term {
	rt.Steps++
	return fun_implies(fun_and(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a11))), fun_implies(fun_or(con_a4, fun_and(con_a3, fun_iff(con_a1, con_a2))), fun_not(fun_not(con_a4)))), fun_xor(fun_implies(fun_implies(con_a6, con_a1), fun_not(con_a1)), fun_not(con_a9))))), fun_implies(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a11))), fun_implies(fun_or(con_a4, fun_and(con_a3, fun_iff(con_a1, con_a2))), fun_not(fun_not(con_a4)))), fun_xor(fun_implies(fun_implies(con_a6, con_a1), fun_not(con_a1)), fun_not(con_a9))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a11, fun_implies(con_a2, con_a8)), con_a8))), fun_not(fun_or(fun_implies(fun_or(con_a5, fun_or(con_a8, fun_and(con_a8, con_a9))), fun_not(con_a2)), fun_not(con_a7))))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a11, fun_implies(con_a2, con_a8)), con_a8))), fun_not(fun_or(fun_implies(fun_or(con_a5, fun_or(con_a8, fun_and(con_a8, con_a9))), fun_not(con_a2)), fun_not(con_a7))))))
}

// ---- logic descriptions propc1/2/3: start with () q1 | q2 | q3
func main() {
	var r rt.Term
	switch os.Args[len(os.Args)-1] {
	case "1":
		r = fun_q1()
	case "2":
		r = fun_q2()
	case "3":
		r = fun_q3()
	default:
		os.Stderr.WriteString("usage: propc 1|2|3\n")
		os.Exit(2)
	}
	rt.PrintResult(r)
	rt.PrintSteps()
}
