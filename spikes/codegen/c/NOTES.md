# C spike: notes

Hand-written "generated" C for nqueens, efib and propc, following `../BRIEF.md`
(hash-consing, success continuations, no cpl). `./build.sh` builds
`bin/{nqueens,efib,propc}` (Boehm GC, static `libgc.a`) and `bin/*-arena`
(bump arena, never frees), Apple Clang `-O2`. `./measure.sh` runs every
binary once under `/usr/bin/time -l` and prints the table below.

## Results (all checked)

- nqueens 8/10/11: the `result = ...` lines are identical, in order, to
  `work-current/nqueens/out{8,10,11}.txt` (92 / 724 / 2680 solutions), for
  both builds.
- efib: go(50) = 11011074, go(100) = 35084101, both builds.
- propc: q1, q2, q3 = `t`, both builds.
- Step counts are the same as the Go version's (nqueens 8 = 128949, efib
  3721 / 14946, propc 20546 / 322083 / 810879). Exception: the GC build of
  propc 1 gives 20516 (see "weak table" below).

## Measurements (one run each, Apple Silicon, macOS, `/usr/bin/time -l`)

| run | real s | user s | sys s | max RSS MB | rewrite_step |
|---|---|---|---|---|---|
| nqueens 8 | 0.00 | 0.00 | 0.00 | 3.2 | 128949 |
| nqueens-arena 8 | 0.00 | 0.00 | 0.00 | 2.3 | 128949 |
| nqueens 10 | 0.02 | 0.02 | 0.00 | 4.1 | 3426635 |
| nqueens-arena 10 | 0.02 | 0.02 | 0.00 | 5.3 | 3426635 |
| nqueens 11 | 0.14 | 0.13 | 0.00 | 4.6 | 19286638 |
| nqueens-arena 11 | 0.13 | 0.13 | 0.00 | 17.4 | 19286638 |
| efib 50 | 0.09* | 0.00 | 0.00 | 4.1 | 3721 |
| efib-arena 50 | 0.04* | 0.00 | 0.00 | 3.1 | 3721 |
| efib 100 | 0.01 | 0.00 | 0.00 | 4.1 | 14946 |
| efib-arena 100 | 0.00 | 0.00 | 0.00 | 9.0 | 14946 |
| propc 1 | 0.05* | 0.00 | 0.00 | 4.7 | 20516 |
| propc-arena 1 | 0.04* | 0.00 | 0.00 | 4.8 | 20546 |
| propc 2 | 0.07 | 0.07 | 0.00 | 6.8 | 322083 |
| propc-arena 2 | 0.06 | 0.06 | 0.00 | 48.8 | 322083 |
| propc 3 | 0.53 | 0.52 | 0.00 | 15.0 | 810879 |
| propc-arena 3 | 0.34 | 0.32 | 0.01 | 226.6 | 810879 |

`*` real time of a few hundredths with 0.00 user: process start-up noise
(first run after a rebuild), not computation. For reference the current
compiler (REM + cpl, `work-current/*/t*.txt`) takes 0.41 / 3.85 / 22.16 s
real for nqueens 8/10/11, 0.62 / 9.08 s for efib 50/100 and 0.21 / 1.49 /
9.70 s for propc 1/2/3.

Experiment (not in `bin/`, build with `-DSTRONG_TABLE`): Boehm with a
*strong* unique table (scanned, nothing ever collectable) is the worst of
both: propc 3 0.77 s real / 0.60 user / 288 MB, nqueens 11 0.33 s / 20 MB.
The GC only pays off with a weak table.

## Line counts

| part | files | lines |
|---|---|---|
| runtime | runtime.h + runtime.c | 92 + 278 = 370 |
| generated | nqueens.c / efib.c / propc.c | 170 / 207 / 151 = 528 |
| scripts | build.sh, measure.sh | 13 + 14 |

(propc's q1..q3 right-hand sides are three long lines, mechanically
converted from the `.eln` text.)

## Design

- **Terms** (`struct Term`: sym, arity, hash, id, args[]; 16-byte header).
  Builtin ints are nodes (sym 0) holding the value in `args[0]`; `true` /
  `false` are constants. Every construction goes through one unique table
  (open addressing, linear probing), so equality is pointer equality. Each
  node gets a 32-bit creation `id`; AC arguments are sorted by id.
- **Weak unique table (GC build)**: the table is `malloc`ed (not scanned by
  Boehm) and holds *hidden* pointers registered as disappearing links
  (`GC_general_register_disappearing_link`, moved with
  `GC_move_disappearing_link` on resize). A term reachable only from the
  table is collected and its slot cleared (tombstone). Consequence: a term
  that dies and is rebuilt gets a new id, so the AC order (and the order in
  which AC rules find their first match) can depend on GC timing; the
  canonical form stays consistent (an AC node keeps its arguments alive),
  results are unaffected, but the step count may vary (propc 1: 20516 vs
  20546). Arena build: plain pointers, deterministic.
- **AC**: `ac_build` flattens, insertion-sorts by id, hash-conses. Matching
  helpers: a `used[]` byte array over the argument positions, `ac_next`
  (distinct elements of the remaining multiset, in order), `ac_rest`
  (rest as element / AC node, or NULL when empty: no unit, so a rest
  variable cannot match nothing). The `empty` constant of efib is an
  ordinary element, as in ELAN.
- **Normalisation**: one C function per defined symbol (`fn_*`), innermost
  (arguments are normal before the call); strict builtin `and` (no
  short-circuit, as REM: `noattack` always walks the whole list).
- **Strategies**: CPS, `K = {fn, env}` passed by value, environments are
  structs on the C stack (no heap allocation per call). `kcall` returns
  "stop". `dk`, `dc`, `first one`, `iterate*`, `repeat*` (a loop, since
  its argument is `first one`) and `where x := (S)` are emitted as in the
  brief. A function whose rule has a strategy `where` (efib `go`) takes
  the first result of the where (one_k stops).
- **rewrite_step** counts program rule applications only (labelled and
  unlabelled), not builtin int/bool operations.

## Choices aligned with the Go version (parity note from the controller)

1. efib `compute`: each condition is tested as soon as its variables are
   bound (`n1 == n2+1` after the 2nd element, `n == n1+1` after the 3rd).
   Elements are chosen in pattern order. `v1, v2 : builtinInt`, so a `Fib`
   with `val=UNDEF` does not match them.
2. propc AC rules as REM compiles them: `and(x,x)` becomes
   `and(x,x,R) => and(x,R)` and `xor(x,x)` becomes `xor(x,x,R) => xor(R,f)`
   (implicit extension for non-linear AC patterns; x = the first
   duplicated element). In `and(x,t)`, `and(x,f)`, `xor(x,f)`,
   `and(x,xor(y,z))` x is the whole rest; y = first element of the xor, z
   its rest. A variable bound to a rest is renormalised through the rules
   of its symbol before use.
3. `v1+v2 % 1000000` = `v1 + (v2 % 1000000)` (checked: this gives the
   expected values; `(v1+v2) % 1000000` would give 11074 for go(50)).
4. No small-integer cache, no memoisation of normal forms; unused pattern
   variables are not built (occursFib/result do not build S).

## Deviations from the brief / caveats

- Nested right-hand sides of one AC symbol (`S U Fib[..] U Fib[..]`) are
  flattened at compile time into one `ac_build` with 3-4 items, instead
  of nested binary `U` calls (a compiler knows the symbol is AC).
- propc q1..q3 are nested C calls; C leaves argument evaluation order
  unspecified (Clang evaluates left to right here, which matches the Go
  step counts). A real generator should emit temporaries in a fixed order.
- `det_queens_strat` and `generate_list` are emitted but unused.
- Recursion: `iterate*` and the queens recursion use the C stack (depth
  ~ N); fine for these sizes.

## Difficulties in C

- CPS without closures: every continuation point after a `where` is a
  separate C function plus an env struct; nested envs point to their
  parent (`queens_n_env2 -> up`). Mechanical but verbose; easy to emit.
- Weak hash-consing with Boehm needs the disappearing-link API and care
  with tombstones (the collector writes 0 into a slot, so "empty" must be
  a different value) and with ids changing for rebuilt terms.
- `/usr/bin/time` + zsh word splitting: measure with `measure.sh` (bash).
