// Package rtg is the runtime of the GENERIC representation (G2 variant a):
// every term is a *Node {symbol code, int payload, arguments}, maximally
// shared through a unique table. The table holds weak pointers (table_weak.go,
// default) or strong ones (table_strong.go, build tag "strong", to compare
// with the first spike). Same model and same generated code shape as
// ../go (spikes/codegen/go/rt): AC canonical form (flattened, sorted),
// matching helpers, printing.
package rtg

import (
	"bufio"
	"os"
	"slices"
	"strconv"
	"strings"

	"gorepr/rtstat"
)

// Term is a pointer to a unique (hash-consed) node: equality is pointer
// equality.
type Term = *Node

// Node is an immutable term node.
type Node struct {
	Sym  int32  // symbol code
	Int  int32  // value of a builtin integer (Sym == SymInt), 32-bit wrap
	Ord  uint64 // AC order key: creation id, or structural hash (ELAN_ACORD=hash)
	hash uint64 // table hash (from Sym, Int and the children's Ord)
	Args []Term // arguments (AC symbols: n >= 2 flattened, sorted by Ord)
}

// Nodes of arity 1..3 carry their argument array inline: one allocation.
type node1 struct {
	Node
	buf [1]Term
}
type node2 struct {
	Node
	buf [2]Term
}
type node3 struct {
	Node
	buf [3]Term
}

// Cont is a success continuation: called once per result, returns true to stop.
type Cont = func(Term) bool

// Symbol describes a function symbol.
type Symbol struct {
	Name   string
	Arity  int
	AC     bool
	Format string // "" prefix; else mixfix template with '@' ("@.@", "@ U @")
}

// Syms is the symbol table, indexed by symbol code.
var Syms []Symbol

// Builtin symbol codes.
const (
	SymInt   int32 = 0
	SymTrue  int32 = 1
	SymFalse int32 = 2
)

// NewSym registers a symbol and returns its code.
func NewSym(name string, arity int, ac bool, format string) int32 {
	Syms = append(Syms, Symbol{name, arity, ac, format})
	return int32(len(Syms) - 1)
}

// ---------------------------------------------------------------------------
// Hash-consing (the table itself is in table_weak.go / table_strong.go).

var nextID uint64

func mix(h uint64) uint64 {
	h ^= h >> 33
	h *= 0xff51afd7ed558ccd
	h ^= h >> 33
	return h | 1 // never 0: 0 marks an empty slot
}

func hashKey(sym, iv int32, args []Term) uint64 {
	h := uint64(uint32(sym))*0x9E3779B97F4A7C15 ^ uint64(uint32(iv))*0xC2B2AE3D27D4EB4F
	for _, a := range args {
		h = (h ^ a.Ord) * 0x100000001B3
	}
	return mix(h)
}

func (n *Node) same(sym, iv int32, args []Term) bool {
	if n.Sym != sym || n.Int != iv || len(n.Args) != len(args) {
		return false
	}
	for i := range args {
		if n.Args[i] != args[i] {
			return false
		}
	}
	return true
}

// newNode allocates a node; args is copied (callers may pass stack buffers).
func newNode(sym, iv int32, h uint64, args []Term) *Node {
	var n *Node
	switch len(args) {
	case 0:
		n = &Node{}
	case 1:
		b := &node1{}
		b.buf[0] = args[0]
		b.Args = b.buf[:]
		n = &b.Node
	case 2:
		b := &node2{}
		b.buf = [2]Term{args[0], args[1]}
		b.Args = b.buf[:]
		n = &b.Node
	case 3:
		b := &node3{}
		b.buf = [3]Term{args[0], args[1], args[2]}
		b.Args = b.buf[:]
		n = &b.Node
	default:
		n = &Node{Args: slices.Clone(args)}
	}
	n.Sym, n.Int, n.hash = sym, iv, h
	if rtstat.HashOrder {
		n.Ord = h
	} else {
		nextID++
		n.Ord = nextID
	}
	rtstat.Created++
	return n
}

// mk returns the unique node (sym, iv, args).
func mk(sym, iv int32, args []Term) Term {
	h := hashKey(sym, iv, args)
	if n := lookup(h, sym, iv, args); n != nil {
		rtstat.Hits++
		return n
	}
	n := newNode(sym, iv, h, args)
	insert(n)
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
// Builtins: int (32-bit, wrapping) and bool.

var True, False Term

func init() {
	NewSym("int", 0, false, "")
	NewSym("true", 0, false, "")
	NewSym("false", 0, false, "")
	True = Mk0(SymTrue)
	False = Mk0(SymFalse)
	if smallInts {
		for i := range small {
			small[i] = mk(SymInt, int32(i-smallMin), nil)
		}
	}
	rtstat.Live = liveEntries
	rtstat.Slots = func() int { return len(table) }
}

// MkInt returns the integer node v. Small integers come from a table built
// at start-up (no hash lookup); build tag "nosmallint" disables it (as the
// first spike, which interned every integer).
func MkInt(v int32) Term {
	if smallInts && v >= -smallMin && v < smallMax {
		return small[v+smallMin]
	}
	return mk(SymInt, v, nil)
}

const smallMin, smallMax = 1024, 1024

var small [smallMin + smallMax]Term

func Bool(b bool) Term {
	if b {
		return True
	}
	return False
}

func Plus(a, b Term) Term  { return MkInt(a.Int + b.Int) }
func Minus(a, b Term) Term { return MkInt(a.Int - b.Int) }
func Mod(a, b Term) Term {
	if b.Int == 0 {
		return MkInt(0)
	}
	return MkInt(a.Int % b.Int)
}
func Gt(a, b Term) Term     { return Bool(a.Int > b.Int) }
func EqInt(a, b Term) Term  { return Bool(a.Int == b.Int) }
func NeqInt(a, b Term) Term { return Bool(a.Int != b.Int) }
func And(a, b Term) Term    { return Bool(a == True && b == True) }
func Not(a Term) Term       { return Bool(a != True) }
func IsInt(t Term) bool     { return t.Sym == SymInt }

// ---------------------------------------------------------------------------
// AC canonical form and matching helpers.

// Compare is the total order of AC elements: Ord, then (only for a hash
// collision in ELAN_ACORD=hash mode) structure.
func Compare(a, b Term) int {
	if a == b {
		return 0
	}
	if a.Ord != b.Ord {
		if a.Ord < b.Ord {
			return -1
		}
		return 1
	}
	if a.Sym != b.Sym {
		return int(a.Sym - b.Sym)
	}
	if a.Int != b.Int {
		if a.Int < b.Int {
			return -1
		}
		return 1
	}
	if len(a.Args) != len(b.Args) {
		return len(a.Args) - len(b.Args)
	}
	for i := range a.Args {
		if c := Compare(a.Args[i], b.Args[i]); c != 0 {
			return c
		}
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
	slices.SortFunc(e, Compare)
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
		sb.WriteString(strconv.FormatInt(int64(t.Int), 10))
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
	Out.WriteString(strconv.FormatInt(rtstat.Steps, 10))
	Out.WriteString("\n")
	Out.Flush()
	rtstat.Report()
}

// QueryInt parses the integer query from the command line.
func QueryInt() int32 {
	if len(os.Args) < 2 {
		os.Stderr.WriteString("usage: " + os.Args[0] + " <int>\n")
		os.Exit(2)
	}
	v, err := strconv.ParseInt(os.Args[1], 10, 32)
	if err != nil {
		os.Stderr.WriteString("bad query: " + os.Args[1] + "\n")
		os.Exit(2)
	}
	return int32(v)
}

// Step counts one rule application.
func Step() { rtstat.Steps++ }

// Steps returns the step counter.
func Steps() int64 { return rtstat.Steps }
