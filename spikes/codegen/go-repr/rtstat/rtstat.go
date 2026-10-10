// Package rtstat holds what both runtimes (rtg: generic nodes, rtt: typed
// nodes) share: the step counter, the AC order mode, the interning counters
// and the end-of-run report (ELAN_STATS=1, on stderr).
package rtstat

import (
	"fmt"
	"os"
	"runtime"
	"runtime/debug"
	"runtime/pprof"
	"sync/atomic"
	"time"
)

// Steps counts rule applications (rewrite_step).
var Steps int64

// HashOrder selects the AC order key: false (default) = creation id, as the
// first spike; true (ELAN_ACORD=hash) = structural hash, history-independent.
var HashOrder = os.Getenv("ELAN_ACORD") == "hash"

// Interning counters (all tables).
var (
	Created  int64 // nodes created
	Hits     int64 // lookups that found an existing node
	Rebuilds int64 // table rebuilds (grow or purge)
	Purged   int64 // dead weak entries dropped by rebuilds
	Live     func() int
	Slots    func() int // total table capacity (slots)
)

var start = time.Now()

// ELAN_CPUPROFILE=file writes a CPU profile of the run (stopped in Report).
func init() {
	if f := os.Getenv("ELAN_CPUPROFILE"); f != "" {
		w, err := os.Create(f)
		if err == nil {
			pprof.StartCPUProfile(w)
		}
	}
}

// Report prints the counters and the GC statistics when ELAN_STATS is set.
func Report() {
	pprof.StopCPUProfile()
	if os.Getenv("ELAN_STATS") == "" {
		return
	}
	var ms runtime.MemStats
	runtime.ReadMemStats(&ms)
	var gs debug.GCStats
	gs.PauseQuantiles = make([]time.Duration, 5)
	debug.ReadGCStats(&gs)
	live, liveGC, slots := -1, -1, -1
	if Slots != nil {
		slots = Slots()
	}
	if Live != nil {
		live = Live()
		runtime.GC()
		liveGC = Live()
	}
	fmt.Fprintf(os.Stderr,
		"stats: time %.3fs created %d hits %d table-entries %d (after a GC: %d) slots %d rebuilds %d purged %d\n"+
			"stats: mallocs %d total-alloc %.1fMB heap-sys %.1fMB stack-sys %.1fMB\n"+
			"stats: gc %d (forced by table growth: %d) pause-total %v pause-max %v\n",
		time.Since(start).Seconds(), Created, Hits, live, liveGC, slots, Rebuilds, Purged,
		ms.Mallocs, float64(ms.TotalAlloc)/1e6, float64(ms.HeapSys)/1e6, float64(ms.StackSys)/1e6,
		gs.NumGC, ForcedGCs, gs.PauseTotal, gs.PauseQuantiles[4])
}

// GCCycles counts completed GC cycles (a sentinel object re-armed by a
// cleanup each time it is collected). The weak tables use it to tell
// "the table is full of live nodes" from "dead nodes not yet collected".
var GCCycles atomic.Int64

type sentinel struct{ _ [64]byte }

func arm() {
	runtime.AddCleanup(new(sentinel), func(int) { GCCycles.Add(1); arm() }, 0)
}

func init() { arm() }

// GrowPolicy: when a weak table would grow and no GC cycle completed since
// its last rebuild, run one (ELAN_TABLEGC=0 disables, to measure).
var GrowGC = os.Getenv("ELAN_TABLEGC") != "0"

// ForcedGCs counts the collections triggered by table growth.
var ForcedGCs int64

// MaybeGC is called by a weak table about to grow from cap to newCap slots
// at its rebuild number lastGC (GCCycles value at its previous rebuild): it
// runs a collection if none completed since, and reports whether it did
// (the caller then recounts the live entries).
func MaybeGC(lastGC int64) bool {
	if !GrowGC || GCCycles.Load() != lastGC {
		return false
	}
	runtime.GC()
	ForcedGCs++
	return true
}
