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
	for _, want := range []string{"rt.MergeAC(sym_u_340, a0, a1)", "rt.ACTerm(sym_u_340, e)", "rt.NormalForm(a0, steps)", "rt.NewDown(", "rt.FirstOcc("} {
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

// S7c task 2: a where pattern matched by Eker's matcher; three variables
// at an AC level (REM's transformation into a where); x U x U y compiles.
func TestEkerProgramRuns(t *testing.T) {
	const sSet = 401
	p := acSets()
	p.op(sNat, 343, "pick", []uint32{sSet}, 0)
	p.op(sNat, 344, "three", []uint32{sSet}, 0)
	p.op(sNat, 345, "twice", []uint32{sSet}, 0)
	n, s, y, z := v(1, sNat), v(2, sSet), v(3, sSet), v(4, sSet)
	// pick(s) => n where u(single(n), u(y, z)) :=() s
	p.rule(sNat, f(343, s), n, ref.Match{Sort: sSet, Pattern: f(340, f(341, n), f(340, y, z)), Rhs: s})
	// three(u(y, u(z, single(n)))) => n
	p.rule(sNat, f(344, f(340, y, f(340, z, f(341, n)))), n)
	// twice(u(y, u(y, single(n)))) => n
	p.rule(sNat, f(345, f(340, y, f(340, y, f(341, n)))), n)
	o, so, sso := f(318), f(319, f(318)), f(319, f(319, f(318)))
	set := f(340, f(341, o), f(340, f(341, so), f(341, sso)))
	twice := f(340, f(341, so), f(340, f(341, so), f(341, o)))
	p.p.Query.StartWith = f(400, set, f(343, set), f(344, set))
	src := p.gen(t)
	for _, want := range []string{"rt.NewEker(ekerPat0, 3, ", "ekerPat1 = ", "func acNorm(", "rt.ACSubject(sym_u_340, "} {
		if !strings.Contains(src, want) {
			t.Errorf("no %s in\n%s", want, src)
		}
	}
	// the abstracted part of three's pattern, a part of a term of u (which
	// has rules) given to Eker's matcher: a transient node, never a node of
	// the table (AC terms enter it only as normal forms)
	if strings.Contains(src, "rt.MkAC(sym_u_340, ") {
		t.Errorf("rt.MkAC of u, an operator with rules, in\n%s", src)
	}
	// Eker: the subjects in descending order (single(o) first: a constant
	// argument comes after a function), the element pattern first
	want := "\nresult = r(u(single(o),u(single(s(o)),single(s(s(o))))),o,o)\n"
	if got := run(t, p); !strings.HasPrefix(got, want) {
		t.Errorf("got %q, want %q", got, want)
	}
	// (u is idempotent here: u(x, x) => x; the shape is tested end to end
	// in tests/compiler-rs/programs/ac)
	p.p.Query.StartWith = f(400, twice, f(345, set), f(345, set))
	if got := run(t, p); !strings.HasPrefix(got, "\nresult = r(u(single(o),single(s(o))),twice(") {
		t.Errorf("x U x U y: %q", got)
	}
}
