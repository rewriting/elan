// Code "generated" from programs/efib.eln + efib.lgi, TYPED terms
// (start with () query, query = go(N)). Subsorts by injection
// (Fib, Compute -> Object -> Space) are interface embedding: a *FibOp is an
// Object and a Space, no injection node.
package main

import (
	"strings"

	rt "gorepr/rtt"
)

// ---- sorts
// builtinInt -> int32, bool -> bool.

// EInt is sort eInt. Constructors: @ : (builtinInt) eInt and UNDEF, scalar
// only: an unboxed value. The defined operators go(@), result(@,@) of this
// sort have no representation when stuck (panic).
type EInt struct {
	Undef bool  // UNDEF
	V     int32 // @(V) when !Undef
}

var UNDEF = EInt{Undef: true}

func (e EInt) Print(b *strings.Builder) {
	if e.Undef {
		b.WriteString("UNDEF")
	} else {
		rt.PrintInt(b, e.V)
	}
}

type Space interface {
	rt.Node
	isSpace()
}
type Object interface {
	Space // @ : (Object) Space
	isObject()
}
type Fib interface {
	Object // @ : (Fib) Object
	isFib()
}
type Compute interface {
	Object // @ : (Compute) Object
	isCompute()
}

type U struct { // @ U @ : (Space Space) Space (AC): sorted, >= 2 elements
	rt.Hdr
	Args []Space
}
type Empty struct{ rt.Hdr } // empty : Space
type FibOp struct {         // Fib[farg=@,val=@] : (builtinInt eInt) Fib
	rt.Hdr
	Farg int32
	Val  EInt
}
type ComputeOp struct { // Compute[question=@,answer=@] : (builtinInt eInt) Compute
	rt.Hdr
	Question int32
	Answer   EInt
}

func (*U) isSpace()           {}
func (*Empty) isSpace()       {}
func (*FibOp) isSpace()       {}
func (*FibOp) isObject()      {}
func (*FibOp) isFib()         {}
func (*ComputeOp) isSpace()   {}
func (*ComputeOp) isObject()  {}
func (*ComputeOp) isCompute() {}
func (a *U) Same(b *U) bool   { return slicesEqual(a.Args, b.Args) }
func (a *FibOp) Same(b *FibOp) bool {
	return a.Farg == b.Farg && a.Val == b.Val
}
func (a *ComputeOp) Same(b *ComputeOp) bool {
	return a.Question == b.Question && a.Answer == b.Answer
}

func slicesEqual(a, b []Space) bool {
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

func (u *U) Print(b *strings.Builder) {
	for i, x := range u.Args {
		if i > 0 {
			b.WriteString(" U ")
		}
		x.Print(b)
	}
}
func (*Empty) Print(b *strings.Builder) { b.WriteString("empty") }
func (f *FibOp) Print(b *strings.Builder) {
	b.WriteString("Fib[farg=")
	rt.PrintInt(b, f.Farg)
	b.WriteString(",val=")
	f.Val.Print(b)
	b.WriteByte(']')
}
func (c *ComputeOp) Print(b *strings.Builder) {
	b.WriteString("Compute[question=")
	rt.PrintInt(b, c.Question)
	b.WriteString(",answer=")
	c.Answer.Print(b)
	b.WriteByte(']')
}

// ---- constructors and constants
const (
	tagU = iota + 1
	tagFib
	tagCompute
)

var (
	tabU   rt.Table[U]
	tabFib rt.Table[FibOp]
)

func evalHash(e EInt) uint64 {
	if e.Undef {
		return 1 << 40
	}
	return uint64(uint32(e.V))
}

func mkFib(farg int32, val EInt) *FibOp {
	return rt.Intern(&tabFib, rt.Hash(tagFib, uint64(farg), evalHash(val)), FibOp{Farg: farg, Val: val})
}

var con_empty Space = rt.Const(&Empty{})

// ---- symbol functions

// arguments of x if x is a U-term (flattening)
func argsU(x Space) ([]Space, bool) {
	if a, ok := x.(*U); ok {
		return a.Args, true
	}
	return nil, false
}

// elements of x seen as an argument of U (flattening)
func elemsU(x Space) []Space {
	if u, ok := x.(*U); ok {
		return u.Args
	}
	return []Space{x}
}

// ACMake for U: one element is the element itself, none is empty.
func makeU(sorted []Space) Space {
	switch len(sorted) {
	case 0:
		return con_empty
	case 1:
		return sorted[0]
	}
	return rt.Intern(&tabU, rt.HashSlice(tagU, sorted), U{Args: sorted})
}

// @ U @ (AC): flatten, sort; no unlabelled rules for U.
func fun_U(xs ...Space) Space {
	return makeU(rt.ACFlatten(xs, argsU))
}

// Fib[farg=@,val=@]: constructor
func fun_Fib(v1 int32, v2 EInt) *FibOp { return mkFib(v1, v2) }

// occursFib(@,@):
//
//	[] occursFib(S U Fib[farg=n,val=v],n) => true
//	[] occursFib(S,n)                     => false
func fun_occursFib(v1 Space, v2 int32) bool {
	// rule 1: AC pattern S U Fib[farg=n,val=v], n non-linear (S, v unused)
	e := elemsU(v1)
	for i := range e {
		if rt.Skip(e, nil, i) {
			continue
		}
		p, ok := e[i].(*FibOp)
		if !ok || p.Farg != v2 {
			continue
		}
		rt.Step()
		return true
	}
	// rule 2
	rt.Step()
	return false
}

// go(@): [] go(n) => result(S,n)
//
//	where S:=(loop) empty U Fib[farg=0,val=1] U Fib[farg=1,val=1] U Fib[farg=n,val=UNDEF]
func fun_go(v1 int32) EInt {
	var res EInt
	ok := strat_loop(fun_U(con_empty, fun_Fib(0, EInt{V: 1}), fun_Fib(1, EInt{V: 1}), fun_Fib(v1, UNDEF)),
		func(S Space) bool {
			res = fun_result(S, v1)
			return true // unlabelled rule: first result of the where
		})
	if ok {
		rt.Step()
		return res
	}
	panic("go: no rule applies (eInt is unboxed)")
}

// result(@,@): [] result(S U Fib[farg=n,val=v],n) => v
func fun_result(v1 Space, v2 int32) EInt {
	e := elemsU(v1)
	for i := range e {
		if rt.Skip(e, nil, i) {
			continue
		}
		p, ok := e[i].(*FibOp)
		if !ok || p.Farg != v2 {
			continue
		}
		rt.Step()
		return p.Val
	}
	panic("result: no rule applies (eInt is unboxed)")
}

// ---- labelled rules (for Space)

// [init1] S U Fib[farg=0,val=UNDEF] => S U Fib[farg=0,val=1]
func rule_init1(t Space, k rt.Cont[Space]) bool {
	e := elemsU(t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p, ok := e[i].(*FibOp)
		if !ok || p.Farg != 0 || p.Val != UNDEF {
			continue
		}
		used[i] = true
		S := makeU(rt.Rest(e, used))
		rt.Step()
		if k(fun_U(S, fun_Fib(0, EInt{V: 1}))) {
			return true
		}
		used[i] = false
	}
	return false
}

// [init2] S U Fib[farg=1,val=UNDEF] => S U Fib[farg=1,val=1]
func rule_init2(t Space, k rt.Cont[Space]) bool {
	e := elemsU(t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p, ok := e[i].(*FibOp)
		if !ok || p.Farg != 1 || p.Val != UNDEF {
			continue
		}
		used[i] = true
		S := makeU(rt.Rest(e, used))
		rt.Step()
		if k(fun_U(S, fun_Fib(1, EInt{V: 1}))) {
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
func rule_rec1(t Space, k rt.Cont[Space]) bool {
	e := elemsU(t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p, ok := e[i].(*FibOp)
		if !ok || p.Val != UNDEF {
			continue
		}
		n := p.Farg
		used[i] = true
		S := makeU(rt.Rest(e, used))
		if n > 2 && !fun_occursFib(S, n-1) {
			rt.Step()
			if k(fun_U(S, fun_Fib(n, UNDEF), fun_Fib(n-1, UNDEF))) {
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
func rule_rec2(t Space, k rt.Cont[Space]) bool {
	e := elemsU(t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p, ok := e[i].(*FibOp)
		if !ok || p.Val != UNDEF {
			continue
		}
		n := p.Farg
		used[i] = true
		S := makeU(rt.Rest(e, used))
		if n > 3 && !fun_occursFib(S, n-2) {
			rt.Step()
			if k(fun_U(S, fun_Fib(n, UNDEF), fun_Fib(n-2, UNDEF))) {
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
func rule_compute(t Space, k rt.Cont[Space]) bool {
	e := elemsU(t)
	used := make([]bool, len(e))
	for i := range e {
		if rt.Skip(e, used, i) {
			continue
		}
		p1, ok := e[i].(*FibOp)
		if !ok || p1.Val.Undef { // v1 : builtinInt
			continue
		}
		n1, v1 := p1.Farg, p1.Val.V
		used[i] = true
		for j := range e {
			if rt.Skip(e, used, j) {
				continue
			}
			p2, ok := e[j].(*FibOp)
			if !ok || p2.Val.Undef {
				continue
			}
			n2, v2 := p2.Farg, p2.Val.V
			if !(n1 == n2+1) {
				continue
			}
			used[j] = true
			for l := range e {
				if rt.Skip(e, used, l) {
					continue
				}
				p3, ok := e[l].(*FibOp)
				if !ok || p3.Val != UNDEF {
					continue
				}
				n := p3.Farg
				if !(n == n1+1) {
					continue
				}
				used[l] = true
				S := makeU(rt.Rest(e, used))
				rt.Step()
				if k(fun_U(S, fun_Fib(n1, EInt{V: v1}), fun_Fib(n2, EInt{V: v2}),
					fun_Fib(n, EInt{V: v1 + v2%1000000}))) {
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
func strat_loop_1(t Space, k rt.Cont[Space]) bool {
	var res Space
	one := func(r Space) bool { res = r; return true }
	if rule_rec1(t, one) || rule_rec2(t, one) || rule_compute(t, one) {
		return k(res)
	}
	return false
}

// [] loop => repeat*(first one(rec1, rec2, compute))
// (the body is deterministic: repeat* is compiled as a loop)
func strat_loop(t Space, k rt.Cont[Space]) bool {
	for {
		var next Space
		if !strat_loop_1(t, func(r Space) bool { next = r; return true }) {
			return k(t)
		}
		t = next
	}
}

// ---- logic description: start with () go(query)
func main() {
	query := rt.QueryInt()
	rt.PrintResult(fun_go(query).Print)
	rt.PrintSteps()
}
