# History of contributions/

Dated log of the changes of the living copy, with their reason.

## 2026-10-13: the copy

Copied from `legacy/elan3/contributions` (source commit a753be4, copy commit
c036355) by `tests/apps/import_legacy.py`. Left out: `CVS/`, build products
(`*.o`, `a.out`, `.elan.*/`, `.elan-go.*/`, `*.make`, `*.ref`), the csh
`tst_all` and `tst_one` scripts and `PORTING.md`.

## 2026-10-13: TESTS

Each directory's csh `tst_all` is replaced by a plain `TESTS` file (same tests,
same files; format in `tests/apps/README.md`). Test ids became
`contributions/X::KIND::lgi:spc:inp:out`; the bench baseline and snapshots were
migrated mechanically (same statuses).
