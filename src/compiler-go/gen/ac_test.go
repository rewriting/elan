package gen

// AC operators and AC matching (S7c task 1): generated programs built and
// run (the end-to-end tests against the interpreter and REM are
// tests/compiler-rs/programs/ac).

import (
	"strings"
	"testing"

	"elan/ref"
)

// setAC makes the operator of symbol sym AC (infos 2).
func (p *prog) setAC(sym int32) {
	for i := range p.p.Grammars.Both {
		ops := p.p.Grammars.Both[i].Operators
		for j := range ops {
			if ops[j].Symbol == sym {
				ops[j].Infos = 2
			}
		}
	}
}

// acSets: u AC (prefix), single(n); [] u(x, x) => x (modulo an
// extension, exported as its extension rule by the interpreter);
// [] mem(n, u(single(n), s)) => o; [] mem(n, s) => s(o); the result
// r(u(single(o), single(s(o)), single(o)), mem(s(o), ...), mem(s(s(o)),
// ...)).
func acSets() *prog {
	const sSet = 401
	p := newProg()
	p.p.Sorts = append(p.p.Sorts, ref.Sort{Index: sSet, Name: "set"})
	p.op(sNat, 318, "o", nil, 0)
	p.op(sNat, 319, "s", []uint32{sNat}, 0)
	p.op(sSet, 340, "u", []uint32{sSet, sSet}, 0)
	p.setAC(340)
	p.op(sSet, 341, "single", []uint32{sNat}, 0)
	p.op(sNat, 342, "mem", []uint32{sNat, sSet}, 0)
	p.op(sOut, 400, "r", []uint32{sSet, sNat, sNat}, 0)
	x, e := v(0, sSet), ref.EVar{Index: 1, Sort: sSet}
	p.rule(sSet, f(340, x, x), x)
	p.rule(sSet, f(340, e, f(340, x, x)), f(340, e, x))
	n, s := v(0, sNat), v(1, sSet)
	p.rule(sNat, f(342, n, f(340, f(341, n), s)), f(318))
	p.rule(sNat, f(342, n, s), f(319, f(318)))
	o, so := f(318), f(319, f(318))
	set := f(340, f(341, so), f(340, f(341, o), f(341, so)))
	p.p.Query.ResultSort = sOut
	p.p.Query.StartWith = f(400, set, f(342, o, set), f(342, f(319, so), set))
	return p
}

func TestACProgramRuns(t *testing.T) {
	src := acSets().gen(t)
	for _, want := range []string{"rt.MkAC2(sym_u_340, a0, a1)", "rt.NewDown(", "rt.FirstOcc("} {
		if !strings.Contains(src, want) {
			t.Errorf("no %s in\n%s", want, src)
		}
	}
	// u(single(s(o)), single(o), single(s(o))): the extension keeps
	// single(o); sorted by code then arguments, printed as a right comb
	want := "\nresult = r(u(single(o),single(s(o))),o,s(o))\n\nrewrite_step = 5\n" // (u three times, mem twice)
	if got := run(t, acSets()); got != want {
		t.Errorf("got %q, want %q", got, want)
	}
}
