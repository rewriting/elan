package ir

import (
	"slices"
	"testing"

	"elan/ref"
)

// wide is the shape of Robot/tictac's moves (docs/followups.md): rule i
// tests that argument i of mv is e (the others are variables), then
// mv(x, y, ...) and mv(...) (variables only); the results are the
// constants c0, c1, ...: a Maranget tree of these rules has 2^n paths.
func wide(n int) *prog {
	p := newProg()
	p.op(sNat, 318, "e", nil, 0, 0)
	p.op(sNat, 319, "x", nil, 0, 0)
	p.op(sNat, 320, "y", nil, 0, 0)
	args := make([]uint32, n)
	for i := range args {
		args[i] = sNat
	}
	p.op(sNat, 321, "mv", args, 0, 0)
	for i := range n + 2 {
		p.op(sNat, int32(330+i), "c"+string(rune('a'+i)), nil, 0, 0)
	}
	vars := func() []ref.Term {
		ts := make([]ref.Term, n)
		for i := range ts {
			ts[i] = v(uint32(i), sNat)
		}
		return ts
	}
	for i := range n {
		ts := vars()
		ts[i] = f(318)
		p.rule(sNat, uint32(n), f(321, ts...), f(int32(330+i)))
	}
	ts := vars()
	ts[0], ts[1] = f(319), f(320)
	p.rule(sNat, uint32(n), f(321, ts...), f(int32(330+n)))
	p.rule(sNat, uint32(n), f(321, vars()...), f(int32(330+n+1)))
	start := make([]ref.Term, n)
	for i := range start {
		start[i] = f(int32(318 + i%3))
	}
	p.start(f(321, start...), sNat)
	return p
}

func nodes(d *Decision) int {
	if d == nil {
		return 0
	}
	k := 1 + nodes(d.Next) + nodes(d.Default)
	for _, c := range d.Cases {
		k += nodes(c.Then)
	}
	return k
}

// tried are the rules that d tries, in order, for the arguments of heads
// (constants), when no rule applies (each Try goes on with Next).
func tried(d *Decision, heads []OpID) []RuleID {
	var rs []RuleID
	var walk func(d *Decision)
	walk = func(d *Decision) {
		switch d.Kind {
		case Try:
			rs = append(rs, d.Rule)
			walk(d.Next)
		case Switch:
			h := heads[d.Occ[0]]
			var then *Decision
			for _, c := range d.Cases {
				if c.Head.Kind == HeadOp && c.Head.Op == h {
					then = c.Then
				}
			}
			if then == nil {
				then = d.Default
			}
			if then != nil {
				walk(then)
			}
			if d.Next != nil {
				walk(d.Next)
			}
		}
	}
	walk(d)
	return rs
}

func TestWideRulesHaveALinearTree(t *testing.T) {
	const n = 10
	ir := wide(n).lower(t)
	mv := opNamed(t, ir, "mv")
	if len(mv.Rules) != n+2 || !mv.Total {
		t.Fatalf("%d rules, total %v", len(mv.Rules), mv.Total)
	}
	// the Maranget tree has more than 2^n nodes: split (a Next after a
	// Switch), within the budget
	var rows []row
	for i := range mv.Rules {
		r := row{rule: i}
		for j := range mv.Rules[i].Lhs {
			r.pats = append(r.pats, &mv.Rules[i].Lhs[j])
		}
		rows = append(rows, r)
	}
	var split func(d *Decision) bool
	split = func(d *Decision) bool {
		if d == nil {
			return false
		}
		if d.Kind == Switch && d.Next != nil {
			return true
		}
		return split(d.Next) || split(d.Default) || slices.ContainsFunc(d.Cases, func(c Case) bool { return split(c.Then) })
	}
	if !split(mv.Tree) {
		t.Fatal("the tree is not split")
	}
	if k, max := nodes(mv.Tree), treeBudget(rows); k > max {
		t.Fatalf("the tree has %d nodes, more than %d", k, max)
	}
	// every tuple of heads: the rules that match, in source order
	opOf := map[string]OpID{}
	for i := range ir.Ops {
		opOf[ir.Ops[i].Name] = i
	}
	vals := []OpID{opOf["e"], opOf["x"], opOf["y"]}
	heads := make([]OpID, n)
	for code := 0; code < 59049; code++ { // 3^10
		c := code
		for i := range heads {
			heads[i] = vals[c%3]
			c /= 3
		}
		var want []RuleID
		for i := range n {
			if heads[i] == vals[0] {
				want = append(want, i)
			}
		}
		if heads[0] == vals[1] && heads[1] == vals[2] {
			want = append(want, n)
		}
		want = append(want, n+1)
		if got := tried(mv.Tree, heads); !slices.Equal(got, want) {
			t.Fatalf("%v: rules %v, want %v", heads, got, want)
		}
	}
}
