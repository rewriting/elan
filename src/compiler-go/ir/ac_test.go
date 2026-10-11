package ir

// AC operators and AC patterns (S7c task 1).

import (
	"strings"
	"testing"

	"elan/ref"
)

func ev(index, sort uint32) ref.Term { return ref.EVar{Index: index, Sort: sort} }

// acProg: sets of nats, u AC; single(n), mem(n, s).
func acProg() *prog {
	p := peano()
	p.op(sSet, 340, "u", []uint32{sSet, sSet}, 0, 2)
	p.op(sSet, 341, "single", []uint32{sNat}, 0, 0)
	p.op(sNat, 342, "mem", []uint32{sNat, sSet}, 0, 0)
	return p
}

func TestACOperatorAndItsRules(t *testing.T) {
	p := acProg()
	// [] u(x, x) => x, and its extension exported by the interpreter
	p.rule(sSet, 1, f(340, v(0, sSet), v(0, sSet)), v(0, sSet))
	p.rule(sSet, 2, f(340, ev(1, sSet), f(340, v(0, sSet), v(0, sSet))), f(340, ev(1, sSet), v(0, sSet)))
	// [] mem(n, u(single(n), s)) => o: under a free symbol, non-linear
	p.rule(sNat, 2, f(342, v(0, sNat), f(340, f(341, v(0, sNat)), v(1, sSet))), f(318))
	p.start(f(342, f(318), f(340, f(341, f(318)), f(341, f(318)))), sNat)
	ir := p.lower(t)
	u := opNamed(t, ir, "u")
	if !u.AC || u.Kind != Defined || !u.Variant || u.Total || len(u.Rules) != 1 || len(u.Skipped) != 1 {
		t.Fatalf("u: %+v", u)
	}
	r := &u.Rules[0]
	if len(r.Lhs) != 1 || r.Lhs[0].Kind != PatVar || len(r.Conds) != 1 || r.Conds[0].Kind != CondAC {
		t.Fatalf("u's rule: %+v", r)
	}
	pat := r.Conds[0].Pat
	if pat.Kind != PatAC || !pat.Ext || len(pat.Args) != 0 || len(pat.Vars) != 1 || pat.Vars[0].Mult != 2 {
		t.Fatalf("u(x, x): %+v", pat)
	}
	if !strings.Contains(u.Skipped[0], "extension") {
		t.Fatal(u.Skipped)
	}
	mem := opNamed(t, ir, "mem")
	mr := &mem.Rules[0]
	if mr.Conds[0].Kind != CondAC || mr.Conds[0].Pat.Ext || len(mr.Conds[0].Pat.Args) != 1 ||
		len(mr.Conds[0].Pat.Vars) != 1 || mr.Conds[1].Kind != CondSame {
		t.Fatalf("mem's rule: %+v", mr)
	}
	if mem.Total {
		t.Fatal("an AC match can fail: mem is not total")
	}
}

func TestACShapesOfTask2AreRefused(t *testing.T) {
	cases := map[string]func(p *prog){
		"more than two variables": func(p *prog) {
			p.rule(sNat, 3, f(342, v(0, sNat), f(340, v(1, sSet), f(340, v(2, sSet), v(3, sSet)))), f(318))
		},
		"one of them repeated": func(p *prog) {
			p.rule(sNat, 3, f(342, v(0, sNat), f(340, v(1, sSet), f(340, v(1, sSet), v(2, sSet)))), f(318))
		},
		"in a where": func(p *prog) {
			p.rule(sNat, 3, f(342, v(0, sNat), v(1, sSet)), f(318),
				ref.Match{Sort: sSet, Pattern: f(340, f(341, v(0, sNat)), v(2, sSet)), Rhs: v(1, sSet)})
		},
	}
	for what, add := range cases {
		p := acProg()
		add(p)
		p.start(f(342, f(318), f(340, f(341, f(318)), f(341, f(318)))), sNat)
		if st, msg := unsupportedStage(t, p); st != "S7c" || !strings.Contains(msg, what) || !strings.Contains(msg, "task 2") {
			t.Errorf("%s: %s %s", what, st, msg)
		}
	}
	// an AC operator on a builtin sort
	p := peano()
	p.op(sInt, 343, "w", []uint32{sInt, sInt}, 0, 2)
	p.start(f(343, ref.Int{Value: 1}, ref.Int{Value: 2}), sInt)
	if st, msg := unsupportedStage(t, p); st != "S7c" || !strings.Contains(msg, "builtin sort") {
		t.Errorf("%s %s", st, msg)
	}
}
