package elanrt

import "testing"

//go:noinline
func guarded(n int32) int32 {
	Guard()
	defer Unguard()
	return n + 1
}

//go:noinline
func plain(n int32) int32 { return n + 1 }

func BenchmarkGuardedCall(b *testing.B) {
	var x int32
	for range b.N {
		x = guarded(x)
	}
}

func BenchmarkPlainCall(b *testing.B) {
	var x int32
	for range b.N {
		x = plain(x)
	}
}

// A node built again (table hit) and a new node (weak.Make).
func BenchmarkMk2Hit(b *testing.B) {
	l := list(10)
	for range b.N {
		_ = Mk2(symCons, MkInt(9), l.Args[1])
	}
}

func BenchmarkMk2New(b *testing.B) {
	l := Mk0(symNil)
	for i := range b.N {
		l = Mk2(symCons, MkInt(int32(i)), l)
		if i%1000 == 999 {
			l = Mk0(symNil)
		}
	}
}
