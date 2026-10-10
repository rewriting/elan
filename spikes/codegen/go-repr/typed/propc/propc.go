// Code "generated" from programs/propc.eln + propc{1,2,3}.lgi, TYPED terms
// (start with () q1 / q2 / q3; the query on the command line selects it).
package main

import (
	"os"
	"strings"

	rt "gorepr/rtt"
)

// ---- sort Prop: one struct type per operator

type Prop interface {
	rt.Node
	isProp()
}

type And struct { // and(@,@) : (Prop Prop) Prop (AC): sorted, >= 2 elements
	rt.Hdr
	Args []Prop
}
type Xor struct { // xor(@,@) : (Prop Prop) Prop (AC)
	rt.Hdr
	Args []Prop
}
type Or struct { // or(@,@) (defined: built only if no rule applies)
	rt.Hdr
	X, Y Prop
}
type Iff struct { // iff(@,@) (defined)
	rt.Hdr
	X, Y Prop
}
type Implies struct { // implies(@,@) (defined)
	rt.Hdr
	X, Y Prop
}
type Not struct { // not(@) (defined)
	rt.Hdr
	X Prop
}
type T struct{ rt.Hdr }   // t : Prop
type F struct{ rt.Hdr }   // f : Prop
type A1 struct{ rt.Hdr }  // a1 : Prop
type A2 struct{ rt.Hdr }  // a2 : Prop
type A3 struct{ rt.Hdr }  // a3 : Prop
type A4 struct{ rt.Hdr }  // a4 : Prop
type A5 struct{ rt.Hdr }  // a5 : Prop
type A6 struct{ rt.Hdr }  // a6 : Prop
type A7 struct{ rt.Hdr }  // a7 : Prop
type A8 struct{ rt.Hdr }  // a8 : Prop
type A9 struct{ rt.Hdr }  // a9 : Prop
type A10 struct{ rt.Hdr } // a10 : Prop
type A11 struct{ rt.Hdr } // a11 : Prop
type A12 struct{ rt.Hdr } // a12 : Prop
type A13 struct{ rt.Hdr } // a13 : Prop
type A14 struct{ rt.Hdr } // a14 : Prop
type A15 struct{ rt.Hdr } // a15 : Prop
type A16 struct{ rt.Hdr } // a16 : Prop
type A17 struct{ rt.Hdr } // a17 : Prop
type A18 struct{ rt.Hdr } // a18 : Prop
type Q1 struct{ rt.Hdr }  // q1 : Prop
type Q2 struct{ rt.Hdr }  // q2 : Prop
type Q3 struct{ rt.Hdr }  // q3 : Prop

func (*And) isProp()     {}
func (*Xor) isProp()     {}
func (*Or) isProp()      {}
func (*Iff) isProp()     {}
func (*Implies) isProp() {}
func (*Not) isProp()     {}
func (*T) isProp()       {}
func (*F) isProp()       {}
func (*A1) isProp()      {}
func (*A2) isProp()      {}
func (*A3) isProp()      {}
func (*A4) isProp()      {}
func (*A5) isProp()      {}
func (*A6) isProp()      {}
func (*A7) isProp()      {}
func (*A8) isProp()      {}
func (*A9) isProp()      {}
func (*A10) isProp()     {}
func (*A11) isProp()     {}
func (*A12) isProp()     {}
func (*A13) isProp()     {}
func (*A14) isProp()     {}
func (*A15) isProp()     {}
func (*A16) isProp()     {}
func (*A17) isProp()     {}
func (*A18) isProp()     {}
func (*Q1) isProp()      {}
func (*Q2) isProp()      {}
func (*Q3) isProp()      {}

func (a *And) Same(b *And) bool         { return slicesEqual(a.Args, b.Args) }
func (a *Xor) Same(b *Xor) bool         { return slicesEqual(a.Args, b.Args) }
func (a *Or) Same(b *Or) bool           { return a.X == b.X && a.Y == b.Y }
func (a *Iff) Same(b *Iff) bool         { return a.X == b.X && a.Y == b.Y }
func (a *Implies) Same(b *Implies) bool { return a.X == b.X && a.Y == b.Y }
func (a *Not) Same(b *Not) bool         { return a.X == b.X }

func slicesEqual(a, b []Prop) bool {
	if len(a) != len(b) {
		return false
	}
	for i := range a {
		if a[i] != b[i] {
			return false
		}
	}
	return true
}

func (p *And) Print(b *strings.Builder)     { rt.PrintArgs(b, "and", p.Args...) }
func (p *Xor) Print(b *strings.Builder)     { rt.PrintArgs(b, "xor", p.Args...) }
func (p *Or) Print(b *strings.Builder)      { rt.PrintArgs(b, "or", p.X, p.Y) }
func (p *Iff) Print(b *strings.Builder)     { rt.PrintArgs(b, "iff", p.X, p.Y) }
func (p *Implies) Print(b *strings.Builder) { rt.PrintArgs(b, "implies", p.X, p.Y) }
func (p *Not) Print(b *strings.Builder)     { rt.PrintArgs(b, "not", p.X) }
func (*T) Print(b *strings.Builder)         { b.WriteString("t") }
func (*F) Print(b *strings.Builder)         { b.WriteString("f") }
func (*A1) Print(b *strings.Builder)        { b.WriteString("a1") }
func (*A2) Print(b *strings.Builder)        { b.WriteString("a2") }
func (*A3) Print(b *strings.Builder)        { b.WriteString("a3") }
func (*A4) Print(b *strings.Builder)        { b.WriteString("a4") }
func (*A5) Print(b *strings.Builder)        { b.WriteString("a5") }
func (*A6) Print(b *strings.Builder)        { b.WriteString("a6") }
func (*A7) Print(b *strings.Builder)        { b.WriteString("a7") }
func (*A8) Print(b *strings.Builder)        { b.WriteString("a8") }
func (*A9) Print(b *strings.Builder)        { b.WriteString("a9") }
func (*A10) Print(b *strings.Builder)       { b.WriteString("a10") }
func (*A11) Print(b *strings.Builder)       { b.WriteString("a11") }
func (*A12) Print(b *strings.Builder)       { b.WriteString("a12") }
func (*A13) Print(b *strings.Builder)       { b.WriteString("a13") }
func (*A14) Print(b *strings.Builder)       { b.WriteString("a14") }
func (*A15) Print(b *strings.Builder)       { b.WriteString("a15") }
func (*A16) Print(b *strings.Builder)       { b.WriteString("a16") }
func (*A17) Print(b *strings.Builder)       { b.WriteString("a17") }
func (*A18) Print(b *strings.Builder)       { b.WriteString("a18") }
func (*Q1) Print(b *strings.Builder)        { b.WriteString("q1") }
func (*Q2) Print(b *strings.Builder)        { b.WriteString("q2") }
func (*Q3) Print(b *strings.Builder)        { b.WriteString("q3") }

// ---- constructors (hash-consed) and constants (declaration order)
const (
	tagAnd = iota + 1
	tagXor
)

var (
	tabAnd rt.Table[And]
	tabXor rt.Table[Xor]
)

var (
	con_t   Prop = rt.Const(&T{})
	con_f   Prop = rt.Const(&F{})
	con_a1  Prop = rt.Const(&A1{})
	con_a2  Prop = rt.Const(&A2{})
	con_a3  Prop = rt.Const(&A3{})
	con_a4  Prop = rt.Const(&A4{})
	con_a5  Prop = rt.Const(&A5{})
	con_a6  Prop = rt.Const(&A6{})
	con_a7  Prop = rt.Const(&A7{})
	con_a8  Prop = rt.Const(&A8{})
	con_a9  Prop = rt.Const(&A9{})
	con_a10 Prop = rt.Const(&A10{})
	con_a11 Prop = rt.Const(&A11{})
	con_a12 Prop = rt.Const(&A12{})
	con_a13 Prop = rt.Const(&A13{})
	con_a14 Prop = rt.Const(&A14{})
	con_a15 Prop = rt.Const(&A15{})
	con_a16 Prop = rt.Const(&A16{})
	con_a17 Prop = rt.Const(&A17{})
	con_a18 Prop = rt.Const(&A18{})
)

// arguments of x if x is a and-term (flattening)
func argsAnd(x Prop) ([]Prop, bool) {
	if a, ok := x.(*And); ok {
		return a.Args, true
	}
	return nil, false
}

// arguments of x if x is a xor-term (flattening)
func argsXor(x Prop) ([]Prop, bool) {
	if a, ok := x.(*Xor); ok {
		return a.Args, true
	}
	return nil, false
}

// elements of x seen as an argument of and / xor (flattening)
func elemsAnd(x Prop) []Prop {
	if a, ok := x.(*And); ok {
		return a.Args
	}
	return []Prop{x}
}

func elemsXor(x Prop) []Prop {
	if a, ok := x.(*Xor); ok {
		return a.Args
	}
	return []Prop{x}
}

// ACMake: one element is the element itself (no neutral declared)
func makeAnd(sorted []Prop) Prop {
	if len(sorted) == 1 {
		return sorted[0]
	}
	return rt.Intern(&tabAnd, rt.HashSlice(tagAnd, sorted), And{Args: sorted})
}

func makeXor(sorted []Prop) Prop {
	if len(sorted) == 1 {
		return sorted[0]
	}
	return rt.Intern(&tabXor, rt.HashSlice(tagXor, sorted), Xor{Args: sorted})
}

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
func fun_and(xs ...Prop) Prop {
	e := rt.ACFlatten(xs, argsAnd)
	if len(e) == 1 {
		return e[0]
	}
	// rule 1: and(x, x) with extension
	for i := 1; i < len(e); i++ {
		if e[i] == e[i-1] {
			x := e[i]
			R := rt.Without(e, i-1, i)
			rt.Step()
			return fun_and(append(R, x)...)
		}
	}
	// rule 2: and(x, t)
	for i := range e {
		if e[i] == con_t {
			x := fun_and(rt.Without(e, i, -1)...)
			rt.Step()
			return x
		}
	}
	// rule 3: and(x, f)
	for i := range e {
		if e[i] == con_f {
			rt.Step()
			return con_f
		}
	}
	// rule 4: and(x, xor(y, z)), y one element of the xor, z the rest
	for i := range e {
		if rt.Skip(e, nil, i) {
			continue
		}
		xo, ok := e[i].(*Xor)
		if !ok {
			continue
		}
		x := fun_and(rt.Without(e, i, -1)...)
		y := xo.Args[0]
		z := fun_xor(rt.Without(xo.Args, 0, -1)...)
		rt.Step()
		return fun_xor(fun_and(x, y), fun_and(x, z))
	}
	return makeAnd(e)
}

// xor(@,@) (AC):
//
//	[] xor(x, x) => f                 (xor(x,x,R...) => xor(R...,f))
//	[] xor(x, f) => x
func fun_xor(xs ...Prop) Prop {
	e := rt.ACFlatten(xs, argsXor)
	if len(e) == 1 {
		return e[0]
	}
	// rule 1: xor(x, x) with extension
	for i := 1; i < len(e); i++ {
		if e[i] == e[i-1] {
			R := rt.Without(e, i-1, i)
			rt.Step()
			return fun_xor(append(R, con_f)...)
		}
	}
	// rule 2: xor(x, f)
	for i := range e {
		if e[i] == con_f {
			x := fun_xor(rt.Without(e, i, -1)...)
			rt.Step()
			return x
		}
	}
	return makeXor(e)
}

// not(@): [] not(x) => xor(x, t)
func fun_not(v1 Prop) Prop {
	rt.Step()
	return fun_xor(v1, con_t)
}

// implies(@,@): [] implies(x, y) => not(xor(x, and(x, y)))
func fun_implies(v1, v2 Prop) Prop {
	rt.Step()
	return fun_not(fun_xor(v1, fun_and(v1, v2)))
}

// or(@,@): [] or(x, y) => xor(and(x, y), xor(x, y))
func fun_or(v1, v2 Prop) Prop {
	rt.Step()
	return fun_xor(fun_and(v1, v2), fun_xor(v1, v2))
}

// iff(@,@): [] iff(x, y) => not(xor(x, y))
func fun_iff(v1, v2 Prop) Prop {
	rt.Step()
	return fun_not(fun_xor(v1, v2))
}

// q1: [] q1 => ... (see propc.eln)
func fun_q1() Prop {
	rt.Step()
	return fun_implies(fun_and(fun_iff(fun_iff(fun_or(con_a1, con_a2), fun_or(fun_not(con_a3), fun_iff(fun_xor(con_a4, con_a5), fun_not(fun_not(fun_not(con_a6)))))), fun_not(fun_and(fun_and(con_a7, con_a8), fun_not(fun_xor(fun_xor(fun_or(con_a9, fun_and(con_a10, con_a11)), con_a2), fun_and(fun_and(con_a11, fun_xor(con_a2, fun_iff(con_a5, con_a5))), fun_xor(fun_xor(con_a7, con_a7), fun_iff(con_a9, con_a4)))))))), fun_implies(fun_iff(fun_iff(fun_or(con_a1, con_a2), fun_or(fun_not(con_a3), fun_iff(fun_xor(con_a4, con_a5), fun_not(fun_not(fun_not(con_a6)))))), fun_not(fun_and(fun_and(con_a7, con_a8), fun_not(fun_xor(fun_xor(fun_or(con_a9, fun_and(con_a10, con_a11)), con_a2), fun_and(fun_and(con_a11, fun_xor(con_a2, fun_iff(con_a5, con_a5))), fun_xor(fun_xor(con_a7, con_a7), fun_iff(con_a9, con_a4)))))))), fun_not(fun_and(fun_implies(fun_and(con_a1, con_a2), fun_not(fun_xor(fun_or(fun_or(fun_xor(fun_implies(fun_and(con_a3, con_a4), fun_implies(con_a5, con_a6)), fun_or(con_a7, con_a8)), fun_xor(fun_iff(con_a9, con_a10), con_a11)), fun_xor(fun_xor(con_a2, con_a2), con_a7)), fun_iff(fun_or(con_a4, con_a9), fun_xor(fun_not(con_a6), con_a6))))), fun_not(fun_iff(fun_not(con_a11), fun_not(con_a9))))))), fun_not(fun_and(fun_implies(fun_and(con_a1, con_a2), fun_not(fun_xor(fun_or(fun_or(fun_xor(fun_implies(fun_and(con_a3, con_a4), fun_implies(con_a5, con_a6)), fun_or(con_a7, con_a8)), fun_xor(fun_iff(con_a9, con_a10), con_a11)), fun_xor(fun_xor(con_a2, con_a2), con_a7)), fun_iff(fun_or(con_a4, con_a9), fun_xor(fun_not(con_a6), con_a6))))), fun_not(fun_iff(fun_not(con_a11), fun_not(con_a9))))))
}

// q2: [] q2 => ... (see propc.eln)
func fun_q2() Prop {
	rt.Step()
	return fun_implies(fun_and(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a9))), fun_iff(fun_not(fun_not(con_a2)), fun_implies(fun_or(con_a9, con_a6), fun_or(con_a10, con_a5)))), fun_not(fun_or(con_a9, fun_implies(fun_not(con_a8), fun_or(con_a4, con_a9))))))), fun_implies(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a9))), fun_iff(fun_not(fun_not(con_a2)), fun_implies(fun_or(con_a9, con_a6), fun_or(con_a10, con_a5)))), fun_not(fun_or(con_a9, fun_implies(fun_not(con_a8), fun_or(con_a4, con_a9))))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a4, fun_or(con_a4, con_a1)), con_a2))), fun_or(fun_or(fun_xor(fun_or(con_a4, con_a7), con_a2), fun_and(con_a8, con_a1)), fun_not(fun_not(fun_not(con_a6)))))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a4, fun_or(con_a4, con_a1)), con_a2))), fun_or(fun_or(fun_xor(fun_or(con_a4, con_a7), con_a2), fun_and(con_a8, con_a1)), fun_not(fun_not(fun_not(con_a6)))))))
}

// q3: [] q3 => ... (see propc.eln)
func fun_q3() Prop {
	rt.Step()
	return fun_implies(fun_and(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a11))), fun_implies(fun_or(con_a4, fun_and(con_a3, fun_iff(con_a1, con_a2))), fun_not(fun_not(con_a4)))), fun_xor(fun_implies(fun_implies(con_a6, con_a1), fun_not(con_a1)), fun_not(con_a9))))), fun_implies(fun_not(fun_and(fun_xor(con_a1, fun_xor(fun_or(con_a2, con_a3), con_a4)), fun_xor(fun_iff(fun_xor(fun_not(con_a5), fun_or(fun_xor(fun_iff(con_a6, con_a7), fun_iff(con_a8, con_a9)), fun_and(con_a10, con_a11))), fun_implies(fun_or(con_a4, fun_and(con_a3, fun_iff(con_a1, con_a2))), fun_not(fun_not(con_a4)))), fun_xor(fun_implies(fun_implies(con_a6, con_a1), fun_not(con_a1)), fun_not(con_a9))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a11, fun_implies(con_a2, con_a8)), con_a8))), fun_not(fun_or(fun_implies(fun_or(con_a5, fun_or(con_a8, fun_and(con_a8, con_a9))), fun_not(con_a2)), fun_not(con_a7))))))), fun_not(fun_implies(fun_implies(fun_and(fun_or(con_a1, fun_xor(fun_xor(con_a2, con_a3), fun_not(con_a4))), fun_not(fun_xor(con_a5, fun_and(con_a6, con_a7)))), fun_implies(fun_xor(fun_implies(con_a8, con_a9), con_a10), fun_xor(fun_and(con_a11, fun_implies(con_a2, con_a8)), con_a8))), fun_not(fun_or(fun_implies(fun_or(con_a5, fun_or(con_a8, fun_and(con_a8, con_a9))), fun_not(con_a2)), fun_not(con_a7))))))
}

// ---- logic descriptions propc1/2/3: start with () q1 | q2 | q3
func main() {
	var r Prop
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
	rt.PrintResult(r.Print)
	rt.PrintSteps()
}
