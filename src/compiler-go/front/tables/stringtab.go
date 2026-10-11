// Package tables holds the interpreter's hash tables (base/stringtab.cc,
// base/mitab.cc) and their initial contents (load/module.cc, the generated
// tabofident.cc). Positions in these tables are the codes of the .ref format
// (docs/ref-format.md), so the hash, the probing and the order of insertions
// are the C++'s exactly.
package tables

import (
	"fmt"
	"strconv"

	"elan/front/diag"
)

// Limits (base/constants.h).
const (
	MaxNofIdent     = 3000  // MAXNOFIDENT, size of tabofident
	NTypes          = 500   // NTYPES, size of typet
	MaxNofImports   = 200   // MAXNOFIMPORTS, size of import
	MaxNofTrn       = 2000  // MAXNOFTRN, rule names
	MaxNofStrat     = 500   // MAXNOFSTRAT, strategy names
	MaxNofMac       = 50    // MAXNOFMAC, macros of one mlstream
	MaxNofVar       = 49    // MAXNOFVAR, FOR EACH variables
	MaxDeepConstExp = 10    // MAXDEEPCONSTEXP, ppexsemact's stack
	MRWTSize        = 300   // MRWTSIZE, size of mrwt
	IDLen           = 50    // IDLEN: identifiers keep IDLen-1 characters
	StrLen          = 1000  // STRLEN: string constants are shorter
	MaxNofString    = 10000 // MAXNOFSTRING, string constants
)

// Hash is stringtab::member's start position: the sum of the bytes as
// *signed* chars (the build uses -fsigned-char) modulo size, with C's %
// (truncated, so a negative sum gives a result in (-size, 0]). A negative
// position is an out-of-bounds index in the C++ (a byte >= 0x80 in an
// identifier; none in the corpus): StringTab reports it as an internal error.
func Hash(s string, size int) int {
	sum := 0
	for i := 0; i < len(s); i++ {
		sum += int(int8(s[i]))
	}
	return sum % size
}

// StringTab is stringtab: an open-addressing table of strings, probing by
// +211 modulo the size; positions are the codes.
type StringTab struct {
	d     *diag.D
	id    []string
	used  []bool
	strin int // number of strings
	// Posid is the position of the last string looked up (stringtab::posid,
	// set by Member and AddStr): its slot, or the free slot where it would go.
	Posid int
}

// NewStringTab is stringtab(size, init...): init is added in order.
func NewStringTab(d *diag.D, size int, init ...string) *StringTab {
	t := &StringTab{d: d, id: make([]string, size), used: make([]bool, size)}
	for _, s := range init {
		t.AddStr(s)
	}
	return t
}

// Member is stringtab::member; it sets Posid.
func (t *StringTab) Member(s string) bool {
	t.Posid = Hash(s, len(t.id))
	if t.Posid < 0 {
		t.d.Internal("front/tables", "%s hashes to %d (the C++ indexes out of bounds)", strconv.Quote(s), t.Posid)
	}
	for t.used[t.Posid] {
		if t.id[t.Posid] == s {
			return true
		}
		t.Posid = (t.Posid + 211) % len(t.id)
	}
	return false
}

// Index is stringtab::index: the position of s, or -1.
func (t *StringTab) Index(s string) int {
	if t.Member(s) {
		return t.Posid
	}
	return -1
}

// AddStr is stringtab::addstr: the position of s, added if needed. The table
// holds at most size-1 strings; one more is a fatal error.
func (t *StringTab) AddStr(s string) int {
	if !t.Member(s) {
		if t.strin+1 >= len(t.id) {
			t.d.Errs(fmt.Sprintf("[addstr] ERROR, string tab overflowed over %d!!! \t !!!!!!!!!\n", len(t.id)))
			t.d.Fail()
		}
		t.strin++
		t.id[t.Posid], t.used[t.Posid] = s, true
	}
	return t.Posid
}

// Ide is stringtab::ide: the string at position i ("" when free).
func (t *StringTab) Ide(i int) string { return t.id[i] }

// Used reports whether position i holds a string.
func (t *StringTab) Used(i int) bool { return t.used[i] }

// RemoveStr is stringtab::removestr (the C++ frees the slot and decrements
// the count unconditionally; probe chains through the slot are cut, as in C).
func (t *StringTab) RemoveStr(i int) {
	t.id[i], t.used[i] = "", false
	t.strin--
}

// Len is the number of strings in the table.
func (t *StringTab) Len() int { return t.strin }

// Size is the size of the table.
func (t *StringTab) Size() int { return len(t.id) }

// Each calls f on every string in position order (forinit/fornext, the
// order of the .ref dumps).
func (t *StringTab) Each(f func(i int, s string)) {
	for i, u := range t.used {
		if u {
			f(i, t.id[i])
		}
	}
}
