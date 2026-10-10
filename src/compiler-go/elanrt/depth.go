package elanrt

import (
	"fmt"
	"os"
	"runtime/debug"
	"runtime/metrics"
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
// The limit on the depth is found at run time, because the bytes per level
// depend on the generated code: every CheckEvery levels of descent, Guard
// reads the size of the goroutine stacks (runtime/metrics, ~0.3 us). Go
// grows a stack by doubling it when its use crosses the current size, so
// when the stack is seen to have grown to the ceiling, half the ceiling was
// in use at about the current depth d: the limit becomes 3d/2, where 3/4 of
// the ceiling is in use if the frames keep their average size (the last
// quarter is the margin for frames up to 2x bigger than the average below
// d). While the stack is below its ceiling there is no limit (a GC that
// shrinks the stack removes it).
//
// Measured (Apple M4 Max, Go 1.27, tests in depth_test.go): innermost
// normalisation building a list, 96 bytes per level, stops at 8.4 M
// levels; a recursion on native ints, 48 bytes, 16.8 M; CPS iterate*, 160
// bytes, 5.0 M; 4 KiB frames, 197 K. 10^6 levels of each of the first
// three succeed. A Guard/Unguard pair costs ~2.6 ns.
//
// Limit of the method: the limit is computed from the frames below d. A
// recursion whose frames above d are more than 2x bigger on average, or a
// second deep recursion with bigger frames while the stack is still at its
// ceiling (not shrunk by a GC), could reach Go's fatal overflow.
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
	maxDepth  = NoLimit    // limit, set when the stack reaches the ceiling
	nextCheck = CheckEvery // depth of the next reading
	lowWater  = 0          // below it, nextCheck is moved down (Unguard)
	lastStack uint64       // stack bytes at the last reading
	lastDepth int          // depth at the last reading
	ceiling   uint64       // largest stack size allowed (a power of two)
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
		nextCheck, lowWater = min(depth+CheckEvery, maxDepth+1), depth-CheckEvery
	}
}

// Depth is the current guarded depth (tests).
func Depth() int { return depth }

// NoLimit is the depth limit while the stack is below its ceiling.
const NoLimit = 1 << 62

// MaxDepth is the depth limit, NoLimit while the stack has not reached
// its ceiling (tests).
func MaxDepth() int { return maxDepth }

var stackSample = []metrics.Sample{{Name: "/memory/classes/heap/stacks:bytes"}}

// stackBytes is the memory of the goroutine stacks (the program's stack,
// plus a few KiB for the others).
func stackBytes() uint64 {
	metrics.Read(stackSample)
	return stackSample[0].Value.Uint64()
}

func checkDepth() {
	if depth > maxDepth {
		TooDeep()
	}
	s := stackBytes()
	switch {
	case s < ceiling:
		// below the ceiling (again, after a GC shrank the stack): no
		// limit, a growth to the ceiling is seen at a next reading
		maxDepth = NoLimit
	case lastStack < ceiling:
		// The stack grew to the ceiling since the last reading: half the
		// ceiling was in use at a depth of at most d (the deepest level
		// since the last reading). Usually the growth happened during this
		// descent, d is about the current depth; if the program went back
		// up right after it, d comes from the last reading.
		d := max(depth, lastDepth+CheckEvery)
		maxDepth = d + d/2
	}
	lastStack, lastDepth = s, depth
	nextCheck, lowWater = min(depth+CheckEvery, maxDepth+1), depth-CheckEvery
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
