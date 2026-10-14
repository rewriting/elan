package ir

import (
	"fmt"
	"math/rand/v2"
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
		p.op(sNat, int32(330+i), fmt.Sprintf("c%d", i), nil, 0, 0)
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
	k := 1 + nodes(d.Next) + nodes(d.Default) + nodes(d.Match)
	for _, c := range d.Cases {
		k += nodes(c.Then)
	}
	for _, c := range d.Cands {
		k += nodes(c)
	}
	return k
}

// tried are the rules that d tries, in order, for the arguments of heads
// (constants), when no rule applies (each Try goes on with Next), and the
// number of positions tested.
func tried(d *Decision, heads []OpID) ([]RuleID, int) {
	var rs []RuleID
	tests := 0
	var mask []uint64
	var walk func(d *Decision)
	walk = func(d *Decision) {
		if d == nil {
			return
		}
		switch d.Kind {
		case Try:
			rs = append(rs, d.Rule)
			walk(d.Next)
		case Switch:
			tests++
			h := heads[d.Occ[0]]
			then := d.Default
			for _, c := range d.Cases {
				if c.Head.Kind == HeadOp && c.Head.Op == h {
					then = c.Then
				}
			}
			walk(then)
			walk(d.Next)
		case Mask:
			for w := range mask {
				mask[w] &= d.Mask[w]
			}
			walk(d.Next)
		case Bits:
			mask = make([]uint64, (len(d.Cands)+63)/64)
			for k := range d.Cands {
				mask[k/64] |= 1 << (k % 64)
			}
			walk(d.Match)
			for k, c := range d.Cands {
				if mask[k/64]&(1<<(k%64)) != 0 {
					rs = append(rs, c.Rule)
				}
			}
		}
	}
	walk(d)
	return rs, tests
}

func TestWideRulesHaveALinearTree(t *testing.T) {
	// every tuple of heads of 10 cells
	checkWide(t, 10, 59049, func(code int, heads []OpID, vals []OpID) {
		for i := range heads {
			heads[i] = vals[code%3]
			code /= 3
		}
	})
}

// TestWideRulesOverAWord: 72 rules, two words of mask; random tuples.
func TestWideRulesOverAWord(t *testing.T) {
	r := rand.New(rand.NewPCG(1, 2))
	checkWide(t, 70, 3000, func(_ int, heads []OpID, vals []OpID) {
		for i := range heads {
			// e rarely: several candidates, some beyond bit 64
			heads[i] = vals[1+r.IntN(2)]
			if r.IntN(10) == 0 {
				heads[i] = vals[0]
			}
		}
	})
}

// checkWide checks the bit-vector matching of wide(n) on count tuples of
// heads (fill writes the code-th): the rules tried are the ones that
// match, in source order, and one test per cell at most.
func checkWide(t *testing.T, n, count int, fill func(code int, heads []OpID, vals []OpID)) {
	t.Helper()
	ir := wide(n).lower(t)
	mv := opNamed(t, ir, "mv")
	if len(mv.Rules) != n+2 || !mv.Total {
		t.Fatalf("%d rules, total %v", len(mv.Rules), mv.Total)
	}
	// the Maranget tree has more than 2^n nodes: bit-vector matching, in
	// size linear in the patterns (times the words of the masks)
	if mv.Tree.Kind != Bits {
		t.Fatalf("not a bit-vector matching: %v", mv.Tree.Kind)
	}
	size := 0
	for i := range mv.Rules {
		for j := range mv.Rules[i].Lhs {
			size += patSize(&mv.Rules[i].Lhs[j])
		}
	}
	if k := nodes(mv.Tree); k > 4*size {
		t.Fatalf("the matching has %d nodes for %d pattern nodes", k, size)
	}
	opOf := map[string]OpID{}
	for i := range ir.Ops {
		opOf[ir.Ops[i].Name] = i
	}
	vals := []OpID{opOf["e"], opOf["x"], opOf["y"]}
	heads := make([]OpID, n)
	for code := range count {
		fill(code, heads, vals)
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
		got, tests := tried(mv.Tree, heads)
		if !slices.Equal(got, want) {
			t.Fatalf("%v: rules %v, want %v", heads, got, want)
		}
		// one test per position of the patterns (n cells), whatever the
		// number of rules
		if tests > n {
			t.Fatalf("%v: %d positions tested", heads, tests)
		}
	}
}
