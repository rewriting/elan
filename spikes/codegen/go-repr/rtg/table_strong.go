//go:build strong

package rtg

import "gorepr/rtstat"

// Strong unique table (as the first spike): nodes are never collected.

var (
	table = make([]*Node, 1<<12)
	used  int
)

func lookup(h uint64, sym, iv int32, args []Term) *Node {
	mask := uint64(len(table) - 1)
	for i := h & mask; table[i] != nil; i = (i + 1) & mask {
		if n := table[i]; n.hash == h && n.same(sym, iv, args) {
			return n
		}
	}
	return nil
}

func insert(n *Node) {
	put(table, n)
	used++
	if 2*used > len(table) {
		nt := make([]*Node, 2*len(table))
		for _, m := range table {
			if m != nil {
				put(nt, m)
			}
		}
		table = nt
		rtstat.Rebuilds++
	}
}

func put(t []*Node, n *Node) {
	mask := uint64(len(t) - 1)
	i := n.hash & mask
	for t[i] != nil {
		i = (i + 1) & mask
	}
	t[i] = n
}

func liveEntries() int { return used }
