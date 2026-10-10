# S6a — Rust compiler foundations: Implementation Plan

> Inline execution with delegated tasks; one whole-branch review at the end.

**Spec:** `docs/superpowers/specs/2026-10-10-s6-rust-compiler-design.md` (S6a in §5).

## Global Constraints
- Branch `s6a-rust-foundations`. Rust 1.74, std only, `cargo --offline`; no crates.
- Nothing in the existing chain changes (`elan`, `elanc`, REM, C runtime); `make check` stays 0/0.
- Rust parts are optional: if cargo is not found, CMake/Makefile skip them with a message.
- `legacy/`, `reference/` untouched. Commit messages end with the Co-Authored-By line.

## Review Focus
1. The `.ref` reader must accept every export of the bench (all J/JO programs, the 27 goldens) and print it back identically (modulo whitespace): any field misread is a semantic bug later.
2. Normalisation order: innermost, rule order = source order, as REM; results must equal the interpreter's on the dedicated programs.
3. Builtins (int overflow: the interpreter and REM print the low 32 bits; strings; bool).
4. Deep terms: building and dropping a list of 10^6 elements must not overflow the stack.
5. Generated crates must build offline with a shared target directory; the build time per program is reported.

### Task 1: Workspace and build integration
- [ ] `src/compiler-rs/Cargo.toml` (workspace: `elan-ref`, `elanc-rs`, `elan-runtime`), `rust-toolchain` not pinned (1.74 minimum documented).
- [ ] CMake: `find_program(CARGO cargo)`; target building `elanc-rs` (release) into the build tree, installing `bin/elanc-rs` and the runtime sources into `share/elan-rs/runtime`; `ELAN_RUST=OFF` or missing cargo skips.
- [ ] Makefile: `check-rust` (cargo test --offline of the workspace) included in `check` when available.
- [ ] Commit.

### Task 2: `.ref` reader (`elan-ref`)
- [ ] Lexer and recursive-descent parser for all sections (`docs/ref-format.md`; REM `REFParser.jj` and `src/interpreter/ref/ref.cc` are the references): tables, grammars, RULE/SWRULE with conditions/wheres/choose, STRATEGY, QUERY; typed model.
- [ ] Printer back to `.ref`; test: for the 27 goldens (`tests/golden/expected/*.cref`) and the `--cexport` of every J/JO bench program, parse → print == original modulo whitespace. A script `tests/compiler-rs/test_ref_roundtrip.sh PREFIX` produces the exports.
- [ ] Commit.

### Task 3: Term representation and memory (D2, D5)
- [ ] Record D2 in the spec from the `spikes/codegen/rust-enum` results.
- [ ] Runtime core with unit tests: construction, sharing/equality, printing, iterative drop of deep terms, memory released (a loop building and dropping terms stays at constant memory).
- [ ] Commit.

### Task 4: IR, emission, driver (free operators, unlabelled rules, builtins, `-noInput`)
- [ ] IR from the model; Rust emission; `elanc-rs` driver (`elan --cexport`, generate the crate, `cargo build --release --offline` with a shared target dir, copy `a.out`).
- [ ] Dedicated programs `tests/compiler-rs/programs/` (normalisation, builtins int/bool/string, overloading, sorts with injections, deep terms) compared with the interpreter by `tests/compiler-rs/test_normalise.sh PREFIX`.
- [ ] Commit.

### Task 5: Bench subset
- [ ] `run_tests.py`: kind `R` (elanc-rs, same commands as J, `-noInput` cases only for now), compared with the J snapshots modulo the `rewrite_step` line; report how many J cases pass; no baseline entry required yet.
- [ ] Commit; whole-branch review; fixes; merge.
