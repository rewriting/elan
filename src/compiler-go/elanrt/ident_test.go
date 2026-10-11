package elanrt

import "testing"

// The identifier table: the program's identifiers at their indices (the
// Identifiers table of the .ref file); a new name at its position in REM's
// stringtab (sum of the bytes modulo 3000, collisions probed by steps of
// 211).
func TestIdentTable(t *testing.T) {
	defer InitIdents(nil)
	InitIdents(nil)
	// "toto": (116+111+116+111) mod 3000 = 454, ident1's IDENT(454)
	if i := IdentIndex("toto"); i != 454 || IdentName(454) != "toto" {
		t.Fatalf("toto at %d (%q)", i, IdentName(i))
	}
	if i := IdentIndex("toto"); i != 454 {
		t.Fatalf("toto again at %d", i)
	}
	// "otto" has the same sum: the next position, 454+211
	if i := IdentIndex("otto"); i != 665 || IdentName(665) != "otto" {
		t.Fatalf("otto at %d", i)
	}
	// the program's table: its indices are kept, a new name probes over
	// them (2999+... wraps modulo 3000)
	InitIdents(map[int32]string{97: "a", 308: "x1", 454: "f"})
	if IdentIndex("a") != 97 || IdentName(308) != "x1" {
		t.Fatal("program identifiers not at their index")
	}
	if i := IdentIndex("toto"); i != 665 {
		t.Fatalf("toto with 454 taken at %d", i)
	}
	// 2900 = 'd' (100) x 29: position 2900, then (2900+211) mod 3000 = 111
	InitIdents(map[int32]string{2900: "other"})
	long := string(make([]byte, 0))
	for range 29 {
		long += "d"
	}
	if i := IdentIndex(long); i != 111 {
		t.Fatalf("%s at %d, want 111", long, i)
	}
}

// An identifier is a term (a value of sort ident, its index), printed by
// its name, equal to the identifiers of the same index, ordered after the
// ints and before the strings and the other terms (REM's Gterm_cmp: by
// index).
func TestIdentTerms(t *testing.T) {
	defer InitIdents(nil)
	InitIdents(map[int32]string{97: "a", 98: "b", 729: "alpha"})
	a, b, alpha := MkIdent(97), MkIdent(98), MkIdent(729)
	if !IsIdent(a) || IsIdent(MkInt(97)) || MkIdent(97) != a || Eq(a, MkInt(97)) {
		t.Fatal("ident nodes")
	}
	if v, ok := IdentOf(alpha); !ok || v != 729 {
		t.Fatalf("IdentOf %d %v", v, ok)
	}
	if got := Format(Mk2(symTf, a, alpha)); got != "f(a,alpha)" {
		t.Fatalf("printed %q", got)
	}
	// the spacing of identifier tokens (U, a): after a letter, a space
	if got := Format(Mk1(symS, MkAC(symU, Flatten(symU, Mk1(symTg, b), Mk1(symTg, a))))); got != "s(g(a)U g(b))" {
		t.Fatalf("printed %q", got)
	}
	order := []Term{MkInt(5), a, b, alpha, MkStr("a"), ta}
	for i := range order {
		for j := range order {
			if c := Compare(order[i], order[j]); (c < 0) != (i < j) || (c == 0) != (i == j) {
				t.Errorf("Compare(%s, %s) = %d", Format(order[i]), Format(order[j]), c)
			}
		}
	}
	var w Writer
	w.Ident("f")
	w.IdentValue(98)
	if string(w.Bytes()) != "f b" {
		t.Fatalf("spacing %q", w.Bytes())
	}
}
