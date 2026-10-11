package gen

// Tests of the emission of labels and strategies (S7b): the shape of the
// code, and a program run.

import (
	"fmt"
	"strings"
	"testing"

	"elan/ref"
)

func (p *prog) strategy(name uint32, body ...ref.StrategyExpr) {
	p.p.Strategies = append(p.p.Strategies, ref.StrategyDef{Name: name, Sort: sNat, Module: 1, Body: body})
	p.p.StrategyNames = append(p.p.StrategyNames, ref.TableEntry{Index: name, Name: fmt.Sprintf("st%d:nat/m", name)})
}

func (p *prog) labelled(label uint32, lhs, rhs ref.Term, wheres ...ref.Where) {
	p.rule(sNat, lhs, rhs, wheres...)
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
func body(op ref.StrategyOp, b ...ref.StrategyExpr) ref.StrategyExpr {
	return ref.StrategyExpr{Op: op, Body: b}
}

// strategies is peano with labels and strategies, the start term
// s(s(o)) and the strategy st15:
//
//	[r7] s(x) => x    [r8] x => s(x)    [r8] s(x) => x
//	[r9] o => s(o)    [r9] o => s(s(o))
//	[] h(x) => y where y := (st13) x      (normalisation: the first result)
//	st3 = DC(dc(r7) ; st3, id)            o (a loop)
//	st10 = repeat*(dc(r7)) ; dk(r9)       s(o), s(s(o))
//	st11 = iterate*(dc(r7))               s(s(o)), s(o), o
//	st12 = ONE(dk(r8))                    s(s(s(o)))
//	st13 = dc(r9, r8)                     s(s(s(o))) (the first rule of r8)
//	st14 = st10 ; one(r8)                 s(s(o)), s(s(s(o)))
//	st16 = dk(r8) ; fail ; id             none (dk(r8) runs)
//	st17 = id ; dc(r7) ; id               s(o)
//	st18 = DC(fail ; st18, id)            s(s(o)) (a tail call never reached)
//	st19 = DK(fail, fail) ; dc(r7)        none
//	st15 = DK(st3, st10, st11, st12, st13, st14, st16, st17, st18, st19)
//	       ; DC(fail, id)
func strategies() *prog {
	p := peano()
	p.op(sNat, 330, "h", []uint32{sNat}, 0)
	p.labelled(7, f(319, v(0, sNat)), v(0, sNat))
	p.labelled(8, v(0, sNat), f(319, v(0, sNat)))
	p.labelled(8, f(319, v(0, sNat)), v(0, sNat))
	p.labelled(9, f(318), f(319, f(318)))
	p.labelled(9, f(318), f(319, f(319, f(318))))
	p.rule(sNat, f(330, v(0, sNat)), v(1, sNat),
		ref.Assign{Lhs: v(1, sNat), Strategy: ref.Some(13), Rhs: v(0, sNat)})
	id := ref.StrategyExpr{Op: ref.SId}
	fail := ref.StrategyExpr{Op: ref.SFail}
	dc7 := rules(ref.SDcRules, 7)
	p.strategy(3, strats(ref.SDcStrats, ref.StrategySeq{dc7, call(3)}, ref.StrategySeq{id}))
	p.strategy(10, body(ref.SRepeat, dc7), rules(ref.SDkRules, 9))
	p.strategy(11, body(ref.SIterate, dc7))
	p.strategy(12, strats(ref.SOneStrats, ref.StrategySeq{rules(ref.SDkRules, 8)}))
	p.strategy(13, rules(ref.SDcRules, 9, 8))
	p.strategy(14, call(10), rules(ref.SOneRules, 8))
	p.strategy(16, rules(ref.SDkRules, 8), fail, id)
	p.strategy(17, id, dc7, id)
	p.strategy(18, strats(ref.SDcStrats, ref.StrategySeq{fail, call(18)}, ref.StrategySeq{id}))
	p.strategy(19, strats(ref.SDkStrats, ref.StrategySeq{fail}, ref.StrategySeq{fail}), dc7)
	var alts []ref.StrategySeq
	for _, c := range []uint32{3, 10, 11, 12, 13, 14, 16, 17, 18, 19} {
		alts = append(alts, ref.StrategySeq{call(c)})
	}
	p.strategy(15, strats(ref.SDkStrats, alts...), strats(ref.SDcStrats, ref.StrategySeq{fail}, ref.StrategySeq{id}))
	p.p.Query.Strategy = ref.Some(15)
	p.p.Query.StartWith = f(319, f(319, f(318)))
	return p
}

func TestShapeOfStrategies(t *testing.T) {
	p := strategies()
	p.p.Query.StartWith = f(330, f(319, f(319, f(318)))) // h reached
	src := p.gen(t)
	for _, want := range []string{
		"func rl_r8_8(a0 rt.Term, k rt.K[rt.Term], first bool) bool {\n\trt.Guard()\n\tdefer rt.Unguard()\n\tfound := false",
		"//\t[r9] o => s(s(o))",
		"if first && found {",
		"// st_st3_3: strategy st3:nat/m,\n//\n//\tDC(dc(r7) ; st3, id)\n//\n// At most one result.",
		"again := func(r rt.Term) bool {",
		"return again(r",
		"rt.RepeatStarDet(t, rt.DetStep(step",
		"return rt.IterateStarDet(t, k, rt.DetStep(step",
		"one1 := rt.NewOne(k) // ONE(dk(r8)): the first result",
		"return rl_r9_9(t, dc1.K, true) || !dc1.Found && rl_r8_8(t, dc1.K, true)",
		// dk(r8) runs, its results go to fail
		"k1 := func(r1 rt.Term) bool {\n\t\treturn false\n\t}\n\treturn rl_r8_8(t, k1, false)",
		// id in a sequence is left out
		"func st_st17_17(t rt.Term, k rt.K[rt.Term]) bool {\n\trt.Guard()\n\tdefer rt.Unguard()\n\treturn rl_r7_7(t, k, true)\n}",
		// where y := (st13) x in a normalisation function: the first result
		"st_st13_13(m1, func(y1 rt.Term) bool {",
		"res1, ok1 = x1, true",
		"t := h_330(s_319(s_319(o_318())))",
		"st_st15_15(t, func(r rt.Term) bool {\n\t\t\ts.Result(r)\n\t\t\treturn false\n\t\t})",
	} {
		if !strings.Contains(src, want) {
			t.Errorf("missing %q in\n%s", want, src)
		}
	}
	if n := strings.Count(src, "// tail-recursive"); n != 1 {
		t.Errorf("%d loops", n)
	}
}

func TestStrategiesRun(t *testing.T) {
	out := run(t, strategies())
	var results []string
	for _, l := range strings.Split(out, "\n") {
		if r, ok := strings.CutPrefix(l, "result = "); ok {
			// (the printer's spacing: a space after a previous result)
			results = append(results, strings.TrimSpace(r))
		}
	}
	want := "o | s(o) | s(s(o)) | s(s(o)) | s(o) | o | s(s(s(o))) | s(s(s(o))) | " +
		"s(s(o)) | s(s(s(o))) | s(o) | s(s(o))"
	if got := strings.Join(results, " | "); got != want {
		t.Errorf("results\n%s\nwant\n%s", got, want)
	}
	// h(o): the first result of st13 on o, s(o)
	p := strategies()
	p.p.Query.Strategy = ref.None
	p.p.Query.StartWith = f(330, f(318))
	if got := run(t, p); !strings.Contains(got, "result = s(o)\n") {
		t.Errorf("h(o): %q", got)
	}
}

// commentNames has a label and a strategy named like Go identifiers of the
// generated code, which its comments quote (again, k1): a comment must not
// keep an unused tail loop or closure alive (Go refuses an unused variable).
//
//	[again] s(x) => x    (r7)          [r8] x => s(x)
//	st20 (named k1) = id
//	st21 = DC(dc(again), fail ; st21)  s(o) on s(s(o)): the tail call is never reached
//	st22 = DK(dk(r8) ; fail ; k1) ; dk(r8)   none
func commentNames(start uint32) *prog {
	p := peano()
	p.labelled(7, f(319, v(0, sNat)), v(0, sNat))
	p.labelled(8, v(0, sNat), f(319, v(0, sNat)))
	for i := range p.p.RuleNames {
		if p.p.RuleNames[i].Index == 7 {
			p.p.RuleNames[i].Name = "again:nat/m!GL"
		}
	}
	fail := ref.StrategyExpr{Op: ref.SFail}
	p.strategy(20, ref.StrategyExpr{Op: ref.SId})
	p.p.StrategyNames[len(p.p.StrategyNames)-1].Name = "k1:nat/m"
	p.strategy(21, strats(ref.SDcStrats, ref.StrategySeq{rules(ref.SDcRules, 7)}, ref.StrategySeq{fail, call(21)}))
	p.strategy(22, strats(ref.SDkStrats, ref.StrategySeq{rules(ref.SDkRules, 8), fail, call(20)}),
		rules(ref.SDkRules, 8))
	p.p.Query.Strategy = ref.Some(start)
	p.p.Query.StartWith = f(319, f(319, f(318)))
	return p
}

func TestNamesOfTheGeneratedCodeInComments(t *testing.T) {
	src := commentNames(21).gen(t)
	// (the names are in the comments)
	if want := "label again:nat/m!GL"; !strings.Contains(src, want) {
		t.Errorf("missing %q in\n%s", want, src)
	}
	if src, want := commentNames(22).gen(t), "then fail ; k1"; !strings.Contains(src, want) {
		t.Errorf("missing %q in\n%s", want, src)
	}
	if strings.Contains(src, "// tail-recursive") {
		t.Errorf("a loop for a tail call never reached:\n%s", src)
	}
	t.Run("again", func(t *testing.T) {
		if got := run(t, commentNames(21)); !strings.Contains(got, "result = s(o)\n") || strings.Count(got, "result = ") != 1 {
			t.Errorf("st21: %q", got)
		}
	})
	t.Run("k1", func(t *testing.T) {
		if got := run(t, commentNames(22)); strings.Contains(got, "result = ") {
			t.Errorf("st22: %q", got)
		}
	})
}
