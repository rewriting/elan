# elanc-go against REM with its choice-point optimisation (2026-10-12)

The speed-ups of elanc-go (and of the spike, S6, S7) over REM + cpl were
all measured with REM's default, **without** `-optimiseChoicePoint`
(bench kind J; `-O` = `-optimiseChoicePoint`, `-O2` also removes the
`rewrite_step` counter). With it, REM sets local choice points (a `setjmp`)
instead of global ones (a copy of the whole C stack) where its
non-determinism analysis allows. This note gives the figures again with
`-O`; they replace the older ones wherever a document compares elanc-go
with REM.

Machine: Apple Silicon, macOS, idle; `main` d5ee45e; cpl as in 2004
(stack copying, `setjmp` saving the signal mask on macOS).

## Bench (`bench_jg.py`, 145 cases built by both, best of 15 interleaved runs)

`python3 tests/compiler-rs/bench_jg.py build/install --rem-opts=-O --tsv jg-O.tsv`
and the same without `--rem-opts` (`jg-noO.tsv`). Same exit status and same
`rewrite_step` count for J, J `-O` and G in all 145 cases (the optimisation
does not change the steps). Cases above 20 ms (ms; the program exists in
several `Compiler.*/Test` versions, with the same times):

| program | J | J `-O` | G | J / J `-O` | J / G | **J `-O` / G** | RSS J `-O` / G (MB) |
|---|---|---|---|---|---|---|---|
| propc2 | 1324-1335 | 238-244 | 77-81 | 5.5x | 17x | **3.1x** | 12.3 / 40.4 |
| bool3 | 470-479 | 96-98 | 45-47 | 4.9x | 10.5x | **2.1x** | 21.0 / 29.2 |
| nqueens_jc (elan3 Simplest) | 159 | 14.4 | 4.2 | 11x | 38x | **3.4x** | 7.4 / 8.1 |
| queens_jc (elan3 Simplest) | 108 | 12.3 | 4.0 | 8.7x | 27x | **3.1x** | 7.4 / 7.4 |
| manySortedSet3 | 89 | 50 | 7.5 | 1.8x | 12x | **6.7x** | 10.7 / 13.8 |
| manySortedSet2 | 54 | 30 | 6.2 | 1.8x | 8.7x | **4.8x** | 9.5 / 11.5 |
| propc1 | 55 | 11.3 | 7.0 | 4.9x | 7.9x | **1.6x** | 8.6 / 8.6 |
| nhorse_jc (elan3 Simplest) | 26 | 15.9 | 3.9 | 1.6x | 6.7x | **4.1x** | 7.3 / 7.4 |

The other 124 cases run in the time of a process start (median J 3.4 ms,
J `-O` 3.5 ms, G 3.2 ms). G is never slower than J `-O` beyond the noise
(max(1 ms, 5 %)).

## Spike programs (`spike_rem.sh`, best of 3 runs, s; one run above 10 s)

| case | J | J `-O` | G | J `-O` / G | RSS J `-O` / G (MB) |
|---|---|---|---|---|---|
| nqueens 10 | 4.41 | 0.24 | 0.02 | 12x | 24 / 9 |
| nqueens 11 | 25.7 | 1.34 | 0.12 | 11x | 80 / 9 |
| nqueens 12 | 157 | 7.83 | 0.70 | 11x | 281 / 9 |
| efib 100 | 9.01 | 8.64 | 0.01 | ~900x | 8 / 20 |
| efib 150 | 56.8 | 56.6 | 0.03 | ~1900x | 8.5 / 38 |
| efib 200 | 208 | 204 | 0.07 | ~2900x | 8 / 39 |
| propc 1 | 0.05 | 0.01 | 0.01 | ~1x | 8.6 / 11 |
| propc 2 | 1.37 | 0.24 | 0.07 | 3.4x | 12 / 40 |
| propc 3 | 8.55 | 1.31 | 0.28 | 4.7x | 32 / 43 |

Same `rewrite_step` counts everywhere. `-O` does not help efib: its cost
in REM is not in the choice points, and the steps are equal, so it lies in
the work per step (not analysed here; the spike lists hash-consing and the
early pruning of AC conditions). nqueens with `-O` keeps a growing memory
(281 MB at 12, the backtrail).

## BenchThesis FastCompletion (`elanc-rem [-O] ans SPEC`, best of 3, s)

| spec | J | J `-O` | G | J `-O` / G |
|---|---|---|---|---|
| p8 | 6.68 | 1.33 | 0.81 | 1.6x |
| furtin | 0.47 | 0.09 | 0.04 | ~2x |
| taussky | 0.27 | 0.06 | 0.03 | ~2x |

All 66 BenchThesis cases (J, JO, G) pass (`test_benchthesis.sh`).

## Reading

- Against REM with `-O`, elanc-go is **1.6-7x faster** on the bench
  programs that run longer than a process start (not 4-14x), 1.6x on
  BenchThesis p8 (not 5x), 3-12x on the spike's nqueens and propc (not
  30-100x); efib remains a gap of three orders of magnitude, which `-O`
  does not touch. (The S7e plan measured J/G at 4-14x with an older
  elanc-go; the same binaries today give J/G 7-38x without `-O`.)
- Most of the old gap on search programs was the stack copying of global
  choice points: `-O` alone gains 5-11x on propc2, bool3, nqueens.
- Memory: elanc-go still uses more on small programs (the Go runtime), less
  on nqueens (REM's backtrail).
