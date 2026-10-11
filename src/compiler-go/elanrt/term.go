package elanrt

import "slices"

// Term is a shared term: a pointer to a node of the unique table. Two terms
// are equal if and only if they are the same pointer (maximal sharing),
// except int values: an int node is not in the table ([MkInt]: a weak
// pointer would cost more than the node), two int nodes may have the same
// value, and [Eq] compares terms. The nodes that hold ints are shared
// (their table compares their int arguments by value).
type Term = *Node

// Eq tells whether a and b are the same term: the same node, or ints of
// the same value.
func Eq(a, b Term) bool { return a == b || a.Sym == SymInt && b.Sym == SymInt && a.Int == b.Int }

// Node is an immutable term node. Build nodes with [Mk0]..[Mk3], [Mk],
// [MkInt], [MkStr], [MkAC]; never with a composite literal (the node would
// not be shared).
type Node struct {
	Sym int32 // symbol code (an index into the symbol table)
	Int int32 // payload of an int node (SymInt); of a string node: its slot in the string table
	// Hash is the structural hash (from the symbol's name and arity, the
	// payload and the arguments' hashes) in its low 62 bits ([Node.hash]),
	// and two flags of AC terms in its top bits (no room of their own: a
	// node of arity 1 stays in the allocator's 48-byte class):
	// flagTmp marks a transient AC term ([ACTerm]): not in the unique
	// table, seen only by the normalisation function of its symbol until
	// [Intern] makes it the shared node; flagNF marks a shared AC term
	// that the normalisation function of its symbol found in normal form
	// without a rewrite step ([NormalForm]): that function returns it at
	// once the next times.
	Hash uint64
	Args []Term // arguments; for an AC symbol, n >= 2 flattened elements in canonical order
}

// The flags in the top bits of [Node.Hash].
const (
	flagTmp  uint64 = 1 << 63
	flagNF   uint64 = 1 << 62
	hashMask        = flagNF - 1
)

// hash is the structural hash of n (without its flags).
func (n *Node) hash() uint64 { return n.Hash & hashMask }

// isTmp tells whether n is a transient AC term ([ACTerm]).
func (n *Node) isTmp() bool { return n.Hash&flagTmp != 0 }

// Nodes of arity 1 to 3 carry their arguments inline: one allocation per
// node instead of two.
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

// ---------------------------------------------------------------------------
// Symbols

// Symbol describes a function symbol of the program.
type Symbol struct {
	Name  string
	Arity int
	// AC symbols (associative-commutative): their nodes hold n >= 2
	// flattened elements in canonical order ([MkAC]).
	AC bool
	// Print is the mixfix layout of the symbol (tokens of its declaration);
	// nil is replaced by the prefix layout name(a1,...,an) when declared.
	// The layout of an AC symbol has two arguments.
	Print []PrintItem
	// Code is the symbol's code in the .ref file (REM's code): the order
	// of terms ([Compare]) compares it first. 0 when declared: the
	// runtime code plus 1000 (tests); the generated programs give the
	// .ref code (never 0: false, a builtin).
	Code int32
	seed uint64 // hash of name and arity
}

// Syms is the symbol table, indexed by symbol code; the builtin symbols
// come first. (Package variables are initialised in dependency order: a
// package variable of the program or of a test that declares a symbol is
// initialised after Syms.)
var Syms = builtinSymbols()

func builtinSymbols() []Symbol {
	var syms []Symbol
	// (the codes of true and false are those of the .ref file; ints and
	// strings are ordered apart)
	for i, name := range []string{"int", "string", "true", "false"} {
		code := [...]int32{-2, -1, 1, 0}[i]
		syms = append(syms, Symbol{Name: name, Print: prefixLayout(name, 0), Code: code, seed: symbolSeed(name, 0)})
	}
	return syms
}

// Codes of the builtin symbols, declared first.
const (
	SymInt    int32 = 0 // an int value, in the Int field
	SymString int32 = 1 // a string value (see strings.go)
	SymTrue   int32 = 2
	SymFalse  int32 = 3
)

// The bool terms.
var (
	True  = Mk0(SymTrue)
	False = Mk0(SymFalse)
)

// DeclareSym adds a symbol to the table and returns its code. The
// generated program declares its symbols in package variables, so they are
// declared in a fixed order before main runs.
func DeclareSym(s Symbol) int32 {
	s.seed = symbolSeed(s.Name, s.Arity)
	if s.Print == nil {
		s.Print = prefixLayout(s.Name, s.Arity)
	}
	if s.Code == 0 {
		s.Code = int32(len(Syms)) + 1000
	}
	Syms = append(Syms, s)
	return int32(len(Syms) - 1)
}

// NewSym declares a symbol with the prefix layout (no AC).
func NewSym(name string, arity int) int32 {
	return DeclareSym(Symbol{Name: name, Arity: arity})
}

// The hashes depend only on the program's names, never on symbol codes,
// addresses or a random seed: the AC order is the same in every run and
// every build.
func symbolSeed(name string, arity int) uint64 {
	// FNV-1a
	h := uint64(14695981039346656037)
	for i := 0; i < len(name); i++ {
		h = (h ^ uint64(name[i])) * 1099511628211
	}
	return mix(h ^ uint64(arity)*0x9E3779B97F4A7C15)
}

// mix is a 64-bit finaliser (murmur3).
func mix(h uint64) uint64 {
	h ^= h >> 33
	h *= 0xff51afd7ed558ccd
	h ^= h >> 33
	h *= 0xc4ceb9fe1a85ec53
	h ^= h >> 33
	return h
}

// nodeHash is the structural hash of the node (sym, iv, args); never 0
// (0 marks an empty slot of the unique table), its top bits clear (the
// flags of [Node.Hash]).
func nodeHash(sym, iv int32, args []Term) uint64 {
	h := Syms[sym].seed ^ uint64(uint32(iv))*0xC2B2AE3D27D4EB4F
	for _, a := range args {
		h = (h ^ a.hash()) * 0x100000001B3
		h ^= h >> 29
	}
	return mix(h)&hashMask | 1
}

// ---------------------------------------------------------------------------
// Building terms

// Statistics of the unique table (tests, ELAN_STATS).
var (
	// Created counts the nodes created since the start.
	Created int64
	// Hits counts the constructions that found an existing node.
	Hits int64
)

// mk returns the unique node (sym, iv, args). args is not kept: the
// callers may pass a buffer.
func mk(sym, iv int32, args []Term) Term {
	h := nodeHash(sym, iv, args)
	if n := lookup(h, sym, iv, args); n != nil {
		Hits++
		return n
	}
	n := newNode(sym, iv, h, args)
	insert(n)
	return n
}

func (n *Node) same(sym, iv int32, args []Term) bool {
	if n.Sym != sym || n.Int != iv || len(n.Args) != len(args) {
		return false
	}
	for i, a := range args {
		if b := n.Args[i]; a != b && !(a.Sym == SymInt && b.Sym == SymInt && a.Int == b.Int) {
			return false
		}
	}
	return true
}

// newNode allocates a node (args copied).
func newNode(sym, iv int32, h uint64, args []Term) *Node {
	var n *Node
	switch len(args) {
	case 0:
		n = &Node{}
	case 1:
		b := &node1{buf: [1]Term{args[0]}}
		b.Args = b.buf[:]
		n = &b.Node
	case 2:
		b := &node2{buf: [2]Term{args[0], args[1]}}
		b.Args = b.buf[:]
		n = &b.Node
	case 3:
		b := &node3{buf: [3]Term{args[0], args[1], args[2]}}
		b.Args = b.buf[:]
		n = &b.Node
	default:
		n = &Node{Args: slices.Clone(args)}
	}
	n.Sym, n.Int, n.Hash = sym, iv, h
	Created++
	return n
}

// Mk0 returns the constant sym.
func Mk0(sym int32) Term { return mk(sym, 0, nil) }

// Mk1 returns sym(a).
func Mk1(sym int32, a Term) Term {
	buf := [1]Term{a}
	return mk(sym, 0, buf[:])
}

// Mk2 returns sym(a, b).
func Mk2(sym int32, a, b Term) Term {
	buf := [2]Term{a, b}
	return mk(sym, 0, buf[:])
}

// Mk3 returns sym(a, b, c).
func Mk3(sym int32, a, b, c Term) Term {
	buf := [3]Term{a, b, c}
	return mk(sym, 0, buf[:])
}

// Mk returns sym(args...) for a free (non-AC) symbol of any arity.
func Mk(sym int32, args ...Term) Term { return mk(sym, 0, args) }

// ---------------------------------------------------------------------------
// Builtin values as terms (boxing)

// Int nodes are not in the unique table: a weak pointer costs ~0.25 us,
// 20 times the node (deep normalisation of a list of 3 x 10^6 ints: 2.0 s
// -> 1.3 s). Small ints are preallocated, the others go through a small
// cache of the last ones made (direct-mapped, by value): a repeated value
// is often one node, and memory stays bounded.
const smallMin, smallMax = -1024, 1024

var smallInts = makeSmallInts()

func makeSmallInts() []Term {
	ts := make([]Term, smallMax-smallMin)
	for i := range ts {
		ts[i] = newInt(int32(i + smallMin))
	}
	return ts
}

// intCacheSize is the number of entries of the cache of ints (a power of
// two).
const intCacheSize = 1 << 12

var intCache [intCacheSize]Term

func newInt(v int32) Term {
	Created++
	return &Node{Sym: SymInt, Int: v, Hash: nodeHash(SymInt, v, nil)}
}

// MkInt returns an int term v (compare ints with [Eq] or by value).
func MkInt(v int32) Term {
	if smallMin <= v && v < smallMax {
		return smallInts[v-smallMin]
	}
	c := &intCache[uint32(v)&(intCacheSize-1)]
	if t := *c; t != nil && t.Int == v {
		return t
	}
	t := newInt(v)
	*c = t
	return t
}

// MkBool returns True or False.
func MkBool(b bool) Term {
	if b {
		return True
	}
	return False
}

// IsInt tells whether t is an int value (not a stuck term).
func IsInt(t Term) bool { return t.Sym == SymInt }

// IsBool tells whether t is true or false (not a stuck term).
func IsBool(t Term) bool { return t == True || t == False }
