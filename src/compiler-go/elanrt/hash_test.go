package elanrt

import "testing"

// builtinHashTerm (code 210, S7e task 7) as REM's hashTerm
// (termBase.c: doobs_hfunction over the .ref codes, ints tagged as REM's,
// 16 bits): the values of a REM program (Compiler.4.0/Work testHashSet's
// hash sets; a program printing them: r(23804,50151,11,_,47920)).
func TestHashTermAsREM(t *testing.T) {
	a := Mk0(DeclareSym(Symbol{Name: "a", Code: 300}))
	f := DeclareSym(Symbol{Name: "f", Arity: 2, Code: 301})
	fa := Mk2(f, MkInt(1), a)
	for _, c := range []struct {
		t    Term
		want int32
	}{
		{a, 23804},
		{fa, 50151},
		{MkInt(5), 11},
		{Mk2(f, MkInt(2), fa), 47920},
		{MkInt(-1), 0xFFFF},
	} {
		if got := HashTerm(c.t); got != c.want {
			t.Errorf("hash of %s: %d, REM %d", Format(c.t), got, c.want)
		}
	}
	// strings: a value of elanc-go's (REM's depends on the string's
	// address), the same for the same string
	if HashTerm(MkStr("ab")) != HashTerm(MkStr("ab")) || HashTerm(MkStr("ab")) < 0 {
		t.Error("strings")
	}
}
