//go:build !strong

package rtg

import (
	"weak"

	"gorepr/rtstat"
)

// Weak unique table: open addressing, linear probing, power-of-two size.
// A slot holds the node's hash and a weak pointer; a node nobody references
// is collected by the GC and its slot then reads as dead (Value() == nil).
// Dead slots are not reused: they are dropped when the table is rebuilt,
// which happens when used slots (live or dead) exceed 1/2. The new size is
// chosen from the live count (load <= 1/4), so the table also shrinks.

type slot struct {
	h uint64 // 0 = empty
	p weak.Pointer[Node]
}

var (
	table  = make([]slot, 1<<12)
	used   int
	lastGC int64 // rtstat.GCCycles at the last rebuild
)

func lookup(h uint64, sym, iv int32, args []Term) *Node {
	mask := uint64(len(table) - 1)
	for i := h & mask; table[i].h != 0; i = (i + 1) & mask {
		if table[i].h == h {
			if n := table[i].p.Value(); n != nil && n.same(sym, iv, args) {
				return n
			}
		}
	}
	return nil
}

func insert(n *Node) {
	put(table, n.hash, weak.Make(n))
	used++
	if 2*used > len(table) {
		rebuild()
	}
}

func put(t []slot, h uint64, p weak.Pointer[Node]) {
	mask := uint64(len(t) - 1)
	i := h & mask
	for t[i].h != 0 {
		i = (i + 1) & mask
	}
	t[i] = slot{h, p}
}

func rebuild() {
	live := 0
	for i := range table {
		if table[i].h != 0 && table[i].p.Value() != nil {
			live++
		}
	}
	size := 1 << 12
	for size < 4*live {
		size *= 2
	}
	if size > len(table) && size >= 1<<16 && rtstat.MaybeGC(lastGC) {
		// dead nodes were waiting for a collection: recount
		live = liveEntries()
		for size = 1 << 12; size < 4*live; size *= 2 {
		}
	}
	lastGC = rtstat.GCCycles.Load()
	nt := make([]slot, size)
	for i := range table {
		if table[i].h != 0 && table[i].p.Value() != nil {
			put(nt, table[i].h, table[i].p)
		}
	}
	rtstat.Rebuilds++
	rtstat.Purged += int64(used - live)
	table, used = nt, live
}

func liveEntries() int {
	live := 0
	for i := range table {
		if table[i].h != 0 && table[i].p.Value() != nil {
			live++
		}
	}
	return live
}
