# The Rust compiler (S6): status and remaining work

Frozen on 2026-10-11 at the end of S6b, when the user chose Go for the
compiler, the runtime and the generated code (spec
`docs/superpowers/specs/2026-10-11-s7-go-compiler-design.md`). The code stays
in `src/compiler-rs/` (Cargo workspace: `elan-ref`, `elan-runtime`,
`elanc-rs`), built when cargo is found (`ELAN_RUST=ON`) and tested by
`make check-rust`; it is the second implementation the Go compiler is
compared with.

## Done

* **S6a, foundations** (spec `2026-10-10-s6-rust-compiler-design.md`, plan
  `2026-10-10-s6a-rust-foundations.md`): `.ref` reader and printer (round
  trip of the 54 goldens and of the 145 exportable bench programs);
  runtime with typed terms (one Rust enum per sort, reference counting,
  weak hash-consing, iterative drop, AC multisets, CPS combinators,
  builtins with the interpreter's semantics, byte strings); IR, decision
  trees and Rust emission for normalisation; the `elanc-rs` driver.
* **S6b, strategies** (plan `2026-10-10-s6b-strategies.md`): labelled
  rules as CPS functions, dk/dc/one over rules and strategies, sequences,
  repeat*, iterate*, named and recursive strategies, `where` with
  strategies (backtracking), strategy terms through the compiled `eval` of
  `strat[X]`, `call "name":X` and META (set_of, meta_apply) with the
  sort-aware lookup, a stack guard (exit status 3) and loops for
  deterministic and tail-recursive strategies.
* Tests (`tests/compiler-rs/`): `test_ref_roundtrip.sh`, `test_normalise.sh`
  (20 programs), `test_strategies.sh` (35) and `-d strategy_terms` (93),
  `test_depth.sh` (17), `bench_parity.py` (kind R of the bench: 64 cases
  compiled, same status as `elanc`).
* Measured: nqueens 11 in 0.067 s (REM: 19.8 s), 1-2 MB.

## Remaining (if the Rust compiler is resumed)

* **S6c, AC matching**: plan `docs/superpowers/plans/2026-10-11-s6c-ac.md`
  (canonical form, common and general matching, performance on efib and
  propc); 70 bench cases are refused as S6c today.
* **S6d, completeness**: query input (Earley parsing of the `.ref`
  grammars), traversals (tall/tone/tsome/rewrite), normin/normout, choose/try,
  builtin identifiers and the remaining builtin codes (16, 191), labels
  named by strings in Meta_strat; 11 bench cases are refused as S6d.
* **S6e, performance and switch**: benchmarks, compile time of large
  generated crates (BenchThesis), AC speed (the typed representation was
  1.4x slower than an arena on propc: lookup before build, inline short
  multisets).
* Deferred review notes: non-tail recursion still uses the 1 GiB stack
  (exit 3 beyond); mutual tail recursion is not a loop; the order of AC
  arguments follows creation ids (D5) and must not reach the output.
* Known bugs found by the review of the Go compiler (S7a), not fixed:
  - a builtin operation declared on a sort of terms (`@ pl @ : (t t) t
    code 3;`, the interpreter leaves `a pl a`) gives Rust that does not
    compile (cargo build fails, exit 1, instead of a refusal with exit 2);
    tests/compiler-rs/programs/refused/builtinterms.no-elanc-rs skips the
    test for elanc-rs (elanc-go refuses it, S7d).
