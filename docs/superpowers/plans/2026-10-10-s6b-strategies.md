# S6b — Strategies in the Rust compiler: Implementation Plan

> Delegated tasks, verified by the controller; one whole-branch review at the end.

**Spec:** `docs/superpowers/specs/2026-10-10-s6-rust-compiler-design.md` (S6b in §5; D3).

## Global Constraints
- Branch `s6b-strategies`. Rust 1.74, std only, offline. Nothing in `elan`/`elanc`/REM changes unless a bug is found (then: own commit, regression case, CONTRIBUTING rule 4).
- Oracle: the interpreter for semantics (result order included), `elanc` (REM) for the output format; every refusal stays explicit (exit 2), never wrong code.
- `make check` green at every commit; `bench_parity.py` may only improve (cases leave UNSUPPORTED, none gets worse); `bench_r_expected.tsv` is re-saved when cases are added.

## Review Focus
1. Order of results: `dk`, `iterate*` (0th iterate first, depth first), `repeat*` (all maximal results), `where` with a non-deterministic strategy, conditions after a non-deterministic `where` (backtracking into the `where`).
2. `dc` / `first` / `one` / `ONE` stop semantics: one alternative only, one result only, and no result leaks from an abandoned branch.
3. Strategies inside normalisation: a labelled rule never applies during normalisation; unlabelled rules normalise every result of a strategy (as the interpreter).
4. Deep strategies: `repeat*` of a deterministic step running a million times must not overflow the stack (loop or trampoline, D3); a non-deterministic `iterate*` deep chain: explicit depth check with a clear error, not a segfault.
5. Strategy terms built at run time (`strat[X]`, `call "name":X`, defined strategies with parameters `[] app(s) => s`): semantics as the interpreter (including the sort-aware `call` lookup fixed in commit 93ec493).

### Task 1: Labelled rules and elementary strategies
- Labelled rules compiled as CPS functions (`fn r_l(t, k) -> bool`), reusing the decision trees of S6a (a leaf calls the continuation; several rules with the same label: all, in source order).
- `id`, `fail`, `;`, `dk/dc/one` over rule lists and over strategies (`DK/DC/ONE`), `first` forms as exported, `repeat*`, `iterate*`, `call(n)` of named strategies (STRATEGY definitions, recursion allowed), `start with (S)`, `where x := (S) t` and `where p := (S) t` (backtracking), conditions after them.
- Tests: `tests/compiler-rs/programs/strategies/` (one construct per program, several queries via `-noInput` start terms) compared with the interpreter and REM by `tests/compiler-rs/test_strategies.sh`; runtime CPS unit tests; nqueens (spike program) result order.

### Task 2: Strategy terms evaluated at run time
- `strat[X]` terms (`elanlib/strategy/strat.eln`), defined strategies with parameters, `call "name":X` (the interpreter's sort-aware lookup), `if ... then ... orelse ... fi`, `if b then .. else .. fi`.
- Tests: `tests/compiler/programs/runtime_strategies/*` and `start_term/*` compiled with `elanc-rs` (the same programs as `test_runtime_strategies.sh`), compared with the interpreter.

### Task 3: Depth and performance
- Deterministic `repeat*`/`one` chains compiled as loops; explicit depth bound with a message for the others (D3); test: a 10^6-step `repeat*`.
- nqueens 8/10/11 via elanc-rs vs the spike figures (RESULTS.md); report.

### Task 4: Bench
- `bench_parity.py`: S6b cases leave UNSUPPORTED; re-save `bench_r_expected.tsv`; every newly compiled case PASS/PASS≈ as J.
- Whole-branch review; fixes; merge.
