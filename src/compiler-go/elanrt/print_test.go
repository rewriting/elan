package elanrt

import (
	"bytes"
	"testing"
)

func TestMixfixPrinting(t *testing.T) {
	for _, c := range []struct {
		t    Term
		want string
	}{
		{nat(2), "s(s(o))"},
		{Mk2(symCons, MkInt(4), Mk2(symCons, MkInt(7), Mk0(symNil))), "4.7.nil"},
		{Mk2(symCons, MkInt(IntLit(65536*65536)), Mk0(symNil)), "0.nil"},
		{Mk2(symPlus, nat(1), Mk0(symO)), "s(o)+o"},
		{Mk2(symPlus, Mk2(symPlus, nat(1), nat(1)), Mk0(symO)), "s(o)+s(o)+o"},
		{Mk(symF4, MkInt(1), True, MkStr("a b"), list(2)), "f(1,true,a b,1.0.nil)"},
		{and(atom(1), atom(2)), "and(p(1),p(2))"},
		{MkInt(-2147483648), "-2147483648"},
	} {
		if got := Format(c.t); got != c.want {
			t.Errorf("got %q, want %q", got, c.want)
		}
	}
}

// AC terms print with the layout of their binary symbol, repeated.
func TestACPrinting(t *testing.T) {
	x := MkAC(symAnd, Flatten(symAnd, atom(1), atom(1), atom(2)))
	if got := Format(x); len(got) != len("and(p(1),p(1),p(2))") || got[:4] != "and(" {
		t.Errorf("prefix AC: %q", got)
	}
	u := MkAC2(symU, Mk0(symO), Mk0(symNil))
	if got := Format(u); got != "o U nil" && got != "nil U o" {
		t.Errorf("infix AC: %q", got)
	}
}

// The C printer's spacing: a space between two alphanumeric tokens; ints
// and strings neither test nor set the flag.
func TestTokenSpacingAsTheCPrinter(t *testing.T) {
	var w Writer
	w.Ident("if")
	w.Ident("x") // alnum after alnum: a space
	w.Char('+')
	w.Ident("y")
	w.Int(3) // ints neither test nor set the flag
	w.Ident("z")
	w.Char('(')
	w.NumCode("2")
	w.Ident("w")
	w.Str("s t") // strings neither
	w.Ident("v")
	w.Raw(" = ")
	w.Ident("u") // Raw keeps the flag
	if got := string(w.Take()); got != "if x+y3 z(2 ws t v =  u" {
		t.Fatalf("got %q", got)
	}
	// identifiers are bytes: Latin-1 characters are one byte, not
	// alphanumeric for the spacing rule
	w.Reset()
	w.Ident("caf\xe9")
	w.Ident("x")
	if got := w.Take(); !bytes.Equal(got, []byte("caf\xe9x")) {
		t.Fatalf("got %q", got)
	}
}

func TestStringsRawOrQuoted(t *testing.T) {
	var w Writer
	w.Str("a\"b \xe9")
	if got := string(w.Take()); got != "a\"b \xe9" {
		t.Errorf("raw: %q", got)
	}
	w.Quoted = true
	w.Str("a\"b\\")
	if got := string(w.Take()); got != `"a\"b\\"` {
		t.Errorf("quoted: %q", got)
	}
	if Format(MkStr("x y")) != "x y" {
		t.Error("string term")
	}
}

// A list of a million elements prints without deep recursion (the
// printer iterates on the last argument): the guard depth stays at 0.
func TestDeepListPrinting(t *testing.T) {
	const n = 1_000_000
	l := list(n)
	var w Writer
	maxSeen := 0
	w.Term(l)
	maxSeen = max(maxSeen, Depth())
	out := w.Bytes()
	if !bytes.HasPrefix(out, []byte("999999.999998.")) || !bytes.HasSuffix(out, []byte(".1.0.nil")) {
		t.Fatalf("output %q...%q", out[:20], out[len(out)-20:])
	}
	if bytes.Count(out, []byte(".")) != n {
		t.Fatal("elements")
	}
	// a deep prefix term too: s(s(...)) closes its parentheses at the end
	x := nat(n)
	s := Format(x)
	if len(s) != 2*n+1+n || s[len(s)-2:] != "))" || s[:4] != "s(s(" {
		t.Fatalf("deep s(...): %d bytes", len(s))
	}
	if Depth() != 0 || maxSeen != 0 {
		t.Fatal("guard depth")
	}
}
