// Package rtu: generic nodes canonicalised with the standard library's
// unique.Make (G2 variant a, second table option). A term is a
// unique.Handle of a comparable node value; handles compare by pointer.
// unique needs a comparable (fixed-size) value: nodes have at most 3
// arguments here; an n-ary AC node would need a binary (comb) encoding or a
// side table, so this runtime only serves free symbols (nqueens).
package rtu

import (
	"bufio"
	"os"
	"strconv"
	"strings"
	"unique"

	"gorepr/rtstat"
)

// Term is a canonical node: equality is pointer equality of the handle.
type Term struct{ h unique.Handle[node] }

type node struct {
	sym   int32
	iv    int32
	arity int32
	a     [3]Term
}

type Cont = func(Term) bool

func (t Term) Sym() int32     { return t.h.Value().sym }
func (t Term) Int() int32     { return t.h.Value().iv }
func (t Term) Arg(i int) Term { return t.h.Value().a[i] }
func (t Term) IsZero() bool   { return t == Term{} }
func mk(n node) Term          { rtstat.Created++; return Term{unique.Make(n)} }
func Mk0(sym int32) Term      { return mk(node{sym: sym}) }
func Mk2(sym int32, a, b Term) Term {
	return mk(node{sym: sym, arity: 2, a: [3]Term{a, b}})
}

// Symbol table and printing (as rtg).
type Symbol struct {
	Name   string
	Arity  int
	Format string
}

var Syms []Symbol

const SymInt int32 = 0

func NewSym(name string, arity int, format string) int32 {
	Syms = append(Syms, Symbol{name, arity, format})
	return int32(len(Syms) - 1)
}

const smallMin, smallMax = 1024, 1024

var small [smallMin + smallMax]Term

func init() {
	NewSym("int", 0, "")
	for i := range small {
		small[i] = mk(node{sym: SymInt, iv: int32(i - smallMin)})
	}
}

func MkInt(v int32) Term {
	if v >= -smallMin && v < smallMax {
		return small[v+smallMin]
	}
	return mk(node{sym: SymInt, iv: v})
}

func Step() { rtstat.Steps++ }

func write(sb *strings.Builder, t Term) {
	n := t.h.Value()
	if n.sym == SymInt {
		sb.WriteString(strconv.Itoa(int(n.iv)))
		return
	}
	s := &Syms[n.sym]
	if n.arity == 0 {
		sb.WriteString(s.Name)
		return
	}
	if s.Format == "" {
		sb.WriteString(s.Name)
		sb.WriteByte('(')
		for i := 0; i < int(n.arity); i++ {
			if i > 0 {
				sb.WriteByte(',')
			}
			write(sb, n.a[i])
		}
		sb.WriteByte(')')
		return
	}
	k := 0
	for i := 0; i < len(s.Format); i++ {
		if s.Format[i] == '@' {
			write(sb, n.a[k])
			k++
		} else {
			sb.WriteByte(s.Format[i])
		}
	}
}

var Out = bufio.NewWriterSize(os.Stdout, 1<<16)

func PrintResult(t Term) {
	var sb strings.Builder
	sb.WriteString("result = ")
	write(&sb, t)
	sb.WriteByte('\n')
	Out.WriteString(sb.String())
}

func PrintSteps() {
	Out.WriteString("rewrite_step = " + strconv.FormatInt(rtstat.Steps, 10) + "\n")
	Out.Flush()
	rtstat.Report()
}

func QueryInt() int32 {
	v, err := strconv.ParseInt(os.Args[len(os.Args)-1], 10, 32)
	if err != nil {
		os.Exit(2)
	}
	return int32(v)
}
