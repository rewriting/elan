# S1 — Foundations of the modern ELAN interpreter: Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** build the (unchanged) ELAN interpreter from `src/` with CMake, check it with `make check` against the reference snapshots, make the reference build on Linux too, and run all of it in CI on macOS and Linux.

**Architecture:** the interpreter sources are imported verbatim (legacy + reference patch 01) into `src/interpreter/`; a CMake build reproduces the 2003 automake rules (host tools `macc`/`mtokdef`/`mabident`, parser-table generation, `libmatch`, `elan`); a `Makefile` façade drives CMake, the reference build and the test bench; `tests/legacy-bench/run_tests.py` gains regression cases, platform exceptions and a case-sensitivity guard; GitHub Actions runs everything on `ubuntu-24.04` and `macos-15`.

**Tech Stack:** C/C++ (GCC, pre-standard modes kept in S1), CMake ≥ 3.20, GNU make, Python 3 (stdlib only, `unittest`), bash, Docker (local Linux checks), GitHub Actions.

**Spec:** `docs/superpowers/specs/2026-10-09-s1-foundations-design.md`

## Global Constraints

- The repository must be on a case-sensitive file system (`~/github/elan` is a case-sensitive APFS volume; see README).
- `legacy/` is never modified. `reference/patches/01..05` are frozen; a reference portability fix is a new numbered patch.
- S1 does not change the interpreter's C++ code: `src/interpreter/` must stay identical to legacy + patch 01 (+ `compat/`).
- Compatibility flags kept in S1: C++ `-std=gnu++98 -fpermissive -w -I compat`, C `-std=gnu89 -w -fcommon`.
- Interpreter definitions: `-DALPHA -DCOMMAND -DPEM -DBORO -DANYS -DSYMBS -DEARLEY -DTO_BE_DISTRIBUTED -DMETA -DPEVAL -DPREFIX="<install prefix>"`, plus `-DPACKAGE="elan-interpreter" -DVERSION="3.6g" -DYYTEXT_POINTER=1`.
- Compilers: macOS → Homebrew GCC (`gcc-16`/`g++-16` by default, overridable with `ELAN_CC`/`ELAN_CXX`); Linux → distribution `gcc`/`g++`.
- One set of snapshots (made on macOS). Linux differences are analysed and listed in `tests/legacy-bench/platform-exceptions.tsv`; never a second snapshot set.
- Success: `make check` (kinds I and A, 368 tests) → 0 regressions, 0 snapshot differences (except documented exceptions), on macOS and Linux; `make check-reference` (778 tests) → same on both.
- Every commit keeps `make check` green; commit messages end with `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`.

## Review Focus

1. `make check` run from a case-insensitive directory must stop with a clear error, not produce wrong results silently → Task 1 (guard + unit test).
2. An installed `elan` run **without** `ELANLIB` must find its library under its install prefix → Task 4, Step 6.
3. Editing a grammar (`*.t`) must regenerate the parser sources on the next build → Task 4, Step 7.
4. A build must not write into the source tree (generated files live in `build/`) → Task 3, Step 6 and Task 4, Step 8 (`git status` clean).
5. A missing toolchain (e.g. no `gcc-16`) must fail early with a clear message → Task 5, Step 3.

---

## File structure

| Path | Responsibility |
|------|----------------|
| `tests/legacy-bench/run_tests.py` (modify) | test runner: + regression cases, platform exceptions, case-sensitivity guard |
| `tests/legacy-bench/test_run_tests.py` (create) | unit tests of the runner (stdlib `unittest`) |
| `tests/legacy-bench/platform-exceptions.tsv` (create) | documented per-platform snapshot exceptions |
| `tests/regression/README.md` (create) | format of regression cases (directory empty otherwise) |
| `src/interpreter/{src,parser,acmatcher,compat}/` (create) | verbatim interpreter sources (legacy + patch 01) and C++ shims |
| `src/lib/elanlib/{common,noquote,ref,strategy}/` (create) | verbatim standard library |
| `src/interpreter/gen-parsers.sh` (create) | generates parser sources exactly like the 2003 `$(generated_c)` rule |
| `CMakeLists.txt` (create) | top-level build |
| `src/interpreter/CMakeLists.txt` (create) | host tools, generation, `libmatch`, `elan`, install |
| `Makefile` (create) | façade: `all install check reference check-reference clean` |
| `reference/build.sh` (modify) | platform defaults for macOS and Linux |
| `reference/patches/06-rem-linux-makefile.patch` (create) | Linux branch of REM's generated Makefile |
| `ci/Dockerfile.linux` (create) | Ubuntu 24.04 image used for local Linux checks (same packages as CI) |
| `.github/workflows/ci.yml` (create) | CI matrix macOS + Linux |
| `README.md`, `CONTRIBUTING.md` (modify/create) | how to build/test; working rules |

---

### Task 1: Runner — regression cases, platform exceptions, case-sensitivity guard

**Files:**
- Modify: `tests/legacy-bench/run_tests.py`
- Create: `tests/legacy-bench/test_run_tests.py`, `tests/legacy-bench/platform-exceptions.tsv`, `tests/regression/README.md`

**Interfaces:**
- Produces (module `run_tests`):
  - `samples(d: Path) -> Path` — `d/SAMPLES` (or `Sample`, `Samples`) if it exists, else `d` itself.
  - `discover_regression(root: Path = REGRESSION) -> list[dict]` — one test dict per subdirectory containing `prog.lgi`: `id="regression/<name>::I::prog:no:input:expected"`, `dir`, `kind="I"`, `lgi="prog"`, `spc="no"`, `inp="input"`, `out="expected"`, `flags=[]`, `long=False`.
  - `load_exceptions(path: Path = EXCEPTIONS, platform: str = sys.platform) -> dict[str, str]` — test id → reason, for lines whose platform column is `platform` or `all`.
  - `case_sensitive(directory: Path) -> bool`.
  - CLI: exits with status 2 and message `ERROR: <dir> is on a case-insensitive file system` when `WORK` is not case-sensitive (except with `--list`); snapshot differences of excepted tests are printed as `SNAP-EXC` and not counted.

- [ ] **Step 1: Write the failing tests**

Create `tests/legacy-bench/test_run_tests.py`:

```python
#!/usr/bin/env python3
"""Unit tests of run_tests.py (stdlib unittest; run: python3 test_run_tests.py)."""
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import run_tests as rt  # noqa: E402


class SamplesTest(unittest.TestCase):
    def test_uses_samples_directory_when_present(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            (Path(d) / "SAMPLES").mkdir()
            self.assertEqual(rt.samples(Path(d)), Path(d) / "SAMPLES")

    def test_falls_back_to_directory_itself(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            self.assertEqual(rt.samples(Path(d)), Path(d))


class RegressionDiscoveryTest(unittest.TestCase):
    def test_finds_cases_with_prog_lgi(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            case = Path(d) / "dot-printing"
            case.mkdir()
            (case / "prog.lgi").write_text("LPL prog description end\n")
            tests = rt.discover_regression(Path(d))
            self.assertEqual(len(tests), 1)
            t = tests[0]
            self.assertEqual(t["id"], "regression/dot-printing::I::prog:no:input:expected")
            self.assertEqual((t["kind"], t["lgi"], t["spc"], t["inp"], t["out"]),
                             ("I", "prog", "no", "input", "expected"))
            self.assertEqual(t["dir"], case)

    def test_ignores_directories_without_prog_lgi(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            (Path(d) / "notes").mkdir()
            self.assertEqual(rt.discover_regression(Path(d)), [])

    def test_missing_root_gives_no_case(self):
        self.assertEqual(rt.discover_regression(rt.HERE / "does-not-exist"), [])


class ExceptionsTest(unittest.TestCase):
    def test_filters_by_platform(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as d:
            f = Path(d) / "exc.tsv"
            f.write_text("# id\tplatform\treason\n"
                         "a::I::x\tlinux\tprintf rounding\n"
                         "b::I::y\tdarwin\tsomething\n"
                         "c::I::z\tall\tundefined behaviour, see S2\n")
            self.assertEqual(rt.load_exceptions(f, "linux"),
                             {"a::I::x": "printf rounding",
                              "c::I::z": "undefined behaviour, see S2"})

    def test_missing_file_means_no_exception(self):
        self.assertEqual(rt.load_exceptions(rt.HERE / "nope.tsv", "linux"), {})


class CaseSensitivityTest(unittest.TestCase):
    def test_repository_volume_is_case_sensitive(self):
        self.assertTrue(rt.case_sensitive(rt.HERE / "work"))

    @unittest.skipUnless(sys.platform == "darwin", "macOS default volume only")
    def test_macos_temp_dir_is_not(self):
        self.assertFalse(rt.case_sensitive(Path(tempfile.gettempdir()) / "elan-case-probe"))


if __name__ == "__main__":
    unittest.main()
```

- [ ] **Step 2: Run the tests to verify they fail**

Run: `cd ~/github/elan/tests/legacy-bench && python3 test_run_tests.py`
Expected: errors such as `AttributeError: module 'run_tests' has no attribute 'discover_regression'` (and `samples` fallback test FAIL).

- [ ] **Step 3: Implement in `run_tests.py`**

(a) Next to the `SNAPDIR`/`WORK` constants add:

```python
REGRESSION = REPO / "tests" / "regression"
EXCEPTIONS = HERE / "platform-exceptions.tsv"
```

(b) Replace the body of `samples(d)` so that the last line falls back to the directory itself:

```python
def samples(d):
    for name in ("SAMPLES", "Sample", "Samples"):
        if (d / name).is_dir():
            return d / name
    return d
```

(c) Add after `discover()`:

```python
def discover_regression(root=REGRESSION):
    """tests/regression/<case>/{prog.lgi,*.eln,input.inp,expected.out}: one I test per case."""
    if not root.is_dir():
        return []
    tests = []
    for d in sorted(p for p in root.iterdir() if p.is_dir()):
        if (d / "prog.lgi").exists():
            tests.append(dict(id=f"regression/{d.name}::I::prog:no:input:expected", dir=d,
                              kind="I", lgi="prog", spc="no", inp="input", out="expected",
                              flags=[], long=False))
    return tests


def load_exceptions(path=EXCEPTIONS, platform=None):
    """platform-exceptions.tsv: test-id <TAB> darwin|linux|all <TAB> reason."""
    platform = platform or sys.platform
    exc = {}
    if path.exists():
        for line in path.read_text().splitlines():
            if not line.strip() or line.startswith("#"):
                continue
            tid, plat, reason = line.split("\t", 2)
            if plat in (platform, "all"):
                exc[tid] = reason
    return exc


def case_sensitive(directory):
    directory.mkdir(parents=True, exist_ok=True)
    probe = Path(tempfile.mkdtemp(prefix="case-", dir=directory))
    try:
        (probe / "a").touch()
        (probe / "A").touch()
        return len(list(probe.iterdir())) == 2
    finally:
        shutil.rmtree(probe, ignore_errors=True)
```

(d) In `main()`, replace the line building `tests = [...]` by:

```python
    tests = [t for t in discover() + discover_regression()
             if t["kind"] in kinds and (a.long or not t["long"]) and re.search(a.filter, t["id"])]
```

and right after the `if a.list: ... return 0` block insert:

```python
    if not case_sensitive(WORK):
        print(f"ERROR: {WORK} is on a case-insensitive file system "
              "(ELAN sources contain files differing only by case; see README)")
        return 2
    exceptions = load_exceptions()
```

(e) In the result loop, replace

```python
                if sst == "SNAP-DIFF":
                    snapdiff.append(t["id"])
```

by

```python
                if sst == "SNAP-DIFF" and t["id"] in exceptions:
                    sst = "SNAP-EXC"
                if sst == "SNAP-DIFF":
                    snapdiff.append(t["id"])
```

(f) Create `tests/legacy-bench/platform-exceptions.tsv`:

```
# Snapshot differences accepted on one platform, each analysed and justified.
# test-id<TAB>darwin|linux|all<TAB>reason
```

(g) Create `tests/regression/README.md`:

```markdown
# Regression tests

One directory per case, added whenever a bug is fixed or a subtle behaviour is
pinned down. A case is an interpreter (`I`) test run by
`tests/legacy-bench/run_tests.py`:

    <case>/prog.lgi       LPL description (logic name: prog)
    <case>/*.eln          modules it imports
    <case>/input.inp      query terms, as for `elan -b prog.lgi < input.inp`
    <case>/expected.out   expected standard output
    <case>/README         one line: what the case checks and why
```

- [ ] **Step 4: Run the unit tests and the bench**

Run: `cd ~/github/elan/tests/legacy-bench && python3 test_run_tests.py && ./run_tests.py --prefix ../../reference/install | tail -4`
Expected: `OK` (7 tests); bench ends with `vs baseline: 0 regressions, 0 improvements` and `vs snapshots: 0 differences`.

Run: `cd /tmp && python3 ~/github/elan/tests/legacy-bench/run_tests.py --filter Simplest; echo rc=$?`
Expected: still works (WORK is in the repo, so guard passes) — `rc=0`.

- [ ] **Step 5: Commit**

```bash
cd ~/github/elan
git add tests/legacy-bench/run_tests.py tests/legacy-bench/test_run_tests.py \
        tests/legacy-bench/platform-exceptions.tsv tests/regression/README.md
git commit -m "tests: regression cases, platform exceptions and case-sensitivity guard in the bench runner

Behaviour change: none for existing tests.

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 2: Import the interpreter and library sources verbatim

**Files:**
- Create: `src/interpreter/src/`, `src/interpreter/parser/`, `src/interpreter/acmatcher/`, `src/interpreter/compat/`, `src/lib/elanlib/`

**Interfaces:**
- Produces: the source tree used by Tasks 3–4. Source lists (from the 2003 `Makefile.am` files):
  - `parser/` (`macc`): `globdef.c rtglobde.c mallo.c tiderr.c mlexan.c dump.c envir.c gen.c rwtab.c ingram.c init.c macc.c trapsa.c mh.c`
  - `acmatcher/matcher/` (`libmatch`): `build_free.c solve_free.c build_match.c flatten.c solve_pure.c tools.c build_pure.c gccmatch.c`
  - `src/` common: `commondefs.cc ochstream.cc ichstream.cc lstream.cc stringtab.cc mitab.cc mallo.cc misc.cc`
  - `src/` `mtokdef`: `mtokdefmain.cc specials.cc` + common; `mabident`: `mtokmain.cc specials.cc` + common
  - `src/` `elan`: `ldmain.cc msemact.cc msemact3.cc termcompile.cc matchcompile.cc compilemisc.cc strategy.cc grammar.cc earley.cc esemact.cc term.cc module.cc tmisc.cc rtmisc.cc match.cc trsystem.cc normalise.cc aterm.cc partial.cc stateofexecution.cc meta.cc lbuffer.cc mlstream.cc command.cc` + common
  - grammar inputs in `src/`: `ppexgram.t modgram.t ldmodgram.t aterm.orig aterm.ref aterm.reduce include.parser toaterm ppexparser.h mparser.h atermparser.h reduceparser.h ldparser.h`

- [ ] **Step 1: Copy the sources and apply patch 01**

```bash
cd ~/github/elan
mkdir -p src/interpreter src/lib
L=legacy/elan3/src/elan-interpreter
cp -R $L/src $L/parser $L/acmatcher src/interpreter/
patch -p4 -d src/interpreter -i ../../reference/patches/01-interpreter-missing-return.patch
cp -R reference/compat src/interpreter/compat
cp -R legacy/elan3/src/elan-library/elanlib src/lib/elanlib
```

Expected: `patch` reports 5 files patched, no rejects.

- [ ] **Step 2: Remove what is not source**

```bash
cd ~/github/elan/src
find . -name CVS -type d -prune -exec rm -rf {} +
find . \( -name '*.o' -o -name '*.a' -o -name 'Makefile' -o -name 'Makefile.in' \
          -o -name 'Makefile.am' -o -name '.#*' \) -delete
cd interpreter
rm -f src/elan src/mtokdef src/mabident parser/macc acmatcher/match \
      src/f1.tmp src/f2.tmp \
      src/tabofident.cc src/atabofident.cc src/rtabofident.cc src/commtokens.h \
      src/ppexparser.cc src/mparser.cc src/atermparser.cc src/reduceparser.cc src/ldparser.cc \
      acmatcher/lex.yy.c acmatcher/y.tab.c acmatcher/y.tab.h
```

- [ ] **Step 3: Verify the import is exactly legacy + patch 01**

```bash
cd ~/github/elan
T=$(mktemp -d tests/legacy-bench/work/import.XXXX)
cp -R legacy/elan3/src/elan-interpreter/src legacy/elan3/src/elan-interpreter/parser legacy/elan3/src/elan-interpreter/acmatcher $T/
patch -s -p4 -d $T -i "$PWD/reference/patches/01-interpreter-missing-return.patch"
for d in src parser acmatcher; do diff -rq $T/$d src/interpreter/$d | grep -v '^Only in '"$T" ; done
diff -rq legacy/elan3/src/elan-library/elanlib src/lib/elanlib | grep -v '^Only in legacy'
rm -rf $T
```

Expected: no output (only files removed in Step 2 differ, and those appear as `Only in <tmp>` lines, filtered out). Then check that both case variants survived: `ls src/lib/elanlib/strategy | grep -i '^any.eln'` → `Any.eln` and `any.eln`.

- [ ] **Step 4: Commit**

```bash
git add src
git commit -m "src: import interpreter and library sources (legacy + reference patch 01)

Verbatim copy of legacy/elan3/src/elan-interpreter/{src,parser,acmatcher}
with reference/patches/01 applied, the C++ compatibility shims, and
legacy/elan3/src/elan-library/elanlib. Generated files, binaries, CVS and
autotools files are not imported. Behaviour change: none.

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 3: CMake — host tools, `libmatch` and byte-identical parser generation

**Files:**
- Create: `CMakeLists.txt`, `src/interpreter/CMakeLists.txt`, `src/interpreter/gen-parsers.sh`
- Modify: `.gitignore` (add `build/`)

**Interfaces:**
- Consumes: Task 2 tree.
- Produces: CMake targets `macc`, `mtokdef`, `mabident` (executables), `match` (static library), custom target `elan_parsers` producing in `${CMAKE_CURRENT_BINARY_DIR}/generated/`: `ppexparser.cc mparser.cc atermparser.cc reduceparser.cc ldparser.cc tabofident.cc atabofident.cc rtabofident.cc commtokens.h`. CMake variables `ELAN_CXX_COMPAT_OPTIONS`, `ELAN_C_COMPAT_OPTIONS`, `ELAN_DEFINITIONS` (used by Task 4).
- `gen-parsers.sh MACC MTOKDEF MABIDENT SRCDIR OUTDIR` — exit 0 and the 9 files above in `OUTDIR`.

- [ ] **Step 1: Write the check that must pass at the end (generated files identical to the reference build)**

The reference build keeps its generated files in `reference/build/elan3/src/elan-interpreter/src/`. Make sure it exists (`make`-less for now):

Run: `cd ~/github/elan && ls reference/build/elan3/src/elan-interpreter/src/{ppexparser,mparser,atermparser,reduceparser,ldparser,tabofident,atabofident,rtabofident}.cc reference/build/elan3/src/elan-interpreter/src/commtokens.h`
Expected: 9 files listed (if not, run `reference/build.sh` first).

The check (run in Step 5):

```bash
for f in ppexparser.cc mparser.cc atermparser.cc reduceparser.cc ldparser.cc \
         tabofident.cc atabofident.cc rtabofident.cc commtokens.h; do
  cmp build/src/interpreter/generated/$f reference/build/elan3/src/elan-interpreter/src/$f || echo "DIFF $f"
done
```

Run it now: Expected: `cmp: build/src/interpreter/generated/...: No such file or directory` for each (nothing built yet).

- [ ] **Step 2: Write `src/interpreter/gen-parsers.sh`** (transcription of the `$(generated_c)` rule of the 2003 `src/Makefile.am`)

```bash
#!/bin/sh
# Generate the parser sources of the ELAN interpreter, exactly as the
# $(generated_c) rule of elan-interpreter/src/Makefile.am (2003) did.
# Usage: gen-parsers.sh MACC MTOKDEF MABIDENT SRCDIR OUTDIR
set -e
MACC=$1; MTOKDEF=$2; MABIDENT=$3; S=$4; O=$5
mkdir -p "$O"; cd "$O"

gen() { # gen <grammar file> <table name> <keywords name>
  cp "$1" tmpgram.t; "$MACC" tmpgram.t; mv sttab.c "$2"; mv idtab.tid "$3"
}
gen "$S/ppexgram.t"  ppexparsertab.cc ppexrw
gen "$S/modgram.t"   mparsertab.cc    modrw
gen "$S/ldmodgram.t" ldparsertab.cc   ldrw
cat "$S/aterm.orig" "$S/aterm.ref"    > aterm.t;  gen aterm.t  atermparsertab.cc  atermrw
cat "$S/aterm.orig" "$S/aterm.reduce" > raterm.t; gen raterm.t reduceparsertab.cc reducerw

cat ldrw modrw ppexrw | "$MTOKDEF"  > commtokens.h
cat ldrw modrw ppexrw | "$MABIDENT" > tabofident.cc
"$MTOKDEF"  < atermrw  | sed -f "$S/toaterm" > acommtokens.h
"$MABIDENT" < atermrw  | sed -f "$S/toaterm" > atabofident.cc
"$MTOKDEF"  < reducerw | sed -f "$S/toaterm" > rcommtokens.h
"$MABIDENT" < reducerw | sed -f "$S/toaterm" > rtabofident.cc

cat "$S/ppexparser.h"   commtokens.h  ppexparsertab.cc   "$S/include.parser" > ppexparser.cc
cat "$S/mparser.h"      commtokens.h  mparsertab.cc      "$S/include.parser" > mparser.cc
cat "$S/atermparser.h"  acommtokens.h atermparsertab.cc  "$S/include.parser" > atermparser.cc
cat "$S/reduceparser.h" rcommtokens.h reduceparsertab.cc "$S/include.parser" > reduceparser.cc
cat "$S/ldparser.h"     commtokens.h  ldparsertab.cc     "$S/include.parser" > ldparser.cc

rm -f ppexparsertab.cc ppexrw mparsertab.cc modrw ldparsertab.cc ldrw \
      aterm.t atermparsertab.cc atermrw raterm.t reduceparsertab.cc reducerw \
      tmpgram.t acommtokens.h rcommtokens.h
```

Then: `chmod +x src/interpreter/gen-parsers.sh`

- [ ] **Step 3: Write the top-level `CMakeLists.txt`**

```cmake
cmake_minimum_required(VERSION 3.20)
project(elan VERSION 3.6 LANGUAGES C CXX)

# S1: the 1998-2004 sources are built unchanged, in pre-standard modes.
# Removing these options is sub-project S2.
set(ELAN_C_COMPAT_OPTIONS   -std=gnu89 -w -fcommon)
set(ELAN_CXX_COMPAT_OPTIONS -std=gnu++98 -fpermissive -w)
set(ELAN_DEFINITIONS
    PACKAGE="elan-interpreter" VERSION="3.6g" YYTEXT_POINTER=1)

add_subdirectory(src/interpreter)
```

- [ ] **Step 4: Write `src/interpreter/CMakeLists.txt` (tools, library, generation)**

```cmake
# --- macc: parser-table generator (C, host tool) --------------------------
add_executable(macc
  parser/globdef.c parser/rtglobde.c parser/mallo.c parser/tiderr.c parser/mlexan.c
  parser/dump.c parser/envir.c parser/gen.c parser/rwtab.c parser/ingram.c
  parser/init.c parser/macc.c parser/trapsa.c parser/mh.c)
target_compile_options(macc PRIVATE ${ELAN_C_COMPAT_OPTIONS} -g)
target_compile_definitions(macc PRIVATE ${ELAN_DEFINITIONS})

# --- libmatch: AC matcher (C) ----------------------------------------------
add_library(match STATIC
  acmatcher/matcher/build_free.c acmatcher/matcher/solve_free.c
  acmatcher/matcher/build_match.c acmatcher/matcher/flatten.c
  acmatcher/matcher/solve_pure.c acmatcher/matcher/tools.c
  acmatcher/matcher/build_pure.c acmatcher/matcher/gccmatch.c)
target_compile_options(match PRIVATE ${ELAN_C_COMPAT_OPTIONS} -g)
target_compile_definitions(match PRIVATE ${ELAN_DEFINITIONS})

# --- options shared by every C++ target of src/ ------------------------------
set(ELAN_INTERP_DEFINITIONS ${ELAN_DEFINITIONS}
    ALPHA COMMAND PEM BORO ANYS SYMBS EARLEY TO_BE_DISTRIBUTED META PEVAL
    PREFIX="${CMAKE_INSTALL_PREFIX}")
set(ELAN_COMMON_SOURCES
  src/commondefs.cc src/ochstream.cc src/ichstream.cc src/lstream.cc
  src/stringtab.cc src/mitab.cc src/mallo.cc src/misc.cc)

function(elan_cxx_target target)
  target_compile_options(${target} PRIVATE ${ELAN_CXX_COMPAT_OPTIONS} -pipe -O2)
  target_compile_definitions(${target} PRIVATE ${ELAN_INTERP_DEFINITIONS})
  target_include_directories(${target} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/compat ${CMAKE_CURRENT_SOURCE_DIR}/src)
endfunction()

# --- mtokdef / mabident: keyword-table generators (host tools) ------------
add_executable(mtokdef src/mtokdefmain.cc src/specials.cc ${ELAN_COMMON_SOURCES})
elan_cxx_target(mtokdef)
add_executable(mabident src/mtokmain.cc src/specials.cc ${ELAN_COMMON_SOURCES})
elan_cxx_target(mabident)

# --- generated parser sources ----------------------------------------------
set(ELAN_GEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
set(ELAN_GENERATED
  ${ELAN_GEN_DIR}/ppexparser.cc ${ELAN_GEN_DIR}/mparser.cc ${ELAN_GEN_DIR}/atermparser.cc
  ${ELAN_GEN_DIR}/reduceparser.cc ${ELAN_GEN_DIR}/ldparser.cc ${ELAN_GEN_DIR}/tabofident.cc
  ${ELAN_GEN_DIR}/atabofident.cc ${ELAN_GEN_DIR}/rtabofident.cc ${ELAN_GEN_DIR}/commtokens.h)
set(S ${CMAKE_CURRENT_SOURCE_DIR}/src)
add_custom_command(
  OUTPUT ${ELAN_GENERATED}
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/gen-parsers.sh
          $<TARGET_FILE:macc> $<TARGET_FILE:mtokdef> $<TARGET_FILE:mabident> ${S} ${ELAN_GEN_DIR}
  DEPENDS macc mtokdef mabident ${CMAKE_CURRENT_SOURCE_DIR}/gen-parsers.sh
          ${S}/ppexgram.t ${S}/modgram.t ${S}/ldmodgram.t ${S}/aterm.orig ${S}/aterm.ref
          ${S}/aterm.reduce ${S}/include.parser ${S}/toaterm ${S}/ppexparser.h ${S}/mparser.h
          ${S}/atermparser.h ${S}/reduceparser.h ${S}/ldparser.h
  COMMENT "Generating ELAN parser sources with macc")
add_custom_target(elan_parsers DEPENDS ${ELAN_GENERATED})
```

Add `build/` to the top-level `.gitignore` (`printf 'build/\n' >> .gitignore`).

- [ ] **Step 5: Build and run the check of Step 1**

```bash
cd ~/github/elan
cmake -S . -B build -DCMAKE_C_COMPILER=gcc-16 -DCMAKE_CXX_COMPILER=g++-16 \
      -DCMAKE_INSTALL_PREFIX=$PWD/build/install
cmake --build build --target elan_parsers match -j
for f in ppexparser.cc mparser.cc atermparser.cc reduceparser.cc ldparser.cc \
         tabofident.cc atabofident.cc rtabofident.cc commtokens.h; do
  cmp build/src/interpreter/generated/$f reference/build/elan3/src/elan-interpreter/src/$f || echo "DIFF $f"
done
```

Expected: build succeeds; the loop prints nothing (all 9 files byte-identical).

- [ ] **Step 6: Check the source tree is untouched**

Run: `git status --short` → only the new files of this task (`CMakeLists.txt`, `src/interpreter/CMakeLists.txt`, `src/interpreter/gen-parsers.sh`, `.gitignore`); nothing under `src/interpreter/src`, `parser`, `acmatcher`.

- [ ] **Step 7: Commit**

```bash
git add CMakeLists.txt src/interpreter/CMakeLists.txt src/interpreter/gen-parsers.sh .gitignore
git commit -m "build: CMake for macc, mtokdef, mabident, libmatch and parser generation

The generated parser sources are byte-identical to those of the 2003
automake rules (checked against reference/build). Behaviour change: none.

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 4: CMake — the `elan` executable and installation

**Files:**
- Modify: `src/interpreter/CMakeLists.txt` (append)

**Interfaces:**
- Consumes: targets/variables of Task 3.
- Produces: target `elan`; `cmake --install build` installs `<prefix>/bin/elan` and `<prefix>/share/elanlib/{common,noquote,ref,strategy}`.

- [ ] **Step 1: Write the failing check**

```bash
cd ~/github/elan
T=$(mktemp -d tests/legacy-bench/work/enum.XXXX)
cp legacy/elan/sources/Compiler.4.0/Test/enum.* $T/
echo 'enum(o,fac(s(s(s(o))))) end' > $T/q.inp
( cd $T && ELANLIB=$OLDPWD/build/install $OLDPWD/build/install/bin/elan -b enum.lgi < q.inp ) > $T/modern.out; echo rc=$?
```

Expected now: `No such file or directory` for `build/install/bin/elan`, `rc=127`. Keep `$T` for Step 5.

- [ ] **Step 2: Append to `src/interpreter/CMakeLists.txt`**

```cmake
# --- elan: the interpreter ---------------------------------------------------
add_executable(elan
  src/ldmain.cc src/msemact.cc src/msemact3.cc src/termcompile.cc src/matchcompile.cc
  src/compilemisc.cc src/strategy.cc src/grammar.cc src/earley.cc src/esemact.cc src/term.cc
  src/module.cc src/tmisc.cc src/rtmisc.cc src/match.cc src/trsystem.cc src/normalise.cc
  src/aterm.cc src/partial.cc src/stateofexecution.cc src/meta.cc src/lbuffer.cc
  src/mlstream.cc src/command.cc
  ${ELAN_COMMON_SOURCES}
  ${ELAN_GEN_DIR}/tabofident.cc ${ELAN_GEN_DIR}/ppexparser.cc ${ELAN_GEN_DIR}/mparser.cc
  ${ELAN_GEN_DIR}/atermparser.cc ${ELAN_GEN_DIR}/reduceparser.cc ${ELAN_GEN_DIR}/ldparser.cc
  ${ELAN_GEN_DIR}/atabofident.cc)
elan_cxx_target(elan)
target_include_directories(elan PRIVATE ${ELAN_GEN_DIR})
add_dependencies(elan elan_parsers)
target_link_libraries(elan PRIVATE match m)

install(TARGETS elan RUNTIME DESTINATION bin)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/lib/elanlib/
        DESTINATION share/elanlib)
```

- [ ] **Step 3: Build and install**

Run: `cmake --build build -j && cmake --install build`
Expected: `build/install/bin/elan` and `build/install/share/elanlib/strategy/{any,Any}.eln` exist.

- [ ] **Step 4: Run the check of Step 1 again**

Run the `( cd $T && ... )` line of Step 1 again.
Expected: `rc=0`, and `grep -c 's(' $T/modern.out` is non-zero.

- [ ] **Step 5: Compare with the reference interpreter on the same input**

```bash
( cd $T && ELANLIB=$OLDPWD/reference/install $OLDPWD/reference/install/bin/elan -b enum.lgi < q.inp ) > $T/ref.out
cmp $T/ref.out $T/modern.out && echo IDENTICAL
```

Expected: `IDENTICAL`.

- [ ] **Step 6: Library found without `ELANLIB` (Review Focus 2)**

```bash
( cd $T && env -u ELANLIB $OLDPWD/build/install/bin/elan -b enum.lgi < q.inp ) > $T/noenv.out; echo rc=$?
cmp $T/noenv.out $T/modern.out && echo IDENTICAL
```

Expected: `rc=0`, `IDENTICAL` (the interpreter defaults to `PREFIX`, which is the install prefix).

- [ ] **Step 7: Editing a grammar regenerates the parsers (Review Focus 3)**

```bash
touch src/interpreter/src/modgram.t
cmake --build build 2>&1 | grep -c "Generating ELAN parser sources"
```

Expected: `1`. Then `rm -rf $T`.

- [ ] **Step 8: Source tree untouched (Review Focus 4)**

Run: `git status --short` → only `src/interpreter/CMakeLists.txt` modified.

- [ ] **Step 9: Commit**

```bash
git add src/interpreter/CMakeLists.txt
git commit -m "build: CMake for the elan interpreter and its installation

Output on the enum example is identical to the reference interpreter.
Behaviour change: none.

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 5: `Makefile` façade and `make check`

**Files:**
- Create: `Makefile`

**Interfaces:**
- Consumes: CMake build (Tasks 3–4), `reference/build.sh`, `tests/legacy-bench/run_tests.py` (Task 1).
- Produces: targets `all` (default), `install`, `check`, `reference`, `check-reference`, `test-runner`, `clean`; variables `PREFIX` (default `$(CURDIR)/build/install`), `ELAN_CC`, `ELAN_CXX`.

- [ ] **Step 1: Write the failing check**

Run: `cd ~/github/elan && make check; echo rc=$?`
Expected: `make: *** No rule to make target 'check'.` and `rc=2`.

- [ ] **Step 2: Write `Makefile`**

```make
# Façade over CMake, the reference build and the test bench.
#   make                 build the interpreter (build/)
#   make install         install into $(PREFIX)
#   make check           unit tests of the runner + interpreter tests (I, A) of the bench
#   make reference       build the 2004 reference system (reference/install)
#   make check-reference full bench (778 tests) against the reference
BUILD   ?= build
PREFIX  ?= $(CURDIR)/$(BUILD)/install
BENCH    = tests/legacy-bench/run_tests.py

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
  ELAN_CC  ?= gcc-16
  ELAN_CXX ?= g++-16
else
  ELAN_CC  ?= gcc
  ELAN_CXX ?= g++
endif
export ELAN_CC ELAN_CXX

.PHONY: all configure install check test-runner reference check-reference clean toolchain

all: configure
	cmake --build $(BUILD) -j

toolchain:
	@command -v $(ELAN_CC)  >/dev/null || { echo "ERROR: C compiler '$(ELAN_CC)' not found (set ELAN_CC, see README)"; exit 1; }
	@command -v $(ELAN_CXX) >/dev/null || { echo "ERROR: C++ compiler '$(ELAN_CXX)' not found (set ELAN_CXX, see README)"; exit 1; }
	@command -v cmake >/dev/null || { echo "ERROR: cmake not found (see README)"; exit 1; }

configure: toolchain
	@test -f $(BUILD)/CMakeCache.txt || \
	  cmake -S . -B $(BUILD) -DCMAKE_C_COMPILER=$(ELAN_CC) -DCMAKE_CXX_COMPILER=$(ELAN_CXX) \
	        -DCMAKE_INSTALL_PREFIX=$(PREFIX)

install: all
	cmake --install $(BUILD) --prefix $(PREFIX)

test-runner:
	cd tests/legacy-bench && python3 test_run_tests.py

check: install test-runner
	$(BENCH) --prefix $(PREFIX) --kinds I,A

reference:
	reference/build.sh

check-reference:
	$(BENCH) --prefix reference/install

clean:
	rm -rf $(BUILD)
```

- [ ] **Step 3: Missing toolchain fails early (Review Focus 5)**

Run: `make ELAN_CC=gcc-nonexistent BUILD=build-x; echo rc=$?`
Expected: `ERROR: C compiler 'gcc-nonexistent' not found (set ELAN_CC, see README)`, `rc=2`; `build-x` not created.

- [ ] **Step 4: Run `make check`**

Run: `make check 2>&1 | tail -5; echo rc=${PIPESTATUS[0]}`
Expected: runner unit tests `OK`; bench `368 tests`; `vs baseline: 0 regressions, 0 improvements`; `vs snapshots: 0 differences`; `rc=0`.

- [ ] **Step 5: Run `make check-reference`**

Run: `make check-reference 2>&1 | tail -3`
Expected: `778 tests`, `0 regressions`, `0 differences`.

- [ ] **Step 6: Commit**

```bash
git add Makefile
git commit -m "build: Makefile façade (all, install, check, reference, check-reference)

make check: 368 interpreter tests, 0 regressions, 0 snapshot differences.

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 6: Reference build on Linux (verified locally with Docker)

**Files:**
- Modify: `reference/build.sh`
- Create: `reference/patches/06-rem-linux-makefile.patch`, `ci/Dockerfile.linux`
- Modify: `reference/README.md` (patch table: add 06; requirements: Linux packages)

**Interfaces:**
- Produces: `reference/build.sh` working on Darwin and Linux with defaults chosen by `uname -s`, still overridable by `ELAN_CC ELAN_CXX GC_PREFIX JAVA_HOME AUX`; Docker image `elan-linux` with the CI's Linux packages.

- [ ] **Step 1: Write `ci/Dockerfile.linux`**

```dockerfile
# Local reproduction of the Linux CI job (same packages as .github/workflows/ci.yml).
FROM ubuntu:24.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      build-essential cmake bison flex libfl-dev default-jdk-headless libgc-dev \
      automake python3 git ca-certificates && rm -rf /var/lib/apt/lists/*
WORKDIR /work
```

Run: `cd ~/github/elan && docker build -t elan-linux -f ci/Dockerfile.linux ci`
Expected: image built.

- [ ] **Step 2: Reproduce the failure on Linux**

```bash
docker run --rm -v ~/github/elan:/repo:ro elan-linux bash -c \
  'git clone -q /repo /work/elan && cd /work/elan && reference/build.sh 2>&1 | tail -5'
```

Expected: failure (`gcc-16: command not found` or Homebrew path errors).

- [ ] **Step 3: Platform defaults in `reference/build.sh`**

Replace the block from `GCC="${ELAN_CC:-gcc-16}"` to `export PATH="$JAVA_HOME/bin:$PATH"` by:

```bash
case "$(uname -s)" in
  Darwin)
    GCC="${ELAN_CC:-gcc-16}"; GXX="${ELAN_CXX:-g++-16}"
    GC_PREFIX="${GC_PREFIX:-/opt/homebrew/opt/bdw-gc}"
    JAVA_HOME="${JAVA_HOME:-/opt/homebrew/opt/openjdk}"
    AUX="${AUX:-$(dirname "$(find /opt/homebrew/Cellar/libtool -name config.guess -path '*build-aux*' | head -1)")}"
    AR=/usr/bin/ar ;;
  Linux)
    GCC="${ELAN_CC:-gcc}"; GXX="${ELAN_CXX:-g++}"
    GC_PREFIX="${GC_PREFIX:-/usr}"
    JAVA_HOME="${JAVA_HOME:-$(dirname "$(dirname "$(readlink -f "$(command -v javac)")")")}"
    AUX="${AUX:-$(dirname "$(ls /usr/share/automake-*/config.guess | sort -V | tail -1)")}"
    AR=ar ;;
  *) echo "unsupported platform $(uname -s)"; exit 1 ;;
esac
export PATH="$JAVA_HOME/bin:$PATH"
```

and delete the later line `AR=/usr/bin/ar`.

- [ ] **Step 4: Patch 06 — Linux branch of the Makefile generated by REM**

Create `reference/patches/06-rem-linux-makefile.patch` (applies after 04):

```diff
--- a/elan3/src/elan-compiler/src/rem/REM.java
+++ b/elan3/src/elan-compiler/src/rem/REM.java
@@ -346,8 +346,9 @@
       subMakefile.write("YLIB = -lfl\n");
       subMakefile.write("DLLIB = \n");
       subMakefile.write("else\n");
-      subMakefile.write("CC = gcc -static -pipe\n");
-      subMakefile.write("CXX = libtool --mode=link g++\n");
+      // Linux (2026): pre-standard C mode, no static link, no libtool
+      subMakefile.write("CC = gcc -pipe -std=gnu89 -w -fcommon\n");
+      subMakefile.write("CXX = g++\n");
       //subMakefile.write("CXX = g++ \n");
       if(Flags.choicePointDebug) {
         if(Flags.onlyC) {
```

(The Darwin block added by patch 04 comes after `endif` and still overrides `CC`/`CXX` on macOS, so macOS output is unchanged.)

Check it applies: `T=$(mktemp -d tests/legacy-bench/work/p6.XXXX); mkdir -p $T/elan3/src/elan-compiler/src/rem; cp legacy/elan3/src/elan-compiler/src/rem/REM.java $T/elan3/src/elan-compiler/src/rem/; patch -p1 -d $T -i $PWD/reference/patches/04-rem-darwin-makefile.patch && patch -p1 -d $T -i $PWD/reference/patches/06-rem-linux-makefile.patch && echo APPLIES; rm -rf $T`
Expected: `APPLIES` (offsets are acceptable, no fuzz rejects).

- [ ] **Step 5: Reference on Linux, full bench**

```bash
docker run --rm -v ~/github/elan:/repo:ro elan-linux bash -c \
  'git clone -q /repo /work/elan && cd /work/elan && \
   cp -R /repo/reference/build.sh /repo/reference/patches reference/ && \
   reference/build.sh 2>&1 | grep -E "FAILED|Done" && \
   tests/legacy-bench/run_tests.py --prefix reference/install > /tmp/bench.log 2>&1; \
   tail -8 /tmp/bench.log; grep -E "SNAP-DIFF|REGRESSION" /tmp/bench.log | sort -u'
```

(The working-tree files are copied in because they are not committed yet.)
Expected: `Done. Installed in /work/elan/reference/install`; bench prints `778 tests`; then a list of `SNAP-DIFF` lines (possibly empty) and the regressions summary.

- [ ] **Step 6: Analyse every Linux difference**

For each `SNAP-DIFF` (and each `REGRESSION`) from Step 5, rerun the single test inside the container with its diff shown:

```bash
docker run --rm -v ~/github/elan:/repo:ro elan-linux bash -c \
  'git clone -q /repo /work/elan && cd /work/elan && cp -R /repo/reference/build.sh /repo/reference/patches reference/ && \
   reference/build.sh >/dev/null 2>&1; tests/legacy-bench/run_tests.py --prefix reference/install --filter "<TEST-ID-REGEX>"; \
   cat tests/legacy-bench/results/*/*.snapdiff.txt'
```

Classify each one:
- output depends on uninitialised memory, pointer values, hash order of addresses → **undefined behaviour**: add a line `<id>\tlinux\tundefined behaviour: <one-line description>; fix in S2` to `tests/legacy-bench/platform-exceptions.tsv`;
- genuine platform difference (e.g. `printf` of doubles, `uname`) → line `<id>\tlinux\tplatform: <one-line description>`;
- a bug of the Linux build itself (missing flag, wrong library) → fix `build.sh` / patch 06 and go back to Step 5.

Regressions (status worse than baseline) are not accepted as exceptions: they must be explained by a build problem and fixed.

- [ ] **Step 7: macOS unchanged**

Run: `cd ~/github/elan && reference/build.sh 2>&1 | grep -E "FAILED|Done" && make check-reference 2>&1 | tail -3`
Expected: `Done`; `778 tests`, `0 regressions`, `0 differences`.

- [ ] **Step 8: Linux final check**

Rerun Step 5 with the exceptions file copied too (`cp /repo/tests/legacy-bench/platform-exceptions.tsv tests/legacy-bench/`).
Expected: `0 regressions`, `vs snapshots: 0 differences` (excepted ones shown as `SNAP-EXC`).

- [ ] **Step 9: Update `reference/README.md`**

Add the row to the patch table:

```markdown
| `06-rem-linux-makefile` | the Linux branch of the Makefile generated for compiled programs links statically and through `libtool`, and compiles the 1990s C in the default (C17) mode | compiled programs do not build on current Linux distributions |
```

and under "Requirements" add: `Linux (Ubuntu 24.04): apt install build-essential cmake bison flex libfl-dev default-jdk-headless libgc-dev automake python3`.

- [ ] **Step 10: Commit**

```bash
git add reference/build.sh reference/patches/06-rem-linux-makefile.patch reference/README.md \
        ci/Dockerfile.linux tests/legacy-bench/platform-exceptions.tsv
git commit -m "reference: build on Linux (platform defaults, patch 06 for REM's Linux Makefile)

Verified in Docker (Ubuntu 24.04): full bench, 0 regressions; Linux snapshot
differences analysed in platform-exceptions.tsv. macOS unchanged.

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 7: Modern interpreter on Linux

**Files:**
- Modify (only if needed): `tests/legacy-bench/platform-exceptions.tsv`, `src/interpreter/CMakeLists.txt`

**Interfaces:**
- Consumes: Tasks 1–6.

- [ ] **Step 1: `make check` on Linux**

```bash
docker run --rm -v ~/github/elan:/repo:ro elan-linux bash -c \
  'git clone -q /repo /work/elan && cd /work/elan && make reference >/dev/null 2>&1 && make check 2>&1 | tail -8'
```

Expected: `368 tests`, `0 regressions`, `vs snapshots: 0 differences`. Interpreter tests already excepted in Task 6 show as `SNAP-EXC`.

- [ ] **Step 2: Handle any new difference**

A difference that appears for the modern build but not for the Linux reference is a **build** difference (flags, generated tables): compare the generated sources as in Task 3 Step 5 inside the container, fix `src/interpreter/CMakeLists.txt`, and rerun Step 1. It is never added to the exceptions file.

- [ ] **Step 3: Commit (only if something changed)**

```bash
git add -A src/interpreter/CMakeLists.txt tests/legacy-bench/platform-exceptions.tsv
git commit -m "build: Linux fixes for the modern interpreter

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 8: Continuous integration

**Files:**
- Create: `.github/workflows/ci.yml`

**Interfaces:**
- Consumes: `make reference`, `make check-reference`, `make check` (Task 5), Linux packages of `ci/Dockerfile.linux`.

- [ ] **Step 1: Write `.github/workflows/ci.yml`**

```yaml
name: ci
on:
  push:
  pull_request:

jobs:
  build-and-test:
    strategy:
      fail-fast: false
      matrix:
        os: [ubuntu-24.04, macos-15]
    runs-on: ${{ matrix.os }}
    steps:
      - uses: actions/checkout@v4
        with:
          path: checkout

      - name: Dependencies (Linux)
        if: runner.os == 'Linux'
        run: |
          sudo apt-get update
          sudo DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
            build-essential cmake bison flex libfl-dev default-jdk-headless libgc-dev automake python3
          echo "ELAN_WS=$RUNNER_TEMP/elan" >> "$GITHUB_ENV"

      - name: Dependencies and case-sensitive volume (macOS)
        if: runner.os == 'macOS'
        run: |
          brew install gcc openjdk bdw-gc bison flex libtool cmake
          GCC=$(ls "$(brew --prefix)"/bin/gcc-[0-9]* | sort -V | tail -1)
          echo "ELAN_CC=$(basename "$GCC")" >> "$GITHUB_ENV"
          echo "ELAN_CXX=$(basename "$GCC" | sed 's/gcc/g++/')" >> "$GITHUB_ENV"
          hdiutil create -size 4g -type SPARSE -fs "Case-sensitive APFS" -volname elan "$RUNNER_TEMP/elan.sparseimage"
          mkdir -p "$RUNNER_TEMP/cs"
          hdiutil attach "$RUNNER_TEMP/elan.sparseimage" -mountpoint "$RUNNER_TEMP/cs" -nobrowse
          echo "ELAN_WS=$RUNNER_TEMP/cs/elan" >> "$GITHUB_ENV"

      # Clone from the checkout: on macOS the checkout itself is on a
      # case-insensitive file system, but its object store is complete.
      - name: Case-sensitive working copy
        run: git clone --quiet "$GITHUB_WORKSPACE/checkout" "$ELAN_WS"

      - name: Reference build
        run: make -C "$ELAN_WS" reference

      - name: Full bench against the reference
        run: make -C "$ELAN_WS" check-reference

      - name: Modern interpreter, make check
        run: make -C "$ELAN_WS" check

      - name: Bench results
        if: failure()
        uses: actions/upload-artifact@v4
        with:
          name: bench-results-${{ matrix.os }}
          path: ${{ env.ELAN_WS }}/tests/legacy-bench/results/
```

On macOS the reference build uses `ELAN_CC`/`ELAN_CXX` from the environment (exported by the step), so the Homebrew GCC major version is not hard-coded in CI.

- [ ] **Step 2: Validate the YAML locally**

Run: `python3 -c "import yaml,sys; yaml.safe_load(open('.github/workflows/ci.yml')); print('ok')" 2>/dev/null || ruby -ryaml -e 'YAML.load_file(".github/workflows/ci.yml"); puts "ok"'`
Expected: `ok`.

- [ ] **Step 3: Commit and push, watch the run**

```bash
git add .github/workflows/ci.yml
git commit -m "ci: build and test on Ubuntu 24.04 and macOS 15

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
git push
```

Then check the run on https://github.com/rewriting/elan/actions (or `curl -s https://api.github.com/repos/rewriting/elan/actions/runs?per_page=1 | python3 -c "import sys,json; r=json.load(sys.stdin)['workflow_runs'][0]; print(r['status'], r['conclusion'])"` until `completed success`).
Expected: both jobs green. If a job fails, download the `bench-results-*` artifact, fix (Task 6 rules for Linux differences), commit, push.

---

### Task 9: Documentation and working rules

**Files:**
- Modify: `README.md`
- Create: `CONTRIBUTING.md`

- [ ] **Step 1: README — layout and quick start**

In `README.md`, replace the row `| *(to come)* \`src/\`, \`examples/\`, \`tests/\` | the modern, maintained implementation |` by:

```markdown
| `src/interpreter/`, `src/lib/elanlib/` | the modern, maintained interpreter and its standard library (S1: sources still identical to the reference) |
| `tests/regression/` | targeted regression tests (one directory per case) |
| `ci/`, `.github/workflows/` | Linux image for local checks, CI on Ubuntu and macOS |
```

Replace the section "Quick start (reference build)" by:

```markdown
## Quick start

```sh
make                 # build the interpreter (CMake, into build/)
make check           # runner unit tests + 368 interpreter tests of the bench
make install PREFIX=$HOME/.local
make reference       # build the 2004 reference system (interpreter + compiler)
make check-reference # full bench (778 tests) against the reference
```

On macOS the default compilers are Homebrew's `gcc-16`/`g++-16`; override
with `make ELAN_CC=gcc-17 ELAN_CXX=g++-17`. Linux needs
`build-essential cmake bison flex libfl-dev default-jdk-headless libgc-dev automake python3`;
`ci/Dockerfile.linux` reproduces the Linux CI job locally.
```

- [ ] **Step 2: `CONTRIBUTING.md`**

```markdown
# Contributing

The goal is *executable heritage*: historical ELAN programs must keep running
with their original behaviour. Every change is checked against the reference
build of 2004.

## Rules

1. `legacy/` is never modified.
2. `reference/patches/` is frozen; a portability fix of the reference is a
   new numbered patch documented in `reference/README.md`.
3. Every commit touching `src/` keeps `make check` green.
4. A test may improve, never regress. When a fix changes an output on purpose,
   commit it on its own: refresh the snapshot and baseline
   (`tests/legacy-bench/run_tests.py --save-baseline --save-snapshots`),
   explain the fix in the message, and add a case to `tests/regression/`.
5. Platform-specific snapshot differences go to
   `tests/legacy-bench/platform-exceptions.tsv` with their analysis; there is
   only one set of snapshots.
6. Commit messages state the behaviour change ("Behaviour change: none" when
   there is none).

## Before pushing

    make check            # always
    make check-reference  # when touching reference/ or the bench runner
```

- [ ] **Step 3: Check the commands of the README work**

Run: `make clean && make && make check 2>&1 | tail -3`
Expected: `0 regressions`, `0 differences`.

- [ ] **Step 4: Commit and push**

```bash
git add README.md CONTRIBUTING.md
git commit -m "docs: build/test instructions and contribution rules

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
git push
```

Expected: CI green on both platforms.
