package gen

// AC operators and AC matching (S7c task 1): generated programs built and
// run (the end-to-end tests against the interpreter and REM are
// tests/compiler-rs/programs/ac).

import (
	"context"
	"os/exec"
	"path/filepath"
	"strings"
	"testing"
	"time"

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
	p.op(sSet, 346, "w", []uint32{sSet, sSet}, 0)
	p.setAC(346)
	n, s, y, z := v(1, sNat), v(2, sSet), v(3, sSet), v(4, sSet)
	// pick(s) => n where u(single(n), u(y, z)) :=() s
	p.rule(sNat, f(343, s), n, ref.Match{Sort: sSet, Pattern: f(340, f(341, n), f(340, y, z)), Rhs: s})
	// three(u(y, u(z, single(n)))) => n
	p.rule(sNat, f(344, f(340, y, f(340, z, f(341, n)))), n)
	// twice(w(y, w(y, single(n)))) => n (w: AC, no rule)
	p.rule(sNat, f(345, f(346, y, f(346, y, f(341, n)))), n)
	o, so, sso := f(318), f(319, f(318)), f(319, f(319, f(318)))
	set := f(340, f(341, o), f(340, f(341, so), f(341, sso)))
	// w(single(s(o)), single(o), single(s(s(o))), single(o), single(s(o))):
	// y = w(single(o), single(s(o))), n = s(s(o)), the only solution;
	// w(single(o), single(s(o)), single(s(s(o)))): no solution
	twice := f(346, f(341, so), f(346, f(341, o), f(346, f(341, sso), f(346, f(341, o), f(341, so)))))
	odd := f(346, f(341, o), f(346, f(341, so), f(341, sso)))
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
	// x U x U y: y twice and an element
	p.p.Query.StartWith = f(400, f(340, f(341, o), f(341, so)), f(345, twice), f(345, odd))
	want = "\nresult = r(u(single(o),single(s(o))),s(s(o)),twice(w(single(o),w(single(s(o)),single(s(s(o)))))))\n"
	if got := run(t, p); !strings.HasPrefix(got, want) {
		t.Errorf("x U x U y: got %q, want %q", got, want)
	}
}

// nonLinearAC: u(x, w(x, y)) => y (u, w AC; w without rules): x at the
// level of u modulo an extension, its 2^n sub-multisets the candidates,
// compared with x in w(...). The start term u(w(o, s(o)), w(o, s(o)),
// s^2(o), ..., s^(n+1)(o)) has no solution.
func nonLinearAC(n int) *prog {
	p := newProg()
	p.op(sNat, 318, "o", nil, 0)
	p.op(sNat, 319, "s", []uint32{sNat}, 0)
	p.op(sNat, 340, "u", []uint32{sNat, sNat}, 0)
	p.setAC(340)
	p.op(sNat, 341, "w", []uint32{sNat, sNat}, 0)
	p.setAC(341)
	x, y, ext := v(0, sNat), v(1, sNat), ref.EVar{Index: 2, Sort: sNat}
	p.rule(sNat, f(340, x, f(341, x, y)), y)
	p.rule(sNat, f(340, ext, f(340, x, f(341, x, y))), f(340, ext, y))
	o := f(318)
	w := f(341, o, f(319, o))
	t, e := f(340, w, w), f(319, o)
	for range n {
		e = f(319, e)
		t = f(340, e, t)
	}
	p.p.Query.StartWith = t
	return p
}

// TestNonLinearACLevelVariable: the candidates of a variable that occurs
// twice are compared as they are (the multiset), only the chosen one is
// normalised: normalising each of the 2^n candidates (each one a
// normalisation of u that enumerates its own sub-multisets) took 24 s
// with n = 16 elements besides the two w (tests/compiler-rs/programs/ac
// nl_slow; REM 0.1 s).
func TestNonLinearACLevelVariable(t *testing.T) {
	if testing.Short() {
		t.Skip("go build")
	}
	dir, src := module(t, nonLinearAC(16))
	build := goCmd(dir, "build", "-trimpath", "-buildvcs=false", "-o", "prog", ".")
	if out, err := build.CombinedOutput(); err != nil {
		t.Fatalf("go build: %v\n%s\n%s", err, out, src)
	}
	ctx, cancel := context.WithTimeout(context.Background(), 2*time.Second)
	defer cancel()
	out, err := exec.CommandContext(ctx, filepath.Join(dir, "prog"), "-noInput", "-quiet").Output()
	if ctx.Err() != nil {
		t.Fatalf("not finished in 2 s (exponential enumeration of the candidates)")
	}
	if err != nil {
		t.Fatalf("run: %v\n%s", err, out)
	}
	if !strings.HasSuffix(string(out), "\nrewrite_step = 0\n") {
		t.Errorf("got %q", out)
	}
}
