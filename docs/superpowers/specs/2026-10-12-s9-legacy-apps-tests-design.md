# S9 — Every ELAN 3 application and contribution tested

Date: 2026-10-12 · Status: decided by the user. The request (translated from
French): "In ~/ELAN/elan3 there are lots of applications/contributions, etc.
Integrate everything into the repository and make everything
executable/testable." The design below was proposed and approved section by
section on 2026-10-12 (§7).

## 1. Context

`legacy/elan3` was imported from `~/ELAN/elan3` in S1. Checked on
2026-10-12: the 4 955 files of `~/ELAN/elan3` are all in `legacy/elan3`,
byte for byte (the repository has 25 more: the case twins that the
case-insensitive volume of `~/ELAN` had overwritten, and symbolic links).
**The integration is done; the work is the testing.**

The legacy bench (`tests/legacy-bench/run_tests.py`) replays the historical
`tst_all` files: 388 I/A tests and 410 J/JO tests today, compared with the
historical outputs, with snapshots of a trusted build and with
`baseline.tsv`. It covers 112 of the 293 programs (`.lgi`) of
`legacy/elan3/applications` and `legacy/elan3/contributions`.

## 2. Findings (main 3fd1b69)

179 programs have no test (no `tst_all` line names them):

| directory | programs | candidate `.inp` | candidate `.out` |
|---|---|---|---|
| applications/Features | 56 | 11 | 10 |
| applications/CStrategies | 21 | 16 | 7 |
| applications/MetaApply | 19 | 0 | 0 |
| applications/Strategies | 14 | 2 | 2 |
| applications/UnifElanGen | 9 | 0 | 0 |
| applications/APT | 6 | 5 | 4 |
| applications/HOU_1_17 | 6 | 0 | 0 |
| applications/Simplest | 6 | 1 | 1 |
| applications/Hou, HOU_1_18, Robot | 4 each | 0, 0, 1 | 0, 0, 1 |
| applications/UnifElanRCF | 3 | 0 | 0 |
| applications/FastCompletion | 2 | 0 | 1 |
| applications/IO_Call, Picalc, Prover, Server, Server/Firewall | 1 each | Prover 1 | Prover 1 |
| contributions/Proving (Automata 4, TimedAutomata 8, Provers 3, Termination 2, NeedhamSchroeder 1) | 18 | 0 | 0 |
| contributions/Solving (COLETTE, Equational/MatchingAC) | 2 | 0 | 0 |

"Candidate" files are those of `SAMPLES/`/`Sample/` whose name contains
the program's name: about 37 programs have a likely historical input, the
other ~140 need queries written for them. Every `.lgi` has a `start with`
clause; its start term usually refers to the query.

## 3. Goal and success criteria

Every program of `legacy/elan3/applications` and
`legacy/elan3/contributions` is run by `make check`, with the 2004 reference
system as the oracle, or is listed with a checkable reason. Done when:

1. **Coverage.** Each of the 293 programs is tested (an I and an A test of
   at least one query), or tested through a ported copy, or listed in
   `tests/legacy-overlay/unloadable.tsv` with its reason. A test
   (`check_overlay_coverage`) walks every `.lgi` of both trees and fails on
   a program in none of these states, and on a listed program that the
   reference now loads.
2. **The modern interpreter reproduces the reference** on every new I/A
   test; the only exceptions are differences the user approved case by
   case, recorded in `baseline.tsv` with their reason.
3. **elanc (Go)**: each new test is run in kind G; it passes (same output as
   the reference, `rewrite_step` lines excepted) or is UNSUPPORTED (elanc
   refuses the program, status 2); no ERROR, no FAIL. `bench_parity.py`'s
   expected statuses are updated.
4. **`make check` stays green and fast**: the default bench run takes less
   than 60 s more than today; slower cases are marked long (`*L*`) and run
   by `--long` and `check-reference`.
5. CI (Linux, macOS) runs the new tests; `check-reference` runs them against
   the reference.

The 112 programs tested today keep their tests; those that FAIL against
their historical output (printing changes since 2000, recorded in
`baseline.tsv` and checked by the snapshots) are not reopened by S9.

Out of scope: the other directories of `~/ELAN` (elan, elan4, Tom, APP,
Bench, TEST, Sophia...); kinds J/JO (REM) for the new tests.

## 4. Architecture: an overlay of `legacy/`

`legacy/` does not change. `tests/legacy-overlay/elan3/<application path>/`
mirrors `legacy/elan3/<application path>/` and may hold:

- `tst_all.overlay`: `itest`/`aitest`/`jtest` lines in the historical
  format (same parser, `if ($1 =~ *L*)` blocks included); the name differs
  from `tst_all` so that it is never mistaken for an original;
- `SAMPLES/<name>.inp` (queries written for S9) and `SAMPLES/<name>.out`
  (the reference's output on them);
- ported files, which replace the file of the same name of `legacy/`;
- `PORTING.md` whenever a file is ported: each change and its reason.

**Discovery.** `discover()` reads, per application directory, the original
`tst_all` and then `tst_all.overlay`. Test ids keep their current form
(`elan3/applications/X::I::lgi:spc:inp:out`): existing tests keep their ids,
snapshots and baseline lines. A directory without an original `tst_all`
(CStrategies, HOU_1_17, Picalc...) is found through its overlay alone.

**Private copy of a test.** The `legacy/` directory is copied, then the
overlay directory over it (files of the same name replaced), then the case
names are restored (`CASE-RENAMES.tsv`). The expected output and the
existence checks of the `.lgi` and `.inp` look in the overlay first.

**Oracle.** An overlay `.out` is the reference's output by construction:
`tests/legacy-overlay/make_expected.py` runs the reference
(`REFERENCE_PREFIX`, built on a case-sensitive volume) on each overlay query
with the bench's commands (`elan -b LGI [SPC] < INP`) and writes the `.out`.
It is not part of `make check`; it is run when queries are added, and
refuses to overwrite an existing `.out` without `--force`. A historical
`.out` (category 1 below) stays the historical oracle; the reference's
output then goes to the snapshot, as for the existing tests.

**Kinds.** I and A for each `itest`/`aitest` line; G for each of them too
(elanc compiles the program with the query as start term, as
`bench_parity.py` does today); UNSUPPORTED when elanc refuses.

## 5. Triage of each program

1. **A historical input exists but no `tst_all` names it**: a line in
   `tst_all.overlay`.
2. **The reference loads it, no input**: queries are written. Sources, in
   order: examples in comments of its `.lgi`/`.eln` files, inputs of sibling
   programs, the application's documentation, else a query built from the
   declared sorts and strategies. At least one query per program that runs
   its main strategy and ends within 10 s with the reference; a
   non-deterministic program gets a query with several results. Each new
   `.out` is read before it is committed: it must make sense for the program
   (not a syntax error of the query frozen as expected).
3. **The reference does not load it**: a minimal ported copy in the overlay
   (ELAN 2.x syntax as in BenchThesis, a missing module taken from another
   directory of `legacy/`...), documented in `PORTING.md`; without a
   reasonable port, a line in `unloadable.tsv` (`path<TAB>reason`).
4. **The reference loads it and the modern system differs**: a bug of the
   modern system. Fixed, with a case in `tests/regression/`, when the fix is
   local; otherwise recorded in `docs/followups.md` and kept in
   `baseline.tsv` with its current status, each such case approved by the
   user.

Programs that read or write files or are interactive (Server, IO_Call...)
get their input through `.inp`; files they write are compared when the bench
can; otherwise they are listed with the reason. Programs that need more than
10 s even with a small query are marked long.

## 6. Stages (each: plan, branch, tests first, review, merged green)

* **S9a — infrastructure**: the overlay in `run_tests.py` (discovery,
  private copy, expected output), `make_expected.py`, `unloadable.tsv`,
  `check_overlay_coverage` (the 179 programs listed as "to do" at first, the
  list shrinking to empty by S9d), unit tests; an automatic triage report of
  the 179 programs (loads with the reference, with the modern interpreter,
  candidate historical input).
* **S9b — historical inputs**: the programs of category 1 (~37) and those
  whose query is obvious.
* **S9c — applications**, by batches, largest first: Features (56),
  CStrategies (21), MetaApply (19), Strategies (14), UnifElanGen (9), then
  the others.
* **S9d — contributions**: Proving (18), Solving (2).
* **S9e — differences of the modern system** found on the way (category 4):
  the interpreter's fixes, or the list of differences submitted to the user.

Each stage is checked by `make check` in a clone on the default
(case-insensitive) volume and on a case-sensitive one, and by
`check-reference` against the reference without regression.

**Practical constraint.** The 2004 reference must be built on a
case-sensitive volume (its library installs both `strategy/Any.eln` and
`strategy/any.eln`); a small image dedicated to the reference
(`reference/README.md`) is kept for the duration of S9.

## 7. Decisions of the user (2026-10-12)

- Oracle for a program without historical output: the 2004 reference
  system ("Référence 2004").
- A program the reference cannot load: a ported copy in `tests/`, else
  listed with its reason ("Copie portée").
- Scope: `legacy/elan3` only ("elan3 seulement").
- Kinds: I, A and G ("I, A et G").
- Layout: an overlay of `legacy/` (approach A, against a separate manifest
  with its own runner, or copies of the applications in `tests/`).
- The three design sections (overlay and bench, triage, criteria and
  stages): approved ("oui, continue", "oui, écris la spec").
