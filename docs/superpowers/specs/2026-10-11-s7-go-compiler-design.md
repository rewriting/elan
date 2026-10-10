# S7 — An ELAN compiler in Go (compiler, runtime and generated code)

Date: 2026-10-11 · Status: decided by the user ("je n'aime vraiment pas le
langage rust ... go correspond mieux à mon esprit ... conserve ce que tu as
fait en rust ... écris aussi la spec pour tout passer en go").

## 1. Context and decision

S6 (`2026-10-10-s6-rust-compiler-design.md`) started a compiler in Rust:
S6a (foundations) and S6b (strategies) are done and tested; S6c-S6e remain
(`docs/s6-rust-status.md`). The user, who will maintain the project, finds
Rust (and C++) hard to read and prefers Go. The Rust work is kept in the tree
(`src/compiler-rs`, frozen, still built and tested while it is useful as a
second implementation); the compiler is redone in **Go: the compiler, the
runtime and the generated code**.

Evidence (`spikes/codegen/RESULTS.md`): with the same execution model, Go is
1.4-4.5x slower than Rust/C, and 30-100x faster than the current compiler
(REM + cpl). Go brings: a garbage collector (S6's third hard problem, memory,
disappears), weak references and canonicalisation in the standard library
(`weak`, `unique`, Go >= 1.24), goroutine stacks that grow on demand, fast
compilation, one static binary per program, code that is simple to read and
to generate.

## 2. Goal and success criteria

`elanc-go` next to `elanc` and `elanc-rs` (REM stays until the switch):

1. `elanc-go prog [spec]` reads the program through `elan --cexport` (the
   `.ref` contract), generates a Go module, builds it with `go build`
   (offline, no third-party module) and produces a native program with the
   command line of today's compiled programs (`-noInput`, `-quiet`, query on
   stdin), the same output format and the same exit statuses as `elanc-rs`
   (0 ok, 1 run-time error, 2 unsupported construct, 3 recursion too deep).
2. **Parity with what S6 reached, then beyond:** every test that `elanc-rs`
   passes, `elanc-go` passes (the test scripts of `tests/compiler-rs` are
   made compiler-independent: `test_normalise`, `test_strategies` (35
   programs), `strategy_terms` (93), `test_depth`, the bench parity with its
   expected cases); then AC (S6c's plan), query input and the remaining
   constructs (S6d), all J/JO cases and BenchThesis.
3. Results equal to the interpreter's (order included) on the dedicated
   tests; output byte-identical to REM's where REM is right.
4. Performance within 2x of the Go spike figures (RESULTS.md) and at least as
   fast as `elanc` on every bench case; compile time of a generated program
   reported (target: under 2 s for bench programs with a warm build cache).
5. `make check`, `make check-sanitize` and the CI run the Go tests when Go
   >= 1.24 is available; nothing in the existing chain changes.

## 3. Architecture

```
elan --cexport ──► prog.ref ──► elanc-go ──► .elan-go.prog/ (Go module) ──► a.out
                                 │  ref reader → model → IR → Go emission
                                 └─ imports the runtime package (installed sources)
```

One Go module `src/compiler-go/` (module path `elan`, Go >= 1.24, standard
library only):

* `ref/`: reader and printer of `.ref` files into a typed model (port of
  `elan-ref`, same round-trip tests on the goldens and the bench exports).
* `ir/`, `gen/`: analysis (sorts, operators, rules, decision trees,
  strategies in continuation form, determinism and totality analyses — the
  S6 IR is the reference design) and emission of Go source.
* `runtime/` (package `elanrt`): terms, sharing, AC multisets, strategy
  combinators, builtins with the interpreter's semantics (32-bit wrap,
  `x/0 = 0`, strcmp sign, byte strings, printing quirks of the C printer),
  query parsing (S7d), the main program.
* `cmd/elanc-go/`: the driver.

Generated module: `go.mod` with a `replace` directive to the installed
runtime sources (`$PREFIX/share/elan-go/runtime`), built with a shared
`GOCACHE` so the runtime is compiled once.

## 4. Decisions

| # | Decision | Reason |
|---|---|---|
| G1 | Input is the `.ref` export (as D1). | Same contract, both compilers can be compared. |
| G2 | **Term representation, measured before the runtime task:** (a) a generic node (symbol code + arguments) with maximal sharing through a weak unique table (`weak.Pointer`) or `unique.Handle`; (b) one Go struct type per operator implementing a `Term` interface per sort, also shared. Choose by a small spike (nqueens, propc, efib: time, memory, readability of the generated code). Go has no sum types: exhaustiveness is not checked by the compiler; the differential tests carry that role. | The spike measured only (a) with a strong table; readability is the user's criterion. |
| G3 | Memory: the Go garbage collector; the unique table holds weak references (terms no longer used are collected); no reference counting, no `unsafe`. | Removes S6's third hard problem. |
| G4 | Strategies: success continuations `func(t T) bool` (true = stop), as S6 D3; deterministic strategies (`repeat*`, tail-recursive `first one`) compiled as loops (S6b task 3 rules); recursion depth: goroutine stacks grow, but a Go stack overflow is a fatal, unrecoverable crash, so the program runs on a goroutine with `debug.SetMaxStack` set high and a depth counter at strategy entry stops cleanly with status 3 before the limit. | Same semantics and guarantees as S6b, simpler mechanism. |
| G5 | AC: as S6 D4 (canonical sorted multisets, specialised matching for common shapes, a general matcher), following the S6c plan. | Unchanged design. |
| G6 | Tests are shared: the scripts of `tests/compiler-rs` take the compiler as a parameter (`ELANC=elanc-go`); the bench gets a kind `G` like `R`; a G case may not be worse than J. | No duplicated oracles; the Rust results stay a second reference. |
| G7 | The Rust compiler is kept, frozen at the end of S6b, built and tested while useful (`ELAN_RUST`); its remaining work is listed in `docs/s6-rust-status.md`. When `elanc-go` reaches parity, the Rust tests become optional (`ELAN_RUST=OFF` by default), the code stays in the tree. | The user asked to keep it. |
| G8 | Go toolchain: >= 1.24 (weak, unique); CI installs it with `actions/setup-go` (Ubuntu 24.04's golang-go is 1.22). | Standard library features needed. |
| G9 | Not in scope (as S6 D7): `-coq`, `-proofterm`, `-lib`, `-aterm`. | Broken or experimental. |

## 5. Stages (each: plan, branch, tests first, review, merged green)

* **S7a — foundations** (= S6a): module, CMake/Makefile integration
  (optional without Go), `ref` reader with the round-trip tests, the term
  representation spike (G2) and the runtime core (sharing, builtins,
  printing, driver), IR and emission for normalisation, driver `elanc-go`,
  `-noInput`. Acceptance: `test_normalise` (20 programs) and the bench
  parity of S6a's cases with `ELANC=elanc-go`.
* **S7b — strategies** (= S6b): labelled rules, elementary strategies,
  `where` with strategies, strategy terms via the compiled `eval` of
  `strat[X]`, `call "name":X`, META, depth guard and loops. Acceptance:
  `test_strategies` (35), `strategy_terms` (93), `test_depth` (17), bench
  parity of S6b's cases, nqueens against REM.
* **S7c — AC** (S6c's plan, `docs/superpowers/plans`): canonical form,
  specialised and general matching, performance on efib and propc.
* **S7d — completeness**: query input (Earley parser port or a parser
  generated from the `.ref` grammars), traversals, the remaining builtins,
  all J/JO cases, BenchThesis.
* **S7e — performance and switch**: benchmarks, compile times, the decision
  to make `elanc` the Go compiler.

## 6. Risks

* Performance of CPS closures in Go (allocation per continuation): measured
  from S7a; mitigations: loops for deterministic strategies, avoiding
  closures on hot paths, `sync.Pool` only if measured.
* No sum types: a forgotten case is a run-time panic, not a compile error;
  every generated type switch has an explicit default that panics with the
  operator's name; the differential tests and the bench catch the rest.
* Go version on CI and user machines (>= 1.24): checked by CMake with a
  clear message.
