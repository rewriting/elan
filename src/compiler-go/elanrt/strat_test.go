package elanrt

import (
	"slices"
	"testing"
)

// The rules of elanc-rs's elan-runtime/tests/strat.rs, checked with the
// interpreter (elan -b):
//
//	[a] n => n*10   if n < 100
//	[b] n => n*10+1 if n < 100
//	[c] n => n      if n > 100
//
// start term 1: iterate*(dk(a,b)) = 1 10 100 101 11 110 111;
// repeat*(dk(a,b)) = repeat+(dk(a,b)) = dk(a,b);dk(a,b) = 100 101 110 111;
// iterate+(dk(a,b)) = 10 100 101 11 110 111; first(a,b) = 10;
// first(c,b) = 11; dc one(dk(a,b)) = first one(dk(a,b)) = 10.
func ruleA(t int32, k K[int32]) bool { return t < 100 && k(t*10) }
func ruleB(t int32, k K[int32]) bool { return t < 100 && k(t*10+1) }
func ruleC(t int32, k K[int32]) bool { return t > 100 && k(t) }
func dkAB(t int32, k K[int32]) bool  { return Dk(t, k, ruleA, ruleB) }

func results(s Strat[int32]) []int32 { return AllResults(1, s) }

func TestOrdersOfResultsMatchTheInterpreter(t *testing.T) {
	for _, c := range []struct {
		name string
		s    Strat[int32]
		want []int32
	}{
		{"iterate*", func(t int32, k K[int32]) bool { return IterateStar(t, k, dkAB) }, []int32{1, 10, 100, 101, 11, 110, 111}},
		{"iterate+", func(t int32, k K[int32]) bool { return IteratePlus(t, k, dkAB) }, []int32{10, 100, 101, 11, 110, 111}},
		{"repeat*", func(t int32, k K[int32]) bool { return RepeatStar(t, k, dkAB) }, []int32{100, 101, 110, 111}},
		{"repeat+", func(t int32, k K[int32]) bool { return RepeatPlus(t, k, dkAB) }, []int32{100, 101, 110, 111}},
		{"seq", func(t int32, k K[int32]) bool { return Seq(t, k, dkAB, dkAB) }, []int32{100, 101, 110, 111}},
		{"dk", dkAB, []int32{10, 11}},
		{"first(a,b)", func(t int32, k K[int32]) bool { return First(t, k, ruleA, ruleB) }, []int32{10}},
		{"first(c,b)", func(t int32, k K[int32]) bool { return First(t, k, ruleC, ruleB) }, []int32{11}},
		{"first(c,dk)", func(t int32, k K[int32]) bool { return First(t, k, ruleC, dkAB) }, []int32{10, 11}},
		{"first one", func(t int32, k K[int32]) bool { return FirstOne(t, k, ruleC, dkAB) }, []int32{10}},
		{"one", func(t int32, k K[int32]) bool { return One(t, k, dkAB) }, []int32{10}},
		{"id", Id[int32], []int32{1}},
		{"fail", Fail[int32], nil},
		{"repeat* fails at once", func(t int32, k K[int32]) bool { return RepeatStar(t, k, ruleC) }, []int32{1}},
		{"repeat+ fails at once", func(t int32, k K[int32]) bool { return RepeatPlus(t, k, ruleC) }, nil},
		{"iterate+ fails at once", func(t int32, k K[int32]) bool { return IteratePlus(t, k, ruleC) }, nil},
		{"first(c,fail)", func(t int32, k K[int32]) bool { return First(t, k, ruleC, Fail[int32]) }, nil},
	} {
		if got := results(c.s); !slices.Equal(got, c.want) {
			t.Errorf("%s: %v, want %v", c.name, got, c.want)
		}
	}
	if Depth() != 0 {
		t.Fatal("guard depth not back to 0")
	}
}

func TestStopEndsTheSearch(t *testing.T) {
	var seen []int32
	stopped := IterateStar(1, func(r int32) bool {
		seen = append(seen, r)
		return len(seen) == 3
	}, dkAB)
	if !stopped || !slices.Equal(seen, []int32{1, 10, 100}) {
		t.Fatalf("iterate* stopped %v after %v", stopped, seen)
	}
	never := func(int32) bool { return false }
	always := func(int32) bool { return true }
	if dkAB(1, never) {
		t.Error("a strategy that delivers all its results returns false")
	}
	if !Dk(1, always, ruleA, ruleB) || !Seq(1, func(r int32) bool { return r == 101 }, dkAB, dkAB) ||
		!First(1, always, ruleC, ruleB) {
		t.Error("stop through dk, seq, first")
	}
	n := 0
	if !RepeatStar(1, func(int32) bool { n++; return n == 2 }, dkAB) || n != 2 {
		t.Error("stop through repeat*")
	}
	if r, ok := FirstResult(1, dkAB); !ok || r != 10 {
		t.Error("FirstResult")
	}
	if _, ok := FirstResult(1, ruleC); ok {
		t.Error("FirstResult of a failure")
	}
	if Depth() != 0 {
		t.Fatal("guard depth not back to 0")
	}
}

func TestDeterministicLoops(t *testing.T) {
	// repeat*(first one(a)) from 1: 1 -> 10 -> 100
	if r := RepeatStarDet(1, DetStep(ruleA)); r != 100 {
		t.Errorf("repeat* det = %d", r)
	}
	// a long deterministic loop does not recurse
	inc := func(t int32) (int32, bool) { return t + 1, t < 10_000_000 }
	if r := RepeatStarDet(0, inc); r != 10_000_000 {
		t.Errorf("long loop = %d", r)
	}
	var seen []int32
	IterateStarDet(1, func(r int32) bool { seen = append(seen, r); return false }, DetStep(ruleA))
	if !slices.Equal(seen, []int32{1, 10, 100}) {
		t.Errorf("iterate* det = %v", seen)
	}
}

// Strategies over terms: the same combinators with T = Term.
func TestStrategiesOverTerms(t *testing.T) {
	pred := func(t Term, k K[Term]) bool { return t.Sym == symS && k(t.Args[0]) }
	r := AllResults(nat(5), func(t Term, k K[Term]) bool { return RepeatStar(t, k, pred) })
	if len(r) != 1 || r[0] != nat(0) {
		t.Fatal("repeat*(pred)")
	}
	if n := len(AllResults(nat(5), func(t Term, k K[Term]) bool { return IterateStar(t, k, pred) })); n != 6 {
		t.Fatalf("iterate*(pred): %d results", n)
	}
}

// The choices of the generated code (OneChoice, DcChoice) give the
// results of One and First, and stop as they do.
func TestChoicesOfTheGeneratedCode(t *testing.T) {
	dc := func(t int32, k K[int32], ss ...Strat[int32]) bool {
		d := NewDc(k)
		stop := false
		for _, s := range ss {
			if stop = s(t, d.K); stop || d.Found {
				break
			}
		}
		return stop
	}
	one := func(t int32, k K[int32], ss ...Strat[int32]) bool {
		o := NewOne(k)
		stop := false
		for _, s := range ss {
			if stop = s(t, o.K); stop {
				break
			}
		}
		return o.Pass(stop)
	}
	for _, c := range []struct {
		name string
		s    Strat[int32]
		want []int32
	}{
		{"dc(c,b)", func(t int32, k K[int32]) bool { return dc(t, k, ruleC, ruleB) }, []int32{11}},
		{"dc(c,dk)", func(t int32, k K[int32]) bool { return dc(t, k, ruleC, dkAB) }, []int32{10, 11}},
		{"dc(c,c)", func(t int32, k K[int32]) bool { return dc(t, k, ruleC, ruleC) }, nil},
		{"one(c,dk)", func(t int32, k K[int32]) bool { return one(t, k, ruleC, dkAB) }, []int32{10}},
		{"one(c)", func(t int32, k K[int32]) bool { return one(t, k, ruleC) }, nil},
	} {
		if got := results(c.s); !slices.Equal(got, c.want) {
			t.Errorf("%s: %v, want %v", c.name, got, c.want)
		}
	}
	// a stop without a result (a tail call) is passed on by Pass
	o := NewOne(func(int32) bool { t.Fatal("no result expected"); return false })
	if !o.Pass(true) || o.Pass(false) {
		t.Fatal("Pass")
	}
	// a state reset (each turn of a tail-call loop, S7e task 4) is a new
	// state: no result kept, nothing found
	var got []int32
	o = NewOne(func(r int32) bool { got = append(got, r); return false })
	d := NewDc(func(r int32) bool { return false })
	o.K(7)
	d.K(7)
	o.Reset()
	d.Reset()
	if o.Pass(false) || len(got) != 0 || d.Found {
		t.Fatalf("reset: %v %v", got, d.Found)
	}
	o.K(8)
	if o.Pass(false); !slices.Equal(got, []int32{8}) {
		t.Fatalf("after a reset: %v", got)
	}
	// the step of a deterministic strategy: its first result, each time
	step := DetStep(dkAB)
	for _, c := range []struct{ in, out int32 }{{1, 10}, {10, 100}, {100, 0}} {
		r, ok := step(c.in)
		if ok != (c.out != 0) || ok && r != c.out {
			t.Fatalf("step(%d) = %d, %v", c.in, r, ok)
		}
	}
}
