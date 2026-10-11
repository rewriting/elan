# S7e — Performance of the Go compiler and the switch of `elanc`: Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. Delegated tasks, verified by the controller; one whole-branch review at the end. **Task 10 is a decision gate: stop there and wait for the user's answer; Tasks 11-13 run only if the user decides to switch.**

**Goal:** `elanc-go` meets success criterion 4 of the spec again (within 2x of the Go spike figures, at least as fast as `elanc` on every bench case, compile time under 2 s with a warm cache) with guards that catch a regression, gets the cheapest remaining performance leads and the S7e refusals that real programs hit (I/O, arrays), makes the Rust tests optional (G7), makes every test independent of what the name `elanc` designates, then presents the evidence for the user's decision to make `elanc` the Go compiler — and, only on that decision, switches in a reversible way (`elanc` → Go, `elanc-rem` keeps REM).

**Architecture:** No new pipeline stage. Performance: the IR's choice of native families (`ir.Family.Boxed`) stops counting the operators that only a query reaches when the start term does not use the query; the depth guard (`elanrt/depth.go`) takes its readings every N calls instead of counting levels (no `Unguard`); the other leads are measured spikes with a keep/drop rule. Refusals: runtime functions in `elanrt` for REM's I/O builtins and arrays, as REM's `builtin.h`/`termBase.c`. Switch: REM's driver also installed as `bin/elanc-rem`, every test calling `elanc-rem` or `elanc-go` explicitly; after the gate, a CMake cache variable `ELAN_ELANC` (`go` | `rem`) chooses what `bin/elanc` is (a symlink), so the switch is one configure option away from being undone.

**Tech Stack:** Go >= 1.24 (standard library only, offline; go1.27.1 on the measuring machine), CMake, POSIX sh / Python test scripts (`tests/compiler-rs`, `tests/compiler`, `tests/legacy-bench`).

**Spec:** `docs/superpowers/specs/2026-10-11-s7-go-compiler-design.md` (S7e in §5; criterion 4 in §2; G2, G3, G4, G7), with the S7c plan (Task 3: performance leads) and the S7d plan (kept refusals marked S7e), `docs/followups.md` ("elanc-go: kept refusals", "Performance"), `docs/s6-rust-status.md`.

## Global Constraints

- Branch `s7e-go-switch` (from `main` at d9c73a8). Go >= 1.24, standard library only, offline (`GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off`).
- Nothing in `legacy/` changes (programs are copied out of it into `tests/`, never edited in place); never read credential files. `elan` and REM (`src/compiler`) do not change; a bug found there: its own commit, a regression case, `docs/followups.md`.
- **No switch before Task 10's answer.** Until then `bin/elanc` stays REM's driver, byte for byte; the Tasks 1-9 commits must leave every user-visible command as it is today (new names may be added: `elanc-rem`).
- Oracles as in S7d: the interpreter for semantics, REM for the output format and for the constructs the interpreter does not run (arrays: the interpreter leaves them as constructors). Every deliberate difference is listed (`.no-rem`, `.no-interpreter`, `bench_g_differs.tsv`).
- Performance figures: best of N runs (`bench_perf.sh`, N >= 3), Apple M4 Max, the machine otherwise idle; each change measured alone, the table before/after in the task's results; a change that does not reach its target is reverted and recorded as "tried, not kept".
- `make check` green at every commit (`make install` first: `make` alone does not reinstall `elanc-go`); `bench_parity.py --kind G` may only improve.
- Commit messages end with `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`.

## Review Focus

1. Task 2's rule changes no result: a family is native only when no stuck term of it can reach the program; a query whose operator would need it boxed is refused at run time (status 2) and only when the start term does not use the query. Check the rule on a start term that uses the query (unchanged) and on the 150 queries of `test_query.sh`.
2. The guard of Task 3 never lets Go's fatal "stack overflow" happen: the bound on the bytes used between two readings (N calls x the largest frame) stays under the margin; `test_depth.sh` (17 programs, their statuses and messages) unchanged.
3. I/O and arrays (Tasks 6-7) have REM's semantics, including what happens under backtracking (effects are not undone, arrays are mutated in place) and REM's errors (out of range: REM prints and exits 1).
4. Tasks 8-9 change no command a user types; afterwards no test or CI step calls the bare name `elanc` (grep).
5. The gate (Task 10) presents measured evidence, the losses and the gains, and the switch tasks are reversible (`-DELAN_ELANC=rem` restores today's install; `-DELAN_RUST=ON` restores the Rust tests).

## Investigation findings (2026-10-11)

Measured on `s7e-go-switch` at d9c73a8 (`make install` into `build/install`, Apple M4 Max, go1.27.1, macOS). Scripts used (scratch, to become Task 1's `bench_jg.py`): every G case of the bench built by both compilers in `tests/legacy-bench/work/s7e-perf` (case-sensitive volume), one warm go build cache; run time = best of 3 with `/usr/bin/time -l` (10 ms resolution) and, for the small cases, best of 15-20 runs timed in Python (ms).

### 1. A regression since S7c breaks criterion 4 on nqueens

`tests/compiler-rs/bench_perf.sh build/install 3`:

| case | now real s | RSS MB | S7c final (plan S7c, Task 3) | spike Go (RESULTS.md) | criterion 4 (<= 2x spike) |
|---|---|---|---|---|---|
| nqueens 10 | 0.13 | 8.9 | 0.02 | – | – |
| nqueens 11 | **0.75** | 9.2 | 0.13 | 0.20 | **fails** (3.8x) |
| nqueens 12 | **4.59** | 9.3 | 0.74 | 1.20 | **fails** (3.8x) |
| efib 100 / 150 / 200 | 0.01 / 0.03 / 0.07 | 20 / 39 / 39 | 0.01 / 0.03 / 0.07 | 0.01 / – / 0.07 | ok |
| propc 1 / 2 / 3 | 0.00 / 0.08 / 0.28 | 11 / 40 / 42 | 0.00 / 0.08 / 0.29 | – / 0.13 / 0.64 | ok |
| deeplist3m | 1.26 | 901 | 1.25 | – (elanc-rs 0.36) | – |

Rewrite steps unchanged everywhere (nqueens 12: 653494329, REM's count).

**Cause** (found by comparing the generated code of the S7c merge e8c0846 and of d9c73a8): S7d task 5 compiles "the operators a query can contain" in every program (`ir/lower.go` `run`, `reachQuery`), even when the start term does not use the query. Those operators enter the totality/variant fixpoint (`lower.go:1490-1545`) and box native families:
- nqueens declares a constant `a : int` that no rule uses: reached through the query grammar of `int`, it is a constructor of the int family, so `int` is boxed (`noattack(a0, a1 rt.Term, ...)`, `minus_302` builds nodes through the table: 64 M table hits on nqueens 11, `lookup` 47 % of the profile): **4.59 s**.
- Without that constant, `btoi(@) : (bool) builtinInt` (BoolToInt, S7d task 1), reached through the int module's grammar, takes a bool that can be stuck (`noattack` is stuck when no rule applies), so `btoi` may stay stuck and `builtinInt` is boxed: every `-`, `+`, `!=` goes through `rt.IntOf`/`rt.MkInt` (`MkInt` 22 % of the profile): **1.57 s**. Confirmed on a 15-line program (`p(@) : (int) bool` with only `p(0) => true`, start term `f(3)` not using p): `minus_4` "may stay stuck" and `btoi_25` "may stay stuck"; with a total `p`, both native.
- The S7c compiler on the same program: 0.73 s. So any program with a partial bool operator (most programs) or a stray constant of a builtin family pays this since S7d. The bench does not show it (all its programs are small, see 2) and nothing guards bench_perf's times.

### 2. Bench, kind J (elanc = REM) vs kind G (elanc-go): run time and memory

175 G cases; 145 built and run by both (the 30 others fail for both: `elan3/applications/BCompletion` (15, elanc fails) and `Compiler.2.1/BenchThesis` (15, ELAN 2.1 syntax), as in the bench baseline). Same exit status (0) and same `rewrite_step` in all 145.

**No case where elanc-go is slower above the startup scale.** Cases whose J run takes more than 20 ms (best of 15-20 runs, ms; RSS = max of 3 runs, MB):

| program (versions) | J ms | J RSS | G ms | G RSS | steps | J/G |
|---|---|---|---|---|---|---|
| Compiler.*/Test propc2 (4) | 1188-1200 | 14.5 | 85-134 | 40.1-40.4 | 218978 | 9-14x |
| Compiler.{2.1,3.0}/Test bool3 (2) | 430-431 | 21.4 | 75-77 | 29.0-29.6 | 57664 | 5.6x |
| elan3 Simplest nqueens_jc | 143 | 7.5 | 11 | 5.8 | 648617 | 13x |
| elan3 Simplest queens_jc | 90 | 7.5 | 9 | 6.2 | 253888 | 10x |
| Compiler.*/Test manySortedSet3 (4) | 80-88 | 13.0 | 11 | 13.8-13.9 | 9507 | 7.5-8.3x |
| Compiler.*/Test manySortedSet2 (4) | 72-84 | 10.2 | 11 | 11.6-11.7 | 6623 | 6.5-7.8x |
| Compiler.*/Test propc1 (4) | 82-84 | 8.8 | 11 | 11.3-11.5 | 12841 | 7.7x |
| elan3 Simplest nhorse_jc | 41 | 7.5 | 6 | 6.1 | 45157 | 7x |
| Compiler.{3.2,4.0}/Test manySortedSet1 (2) | 22 | 8.9 | 5-6 | 7.8-7.9 | 1984 | 4x |

The other 122 cases run in the time of starting a process. In the bench harness (fresh binaries in the work directory) both take 4.6-6.1 ms and elanc-go was slower in 80 of them by 0.01-1.3 ms, faster in the others; an interleaved re-run of the 33 Compiler.4.0 cases: G slower in 9, by 0.02-0.8 ms, medians J 5.72 / G 5.71 ms. Measured outside the harness on the same binaries (match14, 60 runs): J 2.5-2.6 ms, G 2.2-2.3 ms; a Go "hello" 2.0-2.3 ms, a C "hello" 1.55 ms. So below ~10 ms the comparison is noise of the process start (both directions); Task 1's harness states the tolerance.

**Memory:** elanc-go's peak RSS is larger in 137 of 145 cases (median 5.4 MB vs 2.9 MB: the Go runtime), up to 2.8x on propc2 (40 vs 14.5 MB: the GC tuner's heap goal of at least 32 MiB, G3). The criterion is about time; the gate reports memory too.

### 3. Compile times (warm go build cache)

| | median s | max s | sum over 145 cases s |
|---|---|---|---|
| elanc (REM: export + Java + C generation) | 0.08 | 0.09 | 11.2 |
| make (C compilation of REM's output) | 0.11 | 0.15 | 16.9 |
| **J total** (elanc + make) | 0.19 | 0.24 | 28.1 |
| **elanc-go, new program** (runtime in the cache) | 0.11 | 0.25 | 17.9 |
| elanc-go, unchanged program | 0.06 | 0.12 | 8.4 |

elanc-go compiles faster than elanc + make except in 2 cases (Compiler.2.1 ident1 0.21 vs 0.17 s, manySortedSet2 0.25 vs 0.23 s; within noise). Largest programs (BenchThesis FastCompletion `ans`, 339 rules, `main.go` 390 KB): elanc-go 0.39 s for p8 (J 0.71 s), 0.28-0.32 s for the others; the first program with an empty cache 1.53-1.70 s (the runtime compiled). All under the 2 s target. BenchThesis G: 22 ok, steps equal to J's; p8 runs in 0.97-1.07 s (J 5.38-5.62 s; S7d measured 0.88 s), RSS 11-12 MB (J 8-9 MB).

### 4. The remaining perf leads of S7c Task 3, measured

Each lead's upper bound measured by removing the cost from a generated program (results then wrong where noted; a measurement only): nqueens 12 on S7c's generated code (native ints, i.e. after Task 2), deeplist3m and propc 3 on today's code; best of 5, real s / max RSS MB.

| lead | how measured | nqueens 12 | deeplist3m | propc 3 |
|---|---|---|---|---|
| (base) | | 0.74 / 10 | 1.24 / 901 | 0.28 / 42 |
| guard counter (`Guard`/`Unguard`, the strategies' deferred `Unguard`) | every `rt.Guard()`, `rt.Unguard()`, `defer rt.Unguard()` removed | 0.64 / 9 (-13 %) | 1.25 (0) | 0.28 (0) |
| step counting (`rt.Step`, `Steps++`) | every `rt.Step()` removed | 0.71 (-4 %) | 1.25 (0) | 0.28 (0) |
| weak pointer per node (`weak.Make` in `insert`) | the table holds strong pointers (nothing collected) | 0.60 / **143** (-19 %) | 0.59 / 761 (-52 %) | 0.27 / **146** (-4 %) |
| AC hash/compare (`rt.ACTerm` hashes and looks up the whole multiset; `Compare`) | profile (`GOMAXPROCS=1`): of ~160 ms of mutator, `ACTerm` + `lookup` ~50 ms, `Compare` ~10 ms | – | – | <= -0.05 s (estimate) |
| Eker subject conversion (`rt.Eker` converts its subject at each match) | no bench program spends time there (S7c) | – | – | – |

Notes: propc 3's GOMAXPROCS=1 profile is 45 % `runtime.madvise` (the scavenger); with all processors it runs on other threads (GOGC, `madvdontneed`: no change in real time). Readings:
- The guard is the only cheap, safe lead with a measurable gain (up to 0.10 s on nqueens 12); it also removes the deferred `Unguard` of the strategies. Task 3.
- Step counting: 4 % at most, on the program that counts 653 M builtin steps; REM's count is part of the output. Not worth a task (recorded here).
- The weak pointer is the largest cost where nodes die young (deeplist: half the time), but its removal keeps every node alive (143 MB instead of 10 on nqueens, G2/G3's reason for the weak table). Only a design that keeps the table complete (maximal sharing is what `==` relies on) and frees dead nodes is acceptable: Task 5 is a time-boxed spike with a keep/drop rule.
- AC hash/compare: propc 3 is already at 0.28 s (spike 0.43-0.68 s, criterion met); an order-independent incremental hash and a precomputed order key would gain at most ~0.05 s for a change to the node layout (`TestNodeSizes`) — **not planned in S7e** (recorded in `docs/followups.md` Performance).
- Eker subject conversion: no program depends on it — **not planned** (followups).
- `docs/followups.md` "Tail-call loops of elanc-go allocate every turn": not measured here; Task 4 measures it first (keep/drop).

### 5. The S7e refusals, and which are worth doing

From `docs/followups.md` ("elanc-go: kept refusals", S7e items) and `grep -n 'unsupported("S7e"' src/compiler-go/ir/*.go`:

| refusal | who has it | programs that need it | decision |
|---|---|---|---|
| I/O builtins 113-123 (`common/builtinStdio.eln`, `builtinIO.eln`: getc, putc, open, close, flush, write/read a term) | REM (`runtime/builtin.h` fun_113, 114, 116, 118-122; 115, 117, 123 are not defined there) and the interpreter (`rewrite/rtmisc.cc` BUILTGETC...) | library modules any user can import; none in the tests | **Task 6** (113, 114, 116, 118-122; 115/117/123 stay refused: REM has none) |
| arrays 200-203 (`common/builtinArray.eln`) | REM only (`termBase.c` term_newArray/getArray/setArray: destructive, out of range = exit 1); the interpreter leaves `new(3,empty)[0]<-c` as a term | `legacy/elan/sources/Compiler.4.0/Work` testArray, testVector, testMatrice, testHashSet (REM: `result = c`, `c`, `c`, `[true]`), TimedAutomata contributions | **Task 7** |
| AC shapes REM transforms before matching (`programs/refused/acnested`, `acallrep`) | both oracles agree | none (contrived shapes written to find the limit) | not planned in S7e: a port of REM's bipartite matcher and transformation2 for no program |
| identifier in an AC pattern | REM's order of a tagged identifier among the matcher's items is not established | none | not planned in S7e |
| AC operator whose arguments are not of its sort or printed without two arguments; AC where pattern of a builtin sort; rules for a builtin operation; any other builtin code | REM partly | none | not planned in S7e |

Each "not planned" is renamed in the IR (`unsupported(NotPlanned, ...)` with the reason) and listed in followups so that the S7e stage is closed.

### 6. What a switch means concretely

**Commands and install today** (`make install`, `build/install/bin`): `elan`, `elanc` (REM's POSIX sh driver, `src/compiler/scripts/elanc.src`, needs a JDK, the Boehm GC, a C/C++ compiler at use time; writes `<prog>.make` and `.elan.<prog>/`, the user then runs `make -f <prog>.make`), `elanc-rs` (Rust, `ELAN_RUST=ON` by default, built when cargo is found), `elanc-go` (built when go >= 1.24 is found; writes `.elan-go.<prog>/` and builds `./a.out` itself; installs `share/elan-go/elanrt` and `share/elan-go/go-command`).

**Who calls the name `elanc`** (all would silently start testing Go if `elanc` changed — hence Task 9 before any switch):
- `tests/legacy-bench/run_tests.py` kinds J/JO (`["elanc", ...]` then `make -f`), R → `elanc-rs`, G → `elanc-go` (`COMPILER_KINDS`); `Makefile` `CHECK_COMPILER` (`[ -x $(PREFIX)/bin/elanc ]`), `install` (removes `bin/elanc*`).
- `tests/compiler/test_elanc.sh` (REM's driver and Makefile), `test_runtime_strategies.sh` (`-strategy 1/2`: REM only), `test_start_term.sh`, `test_benchthesis.sh` (J, JO; G via `elanc-go`).
- `tests/compiler-rs/test_normalise.sh`, `test_strategies.sh`, `test_query.sh` (REM as the second oracle: `command -v elanc`, `elanc -b -nosplit -quiet` + make); `test_depth.sh`, `test_normalise.sh`, `test_strategies.sh` default to `ELANC=elanc-rs`; `bench_parity.py` defaults to kind R.
- Docs: `README.md` ("Compiling ELAN programs", layout, quick start), `CONTRIBUTING.md`, `docs/manual/install.tex`, `system.tex` (the 2003 manual: historical, kept), the expected files of `tests/compiler-rs/programs/*` mention `elanc` in comments only. CI (`.github/workflows/ci.yml`) and `ci/Dockerfile.linux` call `make`, already install Go 1.24; GitHub's runners have cargo, so CI runs the Rust tests today.
- `examples/` are run by the interpreter only (`make check`); `test_query.sh` compiles the 13 examples with both compilers.

**What users of `elanc` would lose with Go** (REM supports it, elanc-go refuses or ignores it), after Tasks 6-7:
- compiler options: `-coq`, `-proofterm`, `-lib`, `-debug` (debug runtime), `-strategy 1|2` (strategy terms evaluated at run time), `-onlyC`/`-noC`/`-nocode` (C output), `-optimiseChoicePoint` (meaningless without cpl), `-aterm` (already refused) — elanc-go ignores them silently today (`cmd/elanc-go/main.go:97-100`): Task 11 refuses the ones that change the result or the output files, naming `elanc-rem`;
- options of the generated program: `-sort=`, `-strategy=`, `-REFInput`, `-commandLine`, `-filequery` (status 2);
- constructs: I/O codes 115, 117, 123 (REM has none either), the AC shapes acnested/acallrep and an identifier in an AC pattern (REM computes them), AC operators on builtin sorts (oracles disagree; REM computes `pl(1,pl(2,3))`), `call "name":X` of another sort only (REM applies it ill-sorted), rules for a builtin operation;
- the workflow: no `<prog>.make`, no C sources to inspect; Go >= 1.24 needed instead of JDK + Boehm + C toolchain.
- (Doubles, codes 34-78, and the ELAN system calls 160-174: REM has none — nothing lost.)

**What they gain:** 4-14x faster programs on every bench program that runs longer than a process start (propc2 1.19 s → 0.09 s, bool3 0.43 → 0.08 s, BenchThesis p8 5.4 → 1.0 s; the spike's 30-100x on nqueens/efib/propc 3 against REM + cpl), compile 0.11 s instead of 0.19 s (median), the query parser byte-identical to REM's, outputs equal to REM's where REM is right and the REM bugs fixed (traversals, `IDENT(n)` printing, empty AC extensions, `x^n U y^m` values, Earley reads past its arrays, deep recursion: status 3 instead of a crash), Meta_strat terms (REM's C does not compile), a single static binary per program; costs: more memory (median 5.4 vs 2.9 MB, propc2 40 vs 14.5 MB, deep lists ~300 B/node), binaries of ~3.3 MB (REM 0.2 MB).

**REM and elanc-rs after a switch:** REM stays in the tree, built and installed as `elanc-rem` when `ELAN_COMPILER=ON` (default), still the J/JO oracle of the bench and of `tests/compiler`; nothing removed. elanc-rs: G7 says its tests become optional once elanc-go reaches parity — reached (the 79 cases of `bench_r_expected.tsv` — 36 PASS, 1 PASS~, 27 PASS≈, 15 ERROR — all have the same or a better status in `bench_g_expected.tsv`, which has 160; the only `.no-elanc-go` program, `programs/refused/ac`, is one elanc-rs refuses; `test_normalise`/`test_strategies`/`test_depth` run the same programs for both) — so `ELAN_RUST` defaults to OFF (Task 8, before the gate: decided by G7, independent of the switch), the code stays, `-DELAN_RUST=ON` builds and tests it.

**A reversible switch:** `bin/elanc-rem` = REM's driver (installed from Task 9 on, whatever `elanc` is); after the gate, a CMake cache variable `ELAN_ELANC` = `go` | `rem` makes `bin/elanc` a symlink to `elanc-go` or `elanc-rem` (default `go` when Go >= 1.24 is found, else `rem` with a STATUS message); `make install CMAKE_FLAGS=-DELAN_ELANC=rem` gives today's behaviour back; `elanc-go` keeps its name.

## File structure

| file | responsibility |
|---|---|
| `tests/compiler-rs/bench_perf.sh` | `--check`: fails when a case exceeds its limit (criterion 4) |
| `tests/compiler-rs/bench_jg.py` (new) | J vs G per bench case: compile times, run time (interleaved, warm), RSS, steps; `--check` |
| `src/compiler-go/ir/lower.go` | query operators that would box a family of the program (start term without the query): `QueryUnsupported` |
| `src/compiler-go/elanrt/depth.go`, `gen/*.go` | Guard every N calls, no `Unguard` |
| `src/compiler-go/gen/strat.go` | tail-call loops without per-turn closures (Task 4, if kept) |
| `src/compiler-go/elanrt/table.go` | weak pointers (Task 5, only if kept) |
| `src/compiler-go/elanrt/io.go` (new), `ir/ir.go`, `ir/lower.go`, `gen/gen.go` | I/O builtins 113, 114, 116, 118-122 |
| `src/compiler-go/elanrt/array.go` (new), `ir`, `gen`, `elanrt/print.go` | arrays 200-203 |
| `tests/compiler-rs/programs/{query,normalise,io,arrays}/` | dedicated programs |
| `CMakeLists.txt`, `src/compiler/CMakeLists.txt`, `src/compiler-go/CMakeLists.txt`, `Makefile` | `ELAN_RUST` OFF, `elanc-rem`, (after the gate) `ELAN_ELANC` |
| `tests/legacy-bench/run_tests.py`, `tests/compiler/*.sh`, `tests/compiler-rs/*.sh`, `bench_parity.py` | REM called as `elanc-rem`, defaults to elanc-go |
| `cmd/elanc-go/main.go` | (after the gate) REM-only options refused when invoked as `elanc` |
| `README.md`, `CONTRIBUTING.md`, `docs/followups.md`, `docs/s6-rust-status.md`, spec §5 | documentation |

---

### Task 1: Performance guards (criterion 4 as tests)

Today nothing fails when a case gets slower (finding 1 went unnoticed through S7d). This task writes the guards; nqueens fails them until Task 2.

**Files:**
- Modify: `tests/compiler-rs/bench_perf.sh` (a limit column; `--check`)
- Create: `tests/compiler-rs/bench_jg.py` (from the scratch scripts of this investigation)

**Interfaces:**
- `bench_perf.sh PREFIX [N] [--check]`: the table as today plus a `limit_s` column; limits: 2x the Go spike figure of RESULTS.md where there is one (nqueens 11: 0.40, nqueens 12: 2.40, efib 100/200: 0.02/0.14, propc 2/3: 0.26/1.28), else 1.5x the S7c final figure (nqueens 10: 0.05, efib 150: 0.06, propc 1: 0.02, deeplist3m: 1.9); with `--check`, a case over its limit prints `SLOW <case>: <t> s > <limit> s` and the exit status is 1.
- `bench_jg.py PREFIX [--filter REGEX] [--runs N] [--check] [--tsv FILE]`: discovers the G cases with `run_tests.discover()`, builds each with `elanc-rem` (from Task 9 on; `elanc` before) + make and with `elanc-go` (shared warm cache in `tests/legacy-bench/work/jg/gocache`, warmed by one build first), runs each `a.out -noInput -quiet` 3 times untimed then N times interleaved J/G (default 15), and prints per case: compile J (elanc, make), compile G (new, unchanged), run J/G (best, ms), RSS J/G (MB), steps J/G, status. `--check` fails (exit 1) when a case's steps or status differ, or when G is slower than J by more than max(1 ms, 5 %) (the process-start noise measured in finding 2), or when a G compile exceeds 2 s.

- [x] **Step 1: Write `--check` and the limits** in `bench_perf.sh`.
- [x] **Step 2: Run it to see it fail.** `tests/compiler-rs/bench_perf.sh build/install 3 --check` — Expected: exit 1, `SLOW nqueens11: 0.75 s > 0.40 s`, `SLOW nqueens12: 4.59 s > 2.40 s` (and nqueens10), every other case under its limit.
- [x] **Step 3: Write `bench_jg.py`**; run `python3 tests/compiler-rs/bench_jg.py build/install --filter 'Compiler.4.0/Test' --check` — Expected: 33 cases, steps and statuses equal, exit 0 (or a `SLOWER` line within the noise analysed in finding 2: then widen nothing, re-run; a reproducible one is a finding for Task 2).
- [x] **Step 4: Commit** (`tests: performance guards for elanc-go: bench_perf --check, bench_jg.py (S7e task 1)`). `bench_perf.sh --check` stays out of `make check` (timings depend on the machine); the README of `tests/compiler-rs` (or the script header) says to run both after a change of the runtime or of the generated code.

### Task 2: Native families again: the query's operators do not box the program

Target: nqueens 11 <= 0.15 s, nqueens 12 <= 0.80 s (S7c: 0.13 / 0.74), every row of `bench_perf.sh` within its limit; no output or step count changes.

**Files:**
- Modify: `src/compiler-go/ir/lower.go` (`run`, around the query reachability at `:553-596`; the variant fixpoint at `:1490-1545`)
- Test: `src/compiler-go/ir/grammar_test.go` or `ir_test.go` (helpers `ints()`, `p.start`, `p.lower`, `sortNamed`, `opBySymbol` exist), `src/compiler-go/gen/gen_test.go`
- Create: `tests/compiler-rs/programs/query/queryonly.{lgi,eln}` + its queries and `.status`/`.no-rem` files (test_query.sh conventions)

**Interfaces:**
- Consumes: `queryRule`, `QueryUnsupported` (S7d task 5), `Family.Boxed()`, `Start.UsesQuery`.
- Produces: when `!Start.UsesQuery`, the families are computed from the program's operators (the `base` reach) first; then each query operator is added only if it leaves every native family of the program native, else its rule becomes `QueryUnsupported` with `Why = "the query operator <op> would make <family> a sort of terms in the whole program (the start term does not use the query)"`. When `Start.UsesQuery`, unchanged (the query's value flows into the program: correctness first).

- [x] **Step 1: Write the failing tests.** `TestQueryOnlyOperatorsKeepIntNative`: the `ints()` program plus a constant `a : int` and a bool operator `p(@) : (int) bool` with only `p(0) => true` (both unused by the ground start term `1 + 1`): `int` and `builtinInt` native (`ir.Native` ok), the query rules of `a` and `btoi` are `QueryUnsupported` with the reason; `TestQueryUsedKeepsBoxing`: the same with a start term using the query: `int` boxed, as today. `gen`: a nqueens-shaped program gives `func noattack_N(a0 int32, a1 int32, a2 rt.Term)`. `programs/query/queryonly`: start term ground (`f(3)`), queries `1 end` (prompt, result as REM) and `a end` (status 2, the message on stderr; `.no-rem`: REM accepts it).
- [x] **Step 2: Run them to see them fail.** `cd src/compiler-go && go test ./ir -run 'QueryOnly|QueryUsed'` — Expected: FAIL (int boxed). `ELANC=elanc-go tests/compiler-rs/test_query.sh build/install query/queryonly` — Expected: FAIL (`a end` gives a result, status 0).
- [x] **Step 3: Implement** in `run`: compute the boxed families of `base` (a `builder` dry run or the fixpoint on `base`), then add the query operators one by one as `reachQueryOp` does, reverting an operator (snapshot) whose addition boxes a family native in `base`. Keep the S7d fallback (the program alone when the query set fails to build).
- [x] **Step 4: Run the tests.** Same commands — Expected: PASS. Then `ELANC=elanc-go tests/compiler-rs/test_query.sh build/install` (48 programs, 150 queries) — Expected: 0 failures; a query newly refused there means the rule is too wide: analyse and record before going on.
- [x] **Step 5: Measure.** `make install && tests/compiler-rs/bench_perf.sh build/install 3 --check` — Expected: exit 0, nqueens 12 <= 0.80 s, nqueens 11 <= 0.15 s, steps unchanged; the before/after table in this plan.
- [x] **Step 6: Bench and check.** `python3 tests/compiler-rs/bench_parity.py build/install --kind G` (no `PARITY:` line), `make check-go` green.
- [x] **Step 7: Commit** (`elanc-go: the query's operators no longer box the program's native families (S7e task 2)`), with `docs/followups.md` (the ruling and its cost: a query using such an operator is refused at run time when the start term ignores the query; REM accepts it).

Ruling (to confirm in Step 4): refuse such queries rather than compile two copies of the query's operators (one boxed, for the query only) — the query's value is dropped when the start term does not use it, only its normalisation steps are visible; cost if wrong: status 2 for a query naming such an operator in such a program.

### Task 3: A guard without Unguard (readings every N calls) — tried, not kept (see Task 3 results)

Target: nqueens 12 <= 0.67 s after Task 2 (measured upper bound without any guard: 0.64 s), deeplist3m and propc 3 not slower; `test_depth.sh` unchanged (statuses, messages, results).

**Files:**
- Modify: `src/compiler-go/elanrt/depth.go` (design comment and code), `depth_test.go`, `driver.go` (the end-of-program depth check goes), `src/compiler-go/gen/gen.go`, `gen/strat.go`, `gen/meta.go`, `gen/traverse.go`, `elanrt/strat.go` (every `Unguard`/`defer rt.Unguard()`), the gen tests that pin `rt.Guard()\n\tdefer rt.Unguard()` (`gen/strat_test.go:97,110`, `gen/meta_test.go:89`)

**Interfaces:**
- `func Guard()`: increments a call counter (never decremented); every `ReadEvery` calls (a power of two, e.g. 1024) it takes a reading: the goroutine stacks' size (runtime/metrics) and the stack pointer (address of a local). Below the ceiling: no limit. At the first reading that sees the ceiling (the anchor), the use is at most `C/2 + ReadEvery x MaxFrame` (the stack doubled within the last `ReadEvery` calls, each at most one level deeper); afterwards `use = useAtAnchor + (spAnchor - sp)`; past `C - margin` it stops with `TooDeep` (status 3, same message). A change of the stacks' size (a GC shrink) re-anchors. `Unguard` is removed.
- Kept: `DepthExit`, the message, `SetMaxStack`, `MaxStack`.

- [ ] **Step 1: Write the failing tests.** `depth_test.go`: a recursion without any `Unguard` stops with status 3 before Go's stack overflow (child process, as the existing tests); a long iteration of shallow calls (10^8 `Guard` calls at depth 1) does not stop and takes less than 1 ns per call above the bare loop; the gen tests expect no `Unguard` in the emitted code.
- [ ] **Step 2: Run them to see them fail.** `cd src/compiler-go/elanrt && go test -run Depth ./...`, `cd src/compiler-go && go test ./gen` — Expected: FAIL.
- [ ] **Step 3: Implement**; remove every `Unguard` from the runtime and the emitted code (normalisation functions return directly again).
- [ ] **Step 4: Run** the unit tests, then `ELANC=elanc-go tests/compiler-rs/test_depth.sh build/install` — Expected: PASS (17 programs; count8 may stop at another depth, same message and status), `make check-go` green.
- [ ] **Step 5: Measure** (`bench_perf.sh --check`): nqueens 12 <= 0.67 s; if not reached, revert and record "tried, not kept" with the figures.
- [ ] **Step 6: Commit** (`elanrt: the depth guard reads the stack every N calls, no Unguard (S7e task 3)`).

### Task 4: Tail-call loops without per-turn allocations (measure first)

`docs/followups.md` Performance: the closures `again`, those of the loop body and the `rt.NewOne`/`rt.NewDc` states are created inside the `for` of a tail-call loop.

**Files:** `src/compiler-go/gen/strat.go` (`strategies`, the loop emission), `gen/strat_test.go`; `elanrt/strat.go` (a `Reset` on the One/Dc states)

- [x] **Step 1: Measure.** `programs/depth/tail6`, `taildc6`, `tailone6`, `iterdet6` (and nqueens 12): run with `ELAN_MEMPROFILE` and `ELAN_STATS=1`; count the allocations per loop turn (`go tool pprof -sample_index=alloc_objects`). Keep the task only if a program spends >= 10 % of its time in these allocations; else record the numbers in this plan and in followups ("measured, not worth it") and skip to Task 5.
- [x] **Step 2: Failing test.** A gen test that runs a tail-recursive strategy loop 10^6 turns and checks `testing.AllocsPerRun` per turn <= 1 (today: the count of closures + states).
- [x] **Step 3: Implement** (closures and states hoisted out of the `for`, reset each turn), **Step 4:** tests (`go test ./gen`, `test_strategies.sh`, `test_depth.sh`) green, **Step 5:** measure, **Step 6:** commit (`elanc-go: tail-call loops allocate once (S7e task 4)`).

### Task 5: Spike — the weak pointer per node (time-boxed, keep/drop) — tried, not kept (see Task 5 results)

Measured: the weak pointer is half of deeplist3m (1.24 → 0.59 s with strong pointers) and 19 % of nqueens 12, but strong pointers keep every node (nqueens 143 MB instead of 10). Time box: one day of work; decision rule below.

**Files:** `src/compiler-go/elanrt/table.go`, `term.go`, `gc.go`, their tests; `spikes/` is not needed (the runtime has the switches).

- [ ] **Step 1: Profile** deeplist3m and nqueens 12 (`GOMAXPROCS=1`, `ELAN_CPUPROFILE`): split the weak cost into `weak.Make` (handle creation: `getWeakHandle`, `addspecial` walking the span's specials list) and `Value()`; record.
- [ ] **Step 2: Try, each alone, behind a runtime variable for the measurement only:** (a) nodes padded or allocated so that fewer share a span's specials list (cost per `addspecial`); (b) interning only the nodes that are ever compared or looked up again (a node built by a right-hand side and given at once to a function that consumes it), if the IR can tell; (c) any other design that keeps the table complete (every live node findable: `==` is equality) and lets dead nodes go.
- [ ] **Step 3: Decision rule.** Keep a variant only if deeplist3m <= 0.9 s and nqueens 12 improves by >= 10 %, peak RSS <= 1.5x today on every `bench_perf.sh` case, `make check-go` green (test_depth's RSS bounds included); else revert and write the figures in this plan and in followups ("weak table: measured, kept as G2 decided").
- [ ] **Step 4: Commit** what is kept (`elanrt: ... (S7e task 5)`), or the plan/followups update only.

### Task 6: I/O builtins (codes 113, 114, 116, 118-122)

**Files:**
- Modify: `src/compiler-go/ir/ir.go` (`BuiltinOp`: `IOGetc` 113, `IOPutc` 114, `IOOpen` 116, `IOClose` 118, `IOWrite` 119, `IORead` 120, `IOPid` 121, `IOFlush` 122; `NeedsValues`; side effects: not pure — never hoisted, never folded), `ir/lower.go` (the S7e refusal of 113-123 narrowed to 115, 117, 123 as NotPlanned), `gen/gen.go`
- Create: `src/compiler-go/elanrt/io.go`, `io_test.go`; `tests/compiler-rs/programs/io/*.{lgi,eln}`

**Interfaces:**
- Produces (elanrt): `func Getc(pid int32) int32`, `Putc(pid, c int32) int32`, `Open(file, mode []byte) int32`, `Close(pid int32) int32`, `Flush(pid int32) int32`, `WriteTerm(pid int32, t Term) Term`, `ReadTerm(pid int32, t Term) Term` — REM's semantics (`src/compiler/runtime/builtin.h:140-151` and the C functions they call: pid 0/1/2 = stdin/stdout/stderr, MAXFILE 300, the error values REM returns), stdout shared with the result printer (`rt.Out`, flushed in order).

- [x] **Step 1: Establish the semantics.** Read REM's `Getc`, `Putc`, `open_file`, `close_file`, `flush_file`, `term_write`, `term_read` and the interpreter's `rtmisc.cc` (BUILTGETC...); write one program per builtin (putc to stdout, write a term to a file then read it back, getc from stdin with `-noInput`... and from a file, close/flush, an invalid pid), run both oracles; record differences (`.no-rem`/`.no-interpreter`) in the programs' notes.
- [x] **Step 2: Failing tests.** `io_test.go` (each function on temp files); `ELANC=elanc-go tests/compiler-rs/test_normalise.sh build/install io/...` (or `-d io` of test_strategies) — Expected: FAIL `not supported yet (S7e): builtin operation code 113`.
- [x] **Step 3: Implement**; **Step 4:** tests PASS (output byte-identical to REM's, files written equal), `make check-go` green; **Step 5:** followups updated (115, 117, 123 not planned: REM has none); commit (`elanc-go: I/O builtins as REM (S7e task 6)`).

### Task 7: Arrays (codes 200-203)

**Files:**
- Modify: `ir/ir.go` (`ArrayNew` 200, `ArrayGet` 201, `ArraySet` 202, `ArrayLength` 203; the sort `builtinArray[X]` is a sort of terms whose values are array nodes), `ir/lower.go` (refusal removed), `gen/gen.go`, `elanrt/print.go` (REM's printing of an array: `termOut.c`, CODE_ARRAY)
- Create: `src/compiler-go/elanrt/array.go`, `array_test.go`; `tests/compiler-rs/programs/arrays/` with copies of `legacy/elan/sources/Compiler.4.0/Work/{testArray,testVector,testMatrice,testHashSet}.lgi` and the modules they import (copied, not edited), each `.no-interpreter` (the interpreter leaves the array operations as constructors: `new(3,empty)[0]<-c[1]<-b[2]<-a[0]`)

**Interfaces:**
- Produces (elanrt): an array node kind outside the unique table (identity: two `new` are two arrays, `rt.Eq` by pointer, never hashed into a parent's sharing — a node holding an array is built outside the table too, as REM's terms holding a CODE_ARRAY are not shared), `func NewArray(n int32, init Term) Term`, `ArrayGet(a Term, i int32) Term`, `ArraySet(a Term, i int32, x Term) Term` (mutates and returns a, REM's `term_setArray`), `ArrayLength(a Term) int32`; out of range: REM's message on stdout and exit 1.

- [x] **Step 1: Establish REM's semantics** on the four programs (results `c`, `c`, `c`, `[true]`, steps 2, 21, 10, 8) and on dedicated ones: an array under backtracking (a `where` that sets then fails: is the mutation visible to the next branch in REM? record it), an array printed, out-of-range get/set, an array as an argument of a hash-consed constructor.
- [x] **Step 2: Failing tests** (`array_test.go`; `test_normalise.sh ... arrays/...` — Expected: FAIL `not supported yet (S7e): builtin operation code 200`).
- [x] **Step 3: Implement**; **Step 4:** tests PASS (results and steps equal to REM's), `make check-go` green; commit (`elanc-go: arrays as REM (S7e task 7)`).

### Task 8: The Rust tests become optional (G7)

Parity is reached (finding 6). Decided by the spec, independent of the switch.

**Files:** `CMakeLists.txt` (`option(ELAN_RUST ... OFF)`), `Makefile` (`check-rust`: runs `cargo test` and the scripts only when `$(PREFIX)/bin/elanc-rs` exists, i.e. a `-DELAN_RUST=ON` build; else prints `elanc-rs not built (-DELAN_RUST=OFF, the default since S7e): Rust tests skipped`), `tests/compiler-rs/test_normalise.sh`, `test_strategies.sh`, `test_depth.sh` (default `ELANC=elanc-go`), `bench_parity.py` (default kind G), `README.md` (the elanc-rs paragraph), `docs/s6-rust-status.md`, spec G7 ("done in S7e")

- [x] **Step 1: Evidence of parity** recorded in this plan: `bench_r_expected.tsv` cases vs their G status in `bench_g_expected.tsv` (each G status equal or better); programs with `.no-elanc-go` (must be none that elanc-rs passes).
- [x] **Step 2: Change** the defaults; **Step 3:** `make install && make check` — Expected: green, the Rust tests skipped with the message; `make CMAKE_FLAGS=-DELAN_RUST=ON check-rust` — Expected: green (the Rust tests still pass when asked for).
- [x] **Step 4: Commit** (`build: the Rust compiler is optional, ELAN_RUST=OFF by default (S7e task 8, G7)`).

### Task 9: Tests independent of the name `elanc`; REM also installed as `elanc-rem`

No user-visible change: `bin/elanc` is still REM's driver; `bin/elanc-rem` is the same script.

**Files:** `src/compiler/CMakeLists.txt` (install the generated driver also as `elanc-rem`), `Makefile` (`install` removes `bin/elanc-rem`; `CHECK_COMPILER` tests `bin/elanc-rem`), `tests/legacy-bench/run_tests.py` (J/JO run `elanc-rem`), `tests/compiler/test_elanc.sh`, `test_runtime_strategies.sh`, `test_start_term.sh`, `test_benchthesis.sh` (J/JO), `tests/compiler-rs/test_normalise.sh`, `test_strategies.sh`, `test_query.sh` (the REM oracle: `command -v elanc-rem`, `elanc-rem -b -nosplit -quiet`), `tests/compiler-rs/bench_jg.py`

- [x] **Step 1: Failing check.** A test in `tests/compiler/test_elanc.sh`: `elanc-rem` exists and is REM's driver (its usage text, its `.make`); and an architecture-style check (`tests/architecture` or a Makefile target run by `make check`): `grep -nE '(^|[^-_a-z])elanc( |"|\x27|$)'` over `tests/`, `Makefile`, `ci/`, `.github/` finds no invocation of the bare `elanc` (comments allowed by a marker) — Expected: FAIL (the call sites listed in finding 6).
- [x] **Step 2: Change** every call site; **Step 3:** `make install && make check` — Expected: green, the check passes, `bin/elanc` unchanged (`cmp build/install/bin/elanc build/install/bin/elanc-rem`).
- [x] **Step 4: Commit** (`tests: REM is called elanc-rem, the tests no longer depend on what elanc is (S7e task 9)`).

### Task 10: DECISION GATE — make `elanc` the Go compiler?

**Stop here.** Nothing after this task runs without the user's explicit answer.

- [ ] **Step 1: Collect the evidence** on the branch as it is: `bench_perf.sh build/install 5 --check` (table), `bench_jg.py build/install --check --tsv ...` (all 145 cases: the slowest-G list, compile times, RSS), `BENCHTHESIS_KINDS="J G" tests/compiler/test_benchthesis.sh` (p8, primes8), `bench_parity.py --kind G` (statuses), the refusal inventory (`grep -n 'unsupported(' src/compiler-go/ir/*.go`), `make check` green.
- [ ] **Step 2: Write the decision note** in this plan (section "Decision gate") and present it to the user: (a) criterion 4 met or not, per case; (b) the losses of finding 6 that remain after Tasks 6-7; (c) the gains; (d) the costs (memory, Go >= 1.24 as a requirement of `elanc`, binary size); (e) the reversible mechanism and its one-line undo; (f) the open choices: switch now / later / never; keep `elanc-go` as an alias (proposed: yes); `elanc` refusing REM-only options vs passing them to `elanc-rem` (proposed: refuse with a message naming `elanc-rem`); a `<prog>.make` compatibility stub for scripts that run `make -f prog.make` after `elanc` (proposed: no; the README says `elanc` builds `./a.out` itself).
- [ ] **Step 3: Record the user's answer** verbatim in the plan and in spec §5 S7e; commit (`docs: S7e decision gate, evidence and the user's decision`). If the answer is not "switch", mark Tasks 11-12 "not done (decision of <date>)" and go to Task 13.

### Task 11 (only after a "switch" decision): `ELAN_ELANC`, `bin/elanc` → elanc-go

**Files:** `CMakeLists.txt` (`set(ELAN_ELANC go CACHE STRING "what bin/elanc is: go (elanc-go) or rem (elanc-rem)")`; `go` falls back to `rem` with a STATUS message when elanc-go is not built; `rem` without the compiler built: error), `src/compiler-go/CMakeLists.txt` / `src/compiler/CMakeLists.txt` (install `bin/elanc` as a relative symlink `elanc -> elanc-go` or `elanc -> elanc-rem`), `Makefile` (`install` removes the symlink), `src/compiler-go/cmd/elanc-go/main.go` (+ `main_test.go`), `tests/compiler/test_elanc_switch.sh` (new, in `make check`)

**Interfaces:**
- elanc-go finds its prefix through the symlink (`os.Executable` + `filepath.EvalSymlinks`): `share/elan-go/elanrt`, `go-command`, `bin/elan`.
- Invoked under any name, elanc-go refuses (exit 1, `elanc: option -coq is REM's: use elanc-rem`) `-coq`, `-proofterm`, `-lib`, `-debug`, `-onlyC`, `-noC`, `-nocode`, `-strategy 1|2`; keeps ignoring the options without effect on the result (`-nosplit`, `-quiet`, `-O*`, `-optimiseChoicePoint`, ...).

- [ ] **Step 1: Failing tests.** `test_elanc_switch.sh`: with the default build, `elanc -help` is elanc-go's, `elanc -b enum` (Compiler.4.0/Test copy) builds `./a.out` whose output equals the J snapshot (rewrite_step excluded), `elanc -coq enum` exits 1 naming `elanc-rem`, `elanc-rem -nosplit -quiet enum && make -f enum.make` works; with `CMAKE_FLAGS=-DELAN_ELANC=rem` (a second build dir), `elanc` is REM's (`test_elanc.sh` passes against `elanc`). `main_test.go`: the refused options; the prefix found through a symlink. — Expected: FAIL.
- [ ] **Step 2: Implement**; **Step 3:** `make install && make check` (both values of `ELAN_ELANC` for `test_elanc_switch.sh`) — Expected: green; CI unchanged (`make check` builds with the default).
- [ ] **Step 4: Commit** (`build: elanc is the Go compiler (ELAN_ELANC=go), elanc-rem keeps REM (S7e task 11)`).

### Task 12 (only after a "switch" decision): documentation

- [ ] `README.md`: layout (`src/compiler-go/`: the compiler `elanc`; `src/compiler/`: REM, `elanc-rem`), requirements (Go >= 1.24 for `elanc`; JDK + bdw-gc only for `elanc-rem` and its tests), "Compiling ELAN programs" rewritten around `elanc` (builds `./a.out`, options, exit statuses of the compiler and of the programs, the query read on stdin), a paragraph on `elanc-rem` with what only it does (finding 6 list), how to undo (`-DELAN_ELANC=rem`), the timings (finding 2 table, short); quick start counts; `CONTRIBUTING.md`; `docs/followups.md` (the kept refusals become "differences from elanc-rem"); spec §2/§5 (S7e done, the decision); `tests/compiler/benchthesis/README.md` timings; `docs/manual/*.tex` unchanged (the 2003 manual: README says `elanc` there is today's `elanc-rem`).
- [ ] Commit (`docs: elanc is the Go compiler (S7e task 12)`).

### Task 13: Bench, review, merge

- [ ] **Step 1:** `python3 tests/compiler-rs/bench_parity.py build/install --kind G --save` — Expected: `UNSUPPORTED=0`, none worse; `tests/compiler-rs/bench_perf.sh build/install 5 --check` and `bench_jg.py --check` — Expected: exit 0; the final tables in this plan.
- [ ] **Step 2:** `make check` green (and `make check-sanitize`), on macOS; the Linux image (`ci/Dockerfile.linux`) if available.
- [ ] **Step 3:** Results per task in this plan ("Task N results", as S7c/S7d), followups updated.
- [ ] **Step 4:** Whole-branch review; fixes, each with its test; merge into main (after the user's go).

## Acceptance (S7e)

- Criterion 4: every `bench_perf.sh` case within 2x of the Go spike figure (or 1.5x S7c's where the spike has none); no bench case where elanc-go is slower than elanc beyond the process-start noise (`bench_jg.py --check`); compile time of every bench and BenchThesis program under 2 s with a warm cache (measured: max 0.39 s), 1.7 s cold.
- I/O (113, 114, 116, 118-122) and arrays (200-203) compiled with REM's results; the other S7e refusals renamed "not planned" with their reason.
- `ELAN_RUST=OFF` by default, Rust tests green with `-DELAN_RUST=ON`.
- No test calls the bare `elanc`; `bin/elanc-rem` exists.
- The decision gate presented and its answer recorded; if "switch": `bin/elanc` → elanc-go, reversible with `-DELAN_ELANC=rem`, docs updated.
- `make check` green.

## Risks

- Task 2's rule could refuse a query that a user expects to work (an operator only the query uses, in a program whose start term ignores the query). Mitigation: only when the start term does not use the query; the 150 queries of `test_query.sh` as evidence; the message names the cause.
- The sampled guard (Task 3) relies on the stack doubling only when its use crosses the size and on runtime/metrics reporting the stacks' size at once; a Go version that changes either breaks the bound. Mitigation: `depth_test.go` runs a real overflow attempt in a child process; the margin stays a quarter of the ceiling.
- Timing noise below 10 ms makes "at least as fast on every case" undecidable at the process-start scale; `bench_jg.py` states its tolerance (max(1 ms, 5 %)) and interleaves runs; the gate shows the raw numbers.
- I/O and arrays mutate state outside terms: hash-consing must never share an array, and the order of effects must follow REM's evaluation order (innermost, left to right); the tests compare files and stdout with REM's byte for byte.
- The switch changes the requirements of `elanc` (Go instead of JDK + Boehm + C toolchain): a machine with only the old requirements gets `elanc` = REM through the CMake fallback, with a STATUS message.

## Results

### Task 1 results

`bench_perf.sh PREFIX [N] [--check]` (limit_s column, `SLOW` lines, exit 1 with `--check`) and `bench_jg.py` (new). Measured at d9c73a8 + Task 1:

- `bench_perf.sh build/install 3 --check`: exit 1 (RED, as expected until Task 2): `SLOW nqueens10: 0.13 s > 0.05 s`, `SLOW nqueens11: 0.80 s > 0.40 s`, `SLOW nqueens12: 4.75 s > 2.40 s`; every other case under its limit (efib 200 0.07, propc 3 0.29, deeplist3m 1.33 s).
- `bench_jg.py build/install --filter 'Compiler.4.0/Test' --check`: 33 cases built by both, steps and statuses equal, no `SLOWER`, exit 0. Run times are timed in Python (fork/exec + wait4, which also gives the child's own max RSS), so the process-start floor is ~2.7 ms (G) / ~3.1 ms (J) here; G is faster than J in all 33 (largest: propc2 1172 → 83 ms, manySortedSet3 77 → 7.9 ms); G compile of a new program 0.12-0.17 s, unchanged 0.05-0.08 s.
- Ruling: the J compiler of `bench_jg.py` is `elanc-rem` when `PREFIX/bin/elanc-rem` exists, else `elanc` — why: Task 9 renames REM's call sites and the script must work before and after it — cost if wrong: none (both are REM's driver until the gate; after a switch `elanc` alone would be Go, which the `elanc-rem` test avoids).
- Ruling: a case that J cannot build is reported (`J fails`) but is not a `--check` failure; one that G cannot build while J can is — why: the guard is about elanc-go regressing against REM, not about REM's own failures (BCompletion, ELAN 2.1 syntax) — cost if wrong: none for G's regressions; a REM build regression is caught by the J kinds of `make check`.

### Task 2 results

`ir/lower.go` `run`: when the start term does not use the query, the program alone (`base`) is built first and the sorts native there recorded; the query's operators are then reached all at once and built: kept if no native sort is lost; else (only then: a build per operator costs) they are added one by one, each refused (`QueryUnsupported`, `the query operator <decl> would make <sort> a sort of terms in the whole program (the start term does not use the query)`) when its build loses a native sort or fails. The cheap S7d pass (reach errors only) runs first when the whole set does not reach. A start term using the query: unchanged.

Tests RED → GREEN: `ir` `TestQueryOnlyOperatorsKeepIntNative` (int, builtinInt boxed; `a` and `btoi` compiled → native, `a` refused with the reason, btoi or p refused), `TestQueryUsedKeepsBoxing` (green before and after: the unchanged path), `gen` `TestQueryOnlyOperatorsKeepIntsNative` (`noattack_402(a0 rt.Term, ...)` → `(a0 int32, a1 int32, a2 rt.Term)`), `programs/query/queryonly` (`a end`: status 0 → 2; `1 end`: result 2 as REM, `.expected`).

Found on the way (own commit 8ea6e94): `strategy_terms/ms_labels_seq` stopped with `term2labels internal error`: without the query's `@@`, `Labels[int]` is a native family (the injection of builtinString) and the labels function of META did not read a boxed string. Hidden since S7d task 5, which reached `@@` through the query grammar in every program.

| case | before (d9c73a8) s | after s | limit s |
|---|---|---|---|
| nqueens 10 | 0.13 | 0.02 | 0.05 |
| nqueens 11 | 0.80 | 0.13 | 0.40 |
| nqueens 12 | 4.75 | 0.73 | 2.40 |
| efib 200 | 0.07 | 0.07 | 0.14 |
| propc 3 | 0.29 | 0.28 | 1.28 |
| deeplist3m | 1.33 | 1.27 | 1.9 |

Steps unchanged everywhere; `bench_perf.sh --check` exit 0. Compile time: nqueens 0.05-0.07 s (one build per query operator not in the program, as before within noise), FastCompletion p8 (unchanged program) 0.11 s as before (its query set fails to reach as a whole, so the cheap pass runs first; a first version that built per operator there took 0.44 s). `test_query.sh`: 49 programs, 152 queries, 0 failures — only `queryonly`'s query is refused by the rule (of the 49 programs only `start_term/ground` and `queryonly` have a start term without the query). `bench_parity.py --kind G`: PASS=78 PASS~=1 PASS≈=53 FAIL=13, no `PARITY:` line; BenchThesis G 22 ok; `make check-go` green.

- Ruling (confirmed): refuse such a query (status 2) rather than compile the query's operators twice (boxed for the query only) — why: when the start term does not use the query its value is dropped, only its normalisation steps would be visible, and none of the 152 test queries is affected — cost if wrong: status 2 for a query naming such an operator in a program whose start term ignores the query (REM accepts it).
- Ruling: "native in the program alone" is checked per sort (a member of a family, e.g. `int`, included), and the message names the first such sort in sort order (`a : int` names builtinInt, which it boxes too) — why: a sort leaving its family changes the generated signatures as much as a boxed family — cost if wrong: a message naming builtinInt where int is meant.

### Task 3 results — tried, not kept

Implemented as planned (`Guard` counts its calls and reads the stack every `ReadEvery` = 1024 calls; the anchor's use bounded by C/2 + ReadEvery x MaxFrame (16 KiB); stop past 3C/4; re-anchor when a reading sees the stack below the ceiling; every `Unguard`/`defer rt.Unguard()` removed from the runtime and the emitted code; Main's end-of-program depth check removed). Tests RED → GREEN: `depth_test.go` recursions without exit call (RED: status 1, "guarded depth 1000000 at the end of the program"), new `TestGuardReadsEveryReadEvery` (RED: does not compile), `TestGuardShallowCalls` (10^8 guarded calls at depth 1: no stop, 0.40 ns per call above the bare loop), GOMAXPROCS=1 cases in `TestDepthGuard`; gen `TestNoUnguard` and the shapes of `gen_test`/`strat_test`/`meta_test` (RED: Unguard emitted). All green, also on Linux (golang:1.25-alpine, arm64, `go test -run 'DepthGuard|GuardReads|GuardShallow'`); `test_depth.sh` 20 programs, 0 failures, with the default GOMAXPROCS and GOMAXPROCS=1 (same statuses, messages, results; RSS within 1-5 %: count8 1783 vs 1799 MB, upto7 2598 vs 2471 MB). Stop depths (ELAN_STATS): list 9.86 M levels (before 8.4 M), native ints 24.6 M (16.8 M), CPS iterate* 5.48 M (5.0 M), 4 KiB frames 193 K (180 K).

Measured (nqueens 12, generated code of this branch, interleaved best of 7, Python wall time):

| variant | s |
|---|---|
| Task 2 (Guard + Unguard) | 0.741 |
| Task 3 (Guard every 1024 calls, no Unguard) | 0.705 |
| Guard = a load and a branch only (`if flag { ... }`, never true; no counter) | 0.689 |
| the same without a call in the branch | 0.700 |
| no guard at all (unsafe) | 0.640 |

`bench_perf.sh build/install 5 --check` with Task 3: nqueens 12 0.70 s, nqueens 11 0.12, deeplist3m 1.27, propc 3 0.28, efib 200 0.07 (exit 0: all within the criterion-4 limits). nqueens 12 makes 107.5 M guarded calls; any check at a function entry costs ~0.5 ns there (a load and a branch alone: 0.05 s; the inlining decisions are the same with and without the guard, `-gcflags=-m`). The target (<= 0.67 s) assumed the no-guard figure minus a little; it is out of reach for any per-call guard.

- Ruling: Task 3 reverted on `s7e-go-switch` (the plan's rule: a change that does not reach its target is reverted), kept as commit 96f18ec on the local branch `s7e-task3-guard-tried` (`git cherry-pick 96f18ec` restores it, tests included) — why: 0.705 s > 0.67 s; the gain (-5 % on nqueens 12, nothing elsewhere) does not reach the target the plan set, and the depth guard is safety-critical code that should change only for a clear gain — cost if wrong: 0.036 s on nqueens 12 (5 %) and slightly smaller depth limits (8.4 M instead of 9.9 M list levels) left on the table; the user may decide to take it at the gate (Task 10) with a revised target.

### Task 4 results

Measured first (the programs of `programs/depth` at 10^7 instead of 10^6 so that the loop dominates the process start; `ELAN_MEMPROFILE`, `GOMAXPROCS=1` `ELAN_CPUPROFILE`): every turn of a tail-call loop allocated its `OneChoice` and `DcChoice` (the closures `again`, `k2` stayed on the stack, but `rt.NewOne`/`rt.NewDc` escape: their `K` closes over the state): tail7 37.6 M objects (NewDc 19.2 M, NewOne 18.4 M), taildc7 57.6 M; their share of the CPU profile: tail7 50 % (NewDc 36 %, NewOne 14 %), taildc7 31 %, tailone7 18 % — above the plan's 10 %: task kept. None of `bench_perf.sh`'s programs has a tail-call loop (no `NewOne`/`NewDc` in their generated code): their figures do not move (nqueens 12 0.74 s, deeplist3m 1.27 s, propc 3 0.28 s; `--check` exit 0).

`gen/strat.go`: a loop declares `next`, `tail`, `again`, its `one` and the hoisted closures and states once, before the `for`; each turn sets `tail = false` and calls `Reset()` on `one` and on every top-level dc/one state (`elanrt` `OneChoice.Reset`, `DcChoice.Reset`). The closures read `t`, the loop's variable, so they see each turn's term as before.

Tests RED → GREEN: `elanrt` `TestChoicesOfTheGeneratedCode` (Reset: RED, does not compile), `gen` `TestTailLoopsAllocateOnce` (a DC loop and a ONE loop of 10^6 turns run by `go test` in the generated module, `testing.AllocsPerRun`: RED 4.00 allocations per turn, GREEN < 0.01; the shape: states before the `for`, resets in it). `go test ./...`, `test_strategies.sh` (96 programs), `test_depth.sh` (20), `test_normalise.sh` (36): 0 failures.

| program (best of 5, real s / max RSS MB) | before | after |
|---|---|---|
| tail 10^7 (`first(dk(dec) ; tail, id)`) | 0.24 / 40 | 0.03 / 5 |
| taildc 10^7 (`dc(dk(dec) ; dc one(taildc), id)`) | 0.33 / 40 | 0.13 / 40 |
| tailone 10^7 (`first one(dc(dec) ; tailone, id)`) | 0.23 / 40 | 0.03 / 5 |
| iterdet 10^7 (not a tail call: `iterate*`) | 0.04 / 5 | 0.04 / 5 |

taildc keeps one allocation per turn: its `ONE(taildc)` is the state of a continuation closure (`k2`), made at each call of the closure, not a state of the loop's body; not hoisted (a closure may run several times in a turn, each needing its own state) — recorded in followups.

- Ruling: the states are reset by name, the top-level hoisted declarations matching `^(dc|one)N := rt.New(Dc|One)(` — why: the declarations removed while emitting (a dc whose alternatives all fail) never get a reset that would name an undeclared variable, and the states inside closures keep their per-call creation — cost if wrong: a state left unreset would keep a previous turn's result; `TestTailLoopsAllocateOnce` checks both kinds and `test_depth`/`test_strategies` the results.

### Task 5 results — tried, not kept

Profile (`GOMAXPROCS=1`, `ELAN_CPUPROFILE`, go1.27.1): the weak cost is in `weak.Make` (`runtime.getOrAddWeakHandle`: a special record from a fixalloc under the global `speciallock`, a padded 16-byte handle allocated, `addspecial` and `specialFindSplicePoint` walking the span's sorted specials list), not in `Value()` (`makeStrongFromWeak` 3 % of deeplist3m). deeplist3m: weak.Make 13 % of the samples, `madvise` of the growing heap 41 %, the GC's mark 26 %; nqueens 12: weak.Make 9 % of the samples (a third of the mutator once the scheduler's idle `kevent` is left out), `specialFindSplicePoint` 7 %.

Variants tried (each alone):
- (a) padding to fewer nodes per span: not measured further — the list walk is only part of the cost (the lock, the handle and the record are per weak pointer), and any padding multiplies the node size (deeplist's RSS rule).
- (b) interning only the nodes compared or looked up again: the IR cannot tell (a node built by a right-hand side may be matched non-linearly, compared by `==` or rebuilt equal later, nqueens: 10.1 M table hits for 0.86 M nodes); a runtime-only variant needs pointer equality to fall back to structural equality — a change of G2, not a spike.
- (c) **slabs**: nodes of arity 0-3 of free symbols allocated in slabs of N `node3`s, the table holding one weak pointer per slab (`weak.Make` once per slab) and the node's index in the top bits of the slot's hash. Complete (a slab lives while any of its nodes is reachable: interior pointers keep the whole object; a dead node of a live slab is still a valid node and may be found again), dead slabs freed. Local branch `s7e-task5-slabs-tried` (65cc5ec, N = 3; the table's unit tests not adapted).

Slab size (the same binary, slabs on/off by a variable; best of 3 with `/usr/bin/time`, s / max RSS MB; off: nqueens 12 0.75 / 10, deeplist3m 1.30 / 901):

| N | 1 | 2 | 3 | 4 | 8 | 16 | 64 |
|---|---|---|---|---|---|---|---|
| nqueens 12 | 0.75 / 10 | 0.67 / 10 | 0.66 / 13 | 0.65 / 18 | 0.64 / 55 | 0.64 / 63 | 0.65 / 71 |
| deeplist3m | 1.30 / 901 | 0.92 / 830 | 0.85 / 809 | 0.79 / 795 | 0.76 / 784 | 0.71 / 790 | 0.67 / 790 |

nqueens's RSS grows with N because a slab holding one long-lived node (a partial board shared by a subtree of the search) keeps its other nodes alive: with N = 64, 326 K live table entries at the end instead of 6.7 K.

The candidate N = 3 (no variable, `node3` slabs of 192 bytes) against s7e-go-switch at 5107ad1, the same generated code built against each runtime, interleaved best of 9 (ms / max RSS MB):

| case | weak per node | slabs of 3 | change |
|---|---|---|---|
| nqueens 12 | 737 / 9.8 | 672 / 13.1 | -8.8 %, RSS 1.34x |
| nqueens 11 | 132 / 9.4 | 122 / 10.7 | -7.6 % |
| deeplist3m | 1263 / 901 | 828 / 809 | -34 % |
| propc 3 | 289 / 42.9 | 289 / 42.8 | 0 |
| propc 2 | 82 / 40.5 | 82 / 40.4 | 0 |
| efib 200 | 78 / 38.8 | 77 / 38.9 | 0 |

Decision rule: deeplist3m <= 0.9 s (yes, 0.83), nqueens 12 >= 10 % (no: 8.8 %; N = 4 gives 12 % but 18 MB, 1.8x > 1.5x), RSS <= 1.5x (yes for N = 3). No variant meets all three.

- Ruling: weak table kept as G2 decided; the slab variant is not merged (plan rule), kept on `s7e-task5-slabs-tried` (`git cherry-pick 65cc5ec`, then adapt the table tests) — why: nqueens 12 gains 8.8 %, under the 10 % the plan set, and every N that reaches 10 % breaks the memory rule — cost if wrong: deeplist3m 0.43 s (34 %) and nqueens 12 0.065 s left on the table; the user may take it at the gate with a revised rule (the memory cost is a slab kept by one live node, at most N = 3 nodes per live node).

### Task 6 results

REM's semantics (termCommon.c `open_file`, `close_file`, `flush_file`, `Getc`, `Putc`; termOut.c `term_write`; termIn.c `term_read`; earley/runtimeInit.cc `earleyCall`), established on dedicated programs run by REM and the interpreter: files are numbers into a table of 300 (0-2 the standard streams, `open` takes the first free one, a closed number is free again); `getc` gives C's char (signed on macOS: byte 0xE9 → -23) or -1 at the end; `putc` writes `(char)c` and gives c (300 → `,`); `open` calls fopen, then on failure writes `Error in open_file(<file>,<mode>)` on stderr and gives -1 (`errno` of stdio is then 1); `close`/`flush` give their argument; `close(stdout)` ends all output (the results too); `write` prints the term as a result (no newline, raw strings) with the results' spacing state (`write(stdout, a)` then the result `re(a,0)`: `a` then `result =  re(a,0)`); `read` parses a term of the sort of its second argument's operator with the query's Earley parser on the file (messages name `file input`), consuming the character after `end`, prints a newline on stdout, and on a syntax error prints `term_read error: earleyCall failed` on stdout and exits 1; effects under backtracking are not undone (a rule printing then failing its condition). The interpreter runs only putc/getc(stdin) as REM (its bugs: followups).

Implemented: `elanrt/io.go` (`Getc`, `Putc`, `Open`, `Close`, `Flush`, `WriteTerm`, `ReadTerm`; stdout is `Out`, stderr unbuffered, files buffered and flushed by flush/close/the end of the program; the standard input shared by the query parser and file 0), `earley.ParseFile` (the input's name), `ir` (`IOGetc` ... `IOFlush`; write/read are operations on terms whose first argument is a file number, total when it is a value; the sorts a program reads are roots of the query rules and their operators all compiled, as for a query the start term uses), `gen` (the calls; read gets the result sort's grammar index). 115, 117, 123 renamed "not planned" (REM has no fun_115/117/123).

Tests RED → GREEN: `elanrt/io_test.go` (RED: does not compile; files written then read, signed chars, end of file, first free number, append, r+, open failures and their messages, the standard streams in a child process, close(stdout), a number not open, files flushed at exit), `ir` `TestIOBuiltins`, `TestReadSortsAreParsed` (RED: do not compile), `TestQueryRules`/`TestUnsupportedConstructsAreRefusedWhenReachable` updated (115/117/123 not planned); `programs/io` (10 programs, `test_strategies.sh -d io`, RED: all refused `not supported yet (S7e): builtin operation code 11x`; GREEN: results equal to REM's, whole output, error output and written files byte for byte, 4 also equal to the interpreter's). `test_strategies.sh`: the programs read /dev/null; `DIR/output` compares the whole output and `<name>.files` the written files with REM's; `DIR/data` copied next to each program. Checked by hand against REM (byte-identical stdout and stderr, same status): a read error at the first term (status 1), a second read after a getc ate the `f` (status 1), read without `end` up to the end of the file then getc -1, close(stdout); the first and the last became `programs/io/io_readbad` and `io_closeout` (with `<name>.status`, Task 7's addition to `test_strategies.sh`; 12 programs).

- Ruling: a file number that is not open (or out of 0..299) stops the program with status 1 and `<op>_builtinStdio: file N is not open` — why: REM dereferences an invalid `FILE *` (segmentation fault, e.g. `getc` after a failed open gives status 139), no defined behaviour to copy — cost if wrong: a program relying on REM's crash gets status 1 instead of 139.
- Ruling: getc gives C's signed char (-128..127, -1 at the end) — why: REM on the reference platforms (macOS arm64, x86) — cost if wrong: on Linux arm64, where REM's char is unsigned, REM gives 0..255 for the same bytes (and 255 at the end: its `char c = getc(fp)` loses EOF there).
- Ruling: read parses a term of the result sort X of `read_builtinIO : (builtinInt X) X` — why: REM uses the sort of the second argument's operator, which is X in a well-sorted program (`IO[X]` passes `XX : X`) — cost if wrong: none for well-sorted programs.
- Ruling: a program that reads terms compiles every operator of the sorts it reads (Task 2's "start term ignores the query" economy does not apply) — why: the term read flows into the program, a refused operator would make a correct read fail at run time — cost if wrong: such a program may box an int family (slower), never a wrong result.

### Task 7 results

REM's semantics (termBase.c `term_newArray`, `term_getArray`, `term_setArray`, `term_getLength`; termCommon.c `print_f_prefix`; termBase.c `term_notDestructEqual`, `hashTerm`), established on the Compiler.4.0/Work programs and on dedicated ones (`programs/arrays/arrtest.eln`): an array is mutated in place, the mutation is seen by the terms holding it and is not undone by backtracking (a rule that sets then fails its condition: the next branch reads the new element); get/set out of range print `getArray error: size = 2\tn = 5` (`setArray ...`) on stdout and exit 1; get/set/length on a term that is not an array print `getArray error: symb = <its .ref code>` and exit 1; printing an array (a result, alone or inside a term) prints `print_f_prefix Array not yet implemented` after what was printed (`result = h(`) and exits 1; the array operations count no rewrite step; `==` on two arrays is always true (a REM bug, followups). The interpreter leaves the operations as terms (its codes are 300-303). Work's programs: testArray `c` (2 steps), testVector `c` (21), testMatrice `c` (10), testHashSet `[true]` (8) — and testMatrix `empty` (42), a fifth one REM runs (testNArray1: REM's parser fails). testHashSet needs builtinHashTerm (code 210, a kept refusal "not planned": REM's value depends on its representation): REM's `hashTerm` is a Jenkins hash of the .ref symbol codes and REM's tagged ints, deterministic except for strings and arrays, so it was ported (`elanrt/hash.go`; the values of a REM program printing hashes of constants, terms, ints and an AC term are equal).

Implemented: `elanrt/array.go` (an array is a node of the builtin symbol `array` (REM's CODE_ARRAY 204, `SymArray`, `FirstSym` now 6) outside the unique table: its Int the size given to new, its Args the elements, its Hash a number of its own), `NewArray`, `ArrayGet`, `ArraySet`, `ArrayLength`, `ArrayEq` (the `==` and non-linear patterns of an array sort: same size and equal elements; `Eq` stays identity, small enough to be inlined: with the array case it was not, propc 3 +13 %), `Compare` (two arrays of equal contents ordered by their own hashes), the session's printer stops on an array as REM's (`Writer.onArray`, also for write); `elanrt/hash.go` (`HashTerm`); `ir` (`ArrayNew`..`ArrayLength`, `TermHash`: operations on terms whose `IntArgs` are builtinInt values (`FileArg` of Task 6 generalised), total when those are values); `gen`. `test_strategies.sh`: `<name>.status` (a run-time error of REM's: the status, then the whole output compared with REM's).

Tests RED → GREEN: `elanrt/array_test.go` (RED: does not compile; new/get/set/length, two new two arrays, a node holding an array shared by identity and seeing the mutations, equality of contents, REM's errors and the printing of an array in child processes), `hash_test.go` (REM's values; RED: does not compile), `ir` `TestArrayBuiltins` (RED: does not compile; codes 200-203 and 210, int arguments, totality), the refusal table of `TestUnsupportedConstructsAreRefusedWhenReachable` (200 and 210 removed); `programs/arrays` (17 programs, `test_strategies.sh -d arrays`; RED: 16 refused `not supported yet (S7e): builtin operation code 200`/`201`, testHashSet then `not planned ... 210`): the five Work programs with REM's results and rewrite steps (`rem-steps`), arr_set, arr_length, arr_backtrack, arr_shared, arr_eq, arr_hash equal to REM's, arr_neq `.expected` (`.no-rem`: REM's bug), arr_print, arr_inside, arr_range, arr_setrange, arr_stuck with REM's status 1 and its output byte for byte.

- Ruling: two arrays are equal (`==` and non-linear patterns on an array sort, `rt.ArrayEq`; `rt.Eq` elsewhere compares them by identity) when they are the same or have the same size and equal elements — why: REM finds any two arrays equal (a bug), the interpreter compares the terms `new(...)` structurally; content equality agrees with both where REM is right (`arr_eq`: rb(true,true)) and with the interpreter where REM is wrong (`arr_neq`: rb(true,false)) — cost if wrong: a program comparing two distinct arrays of equal contents gets true where identity would give false (no oracle gives false).
- Ruling: a term holding an array is a shared node keyed by the array's identity (not built outside the table as the plan proposed) — why: equal arrays-by-identity make equal parents, the parent sees the array's mutations through its pointer (`arr_shared`: REM's c), and no path of the runtime has to handle unshared nodes — cost if wrong: `h(a1) == h(a2)` is false for two distinct arrays of equal contents (the interpreter: true).
- Ruling: builtinHashTerm (code 210) is compiled, with REM's values (port of `hashTerm`) except for strings and arrays — why: testHashSet (Work, named by this task) needs it; REM's values are deterministic for every term but those, so its outputs are reproducible byte for byte — cost if wrong: a program printing the hash of a string or of an array gets another value than REM's (whose value depends on its memory).
- Ruling: `new(n, x)` with n < 0 makes an array of no element that reports size n in its errors — why: REM's allocation then fails (GC warnings on stderr) and the first get reports `size = -1`; the warnings are not reproduced — cost if wrong: REM's GC warnings missing on stderr.

Performance (the same generated code built against each runtime, interleaved best of 21): nqueens 12 732 ms (Task 6's runtime) → 738 ms; a first version that checked a "stopped" flag after printing each result cost 18 ms there (the result printer no longer inlined), replaced by a callback of the session's printer. propc 3, efib 200 unchanged (288, 78 ms). `bench_perf.sh --check` exit 0.

### Tasks 4-7: check

`make check` (macOS, after `make install`): legacy bench 0 regressions, 0 snapshot differences; `bench_parity.py --kind G` PASS=78 PASS~=1 PASS≈=53 FAIL=13 (no `PARITY:` line, no UNSUPPORTED); BenchThesis G 22 ok (steps equal to J's); `test_query.sh` 49 programs / 152 queries, `test_depth.sh` 20, `test_strategies.sh` (strategies, strategy_terms, ac, io 12, arrays 17): 0 failures. `bench_perf.sh build/install 5 --check`: exit 0 (nqueens 12 0.73 s, deeplist3m 1.26 s, propc 3 0.28 s, efib 200 0.07 s).

### Task 8 results

Evidence of parity (Step 1): the 79 cases of `bench_r_expected.tsv` (36 PASS, 1 PASS~, 27 PASS≈, 15 ERROR) all have the same status in `bench_g_expected.tsv` (160 cases: 78 PASS, 1 PASS~, 53 PASS≈, 13 FAIL, 15 ERROR); none worse, none missing. The only `.no-elanc-go` program is `programs/refused/ac`, a refusal of elanc-rs (AC) that elanc-go compiles.

Changed: `CMakeLists.txt` `option(ELAN_RUST ... OFF)`; `Makefile` `check-rust` runs `cargo test`, the round trip (`rust` reader) and the scripts (`ELANC=elanc-rs`, `bench_parity.py --kind R`) only when `$(PREFIX)/bin/elanc-rs` exists, else prints `elanc-rs not built (-DELAN_RUST=OFF, the default since S7e): Rust tests skipped`; `ELAN_CONFIG_EPOCH` in the configuration key (a build directory configured before has `ELAN_RUST=ON` in its CMake cache: it is recreated once); `test_normalise.sh`, `test_strategies.sh`, `test_depth.sh` default to `ELANC=elanc-go`, `bench_parity.py` to `--kind G`; CI: a `workflow_dispatch` input `rust` (`gh workflow run ci.yml -f rust=true`) adds a step `make BUILD=build-rust ... CMAKE_FLAGS=-DELAN_RUST=ON check-rust`; README (elanc-rs paragraph), `docs/s6-rust-status.md`, spec G7.

How to run the Rust tests: `make BUILD=build-rust CMAKE_FLAGS=-DELAN_RUST=ON check-rust` (or `check`; a separate `BUILD` keeps `build/`, since changing `CMAKE_FLAGS` recreates the build directory).

Checked: `make install` (build/ recreated by the epoch: `bin/` has elan, elanc, elanc-go, no elanc-rs), `make check-rust`: the skip message; `make BUILD=build-rust CMAKE_FLAGS=-DELAN_RUST=ON check-rust`: exit 0 (cargo test 16 suites ok, round trip ok, test_normalise 23 programs, test_strategies 39, strategy_terms 95, test_depth 19: 0 failures; bench R PASS=36 PASS~=1 PASS≈=27 ERROR=15, as `bench_r_expected.tsv`).

- Ruling: the Rust tests in CI run only on demand (`workflow_dispatch`, input `rust`), not on every push — why: G7 makes them optional, and a second build per runner doubles CI time for frozen code — cost if wrong: a change breaking elanc-rs is seen only when someone runs the workflow with `rust`.
- Ruling: the configuration key gets an epoch so that existing build directories pick up the new default — why: `option()` keeps the cached ON, so `make check` on an existing `build/` would still build and test elanc-rs — cost if wrong: one full rebuild of each existing build directory.

### The remaining S7e refusals (acceptance: "the other S7e refusals renamed not planned")

After Tasks 6-7, `grep -n 'unsupported("S7e"' src/compiler-go/ir/*.go` found five sites (`builtinRefusal`'s default, rules for a builtin operation, the two checks of `checkAC`, the identifier in an AC pattern) and `ruleLowerer.refuse` (acallrep, acnested, a where pattern with an AC operator of a builtin sort). None is implemented: no program of the tests, of the bench or of the library needs one, and only acallrep/acnested have an oracle program (both oracles agree there). All are `NotPlanned` now, with their reason; no refusal of the IR names a stage.

Tests RED → GREEN: `ir` `TestKeptRefusalsAreNotPlanned` (RED: code 155, rules for `+` on builtinInt and an AC operator `(nat set) set` refused as S7e; the scan of the IR's sources for `unsupported("S7[a-e]"`), the code-155 case of `TestUnsupportedConstructsAreRefusedWhenReachable`; `test_normalise.sh` now accepts only `not supported (not planned)` for elanc-go's `programs/refused` (RED: acallrep, acnested "not supported yet (S7e)"). `docs/followups.md` "elanc-go: kept refusals" lists each with its reason.

- Ruling: acallrep and acnested (AC shapes REM transforms before matching) and the where pattern with an AC operator of a builtin sort: not planned — why: a port of REM's bipartite matcher and of `RewriteRule.java` transformation2 for two contrived programs written to find the limit, no real program — cost if wrong: status 2 for a program with such a pattern (REM computes it).
- Ruling: an identifier in an AC pattern: not planned — why: REM's order of a tagged identifier among its matchers' items is not established and no oracle program exists; guessing it could give solutions in another order than REM's — cost if wrong: status 2 for such a program.
- Ruling: an AC operator whose arguments are not of its result sort: not planned — why: AC terms are flattened as `(s s) s`; neither the bench nor the library declares another profile, and what the oracles do with it is not established — cost if wrong: status 2.
- Ruling: rules for a builtin operation: not planned — why: no program has one, and whether each oracle applies the builtin or the rules first is not established — cost if wrong: status 2 for such a program.
- Ruling: any other builtin code: not planned — why: no module of the library declares one (REM's ASF list codes 192-199 and the interpreter's 79-82, 155, 179-190, 205-208 are not reachable from `elanlib`); the I/O codes, arrays and builtinHashTerm are compiled — cost if wrong: status 2 for a user module declaring such a code.
- Ruling: the check "AC operator printed without its two arguments" removed rather than renamed — why: unreachable: a candidate's argument sorts are read from the `LexType`s of its printing form, so a form of the profile always has the two arguments the first check of `checkAC` requires — cost if wrong: none (an AC operator with a form of another arity would already fail the first check).

### Task 9 results

`src/compiler/CMakeLists.txt` installs the generated driver twice, `bin/elanc` and `bin/elanc-rem` (`cmp`: identical; `bin/elanc` unchanged, its source `scripts/elanc.src` untouched); `Makefile`: `install` removes both, `CHECK_COMPILER` tests `bin/elanc-rem`; call sites renamed: `run_tests.py` J/JO, `tests/compiler/test_elanc.sh`, `test_runtime_strategies.sh`, `test_start_term.sh`, `test_benchthesis.sh`, the REM oracle of `test_normalise.sh`, `test_strategies.sh`, `test_query.sh` (`command -v elanc-rem`), `bench_jg.py` (always `elanc-rem`: Task 1's fallback to `elanc` removed), messages of `bench_parity.py`; `reference/build.sh` gets a component `elanc_rem` (a link `elanc-rem -> elanc` in the 2004 install; `reference/build.sh elanc_rem` adds it to an existing one) so that `make check-reference` runs the J kinds by the same name; docs: README (the driver is also installed as `elanc-rem`), `tests/legacy-bench/README.md`, `reference/README.md`.

Tests RED → GREEN: `test_elanc.sh` 0 (`elanc-rem` prints REM's usage, `Reduce ELAN Machine`; `elanc-rem -nosplit -quiet enum` writes `enum.make` and `.elan.enum/Makefile`; RED: not installed); `tests/architecture/check_rem_name.py` (+ `test_check_rem_name.py`, 3 unit tests), run by `make check-arch`: no line of a script (`*.sh`, `*.py`, `Makefile`, `*.yml`, `Dockerfile*`) under `tests/`, `ci/`, `.github/`, `reference/` or of the top `Makefile` names the bare `elanc` outside a comment, unless marked `bare-elanc-ok` (the install's removal of `bin/elanc`, the reference's link); RED: 41 lines, GREEN: 0.

Verification that no test depends on what `elanc` is: `build/install` copied to a scratch prefix whose `bin/elanc` was replaced by a stub that logs its call and exits 97; every test of `make check` that uses the prefix run against it (smoke, golden 55 cases, `test_elanc.sh`, `test_runtime_strategies.sh`, `test_start_term.sh`, `test_benchthesis.sh` J/JO and G, round trip (go), `test_normalise.sh` 36, `test_strategies.sh` 96 / strategy_terms 100 / ac 48 / io 12 / arrays 17, `test_depth.sh` 20, `bench_parity.py --kind G` (PASS=78 PASS~=1 PASS≈=53 FAIL=13), `test_query.sh` 49 programs / 152 queries, bench I,A and J,JO (0 regressions), `bench_jg.py --filter Compiler.4.0/Test --check`): all exit 0, the stub called 0 times; the stub prefix then removed.

- Ruling: the check of bare `elanc` is a line scan of scripts (comments and marked lines skipped), not a parse — why: the call sites are shell, Python, Make and YAML; a word-boundary regexp (`elanc` not part of `elanc-go`, `elanc.log`, `$ELANC`) found every call site of finding 6 with no false positive outside comments — cost if wrong: a call written so that the regexp misses it (e.g. a name built from parts) would go unnoticed; the stub verification above is the end-to-end check.
- Ruling: the reference install gets an `elanc-rem` link rather than the bench falling back to `elanc` when `elanc-rem` is missing — why: no test may depend on what `elanc` is, and a fallback would silently test Go after a switch in a prefix without `elanc-rem` — cost if wrong: an existing reference install needs `reference/build.sh elanc_rem` once before `make check-reference`.
