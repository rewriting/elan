# Legacy test bench

`run_tests.py` replays the historical ELAN test bench — the `tst_all` files that
drove the csh scripts `itest`, `aitest` and `jtest` of
`legacy/elan/sources/Scripts` — against an installed ELAN system.

```sh
./run_tests.py                         # test reference/install (default)
./run_tests.py --prefix /path/install  # test another build (e.g. the modern port)
./run_tests.py --filter Simplest -j 4  # subset, parallelism
./run_tests.py --repeat 5              # detect non-deterministic tests (FLAKY)
./run_tests.py --list
```

It must run on a case-sensitive file system (it copies each application
directory to `work/`).

## Test kinds

| Kind | Original script | What is run | Count |
|------|-----------------|-------------|-------|
| `I`  | `itest`  | `elan -b X.lgi [Y.spc] < SAMPLES/in.inp` | 232 |
| `A`  | `aitest` | `elan --export X.ref ...` then `elan --import X.ref < in.inp` | 136 |
| `J`  | `jtest`  | `elanc -nosplit X`, `make`, `a.out -noInput -quiet` | 205 |
| `JO` | `jtest` (2nd half) | same with `-optimiseChoicePoint` | 205 |

`ctest`/`actest` (C compiler of ELAN 2), `pitest` (partial evaluator) and the
Java compiler of `Compiler.*/Test` have no driver anymore and are not replayed.

## Oracles and statuses

Two independent oracles are used.

**Historical reference** (`SAMPLES/OUT.out`, written 1997–2003):

| Status | Meaning |
|--------|---------|
| `PASS`  | identical output |
| `PASS~` | identical up to white space |
| `PASS≈` | same identifiers/numbers in the same order; only the printing differs. Reference outputs were produced with older libraries and printers: `a.b.nil` vs `a,b,nil` vs `cons_X(a,cons_X(b,nil))` (the printer uses the *last* alias declared for a symbol), `h(,)(a,b)` and `[](12)` printed by the old compiled runtime. Statistics lines (`rewrite_step`, times) are ignored. |
| `FAIL`  | different output |
| `ERROR` | export, compilation or link failed |
| `NOREF`/`NOLGI`/`NOINP` | the bench refers to a file missing from the archive |
| `FLAKY` | outputs differ between repeated runs (`--repeat`) |

**Snapshots** (`snapshots/*.out`): the exact outputs of the reference build.
They are the oracle for the modern port: `SNAP-DIFF` means the behaviour
changed. `baseline.tsv` stores the historical status of every test; the run
fails if a status gets worse (regression) or if a snapshot differs.

Refreshing them (`--save-baseline --save-snapshots`) is a deliberate act: do it
only when a behaviour change is understood and intended, and say why in the
commit message.

## State of the reference build (reference/install)

778 tests: PASS 276, PASS~ 2, PASS≈ 174, FAIL 126, ERROR 60, missing files 140;
stable over repeated runs (0 FLAKY with `--repeat 3`).
