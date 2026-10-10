package elanrt

import "testing"

// Values checked with the interpreter (elan -b, macOS arm64; elanc-rs
// elan-runtime/tests/builtins.rs).
func TestIntArithmeticWrapsAt32Bits(t *testing.T) {
	max := IntLit(2147483647)
	min := 0 - max - 1
	one := int32(1)
	x := int32(65536)
	checks := []struct {
		name      string
		got, want int32
	}{
		{"0-2147483647-1", min, -2147483648},
		{"65536*65536", x * x, 0},
		{"65536*65536+2147483647+1", x*x + max + one, -2147483648},
		{"0-2147483647-1-1", min - one, 2147483647},
		{"65536*65536*3+1", x*x*3 + one, 1},
		{"2147483647*2", max * 2, -2},
		{"literal 4294967297", IntLit(4294967297), 1},
		{"-(MIN)", -min, min},
		{"(0-7)/2", Div(-7, 2), -3},
		{"(0-7)%3", Mod(-7, 3), -1},
		{"MIN/7 + MIN%7", Div(min, 7) + Mod(min, 7), -306783380},
		{"5/0", Div(5, 0), 0},
		{"5%0", Mod(5, 0), 5},
		{"(0-5)%0", Mod(-5, 0), -5},
		{"MIN/(0-1)", Div(min, -1), min},
		{"MIN%(0-1)", Mod(min, -1), 0},
		{"12 and 10", 12 & 10, 8},
		{"12 or 10", 12 | 10, 14},
	}
	for _, c := range checks {
		if c.got != c.want {
			t.Errorf("%s = %d, want %d", c.name, c.got, c.want)
		}
	}
}

func TestStringsAsTheInterpreter(t *testing.T) {
	if StrLen("abc") != 3 || StrCat("ab", "c d") != "abc d" {
		t.Error("strlen, strcat")
	}
	for _, c := range []struct {
		a, b string
		want int32
	}{{"a", "d", -1}, {"da", "a", 1}, {"ab", "ab", 0}, {"ab", "a", 1}, {"\xe9", "e", 1}} {
		if got := StrCmp(c.a, c.b); got != c.want {
			t.Errorf("strcmp(%q,%q) = %d, want %d", c.a, c.b, got, c.want)
		}
	}
	if StrSpn("aab", "a") != 2 || StrOfChar(65) != "A" {
		t.Error("strspn, string(65)")
	}
	if s, ok := StrSubstr("abcd", 1, 2); !ok || s != "bc" {
		t.Error("substr")
	}
	if _, ok := StrSubstr("abc", 1, 5); ok {
		t.Error("substr out of range must be stuck")
	}
	if c, ok := StrIndex("abc", 1); !ok || c != 98 {
		t.Error("index")
	}
	if _, ok := StrIndex("abc", 3); ok {
		t.Error("index out of range")
	}
	if s, ok := StrModif("abc", 0, 120); !ok || s != "xbc" {
		t.Error("modif")
	}
	if _, ok := StrModif("abc", -1, 120); ok {
		t.Error("modif out of range")
	}
}

// Strings are bytes (Latin-1 sources): a byte above 127 is one character,
// a signed C char for s[i].
func TestStringsAreBytes(t *testing.T) {
	if StrLen("caf\xe9") != 4 {
		t.Error("len")
	}
	if s, _ := StrSubstr("h\xc3\xa9llo", 1, 1); s != "\xc3" {
		t.Error("substr cuts bytes")
	}
	if c, _ := StrIndex("\xe9", 0); c != -23 {
		t.Error("signed char")
	}
	if StrOfChar(233) != "\xe9" || StrCat(StrOfChar(233), "t\xe9") != "\xe9t\xe9" {
		t.Error("string(233)")
	}
	if s, _ := StrModif("abc", 1, 0xe9); s != "a\xe9c" {
		t.Error("modif with a byte above 127")
	}
}

// C strings: string(c) and s[i <- c] store the low byte of c, and a NUL
// byte ends the string.
func TestStringsStopAtNUL(t *testing.T) {
	if StrOfChar(0) != "" || StrOfChar(256) != "" {
		t.Error("string(0)")
	}
	if s, _ := StrModif("abc", 1, 0); s != "a" {
		t.Error("s[1 <- 0]")
	}
	if s, _ := StrModif("xyz", 0, 256); s != "" {
		t.Error("s[0 <- 256]")
	}
	if s, _ := StrModif("abc", 2, 100+256); s != "abd" {
		t.Error("s[2 <- 356]")
	}
	if StrCat("ab", StrOfChar(0)) != "ab" || CString("a\x00b") != "a" {
		t.Error("strcat with NUL")
	}
}

// A builtin sort that can be stuck: values are terms; f(n) => n*2 if n > 0
// is stuck otherwise, and + applied to a stuck argument is stuck.
func TestStuckBuiltinTerms(t *testing.T) {
	symF := NewSym("f", 1)
	symAdd := DeclareSym(Symbol{Name: "+", Arity: 2, Print: []PrintItem{Arg(0), Char('+'), Arg(1)}})
	f := func(a Term) Term {
		return Lift1(symF, a, func(a Term) (Term, bool) {
			if n, _ := IntOf(a); n > 0 {
				return MkInt(n * 2), true
			}
			return nil, false
		})
	}
	plus := func(a, b Term) Term {
		return Lift2(symAdd, a, b, IntOp2(func(x, y int32) int32 { return x + y }))
	}
	one := MkInt(1)
	if r := plus(f(one), one); r != MkInt(3) {
		t.Fatalf("f(1)+1 = %s", Format(r))
	}
	st := plus(f(MkInt(0)), one)
	if IsValue(st) || Format(st) != "f(0)+1" {
		t.Fatalf("stuck: %s", Format(st))
	}
	if _, ok := IntOf(st); ok {
		t.Fatal("IntOf of a stuck term")
	}
	// stuck terms are shared terms
	if plus(f(MkInt(0)), one) != st {
		t.Fatal("stuck terms not shared")
	}
	lt := Lift2(symAdd, MkInt(1), MkInt(2), IntCmp2(func(x, y int32) bool { return x < y }))
	if lt != True {
		t.Fatal("IntCmp2")
	}
	if b, ok := BoolOf(True); !b || !ok {
		t.Fatal("BoolOf")
	}
	if s, ok := StrValue(MkStr("a")); !ok || s != "a" {
		t.Fatal("StrValue")
	}
}
