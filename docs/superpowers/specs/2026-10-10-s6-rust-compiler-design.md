# S6 — An ELAN compiler in Rust (compiler, runtime and generated code)

Date: 2026-10-10 · Status: approved in principle by the user ("tu peux écrire
la spec et commencer"); details decided autonomously, recorded here.

## 1. Context and decision

The current compiler is a chain: `elan --cexport` writes the program as a
`.ref` file, REM (Java, 18k lines, 1998-2003) generates C, which is linked
with the C runtime, libearley (query parsing) and cpl (backtracking by
copying the C stack). It works on the bench (410 J/JO tests, BenchThesis)
but its less-used paths had rotted, cpl is the main technical risk, and REM
is hard to evolve (`docs/followups.md`, review of September-October 2026).

The spike `spikes/codegen/` (RESULTS.md) compared the generated code in C,
Rust and Go with one execution model (shared terms, flattened AC, success
continuations, no stack copying): C and Rust equal, Go 1.4-4.5x slower, all
30-150x faster than REM + cpl. The user chose **Rust for the compiler, the
runtime and the generated code**, with ELAN terms represented by Rust
algebraic data types. The three hard problems, named by the user: the
**strategies**, **AC matching** and **memory management**.

## 2. Goal and success criteria

A new compiler `elanc-rs` next to `elanc` (REM stays unchanged until the
switch, decided after S6):

1. `elanc-rs prog [spec]` reads the program through `elan --cexport` (the
   `.ref` contract, `docs/ref-format.md`), generates a Rust crate, builds it
   with cargo (offline) and produces a native program with the command line
   of today's compiled programs (`-noInput`, `-quiet`, query on stdin).
2. Every J/JO case of the legacy bench that passes with `elanc` gives the
   same results with `elanc-rs` (same `result = ...` lines, same order), and
   so do the 64 BenchThesis cases. The `rewrite_step` count is printed but
   not compared (it depends on the compilation scheme; the spike counts
   rule applications).
3. Compiled programs agree with the interpreter on dedicated tests per
   construct (strategies, AC, builtins, start term), as
   `tests/compiler/test_runtime_strategies.sh` does today.
4. Performance: at least as fast as `elanc` on every bench case, and within
   2x of the spike figures on nqueens, efib, propc (RESULTS.md).
5. Memory: a long-running program (a `repeat*` loop over a bounded state)
   runs in constant memory; peak memory is reported by the performance test.
6. `make check` (Clang and GCC builds of the C++ parts), the sanitizer check
   and the CI run the new tests when cargo is available; nothing in the
   existing chain changes.

## 3. Architecture

```
elan --cexport ──► prog.ref ──► elanc-rs ──► .elan-rs.prog/ (cargo crate) ──► a.out
                                 │  ref reader → model → IR → Rust emission
                                 └─ uses elan-runtime (installed crate sources)
```

Cargo workspace `src/compiler-rs/` (Rust 1.74, std only, offline):

* `elan-ref`: reader of `.ref` files into a typed **model** (sorts,
  operators with profiles, AC/assoc flags, rules with conditions and
  `where`s, strategies, the query, grammars for query parsing).
* `elanc-rs`: analysis and the **IR** (per sort: operators; per operator:
  normalisation rules; matching automata; strategies in continuation form;
  AC patterns), then emission of Rust source. The IR is independent of the
  target so that another back end (C) stays possible.
* `elan-runtime`: what every generated program links: hash-consing, AC
  multisets and matching helpers, strategy combinators and continuation
  helpers, builtins (int, bool, string, ident), printing, query parsing,
  the main program (options, statistics).

Generated crate: one Rust module per ELAN module (or per sort if a module is
large), depending on `elan-runtime` by path; built with a shared target
directory so the runtime is compiled once per installation, not per
program.

## 4. Decisions

| # | Decision | Reason |
|---|---|---|
| D1 | Input is the `.ref` export (no new parser of ELAN source). | Stable, documented, pinned by golden tests; REM reads the same file, so both compilers can run side by side. |
| D2 | **Typed terms**: each ELAN sort is a Rust `enum`, each operator a variant (injections included; a defined operator also has a variant, for terms where no rule applies); shared terms are reference-counted nodes with a global creation id, **hash-consed** in one weak table per sort (equality = pointer equality). Builtins are native (`i64`, `bool`, strings) where a sort has no other constructor; a builtin result sort that can be stuck is wrapped. Dropping is iterative (a per-thread queue in the runtime, for every sort, mutually recursive ones included: no generated drop code). Decided from `spikes/codegen/rust-enum/` (NOTES.md): versus the u32 arena, nqueens 4x faster, propc 3 15 MB instead of 90 MB (286 MB unfreed), AC 1.4x slower (table lookups and multiset copies; to optimise in S6c). | Memory is solved without a collector (shared terms are acyclic); the Rust type checker checks the sorts of the generated code; the code is close to the source. |
| D3 | Strategies: success continuations (`s(t, k)`, `k` returns "stop"), deterministic strategies (`repeat*` of `first one`, `one`, normalisation) compiled as loops; recursion depth bounded by an explicit check and a large stack for the main thread, a trampoline if the bench needs it. | Model validated by the spike (same solution order as REM). |
| D4 | AC: flattened canonical form (sorted multiset); matching code generated per pattern for the common shapes (one AC symbol at the root with a rest variable, conditions pruned as soon as their variables are bound); a general AC matcher in the runtime for the other shapes (nested AC symbols, several rest variables). | Specialised code is what made efib fast; the general case must exist for correctness. |
| D5 | Memory: reference counting (D2); the weak tables are purged when they grow; dropping deep terms is iterative (a 10^7-element list is freed on an 8 MB stack in the spike). AC multisets are ordered by creation id: deterministic for a given program and input, but it depends on which terms were alive (the spike's propc step counts differ from the arena's; results are equal). Order-independent output (e.g. printing AC terms) must not rely on it. | User's third hard problem; settled early because everything depends on it. |
| D6 | Query parsing at run time: the Earley parser of the runtime is ported to Rust and reads the grammars of the `.ref` file (S6d); until then, programs run with `-noInput` (start term) only. | Keeps the first stages small; the port is mechanical and testable against the C parser. |
| D7 | Not in scope: `-coq`, `-proofterm`, `-lib`, `-aterm`, `-debug` variants (broken or experimental today). | Recorded in docs/followups.md if needed later. |
| D8 | Tests are differential: the interpreter and REM are the oracles; every stage adds dedicated programs and runs the bench subset its constructs cover. | The method that made the revival work. |

## 5. Stages (each one a plan, a branch, a review, merged green)

* **S6a — foundations.** Workspace, CMake/Makefile integration (optional
  when cargo is missing), `elan-ref` with a round-trip test on the `.ref`
  exports of every bench program, the term representation (D2) and memory
  (D5) with runtime unit tests, the IR and emission for sorts, free
  operators, unlabelled rules (normalisation), builtins (int, bool,
  string), printing, `-noInput`. `elanc-rs` driver. Tests: dedicated
  programs vs interpreter; the bench cases that use only these constructs.
* **S6b — strategies.** Labelled rules, `dk`, `dc`, `first`, `one`, `id`,
  `fail`, `;`, `iterate*`, `repeat*`, `where` with strategies, conditions,
  defined strategies with parameters, congruences, strategy terms
  evaluated at run time (`strat[X]`, `call "name":X`). Tests: the
  run-time strategy and start-term programs, nqueens, the bench strategy
  cases.
* **S6c — AC.** Canonical form, specialised matching, general matcher,
  AC in conditions and right-hand sides. Tests: `Compiler.4.0/Test`
  AC cases, efib, propc, unit tests of the matcher (all solutions in
  order).
* **S6d — completeness.** Query parsing (Earley port), the remaining
  constructs met in the bench, statistics and options, all J/JO cases
  and BenchThesis.
* **S6e — performance and switch.** Benchmarks (spike programs, bench,
  BenchThesis), peak memory, compile time of large programs; decision to
  make `elanc` the Rust compiler.

## 6. Risks

* Compile time of large generated crates (BenchThesis): measured from S6a
  (time of `cargo build` per program), mitigations: modules, opt-level,
  shared target dir, incremental builds.
* Exactness of the `.ref` semantics (the meaning of every field): REM's
  parser (`REFParser.jj`) and `ref.cc` are the references; round-trip and
  differential tests catch misreadings.
* AC general case and strategies as terms: the largest unknowns; S6b and
  S6c start with their tests.
* Generic views of typed terms (query parsing, printing, tracing,
  strategy terms): generated per sort (a uniform view or a conversion to an
  untyped tree); the generator carries more than with untyped nodes.
* AC speed with typed terms (spike: 1.4x slower than the arena): look up a
  multiset before building it, store short multisets inline, per-sort
  allocation; measured in S6c against the spike figures.
