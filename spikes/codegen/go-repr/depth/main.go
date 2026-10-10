// Recursion depth experiment: what Go does with deep non-tail recursion in
// the shapes the generated code has.
//
//	depth list N    innermost normalisation: generate_list(N) => N . generate_list(N-1)
//	                (generic nodes, rtg), N nested Go calls
//	depth print N   print the N-element list (recursive printer)
//	depth cps N     iterate*(dc(range_rule)) from N down to 1, all results:
//	                N nested continuations (closures calling closures)
//	depth guard N   as list, with a depth counter at function entry that
//	                stops cleanly (exit 3) above ELAN_MAXDEPTH (G4)
//
// ELAN_MAXSTACK=bytes calls debug.SetMaxStack; ELAN_GOROUTINE=1 runs the
// work on a fresh goroutine (as G4 plans) instead of the main goroutine.
package main

import (
	"fmt"
	"os"
	"runtime"
	"runtime/debug"
	"strconv"
	"time"

	rt "gorepr/rtg"
)

var (
	sym_nil  = rt.NewSym("nil", 0, false, "")
	sym_cons = rt.NewSym(".", 2, false, "@.@")
	con_nil  = rt.Mk0(sym_nil)
)

// [] generate_list(0) => nil ; [] generate_list(n) => n . generate_list(n-1)
func fun_generate_list(v1 int32) rt.Term {
	if v1 == 0 {
		rt.Step()
		return con_nil
	}
	rt.Step()
	return rt.Mk2(sym_cons, rt.MkInt(v1), fun_generate_list(v1-1))
}

var depth, maxDepth int

// the same, with the G4 depth guard
func fun_generate_list_guarded(v1 int32) rt.Term {
	depth++
	if depth > maxDepth {
		rt.Out.Flush()
		os.Stderr.WriteString("error: recursion too deep\n")
		os.Exit(3)
	}
	defer func() { depth-- }()
	if v1 == 0 {
		rt.Step()
		return con_nil
	}
	rt.Step()
	return rt.Mk2(sym_cons, rt.MkInt(v1), fun_generate_list_guarded(v1-1))
}

// [range_rule] x => x-1 if x > 1 ; dc(range_rule) ; iterate*(...)
func rule_range_rule(t int32, k func(int32) bool) bool {
	if !(t > 1) {
		return false
	}
	rt.Step()
	return k(t - 1)
}

func strat_range(t int32, k func(int32) bool) bool {
	if k(t) {
		return true
	}
	got := false
	stop := rule_range_rule(t, func(r int32) bool { got = true; return strat_range(r, k) })
	return got && stop
}

func main() {
	if len(os.Args) < 3 {
		fmt.Fprintln(os.Stderr, "usage: depth list|print|cps|guard N")
		os.Exit(2)
	}
	n, _ := strconv.Atoi(os.Args[2])
	if s := os.Getenv("ELAN_MAXSTACK"); s != "" {
		v, _ := strconv.Atoi(s)
		old := debug.SetMaxStack(v)
		fmt.Fprintf(os.Stderr, "SetMaxStack(%d), was %d\n", v, old)
	}
	maxDepth, _ = strconv.Atoi(os.Getenv("ELAN_MAXDEPTH"))
	if maxDepth == 0 {
		maxDepth = 1 << 30
	}
	work := func() {
		t0 := time.Now()
		switch os.Args[1] {
		case "list":
			l := fun_generate_list(int32(n))
			fmt.Printf("built, head %d\n", l.Args[0].Int)
		case "guard":
			l := fun_generate_list_guarded(int32(n))
			fmt.Printf("built, head %d\n", l.Args[0].Int)
		case "print":
			l := fun_generate_list(int32(n))
			s := rt.Format(l)
			fmt.Printf("printed %d bytes\n", len(s))
		case "cps":
			count := 0
			strat_range(int32(n), func(int32) bool { count++; return false })
			fmt.Printf("results %d\n", count)
		}
		var ms runtime.MemStats
		runtime.ReadMemStats(&ms)
		fmt.Printf("time %.3fs steps %d stack-sys %.0fMB\n", time.Since(t0).Seconds(), rt.Steps(), float64(ms.StackSys)/1e6)
	}
	if os.Getenv("ELAN_GOROUTINE") == "1" {
		done := make(chan struct{})
		go func() { work(); close(done) }()
		<-done
	} else {
		work()
	}
}
