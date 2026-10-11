package ir

// Unit tests of the labels and strategies of the IR (S7b), as elanc-rs's
// tests/ir.rs: labels, strategies, determinism, totality, tail calls.

import (
	"fmt"
	"reflect"
	"strings"
	"testing"

	"elan/ref"
)

func (p *prog) strategy(name uint32, body ...ref.StrategyExpr) {
	p.p.Strategies = append(p.p.Strategies, ref.StrategyDef{Name: name, Sort: sNat, Module: 1, Body: body})
	p.p.StrategyNames = append(p.p.StrategyNames, ref.TableEntry{Index: name, Name: fmt.Sprintf("st%d:nat/m", name)})
}

func (p *prog) labelled(label, nvars uint32, lhs, rhs ref.Term, wheres ...ref.Where) {
	p.rule(sNat, nvars, lhs, rhs, wheres...)
	p.p.Rules[len(p.p.Rules)-1].Name = ref.Some(label)
	for _, e := range p.p.RuleNames {
		if e.Index == label {
			return
		}
	}
	p.p.RuleNames = append(p.p.RuleNames, ref.TableEntry{Index: label, Name: fmt.Sprintf("r%d:nat/m!GL", label)})
}

func rules(op ref.StrategyOp, ls ...uint32) ref.StrategyExpr {
	return ref.StrategyExpr{Op: op, Rules: ls}
}
func strats(op ref.StrategyOp, alts ...ref.StrategySeq) ref.StrategyExpr {
	return ref.StrategyExpr{Op: op, Alts: alts}
}
func call(n uint32) ref.StrategyExpr { return ref.StrategyExpr{Op: ref.SCall, Call: n} }

var id = ref.StrategyExpr{Op: ref.SId}

func strategyNames(ir *IR) []string {
	var v []string
	for _, s := range ir.Strategies {
		v = append(v, s.Name)
	}
	return v
}

// Labels: all the rules of a label in source order, the left-hand side as
// one pattern; labelled rules do not normalise; determinism.
func TestLabelsAndStrategies(t *testing.T) {
	p := peano()
	p.op(sNat, 321, "a", nil, 0, 0)
	// [r7] a => s(o)  [r8] x => s(x)  [r7] s(x) => x  [r7] a => o
	p.labelled(7, 0, f(321), f(319, f(318)))
	p.labelled(8, 1, v(0, sNat), f(319, v(0, sNat)))
	p.labelled(7, 1, f(319, v(0, sNat)), v(0, sNat))
	p.labelled(7, 0, f(321), f(318))
	// st3 = dk(r8, r7) ; st4      st4 = repeat*(one(r7))
	// st5 = dc(r7, r9)            [r9] o => o where x := (st3) o
	p.strategy(3, rules(ref.SDkRules, 8, 7), call(4))
	p.strategy(4, ref.StrategyExpr{Op: ref.SRepeat, Body: ref.StrategySeq{rules(ref.SOneRules, 7)}})
	p.strategy(5, rules(ref.SDcRules, 7, 9))
	p.labelled(9, 1, f(318), f(318), ref.Assign{Lhs: v(0, sNat), Strategy: ref.Some(3), Rhs: f(318)})
	p.p.Query.Strategy = ref.Some(5)
	p.start(f(321), sNat)
	ir := p.lower(t)
	// a has labelled rules only: a constructor
	if a := opNamed(t, ir, "a"); a.Kind != Constructor || !a.Variant {
		t.Fatalf("a: %v", a.Kind)
	}
	var labels []string
	for _, l := range ir.Labels {
		labels = append(labels, l.Name)
	}
	if !reflect.DeepEqual(labels, []string{"r7:nat/m!GL", "r8:nat/m!GL", "r9:nat/m!GL"}) {
		t.Fatal(labels)
	}
	r7 := &ir.Labels[0]
	if len(r7.Rules) != 3 || !strings.HasPrefix(r7.Rules[0].Text, "[r7] a =>") || r7.Fun != "rl_r7_7" {
		t.Fatalf("%+v", r7)
	}
	for _, r := range r7.Rules {
		if len(r.Lhs) != 1 {
			t.Fatal(r.Text)
		}
	}
	if got := strategyNames(ir); !reflect.DeepEqual(got, []string{"st3:nat/m", "st4:nat/m", "st5:nat/m"}) {
		t.Fatal(got)
	}
	want := []Strat{{Kind: StRules, Choice: Dk, Labels: []LabelID{1, 0}}, {Kind: StCall, Call: 1}}
	if !reflect.DeepEqual(ir.Strategies[0].Body, want) {
		t.Fatalf("%+v", ir.Strategies[0].Body)
	}
	if ir.Start.Strategy != 2 {
		t.Fatal(ir.Start.Strategy)
	}
	// st3: dk over 4 rules; st4: repeat* of one; st5: dc of r7 (3 rules
	// without where) and r9 (a where with st3)
	var det []bool
	for _, s := range ir.Strategies {
		det = append(det, s.Det)
	}
	if !reflect.DeepEqual(det, []bool{false, true, false}) {
		t.Fatal(det)
	}
	if !ir.Labels[0].RulesDet || ir.Labels[2].RulesDet {
		t.Fatal("rules_det")
	}
	c := ir.Labels[2].Rules[0].Conds
	if len(c) != 1 || c[0].Kind != CondApply || c[0].Strategy != 0 || c[0].Pat.Kind != PatVar {
		t.Fatalf("%+v", c)
	}
	if text := ir.Labels[2].Rules[0].Text; text != "[r9] o => o where x0 := (st3) o" {
		t.Fatal(text)
	}
}

// Deterministic strategies calling themselves in tail position are loops
// (spec G4); totality.
func TestTailCallsAndTotality(t *testing.T) {
	p := peano()
	// [r7] s(x) => x   [r8] x => s(x)   [r8] s(x) => x
	p.labelled(7, 1, f(319, v(0, sNat)), v(0, sNat))
	p.labelled(8, 1, v(0, sNat), f(319, v(0, sNat)))
	p.labelled(8, 1, f(319, v(0, sNat)), v(0, sNat))
	dc7 := rules(ref.SDcRules, 7)
	// st3 = DC(dc(r7) ; st3, id)        total, a tail call
	// st4 = DC(dk(r8) ; st4, id)        not deterministic: no loop
	// st5 = DC(dc(r7) ; st5, dc(r7))    st5 can fail: not in tail position
	// st6 = DC(dc(r7), dc(r7) ; st6)    last alternative: a tail call
	// st7 = ONE(dk(r8) ; st7, id)       dk(r8) before the call: no loop
	// st9 = iterate*(dc(r7))
	// st10 = DK(st3, st4, st5, st6, st7, st9) (start)
	p.strategy(3, strats(ref.SDcStrats, ref.StrategySeq{dc7, call(3)}, ref.StrategySeq{id}))
	p.strategy(4, strats(ref.SDcStrats, ref.StrategySeq{rules(ref.SDkRules, 8), call(4)}, ref.StrategySeq{id}))
	p.strategy(5, strats(ref.SDcStrats, ref.StrategySeq{dc7, call(5)}, ref.StrategySeq{dc7}))
	p.strategy(6, strats(ref.SDcStrats, ref.StrategySeq{dc7}, ref.StrategySeq{dc7, call(6)}))
	p.strategy(7, strats(ref.SOneStrats, ref.StrategySeq{rules(ref.SDkRules, 8), call(7)}, ref.StrategySeq{id}))
	p.strategy(9, ref.StrategyExpr{Op: ref.SIterate, Body: ref.StrategySeq{dc7}})
	var alts []ref.StrategySeq
	for _, c := range []uint32{3, 4, 5, 6, 7, 9} {
		alts = append(alts, ref.StrategySeq{call(c)})
	}
	p.strategy(10, strats(ref.SDkStrats, alts...))
	p.p.Query.Strategy = ref.Some(10)
	p.start(f(318), sNat)
	ir := p.lower(t)
	if got := strategyNames(ir); !reflect.DeepEqual(got, []string{"st3:nat/m", "st4:nat/m", "st5:nat/m",
		"st6:nat/m", "st7:nat/m", "st9:nat/m", "st10:nat/m"}) {
		t.Fatal(got)
	}
	total := ir.TotalStrategies()
	if !reflect.DeepEqual(total, []bool{true, true, false, false, true, true, true}) {
		t.Fatal(total)
	}
	var tails []int
	for i := range ir.Strategies {
		tails = append(tails, len(ir.TailCalls(i, total)))
	}
	if !reflect.DeepEqual(tails, []int{1, 0, 0, 1, 0, 0, 0}) {
		t.Fatal(tails)
	}
	// the tail call of st3 is the node in its body
	for s := range ir.TailCalls(0, total) {
		if s != &ir.Strategies[0].Body[0].Alts[0][1] {
			t.Fatal("not the call node of the body")
		}
	}
	if got := ir.SeqText(ir.Strategies[0].Body); got != "DC(dc(r7) ; st3, id)" {
		t.Fatal(got)
	}
}

// The determinism of the elementary strategies (StrategyDet).
func TestStrategyDet(t *testing.T) {
	rulesDet := func(l LabelID) bool { return l != 2 }
	nRules := func(l LabelID) int { return l + 1 } // label 0: one rule
	stratDet := func(c StratID) bool { return c == 0 }
	fail := Strat{Kind: StFail}
	for _, c := range []struct {
		s    Strat
		want bool
	}{
		{Strat{Kind: StId}, true},
		{Strat{Kind: StRules, Choice: One, Labels: []LabelID{2}}, true},
		{Strat{Kind: StRules, Choice: Dc, Labels: []LabelID{0, 1}}, true},
		{Strat{Kind: StRules, Choice: Dc, Labels: []LabelID{0, 2}}, false},
		{Strat{Kind: StRules, Choice: Dk, Labels: []LabelID{0}}, true},
		{Strat{Kind: StRules, Choice: Dk, Labels: []LabelID{1}}, false},
		{Strat{Kind: StStrats, Choice: Dk, Alts: [][]Strat{{fail}, {fail}}}, false},
		{Strat{Kind: StStrats, Choice: Dk, Alts: [][]Strat{{{Kind: StCall, Call: 0}}}}, true},
		{Strat{Kind: StStrats, Choice: Dc, Alts: [][]Strat{{fail}, {{Kind: StCall, Call: 1}}}}, false},
		{Strat{Kind: StStrats, Choice: One, Alts: [][]Strat{{{Kind: StCall, Call: 1}}}}, true},
		{Strat{Kind: StRepeat, Body: []Strat{{Kind: StCall, Call: 1}}}, false},
		{Strat{Kind: StRepeat, Body: []Strat{{Kind: StCall, Call: 0}}}, true},
		{Strat{Kind: StIterate, Body: []Strat{{Kind: StCall, Call: 0}}}, false},
		{Strat{Kind: StIterate, Body: []Strat{{Kind: StCall, Call: 0}, fail}}, true},
		// traversals: tall as its body; tone, tsome, rewrite several
		// results; normin, normout one
		{Strat{Kind: StTAll, Body: []Strat{{Kind: StCall, Call: 0}}}, true},
		{Strat{Kind: StTAll, Body: []Strat{{Kind: StCall, Call: 1}}}, false},
		{Strat{Kind: StTOne, Body: []Strat{{Kind: StCall, Call: 0}}}, false},
		{Strat{Kind: StTSome, Body: []Strat{{Kind: StCall, Call: 0}}}, false},
		{Strat{Kind: StRewrite, Body: []Strat{{Kind: StCall, Call: 0}}}, false},
		{Strat{Kind: StNormIn, Labels: []LabelID{2}}, true},
		{Strat{Kind: StNormOut, Labels: []LabelID{2}}, true},
	} {
		if got := StrategyDet(&c.s, rulesDet, nRules, stratDet); got != c.want {
			t.Errorf("%+v: %v", c.s, got)
		}
	}
}

// A labelled rule whose left-hand side has an operator that no term of the
// program contains is not compiled, and what only it uses is not reached
// (strat[X]: the [TONE] rule and its traversal); it is compiled when the
// operator is built.
func TestLabelledRulesNeedTheirOperators(t *testing.T) {
	p := peano()
	p.op(sNat, 321, "a", nil, 0, 0)
	p.op(sNat, 322, "b", []uint32{sNat}, 0, 0)
	// [r7] b(x) => x where y := (st4) x     [r7] x => s(x)
	// st3 = dk(r7)    st4 = tall(id)        start: (st3) a
	p.labelled(7, 2, f(322, v(0, sNat)), v(0, sNat),
		ref.Assign{Lhs: v(1, sNat), Strategy: ref.Some(4), Rhs: v(0, sNat)})
	p.labelled(7, 1, v(0, sNat), f(319, v(0, sNat)))
	p.strategy(3, rules(ref.SDkRules, 7))
	p.strategy(4, ref.StrategyExpr{Op: ref.STAll, Body: ref.StrategySeq{id}})
	p.p.Query.Strategy = ref.Some(3)
	// (a query of sort set: it cannot build b)
	p.p.Query.Sort = sSet
	p.start(f(321), sNat)
	ir := p.lower(t)
	if len(ir.Labels[0].Rules) != 1 || !strings.Contains(ir.Labels[0].Skipped[0], "never applies") {
		t.Fatalf("%+v", ir.Labels[0])
	}
	for _, o := range ir.Ops {
		if o.Name == "b" {
			t.Fatal("b reached")
		}
	}
	if len(ir.Strategies) != 1 {
		t.Fatal(strategyNames(ir))
	}
	// b built by the start term: the rule and the traversal are reached
	p.start(f(322, f(321)), sNat)
	ir = p.lower(t)
	if len(ir.Labels[0].Rules) != 2 || opNamed(t, ir, "b").Kind != Constructor {
		t.Fatalf("%+v", ir.Labels[0])
	}
	if len(ir.Strategies) != 2 || ir.Strategies[1].Body[0].Kind != StTAll {
		t.Fatal(strategyNames(ir))
	}
}

// Traversals (S7d task 6): their bodies and labels are lowered and
// reached; normin/normout give one result, tall as many as its body,
// tone, tsome and rewrite several.
func TestTraversalsAreLowered(t *testing.T) {
	p := peano()
	p.op(sNat, 322, "b", []uint32{sNat, sNat}, 0, 0)
	// [r7] s(x) => x    [r8] o => s(o)
	p.labelled(7, 1, f(319, v(0, sNat)), v(0, sNat))
	p.labelled(8, 0, f(318), f(319, f(318)))
	// st3 = DK(tall(dk(r7)), tone(st4), tsome(dk(r8)), rewrite(st4),
	//          normin(r7, r8), normout(r8))
	// st4 = one(r7)
	p.strategy(3, strats(ref.SDkStrats,
		ref.StrategySeq{{Op: ref.STAll, Body: ref.StrategySeq{rules(ref.SDkRules, 7)}}},
		ref.StrategySeq{{Op: ref.STOne, Body: ref.StrategySeq{call(4)}}},
		ref.StrategySeq{{Op: ref.STSome, Body: ref.StrategySeq{rules(ref.SDkRules, 8)}}},
		ref.StrategySeq{{Op: ref.SRewrite, Body: ref.StrategySeq{call(4)}}},
		ref.StrategySeq{rules(ref.SNormIn, 7, 8)},
		ref.StrategySeq{rules(ref.SNormOut, 8)}))
	p.strategy(4, rules(ref.SOneRules, 7))
	p.p.Query.Strategy = ref.Some(3)
	p.start(f(322, f(318), f(318)), sNat)
	ir := p.lower(t)
	if got := strategyNames(ir); !reflect.DeepEqual(got, []string{"st3:nat/m", "st4:nat/m"}) {
		t.Fatal(got)
	}
	if len(ir.Labels) != 2 {
		t.Fatalf("%d labels", len(ir.Labels))
	}
	want := "DK(tall(dk(r7)), tone(st4), tsome(dk(r8)), rewrite(st4), normin(r7, r8), normout(r8))"
	if got := ir.SeqText(ir.Strategies[0].Body); got != want {
		t.Fatalf("%s, want %s", got, want)
	}
	alts := ir.Strategies[0].Body[0].Alts
	kinds := []StratKind{StTAll, StTOne, StTSome, StRewrite, StNormIn, StNormOut}
	for i, k := range kinds {
		if alts[i][0].Kind != k || !k.IsTraversal() {
			t.Fatalf("alternative %d: %v", i, alts[i][0].Kind)
		}
	}
	if len(alts[4][0].Labels) != 2 || len(alts[0][0].Body) != 1 {
		t.Fatalf("%+v", alts)
	}
}

// An operator whose rules call a strategy that builds it again is on a
// cycle (through the strategy and the label): it guards its depth.
func TestRecursionThroughStrategies(t *testing.T) {
	for _, cycle := range []bool{false, true} {
		p := peano()
		p.op(sNat, 330, "h", []uint32{sNat}, 0, 0)
		// [] h(x) => y where y := (st3) x     st3 = dk(r7)
		// [r7] x => h(x) (cycle) or s(x)
		p.rule(sNat, 2, f(330, v(0, sNat)), v(1, sNat),
			ref.Assign{Lhs: v(1, sNat), Strategy: ref.Some(3), Rhs: v(0, sNat)})
		rhs := f(319, v(0, sNat))
		if cycle {
			rhs = f(330, v(0, sNat))
		}
		p.labelled(7, 1, v(0, sNat), rhs)
		p.strategy(3, rules(ref.SDkRules, 7))
		p.start(f(330, f(318)), sNat)
		ir := p.lower(t)
		if h := opNamed(t, ir, "h"); h.Recursive != cycle || h.Total {
			t.Fatalf("cycle %v: recursive %v, total %v", cycle, h.Recursive, h.Total)
		}
	}
}

// A strategy of another sort.
func TestStrategyRefusals(t *testing.T) {
	// start with (S) of another sort
	p := peano()
	p.strategy(3, id)
	p.p.Strategies[0].Sort = sSet
	p.p.Query.Strategy = ref.Some(3)
	if _, err := Lower(&p.p, "test"); err == nil || !strings.Contains(err.Error(), "start with") {
		t.Fatal(err)
	}
}
