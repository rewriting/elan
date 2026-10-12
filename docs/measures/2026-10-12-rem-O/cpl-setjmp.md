# cpl: `_setjmp`/`_longjmp` on macOS, and where REM's time goes (2026-10-12)

Branch `cpl-perf`. On macOS `setjmp` saves and `longjmp` restores the signal
mask (a system call each); cpl used them for every choice point, local or
global, and every failure. `choice.h` now uses `_setjmp`/`_longjmp` on
Apple platforms (`CPL_SETJMP`/`CPL_LONGJMP`; glibc's `setjmp` already keeps
the mask, Linux is unchanged). The runtime never changes the mask.

Checks: `make check-compiler` (BenchThesis J/JO 64 ok; legacy bench J/JO 0
regressions, 0 snapshot differences); same `rewrite_step` counts in every
measured case.

## Run times, REM before → after (same machine, same session)

Bench (`bench_jg.py`, ms, best of 15):

| program | J | J `-O` | G | J `-O` / G after |
|---|---|---|---|---|
| propc2 | 1335 → 792 | 244 → 249 | 81 | 3.1x |
| bool3 | 479 → 271 | 98 → 100 | 48 | 2.1x |
| nqueens_jc | 159 → 21 | 14.4 → 8.1 | 4.2 | 1.9x |
| queens_jc | 108 → 14.4 | 12.3 → 5.9 | 3.8 | 1.5x |
| manySortedSet3 | 89 → 43 | 50 → 14.6 | 7.6 | 1.9x |
| manySortedSet2 | 54 → 27 | 30 → 10.5 | 6.4 | 1.6x |
| propc1 | 55 → 25 | 11.4 → 11.6 | 7.4 | 1.6x |
| nhorse_jc | 26 → 17 | 15.9 → 15.5 | 3.8 | 4.1x |

Spike programs (`spike_rem.sh`, s):

| case | J | J `-O` | G |
|---|---|---|---|
| nqueens 12 | 157 → 18.0 | 7.83 → 3.29 | 0.70 |
| efib 200 | 208 → 148 | 204 → 143 | 0.07 |
| propc 3 | 8.55 → 6.61 | 1.31 → 1.32 | 0.28 |

## Where the time goes now (`-DCPL_STATS`, `sample`)

`-DCPL_STATS` (not in the installed library) counts global choice points,
global fails and bytes copied, and how many 4 KiB chunks of each copy equal
the previous live global snapshot:

| program | global CPs | bytes copied (mean) | equal 4 KiB chunks |
|---|---|---|---|
| propc2 J | 1 423 278 | 36 GB (25 KB) | 13 % |
| propc2 J `-O` | 1 | 248 B | – |
| nqueens 10 J | 18 724 340 | 29 GB (1.6 KB) | – |
| nqueens 10 J `-O` | 348 152 | 0.5 GB (1.4 KB) | – |
| efib 100 J | 9 181 484 | 13 GB (1.5 KB) | – |
| efib 100 J `-O` | 1 | 248 B | – |

Profiles with `-O` (`sample`, after the change): efib 100 63 %
`substitution_build` (C AC matcher), ~30 % Boehm GC; propc 3
`extract_xy_from_pe` (AC matcher) first, GC ~25 %; nqueens 12 GC and
allocation ~45 %, cpl (`memmove`, `setChoicePoint`) ~10 %.

So with `-O` the stack copying no longer dominates: sharing chunks between
snapshots (snapshots of 1.4 KB) or starting the copy below `main` (the
fixed part is 248 bytes) would gain a few percent on nqueens only, nothing
on propc or efib. The remaining costs are the AC matcher and the GC.
`GC_INITIAL_HEAP_SIZE=256M` gains 6-23 % (nqueens 12 3.29 → 3.09 s,
propc 3 1.32 → 1.02 s, efib 100 4.60 → 3.87 s, propc2 0.24 → 0.20 s) for
more memory; `GC_FREE_SPACE_DIVISOR=1` on top loses it again. Not applied.
