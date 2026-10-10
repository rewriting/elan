// Package rtt is the runtime of the TYPED representation (G2 variant b):
// one Go struct type per operator, one interface per sort; the generated
// code matches by type switch. Shared nodes are hash-consed per operator
// type through a weak unique table (Table[N]); builtins are native Go
// values (int32, bool) and are not hash-consed.
package rtt

import (
	"bufio"
	"os"
	"slices"
	"strconv"
	"strings"
	"weak"

	"gorepr/rtstat"
)

// Hdr is embedded first in every shared node type.
type Hdr struct {
	Ord  uint64 // AC order key: creation id, or structural hash (ELAN_ACORD=hash)
	hash uint64 // table hash
}

// H gives access to the header (promoted to every node type).
func (h *Hdr) H() *Hdr { return h }

// Node is implemented by every shared node type; a sort interface embeds it.
type Node interface {
	H() *Hdr
	Print(b *strings.Builder)
}

// Ord is the AC order key of a node.
func Ord(n Node) uint64 { return n.H().Ord }

// Cont is a success continuation over sort T (true = stop).
type Cont[T any] = func(T) bool

var nextID uint64

func setOrd(h *Hdr, hash uint64) {
	h.hash = hash
	if rtstat.HashOrder {
		h.Ord = hash
	} else {
		nextID++
		h.Ord = nextID
	}
	rtstat.Created++
}

// Const registers a constant (operator of arity 0): one node, never collected.
func Const[P Node](n P) P {
	setOrd(n.H(), Hash(uint64(len(consts))+0x51, 0, 0))
	consts = append(consts, n)
	return n
}

var consts []Node

// ---------------------------------------------------------------------------
// Hashing (from a type tag, scalar fields and the children's Ord).

func mix(h uint64) uint64 {
	h ^= h >> 33
	h *= 0xff51afd7ed558ccd
	h ^= h >> 33
	return h | 1 // never 0: 0 marks an empty slot
}

// Hash combines a type tag and two words (scalars or children's Ord).
func Hash(tag, a, b uint64) uint64 {
	h := tag*0x9E3779B97F4A7C15 ^ a*0xC2B2AE3D27D4EB4F
	h = (h ^ b) * 0x100000001B3
	return mix(h)
}

// HashSlice hashes a tag and the Ord of a slice of nodes (AC arguments).
func HashSlice[S Node](tag uint64, xs []S) uint64 {
	h := tag * 0x9E3779B97F4A7C15
	for _, x := range xs {
		h = (h ^ x.H().Ord) * 0x100000001B3
	}
	return mix(h)
}

// ---------------------------------------------------------------------------
// Weak unique table, one per operator type (same algorithm as rtg's).

type slot[N any] struct {
	h uint64
	p weak.Pointer[N]
}

// Table is the unique table of the node type N.
type Table[N any] struct {
	slots  []slot[N]
	used   int
	lastGC int64 // rtstat.GCCycles at the last rebuild
}

// Shared is the constraint of a hash-consed node type: *N is a Node and
// can compare its fields with another node's (Same: generated, shallow).
type Shared[N any] interface {
	*N
	Node
	Same(*N) bool
}

var tables []func() int // live counts, for rtstat
var sizes []func() int  // capacities, for rtstat

// Intern returns the unique node equal to key (hash h); key is moved into a
// new node when there is none (AC slices are kept, not copied).
func Intern[N any, P Shared[N]](t *Table[N], h uint64, key N) P {
	if t.slots == nil {
		t.slots = make([]slot[N], 1<<10)
		tables = append(tables, t.live)
		sizes = append(sizes, func() int { return len(t.slots) })
	}
	mask := uint64(len(t.slots) - 1)
	i := h & mask
	for ; t.slots[i].h != 0; i = (i + 1) & mask {
		if t.slots[i].h == h {
			if n := P(t.slots[i].p.Value()); n != nil && n.Same(&key) {
				rtstat.Hits++
				return n
			}
		}
	}
	n := P(new(N))
	*n = key
	setOrd(n.H(), h)
	t.slots[i] = slot[N]{h, weak.Make((*N)(n))}
	t.used++
	if 2*t.used > len(t.slots) {
		t.rebuild()
	}
	return n
}

func (t *Table[N]) live() int {
	live := 0
	for i := range t.slots {
		if t.slots[i].h != 0 && t.slots[i].p.Value() != nil {
			live++
		}
	}
	return live
}

func (t *Table[N]) rebuild() {
	live := t.live()
	size := 1 << 10
	for size < 4*live {
		size *= 2
	}
	if size > len(t.slots) && size >= 1<<16 && rtstat.MaybeGC(t.lastGC) {
		// dead nodes were waiting for a collection: recount
		live = t.live()
		for size = 1 << 10; size < 4*live; size *= 2 {
		}
	}
	t.lastGC = rtstat.GCCycles.Load()
	nt := make([]slot[N], size)
	mask := uint64(size - 1)
	for _, s := range t.slots {
		if s.h != 0 && s.p.Value() != nil {
			i := s.h & mask
			for nt[i].h != 0 {
				i = (i + 1) & mask
			}
			nt[i] = s
		}
	}
	rtstat.Rebuilds++
	rtstat.Purged += int64(t.used - live)
	t.slots, t.used = nt, live
}

func init() {
	rtstat.Live = func() int {
		n := len(consts)
		for _, f := range tables {
			n += f()
		}
		return n
	}
	rtstat.Slots = func() int {
		n := 0
		for _, f := range sizes {
			n += f()
		}
		return n
	}
}

// ---------------------------------------------------------------------------
// AC canonical form (flattened, sorted by Ord, duplicates kept) and
// matching helpers, generic over the sort interface S.

// Compare orders AC elements by Ord. Two distinct nodes with the same Ord
// can only be a structural-hash collision (ELAN_ACORD=hash); a real runtime
// would fall back to a generated structural compare.
func Compare[S Node](a, b S) int {
	oa, ob := a.H().Ord, b.H().Ord
	if oa < ob {
		return -1
	}
	if oa > ob {
		return 1
	}
	if Node(a) != Node(b) {
		panic("rtt: AC order key collision")
	}
	return 0
}

// ACFlatten returns a fresh sorted slice of the elements of xs, splicing
// the arguments of the elements that are terms of the AC symbol itself
// (args(x) gives them, ok = x is such a term).
func ACFlatten[S Node](xs []S, args func(S) ([]S, bool)) []S {
	n := 0
	for _, x := range xs {
		if a, ok := args(x); ok {
			n += len(a)
		} else {
			n++
		}
	}
	e := make([]S, 0, n)
	for _, x := range xs {
		if a, ok := args(x); ok {
			e = append(e, a...)
		} else {
			e = append(e, x)
		}
	}
	slices.SortFunc(e, Compare[S])
	return e
}

// Skip reports whether position i of the sorted multiset e must be skipped
// when enumerating distinct choices (see rtg.Skip).
func Skip[S comparable](e []S, used []bool, i int) bool {
	if used != nil && used[i] {
		return true
	}
	return i > 0 && e[i] == e[i-1] && (used == nil || !used[i-1])
}

// Rest returns the unused elements of e (still sorted).
func Rest[S any](e []S, used []bool) []S {
	r := make([]S, 0, len(e))
	for i, x := range e {
		if !used[i] {
			r = append(r, x)
		}
	}
	return r
}

// Without returns e minus the positions i (and j if j >= 0).
func Without[S any](e []S, i, j int) []S {
	r := make([]S, 0, len(e))
	for p, x := range e {
		if p != i && p != j {
			r = append(r, x)
		}
	}
	return r
}

// ---------------------------------------------------------------------------
// Steps, printing, driver.

// Step counts one rule application.
func Step() { rtstat.Steps++ }

// PrintArgs writes name(a1,...,an).
func PrintArgs[S Node](b *strings.Builder, name string, xs ...S) {
	b.WriteString(name)
	b.WriteByte('(')
	for i, x := range xs {
		if i > 0 {
			b.WriteByte(',')
		}
		x.Print(b)
	}
	b.WriteByte(')')
}

// PrintInt writes a builtin integer.
func PrintInt(b *strings.Builder, v int32) { b.WriteString(strconv.Itoa(int(v))) }

// Out is the buffered standard output.
var Out = bufio.NewWriterSize(os.Stdout, 1<<16)

// PrintResult prints "result = <t>" with the given printer.
func PrintResult(print func(*strings.Builder)) {
	var sb strings.Builder
	sb.WriteString("result = ")
	print(&sb)
	sb.WriteByte('\n')
	Out.WriteString(sb.String())
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
