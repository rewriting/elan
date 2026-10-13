# Overlay of legacy/ (spec S9)

Tests added to the ELAN 3 applications and contributions without touching
`legacy/`. `elan3/<application path>/` mirrors `legacy/elan3/<application
path>/` and holds:

- `tst_all.overlay`: lines in the format of the historical `tst_all` files:
  `itest LGI SPC INP OUT` (interpreter), `aitest ...` (export/import),
  `gtest LGI SPC INP OUT` (elanc-go, the compiled program reading INP; give
  OUT its own name, e.g. `prog-c`: a compiled program does not print as the
  interpreter);
- `SAMPLES/INP.inp`: the queries; `SAMPLES/OUT.out`: the expected output,
  written by `make_expected.py` with the 2004 reference (never by hand);
- ported files, which replace the legacy file of the same name in the test's
  private copy, and `PORTING.md`, which says what was changed and why.

The legacy bench (`tests/legacy-bench/run_tests.py`) runs these tests with
the historical ones; an overlay test must PASS unless `baseline.tsv` lists it.

Scripts: `make_expected.py` (expected outputs, needs the reference:
`make reference`, see `reference/README.md`), `check_coverage.py` (in
`make check`: every program of `legacy/elan3/{applications,contributions}`
is tested, in `unloadable.tsv` or in `todo.tsv`), `triage.py` (load status of
the programs of `todo.tsv` with the reference and the modern system, written
to `triage.tsv`). Unit tests: `python3 test_overlay.py`.
