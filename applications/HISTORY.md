# History of applications/

Dated log of the changes of the living copy, with their reason.

## 2026-10-13: the copy

Copied from `legacy/elan3/applications` (source commit a753be4, copy commit
c036355) by `tests/apps/import_legacy.py`. Left out: `CVS/`, build products
(`*.o`, `a.out`, `.elan.*/`, `.elan-go.*/`, `*.make`, `*.ref`), the csh
`tst_all` and `tst_one` scripts (replaced by `TESTS`, below) and `PORTING.md`
(replaced by this file).

## 2026-10-13: Robot, case variants renamed

`Robot/` held two variants of a program whose files differ only by case
(stored as `x-case-*` in `legacy/`, see `CASE-RENAMES.tsv`). The capitalised
variants are renamed, so that the copy works on case-insensitive file
systems and has no `x-case-` file: `Nim` -> `nim2`, `Robot` -> `robot2`,
`Space` -> `space2`, `State` -> `state2` (files, module and LPL names,
imports, e.g. `Quick.eln` importing `State` now imports `state2`; test lines).

## 2026-10-13: TESTS

Each directory's csh `tst_all` is replaced by a plain `TESTS` file (same tests,
same files; format in `tests/apps/README.md`). Test ids became
`applications/X::KIND::lgi:spc:inp:out`; the bench baseline and snapshots were
migrated mechanically (same statuses).

## 2026-10-13: Features/toi_meme tests (S9a)

First new tests of the copy: `toi_meme` (`itest`, `aitest` with
`toi_meme-i.out`; `gtest` with `toi_meme-c.out`, a compiled program does not
print as the interpreter), expected outputs produced by the 2004 reference.
