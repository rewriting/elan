# Spike results: target language of the generated code

Brief: `BRIEF.md`. Code: `c/`, `rust/`, `go/` (throwaway, hand-written as a
compiler would emit it; see each `NOTES.md`). Measurements: `measure.sh`
(best of 5 runs, Apple Silicon, 2026-10-10), raw data `results.tsv`.

All versions give the oracle's results (nqueens solutions in the same order
as the current compiler; efib(50,100,150,200) = 11011074, 35084101,
60386349, 83233826; propc q1..q3 = t) and the same rewrite step counts:
the three languages do the same work with the same model (hash-consed
terms, flattened AC, success continuations, no stack copying).

## Run time (s) and peak memory (MB)

| Case | REM + cpl (today) | C, Boehm GC | C, arena | Rust (arena) | Go | Go, 1 core |
|---|---|---|---|---|---|---|
| nqueens 11 | 22.2 | 0.14 / 5 | 0.13 / 18 | 0.14 / 10 | 0.20 / 23 | 0.20 / 22 |
| nqueens 12 | – | 0.84 / 5 | 0.84 / 69 | 0.85 / 40 | 1.20 / 87 | 1.27 / 85 |
| efib 100 | 9.1 | 0.01 / 4 | <0.01 / 9 | <0.01 / 8 | 0.01 / 21 | 0.01 / 21 |
| efib 200 | ~140 (2000) | 0.06 / 4 | 0.05 / 57 | 0.05 / 31 | 0.07 / 108 | 0.15 / 126 |
| propc 2 | 1.49 | 0.07 / 7 | 0.06 / 49 | 0.05 / 25 | 0.13 / 32 | 0.23 / 35 |
| propc 3 | 9.7 | 0.52 / 16 | 0.33 / 227 | 0.29 / 90 | 0.64 (user 2.1) / 130 | 1.32 / 182 |

Clean builds: C 0.6 s (6 binaries), Rust 0.26 s (3 binaries, release),
Go 1.3 s with a cold cache (standard library included), 0.09 s warm.

## Reading

- The model, not the language, explains the gap with today's compiler
  (30 to 150 times faster): no C stack copying, cheap continuations, early
  pruning of AC conditions.
- C and Rust are equal on time (within 10 %). Rust's arena is denser than
  C's (90 vs 227 MB on propc 3).
- Go is 1.4x (nqueens) to 2-4.5x (propc, one core) slower, and uses the most
  memory: closures allocated per continuation, GC work.
- Memory management is the open question for C and Rust: the arenas never
  free. Boehm with a weak hash-consing table frees (propc 3: 16 MB) for
  +60 % time; a Rust runtime will need its own collector (mark and sweep
  of the hash-consing table from the continuation roots).
- Compile time of generated Rust is not a problem at this size (0.26 s);
  to be checked on a large program (BenchThesis) once a generator exists.
- Code shape (NOTES.md): C compact (argument evaluation order must be
  controlled); Rust: explicit `rt` context, one `let` per subterm, nested
  closures for `where`, compiled at the first attempt; Go: the most direct,
  every continuation a heap closure.
