package elanrt

import (
	"runtime"
	"runtime/metrics"
	"weak"
)

// The unique table: open addressing, linear probing, a power-of-two number
// of slots. A slot holds a node's hash (0: empty) and a weak pointer to it:
// the table never keeps a term alive. When the garbage collector frees a
// node, its slot reads as dead (Value() == nil); a later construction of
// the same term makes a new node.
//
// Dead slots are not reused: they are dropped when the table is rebuilt,
// which happens when the used slots (live or dead) exceed half of the
// table. The new size is computed from the live entries (load <= 1/4,
// MinTableSize at least), so the table shrinks as well as grows.
//
// A weak pointer reads as live until a GC cycle has found its node
// unreachable. Before growing the table past GCBeforeGrow slots, if no GC
// cycle completed since the last rebuild, the rebuild runs one first and
// recounts: without it, a table full of dead entries grows, its size
// raises the GC's heap goal, the GC runs less often, and the table grows
// again (spike G2, NOTES.md "table sizing").
//
// Correctness: Value() returns either nil or a strong pointer, never a
// pointer to a freed node, so a lookup can never resurrect a dead term;
// two live equal terms are always the same node because a construction
// sees every live node (its slot is kept until the node is dead).

// MinTableSize is the smallest size of the table (slots).
const MinTableSize = 1 << 12

// GCBeforeGrow is the size from which growing the table first runs a GC
// (if none completed since the last rebuild).
const GCBeforeGrow = 1 << 16

type slot struct {
	hash uint64 // 0: empty
	p    weak.Pointer[Node]
}

var (
	slots      = make([]slot, MinTableSize)
	usedSlots  int    // non-empty slots (live or dead)
	gcsAtBuild uint64 // GC cycles completed at the last rebuild
	// Rebuilds and ForcedGCs count table rebuilds and the GCs they ran.
	Rebuilds, ForcedGCs int64
)

// lookup returns the live node (sym, iv, args) with hash h, or nil.
func lookup(h uint64, sym, iv int32, args []Term) *Node {
	mask := uint64(len(slots) - 1)
	for i := h & mask; slots[i].hash != 0; i = (i + 1) & mask {
		if slots[i].hash == h {
			if n := slots[i].p.Value(); n != nil && n.same(sym, iv, args) {
				return n
			}
		}
	}
	return nil
}

// insert adds a new node (not in the table).
func insert(n *Node) {
	put(slots, n.Hash, weak.Make(n))
	usedSlots++
	if 2*usedSlots > len(slots) {
		rebuild()
	}
}

func put(t []slot, h uint64, p weak.Pointer[Node]) {
	mask := uint64(len(t) - 1)
	i := h & mask
	for t[i].hash != 0 {
		i = (i + 1) & mask
	}
	t[i] = slot{h, p}
}

// sizeFor is the table size for live entries.
func sizeFor(live int) int {
	size := MinTableSize
	for size < 4*live {
		size *= 2
	}
	return size
}

func rebuild() {
	live := LiveTerms()
	size := sizeFor(live)
	if size > len(slots) && size >= GCBeforeGrow && gcCycles() == gcsAtBuild {
		runtime.GC()
		ForcedGCs++
		live = LiveTerms()
		size = sizeFor(live)
	}
	copyLive(size, live)
}

// copyLive replaces the table by one of size slots holding its live
// entries (live of them).
func copyLive(size, live int) {
	t := make([]slot, size)
	for _, s := range slots {
		if s.hash != 0 && s.p.Value() != nil {
			put(t, s.hash, s.p)
		}
	}
	slots, usedSlots = t, live
	gcsAtBuild = gcCycles()
	Rebuilds++
}

var gcSample = []metrics.Sample{{Name: "/gc/cycles/total:gc-cycles"}}

// gcCycles is the number of GC cycles completed.
func gcCycles() uint64 {
	metrics.Read(gcSample)
	return gcSample[0].Value.Uint64()
}

// LiveTerms is the number of entries of the table whose node is alive
// (O(table size); tests and statistics). A node becomes dead at the end of
// the GC cycle that finds it unreachable.
func LiveTerms() int {
	live := 0
	for _, s := range slots {
		if s.hash != 0 && s.p.Value() != nil {
			live++
		}
	}
	return live
}

// TableSize is the number of slots of the unique table.
func TableSize() int { return len(slots) }

// Purge rebuilds the table now with its live entries only (tests, and a
// program that wants to release memory after a phase).
func Purge() {
	live := LiveTerms()
	copyLive(sizeFor(live), live)
}
