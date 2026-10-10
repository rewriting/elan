package elanrt

// Strategies in continuation-passing style (spec G4).
//
// A strategy applied to t calls its continuation k once per result r, in
// ELAN's order. k returns true to stop: the strategy then returns true at
// once, without computing more results (a `one`, a `first one`, the end of
// the program). A strategy that returns false has delivered all its results
// (possibly none: failure).
//
// The combinators are generic over the value type T: a strategy on a user
// sort works on [Term]s, a strategy on a builtin sort without stuck terms
// on native values (int32, bool, string), as the generated functions do
// (spec G2: native builtins). Generated code may also inline its own loops
// instead of calling them.
//
// Orders (checked against the interpreter, elan -b, as elanc-rs's
// elan-runtime/tests/strat.rs): iterate* is depth first, the term itself
// first; repeat* yields the leaves of the same tree in the same order.

// K is a success continuation: called once per result; true means stop.
type K[T any] func(r T) bool

// Strat is a strategy: its results on t go to k; it returns true if k
// asked to stop.
type Strat[T any] func(t T, k K[T]) bool

// Id is the strategy `id`: the term itself.
func Id[T any](t T, k K[T]) bool { return k(t) }

// Fail is the strategy `fail`: no result.
func Fail[T any](t T, k K[T]) bool { return false }

// Seq is `s1 ; s2`: s2 applied to every result of s1.
func Seq[T any](t T, k K[T], s1, s2 Strat[T]) bool {
	return s1(t, func(r T) bool { return s2(r, k) })
}

// Dk is `dk(s1,...,sn)`: all the results of every si, in order.
func Dk[T any](t T, k K[T], ss ...Strat[T]) bool {
	for _, s := range ss {
		if s(t, k) {
			return true
		}
	}
	return false
}

// First is `first(s1,...,sn)` (and `dc`, compiled as first): all the
// results of the first si that has at least one.
func First[T any](t T, k K[T], ss ...Strat[T]) bool {
	for _, s := range ss {
		found := false
		stop := s(t, func(r T) bool {
			found = true
			return k(r)
		})
		if stop || found {
			return stop
		}
	}
	return false
}

// FirstResult returns the first result of s on t (the search stops there),
// and false if there is none.
func FirstResult[T any](t T, s Strat[T]) (T, bool) {
	var res T
	found := false
	s(t, func(r T) bool {
		res, found = r, true
		return true
	})
	return res, found
}

// AllResults returns all the results of s on t, in order (tests, small
// searches).
func AllResults[T any](t T, s Strat[T]) []T {
	var rs []T
	s(t, func(r T) bool {
		rs = append(rs, r)
		return false
	})
	return rs
}

// One is `one(s)`: the first result of s.
func One[T any](t T, k K[T], s Strat[T]) bool {
	if r, ok := FirstResult(t, s); ok {
		return k(r)
	}
	return false
}

// FirstOne is `first one(s1,...,sn)` (and `dc one`): one result, from the
// first si that has one.
func FirstOne[T any](t T, k K[T], ss ...Strat[T]) bool {
	for _, s := range ss {
		if r, ok := FirstResult(t, s); ok {
			return k(r)
		}
	}
	return false
}

// IterateStar is `iterate*(s)`: t, then iterate*(s) on each result of
// s(t), depth first.
func IterateStar[T any](t T, k K[T], s Strat[T]) bool {
	Guard()
	defer Unguard()
	if k(t) {
		return true
	}
	return s(t, func(r T) bool { return IterateStar(r, k, s) })
}

// IteratePlus is `iterate+(s)`: iterate*(s) on each result of s(t) (t
// itself excluded).
func IteratePlus[T any](t T, k K[T], s Strat[T]) bool {
	return s(t, func(r T) bool { return IterateStar(r, k, s) })
}

// RepeatStar is `repeat*(s)`: the terms reached from t on which s fails,
// depth first (t itself if s fails on t).
func RepeatStar[T any](t T, k K[T], s Strat[T]) bool {
	Guard()
	defer Unguard()
	applied := false
	stop := s(t, func(r T) bool {
		applied = true
		return RepeatStar(r, k, s)
	})
	if stop {
		return true
	}
	if !applied {
		return k(t)
	}
	return false
}

// RepeatPlus is `repeat+(s)`: as repeat*(s), but s must apply at least
// once.
func RepeatPlus[T any](t T, k K[T], s Strat[T]) bool {
	return s(t, func(r T) bool { return RepeatStar(r, k, s) })
}

// Deterministic strategies (at most one result: `first one`, `dc one`,
// `one`, a labelled rule set proved deterministic) are given as a step
// function: the next term and true, or false when the strategy fails.
// Their iterations are loops: constant stack, whatever the number of
// steps (spec G4, elanc-rs S6b task 3).

// RepeatStarDet is `repeat*(s)` for a deterministic s: the last term.
func RepeatStarDet[T any](t T, step func(T) (T, bool)) T {
	for {
		r, ok := step(t)
		if !ok {
			return t
		}
		t = r
	}
}

// IterateStarDet is `iterate*(s)` for a deterministic s: t, s(t),
// s(s(t)), ... to k, until s fails or k stops.
func IterateStarDet[T any](t T, k K[T], step func(T) (T, bool)) bool {
	for {
		if k(t) {
			return true
		}
		r, ok := step(t)
		if !ok {
			return false
		}
		t = r
	}
}

// DetStep turns a strategy into the step function of a deterministic
// strategy (its first result).
func DetStep[T any](s Strat[T]) func(T) (T, bool) {
	return func(t T) (T, bool) { return FirstResult(t, s) }
}
