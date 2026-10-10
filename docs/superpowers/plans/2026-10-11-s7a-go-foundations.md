# S7a — Go compiler foundations: Implementation Plan

> Delegated tasks, verified by the controller; one whole-branch review at the end.

**Spec:** `docs/superpowers/specs/2026-10-11-s7-go-compiler-design.md` (S7a in §5; G1-G9). G2 decided by `spikes/codegen/go-repr/` (NOTES.md): generic nodes with native builtins.

## Global Constraints
- Branch `s7a-go-foundations`. Go >= 1.24, standard library only, offline (`GOTOOLCHAIN=local`, `GOFLAGS=-mod=mod`, no network). Module `src/compiler-go` (module path `elan`).
- Nothing in `elan`, `elanc`, REM, `elanc-rs` changes (bugs found there: own commit, regression case, CONTRIBUTING rule 4).
- The test scripts of `tests/compiler-rs` become compiler-independent (`ELANC=elanc-rs|elanc-go`, default elanc-rs) without weakening them; `make check` green at every commit.
- Generated programs: same command line, output format and exit statuses as elanc-rs (0 ok, 1 run-time error, 2 unsupported construct, 3 recursion too deep). A Go stack overflow (fatal, exit 2) must never happen: the depth guard stops before it.

## Decisions from the G2 spike (recorded in the spec by Task 1)
- Terms: one node type `{Sym int32; Int int32; Args ...}` (arguments of arity <= 3 inline), maximal sharing through a weak unique table (`weak.Pointer`; before growing the table past 64K slots, run a GC if none completed since the last rebuild); small ints preallocated; builtins (`int` as `int32` with 32-bit wrap, `bool`, byte strings) native in generated functions wherever the sort has no stuck terms (totality analysis as S6a), boxed only when stored in a node.
- AC arguments ordered by a **history-independent** total order (structural: hash then structural comparison), never by creation id: outputs and step counts must not depend on GC timing.
- GC: the program sets `debug.SetGCPercent(400)` at start (overridable by `GOGC`); measured.
- Depth: the program runs on a goroutine with `debug.SetMaxStack(2e9)` (1 GiB usable), a depth guard at strategy and recursive-operator entry calibrated to stop before (exit 3, message as elanc-rs); the printer is iterative on right-recursive structures (lists).

## Review Focus
1. Semantics parity with elanc-rs and the interpreter on every S6a test (normalisation order, non-linear variables, conditions, where, builtins: wrap, x/0, strcmp sign, byte strings, NUL truncation, C printer spacing).
2. Weak table correctness: no lost sharing that breaks pointer equality (two live equal terms must be the same pointer), no resurrection races with the GC, bounded memory on long loops.
3. AC order independent of history (two runs, different GOGC, same output and steps).
4. Depth: deep normalisation and deep printing never crash with Go's fatal stack overflow.
5. Build integration: Go missing or too old (clear message, Go parts skipped), CI with setup-go, shared GOCACHE for generated programs, no network.

### Task 1: Module, build integration, spec update
- `src/compiler-go/go.mod` (go 1.24), packages `ref`, `ir`, `gen`, `elanrt`, `cmd/elanc-go`; CMake: find `go` >= 1.24 (`ELAN_GO`, optional), build `elanc-go` into the build tree, install `bin/elanc-go` and the runtime sources in `share/elan-go/elanrt`; Makefile `check-go` (go vet + go test, then the shared scripts with `ELANC=elanc-go`) in `check`; CI: `actions/setup-go` (Go 1.24+) on both runners; spec: record G2's decisions (above).
### Task 2: `.ref` reader (port of elan-ref)
- Lexer, parser, typed model, printer; round trip of the goldens (`tests/golden/expected/*`) and of the bench exports (`tests/compiler-rs/test_ref_roundtrip.sh` made compiler-independent or a Go twin).
### Task 3: Runtime core (`elanrt`)
- Terms, weak unique table, AC canonical order, builtins with the interpreter's semantics, printing (C printer quirks), driver (options, `result =`, `rewrite_step`, exit statuses, output flushed before exits), depth guard, GC setting; unit tests ported from `elan-runtime/tests` (sharing, collection of unused terms with bounded table, deep lists built/printed, builtins, AC order stable across GOGC values).
### Task 4: IR, emission, driver (normalisation, builtins, `-noInput`)
- IR from the model (the S6 IR is the reference design: sorts, families of builtins, totality, decision trees), Go emission, `elanc-go` driver (`elan --cexport` as elanc.src, REM options ignored, `go build` with a shared GOCACHE, `-o`, `-v`, exit statuses, refusals with exit 2 for S7b/S7c/S7d constructs).
- `test_normalise.sh` with `ELANC=elanc-go`: 20 programs + refusals, same results as with elanc-rs.
### Task 5: Bench
- `run_tests.py` kind `G` (as `R`), `bench_parity` for G with its own expected file; the S6a cases (35 then) at the same status as J.
