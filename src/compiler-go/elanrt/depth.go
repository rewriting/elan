package elanrt

import (
	"fmt"
	"os"
	"runtime/debug"
	"runtime/metrics"
	"unsafe"
)

// Bounded recursion (spec G4): a program never dies of Go's fatal "stack
// overflow" (exit status 2, a traceback, no recover), it stops with
//
//	elan: recursion too deep (stack of 1024 MiB exhausted); ...
//
// on the standard error and the exit status [DepthExit] (3), its output
// flushed, as elanc-rs's programs.
//
// Normalisation functions, strategies in continuation-passing style and
// the printer recurse, as deep as the data. Goroutine stacks grow by
// doubling up to the maximum stack size (debug.SetMaxStack; [Main] sets
// 2e9, so the largest stack is 1 GiB, the "ceiling"). The functions that
// can recurse call [Guard] at their entry and [Unguard] at their exit
// (defer): they count the depth.
//
// The limit is found at run time, because the bytes per level depend on
// the generated code and change from one recursion to the next. Every
// CheckEvery levels of descent, Guard takes a reading: the size of the
// goroutine stacks (runtime/metrics, ~0.3 us) and the address of a local
// variable (the stack pointer). Go grows a stack by doubling it when its
// use crosses the current size, and moves it then (a copy); a GC halves a
// stack used for less than a quarter of it. While the stack is below its
// ceiling there is no limit: it can still grow.
//
// When a reading sees the stack at the ceiling for the first time (the
// "anchor"), half the ceiling was in use at the depth dg where it grew,
// at most the deepest level since the last reading: the use at the anchor
// is estimated as C/2 x (d + CheckEvery)/dg (C the ceiling, d the current
// depth: the frames of the last levels have the average size). From then
// on the stack does not move: it cannot grow (Go's fatal overflow), and a
// shrink by a GC is seen by the next reading (the stack below its
// ceiling: no limit again, a new anchor when it grows back). The use at a
// reading is then measured, not estimated: the use at the anchor plus the
// distance between the two stack pointers, whatever the frames of the
// recursions since the anchor (a second deep recursion with bigger frames
// on the stack left at its ceiling by a first one, frames that grow with
// the depth). The program stops ([TooDeep]) when the use passes 3/4 of
// the ceiling: the last quarter is the margin for the levels until the
// next reading (CheckEvery levels of up to 16 KiB). The next reading is
// sooner (down to CheckEvery/16 levels) when the frames of the last
// levels would reach the limit before.
//
// Condition: a shrink and a growth back to the ceiling between two
// readings would move the stack unseen; it needs C/4 bytes of frames in at
// most 2 x CheckEvery levels (Unguard schedules a reading CheckEvery
// levels below the lowest level since the last one), frames of 8 KiB or
// more. The frames of the generated code are below 1 KiB (a function of
// 25 arguments: 160 bytes).
//
// Measured (Apple M4 Max, Go 1.27, tests in depth_test.go): innermost
// normalisation building a list, 96 bytes per level, stops at 8.4 M
// levels; a recursion on native ints, 48 bytes, 16.8 M; CPS iterate*, 160
// bytes, 5.0 M; 4 KiB frames, 180 K; 12 M levels of 48 bytes then 112
// bytes per level stops at 7.2 M levels of the second recursion. 10^6
// levels of each of the first three succeed. A Guard/Unguard pair costs
// ~2.6 ns, a reading ~0.3 us every 16 K levels.
//
// The guard and the program run on one goroutine; the counters are plain
// variables. Go's stack copy needs the old stack: memory peaks at 1.5 GiB
// of stacks when the stack grows to 1 GiB.

// DepthExit is the exit status of a program stopped by [TooDeep] (1:
// run-time error, 2: construct not supported).
const DepthExit = 3

// CheckEvery is the number of levels between two readings of the stack
// size.
const CheckEvery = 1 << 14

var (
	depth     int          // current guarded depth
	maxDepth  = NoLimit    // estimated depth of the limit (tests, ELAN_STATS)
	nextCheck = CheckEvery // depth of the next reading
	lowWater  = 0          // below it, nextCheck is moved down (Unguard)
	lastStack uint64       // stack bytes at the last reading
	lastDepth int          // depth at the last reading
	lastSP    uintptr      // stack pointer at the last reading
	ceiling   uint64       // largest stack size allowed (a power of two)
	// the anchor: the first reading with the stack at its ceiling
	anchored  bool
	anchorSP  uintptr // its stack pointer
	anchorUse int64   // the bytes of stack in use there (estimated)
)

func init() {
	old := debug.SetMaxStack(1 << 30)
	debug.SetMaxStack(old)
	setCeiling(old)
}

// setCeiling records the largest stack size for a maximum of max bytes:
// stacks are powers of two, a stack of 2^k > max fails.
func setCeiling(max int) {
	c := uint64(1)
	for 2*c <= uint64(max) {
		c *= 2
	}
	ceiling = c
}

// SetMaxStack sets Go's maximum stack size (debug.SetMaxStack) and the
// guard's ceiling accordingly.
func SetMaxStack(bytes int) {
	debug.SetMaxStack(bytes)
	setCeiling(bytes)
}

// Guard counts one level of recursion: call it at the entry of a function
// that can recurse, with `defer Unguard()`. It stops the program
// ([TooDeep]) past the limit.
func Guard() {
	depth++
	if depth >= nextCheck {
		checkDepth()
	}
}

// Unguard ends the level of the matching [Guard].
func Unguard() {
	depth--
	if depth < lowWater {
		// back up: the next descent is checked again from here
		nextCheck, lowWater = depth+CheckEvery, depth-CheckEvery
	}
}

// Depth is the current guarded depth (tests).
func Depth() int { return depth }

// NoLimit is the depth limit while the stack is below its ceiling.
const NoLimit = 1 << 62

// MaxDepth is the depth where the use of the stack would reach the limit
// if the frames kept the size of the last levels measured, NoLimit while
// the stack is below its ceiling (tests, ELAN_STATS).
func MaxDepth() int { return maxDepth }

var stackSample = []metrics.Sample{{Name: "/memory/classes/heap/stacks:bytes"}}

// stackBytes is the memory of the goroutine stacks (the program's stack,
// plus a few KiB for the others).
func stackBytes() uint64 {
	metrics.Read(stackSample)
	return stackSample[0].Value.Uint64()
}

// stackPointer is the address of a local variable of its caller's
// caller frame, near the stack pointer (the stack grows down).
//
//go:noinline
func stackPointer() uintptr {
	var x byte
	return uintptr(unsafe.Pointer(&x))
}

func checkDepth() {
	s, sp := stackBytes(), stackPointer()
	step := CheckEvery
	if s < ceiling {
		// below the ceiling (again, after a GC shrank the stack): no
		// limit, a growth to the ceiling is seen at a next reading
		anchored, maxDepth = false, NoLimit
	} else {
		if !anchored {
			// The stack grew to the ceiling since the last reading: half
			// the ceiling was in use at a depth dg of at most the deepest
			// level since then (usually this descent, dg = depth; if the
			// program went back up after it, the last reading's depth +
			// CheckEvery bounds it).
			dg := max(depth, lastDepth+CheckEvery)
			anchored, anchorSP = true, sp
			anchorUse = int64(ceiling/2) * int64(depth+CheckEvery) / int64(dg)
		}
		use := anchorUse + int64(anchorSP) - int64(sp)
		limit := int64(ceiling / 4 * 3)
		if use > limit {
			TooDeep()
		}
		// the frames since the last reading, if it was on this stack and
		// above: the depth of the limit at that size, the next reading
		// before it
		maxDepth = NoLimit
		if lastStack >= ceiling && depth > lastDepth && lastSP > sp {
			perLevel := int64(lastSP-sp) / int64(depth-lastDepth)
			if perLevel > 0 {
				left := int((limit - use) / perLevel)
				maxDepth = depth + left
				step = min(max(left+1, CheckEvery/16), CheckEvery)
			}
		}
	}
	lastStack, lastDepth, lastSP = s, depth, sp
	nextCheck, lowWater = depth+step, depth-CheckEvery
}

// TooDeepMessage is the message of [TooDeep] for a stack of size bytes.
func TooDeepMessage(size uint64) string {
	return fmt.Sprintf("elan: recursion too deep (stack of %d MiB exhausted); the program stops "+
		"(exit status %d). A deterministic formulation (repeat*, first one, "+
		"a tail-recursive strategy) runs in constant stack.", size>>20, DepthExit)
}

// TooDeep writes the results printed so far, prints the message on the
// standard error and exits with [DepthExit].
func TooDeep() {
	FlushOutput()
	fmt.Fprintln(os.Stderr, TooDeepMessage(ceiling))
	if os.Getenv("ELAN_STATS") == "1" {
		fmt.Fprintf(os.Stderr, "elanrt: depth %d, limit %d, stack %d MiB\n", depth, maxDepth, stackBytes()>>20)
	}
	os.Exit(DepthExit)
}
