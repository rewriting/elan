# S7b — Strategies in the Go compiler: Implementation Plan

> Delegated tasks, verified by the controller; one whole-branch review at the end.

**Spec:** `docs/superpowers/specs/2026-10-11-s7-go-compiler-design.md` (S7b in §5; G4). **Design reference:** S6b (`docs/superpowers/plans/2026-10-10-s6b-strategies.md`, the elanc-rs code in `src/compiler-rs/elanc-rs`, its review findings and fixes), whose semantics decisions were checked against the interpreter and are reused as they are.

## Global Constraints
- Branch `s7b-go-strategies`. Go >= 1.24, standard library only, offline.
- Acceptance = the S6b tests with `ELANC=elanc-go`: `test_strategies.sh` (37 programs) and `-d strategy_terms` (95), `test_depth.sh` (all programs that elanc-rs passes, marked `compilers elanc-rs elanc-go`), `bench_parity.py --kind G` (cases leave UNSUPPORTED, none worse; the R figures: PASS=36, PASS~=1, PASS≈=27), nqueens against REM and the spike figures.
- Same semantics as elanc-rs where it agrees with the interpreter; where elanc-rs is wrong (docs/s6-rust-status.md), elanc-go follows the interpreter.
- `make check` green at every commit.

## Review Focus
As S6b's: order and multiplicity of results (dk, dc = all results of the first rule with a result, one/ONE/DC, repeat*, iterate*, where := (S) backtracking); stop semantics (no leak from abandoned branches; ONE under DC); tail-call loops only where result-equivalent; depth guard on every strategy path (a Go stack overflow must never happen); strategy terms through the compiled `eval` of strat[X] and the interpreter's generated [LAB]/[DSTR]/[FSYM] rules; call "name":X sort-aware lookup and META; refusals for traversals, normin/normout, choose/try, labels by string.

### Task 1: Labelled rules, elementary strategies, loops, guards
- Labelled rules as CPS functions (`func(t T, k rt.K[T], first bool) bool`), dk/dc/one over rules and over strategies, sequences, repeat*, iterate*, named and recursive strategies, `start with (S)`, `where := (S)` with backtracking, conditions after them; "at most one result" and totality analyses, tail calls and deterministic repeat*/iterate* as loops; `rt.Guard` at strategy/label entry.
- Tests: `ELANC=elanc-go tests/compiler-rs/test_strategies.sh` (37), `test_depth.sh` with the strategy programs enabled for elanc-go, nqueens 8/10/11/12 (times vs REM, elanc-rs and RESULTS.md).
### Task 2: Strategy terms, call "name":X, META
- strat[X] terms via the compiled `eval`, defined strategies with parameters, labels/defined strategies as constructors, `call "name":X` (export index read back, sort-aware lookup as meta.cc after 93ec493), META (set_of, meta_apply), Strategy[X] terms.
- Tests: `ELANC=elanc-go tests/compiler-rs/test_strategies.sh -d strategy_terms` (95).
### Task 3: Bench, review
- `bench_parity.py --kind G --save` (the S6b cases leave UNSUPPORTED); whole-branch review; fixes; merge.
