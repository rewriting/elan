package elanrt

import (
	"fmt"
	"os"
	"strconv"
	"strings"
	"testing"
	"time"
)

// Recursions in the shapes of the generated code, guarded at entry.

// genList is innermost normalisation: gen(n) => n . gen(n-1), gen(0) => nil.
func genList(n int32) Term {
	Guard()
	defer Unguard()
	Step()
	if n == 0 {
		return Mk0(symNil)
	}
	return Mk2(symCons, MkInt(n), genList(n-1))
}

// count is a recursion with native ints and no terms: count(n) =>
// count(n-1) + 1.
func count(n int32) int32 {
	Guard()
	defer Unguard()
	Step()
	if n == 0 {
		return 0
	}
	return count(n-1) + 1
}

// bigFrames is a recursion with 4 KiB frames.
func bigFrames(n int32) int32 {
	Guard()
	defer Unguard()
	var pad [512]int64
	pad[n%512] = int64(n)
	if n == 0 {
		return 0
	}
	return bigFrames(n-1) + int32(pad[(n+1)%512])
}

// midFrames is a recursion with frames about 2.5x those of count (the
// second phase of a run, see TestDepthGuardTwoPhases).
func midFrames(n int32) int32 {
	Guard()
	defer Unguard()
	var pad [8]int64
	pad[n%8] = int64(n)
	if n == 0 {
		return 0
	}
	return midFrames(n-1) + int32(pad[(n+1)%8])
}

// down is iterate*(dc(down)) with [down] n => n-1 if n > 0, in CPS:
// every result is a nested continuation.
func down(t int32, k K[int32]) bool { return t > 0 && k(t-1) }

func init() {
	helpers["deep"] = func() {
		n, _ := strconv.Atoi(os.Args[len(os.Args)-1])
		Main("List", func(s *Session) {
			s.ResultInt(int32(n)) // printed before the deep recursion: must be flushed
			t0 := time.Now()
			var r int32
			switch os.Getenv("ELANRT_SHAPE") {
			case "list":
				r = genList(int32(n)).Args[0].Int
			case "count":
				r = count(int32(n))
			case "big":
				r = bigFrames(int32(n))
			case "cps":
				IterateStar(int32(n), func(t int32) bool { r = t; return false }, down)
			case "twophase":
				// small frames, deep enough for the stack to reach its
				// ceiling, then frames 2.5x bigger on the same stack
				s.ResultInt(count(12_000_000))
				r = midFrames(int32(n))
			}
			s.ResultInt(r)
			fmt.Fprintf(os.Stderr, "%.2fs max depth %d\n", time.Since(t0).Seconds(), MaxDepth())
		})
	}
}

// Deep recursions stop with status 3 and the message of elanc-rs, never
// with Go's fatal stack overflow (status 2); 10^6 levels succeed.
func TestDepthGuard(t *testing.T) {
	if testing.Short() {
		t.Skip("deep recursions: seconds and GiBs of stack")
	}
	for _, c := range []struct {
		shape  string
		n      int
		status int
	}{
		{"list", 1_000_000, 0},
		{"count", 1_000_000, 0},
		{"cps", 1_000_000, 0},
		{"list", 10_000_000, 3},
		{"count", 100_000_000, 3},
		{"big", 1_000_000, 3},
		{"cps", 100_000_000, 3},
	} {
		start := time.Now()
		out, errs, st := runHelper(t, "deep", []string{"ELANRT_SHAPE=" + c.shape}, "-noInput", "-quiet", strconv.Itoa(c.n))
		t.Logf("%s %d: status %d, %.1fs; %s", c.shape, c.n, st, time.Since(start).Seconds(), strings.TrimSpace(lastLine(errs)))
		if strings.Contains(errs, "stack overflow") || strings.Contains(errs, "goroutine ") {
			t.Fatalf("%s %d: Go crash:\n%.2000s", c.shape, c.n, errs)
		}
		if st != c.status {
			t.Fatalf("%s %d: status %d, want %d; stderr:\n%.2000s", c.shape, c.n, st, c.status, errs)
		}
		first := fmt.Sprintf("\nresult = %d\n", c.n)
		if !strings.HasPrefix(out, first) {
			t.Fatalf("%s %d: output %q: the first result is lost", c.shape, c.n, out)
		}
		if st == 3 && !strings.HasPrefix(errs, "elan: recursion too deep (stack of 1024 MiB exhausted); the program stops (exit status 3).") {
			t.Fatalf("%s %d: message %q", c.shape, c.n, errs)
		}
	}
}

// A first descent with small frames takes the stack to its ceiling; a
// second one with bigger frames on the same stack must be measured on its
// own frames (the limit of the first would let Go's stack overflow).
func TestDepthGuardTwoPhases(t *testing.T) {
	if testing.Short() {
		t.Skip("deep recursions: seconds and GiBs of stack")
	}
	for _, c := range []struct {
		n      int
		status int
	}{
		{1_000_000, 0},
		{12_000_000, 3},
	} {
		out, errs, st := runHelper(t, "deep", []string{"ELANRT_SHAPE=twophase", "ELAN_STATS=1"}, "-noInput", "-quiet", strconv.Itoa(c.n))
		t.Logf("twophase %d: status %d; %s", c.n, st, strings.TrimSpace(errs))
		if strings.Contains(errs, "stack overflow") || strings.Contains(errs, "goroutine ") {
			t.Fatalf("twophase %d: Go crash:\n%.2000s", c.n, errs)
		}
		if st != c.status {
			t.Fatalf("twophase %d: status %d, want %d; stderr:\n%.2000s", c.n, st, c.status, errs)
		}
		want := fmt.Sprintf("\nresult = %d\n\nresult = 12000000\n", c.n)
		if !strings.HasPrefix(out, want) {
			t.Fatalf("twophase %d: output %q, want the prefix %q", c.n, out, want)
		}
		if st == 3 && !strings.HasPrefix(errs, "elan: recursion too deep (stack of 1024 MiB exhausted)") {
			t.Fatalf("twophase %d: message %q", c.n, errs)
		}
	}
}

func lastLine(s string) string {
	s = strings.TrimSpace(s)
	return s[strings.LastIndex(s, "\n")+1:]
}

// The guard's counters go back to their state after a recursion.
func TestGuardCounts(t *testing.T) {
	d := Depth()
	if r := count(100_000); r != 100_000 || Depth() != d {
		t.Fatalf("count = %d, depth %d", r, Depth())
	}
	if TooDeepMessage(1 << 30)[:46] != "elan: recursion too deep (stack of 1024 MiB ex" {
		t.Fatal(TooDeepMessage(1 << 30))
	}
}
