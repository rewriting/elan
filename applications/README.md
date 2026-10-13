# applications

The living copy of `legacy/elan3/applications`, made on 2026-10-13 by
`tests/apps/import_legacy.py` from commit a753be4 (copy commit c036355).
It is edited, tested and extended in place; `legacy/` stays the archive.
Every change of a program is recorded, with its reason, in `HISTORY.md`.

## Running the tests

```sh
./tst_all              # kinds I,A of this tree (tst_all J,JO or G for the others)
make check             # from the repository root: everything, this tree included
```

`tst_all` runs the bench (`tests/legacy-bench/run_tests.py`) on the `TESTS`
files of this tree, against $ELAN_PREFIX, else `build/install`, else
`reference/install`.

## Adding a test

See `tests/apps/README.md` (format of `TESTS`, `make_expected.py`, the lists).

## Not yet tested

The programs without a test are listed in `tests/apps/todo.tsv`; those that
cannot be tested, with the reason, in `tests/apps/unloadable.tsv`.
