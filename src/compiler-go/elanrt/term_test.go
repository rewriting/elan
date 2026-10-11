package elanrt

import (
	"runtime"
	"testing"
	"time"
)

func TestSameStructureIsTheSamePointer(t *testing.T) {
	x, y := nat(3), nat(3)
	if x != y {
		t.Fatal("s(s(s(o))) built twice: two nodes")
	}
	if x.Args[0] != nat(2) {
		t.Fatal("children are not shared")
	}
	if nat(2) == x {
		t.Fatal("different terms are equal")
	}
	if Mk2(symCons, MkInt(5), Mk0(symNil)) != Mk2(symCons, MkInt(5), Mk0(symNil)) {
		t.Fatal("lists not shared")
	}
	// big ints (not preallocated) and arity > 3 are shared too
	if MkInt(1<<20) != MkInt(1<<20) || MkInt(-1<<31) != MkInt(-1<<31) {
		t.Fatal("big ints not shared")
	}
	a, b := MkInt(1), MkInt(2)
	f := Mk(symF4, a, b, a, b)
	if f != Mk(symF4, a, b, a, b) || f == Mk(symF4, b, a, a, b) || len(f.Args) != 4 {
		t.Fatal("arity 4")
	}
	if MkBool(true) != True || MkBool(false) != False || True == False {
		t.Fatal("bools")
	}
}

func TestStringTermsAreShared(t *testing.T) {
	s := MkStr("caf\xe9")
	if s != MkStr("caf\xe9") || s == MkStr("cafe") || StrOf(s) != "caf\xe9" {
		t.Fatal("string sharing")
	}
	if !IsStr(s) || IsStr(MkInt(1)) || MkStr("") != MkStr("") {
		t.Fatal("IsStr / empty string")
	}
}

// A hit creates nothing; a construction of a new term creates one node.
func TestCreatedCounts(t *testing.T) {
	keep, big := nat(50), MkInt(987654)
	c0 := Created
	if nat(50) != keep || Created != c0 {
		t.Fatalf("hits created %d nodes", Created-c0)
	}
	_ = Mk2(symCons, big, keep) // a term built nowhere else
	if Created != c0+1 {
		t.Fatalf("created %d nodes, want 1", Created-c0)
	}
	runtime.KeepAlive(keep)
}

// Unused terms are collected: the live count goes back down after a GC,
// and the table shrinks when it is rebuilt.
func TestUnusedTermsAreCollected(t *testing.T) {
	runtime.GC()
	base := LiveTerms()
	l := list(200_000)
	if got := LiveTerms(); got < base+200_000 {
		t.Fatalf("live %d, want at least %d", got, base+200_000)
	}
	head := l.Args[0].Int
	l = nil
	runtime.GC()
	if got := LiveTerms(); got > base+100 {
		t.Fatalf("live %d after GC, want about %d", got, base)
	}
	Purge()
	if TableSize() > 4*MinTableSize {
		t.Fatalf("table of %d slots after purge", TableSize())
	}
	// a rebuilt term is a new node, equal by structure to the old one
	if list(3).Args[0].Int != 2 || head != 199_999 {
		t.Fatal("rebuild")
	}
}

// A long loop that builds and drops terms keeps the table bounded (GC
// before growing past GCBeforeGrow slots).
func TestTableSizeIsBoundedInABuildDropLoop(t *testing.T) {
	Purge()
	maxSize := 0
	for i := range 2000 {
		l := Mk0(symNil)
		for j := range 1000 {
			// distinct terms in every round: big ints
			l = Mk2(symCons, MkInt(int32(1<<20+i*1000+j)), l)
		}
		maxSize = max(maxSize, TableSize())
		runtime.KeepAlive(l)
	}
	// 2 M nodes created, at most a few thousand alive at a time
	if maxSize > 2*GCBeforeGrow {
		t.Fatalf("table grew to %d slots (%s)", maxSize, Stats())
	}
	t.Logf("max table size %d slots; %s", maxSize, Stats())
}

func TestStringsAreCollected(t *testing.T) {
	for i := range 100_000 {
		MkStr("s" + itoa(i))
	}
	n := len(strs)
	// the cleanups run on another goroutine after a GC
	for try := 0; try < 100 && len(freeStrs)+freedCount() < 50_000; try++ {
		runtime.GC()
		time.Sleep(time.Millisecond)
	}
	// the indices of the collected strings are reused
	for i := range 1000 {
		s := MkStr("again" + itoa(i))
		if StrOf(s) != "again"+itoa(i) {
			t.Fatal("a reused index holds the wrong bytes")
		}
	}
	if len(strs) != n {
		t.Fatalf("string table grew from %d to %d", n, len(strs))
	}
}

func freedCount() int {
	strsMu.Lock()
	defer strsMu.Unlock()
	return len(freedStrs)
}

func itoa(i int) string {
	var w Writer
	w.Int(int32(i))
	return string(w.Bytes())
}

// Int nodes are not shared beyond the cache: Eq compares them by value,
// and the nodes holding them are shared all the same.
func TestIntsAreComparedByValue(t *testing.T) {
	big := int32(1 << 20)
	a := MkInt(big)
	intCache[uint32(big)&(intCacheSize-1)] = nil // as if evicted
	b := MkInt(big)
	if a == b || !Eq(a, b) || Eq(a, MkInt(big+1)) || Eq(a, Mk0(symO)) || Compare(a, b) != 0 {
		t.Fatal("two int nodes of the same value: Eq, Compare")
	}
	if MkInt(big) != b || MkInt(5) != MkInt(5) {
		t.Fatal("the cache of ints, the small ints")
	}
	if Mk2(symCons, a, Mk0(symNil)) != Mk2(symCons, b, Mk0(symNil)) {
		t.Fatal("cons(a) and cons(b), a and b of the same value: one node")
	}
	e := []Term{a, b, MkInt(big + 1)}
	if Mult(e, 0) != 2 || FirstOcc(e, 1, nil) || !FirstOcc(e, 1, []int{0}) {
		t.Fatal("multiplicities of equal ints")
	}
	if x := MkAC(symU, Flatten(symU, a, b, MkInt(big+1))); MkAC(symU, Flatten(symU, b, MkInt(big+1), a)) != x {
		t.Fatal("AC terms of equal ints: one node")
	}
}
