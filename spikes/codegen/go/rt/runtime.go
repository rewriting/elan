// Package rt is the runtime library of the Go back-end spike for ELAN:
// maximally shared terms (hash-consing), builtin integers and booleans,
// AC canonical form (flattened, sorted by unique id, duplicates kept),
// AC matching helpers and printing. Written once, used by every program.
package rt

import (
	"bufio"
	"os"
	"slices"
	"strconv"
	"strings"
)

// Term is a pointer to a unique (hash-consed) node: structural equality is
// pointer equality.
type Term = *Node

// Node is an immutable term node.
type Node struct {
	Sym  int32  // symbol code
	ID   uint32 // unique id, creation order; total order for AC sorting
	Int  int64  // value of a builtin integer (Sym == SymInt)
	hash uint64 // cached hash (for table growth)
	Args []Term // arguments (AC symbols: n >= 2 flattened, sorted)
}

// Cont is a success continuation: called once per result, returns true to stop.
type Cont = func(Term) bool

// Symbol describes a function symbol.
type Symbol struct {
	Name   string
	Arity  int  // fixed arity (AC symbols: 2 in the signature, n >= 2 at run time)
	AC     bool // associative-commutative: flattened canonical form
	Format string
	// Format: "" -> prefix name(a,b); otherwise an ELAN mixfix template with
	// '@' placeholders ("@.@", "Fib[farg=@,val=@]", "@ U @").
}

// Syms is the symbol table, indexed by symbol code.
var Syms []Symbol

// Builtin symbol codes.
const (
	SymInt   int32 = 0
	SymTrue  int32 = 1
	SymFalse int32 = 2
)

// Steps counts rule applications (rewrite_step).
var Steps int64

// NewSym registers a symbol and returns its code.
func NewSym(name string, arity int, ac bool, format string) int32 {
	Syms = append(Syms, Symbol{name, arity, ac, format})
	return int32(len(Syms) - 1)
}

// ---------------------------------------------------------------------------
// Unique table (open addressing, linear probing, power-of-two size).

var (
	table  = make([]Term, 1<<12)
	count  int
	nextID uint32
)

func mix(h uint64) uint64 {
	h ^= h >> 33
	h *= 0xff51afd7ed558ccd
	h ^= h >> 33
	return h
}

func hashKey(sym int32, iv int64, args []Term) uint64 {
	h := uint64(sym)*0x9E3779B97F4A7C15 ^ uint64(iv)*0xC2B2AE3D27D4EB4F
	for _, a := range args {
		h = (h ^ uint64(a.ID)) * 0x100000001B3
	}
	return mix(h)
}

func sameArgs(a, b []Term) bool {
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

func grow() {
	old := table
	table = make([]Term, 2*len(old))
	mask := uint64(len(table) - 1)
	for _, n := range old {
		if n != nil {
			i := n.hash & mask
			for table[i] != nil {
				i = (i + 1) & mask
			}
			table[i] = n
		}
	}
}

// mk returns the unique node (sym, iv, args). args is copied on insertion,
// never retained, so callers may pass stack buffers.
func mk(sym int32, iv int64, args []Term) Term {
	h := hashKey(sym, iv, args)
	mask := uint64(len(table) - 1)
	i := h & mask
	for {
		e := table[i]
		if e == nil {
			break
		}
		if e.hash == h && e.Sym == sym && e.Int == iv && sameArgs(e.Args, args) {
			return e
		}
		i = (i + 1) & mask
	}
	n := &Node{Sym: sym, ID: nextID, Int: iv, hash: h}
	nextID++
	if len(args) > 0 {
		n.Args = slices.Clone(args)
	}
	table[i] = n
	count++
	if 2*count > len(table) {
		grow()
	}
	return n
}

// Mk0 .. Mk3, Mk: build a free (non-AC) term from its (normal) arguments.
func Mk0(sym int32) Term { return mk(sym, 0, nil) }
func Mk1(sym int32, a Term) Term {
	buf := [1]Term{a}
	return mk(sym, 0, buf[:])
}
func Mk2(sym int32, a, b Term) Term {
	buf := [2]Term{a, b}
	return mk(sym, 0, buf[:])
}
func Mk3(sym int32, a, b, c Term) Term {
	buf := [3]Term{a, b, c}
	return mk(sym, 0, buf[:])
}
func Mk(sym int32, args ...Term) Term { return mk(sym, 0, args) }

// ---------------------------------------------------------------------------
// Builtins: int and bool.

var True, False Term

func init() {
	NewSym("int", 0, false, "")
	NewSym("true", 0, false, "")
	NewSym("false", 0, false, "")
	True = Mk0(SymTrue)
	False = Mk0(SymFalse)
}

func MkInt(v int64) Term { return mk(SymInt, v, nil) }
func Bool(b bool) Term {
	if b {
		return True
	}
	return False
}

func Plus(a, b Term) Term  { return MkInt(a.Int + b.Int) }
func Minus(a, b Term) Term { return MkInt(a.Int - b.Int) }
func Mod(a, b Term) Term   { return MkInt(a.Int % b.Int) }
func Gt(a, b Term) Term    { return Bool(a.Int > b.Int) }
func EqInt(a, b Term) Term { return Bool(a.Int == b.Int) }
func NeqInt(a, b Term) Term {
	return Bool(a.Int != b.Int)
}
func And(a, b Term) Term { return Bool(a == True && b == True) }
func Not(a Term) Term    { return Bool(a != True) }
func IsInt(t Term) bool  { return t.Sym == SymInt }

// ---------------------------------------------------------------------------
// AC canonical form and matching helpers.

func byID(a, b Term) int {
	if a.ID < b.ID {
		return -1
	}
	if a.ID > b.ID {
		return 1
	}
	return 0
}

// ACFlatten returns a fresh sorted slice of the elements of sym(xs...),
// splicing the arguments of nested sym nodes.
func ACFlatten(sym int32, xs []Term) []Term {
	n := 0
	for _, x := range xs {
		if x.Sym == sym {
			n += len(x.Args)
		} else {
			n++
		}
	}
	e := make([]Term, 0, n)
	for _, x := range xs {
		if x.Sym == sym {
			e = append(e, x.Args...)
		} else {
			e = append(e, x)
		}
	}
	slices.SortFunc(e, byID)
	return e
}

// ACMake builds the canonical AC term from sorted elements: one element is
// the element itself, none is the neutral (nil if the symbol has none).
func ACMake(sym int32, sorted []Term, neutral Term) Term {
	switch len(sorted) {
	case 0:
		return neutral
	case 1:
		return sorted[0]
	}
	return mk(sym, 0, sorted)
}

// ACElems views t as a multiset of sym: its arguments, or the singleton {t}.
func ACElems(sym int32, t Term) []Term {
	if t.Sym == sym {
		return t.Args
	}
	return []Term{t}
}

// Skip reports whether position i of the sorted multiset e must be skipped
// when enumerating distinct choices: already used, or equal to an unused
// previous element (the same choice was already tried).
func Skip(e []Term, used []bool, i int) bool {
	if used != nil && used[i] {
		return true
	}
	return i > 0 && e[i] == e[i-1] && (used == nil || !used[i-1])
}

// Rest returns the unused elements of e (still sorted).
func Rest(e []Term, used []bool) []Term {
	r := make([]Term, 0, len(e))
	for i, x := range e {
		if !used[i] {
			r = append(r, x)
		}
	}
	return r
}

// Without returns e minus the positions i (and j if j >= 0).
func Without(e []Term, i, j int) []Term {
	r := make([]Term, 0, len(e))
	for p, x := range e {
		if p != i && p != j {
			r = append(r, x)
		}
	}
	return r
}

// ---------------------------------------------------------------------------
// Printing.

func Format(t Term) string {
	var sb strings.Builder
	write(&sb, t)
	return sb.String()
}

func write(sb *strings.Builder, t Term) {
	if t.Sym == SymInt {
		sb.WriteString(strconv.FormatInt(t.Int, 10))
		return
	}
	s := &Syms[t.Sym]
	if len(t.Args) == 0 {
		if s.Format != "" {
			sb.WriteString(s.Format)
		} else {
			sb.WriteString(s.Name)
		}
		return
	}
	if s.Format == "" {
		sb.WriteString(s.Name)
		sb.WriteByte('(')
		for i, a := range t.Args {
			if i > 0 {
				sb.WriteByte(',')
			}
			write(sb, a)
		}
		sb.WriteByte(')')
		return
	}
	if s.AC { // infix AC: "@ U @" -> join with the separator
		sep := s.Format[1 : len(s.Format)-1]
		for i, a := range t.Args {
			if i > 0 {
				sb.WriteString(sep)
			}
			write(sb, a)
		}
		return
	}
	k := 0
	for i := 0; i < len(s.Format); i++ {
		if s.Format[i] == '@' {
			write(sb, t.Args[k])
			k++
		} else {
			sb.WriteByte(s.Format[i])
		}
	}
}

// Out is the buffered standard output.
var Out = bufio.NewWriterSize(os.Stdout, 1<<16)

func PrintResult(t Term) {
	Out.WriteString("result = ")
	Out.WriteString(Format(t))
	Out.WriteString("\n")
}

func PrintSteps() {
	Out.WriteString("rewrite_step = ")
	Out.WriteString(strconv.FormatInt(Steps, 10))
	Out.WriteString("\n")
	Out.Flush()
}

// QueryInt parses the integer query from the command line.
func QueryInt() int64 {
	if len(os.Args) < 2 {
		os.Stderr.WriteString("usage: " + os.Args[0] + " <int>\n")
		os.Exit(2)
	}
	v, err := strconv.ParseInt(os.Args[1], 10, 64)
	if err != nil {
		os.Stderr.WriteString("bad query: " + os.Args[1] + "\n")
		os.Exit(2)
	}
	return v
}
