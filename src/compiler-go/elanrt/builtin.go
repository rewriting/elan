package elanrt

import "strings"

// Builtin sorts: int, bool, string (spec G2: native values in the
// generated functions, boxed only when stored in a node).
//
// # int
//
// An ELAN int is a Go int32 with the interpreter's semantics, the 32-bit C
// int of src/interpreter/rewrite/rtmisc.cc: every operation wraps (two's
// complement), literals keep their low 32 bits ([IntLit]). Go's int32
// arithmetic already wraps (+, -, *, unary -, &, |, comparisons): the
// generated code uses the Go operators. Division and remainder use [Div]
// and [Mod]: x/0 = 0 and x%0 = x (the interpreter on arm64, where sdiv does
// not trap), MIN/-1 = MIN and MIN%-1 = 0 (Go's rules too), truncation
// towards zero (C). Values checked with elan -b (elanc-rs
// elan-runtime/src/builtin.rs).
//
// # bool
//
// Go bool; and/or are strict (both arguments are normalised first, as in
// innermost rewriting); printed true/false.
//
// # string
//
// A Go string holding bytes, as the interpreter's C strings, not UTF-8
// text: ELAN sources are Latin-1, "café" is 4 bytes. A string never
// contains a NUL byte: C strings end at the first NUL, so string(0) is ""
// and s[i <- 0] truncates s at i ([CString]). The partial primitives return
// false where the interpreter leaves the term unreduced (stuck).
//
// # Stuck terms of builtin sorts
//
// A defined operator of a builtin result sort (f : (int) int) has no value
// when no rule applies: the term stays f(3), and it is printed so. The
// generator keeps native values for a builtin sort when no operator of the
// sort can be stuck (a totality analysis, as elanc-rs), and otherwise
// represents the values of that sort as Terms: a value is a boxed builtin
// node ([MkInt], [MkBool], [MkStr]; [IsValue]), a stuck term is a node of
// the stuck operator (f(3)), and a builtin operation applied to a stuck
// argument is stuck too (f(3) + 1 stays +(f(3), 1), as in the
// interpreter). [Lift1] and [Lift2] apply an operation or build the stuck
// term.

// IntLit returns an integer literal as the interpreter reads it: its low
// 32 bits.
func IntLit(x int64) int32 { return int32(x) }

// Div is the C truncating division; x/0 = 0 and MIN/-1 = MIN.
func Div(a, b int32) int32 {
	if b == 0 {
		return 0
	}
	return a / b
}

// Mod is the C remainder (sign of the dividend); x%0 = x and MIN%-1 = 0.
func Mod(a, b int32) int32 {
	if b == 0 {
		return a
	}
	return a % b
}

// CString returns s as a C string: its bytes up to the first NUL.
func CString(s string) string {
	if i := strings.IndexByte(s, 0); i >= 0 {
		return s[:i]
	}
	return s
}

// StrLen is string_strlen(s): the number of bytes.
func StrLen(s string) int32 { return int32(len(s)) }

// StrCat is string_strcat(a, b).
func StrCat(a, b string) string { return CString(a + b) }

// StrIndex is s[i]: the byte at i as a signed C char; false (stuck) out of
// range.
func StrIndex(s string, i int32) (int32, bool) {
	if i < 0 || int(i) >= len(s) {
		return 0, false
	}
	return int32(int8(s[i])), true
}

// StrModif is s[i <- c]: s with byte i replaced by the low byte of c (a
// NUL byte ends the string there); false out of range. (The interpreter
// modifies the string in place, sharing included; here the result is a
// new string.)
func StrModif(s string, i, c int32) (string, bool) {
	if i < 0 || int(i) >= len(s) {
		return "", false
	}
	b := []byte(s)
	b[i] = byte(c)
	return CString(string(b)), true
}

// StrSubstr is string_substr(s, i, n): n bytes from i; false unless
// 0 <= i < len(s), 0 <= n and i+n <= len(s).
func StrSubstr(s string, i, n int32) (string, bool) {
	l := int64(len(s))
	if !(0 <= i && int64(i) < l && 0 <= n && int64(i)+int64(n) <= l) {
		return "", false
	}
	return s[i : i+n], true
}

// StrSpn is string_strspn(s, accept) (C strspn).
func StrSpn(s, accept string) int32 {
	n := 0
	for n < len(s) && strings.IndexByte(accept, s[n]) >= 0 {
		n++
	}
	return int32(n)
}

// StrCmp is string_strcmp(a, b): -1, 0 or 1, as the interpreter (unsigned
// bytes; C strcmp only specifies the sign).
func StrCmp(a, b string) int32 {
	return int32(strings.Compare(a, b))
}

// StrOfChar is string_string(c): the one-byte string of the low byte of
// c; "" when that byte is NUL.
func StrOfChar(c int32) string { return CString(string([]byte{byte(c)})) }

// ---------------------------------------------------------------------------
// Values of builtin sorts that can be stuck (see the package comment above)

// IsValue tells whether t is a builtin value (int, bool or string), not a
// stuck term.
func IsValue(t Term) bool {
	return t.Sym == SymInt || t.Sym == SymString || t == True || t == False
}

// IntOf returns the value of an int term; false if t is stuck.
func IntOf(t Term) (int32, bool) { return t.Int, t.Sym == SymInt }

// BoolOf returns the value of a bool term; false if t is stuck.
func BoolOf(t Term) (bool, bool) { return t == True, IsBool(t) }

// StrValue returns the value of a string term; false if t is stuck.
func StrValue(t Term) (string, bool) {
	if t.Sym != SymString {
		return "", false
	}
	return StrOf(t), true
}

// MustInt is the value of the int term t received by op, an operator
// total on values (the compiler proved that t is not stuck): a stuck term
// is a bug, a panic naming op (a run-time error of the program), not a
// silent 0.
func MustInt(t Term, op string) int32 {
	if t.Sym != SymInt {
		notAValue(t, op)
	}
	return t.Int
}

// MustBool is [MustInt] for bool.
func MustBool(t Term, op string) bool {
	if !IsBool(t) {
		notAValue(t, op)
	}
	return t == True
}

// MustStr is [MustInt] for string.
func MustStr(t Term, op string) string {
	if t.Sym != SymString {
		notAValue(t, op)
	}
	return StrOf(t)
}

func notAValue(t Term, op string) {
	name := "?"
	if t.Sym >= 0 && int(t.Sym) < len(Syms) {
		name = Syms[t.Sym].Name
	}
	panic(op + ": an argument is not a value (a term of head " + name + "), the operator was compiled as total")
}

// Lift1 applies the builtin operation op to a: op's result if a is a value
// and op is defined on it, else the stuck term sym(a).
func Lift1(sym int32, a Term, op func(a Term) (Term, bool)) Term {
	if IsValue(a) {
		if r, ok := op(a); ok {
			return r
		}
	}
	return Mk1(sym, a)
}

// Lift2 is [Lift1] for a binary operation: the stuck term is sym(a, b).
func Lift2(sym int32, a, b Term, op func(a, b Term) (Term, bool)) Term {
	if IsValue(a) && IsValue(b) {
		if r, ok := op(a, b); ok {
			return r
		}
	}
	return Mk2(sym, a, b)
}

// IntOp2 makes the operation of [Lift2] from a total binary int
// operation: rt.Lift2(sym_plus, a, b, rt.IntOp2(func(x, y int32) int32 { return x + y })).
func IntOp2(f func(x, y int32) int32) func(a, b Term) (Term, bool) {
	return func(a, b Term) (Term, bool) {
		x, okx := IntOf(a)
		y, oky := IntOf(b)
		if !okx || !oky {
			return nil, false
		}
		return MkInt(f(x, y)), true
	}
}

// IntCmp2 makes the operation of [Lift2] from an int comparison.
func IntCmp2(f func(x, y int32) bool) func(a, b Term) (Term, bool) {
	return func(a, b Term) (Term, bool) {
		x, okx := IntOf(a)
		y, oky := IntOf(b)
		if !okx || !oky {
			return nil, false
		}
		return MkBool(f(x, y)), true
	}
}
