# Applications and contributions (spec S9 §8)

`applications/` and `contributions/` at the repository root are the living
copy of `legacy/elan3/applications` and `legacy/elan3/contributions`
(copied once on 2026-10-13; `legacy/` stays untouched, as an archive). They
are edited, tested and extended in place. Each directory with tests holds:

- `TESTS`: one test per line, `[long] <cmd> LGI SPC INP OUT [flags...]`,
  with `cmd` one of `itest` (interpreter), `aitest` (export/import), `jtest`
  (`jtest LGI SPC OUT [flags]`, compiled, no INP) and `gtest` (elanc-go, the
  compiled program reading INP; give OUT its own name, e.g. `prog-c`: a
  compiled program does not print as the interpreter); `#` starts a comment;
  an unknown command is an error;
- `SAMPLES/INP.inp`: the queries; `SAMPLES/OUT.out`: the expected output.

The bench (`tests/legacy-bench/run_tests.py`) runs these tests with the
historical ones; a test of the copy must PASS unless `baseline.tsv` lists it
(`gtest` and R cases may also be UNSUPPORTED: the compiler refuses the program).

## Adding a test

1. Write `SAMPLES/<name>.inp`.
2. Add the line to the directory's `TESTS`.
3. `python3 tests/apps/make_expected.py --filter 'applications/X::'` writes
   the missing `OUT.out` (in `SAMPLES/` or `Sample/` when the directory has
   one, else in the directory itself: where the bench looks for `.inp` and
   `.out`) with the 2004 reference (`make reference`, see
   `reference/README.md`); read it before trusting it.
4. Remove the program from `todo.tsv`.

## Lists

- `todo.tsv`: programs not yet tested (the list shrinks);
- `unloadable.tsv`: programs that cannot be tested, with the reason;
- `triage.tsv`: load status of the programs of `todo.tsv` with the reference
  and the modern system, written by `triage.py` (a report, not a test).

`check_coverage.py` fails on a program (.lgi) that is in none of the states
tested / unloadable / to do, or in two.

## Checks

`make check-apps` runs `test_import.py`, `test_tools.py` (unit tests) and
`check_coverage.py`. `import_legacy.py` and `migrate_ids.py` were run once
(2026-10-13) to create the copy and migrate the ids; they are kept as
history.

## Known problems found by the triage

Found while testing the programs of the copy against the 2004 reference
(S9a Task 7):

- `S28` and `S28LOG` take more than 20 s with the reference.

Found in S9b (programs loaded by the reference, still untested, kept in
`todo.tsv`):

- `APT/S28`, `APT/S28LOG`, `APT/S28LIN`: the start term (`new(0)` searched
  by the strategy of the `.lgi`) does all the work and the query cannot make
  it smaller (every placement needs all 27 cells); the reference needs more
  than 60 s (the modern interpreter about 20 s).
- `APT/bubleLIN`: more than 20 s with the reference (100 elements sorted in
  a linear array, fixed by the start term).
- `APT/bubleLOG`: tested by `itest` only: the import of its exported `.ref`
  fails with both systems (syntax error on `tone`), and the reference REM
  build crashes (no `gtest`).
- `Features/con`: hangs with both systems, without output (`dkconcur`,
  `oneconcur` of the start strategy).
- `Features/re`: segfaults with both systems on every query (the rule calls
  `printnl`); `Features/hello` too (`print`) on `hello`: its test only
  queries a string.
- The reference REM build of `bubleLOG` crashes (Boehm GC out of memory,
  rc=-11).
- The reference interpreter segfaults on `Features/hello` (rc=139).
- The historical `.inp` files of `Features/mc` and `Features/re` belong to
  other programs (syntax error / mismatched output).
- The historical `Features/SAMPLES/toi_meme.out` is the output of a ctest
  (compiled) run, hence the two tests `toi_meme-i.out` (interpreter) and
  `toi_meme-c.out` (compiled).
