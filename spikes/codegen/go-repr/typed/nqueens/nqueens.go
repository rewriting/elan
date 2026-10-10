// Code "generated" from programs/nqueens.eln + nqueens.lgi, TYPED terms
// (start with (queens_strat) queens(query)).
package main

import (
	"strings"

	rt "gorepr/rtt"
)

// ---- sorts
// int, bool: builtins -> int32, bool.

// QInt is sort qint. Only constructor: the injection @ : (int) qint, no
// recursion, scalar fields only: an unboxed value (not hash-consed).
type QInt int32

// List is sort list: one struct type per operator (defined ones too, so a
// term on which no rule applies stays representable).
type List interface {
	rt.Node
	isList()
}

type Nil struct{ rt.Hdr } // nil
type Cons struct {        // @.@ : (int list) list
	rt.Hdr
	Hd int32
	Tl List
}
type GenerateList struct { // generate_list(@) : (int) list
	rt.Hdr
	N int32
}
type Queens2 struct { // queens(@,@) : (int int) list
	rt.Hdr
	N, Size int32
}
type Queens1 struct { // queens(@) : (int) list
	rt.Hdr
	N int32
}

func (*Nil) isList()          {}
func (*Cons) isList()         {}
func (*GenerateList) isList() {}
func (*Queens2) isList()      {}
func (*Queens1) isList()      {}

// Same: shallow equality of the fields (children are shared: ==).
func (a *Cons) Same(b *Cons) bool                 { return a.Hd == b.Hd && a.Tl == b.Tl }
func (a *GenerateList) Same(b *GenerateList) bool { return a.N == b.N }
func (a *Queens2) Same(b *Queens2) bool           { return a.N == b.N && a.Size == b.Size }
func (a *Queens1) Same(b *Queens1) bool           { return a.N == b.N }

func (*Nil) Print(b *strings.Builder) { b.WriteString("nil") }
func (c *Cons) Print(b *strings.Builder) {
	rt.PrintInt(b, c.Hd)
	b.WriteByte('.')
	c.Tl.Print(b)
}
func (g *GenerateList) Print(b *strings.Builder) {
	b.WriteString("generate_list(")
	rt.PrintInt(b, g.N)
	b.WriteByte(')')
}
func (q *Queens2) Print(b *strings.Builder) {
	b.WriteString("queens(")
	rt.PrintInt(b, q.N)
	b.WriteByte(',')
	rt.PrintInt(b, q.Size)
	b.WriteByte(')')
}
func (q *Queens1) Print(b *strings.Builder) {
	b.WriteString("queens(")
	rt.PrintInt(b, q.N)
	b.WriteByte(')')
}

// ---- constructors (hash-consed) and constants
const (
	tagCons = iota + 1
	tagGenerateList
	tagQueens2
	tagQueens1
)

var (
	tabCons    rt.Table[Cons]
	tabQueens2 rt.Table[Queens2]
)

func mkCons(hd int32, tl List) *Cons {
	return rt.Intern(&tabCons, rt.Hash(tagCons, uint64(hd), rt.Ord(tl)), Cons{Hd: hd, Tl: tl})
}
func mkQueens2(n, size int32) *Queens2 {
	return rt.Intern(&tabQueens2, rt.Hash(tagQueens2, uint64(n), uint64(size)), Queens2{N: n, Size: size})
}

var con_nil = rt.Const(&Nil{})

// ---- symbol functions (normalisation, unlabelled rules, innermost)

// q2i(@): [] q2i(n) => n
func fun_q2i(v1 QInt) int32 {
	rt.Step()
	return int32(v1)
}

// generate_list(@): [] generate_list(0) => nil ; [] generate_list(n) => n . generate_list(n-1)
func fun_generate_list(v1 int32) List {
	if v1 == 0 {
		rt.Step()
		return con_nil
	}
	rt.Step()
	return mkCons(v1, fun_generate_list(v1-1))
}

// queens(@): [] queens(n) => queens(n,n)
func fun_queens1(v1 int32) List {
	rt.Step()
	return fun_queens2(v1, v1)
}

// queens(@,@): only labelled rules (queens_0, queens_n): constructor
func fun_queens2(v1, v2 int32) List {
	return mkQueens2(v1, v2)
}

// noattack(@,@,@):
//
//	[] noattack(diff,d,nil) => true
//	[] noattack(diff,d,p.l) => d!=p and d-p!=diff and p-d!=diff and noattack(diff+1,d,l)
//
// bool is a builtin: a stuck noattack(...) has no representation (panic).
func fun_noattack(v1, v2 int32, v3 List) bool {
	switch v3 := v3.(type) {
	case *Nil:
		rt.Step()
		return true
	case *Cons:
		p, l := v3.Hd, v3.Tl
		// builtin and is strict (innermost): the recursive call is always made
		rec := fun_noattack(v1+1, v2, l)
		rt.Step()
		return v2 != p && v2-p != v1 && p-v2 != v1 && rec
	default:
		panic("noattack: no rule applies")
	}
}

// ---- labelled rules: match, conditions, wheres, then k(rhs)

// [range_rule] x => x-1 if x > 1        (sort qint)
func rule_range_rule(t QInt, k rt.Cont[QInt]) bool {
	x := int32(t)
	if !(x > 1) {
		return false
	}
	rt.Step()
	return k(QInt(x - 1))
}

// [queens_0] queens(0,size) => nil
func rule_queens_0(t List, k rt.Cont[List]) bool {
	q, ok := t.(*Queens2)
	if !ok || q.N != 0 {
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
func rule_queens_n(t List, k rt.Cont[List]) bool {
	q, ok := t.(*Queens2)
	if !ok {
		return false
	}
	n, size := q.N, q.Size
	if !(n > 0) {
		return false
	}
	return strat_queens_strat(fun_queens2(n-1, size), func(ql List) bool {
		return strat_range(QInt(size), func(xx QInt) bool {
			x := fun_q2i(xx)
			if !fun_noattack(1, x, ql) {
				return false
			}
			rt.Step()
			return k(mkCons(x, ql))
		})
	})
}

// ---- strategies

// dc(range_rule)
func strat_range_1(t QInt, k rt.Cont[QInt]) bool {
	got := false
	stop := rule_range_rule(t, func(r QInt) bool { got = true; return k(r) })
	if got {
		return stop
	}
	return false
}

// [] range => iterate*(dc(range_rule))
func strat_range(t QInt, k rt.Cont[QInt]) bool {
	if k(t) {
		return true
	}
	return strat_range_1(t, func(r QInt) bool { return strat_range(r, k) })
}

// [] queens_strat => dk(queens_0, queens_n)
func strat_queens_strat(t List, k rt.Cont[List]) bool {
	if rule_queens_0(t, k) {
		return true
	}
	return rule_queens_n(t, k)
}

// [] det_queens_strat => first one(queens_0, queens_n)
func strat_det_queens_strat(t List, k rt.Cont[List]) bool {
	var res List
	one := func(r List) bool { res = r; return true }
	if rule_queens_0(t, one) || rule_queens_n(t, one) {
		return k(res)
	}
	return false
}

var _, _ = strat_det_queens_strat, fun_generate_list

// ---- logic description: start with (queens_strat) queens(query)
func main() {
	query := rt.QueryInt()
	strat_queens_strat(fun_queens1(query), func(r List) bool {
		rt.PrintResult(r.Print)
		return false // all results
	})
	rt.PrintSteps()
}
