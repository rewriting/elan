# G2 spike: Go representation of ELAN terms (NOTES)

Question (S7 spec, decision G2): which Go representation of terms should the
generated code use? (a) a generic node `{sym, args}` with maximal sharing
through a weak unique table (or `unique.Handle`); (b) one Go struct type per
operator and an interface per sort, matched by type switch, with native
builtins. Same programs, model and parity choices as `../go` (first Go
spike, strong table) and `../rust-enum` (typed Rust). Go 1.27.1, std only,
`GOTOOLCHAIN=local`, Apple M4 Max, 2026-10-10. Nothing outside this
directory was modified; nothing committed.

## Layout

| dir | what |
|---|---|
| `rtstat/` | shared by all runtimes: step counter, AC-order mode, interning counters, GC-cycle counter, `ELAN_STATS=1` report, `ELAN_CPUPROFILE` |
| `rtg/` | runtime (a), generic nodes: `*Node{Sym, Int int32, Ord, hash, Args}`; `table_weak.go` (default) or `table_strong.go` (tag `strong`); small-int cache (tag `nosmallint` turns it off, as `../go`) |
| `rtu/` | runtime (a) with `unique.Handle[node]` (fixed arity <= 3: free symbols only) |
| `rtt/` | runtime (b), typed: `Hdr`, generic weak `Table[N]` per operator type, `Intern`, AC helpers generic over the sort interface |
| `gen/{nqueens,efib,propc}` | generated code, variant (a): `../go`'s code unchanged except the import |
| `hyb/nqueens` | variant (a) + sort-directed native builtins ("hybrid", see below) |
| `uniq/nqueens` | the hybrid code over `rtu` (unique.Handle) |
| `typed/{nqueens,efib,propc}` | generated code, variant (b) |
| `depth/` | recursion-depth experiment |
| `build.sh`, `check.sh`, `measure.sh`, `results.tsv` | build all, check against oracles, measure (best of 5) |

Binaries (`bin/`): `g-*` generic weak, `gs-*` generic strong, `gn-*` generic
weak without small-int cache, `h-nqueens` hybrid, `u-nqueens` hybrid over
unique, `t-*` typed, `depth`. A private `GOCACHE` (`.gocache/`) was used.

The **hybrid** was not in the brief; I added it because the measurements
showed that the typed variant's gain on nqueens comes from native builtins
(as in rust-enum), and a generator that knows the sorts can give native
builtins to generic nodes too: values of builtin sorts (`int`, `bool`) and
of unboxed sorts (`qint`: only the injection `@ : (int) qint`) are Go
`int32`/`bool` in the generated functions; an int is boxed into a (cached,
shared) int node only when it is stored as an argument of a node.

## Correctness (`check.sh`: ALL OK)

- nqueens 8/10/11: `result` lines byte-identical (order included) to
  `work-current/nqueens/out{8,10,11}.txt`; nqueens 12 identical to
  `../rust/bin/nqueens 12`; for g, gs, gn, h, u, t. rewrite_step 128949 /
  3426635 / 19286638 / 116583341, as `../go` and rust.
- efib 50/100/150/200 = 11011074 / 35084101 / 60386349 / 83233826, steps
  3721 / 14946 / 33671 / 59896 (all variants, all runs).
- propc 1/2/3 = `t` in every variant and run. Steps: strong table 20546 /
  322083 / 810879 (= `../go`). **With a weak table the counts can vary from
  run to run** (seen: q1 20520, 20532, 20546, 20548; q3 810877, 810879), and
  with `GOGC=off` they are exactly 20546 / 322083 / 810879 again, for both g
  and t. Reason: as in rust-enum, AC multisets are sorted by creation id; a
  term that is collected and rebuilt gets a new id, so the order inside some
  multisets (and the redex chosen by `and(x, xor(y,z))`) changes. Unlike
  Rust's reference counting, Go's collection time depends on GC pacing, so
  it is **not deterministic** (see "History-independent order" below).

## Results

Best of 5 (`measure.sh`), real s / user s / max RSS MiB. `go1` = the
first Go spike binaries (`../go/bin`, strong table, every int interned).

| case | setting | go1 | gs (gen, strong) | g (gen, weak) | h (hybrid, weak) | u (hybrid, unique) | t (typed, weak) |
|---|---|---|---|---|---|---|---|
| nqueens 10 | default | 0.03 / 0.03 / 9.7 | 0.02 / 0.02 / 10.0 | 0.03 / 0.03 / 8.3 | 0.01 / 0.01 / 8.5 | 0.02 / 0.02 / 14.1 | 0.01 / 0.01 / 8.8 |
| nqueens 11 | default | 0.18 / 0.20 / 23.3 | 0.13 / 0.14 / 23.3 | 0.17 / 0.17 / 8.8 | 0.08 / 0.09 / 8.9 | 0.09 / 0.13 / 19.5 | 0.07 / 0.07 / 9.0 |
| nqueens 12 | default | 1.15 / 1.26 / 86.8 | 0.85 / 0.92 / 87.1 | 0.97 / 1.00 / 8.7 | 0.46 / 0.49 / 59.1* | 0.52 / 0.71 / 22.6 | **0.41 / 0.43 / 9.4** |
| nqueens 12 | GOGC=400 | 1.14 / 1.19 / 79.3 | 0.84 / 0.89 / 79.6 | 0.97 / 1.00 / 12.0 | 0.48 / 0.51 / 8.8 | 0.60 / 0.64 / 239.8 | 0.42 / 0.43 / 9.5 |
| nqueens 12 | GOMAXPROCS=1 | 1.19 / 1.18 / 84.9 | 0.88 / 0.88 / 84.8 | 0.95 / 0.94 / 7.0 | 0.45 / 0.45 / 6.9 | 0.77 / 0.76 / 156.7 | 0.40 / 0.39 / 7.5 |
| efib 100 | default | 0.01 / 0.02 / 21.1 | 0.01 / 0.02 / 22.0 | 0.01 / 0.01 / 11.1 | – | – | 0.01 / 0.01 / 11.1 |
| efib 150 | default | 0.03 / 0.09 / 52.9 | 0.03 / 0.07 / 53.2 | 0.03 / 0.03 / 11.7 | – | – | 0.03 / 0.04 / 11.7 |
| efib 200 | default | 0.07 / 0.22 / 108.5 | 0.07 / 0.20 / 113.9 | **0.06 / 0.07 / 11.5** | – | – | 0.08 / 0.08 / 13.9 |
| efib 200 | GOGC=400 | 0.06 / 0.08 / 156.9 | 0.05 / 0.07 / 157.4 | 0.05 / 0.06 / 22.8 | – | – | 0.07 / 0.07 / 22.9 |
| efib 200 | GOMAXPROCS=1 | 0.15 / 0.14 / 133.5 | 0.14 / 0.13 / 123.9 | 0.08 / 0.08 / 14.7 | – | – | 0.10 / 0.10 / 35.7 |
| propc 2 | default | 0.13 / 0.29 / 32.4 | 0.14 / 0.33 / 31.4 | 0.12 / 0.14 / 13.6 | – | – | 0.24 / 0.26 / 14.2 |
| propc 2 | GOGC=400 | 0.10 / 0.14 / 56.7 | 0.11 / 0.15 / 59.5 | 0.09 / 0.10 / 25.3 | – | – | 0.20 / 0.21 / 27.3 |
| propc 3 | default | 0.62 / 2.06 / 127.5 | 0.64 / 2.14 / 127.6 | 0.68 / 0.94 / 15.7 | – | – | 1.24 / 1.51 / 16.3 |
| propc 3 | GOGC=200 | 0.56 / 1.23 / 184.6 | 0.59 / 1.30 / 183.5 | 0.56 / 0.68 / 21.1 | – | – | 1.16 / 1.31 / 23.8 |
| propc 3 | GOGC=400 | 0.53 / 0.85 / 267.3 | 0.56 / 0.91 / 267.5 | **0.43 / 0.48 / 30.3** | – | – | 1.08 / 1.15 / 37.9 |
| propc 3 | GOMAXPROCS=1 | 1.25 / 1.24 / 174.3 | 1.31 / 1.30 / 167.1 | 1.02 / 1.00 / 21.6 | – | – | 1.62 / 1.58 / 19.9 |

\* h nqueens 12, default: RSS is the max over 5 runs; 4 runs were at ~9 MB
(see "table sizing"). Full matrix (all variants x default / GOGC=200 /
GOGC=400 / GOMAXPROCS=1, with the steps seen) in `results.tsv`.

Same session, Rust (default): nqueens 12: rust 0.82 s / 40 MB, rust-enum
0.19 s / 3.1 MB; efib 200: rust 0.05 s / 31 MB; propc 3: rust 0.27 s /
90 MB, rust-enum 0.38 s / 15 MB.

Extra settings (best of 3): `GOGC=off GOMEMLIMIT=64MiB`: g propc 3 **0.37 s
/ 62 MB** (t 0.87 s), g efib 200 0.05 s, h nqueens 12 0.48 s / 8 MB, t
nqueens 12 0.41 s / 9 MB. `gn` (weak, every int interned, as `../go`):
nqueens 12 2.54 s, 2.6x slower than g: each of the 268 M integer results is
a table hit, and a weak hit costs a `weak.Pointer.Value()` (~4 ns).

GC (ELAN_STATS, default GOGC): max pause 22-122 us, total 0.2-18 ms per
run; propc 3 (g) 592 cycles, 13 ms total. Pauses are not an issue; the
number of cycles is (see GC settings).

Compile time (warm cache, the generated main package changed, runtime
cached): 0.14-0.15 s for gen/hyb, 0.15-0.16 s for typed; no-op rebuild
0.09 s; cold (empty GOCACHE, std included) 1.5 s. Binaries 2.8-2.9 MB.

## Line counts (total / non-blank non-comment)

| file | lines |
|---|---|
| rtg (runtime.go + table_weak.go + 2 tag files) | 531 / 417 (strong table: 48 / 38 instead of 95 / 76) |
| rtt/runtime.go | 329 / 247 |
| rtu/runtime.go | 133 / 105 |
| rtstat/rtstat.go (instrumentation, shared) | 105 / 72 |
| ../go/rt/runtime.go (first spike) | 361 / 286 |

Generated code, non-blank non-comment lines, split into declarations (sorts,
symbols, types, methods, tables, constants) and rules + strategies + main:

| program | generic (gen) | hybrid (hyb) | typed |
|---|---|---|---|
| nqueens | 17 + 108 = 125 | 13 + 103 = 116 | 77 + 106 = 183 |
| efib | 21 + 215 = 236 | – | 116 + 235 = 351 |
| propc | 58 + 104 = 162 | – | 198 + 107 = 305 |

The rule code has the same size in all variants; typed adds per-sort
declarations: per operator a struct, one marker method per sort it belongs
to (`isProp()`, and `isSpace()`/`isObject()`/`isFib()` for subsorts by
injection), `Same`, `Print`, a table and a constructor. propc's q1..q3 bodies
are textually identical in gen and typed.

## Readability: the same rules side by side

propc, AC rule `[] and(x, xor(y, z)) => xor(and(x, y), and(x, z))` (inside
`fun_and`, `e` = the flattened sorted arguments):

```go
// generic (gen/propc)                          // typed (typed/propc)
for i := range e {                              for i := range e {
	if rt.Skip(e, nil, i) || e[i].Sym != sym_xor {	if rt.Skip(e, nil, i) {
		continue                                		continue
	}                                               	}
	x := fun_and(rt.Without(e, i, -1)...)           	xo, ok := e[i].(*Xor)
	ex := e[i].Args                                 	if !ok {
	y := ex[0]                                      		continue
	z := fun_xor(rt.Without(ex, 0, -1)...)          	}
	rt.Step()                                       	x := fun_and(rt.Without(e, i, -1)...)
	return fun_xor(fun_and(x, y), fun_and(x, z))    	y := xo.Args[0]
}                                                   	z := fun_xor(rt.Without(xo.Args, 0, -1)...)
return rt.ACMake(sym_and, e, nil)                   	rt.Step()
                                                    	return fun_xor(fun_and(x, y), fun_and(x, z))
                                                    }
                                                    return makeAnd(e)
```

nqueens, `[queens_n] queens(n,size) => x . ql if n>0 where ql:=(queens_strat)
queens(n-1,size) where xx:=(range) size where x:=()q2i(xx) if
noattack(1,x,ql)`:

```go
// generic (gen/nqueens): every value is a *Node, builtins are calls
func rule_queens_n(t rt.Term, k rt.Cont) bool {
	if t.Sym != sym_queens2 {
		return false
	}
	n, size := t.Args[0], t.Args[1]
	if rt.Gt(n, int_0) != rt.True {
		return false
	}
	return strat_queens_strat(fun_queens2(rt.Minus(n, int_1), size), func(ql rt.Term) bool {
		return strat_range(size, func(xx rt.Term) bool {
			x := fun_q2i(xx)
			if fun_noattack(int_1, x, ql) != rt.True {
				return false
			}
			rt.Step()
			return k(rt.Mk2(sym_cons, x, ql))
		})
	})
}

// hybrid (hyb/nqueens): generic nodes, native int32/bool
func rule_queens_n(t rt.Term, k rt.Cont) bool {
	if t.Sym != sym_queens2 {
		return false
	}
	n, size := t.Args[0].Int, t.Args[1].Int
	if !(n > 0) {
		return false
	}
	return strat_queens_strat(fun_queens2(n-1, size), func(ql rt.Term) bool {
		return strat_range(size, func(xx int32) bool {
			x := fun_q2i(xx)
			if !fun_noattack(1, x, ql) {
				return false
			}
			rt.Step()
			return k(rt.Mk2(sym_cons, rt.MkInt(x), ql))
		})
	})
}

// typed (typed/nqueens): struct per operator, type assertion, native ints
func rule_queens_n(t List, k rt.Cont[List]) bool {
	q, ok := t.(*Queens2)
	if !ok {
		return false
	}
	n, size := q.N, q.Size
	if !(n > 0) {
		return false
	}
	return strat_queens_strat(fun_queens2(n-1, size), func(ql List) bool {
		return strat_range(QInt(size), func(xx QInt) bool {
			x := fun_q2i(xx)
			if !fun_noattack(1, x, ql) {
				return false
			}
			rt.Step()
			return k(mkCons(x, ql))
		})
	})
}
```

and a function with several rules, `noattack`:

```go
// generic                                       // typed
func fun_noattack(v1, v2, v3 rt.Term) rt.Term {  func fun_noattack(v1, v2 int32, v3 List) bool {
	if v3 == con_nil {                            	switch v3 := v3.(type) {
		rt.Step()                                 	case *Nil:
		return rt.True                            		rt.Step()
	}                                             		return true
	if v3.Sym == sym_cons {                       	case *Cons:
		p, l := v3.Args[0], v3.Args[1]            		p, l := v3.Hd, v3.Tl
		s0 := rt.NeqInt(v2, p)                    		rec := fun_noattack(v1+1, v2, l)
		s1 := rt.NeqInt(rt.Minus(v2, p), v1)      		rt.Step()
		s1 = rt.And(s0, s1)                       		return v2 != p && v2-p != v1 && p-v2 != v1 && rec
		s0 = rt.NeqInt(rt.Minus(p, v2), v1)       	default:
		s0 = rt.And(s1, s0)                       		panic("noattack: no rule applies")
		s1 = fun_noattack(rt.Plus(v1, int_1), v2, l)	}
		rt.Step()                                 }
		return rt.And(s0, s1)
	}
	return rt.Mk3(sym_noattack, v1, v2, v3)
}
```

(The hybrid `noattack` is the typed one with `v3 == con_nil` /
`v3.Sym == sym_cons` and `v3.Args[0].Int, v3.Args[1]` instead of the type
switch.)

Assessment of readability:

- The big readability win is **native builtins** (`n > 0`, `n-1`,
  `v2 != p && ...` instead of `rt.Gt(n, int_0) != rt.True`, `rt.NeqInt`,
  `rt.And`). The hybrid gets it with generic nodes; it is not tied to typed
  terms.
- Typed adds names: `q.N`, `v3.Hd`, `*Xor`, `case *Cons:` read better than
  `t.Args[0].Int`, `e[i].Sym != sym_xor`. But field names exist only where
  ELAN gives them (`Fib[farg=@,val=@]` -> `Farg`, `Val`); for prefix
  operators a generator has only positions, so `queens(@,@)` would get
  `A1, A2` (I wrote `N, Size` by hand, which a generator cannot do), and
  operators with the same name and different arities need mangled type
  names (`Queens1`, `Queens2`), and sort/operator name clashes need
  suffixes (`Fib` sort vs `FibOp`).
- Typed also brings type checking of the generated code (passing a `Space`
  where a `Prop` is expected does not compile) — useful while writing the
  generator, nothing at run time. Exhaustiveness is not checked (a type
  switch needs an explicit `default: panic`), as the spec says.
- Typed costs 60-200 lines of declarations per program (more for programs
  with many operators/sorts: BenchThesis), and the runtime needs generic
  code over sort interfaces (`rt.Intern[N, P]`, `ACFlatten[S]`) that is
  harder to read than rtg's.
- AC rules read the same in both: the rule logic is in the multiset loops,
  not in the node representation.

## Performance assessment

- **nqueens (free symbols, builtins).** Typed is 2.4x faster than generic
  (0.41 vs 0.97 s); the hybrid gets almost all of it (0.46 s, +12 %). The
  gain is native ints (no table probe per arithmetic result), as rust-enum
  found. Typed is still 2x slower than rust-enum (0.19 s): closures
  (heap-allocated continuations, 0.9 M) and `weak.Make` per node.
- **efib (AC, few distinct terms).** All variants are within noise (0.05-
  0.08 s); weak tables cut RSS 10x (11 vs 114 MB) and the GC's parallel work
  (user 0.07 vs 0.20 s).
- **propc (AC heavy).** Generic weak is **2-2.5x faster than typed** at
  every setting (default 0.68 vs 1.24 s; GOGC=400 0.43 vs 1.08 s). Typed AC
  elements are interfaces: 16 bytes per element instead of 8 (total
  allocation 2.35 GB vs 1.58 GB), and each comparison when sorting a
  multiset reads `Ord` through an interface method call (`Compare` alone
  ~17 % of typed's profile, sorting 22 % vs 12 %); Go cannot reach the
  embedded header of an interface value without a call (or `unsafe`, which
  G3 excludes). Generic weak with GOGC=400 (0.43 s / 30 MB) beats the strong
  table (0.56 s / 268 MB) and `../go`, and approaches rust-enum (0.38 s /
  15 MB); `GOGC=off GOMEMLIMIT=64MiB` gives 0.37 s.
- **unique.Handle** (u): 0.52 s on nqueens 12 vs 0.46 s for the same code on
  the weak table; `unique.Make` costs ~1 us for a new value (6 allocations)
  and ~12 ns for a hit; it runs its cleanup on another goroutine (user >
  real) and RSS grows with GOGC (240 MB at GOGC=400). Its values must be
  comparable fixed-size structs: no slices, so n-ary AC nodes would need a
  binary comb encoding (O(n) access, extra nodes) or a side table that
  reintroduces the weak table. Not worth it.

## What matters for the real runtime

1. **weak.Make is the main cost of a weak table.** Micro-benchmark:
   `weak.Make` ~240 ns and 2 allocations (it adds a "special" record to the
   object's span under a lock, plus an 8-byte indirection), against 7.5 ns
   for allocating the node; `Value()` ~4 ns. Every new node pays it once:
   the 1M-deep `generate_list` takes 0.53 s with the weak table, 0.24 s with
   the strong one. Keep it off hot paths: native builtins (no int nodes),
   small-int cache, and do not hash-cons transient values.
2. **Lost sharing.** A collected term that is needed again is rebuilt (and
   pays weak.Make again): propc 3 creates 339 k nodes with the weak table
   and default GOGC, 182 k with GOGC=400, 139 k with the strong table.
3. **AC order must be history-independent.** With weak references, creation
   ids depend on GC timing, so step counts and the printed order of AC
   normal forms vary between runs (observed above). This breaks
   byte-identical output tests. Implemented alternative: `ELAN_ACORD=hash`
   orders AC elements by a structural hash (computed at construction from
   the symbol, scalars and the children's hash; ties of distinct terms =
   collisions, resolved structurally in rtg, a panic in rtt — none seen).
   Results are then identical across runs and GC settings (also GOGC=off):
   generic propc 1/2/3 = 20584 / 322221 / 810915, efib 100/200 = 12710 /
   50745; typed 20562 / 322277 / 811007 and 12695 / 50785 (different
   because the two runtimes hash differently; a real runtime defines one
   hash, e.g. from symbol names, to be stable across builds too). The real
   runtime should use such an order (or REM's order, if outputs must match
   the interpreter's).
4. **Table sizing vs GC pacing.** A weak entry reads as live until a GC
   cycle has run; a table that grows on "live" entries then holds dead
   nodes, its own size raises the heap goal, GC runs less, and so on: h
   nqueens 12 reached a 2 M-slot table and 114 MB. Fix implemented
   (`rtstat.MaybeGC`): when a rebuild would grow a table to >= 64 K slots
   and no GC cycle completed since its last rebuild (counted with a
   `runtime.AddCleanup` sentinel), run `runtime.GC()` first and recount:
   9 MB, ~140 forced GCs of ~20 us each, +5 % time (`ELAN_TABLEGC=0`
   disables it). It is not airtight (the cycle counter lags because cleanups
   run asynchronously: 1 run in 5 still reached 59 MB).
5. **GC settings.** With a weak table the live heap is tiny (2-20 MB), so the
   default GOGC=100 runs hundreds of cycles (propc 3: 592) and the heap is
   repeatedly released and re-acquired (on macOS `madvise`, called when
   spans are reused, was ~40 % of a one-core profile of g-propc 3 even at
   GOGC=400). The runtime should set
   `debug.SetGCPercent(400)` or `SetGCPercent(-1)` + `SetMemoryLimit(64 MiB
   or a fraction of RAM)` at start, overridable by `GOGC`/`GOMEMLIMIT`.
   GC pauses themselves are negligible (max 22-122 us).
6. **Closures.** Every continuation passed to another function escapes and
   is heap-allocated: nqueens 12 typed does 2.6 M allocations for 0.86 M
   nodes (0.86 M weak handles, ~0.9 M closures). Measurable but not
   dominant; the loop compilation of deterministic strategies (G4) matters
   more.
7. **Recursion depth** (`bin/depth`; main goroutine and a fresh goroutine
   behave the same). Go stacks grow by copying, in powers of two:
   - innermost `generate_list(10^6)`: OK, 135 MB of stack, 0.53 s (0.24 s
     strong); `10^7`: **fatal error: stack overflow** ("goroutine stack
     exceeds 1000000000-byte limit"), not recoverable (no panic, no
     `recover`), 512 lines / 35 KB of traceback on stderr, **exit status 2**
     — the status that `elanc-rs` uses for "unsupported construct" (spec
     goal 1), so the generated program must stop before the limit;
   - because stacks double, a stack needing more than 512 MiB must grow to
     1 GiB > 10^9: **the usable stack with the default limit is 512 MiB**;
   - `debug.SetMaxStack(n)` raises the limit, but the runtime caps it at
     2x the default (`maxstackceiling`): with SetMaxStack(2e9, 4e9 or 8e9)
     the limit reported is 2000000000 bytes and the usable stack is 1 GiB.
     `generate_list(10^7)` then works (1 GiB stack, 6.2 s, 4.3 GB RSS);
     `10^7 x 2` crashes;
   - the recursive printer is the deepest frame user: printing a 10^6
     list needs 538 MB of stack (2x10^6 crashes by default, works with
     SetMaxStack, 4x10^6 crashes at the ceiling): the printer must iterate on
     the last argument (lists) rather than recurse;
   - CPS `iterate*` chains are cheap in stack: 10^6 nested continuations =
     34 MB, 10^7 = 537 MB (OK), 0.37 s;
   - the G4 guard works: a depth counter at function entry stops with exit
     3 and a message (`ELAN_MAXDEPTH`); the limit must be set from the frame
     sizes (~135 B per innermost call, ~500 B per printer call) so that it
     triggers below 512 MiB (1 GiB with SetMaxStack).
8. **Native builtins need totality.** With native `bool`/`int32` results
   (typed and hybrid), a stuck `noattack(...)` or `go(...)` has no
   representation; the spike panics, as rust-enum does. The generic variant
   keeps such terms. A real compiler needs a totality analysis per defined
   operator of a builtin sort, or a fallback to nodes for the others.

## Recommendation for G2

**(a) generic nodes with a weak unique table, plus sort-directed native
builtins in the generated code (the "hybrid"), and a structural AC order.**

- Speed: the hybrid is within 12 % of typed on free/builtin code (nqueens)
  and generic is 2-2.5x faster than typed on AC code (propc), the case that
  dominates ELAN's hard benchmarks; efib is a tie. Typed has no case where
  it wins by more than ~15 % over the hybrid.
- Memory: both weak variants give the same small RSS (9-30 MB vs 80-270 MB
  for the strong table) once the table sizing and GOGC are handled.
- Readability: the hybrid has the native arithmetic and conditions that
  make typed code readable; what it lacks is field and type names
  (`t.Args[0].Int` vs `q.N`), which the generator can recover cheaply with
  comments or named locals at the top of each rule (`n, size :=
  t.Args[0].Int, t.Args[1].Int // queens(n,size)`), as done here. Typed
  costs 60-200 lines of declarations per program and a more abstract
  runtime (generics over interfaces).
- Generality: generic nodes keep a uniform view for printing, query
  parsing (one symbol table instead of a generated parser per sort, as
  rust-enum notes), tracing, the `.ref` model and the AC helpers; stuck terms
  of user sorts are representable without a variant per defined operator.
- Weak table rather than `unique.Handle`: unique cannot hold n-ary nodes,
  costs ~1 us per new node and gives no control over sizing.

Runtime tasks that follow: structural (deterministic) AC order; GC-aware
table growth; GOGC/memory-limit default in `main`; small-int cache; avoid
weak.Make for transient terms; iterative printer; depth guard calibrated on
512 MiB (or SetMaxStack(2e9) and 1 GiB); exit status of a Go stack overflow
(2) must never be reached.

## Deviations and choices

- Ints are `int32` with wrapping (spec: 32-bit), `x % 0 = 0` in `rtg.Mod`
  (unused here); `../go` used int64. Results are unaffected.
- rtg keeps `../go`'s generated code unchanged (import line and
  `rt.Step()`); node arguments of arity <= 3 are allocated inline with the
  node (one allocation), and small ints [-1024, 1024) are preallocated
  (`gn` = without, as `../go`).
- Typed: `qint` and `eInt` are unboxed value types (as rust-enum's `QInt`);
  `go`, `result`, `noattack` panic when stuck. efib `Fib`/`Compute` subsorts
  are interface embedding (no injection node). Constructors return the
  concrete pointer type (`*FibOp`): converting to the sort interface is then
  static (an interface-to-interface conversion would need an itab lookup).
- Typed AC flattening first used a callback returning a fresh singleton
  slice per element: 130 M allocations on propc 3 (2.0 s); the measured
  version uses `args(x) ([]S, bool)` (no allocation).
- `measure.sh` uses `/bin/sh`; settings are passed with `env`. Best of 5;
  times under 0.05 s are at the resolution of `/usr/bin/time`.
