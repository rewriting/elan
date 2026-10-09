# S2 — Sanitising the modern interpreter: Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans (chosen: inline). Steps use checkbox (`- [ ]`) syntax.

**Goal:** the interpreter compiles in strict C++17/C17 with `-Wall -Wextra -Werror` under GCC and Clang, runs the bench clean under ASan+UBSan, without dead configuration code, and with unchanged behaviour.

**Architecture:** mechanical, behaviour-preserving steps first (`unifdef`, standard headers, removal of `-fpermissive`/`-fcommon`), each gated by `make check` (exact snapshots); then warnings; then a second compiler; then sanitizers, where each report is fixed at its root cause with the snapshot policy.

**Tech Stack:** GCC 16, Apple Clang 17 / Linux Clang, CMake, `unifdef`, ASan/UBSan, the S1 bench.

**Spec:** `docs/superpowers/specs/2026-10-09-s2-sanitising-design.md`

## Global Constraints

- Branch `s2-sanitising`; every commit keeps `make check` at 0 regressions / 0 snapshot differences, except commits that deliberately refresh snapshots (CONTRIBUTING rule 4).
- `legacy/`, `reference/` untouched.
- `-fsigned-char` stays (D3). ATerm/REF export code stays (D7).
- Commit messages state the behaviour change; end with `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`.

## Review Focus

1. `unifdef` must not touch header guards or macros defined in the sources → Task 1 step 1 lists `#define`d names; header guards are not in the `-U` list.
2. Replacing `iostream.h` must not change output formatting (e.g. `hex`/`dec` state, `form()`) → exact snapshots in `make check`.
3. Removing `-fcommon` must not merge or duplicate globals differently → link errors are explicit; `make check` exact.
4. Clang may evaluate unspecified-order expressions differently (argument evaluation order) → `make check` under Clang must also be 0/0; any difference is a bug to fix (UB/unspecified order), not an exception.
5. Sanitizer fixes may change outputs → D6 procedure.

---

### Task 1: Remove dead configuration code (`unifdef`)

**Files:** all of `src/interpreter/{src,parser,acmatcher}`; `CMakeLists.txt`, `src/interpreter/CMakeLists.txt`.

- [ ] Step 1: Check the macro lists: none of `ALPHA COMMAND PEM BORO ANYS SYMBS EARLEY TO_BE_DISTRIBUTED META PEVAL` and of the `-U` list of the spec is `#define`d in the sources (`grep -rE '^\s*#\s*define\s+NAME\b'`); only `DEEP` is (excluded).
- [ ] Step 2: Run, for every file containing `#if`:
  `unifdef -m -DALPHA -DCOMMAND -DPEM -DBORO -DANYS -DSYMBS -DEARLEY -DTO_BE_DISTRIBUTED -DMETA -DPEVAL -URUNTIME -UGCMEM -USTORM -UDEBUG -UDEBUG2 -UDEBUG3 -UVISIGRAPH -UMEMORY -UPICALC -UBLABLA -ULEX_BLABLABLA -UCONCUR_BLABLA -USUN -U__DECCXX -UPURIFY -UJUNK -UJUNK_CODE -UJUNK_1006 -UTO_BE_REMOVED -UORIGINAL_VERSION -UOLD_HISTORY -UOLDHISTORY -UONLY_FOR_DEBUG -UWASDONE -UMARIANS -UNO_MORE_SWITCH -URSWITCH -UVRSION1901 -UONE_HISTORY -UHISTORY -UBINS FILE` (exit status 1 = changed, 0 = unchanged, 2 = error → stop).
- [ ] Step 3: Remove those names from `ELAN_INTERP_DEFINITIONS` in `src/interpreter/CMakeLists.txt` (keep `PREFIX`, `PACKAGE`, `VERSION`, `YYTEXT_POINTER`).
- [ ] Step 4: `make check` → 0/0; generated parser sources identical to before (`cmp` against a copy taken before Step 2).
- [ ] Step 5: Commit "src: remove dead configuration code with unifdef (behaviour change: none)".

### Task 2: Standard C++ headers instead of `compat/`

**Files:** create `src/interpreter/src/elan_std.h`; modify the files including `iostream.h`, `fstream.h`, `stream.h`, `malloc.h`; delete `src/interpreter/compat/`; `src/interpreter/CMakeLists.txt` (drop `compat` include dir).

- [ ] Step 1: Remove the `compat` include directory from CMake; build → fails on `iostream.h` (RED).
- [ ] Step 2: `elan_std.h` = `#include <iostream>`, `<iomanip>`, `<fstream>`, `<cstdlib>` + `using std::cout; using std::cerr; using std::endl; ...` for exactly the names the build reports as undeclared; replace each `#include <iostream.h>`/`<fstream.h>`/`<stream.h>` by `#include "elan_std.h"`, `<malloc.h>` by `<cstdlib>`.
- [ ] Step 3: Build (still `-std=gnu++98 -fpermissive`) and `make check` → 0/0. Delete `compat/`. Commit.

### Task 3: Strict C++17 (no `-fpermissive`)

- [ ] Step 1: Set `ELAN_CXX_COMPAT_OPTIONS` to `-std=c++17 -fsigned-char -w`; build → errors (RED: extra qualifications in `stringtab.h`, `mitab.h`, `commondefs.h`, ...).
- [ ] Step 2: Fix each error with the smallest standard construct (remove `class::` qualification inside class bodies; declare before use; complete types; `const char*` for literals where an error). Never change logic.
- [ ] Step 3: Build, `make check` → 0/0. Commit.

### Task 4: Strict C17 (no `-fcommon`)

- [ ] Step 1: `ELAN_C_COMPAT_OPTIONS` = `-std=c17 -fsigned-char -w`, and drop `-fcommon` from C++ too if present; link → multiple-definition errors (RED) if any.
- [ ] Step 2: For each duplicated global: one definition in a `.c`/`.cc`, `extern` declaration in the header.
- [ ] Step 3: `make check` → 0/0. Commit.

### Task 5: `-Wall -Wextra -Werror`

- [ ] Step 1: Replace `-w` by `-Wall -Wextra -Werror` (C and C++), plus `-Wno-missing-field-initializers` on the generated parser sources only (`set_source_files_properties`). Build → warnings as errors (RED).
- [ ] Step 2: Fix warnings by category: `-Wwrite-strings` (`const char*`), `-Wunused-*` (remove or `(void)`), `-Wparentheses`/`-Wdangling-else`/`-Wmisleading-indentation` (add braces/parentheses reproducing the *current* parse — the compiler's interpretation is the behaviour of reference), `-Wformat` (fix the format to the argument type), `-Wint-to-pointer-cast` (use `intptr_t`). Never change logic.
- [ ] Step 3: `make check` → 0/0. Commit per category.

### Task 6: Clang

- [ ] Step 1: `make BUILD=build-clang ELAN_CC=clang ELAN_CXX=clang++ check PREFIX=$PWD/build-clang/install` → fix errors/warnings Clang adds (same rules as Task 5).
- [ ] Step 2: 0 regressions / 0 snapshot differences under Clang; any difference = unspecified/undefined behaviour → fix at the root (Review Focus 4).
- [ ] Step 3: Makefile default on macOS becomes the system compiler (`cc`/`c++` = Clang) once both pass; README updated (Homebrew GCC no longer required for the modern interpreter). CI: add a Clang build+check on Linux. Commit.

### Task 7: Sanitizers

- [ ] Step 1: CMake option `ELAN_SANITIZE` adding `-fsanitize=address,undefined -fno-omit-frame-pointer -fno-sanitize-recover=undefined` to compile and link; Makefile target `check-sanitize` (BUILD=build-san, PREFIX=build-san/install, `ASAN_OPTIONS=detect_leaks=0` because the interpreter never frees by design, `UBSAN_OPTIONS=print_stacktrace=1`). The runner must treat any sanitizer report (stderr containing `ERROR: AddressSanitizer` or `runtime error:`) as a test failure: unit test first.
- [ ] Step 2: Run `make check-sanitize`; for each report: reproduce on the single test, find the root cause, fix, rerun `make check` (exact) and `make check-sanitize`. If an output changes: D6.
- [ ] Step 3: Clean run → commit. CI: run `check-sanitize` on Linux.

### Task 8: Documentation

- [ ] README (requirements: no Homebrew GCC needed for the interpreter; `make check-sanitize`), CONTRIBUTING (warnings are errors; sanitizer clean), spec status → done. Commit, push branch, CI green, merge to main.
