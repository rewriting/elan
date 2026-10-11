package elanrt

import (
	"runtime"
	"runtime/debug"
	"runtime/metrics"
	"sync/atomic"
)

// GC pacing (when GOGC is not set). The live heap of a program is often
// tiny (the weak unique table keeps nothing alive: propc 3 has ~2 MB live
// and allocates ~3 GB); with GOGC = [GCPercent] the heap goal is then
// Go's minimum, 4 MiB x GCPercent/100 = 16 MiB, and the GC runs hundreds
// of cycles, each one freeing most of the terms, rebuilt later (lost
// sharing). After each cycle the tuner sets the percentage so that the
// goal is about max([MinHeap], live x (1 + GCPercent/100)): a larger
// percentage while the live heap is small (at most 100 x MinHeap / 4 MiB,
// so that Go's minimum is MinHeap), GCPercent from MinHeap / 5 of live
// heap on. Memory stays bounded: MinHeap, or 5 x the live heap.
//
// Measured (propc 3, Apple M4 Max, best of 2): GCPercent alone: 172
// cycles, 0.66 s, 31 MB; MinHeap 24 MiB: 110 cycles, 0.61 s, 40 MB;
// 32 MiB: 81 cycles, 0.58 s, 48 MB; 48 MiB: 53, 0.56 s, 63 MB; 64 MiB:
// 39, 0.55 s, 80 MB; 128 MiB: 0.57 s, 146 MB.

// MinHeap is the heap goal of the GC while the live heap is small.
const MinHeap = 32 << 20

var (
	gcPercent   = GCPercent // set by the tuner (one goroutine at a time)
	tunerSample = []metrics.Sample{{Name: "/gc/heap/live:bytes"}}
	// GCTunings counts the changes of the GC percentage (ELAN_STATS).
	GCTunings atomic.Int64
)

// gcSentinel is an object whose cleanup runs after the GC cycle that
// finds it unreachable (not a tiny allocation: it has pointers).
type gcSentinel struct{ _ [2]*int }

// startGCTuner sets the percentage now and after each GC cycle.
func startGCTuner() {
	tuneGC()
	armGCTuner()
}

func armGCTuner() {
	runtime.AddCleanup(&gcSentinel{}, func(int) {
		tuneGC()
		armGCTuner()
	}, 0)
}

// percentFor is the GC percentage for a live heap of live bytes.
func percentFor(live uint64) int {
	p := GCPercent
	if live > 0 && live < MinHeap {
		p = max(p, int(100*MinHeap/live)-100)
	}
	return min(p, 100*MinHeap/(4<<20))
}

// tuneGC sets the GC percentage from the live heap (on the goroutine of
// the cleanups, after a cycle).
func tuneGC() {
	metrics.Read(tunerSample)
	p := percentFor(tunerSample[0].Value.Uint64())
	if p != gcPercent {
		gcPercent = p
		debug.SetGCPercent(p)
		GCTunings.Add(1)
	}
}
