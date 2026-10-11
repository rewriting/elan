package elanrt

import (
	"fmt"
	"os"
	"runtime"
	"runtime/debug"
	"testing"
	"time"
)

func TestPercentFor(t *testing.T) {
	for _, c := range []struct {
		live uint64
		want int
	}{
		{0, GCPercent},             // nothing measured yet
		{1 << 20, 800},             // capped: Go's minimum 4 MiB x 8 = MinHeap
		{4 << 20, 700},             // goal 8 x 4 MiB = MinHeap
		{MinHeap / 5, GCPercent},   // goal 5 x live = MinHeap
		{MinHeap, GCPercent},       // large: GCPercent
		{100 * MinHeap, GCPercent}, // large: GCPercent
	} {
		if got := percentFor(c.live); got != c.want {
			t.Errorf("percentFor(%d) = %d, want %d", c.live, got, c.want)
		}
	}
}

func init() {
	helpers["gctuner"] = func() {
		Main("Nat", false, func(s *Session, _ Term) {
			// GC cycles with a small live heap: the tuner raises the
			// percentage after them (a cleanup, on another goroutine)
			wait := 5 * time.Second
			if os.Getenv("GOGC") != "" {
				wait = 200 * time.Millisecond // (no tuner: nothing to wait for)
			}
			for deadline := time.Now().Add(wait); GCTunings.Load() == 0 && time.Now().Before(deadline); {
				runtime.GC()
				time.Sleep(10 * time.Millisecond)
			}
			p := debug.SetGCPercent(100)
			debug.SetGCPercent(p)
			fmt.Fprintf(os.Stderr, "percent %d tunings %d\n", p, GCTunings.Load())
		})
	}
}

// Main starts the GC tuner when GOGC is not set: after GC cycles with a
// small live heap, the percentage is raised above GCPercent (the goal
// MinHeap; capped at 800); with GOGC set, the tuner is off and the
// percentage is GOGC's.
func TestGCTuner(t *testing.T) {
	_, errs, st := runHelper(t, "gctuner", []string{"GOGC="}, "-noInput", "-quiet")
	var p, n int
	if _, err := fmt.Sscanf(errs, "percent %d tunings %d", &p, &n); err != nil || st != 0 {
		t.Fatalf("GOGC unset: status %d, err %q", st, errs)
	}
	if n == 0 || p <= GCPercent || p > 800 {
		t.Errorf("GOGC unset: percent %d after %d tunings, want (%d, 800] after some", p, n, GCPercent)
	}
	_, errs, st = runHelper(t, "gctuner", []string{"GOGC=150"}, "-noInput", "-quiet")
	if _, err := fmt.Sscanf(errs, "percent %d tunings %d", &p, &n); err != nil || st != 0 {
		t.Fatalf("GOGC=150: status %d, err %q", st, errs)
	}
	if n != 0 || p != 150 {
		t.Errorf("GOGC=150: percent %d after %d tunings, want 150 and none", p, n)
	}
}
