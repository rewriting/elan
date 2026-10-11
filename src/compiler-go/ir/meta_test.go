package ir

// Unit tests of the strategy terms of the IR (S7b task 2), as elanc-rs's
// tests/ir.rs: operators with a default strategy, call "name":X (the
// sort-aware lookup of meta.cc) and META.

import (
	"reflect"
	"strings"
	"testing"

	"elan/ref"
)

const (
	sStrategy = 500 // Strategy[nat]
	sStrateg  = 501 // Strateg[nat]
	sList     = 502 // list[nat]
)

// metaProg is peano with set_of over nat (Meta_apply[nat], Meta_strat[nat])
// and two strategies named two, on nat and on set; the start term is
// set_of(call "two":nat, o), the call holding the index callIndex (the
// export writes the index of a strategy of that name, any sort):
//
//	[r7] x => s(x)
//	two:nat/m (3) = dk(r7)     two:set/m (4) = id
//	[] set_of(s, t) => res where res := (WHERE0) meta_apply(s, t.nil, 0, 0)
//	WHERE0:list[nat]/m (9) = META
func metaProg(callIndex int64) *prog {
	p := peano()
	p.p.Sorts = append(p.p.Sorts,
		ref.Sort{Index: sStrategy, Name: "Strategy[nat]"},
		ref.Sort{Index: sStrateg, Name: "Strateg[nat]"},
		ref.Sort{Index: sList, Name: "list[nat]"})
	p.op(sStrategy, 141, "", []uint32{sStrateg}, 141, 0)
	p.op(sStrateg, 144, "call", []uint32{sStr}, 144, 0)
	p.op(sStrateg, 143, "id", nil, 143, 0)
	p.op(sList, 330, "nil", nil, 0, 0)
	p.op(sList, 331, "cons", []uint32{sNat, sList}, 0, 0)
	p.op(sList, 128, "meta_apply", []uint32{sStrategy, sList, sInt, sInt}, -128, 0)
	p.op(sList, 332, "set_of", []uint32{sStrategy, sNat}, 0, 0)
	p.rule(sList, 3, f(332, v(0, sStrategy), v(1, sNat)), v(2, sList),
		ref.Assign{Lhs: v(2, sList), Strategy: ref.Some(9),
			Rhs: f(128, v(0, sStrategy), f(331, v(1, sNat), f(330)), ref.Int{Value: 0}, ref.Int{Value: 0})})
	p.labelled(7, 1, v(0, sNat), f(319, v(0, sNat)))
	p.p.Strategies = append(p.p.Strategies,
		ref.StrategyDef{Name: 3, Sort: sNat, Module: 1, Body: ref.StrategySeq{rules(ref.SDkRules, 7)}},
		ref.StrategyDef{Name: 4, Sort: sSet, Module: 1, Body: ref.StrategySeq{id}},
		ref.StrategyDef{Name: 9, Sort: sList, Module: 1, Body: ref.StrategySeq{{Op: ref.SMeta}}})
	p.p.StrategyNames = append(p.p.StrategyNames,
		ref.TableEntry{Index: 3, Name: "two:nat/m"},
		ref.TableEntry{Index: 4, Name: "two:set/m"},
		ref.TableEntry{Index: 9, Name: "WHERE0:list[nat]/m"})
	p.start(f(332, f(141, f(144, ref.Int{Value: callIndex})), f(318)), sList)
	return p
}

func sortID(t *testing.T, ir *IR, name string) SortID {
	t.Helper()
	s, _ := sortNamed(t, ir, name)
	return s
}

func strategyNamed(t *testing.T, ir *IR, name string) StratID {
	t.Helper()
	for i := range ir.Strategies {
		if ir.Strategies[i].Name == name {
			return i
		}
	}
	t.Fatalf("no strategy %s in %v", name, strategyNames(ir))
	return -1
}

// META: a function per sort it is applied to, an evaluator per
// Strategy[X] with the calls by name, meta_apply evaluated by META and a
// constructor elsewhere.
func TestMetaEvaluators(t *testing.T) {
	ir := metaProg(3).lower(t)
	list := sortID(t, ir, "list[nat]")
	if want := []MetaApply{{Fun: "meta_list_nat", Sort: list}}; !reflect.DeepEqual(ir.MetaApplies, want) {
		t.Fatalf("%+v", ir.MetaApplies)
	}
	where := &ir.Strategies[strategyNamed(t, ir, "WHERE0:list[nat]/m")]
	if !reflect.DeepEqual(where.Body, []Strat{{Kind: StMeta, Meta: 0}}) || where.Det {
		t.Fatalf("%+v", where)
	}
	if got := ir.SeqText(where.Body); got != "META" {
		t.Fatal(got)
	}
	want := []MetaEval{{
		Fun: "ev_Strategy_nat", StrategFun: "ev_Strategy_nat_step", ListFun: "ev_Strategy_nat_list",
		Strategy: sortID(t, ir, "Strategy[nat]"), Strateg: sortID(t, ir, "Strateg[nat]"),
		Strategies: -1, Term: sortID(t, ir, "nat"),
		Calls: []MetaCall{{Name: "two", Typed: strategyNamed(t, ir, "two:nat/m"), Untyped: -1}},
	}}
	if !reflect.DeepEqual(ir.MetaEvals, want) {
		t.Fatalf("%+v\nwant %+v", ir.MetaEvals, want)
	}
	if m := opNamed(t, ir, "meta_apply"); m.Meta != MetaList || m.Kind != Constructor || !m.Variant {
		t.Fatalf("%+v", m)
	}
	if c := opNamed(t, ir, "call"); c.Kind != Constructor || c.Meta != MetaNone {
		t.Fatalf("%+v", c)
	}
	// the call holds the string again
	if a := ir.Start.Term.Args[0].Args[0].Args[0]; a.Kind != ExprStr || a.Str != "two" {
		t.Fatalf("%+v", a)
	}
}

// call "name":X is resolved with the sort of the term (meta.cc, commit
// 93ec493), not with the index of the export (any sort, REM's 2004
// behaviour); a name that only a strategy of another sort has, or that no
// strategy has, is refused.
func TestCallLookupBySort(t *testing.T) {
	ir := metaProg(4).lower(t) // the export found two:set
	if c := ir.MetaEvals[0].Calls[0]; ir.Strategies[c.Typed].Name != "two:nat/m" {
		t.Fatal(ir.Strategies[c.Typed].Name)
	}
	if strings.Contains(strings.Join(strategyNames(ir), " "), "two:set") {
		t.Fatal(strategyNames(ir))
	}
	// the last definition of the name and sort, in table order
	p := metaProg(3)
	p.p.Strategies = append(p.p.Strategies,
		ref.StrategyDef{Name: 12, Sort: sNat, Module: 2, Body: ref.StrategySeq{id}})
	p.p.StrategyNames = append(p.p.StrategyNames, ref.TableEntry{Index: 12, Name: "two:nat/other"})
	ir = p.lower(t)
	if c := ir.MetaEvals[0].Calls[0]; ir.Strategies[c.Typed].Name != "two:nat/other" {
		t.Fatal(ir.Strategies[c.Typed].Name)
	}
	// only two:set: refused
	p = metaProg(4)
	p.p.Strategies[0].Sort = sSet
	if st, what := unsupportedStage(t, p); st != "S7d" || !strings.Contains(what, "another sort") {
		t.Fatalf("%s %s", st, what)
	}
	// no strategy named so: the export writes -1
	p = metaProg(-1)
	if st, what := unsupportedStage(t, p); st != "S7d" || !strings.Contains(what, "unknown strategy in call used") {
		t.Fatalf("%s %s", st, what)
	}
}

// A strategy term operator with a default strategy (the defstrat word,
// exported for REM only) is a constructor, and a strategy that calls
// itself through META is on a cycle: set_of guards its depth.
func TestDefaultStrategiesAndRecursionThroughMeta(t *testing.T) {
	p := metaProg(3)
	b := &p.p.Grammars.Both
	for i := range *b {
		for j := range (*b)[i].Operators {
			if o := &(*b)[i].Operators[j]; o.Symbol == 331 {
				o.Defstrat = ref.DefStrat{Kind: ref.DSFsym, Hi: 331, Lo: 331}
			}
		}
	}
	ir := p.lower(t)
	if c := opNamed(t, ir, "cons"); c.Kind != Constructor {
		t.Fatalf("%+v", c)
	}
	if opNamed(t, ir, "set_of").Recursive {
		t.Fatal("set_of: no cycle")
	}
	// [r7] x => s(x) where l := () set_of(call "two":nat, x)
	p = metaProg(3)
	p.p.Rules[len(p.p.Rules)-1].Nvars = 2
	p.p.Rules[len(p.p.Rules)-1].Body = ref.Plain{Rhs: f(319, v(0, sNat)), Wheres: []ref.Where{
		ref.Assign{Lhs: v(1, sList), Rhs: f(332, f(141, f(144, ref.Int{Value: 3})), v(0, sNat))}}}
	if !opNamed(t, p.lower(t), "set_of").Recursive {
		t.Fatal("set_of: a cycle through META")
	}
}
