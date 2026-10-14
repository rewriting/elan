package ir

// AC operators and AC patterns (S7c tasks 1 and 2).

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

// S7c task 2: the shapes REM matches with Eker's matcher or its
// Diophantine code.
func TestACShapesOfTask2(t *testing.T) {
	lower := func(add func(p *prog)) *Rule {
		p := acProg()
		add(p)
		p.start(f(342, f(318), f(340, f(341, f(318)), f(341, f(318)))), sNat)
		ir := p.lower(t)
		return &opNamed(t, ir, "mem").Rules[0]
	}
	// three variables: F(x1, X) where F(x2, x3) :=() X (REM's
	// transformation3_1: the first variable in REM's order stays)
	r := lower(func(p *prog) {
		p.rule(sNat, 3, f(342, v(0, sNat), f(340, v(3, sSet), f(340, v(2, sSet), v(1, sSet)))), f(318))
	})
	if len(r.Conds) != 2 || r.Conds[0].Kind != CondAC || r.Conds[1].Kind != CondEker {
		t.Fatalf("three variables: %+v", r.Conds)
	}
	pat, ek := r.Conds[0].Pat, r.Conds[1]
	if pat.Abs == nil || len(pat.Vars) != 1 || r.Vars[pat.Vars[0].Var].Name != "x1" || ek.A != pat.Abs.Var {
		t.Fatalf("three variables: %+v", pat)
	}
	if e := ek.Eker; e.Kind != PatAC || len(e.Args) != 2 || e.Args[0].Var != 0 || e.Args[1].Var != 1 ||
		r.Vars[ek.EkerVars[0]].Name != "x2" || r.Vars[ek.EkerVars[1]].Name != "x3" {
		t.Fatalf("the abstracted part: %+v %v", e, ek.EkerVars)
	}
	// x U x U y: x (repeated) first, REM's maximal extract; x^2 U y^2: the
	// variable REM does not abstract first
	r = lower(func(p *prog) {
		p.rule(sNat, 3, f(342, v(0, sNat), f(340, v(2, sSet), f(340, v(1, sSet), v(1, sSet)))), f(318))
	})
	if vs := r.Conds[0].Pat.Vars; len(vs) != 2 || vs[0].Mult != 2 || vs[1].Mult != 1 || r.Conds[0].Pat.Abs != nil {
		t.Fatalf("x U x U y: %+v", vs)
	}
	r = lower(func(p *prog) {
		p.rule(sNat, 3, f(342, v(0, sNat), f(340, f(340, v(1, sSet), v(1, sSet)), f(340, v(2, sSet), v(2, sSet)))), f(318))
	})
	if vs := r.Conds[0].Pat.Vars; len(vs) != 2 || r.Vars[vs[0].Var].Name != "x2" || r.Vars[vs[1].Var].Name != "x1" {
		t.Fatalf("x U x U y U y: %+v", vs)
	}
	// a where: the whole pattern matched by Eker's matcher, its variables
	// numbered in REM's order (the variable before the symbol)
	r = lower(func(p *prog) {
		p.rule(sNat, 3, f(342, v(0, sNat), v(1, sSet)), f(318),
			ref.Match{Sort: sSet, Pattern: f(340, f(341, v(2, sNat)), v(3, sSet)), Rhs: v(1, sSet)})
	})
	if len(r.Conds) != 2 || r.Conds[0].Kind != CondMatch || r.Conds[0].Pat.Kind != PatVar || r.Conds[1].Kind != CondEker {
		t.Fatalf("where: %+v", r.Conds)
	}
	if e := r.Conds[1].Eker; e.Args[0].Kind != PatVar || e.Args[0].Var != 0 || e.Args[1].Kind != PatOp ||
		e.Args[1].Args[0].Var != 1 || r.Vars[r.Conds[1].EkerVars[0]].Name != "x3" {
		t.Fatalf("where pattern: %+v", e)
	}
	// an AC operator on a builtin sort (not planned: the oracles disagree)
	p := peano()
	p.op(sInt, 343, "w", []uint32{sInt, sInt}, 0, 2)
	p.start(f(343, ref.Int{Value: 1}, ref.Int{Value: 2}), sInt)
	if st, msg := unsupportedStage(t, p); st != NotPlanned || !strings.Contains(msg, "builtin sort") {
		t.Errorf("%s %s", st, msg)
	}
}

// REM's transformation2 abstracts every AC subterm of the level of the
// remaining AC pattern (collectACSubterm: one node each), equal ones
// included when they are not the same item of the level (a direct item
// and one under a free symbol): two wheres matched by Eker's matcher,
// then the level u(box(X1), X2, y) of two variables: a third one
// (transformation3_1).
func TestTransformation2EqualSubterms(t *testing.T) {
	p := acProg()
	p.op(sSet, 343, "w", []uint32{sSet, sSet}, 0, 2)
	p.op(sSet, 344, "box", []uint32{sSet}, 0, 0)
	x, y := v(1, sSet), v(2, sSet)
	wx := func() ref.Term { return f(343, x, f(341, f(318))) }
	// mem(n, u(w(x, single(o)), u(box(w(x, single(o))), y))) => o
	p.rule(sNat, 3, f(342, v(0, sNat), f(340, wx(), f(340, f(344, wx()), y))), f(318))
	p.start(f(342, f(318), f(340, f(341, f(318)), f(341, f(318)))), sNat)
	ir := p.lower(t)
	r := &opNamed(t, ir, "mem").Rules[0]
	n := 0
	for _, c := range r.Conds {
		if c.Kind == CondEker {
			n++
		}
	}
	if n != 3 {
		t.Fatalf("three wheres expected: %+v", r.Conds)
	}
}
