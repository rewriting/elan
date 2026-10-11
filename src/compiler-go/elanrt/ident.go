package elanrt

import "strconv"

// Identifiers (sort ident, S7d task 3). An identifier is its index in the
// table of identifiers of the program: the Identifiers table of the .ref
// file, whose indices are the positions of REM's stringtab
// (src/compiler/earley/stringtab.cc: the sum of the characters modulo
// 3000, a collision probed by steps of 211), so that a name gets the
// index REM gives it, and identifiers are ordered as REM orders them
// ([Compare]: by index). As a term, an identifier is a node SymIdent with
// its index in Int, shared; generated functions handle the identifiers of
// a native family as int32 indices.

// identTableSize is the size of REM's table of identifiers (tabofident).
const identTableSize = 3000

var identNames = map[int32]string{}
var identIndices = map[string]int32{}

// InitIdents sets the table of identifiers: the program's, by index (the
// generated program calls it before running; nil empties it).
func InitIdents(names map[int32]string) {
	identNames = map[int32]string{}
	identIndices = map[string]int32{}
	for i, n := range names {
		identNames[i] = n
		identIndices[n] = i
	}
}

// IdentIndex returns the index of the identifier name: its index in the
// table, else the free position REM's stringtab gives it (added to the
// table).
func IdentIndex(name string) int32 {
	if i, ok := identIndices[name]; ok {
		return i
	}
	// (C chars: signed bytes)
	sum := 0
	for i := 0; i < len(name); i++ {
		sum += int(int8(name[i]))
	}
	pos := int32(sum % identTableSize)
	if pos < 0 {
		pos += identTableSize // (out of REM's table: undefined there)
	}
	for n := 0; ; n++ {
		if _, used := identNames[pos]; !used {
			break
		}
		if n == identTableSize {
			Fatal("elanrt: the table of identifiers is full")
		}
		pos = (pos + 211) % identTableSize
	}
	identNames[pos] = name
	identIndices[name] = pos
	return pos
}

// IdentName returns the name of the identifier of index i (IDENT(i) if
// the table has none).
func IdentName(i int32) string {
	if n, ok := identNames[i]; ok {
		return n
	}
	return "IDENT(" + strconv.Itoa(int(i)) + ")"
}

// MkIdent returns the identifier of index i as a term.
func MkIdent(i int32) Term { return mk(SymIdent, i, nil) }

// IsIdent tells whether t is an identifier (not a stuck term).
func IsIdent(t Term) bool { return t.Sym == SymIdent }

// IdentOf returns the index of the identifier t, false for a stuck term.
func IdentOf(t Term) (int32, bool) { return t.Int, t.Sym == SymIdent }

// MustIdent is the index of t, which must be an identifier (a stuck term
// is a bug of the compiler: the operation op receives it).
func MustIdent(t Term, op string) int32 {
	if t.Sym != SymIdent {
		notAValue(t, op)
	}
	return t.Int
}

// IdentString is the name of the identifier of index i as a builtin
// string (ident2string, code 177).
func IdentString(i int32) string { return CString(IdentName(i)) }
