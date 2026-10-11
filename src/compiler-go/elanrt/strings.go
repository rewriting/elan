package elanrt

import (
	"runtime"
	"sync"
)

// String terms. A string node (SymString) keeps its bytes in a side table,
// strs, at the index stored in its Int field: the Node type stays small
// (strings are rare in ELAN programs, nodes are not). The index is freed by
// a cleanup when the node is collected and reused by a later string. The
// index depends on the history of the run: the hash and the order of a
// string node come from its bytes, never from the index.
//
// The side table does not shrink: its length is the peak number of string
// nodes alive at once (16 bytes per entry), whose bytes are released when
// their index is freed (newStrIndex) and kept only for live strings.
//
// Only the program goroutine reads and writes strs; the cleanups (run on
// another goroutine) only append to freedStrs, under strsMu.

var (
	strs      []string
	freeStrs  []int32 // indices free in strs (program goroutine only)
	strsMu    sync.Mutex
	freedStrs []int32 // indices released by cleanups, not yet in freeStrs
)

// MkStr returns the string term s (bytes; see builtin.go for the C string
// rules, which the builtins apply before boxing).
func MkStr(s string) Term {
	h := strHash(s)
	if n := lookupStr(h, s); n != nil {
		Hits++
		return n
	}
	i := newStrIndex(s)
	n := newNode(SymString, i, h, nil)
	runtime.AddCleanup(n, releaseStr, i)
	insert(n)
	return n
}

// StrOf returns the bytes of a string term.
func StrOf(t Term) string { return strs[t.Int] }

// IsStr tells whether t is a string value (not a stuck term).
func IsStr(t Term) bool { return t.Sym == SymString }

func strHash(s string) uint64 {
	h := uint64(14695981039346656037)
	for i := 0; i < len(s); i++ {
		h = (h ^ uint64(s[i])) * 1099511628211
	}
	return mix(h^Syms[SymString].seed)&hashMask | 1 // (the top bits: flags of Node.Hash)
}

func lookupStr(h uint64, s string) *Node {
	mask := uint64(len(slots) - 1)
	for i := h & mask; slots[i].hash != 0; i = (i + 1) & mask {
		if slots[i].hash == h {
			if n := slots[i].p.Value(); n != nil && n.Sym == SymString && strs[n.Int] == s {
				return n
			}
		}
	}
	return nil
}

func newStrIndex(s string) int32 {
	if len(freeStrs) == 0 {
		strsMu.Lock()
		freeStrs, freedStrs = append(freeStrs, freedStrs...), freedStrs[:0]
		strsMu.Unlock()
		for _, i := range freeStrs {
			strs[i] = "" // release the bytes
		}
	}
	if k := len(freeStrs); k > 0 {
		i := freeStrs[k-1]
		freeStrs = freeStrs[:k-1]
		strs[i] = s
		return i
	}
	strs = append(strs, s)
	return int32(len(strs) - 1)
}

// releaseStr runs on the cleanup goroutine when a string node is
// collected. The program goroutine takes the index back in newStrIndex.
func releaseStr(i int32) {
	strsMu.Lock()
	freedStrs = append(freedStrs, i)
	strsMu.Unlock()
}
