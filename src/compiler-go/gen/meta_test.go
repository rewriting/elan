package gen

// Tests of the emission of META (S7b task 2): the shape of the code, and a
// program run.

import (
	"strings"
	"testing"

	"elan/ref"
)

const (
	sStrategy   = 510 // Strategy[nat]
	sStrateg    = 511 // Strateg[nat]
	sStrategies = 512 // Strategies[nat]
	sList       = 513 // list[nat]
)

// metaProg is peano with set_of over nat, its start term
//
//	set_of(dc(call "none":nat, dk(call "inc":nat, id)) iterate call "dec":nat enditerate, s(o))
//
// whose results are s(s(o)), s(o), o, s(o), o; set_of takes 3 of them
// from number 1: cons(s(o), cons(o, cons(s(o), nil))).
//
//	[r7] s(x) => x     [r8] x => s(x)
//	inc (3) = dk(r8)   dec (4) = dk(r7)   none (5) = fail
//	[] set_of(s, t) => res where res := (WHERE0) meta_apply(s, t.nil, 1, 3)
//	WHERE0:list[nat]/m (9) = META
func metaProg() *prog {
	p := peano()
	p.p.Sorts = append(p.p.Sorts,
		ref.Sort{Index: sStrategy, Name: "Strategy[nat]"},
		ref.Sort{Index: sStrateg, Name: "Strateg[nat]"},
		ref.Sort{Index: sStrategies, Name: "Strategies[nat]"},
		ref.Sort{Index: sList, Name: "list[nat]"})
	p.op(sStrategies, 131, "one", []uint32{sStrategy}, 131)
	p.op(sStrategies, 132, "more", []uint32{sStrategy, sStrategies}, 132)
	p.op(sStrateg, 137, "dc", []uint32{sStrategies}, 137)
	p.op(sStrateg, 138, "dk", []uint32{sStrategies}, 138)
	p.op(sStrateg, 139, "repeat", []uint32{sStrategy}, 139)
	p.op(sStrateg, 140, "iterate", []uint32{sStrategy}, 140)
	p.op(sStrategy, 141, "step", []uint32{sStrateg}, 141)
	p.op(sStrategy, 142, "seq", []uint32{sStrateg, sStrategy}, 142)
	p.op(sStrateg, 143, "id", nil, 143)
	p.op(sStrateg, 144, "call", []uint32{sStr}, 144)
	p.op(sList, 330, "nil", nil, 0)
	p.op(sList, 331, "cons", []uint32{sNat, sList}, 0)
	p.op(sList, 128, "meta_apply", []uint32{sStrategy, sList, sInt, sInt}, -128)
	p.op(sList, 332, "set_of", []uint32{sStrategy, sNat}, 0)
	p.rule(sList, f(332, v(0, sStrategy), v(1, sNat)), v(2, sList),
		ref.Assign{Lhs: v(2, sList), Strategy: ref.Some(9),
			Rhs: f(128, v(0, sStrategy), f(331, v(1, sNat), f(330)), ref.Int{Value: 1}, ref.Int{Value: 3})})
	p.labelled(7, f(319, v(0, sNat)), v(0, sNat))
	p.labelled(8, v(0, sNat), f(319, v(0, sNat)))
	p.p.Strategies = append(p.p.Strategies,
		ref.StrategyDef{Name: 3, Sort: sNat, Module: 1, Body: ref.StrategySeq{rules(ref.SDkRules, 8)}},
		ref.StrategyDef{Name: 4, Sort: sNat, Module: 1, Body: ref.StrategySeq{rules(ref.SDkRules, 7)}},
		ref.StrategyDef{Name: 5, Sort: sNat, Module: 1, Body: ref.StrategySeq{{Op: ref.SFail}}},
		ref.StrategyDef{Name: 9, Sort: sList, Module: 1, Body: ref.StrategySeq{{Op: ref.SMeta}}})
	p.p.StrategyNames = append(p.p.StrategyNames,
		ref.TableEntry{Index: 3, Name: "inc:nat/m"},
		ref.TableEntry{Index: 4, Name: "dec:nat/m"},
		ref.TableEntry{Index: 5, Name: "none:nat/m"},
		ref.TableEntry{Index: 9, Name: "WHERE0:list[nat]/m"})
	call := func(i int64) ref.Term { return f(141, f(144, ref.Int{Value: i})) }
	choice := f(137, f(132, call(5), f(131, f(141, f(138, f(132, call(3), f(131, f(141, f(143)))))))))
	s := f(142, choice, f(141, f(140, call(4))))
	p.p.Query.ResultSort = sList
	p.p.Query.StartWith = f(332, s, f(319, f(318)))
	return p
}

func TestShapeOfMeta(t *testing.T) {
	src := metaProg().gen(t)
	for _, want := range []string{
		"func meta_list_nat(t rt.Term, k rt.K[rt.Term]) bool {",
		"case sym_meta_apply_128: // meta_apply(@,@,@,@) : (Strategy[nat] list[nat] builtinInt builtinInt) list[nat]",
		"from, n := metaInt(t.Args[2]), metaInt(t.Args[3])",
		"if l.Sym == sym_cons_331 && l.Args[1].Sym == sym_nil_330 {",
		"ev_Strategy_nat(t.Args[0], l.Args[0], true, func(r rt.Term) bool {",
		"acc = cons_331(res[i], acc)",
		"func ev_Strategy_nat(s rt.Term, t rt.Term, typed bool, k rt.K[rt.Term]) bool {",
		"return ev_Strategy_nat_step(s.Args[0], t, typed, func(r rt.Term) bool { return ev_Strategy_nat(rest, r, typed, k) })",
		"switch metaStr(s.Args[0]) {\n\t\tcase \"dec\":\n\t\t\treturn st_dec_4(t, k)",
		"return rt.First(t, k, ev_Strategy_nat_list(s.Args[0], typed)...)",
		"return rt.IterateStar(t, k, func(t rt.Term, k rt.K[rt.Term]) bool { return ev_Strategy_nat(body, t, typed, k) })",
		"func st_WHERE0_9(t rt.Term, k rt.K[rt.Term]) bool {\n\trt.Guard()\n\tdefer rt.Unguard()\n\treturn meta_list_nat(t, k)\n}",
	} {
		if !strings.Contains(src, want) {
			t.Errorf("missing %q in\n%s", want, src)
		}
	}
	// repeat is not built: no case
	if strings.Contains(src, "rt.RepeatStar") {
		t.Error("repeat")
	}
}

func TestMetaRuns(t *testing.T) {
	want := "\nresult = cons(s(o),cons(o,cons(s(o),nil)))\n"
	if got := run(t, metaProg()); !strings.HasPrefix(got, want) {
		t.Errorf("got %q, want %q", got, want)
	}
}
