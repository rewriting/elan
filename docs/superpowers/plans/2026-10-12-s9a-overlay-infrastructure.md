# S9a — The overlay of legacy/: infrastructure — Implementation Plan

> **Superseded** after Task 7 by spec section 8 and docs/superpowers/plans/2026-10-13-s9a-living-copy.md.

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** The legacy bench reads tests from an overlay of `legacy/` (`tests/legacy-overlay/`), with outputs of the 2004 reference as the oracle, a coverage check of the 293 programs of `legacy/elan3/{applications,contributions}`, a triage report of the 179 untested ones, and one program tested end to end through the overlay (I, A, G).

**Architecture:** `tests/legacy-bench/run_tests.py` discovers `tst_all.overlay` files beside the historical `tst_all` files, builds a test's private copy as legacy directory + restored case names + overlay, looks for inputs and expected outputs in the overlay first, gains a `gtest` line (kind G with an input) and judges overlay tests strictly. Three scripts in `tests/legacy-overlay/`: `make_expected.py` (runs the reference to write expected outputs), `check_coverage.py` (every program tested, unloadable or to do; in `make check`), `triage.py` (load status of each program to do, with the reference and the modern system).

**Tech Stack:** Python 3 standard library (unittest), POSIX sh, GNU make; the 2004 reference system (`reference/build.sh`) on a case-sensitive volume.

**Spec:** `docs/superpowers/specs/2026-10-12-s9-legacy-apps-tests-design.md` (§3 criteria, §4 architecture, §5 triage, §6 stage S9a).

## Global Constraints

- `legacy/` is never modified (contents); the only renames are those of `CASE-RENAMES.tsv`.
- The oracle of a program without historical output is the 2004 reference system; the oracle of a `gtest` line is the program compiled by the reference's REM on the same input, `rewrite_step` lines excepted.
- Test ids keep their current form `<dir relative to legacy>::<kind>::<lgi>:<spc>:<inp or ->:<out>`; no existing id, snapshot or `baseline.tsv` line changes.
- Overlay layout: `tests/legacy-overlay/elan3/<application path>/` holds `tst_all.overlay`, `SAMPLES/<name>.inp`, `SAMPLES/<name>.out`, ported files, `PORTING.md`.
- `make check` stays green at every commit; the default bench run takes less than 60 s more than today.
- Work on branch `s9-legacy-apps` in `~/github/elan-cs` (case-sensitive volume, where the reference is built); final check also in a clone on the default (case-insensitive) volume.
- Commit messages end with `Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>`. No push, no merge without the controller.

---

### Task 0: Build the 2004 reference (prerequisite, no code)

**Files:** none (build products are git-ignored: `reference/build/`, `reference/install/`).

- [ ] **Step 1: Build it** on the case-sensitive volume (the defaults of `reference/build.sh` are inside the repository, which is on that volume):

```bash
cd ~/github/elan-cs
JAVA_HOME=$(/usr/libexec/java_home) make reference
```

Expected: ends without `FAILED`; `reference/install/bin/elan`, `reference/install/bin/elanc` and the link `reference/install/bin/elanc-rem` exist.

- [ ] **Step 2: Check it against the bench**

```bash
make check-reference
```

Expected: `vs baseline: 0 regressions` (the reference is the system the baseline was taken from).

---

### Task 1: Discovery of `tst_all.overlay` files and `gtest` lines

**Files:**
- Modify: `tests/legacy-bench/run_tests.py` (constants near `SUITES`; `KIND_OF_CMD`; `discover()`)
- Test: `tests/legacy-bench/test_run_tests.py`

**Interfaces:**
- Produces: `OVERLAY: Path` (= `REPO / "tests" / "legacy-overlay"`); `discover(suites=None, overlay=None, root=None) -> list[dict]`, each dict with the existing keys plus `overlay: bool` (from a `tst_all.overlay`) and `ovdir: Path` (the overlay directory of the application, which may not exist); `KIND_OF_CMD["gtest"] == ["G"]`.

- [ ] **Step 1: Write the failing tests** (append before `if __name__`):

```python
class OverlayDiscoveryTest(unittest.TestCase):
    def make(self, tmp):
        root = Path(tmp) / "legacy"
        app = root / "elan3" / "applications" / "App"
        (app / "SAMPLES").mkdir(parents=True)
        (app / "tst_all").write_text("itest p no p p\n")
        bare = root / "elan3" / "applications" / "Bare"   # no tst_all of its own
        bare.mkdir(parents=True)
        ov = Path(tmp) / "overlay"
        (ov / "elan3" / "applications" / "App").mkdir(parents=True)
        (ov / "elan3" / "applications" / "App" / "tst_all.overlay").write_text(
            "itest q no q q\naitest q no q q\ngtest q no q q-c\n")
        (ov / "elan3" / "applications" / "Bare").mkdir(parents=True)
        (ov / "elan3" / "applications" / "Bare" / "tst_all.overlay").write_text("itest b no b b\n")
        return root, ov

    def test_overlay_lines_are_tests_with_the_usual_ids(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            root, ov = self.make(tmp)
            tests = rt.discover(suites=[root / "elan3" / "applications"], overlay=ov, root=root)
            ids = {t["id"]: t for t in tests}
            self.assertIn("elan3/applications/App::I::p:no:p:p", ids)
            self.assertFalse(ids["elan3/applications/App::I::p:no:p:p"]["overlay"])
            q = ids["elan3/applications/App::I::q:no:q:q"]
            self.assertTrue(q["overlay"])
            self.assertEqual(q["dir"], root / "elan3" / "applications" / "App")
            self.assertEqual(q["ovdir"], ov / "elan3" / "applications" / "App")
            self.assertIn("elan3/applications/App::A::q:no:q:q", ids)
            self.assertIn("elan3/applications/Bare::I::b:no:b:b", ids)

    def test_gtest_is_kind_G_with_an_input(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            root, ov = self.make(tmp)
            tests = rt.discover(suites=[root / "elan3" / "applications"], overlay=ov, root=root)
            g = [t for t in tests if t["id"] == "elan3/applications/App::G::q:no:q:q-c"]
            self.assertEqual(len(g), 1)
            self.assertEqual((g[0]["kind"], g[0]["inp"], g[0]["out"]), ("G", "q", "q-c"))

    def test_overlay_of_a_missing_directory_is_an_error(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            root, ov = self.make(tmp)
            (ov / "elan3" / "applications" / "Nowhere").mkdir()
            (ov / "elan3" / "applications" / "Nowhere" / "tst_all.overlay").write_text("itest x no x x\n")
            with self.assertRaises(SystemExit):
                rt.discover(suites=[root / "elan3" / "applications"], overlay=ov, root=root)

    def test_real_tree_ids_are_unchanged(self):
        # the default arguments give the historical tests with overlay False
        ids = [t["id"] for t in rt.discover() if not t["overlay"]]
        self.assertIn("elan3/applications/Robot::I::Nim:non:Nim:Nim", ids)
```

- [ ] **Step 2: Run them to verify they fail**

Run: `cd tests/legacy-bench && python3 test_run_tests.py OverlayDiscoveryTest -v`
Expected: FAIL/ERROR (`discover() got an unexpected keyword argument 'suites'`, `KeyError: 'overlay'`).

- [ ] **Step 3: Implement.** In `run_tests.py`, after the `SUITES = [...]` line add:

```python
# Spec S9: tests added to the legacy applications without touching legacy/:
# tests/legacy-overlay/<path below legacy>/ holds tst_all.overlay, SAMPLES/
# inputs and expected outputs, and ported files that replace legacy files.
OVERLAY = REPO / "tests" / "legacy-overlay"
```

Replace the `KIND_OF_CMD` line by:

```python
# gtest LGI SPC INP OUT (spec S9): elanc-go, the compiled program reading INP
KIND_OF_CMD = {"itest": ["I"], "aitest": ["A"], "jtest": ["J", "JO", "R", "G"],
               "gtest": ["G"]}
```

Replace the whole `def discover():` function by:

```python
def discover(suites=None, overlay=None, root=None):
    """Tests of the historical tst_all files of the suites, then of the
    tst_all.overlay files of the overlay (spec S9). t["overlay"] tells which;
    t["ovdir"] is the application's overlay directory (it may not exist)."""
    suites = SUITES if suites is None else suites
    overlay = OVERLAY if overlay is None else overlay
    root = ROOT if root is None else root
    tests, seen = [], set()

    def add(d, path, from_overlay):
        for cmd, a, long_ in parse_tst_all(path):
            if cmd == "jtest":
                if len(a) < 3:
                    continue
                lgi, spc, inp, out, flags = a[0], a[1], None, a[2], a[3:]
            else:
                if len(a) < 4:
                    continue
                lgi, spc, inp, out, flags = a[0], a[1], a[2], a[3], a[4:]
            rel = d.relative_to(root)
            for kind in KIND_OF_CMD[cmd]:
                tid = f"{rel}::{kind}::{lgi}:{spc}:{inp or '-'}:{out}"
                if tid in seen:
                    continue
                seen.add(tid)
                tests.append(dict(id=tid, dir=d, kind=kind, lgi=lgi, spc=spc,
                                  inp=inp, out=out, flags=flags, long=long_,
                                  overlay=from_overlay, ovdir=overlay / rel))

    for suite in suites:
        for tst in sorted(suite.rglob("tst_all")):
            if tst.parent != suite:          # top-level driver (tst_one ...)
                add(tst.parent, tst, False)
    if overlay.is_dir():
        for tst in sorted(overlay.rglob("tst_all.overlay")):
            d = root / tst.parent.relative_to(overlay)
            if not d.is_dir():
                raise SystemExit(f"{tst}: no directory {d} in legacy/")
            add(d, tst, True)
    return tests
```

- [ ] **Step 4: Run the tests**

Run: `cd tests/legacy-bench && python3 test_run_tests.py -v 2>&1 | tail -3`
Expected: `OK` (all tests, the old ones included).

- [ ] **Step 5: Check that the bench sees the same tests as before**

Run, before Step 3 and again now: `python3 tests/legacy-bench/run_tests.py --list --kinds I,A,J,JO | tail -1`
Expected: the same `N tests` line both times (no overlay exists yet).

- [ ] **Step 6: Commit**

```bash
git add tests/legacy-bench/run_tests.py tests/legacy-bench/test_run_tests.py
git commit -m "bench: discover tst_all.overlay files (spec S9) and gtest lines (kind G with an input)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 2: Private copy with the overlay, inputs and outputs found in the overlay first, `produce()`

**Files:**
- Modify: `tests/legacy-bench/run_tests.py` (`run_test()` split into `produce()` + comparison; new `locate()`, `has_lgi()`, `private_copy()`, `Unrunnable`)
- Test: `tests/legacy-bench/test_run_tests.py`

**Interfaces:**
- Consumes: `discover()` dict keys `dir`, `ovdir`, `overlay`, `lgi`, `spc`, `inp`, `out`, `kind`, `flags` (Task 1).
- Produces:
  - `class Unrunnable(Exception)` with attributes `status: str`, `detail: str`, `out: str`.
  - `locate(t, name) -> tuple[Path | None, Path | None]`: the file `name` of the SAMPLES directory (as `samples()` chooses it) of the overlay, else of legacy (through `src_path`), and its path relative to that application directory.
  - `private_copy(t, wd) -> None`.
  - `produce(t, timeout) -> tuple[str, str]` (stdout, stderr) of the test run in a private copy, with `PREFIX`'s tools; raises `Unrunnable("NOLGI"|"NOINP"|"ERROR"|"UNSUPPORTED"|"SANITIZER", ...)` and lets `subprocess.TimeoutExpired` through. A compiled program (kinds J, JO, R, G) reads the input when the test has one (`./a.out -quiet < INP`), else runs its start term (`./a.out -noInput -quiet`).
  - `run_test(t, timeout)` unchanged in signature and statuses.

- [ ] **Step 1: Write the failing tests** (append before `if __name__`):

```python
class OverlayCopyTest(unittest.TestCase):
    def make(self, tmp):
        d = Path(tmp) / "legacy" / "App"
        (d / "SAMPLES").mkdir(parents=True)
        (d / "p.lgi").write_text("legacy lgi\n")
        (d / "m.eln").write_text("legacy module\n")
        (d / "SAMPLES" / "old.inp").write_text("old input\n")
        ov = Path(tmp) / "overlay" / "App"
        (ov / "SAMPLES").mkdir(parents=True)
        (ov / "m.eln").write_text("ported module\n")
        (ov / "SAMPLES" / "new.inp").write_text("new input\n")
        (ov / "tst_all.overlay").write_text("itest p no new new\n")
        (ov / "PORTING.md").write_text("notes\n")
        return dict(dir=d, ovdir=ov, lgi="p", spc="no", inp="new", out="new",
                    kind="I", flags=[], overlay=True, id="App::I::p:no:new:new")

    def test_locate_prefers_the_overlay_then_legacy(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = self.make(tmp)
            f, rel = rt.locate(t, "new.inp")
            self.assertEqual((f, rel), (t["ovdir"] / "SAMPLES" / "new.inp", Path("SAMPLES/new.inp")))
            f, rel = rt.locate(t, "old.inp")
            self.assertEqual((f, rel), (t["dir"] / "SAMPLES" / "old.inp", Path("SAMPLES/old.inp")))
            self.assertEqual(rt.locate(t, "none.out"), (None, None))

    def test_private_copy_puts_the_overlay_over_legacy(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = self.make(tmp)
            wd = Path(tmp) / "wd"
            rt.private_copy(t, wd)
            self.assertEqual((wd / "m.eln").read_text(), "ported module\n")
            self.assertEqual((wd / "p.lgi").read_text(), "legacy lgi\n")
            self.assertTrue((wd / "SAMPLES" / "new.inp").exists())
            self.assertTrue((wd / "SAMPLES" / "old.inp").exists())
            self.assertFalse((wd / "tst_all.overlay").exists())
            self.assertFalse((wd / "PORTING.md").exists())

    def test_produce_runs_with_the_overlay_input(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = self.make(tmp)
            prefix = Path(tmp) / "prefix"
            (prefix / "bin").mkdir(parents=True)
            fake = prefix / "bin" / "elan"
            fake.write_text('#!/bin/sh\necho "elan $*"; cat\n')
            fake.chmod(0o755)
            old = rt.PREFIX
            rt.PREFIX = prefix
            try:
                out, err = rt.produce(t, 30)
            finally:
                rt.PREFIX = old
            self.assertEqual(out, "elan -b p.lgi\nnew input\n")

    def test_produce_reports_a_missing_input(self):
        with tempfile.TemporaryDirectory(dir=rt.HERE) as tmp:
            t = dict(self.make(tmp), inp="absent")
            with self.assertRaises(rt.Unrunnable) as e:
                rt.produce(t, 30)
            self.assertEqual(e.exception.status, "NOINP")
```

- [ ] **Step 2: Run them to verify they fail**

Run: `cd tests/legacy-bench && python3 test_run_tests.py OverlayCopyTest -v`
Expected: ERROR `module 'run_tests' has no attribute 'locate'`.

- [ ] **Step 3: Implement.** In `run_tests.py`, just before `def run_test(t, timeout):` add:

```python
class Unrunnable(Exception):
    """A test that produced no output to compare: status, detail, output so far."""
    def __init__(self, status, detail, out=""):
        super().__init__(status)
        self.status, self.detail, self.out = status, detail, out


def locate(t, name):
    """(file, path relative to its application directory) of the SAMPLES
    file `name` of test t: in the overlay first (spec S9), then in legacy/
    (stored names: CASE-RENAMES.tsv); (None, None) when neither has it."""
    ov = t.get("ovdir")
    for base in ([ov] if ov is not None and ov.is_dir() else []) + [t["dir"]]:
        p = samples(base) / name
        if src_path(p).exists():
            return src_path(p), p.relative_to(base)
    return None, None


def has_lgi(t):
    ov = t.get("ovdir")
    return ((ov is not None and (ov / f"{t['lgi']}.lgi").exists())
            or src_path(t["dir"] / f"{t['lgi']}.lgi").exists())


def private_copy(t, wd):
    """wd: the legacy directory of t, its case names restored, then its
    overlay directory over it (ported files replace legacy ones)."""
    shutil.copytree(t["dir"], wd, symlinks=True,
                    ignore=shutil.ignore_patterns("CVS", "*.o", "a.out", ".elan.*"))
    restore_case(t["dir"], wd, t["lgi"])
    ov = t.get("ovdir")
    if ov is not None and ov.is_dir():
        shutil.copytree(ov, wd, symlinks=True, dirs_exist_ok=True,
                        ignore=shutil.ignore_patterns("tst_all.overlay", "PORTING.md"))


def produce(t, timeout):
    """(stdout, stderr) of test t run in a private copy with PREFIX's tools.
    Raises Unrunnable when there is nothing to compare; lets
    subprocess.TimeoutExpired through. A compiled program reads the test's
    input when it has one, else evaluates its start term (-noInput)."""
    if not has_lgi(t):
        raise Unrunnable("NOLGI", f"missing {t['lgi']}.lgi")
    spc = [] if t["spc"] == "no" else [f"{t['spc']}.spc"]
    flags = [f for f in t["flags"] if f]
    WORK.mkdir(exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="elantest-", dir=WORK))
    try:
        wd = work / t["dir"].name
        private_copy(t, wd)
        inp = None
        if t["inp"]:
            _, rel = locate(t, f"{t['inp']}.inp")
            if rel is None:
                raise Unrunnable("NOINP", f"missing {t['inp']}.inp")
            inp = wd / rel
        compiled = ["./a.out", "-quiet"] if inp is not None else ["./a.out", "-noInput", "-quiet"]
        k = t["kind"]
        if k == "I":
            rc, out, err = run(["elan", "-b", *flags, f"{t['lgi']}.lgi", *spc], wd, inp, timeout)
        elif k == "A":
            rc, o1, e1 = run(["elan", "-b", *flags, "--export", f"{t['lgi']}.ref",
                              f"{t['lgi']}.lgi", *spc], wd, None, timeout)
            if sanitizer_report(e1):
                raise Unrunnable("SANITIZER", f"export: {sanitizer_report(e1)}\n{e1[-4000:]}", o1)
            if rc != 0:
                raise Unrunnable("ERROR", f"export rc={rc}\n{e1[-2000:]}", o1)
            rc, out, err = run(["elan", "-b", "--import", f"{t['lgi']}.ref"], wd, inp, timeout)
        elif k in COMPILER_KINDS:   # elanc-rs (S6) or elanc-go (S7); only with --kinds R/G
            compiler = COMPILER_KINDS[k]
            rc, o1, e1 = run([compiler, "-b", *flags, t["lgi"], *spc], wd, None, timeout * 3)
            if rc == 2:
                raise Unrunnable("UNSUPPORTED", (o1 + e1)[-1500:])
            if rc != 0 or not (wd / "a.out").exists():
                raise Unrunnable("ERROR", f"{compiler} rc={rc}\n{o1[-1500:]}\n{e1[-1500:]}")
            rc, out, err = run(compiled, wd, inp, timeout)
        else:  # J, JO
            opts = ["-b", "-nosplit", "-quiet"] + (["-optimiseChoicePoint"] if k == "JO" else [])
            rc, o1, e1 = run(["elanc-rem", *opts, *flags, t["lgi"], *spc], wd, None, timeout)
            mk = wd / f"{t['lgi']}.make"
            if not mk.exists():
                raise Unrunnable("ERROR", f"elanc-rem rc={rc}\n{o1[-1500:]}\n{e1[-1500:]}")
            rc, o2, e2 = run(["make", "-f", mk.name], wd, None, timeout * 3)
            if rc != 0 or not (wd / "a.out").exists():
                raise Unrunnable("ERROR", f"make rc={rc}\n{(o2 + e2)[-3000:]}")
            rc, out, err = run(compiled, wd, inp, timeout)
        if sanitizer_report(err):
            raise Unrunnable("SANITIZER", f"{sanitizer_report(err)}\n{err[-4000:]}", out)
        return out, err
    finally:
        shutil.rmtree(work, ignore_errors=True)
```

Then replace the body of `run_test` from its first line down to (and including) the line `exp = expected.read_text(encoding="latin-1", errors="replace")` by:

```python
def run_test(t, timeout):
    if modern_only(t, PREFIX, FIXED_CASES):
        return "MODERN-ONLY", "fixes a bug of the 2004 reference (fixes-2004)", ""
    expected, _ = locate(t, f"{t['out']}.out")
    if expected is None:
        return "NOREF", f"missing {t['out']}.out", ""
    try:
        out, err = produce(t, timeout)
    except Unrunnable as u:
        return u.status, u.detail, u.out
    except subprocess.TimeoutExpired:
        return "TIMEOUT", f"> {timeout}s", ""
    k = t["kind"]
    exp = expected.read_text(encoding="latin-1", errors="replace")
```

and delete, at the end of `run_test`, the now unmatched `except subprocess.TimeoutExpired:` / `finally: shutil.rmtree(work, ...)` lines (the comparison code between them stays, dedented by one level so that it belongs to the function body; the `rc` used in the FAIL detail is no longer available: replace `tail = f"\n--- rc={rc} stderr:\n{err[-1500:]}" if (rc or err.strip()) else ""` by `tail = f"\n--- stderr:\n{err[-1500:]}" if err.strip() else ""`).

- [ ] **Step 4: Run the unit tests**

Run: `cd tests/legacy-bench && python3 test_run_tests.py -v 2>&1 | tail -3`
Expected: `OK`.

- [ ] **Step 5: Run the bench (no change of behaviour)**

Run: `python3 tests/legacy-bench/run_tests.py --prefix build/install --kinds I,A | tail -3 && python3 tests/legacy-bench/run_tests.py --prefix build/install --kinds J,JO | tail -3`
Expected: `vs baseline: 0 regressions, 0 improvements` and `vs snapshots: 0 differences` for both, the same counts as on `main` (388 I/A, 410 J/JO).

- [ ] **Step 6: Commit**

```bash
git add tests/legacy-bench/run_tests.py tests/legacy-bench/test_run_tests.py
git commit -m "bench: private copy = legacy + case names + overlay; produce() (compiled programs read the input of gtest/J lines)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 3: Overlay tests are judged strictly

**Files:**
- Modify: `tests/legacy-bench/run_tests.py` (`judge()`, its call in `main()`)
- Test: `tests/legacy-bench/test_run_tests.py`

**Interfaces:**
- Produces: `judge(statuses, base, exceptions, overlay_ids=frozenset()) -> (regressions, improvements)`. An overlay test not in the baseline must be PASS (kind G: PASS or UNSUPPORTED); MODERN-ONLY is exempt. An overlay test in the baseline is judged against it like any other.

- [ ] **Step 1: Write the failing tests:**

```python
class OverlayJudgeTest(unittest.TestCase):
    def test_overlay_tests_must_pass_unless_in_the_baseline(self):
        st = {"A::I::p:no:q:q": "PASS~", "A::G::p:no:q:q-c": "UNSUPPORTED",
              "A::I::r:no:r:r": "FAIL", "A::I::h:no:h:h": "FAIL"}
        regress, _ = rt.judge(st, {"A::I::r:no:r:r": "FAIL"}, set(),
                              overlay_ids=set(st))
        self.assertEqual(sorted(r[0] for r in regress), ["A::I::h:no:h:h", "A::I::p:no:q:q"])

    def test_historical_tests_keep_the_old_rule(self):
        regress, _ = rt.judge({"A::I::p:no:p:p": "FAIL"}, {}, set())
        self.assertEqual(regress, [])
```

- [ ] **Step 2: Run them to verify they fail**

Run: `cd tests/legacy-bench && python3 test_run_tests.py OverlayJudgeTest -v`
Expected: ERROR `judge() got an unexpected keyword argument 'overlay_ids'`.

- [ ] **Step 3: Implement.** Change the signature to `def judge(statuses, base, exceptions, overlay_ids=frozenset()):`, add to its docstring `Overlay tests (spec S9) not in the baseline must PASS (kind G: or UNSUPPORTED): the reference is their oracle.`, and insert, just before `if tid in base:`:

```python
        if tid in overlay_ids and tid not in base:
            ok = ("PASS", "UNSUPPORTED") if "::G::" in tid else ("PASS",)
            if new not in ok:
                regress.append((tid, "PASS", new))
            continue
```

In `main()`, replace `regress, improve = judge({tid: results[tid][0] for tid in ids}, base, exceptions)` by:

```python
    overlay_ids = {t["id"] for t in tests if t.get("overlay")}
    regress, improve = judge({tid: results[tid][0] for tid in ids}, base, exceptions,
                             overlay_ids)
```

and replace `if base or regress:` by `if base or regress or overlay_ids:`.

- [ ] **Step 4: Run the unit tests**

Run: `cd tests/legacy-bench && python3 test_run_tests.py -v 2>&1 | tail -3`
Expected: `OK`.

- [ ] **Step 5: Commit**

```bash
git add tests/legacy-bench/run_tests.py tests/legacy-bench/test_run_tests.py
git commit -m "bench: overlay tests must PASS unless listed in the baseline (G: or UNSUPPORTED)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 4: `make_expected.py` — expected outputs from the reference

**Files:**
- Create: `tests/legacy-overlay/make_expected.py`
- Create: `tests/legacy-overlay/test_overlay.py`
- Create: `tests/legacy-overlay/README.md`

**Interfaces:**
- Consumes: `rt.discover()`, `rt.locate()`, `rt.produce()`, `rt.Unrunnable`, `rt.PREFIX` (Tasks 1-2).
- Produces: `plan(tests) -> list[tuple[dict, Path]]` (one job per distinct `(dir, out)` of overlay tests: the test to run on the reference — kind `I` for itest/aitest lines, `J` for gtest lines — and the target `ovdir / "SAMPLES" / f"{out}.out"`; `SystemExit` when an itest/aitest line and a gtest line share an `out`); `write_expected(jobs, force, timeout) -> tuple[int, int, int]` (written, kept, failed).

- [ ] **Step 1: Write the failing tests** — `tests/legacy-overlay/test_overlay.py`:

```python
#!/usr/bin/env python3
"""Unit tests of the overlay scripts (spec S9; run: python3 test_overlay.py)."""
import sys
import tempfile
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(0, str(HERE.parent / "legacy-bench"))
import make_expected as me  # noqa: E402
import run_tests as rt  # noqa: E402


def overlay_test(tmp, kind="I", out="q", inp="q"):
    d = Path(tmp) / "legacy" / "App"
    d.mkdir(parents=True, exist_ok=True)
    (d / "p.lgi").write_text("lgi\n")
    ov = Path(tmp) / "overlay" / "App"
    (ov / "SAMPLES").mkdir(parents=True, exist_ok=True)
    (ov / "SAMPLES" / f"{inp}.inp").write_text("the query\n")
    return dict(id=f"App::{kind}::p:no:{inp}:{out}", dir=d, ovdir=ov, kind=kind, lgi="p",
                spc="no", inp=inp, out=out, flags=[], overlay=True, long=False)


class PlanTest(unittest.TestCase):
    def test_one_job_per_output_I_for_itest_aitest_J_for_gtest(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            i = overlay_test(tmp, "I")
            a = dict(i, kind="A", id="App::A::p:no:q:q")
            g = overlay_test(tmp, "G", out="q-c")
            h = dict(i, overlay=False, id="App::I::h:no:h:h", out="h")
            jobs = me.plan([i, a, g, h])
            self.assertEqual([(t["kind"], p.name) for t, p in jobs], [("I", "q.out"), ("J", "q-c.out")])
            self.assertEqual(jobs[0][1], i["ovdir"] / "SAMPLES" / "q.out")

    def test_an_output_shared_by_interpreter_and_compiled_lines_is_refused(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            with self.assertRaises(SystemExit):
                me.plan([overlay_test(tmp, "I"), overlay_test(tmp, "G")])


class WriteExpectedTest(unittest.TestCase):
    def fake_reference(self, tmp):
        prefix = Path(tmp) / "ref"
        (prefix / "bin").mkdir(parents=True)
        elan = prefix / "bin" / "elan"
        elan.write_text('#!/bin/sh\necho "reference $*"; cat\n')
        elan.chmod(0o755)
        return prefix

    def test_writes_then_keeps_unless_forced(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = overlay_test(tmp)
            old = rt.PREFIX
            rt.PREFIX = self.fake_reference(tmp)
            try:
                jobs = me.plan([t])
                self.assertEqual(me.write_expected(jobs, False, 30), (1, 0, 0))
                target = t["ovdir"] / "SAMPLES" / "q.out"
                self.assertEqual(target.read_text(), "reference -b p.lgi\nthe query\n")
                target.write_text("edited\n")
                self.assertEqual(me.write_expected(jobs, False, 30), (0, 1, 0))
                self.assertEqual(target.read_text(), "edited\n")
                self.assertEqual(me.write_expected(jobs, True, 30), (1, 0, 0))
            finally:
                rt.PREFIX = old

    def test_a_historical_output_in_legacy_is_kept(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            t = overlay_test(tmp)
            (t["dir"] / "q.out").write_text("historical\n")   # samples(dir) = dir
            old = rt.PREFIX
            rt.PREFIX = self.fake_reference(tmp)
            try:
                self.assertEqual(me.write_expected(me.plan([t]), True, 30), (0, 1, 0))
            finally:
                rt.PREFIX = old


if __name__ == "__main__":
    unittest.main()
```

- [ ] **Step 2: Run them to verify they fail**

Run: `cd tests/legacy-overlay && python3 test_overlay.py -v`
Expected: `ModuleNotFoundError: No module named 'make_expected'`.

- [ ] **Step 3: Implement** — `tests/legacy-overlay/make_expected.py`:

```python
#!/usr/bin/env python3
"""Write the expected outputs of the overlay's tests (spec S9) with the 2004
reference system: for an itest/aitest line, the reference's interpreter
(`elan -b LGI [SPC] < INP`); for a gtest line, the program compiled by the
reference's REM (`elanc-rem -b -nosplit -quiet LGI [SPC]; make; ./a.out
-quiet < INP`). The output goes to <overlay app dir>/SAMPLES/<OUT>.out.
An existing output is kept: always a historical one of legacy/ (which is
never written), an overlay one unless --force.
Usage: make_expected.py [--reference PREFIX] [--filter REGEX] [--force]
                        [--timeout S]
PREFIX: --reference, else $REFERENCE_PREFIX, else reference/install."""
import argparse
import os
import re
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "legacy-bench"))
import run_tests as rt  # noqa: E402


def plan(tests):
    """One (test to run on the reference, target file) per distinct output
    of the overlay tests: kind I for itest/aitest lines, J for gtest lines."""
    jobs, kinds = [], {}
    for t in tests:
        if not t.get("overlay"):
            continue
        kind = "J" if t["kind"] == "G" else "I"
        key = (t["dir"], t["out"])
        if key in kinds:
            if kinds[key] != kind:
                raise SystemExit(f"{t['id']}: output {t['out']} is used by interpreter and "
                                 "compiled lines: give the gtest line its own output name")
            continue
        kinds[key] = kind
        jobs.append((dict(t, kind=kind), t["ovdir"] / "SAMPLES" / f"{t['out']}.out"))
    return jobs


def write_expected(jobs, force, timeout):
    """Run each job on rt.PREFIX; return (written, kept, failed)."""
    written = kept = failed = 0
    for t, target in jobs:
        found, _ = rt.locate(t, target.name)
        if found is not None and not (force and found == target):
            kept += 1
            continue
        try:
            out, _err = rt.produce(t, timeout)
        except rt.Unrunnable as u:
            print(f"FAILED {t['id']}: {u.status}\n{u.detail[-800:]}")
            failed += 1
            continue
        except subprocess.TimeoutExpired:
            print(f"FAILED {t['id']}: TIMEOUT > {timeout}s")
            failed += 1
            continue
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(out, encoding="latin-1")
        print(f"wrote {target}")
        written += 1
    return written, kept, failed


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--reference")
    ap.add_argument("--filter", default="")
    ap.add_argument("--force", action="store_true")
    ap.add_argument("--timeout", type=int, default=60)
    a = ap.parse_args(argv)
    ref = Path(a.reference or os.environ.get("REFERENCE_PREFIX")
               or rt.REPO / "reference" / "install").resolve()
    if not (ref / "bin" / "elan").exists():
        print(f"make_expected: no reference system in {ref} (make reference)", file=sys.stderr)
        return 2
    rt.PREFIX = ref
    os.environ.pop("ELAN_TEST_LIB", None)
    tests = [t for t in rt.discover() if re.search(a.filter, t["id"])]
    written, kept, failed = write_expected(plan(tests), a.force, a.timeout)
    print(f"make_expected: {written} written, {kept} kept, {failed} failed")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
```

`tests/legacy-overlay/README.md`:

```markdown
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
```

- [ ] **Step 4: Run the tests**

Run: `cd tests/legacy-overlay && python3 test_overlay.py -v 2>&1 | tail -3`
Expected: `OK`.

- [ ] **Step 5: Commit**

```bash
git add tests/legacy-overlay
git commit -m "legacy-overlay: make_expected.py writes expected outputs with the 2004 reference

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 5: `check_coverage.py`, `unloadable.tsv`, `todo.tsv`, `make check-overlay`

**Files:**
- Create: `tests/legacy-overlay/check_coverage.py`
- Create: `tests/legacy-overlay/unloadable.tsv`, `tests/legacy-overlay/todo.tsv` (generated in Step 5)
- Modify: `tests/legacy-overlay/test_overlay.py`, `Makefile`

**Interfaces:**
- Consumes: `rt.discover()`, `rt.ELAN3`, `rt.CASE_RENAMES`.
- Produces: `programs(root) -> set[str]` (paths relative to `root` = `legacy/elan3`, original names, of the `.lgi` files under `applications/` and `contributions/`, CVS excluded); `tested(tests, root) -> set[str]`; `load_list(path) -> dict[str, str]` (first column → rest of the line, `#` comments and blank lines skipped); `check(progs, tested, unloadable, todo) -> list[str]` (problems).

- [ ] **Step 1: Write the failing tests** (add to `test_overlay.py`, before `if __name__`; add `import check_coverage as cc` beside `import make_expected as me`):

```python
class CoverageTest(unittest.TestCase):
    def test_every_program_in_exactly_one_state(self):
        progs = {"applications/A/p.lgi", "applications/A/q.lgi", "applications/B/r.lgi",
                 "applications/B/s.lgi"}
        problems = cc.check(progs, tested={"applications/A/p.lgi", "applications/B/s.lgi"},
                            unloadable={"applications/A/q.lgi": "ELAN 2 syntax"},
                            todo={"applications/B/s.lgi": "", "applications/C/x.lgi": ""})
        self.assertEqual(sorted(problems), [
            "applications/B/r.lgi: not tested, not in unloadable.tsv nor todo.tsv",
            "applications/B/s.lgi: in 2 states (tested, todo.tsv)",
            "applications/C/x.lgi: listed but no such program",
        ])

    def test_programs_uses_original_names_and_skips_cvs(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            root = Path(tmp)
            (root / "applications" / "A" / "CVS").mkdir(parents=True)
            (root / "applications" / "A" / "p.lgi").write_text("")
            (root / "applications" / "A" / "CVS" / "old.lgi").write_text("")
            (root / "contributions" / "C").mkdir(parents=True)
            (root / "contributions" / "C" / "c.lgi").write_text("")
            self.assertEqual(cc.programs(root), {"applications/A/p.lgi", "contributions/C/c.lgi"})

    def test_robot_upper_case_variant_has_its_original_name(self):
        progs = cc.programs(rt.ELAN3)
        self.assertIn("applications/Robot/Nim.lgi", progs)
        self.assertIn("applications/Robot/nim.lgi", progs)
        self.assertNotIn("applications/Robot/x-case-Nim.lgi", progs)
```

- [ ] **Step 2: Run them to verify they fail**

Run: `cd tests/legacy-overlay && python3 test_overlay.py CoverageTest -v`
Expected: `ModuleNotFoundError: No module named 'check_coverage'`.

- [ ] **Step 3: Implement** — `tests/legacy-overlay/check_coverage.py`:

```python
#!/usr/bin/env python3
"""Every program (.lgi) of legacy/elan3/applications and contributions is
tested (a tst_all or tst_all.overlay line names it), or listed in
unloadable.tsv (path<TAB>reason), or still to do (todo.tsv) — spec S9 §3.1.
Fails on a program in none of these states or in two, and on a listed path
that names no program. Paths are relative to legacy/elan3, with original
names (CASE-RENAMES.tsv).
Usage: check_coverage.py [--write-todo]
       --write-todo: list in todo.tsv every program neither tested nor
       unloadable (used once, when S9 starts; afterwards the list shrinks)."""
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "legacy-bench"))
import run_tests as rt  # noqa: E402

TREES = ("applications", "contributions")
UNLOADABLE = HERE / "unloadable.tsv"
TODO = HERE / "todo.tsv"


def programs(root):
    orig = {s: o for s, o in rt.CASE_RENAMES}
    found = set()
    for top in TREES:
        for p in (root / top).rglob("*.lgi"):
            if "CVS" in p.parts:
                continue
            found.add(orig.get(p, p).relative_to(root).as_posix())
    return found


def tested(tests, root):
    return {(t["dir"] / f"{t['lgi']}.lgi").relative_to(root).as_posix()
            for t in tests if root in t["dir"].parents}


def load_list(path):
    entries = {}
    if path.exists():
        for line in path.read_text().splitlines():
            if line.strip() and not line.startswith("#"):
                first, _, rest = line.partition("\t")
                entries[first.strip()] = rest.strip()
    return entries


def check(progs, tested, unloadable, todo):
    problems = []
    for p in sorted(progs):
        states = [n for n, s in (("tested", tested), ("unloadable.tsv", unloadable),
                                 ("todo.tsv", todo)) if p in s]
        if not states:
            problems.append(f"{p}: not tested, not in unloadable.tsv nor todo.tsv")
        elif len(states) > 1:
            problems.append(f"{p}: in {len(states)} states ({', '.join(states)})")
    for p in sorted((set(unloadable) | set(todo)) - progs):
        problems.append(f"{p}: listed but no such program")
    return problems


def main(argv):
    progs = programs(rt.ELAN3)
    done = tested(rt.discover(), rt.ELAN3)
    unloadable, todo = load_list(UNLOADABLE), load_list(TODO)
    if argv == ["--write-todo"]:
        rest = sorted(progs - done - set(unloadable))
        TODO.write_text("# programs of S9 still to test (spec S9 §5); check_coverage.py\n"
                        + "".join(f"{p}\n" for p in rest))
        print(f"check_coverage: {len(rest)} programs written to {TODO.name}")
        return 0
    problems = check(progs, done & progs, unloadable, todo)
    for p in problems:
        print(f"check_coverage: {p}")
    print(f"check_coverage: {len(progs)} programs: {len(done & progs)} tested, "
          f"{len(unloadable)} unloadable, {len(todo)} to do"
          + ("" if not problems else f", {len(problems)} problems"))
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
```

- [ ] **Step 4: Run the unit tests**

Run: `cd tests/legacy-overlay && python3 test_overlay.py -v 2>&1 | tail -3`
Expected: `OK`.

- [ ] **Step 5: Create the lists**

```bash
printf '# programs of legacy/elan3/{applications,contributions} that cannot be loaded\n# even with a port (spec S9 §5): path<TAB>reason\n' > tests/legacy-overlay/unloadable.tsv
python3 tests/legacy-overlay/check_coverage.py --write-todo
python3 tests/legacy-overlay/check_coverage.py
```

Expected: `179 programs written to todo.tsv`, then `check_coverage: 293 programs: 114 tested, 0 unloadable, 179 to do` (the tested count is 293 - 179; if it differs, the tested count is what the bench's `discover()` finds and the todo count is its complement — note both in the commit message), exit status 0.

- [ ] **Step 6: Wire it into `make check`.** In `Makefile`: add `check-overlay` to `.PHONY`; change `check: check-case smoke test-runner` to `check: check-case check-overlay smoke test-runner`; add after the `check-case` rule:

```make
# Spec S9: every program of legacy/elan3/{applications,contributions} is
# tested, unloadable or to do; unit tests of the overlay scripts.
check-overlay:
	cd tests/legacy-overlay && python3 test_overlay.py && python3 check_coverage.py
```

and in the help comment at the top, after the `make check-case` line:

```make
#   make check-overlay   S9 overlay: coverage of the legacy applications, script unit tests
```

Run: `make check-overlay`
Expected: `OK` and `check_coverage: 293 programs: ...`, exit 0.

- [ ] **Step 7: Commit**

```bash
git add tests/legacy-overlay Makefile
git commit -m "legacy-overlay: check_coverage.py (make check-overlay), todo.tsv with the 179 untested programs

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

---

### Task 6: `triage.py` and the triage report

**Files:**
- Create: `tests/legacy-overlay/triage.py`
- Create: `tests/legacy-overlay/triage.tsv` (generated)
- Modify: `tests/legacy-overlay/test_overlay.py`

**Interfaces:**
- Consumes: `cc.load_list()`, `cc.TODO`, `rt.produce()`, `rt.Unrunnable`, `rt.ELAN3`, `rt.OVERLAY`, `rt.samples()`, `rt.PREFIX`.
- Produces: `load_status(path, prefix, timeout) -> str` (`ok`, or the first `[error]`/`fatal` line of stderr, or `rc=N`, or `TIMEOUT`); `candidates(path) -> tuple[list[str], list[str]]` (names of `.inp`/`.out` files of the application's SAMPLES directory whose stem contains the program's name, case-insensitively); `triage.tsv` columns `path, reference, modern, inputs, outputs`.

- [ ] **Step 1: Write the failing test** (add to `test_overlay.py`; add `import triage as tr`):

```python
class TriageTest(unittest.TestCase):
    def test_load_status_with_a_fake_prefix(self):
        with tempfile.TemporaryDirectory(dir=HERE) as tmp:
            prefix = Path(tmp) / "p"
            (prefix / "bin").mkdir(parents=True)
            (prefix / "bin" / "elan").write_text(
                '#!/bin/sh\ncase "$2" in good.lgi) exit 0;; *) echo "[error] bad module" >&2; exit 1;; esac\n')
            (prefix / "bin" / "elan").chmod(0o755)
            app = Path(tmp) / "App"
            app.mkdir()
            (app / "good.lgi").write_text("")
            (app / "other.lgi").write_text("")
            self.assertEqual(tr.load_status_in(app, "good", prefix, 30), "ok")
            self.assertEqual(tr.load_status_in(app, "absent", prefix, 30), "NOLGI")
            self.assertEqual(tr.load_status_in(app, "other", prefix, 30), "[error] bad module")

    def test_candidates_match_the_program_name(self):
        inps, outs = tr.candidates("applications/APT/bubleLOG.lgi")
        self.assertIn("bubleLOG.inp", inps)
        self.assertIn("bubleLOG.out", outs)
```

- [ ] **Step 2: Run it to verify it fails**

Run: `cd tests/legacy-overlay && python3 test_overlay.py TriageTest -v`
Expected: `ModuleNotFoundError: No module named 'triage'`.

- [ ] **Step 3: Implement** — `tests/legacy-overlay/triage.py`:

```python
#!/usr/bin/env python3
"""Triage of the programs of todo.tsv (spec S9 §5): does each one load with
the 2004 reference and with the modern interpreter (`elan -b PROG.lgi`
with no input), and which files of its SAMPLES directory look like its
inputs/outputs. Written to triage.tsv: path, reference, modern, inputs,
outputs. A report to read, not a test.
Usage: triage.py [--reference PREFIX] [--modern PREFIX] [--timeout S]
       --reference: else $REFERENCE_PREFIX, else reference/install;
       --modern: else build/install."""
import argparse
import os
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "legacy-bench"))
import run_tests as rt  # noqa: E402
import check_coverage as cc  # noqa: E402

REPORT = HERE / "triage.tsv"


def load_status_in(app, name, prefix, timeout):
    """Load status of app/name.lgi with prefix's elan (see the module doc)."""
    ovdir = rt.OVERLAY / app.relative_to(rt.ROOT) if rt.ROOT in app.parents else None
    t = dict(id=name, dir=app, ovdir=ovdir, kind="I",
             lgi=name, spc="no", inp=None, out=name, flags=[], overlay=False)
    old = rt.PREFIX
    rt.PREFIX = prefix
    try:
        out, err = rt.produce(t, timeout)
    except rt.Unrunnable as u:
        return u.status
    except subprocess.TimeoutExpired:
        return "TIMEOUT"
    finally:
        rt.PREFIX = old
    for line in err.splitlines():
        if "[error]" in line or "fatal" in line:
            return line.strip()[:200]
    return "ok"


def candidates(path):
    p = rt.ELAN3 / path
    sd = rt.samples(p.parent)
    name = Path(path).stem.lower()
    pick = lambda ext: sorted(f.name for f in sd.glob(f"*.{ext}") if name in f.stem.lower())
    return pick("inp"), pick("out")


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--reference")
    ap.add_argument("--modern", default=str(rt.REPO / "build" / "install"))
    ap.add_argument("--timeout", type=int, default=30)
    a = ap.parse_args(argv)
    ref = Path(a.reference or os.environ.get("REFERENCE_PREFIX")
               or rt.REPO / "reference" / "install").resolve()
    modern = Path(a.modern).resolve()
    os.environ.pop("ELAN_TEST_LIB", None)
    todo = sorted(cc.load_list(cc.TODO))

    def one(path):
        p = rt.ELAN3 / path
        name = Path(path).stem
        inps, outs = candidates(path)
        return (path, load_status_in(p.parent, name, ref, a.timeout),
                load_status_in(p.parent, name, modern, a.timeout), ",".join(inps), ",".join(outs))

    # one program at a time: rt.env() reads the module global rt.PREFIX
    rows = [one(p) for p in todo]
    REPORT.write_text("# path\treference\tmodern\tinputs\toutputs (triage.py)\n"
                      + "".join("\t".join(r) + "\n" for r in rows))
    both = sum(1 for r in rows if r[1] == "ok" and r[2] == "ok")
    ref_only = sum(1 for r in rows if r[1] == "ok" and r[2] != "ok")
    neither = sum(1 for r in rows if r[1] != "ok")
    print(f"triage: {len(rows)} programs: {both} load with both, {ref_only} with the reference "
          f"only, {neither} not with the reference; report in {REPORT.name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
```

(`rt.PREFIX` is a module global read by `rt.env()`; the triage therefore runs one program at a time to keep the two prefixes apart. 179 × 2 loads take a few minutes.)

- [ ] **Step 4: Run the unit tests**

Run: `cd tests/legacy-overlay && python3 test_overlay.py -v 2>&1 | tail -3`
Expected: `OK`.

- [ ] **Step 5: Produce the report** (needs Task 0):

```bash
python3 tests/legacy-overlay/triage.py
```

Expected: `triage: 179 programs: X load with both, Y with the reference only, Z not with the reference; report in triage.tsv`. Read `triage.tsv`: every row has a status in both columns; note X, Y, Z for the commit message and the S9b-S9e plans.

- [ ] **Step 6: Commit**

```bash
git add tests/legacy-overlay
git commit -m "legacy-overlay: triage.py and the triage of the 179 programs (X both, Y reference only, Z neither)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

(with X, Y, Z replaced by the numbers printed in Step 5.)

---

### Task 7: First program tested end to end through the overlay

**Files:**
- Create: `tests/legacy-overlay/elan3/<APP>/tst_all.overlay`, `tests/legacy-overlay/elan3/<APP>/SAMPLES/<PROG>.inp`, `SAMPLES/<PROG>.out`, `SAMPLES/<PROG>-c.out`
- Modify: `tests/legacy-overlay/todo.tsv`, `tests/compiler-rs/bench_g_expected.tsv`

**Choice of the program.** The first row of `triage.tsv` (in file order) whose `reference` and `modern` columns are both `ok`, and for which a query can be written from its `.lgi`/`.eln` in a few minutes (a constant or a small term of the query sort). `APP` is its directory below `legacy/elan3` (e.g. `applications/Simplest`), `PROG` its name.

- [ ] **Step 1: Write the query.** Read `legacy/elan3/$APP/$PROG.lgi` (query sort, start strategy) and its modules; write `tests/legacy-overlay/elan3/$APP/SAMPLES/$PROG.inp` with one or two queries in the bench's format (each query followed by `end`), e.g. `queens end`.

- [ ] **Step 2: Add the lines** — `tests/legacy-overlay/elan3/$APP/tst_all.overlay`:

```
itest  PROG no PROG PROG
aitest PROG no PROG PROG
gtest  PROG no PROG PROG-c
```

(with `PROG` replaced by the program's name).

- [ ] **Step 3: Generate the expected outputs with the reference**

```bash
python3 tests/legacy-overlay/make_expected.py --filter "$APP::"
```

Expected: `make_expected: 2 written, 0 kept, 0 failed`. Read both `.out` files: they show the program's results (not an error message about the query). If the query is rejected, fix the `.inp`, delete the `.out` files and run again.

- [ ] **Step 4: Remove the program from `todo.tsv`**: delete its line, the path relative to `legacy/elan3` (`$APP/$PROG.lgi`, e.g. `applications/Simplest/queens4.lgi`).

Run: `make check-overlay`
Expected: `... 115 tested, 0 unloadable, 178 to do`, exit 0.

- [ ] **Step 5: Run the bench on it**

```bash
python3 tests/legacy-bench/run_tests.py --prefix build/install --kinds I,A --filter "$APP::.*$PROG" | tail -4
python3 tests/legacy-bench/run_tests.py --prefix build/install --kinds G --filter "$APP::G::$PROG" | tail -4
python3 tests/legacy-bench/run_tests.py --prefix reference/install --kinds I,A --filter "$APP::.*$PROG" | tail -4
```

Expected: I and A `PASS` with the modern system and with the reference; G `PASS` or `UNSUPPORTED`; `0 regressions`. If the modern system differs from the reference, this program is a category-4 case: choose the next row of `triage.tsv` instead and record this one in the S9e notes (`tests/legacy-overlay/triage.tsv` keeps it).

- [ ] **Step 6: Update the G expectations**

```bash
python3 tests/compiler-rs/bench_parity.py build/install --kind G --save
```

Expected: `saved N cases in bench_g_expected.tsv` with N = the previous count + 1 (if G passed) or the same count (UNSUPPORTED cases are counted only), no `PARITY:` line.

- [ ] **Step 7: Commit**

```bash
git add tests/legacy-overlay tests/compiler-rs/bench_g_expected.tsv
git commit -m "legacy-overlay: first program through the overlay, APP/PROG (I, A, G against the reference)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```

(with APP/PROG replaced.)

---

### Task 8: Documentation and full verification

**Files:**
- Modify: `README.md` (the bench paragraph), `tests/legacy-bench/README.md`, `docs/superpowers/specs/2026-10-12-s9-legacy-apps-tests-design.md` (status line: S9a done, with the triage figures)

- [ ] **Step 1: Document.** In `tests/legacy-bench/README.md`, after the paragraph on the private copy, add:

```markdown
Tests are also read from the overlay `tests/legacy-overlay/` (spec S9):
its `tst_all.overlay` files add `itest`/`aitest` lines and `gtest` lines
(kind G, the compiled program reading the input); the private copy of an
application is the legacy directory with the overlay over it. An overlay
test's expected output is the 2004 reference's (`make_expected.py`), and it
must PASS unless `baseline.tsv` lists it. See `tests/legacy-overlay/README.md`.
```

In `README.md`, in the paragraph that describes `tests/legacy-bench`, add one sentence: `Programs of legacy/elan3 that had no test are added through the overlay tests/legacy-overlay/ (spec S9).` In the spec, replace `Status: decided by the user.` by `Status: decided by the user; S9a (infrastructure) done on <date>: triage X both, Y reference only, Z neither.` (figures of Task 6).

- [ ] **Step 2: Full check on the case-sensitive volume**

Run: `make check` (in `~/github/elan-cs`).
Expected: exit 0; `check_coverage: 293 programs: ...`; both bench runs `0 regressions`, `0 differences`.

- [ ] **Step 3: Full check on the default volume.** Commit Step 1, then in a fresh clone of this branch on the default volume:

```bash
git clone -q -b s9-legacy-apps ~/github/elan-cs /tmp/elan-s9check
cd /tmp/elan-s9check && make check; echo rc=$?
```

Expected: `rc=0` with the same bench figures. Then `rm -rf /tmp/elan-s9check`.

- [ ] **Step 4: Against the reference**

Run: `make check-reference` (in `~/github/elan-cs`).
Expected: `0 regressions`; the overlay I/A tests `PASS`.

- [ ] **Step 5: Commit**

```bash
git add README.md tests/legacy-bench/README.md docs/superpowers/specs/2026-10-12-s9-legacy-apps-tests-design.md
git commit -m "docs: S9a, the overlay of legacy/ (README, bench README, spec status)

Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>"
```
