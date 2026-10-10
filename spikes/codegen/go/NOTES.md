# Go back end: notes

Go 1.27.1 (darwin/arm64), std only, default `go build` (no flags, no PGO).
Module `elanspike`: `rt/` (runtime), `nqueens/`, `efib/`, `propc/` (one
`package main` each, the "generated" code), `build.sh` -> `bin/`.

## Results (all checked)

| Run | result | rewrite_step (this impl.) | REM rewrite_step |
|---|---|---|---|
| nqueens 8 | 92 solutions, same order as `out8.txt` | 128949 | 648617 |
| nqueens 10 | 724 solutions, same order as `out10.txt` | 3426635 | |
| nqueens 11 | 2680 solutions, same order as `out11.txt` | 19286638 | |
| efib 50 | 11011074 | 3721 | 2593 |
| efib 100 | 35084101 | 14946 | 10193 |
| propc 1/2/3 | t / t / t | 20546 / 322083 / 810879 | 12841 / 218978 / 556880 |

(nqueens: the `result = ` lines are byte-identical to the oracle's, diffed.
efib: our step counts equal the ones in the comment at the end of
`efib.eln`, so this enumeration order coincides with an older ELAN's.)

## Measurements

One run each, `/usr/bin/time -l`, Apple Silicon, output to /dev/null.
REM = the current compiler (`work-current/*/t*.txt`, real time).

| Run | real (s) | user (s) | max RSS (MB) | REM real (s) |
|---|---|---|---|---|
| nqueens 8 | 0.00 | 0.00 | 4.5 | 0.41 |
| nqueens 10 | 0.03 | 0.03 | 9.6 | 3.85 |
| nqueens 11 | 0.20 | 0.22 | 23.3 | 22.16 |
| efib 50 | 0.00 | 0.00 | 7.1 | 0.62 |
| efib 100 | 0.01 | 0.02 | 20.3 | 9.08 |
| propc 1 | 0.00 | 0.00 | 10.1 | 0.21 |
| propc 2 | 0.13 | 0.30 | 31.4 | 1.49 |
| propc 3 | 0.65 | 2.09 | 123.7 | 9.70 |

Go's GC is concurrent and parallel: on propc user time is 2-3x real time.
With `GOMAXPROCS=1` (one core, fairer against single-threaded C/Rust):
nqueens 11 0.20 s / 21.6 MB, propc 2 0.23 s / 35.5 MB, propc 3 1.36 s /
172.5 MB.

Build: clean build (empty `GOCACHE`, includes compiling the Go runtime and
std) 1.45 s real; incremental after touching `rt/runtime.go` 0.28 s.
Binaries ~2.2 MB each (static, Go runtime included).

## Line counts

| File | lines | non-blank, non-comment |
|---|---|---|
| rt/runtime.go (runtime) | 361 | 286 |
| nqueens/nqueens.go | 174 | 125 |
| efib/efib.go | 297 | 236 |
| propc/propc.go | 210 | 162 |

(propc's three huge right-hand sides q1..q3 were translated mechanically
from the `.eln` text by a regex, one Go call per ELAN application.)

## Design

- **Terms**: `*Node{Sym, ID, Int, hash, Args []Term}`; one global unique
  table (open addressing, linear probing, power-of-two, grows at load 1/2).
  Equality = pointer equality. `ID` = creation order, the total order used
  to sort AC arguments. Builtin ints are nodes (`SymInt`, value in `Int`),
  hash-consed like everything else (no small-int cache). `true`/`false`
  are nodes; builtin `and`, `not`, `>`, `==`, `!=`, `+`, `-`, `%` take and
  return terms (no rewrite step counted for builtins).
- **Args slices**: `Mk1..Mk3` pass a stack array; `mk` clones the slice only
  on insertion, so a hit in the table allocates nothing.
- **AC**: `ACFlatten` splices nested nodes of the same symbol and sorts by
  ID (`slices.SortFunc`); `ACMake` gives the element itself for one element
  and the neutral (`empty` for `U`) for none. Matching helpers: `ACElems`
  (view a term as a multiset), `Skip` (enumerate distinct choices given a
  `used` mask: skip a position equal to an unused predecessor), `Rest`,
  `Without`.
- **Normalisation**: one `fun_<sym>` per defined symbol, called with normal
  arguments, applying unlabelled rules in source order; constructors only
  hash-cons. Innermost, no short-circuit: `noattack` evaluates all four
  conjuncts, as REM does.
- **Strategies (CPS)**: `rule_<label>(t, k rt.Cont) bool` and
  `strat_<name>(t, k) bool`, `rt.Cont = func(Term) bool` (true = stop).
  `dk` = sequence of calls; `dc(S)` = wrapper continuation setting a `got`
  flag; `first one` = each alternative with a "store and stop"
  continuation, then `k(res)`; `iterate*` = `k(t)` then `S(t, r ->
  iterate*(r, k))`; `repeat*(first one(...))` is deterministic, compiled as
  a `for` loop; `where x := (S) u` nests the rest of the rule in the
  continuation passed to `S` (nqueens: `queens_strat` then `range`,
  depth-first, which gives the oracle's order).
- **Memory**: Go GC. The unique table is strong, so terms are never
  reclaimed (as with a never-freeing arena); the GC only reclaims transient
  garbage: flattening/rest slices, `used` masks, closures. RSS therefore
  grows with the number of distinct terms ever built.

## Deviations from the brief / choices to know

1. **AC rules with extension** (propc), following REM's generated code:
   `and(x,x)` is `and(x,x,R...) => and(x,R...)`; `xor(x,x)` is
   `xor(x,x,R...) => xor(R...,f)`; in `and(x,t)`, `and(x,f)`, `xor(x,f)`,
   `and(x,xor(y,z))` the variable `x` takes the whole rest of the
   multiset; in `xor(y,z)` against an n-ary xor, `y` = first element,
   `z` = rest. A variable bound to an AC rest is rebuilt with the symbol's
   normalising function (`fun_and(rest...)`) before use, otherwise
   `and(x,t) => x` could return a non-normal rest.
2. **efib `empty`**: kept as an ordinary element of the multiset (it is in
   the initial term; `empty` is not declared neutral). An AC rest with no
   element would be `empty` (never happens here). Unused pattern variables
   (`S`, `v` in `occursFib`/`result`) are not built.
3. **efib `compute`**: each condition is tested as soon as its variables
   are bound (`n1 == n2+1` after the 2nd element, `n == n1+1` after the
   3rd), i.e. "conditions pruning". Testing only after the 3 elements are
   chosen would make the match cubic in the multiset size.
4. **efib**: `v1+v2 % 1000000` = `v1 + (v2 % 1000000)` (`%` pri 800 > `+`
   pri 500 in `builtinInt.eln`); this is what gives the expected values.
5. **Right-hand sides**: in efib, `S U a U b` is emitted as one variadic
   call `fun_U(S, a, b)` (compile-time flattening of the same AC symbol);
   in propc the q1..q3 terms are emitted as nested calls without that
   flattening (`fun_and(fun_and(a7,a8), ...)`). Both are normal-form
   equivalent.
6. **`det_queens_strat`, `init1`, `init2`, `generate_list`** are generated
   but unused (kept to show the shape).
7. rewrite_step differs from REM (as allowed): we count every rule
   application, builtins excluded; REM's nqueens counts are much higher.

## Where Go forced (or suggested) a different shape

- No `goto`-based rule fall-through needed; rules are `for` loops over
  multiset positions with `continue`, which maps naturally.
- Continuations are closures capturing the rule's variables; the Go
  compiler heap-allocates a closure whenever it escapes (passed to another
  `strat_`/`rule_` function), so every `where` and every `iterate*` step
  allocates. There is no way to express "environment on the stack" as in
  the C model; this is the main per-call cost.
- AC functions are variadic (`...Term`), so a call allocates the argument
  slice unless inlined; flattening copies into a fresh slice anyway.
- `used []bool` masks have dynamic size, hence heap allocation per match.
- No way to free a term explicitly or to have a weak unique table without
  the `weak` package (std, but more code); we kept a strong table.
- Package `rt` instead of `runtime` (name taken by the std package).
- Symbol codes/constants are package-level `var`s initialised at start-up
  (Go has no constant pointers); the order of initialisation follows the
  dependency order, which is fine here.

## Fairness caveats for the C/Rust comparison

- Go's GC runs on other cores: real time understates CPU (propc 3: 0.65 s
  real, 2.09 s user; 1.36 s with `GOMAXPROCS=1`). Compare user time, or
  GOMAXPROCS=1, against single-threaded C/Rust.
- RSS includes the Go runtime (~4 MB floor) and GC headroom (GOGC=100:
  heap may reach ~2x live data). The unique table is never purged.
- Times below 0.05 s are at the resolution of `/usr/bin/time` (one run
  each, no repetition); only nqueens 11 and propc 2/3 are really
  measurable.
- Design choices that change the work done, and that the other languages
  must match to be comparable: early condition pruning in efib `compute`;
  AC-rest rebuilding through the normalising function in propc; no
  small-int cache; no memoisation of normal forms.
