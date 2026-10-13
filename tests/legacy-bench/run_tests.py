#!/usr/bin/env python3
"""ELAN regression test bench.

Replays the test bench of the ELAN project against an installed ELAN system:
the `TESTS` files of applications/ and contributions/ (the living copy of the
ELAN 3 applications, spec S9 §8; format: tests/apps/README.md) and the
historical `tst_all` files of legacy/elan/sources (they drive the csh scripts
`itest`, `aitest` and `jtest` from elan/sources/Scripts). The system is by
default the reference build (reference/install, see reference/build.sh); use
--prefix to test another one (e.g. the modern port).

Test kinds (same semantics as the original scripts):
  I  itest  : elan -b LGI.lgi [SPC.spc] < SAMPLES/INP.inp       == SAMPLES/OUT.out
  A  aitest : elan -b --export LGI.ref LGI.lgi [SPC.spc];
              elan -b --import LGI.ref < SAMPLES/INP.inp         == SAMPLES/OUT.out
  J  jtest  : elanc-rem -b -nosplit -quiet LGI [SPC]; make; a.out -noInput -quiet == SAMPLES/OUT.out
              (-noInput = evaluate the compiled `start with` query; it was the default
              when the reference outputs were produced)
  JO jtest  : same with -optimiseChoicePoint (second half of the original jtest)
  R  jtest  : elanc-rs (Rust compiler, spec S6) LGI [SPC]; a.out -noInput -quiet,
              rewrite_step lines ignored; UNSUPPORTED when elanc-rs refuses (exit 2).
              Not in the default kinds.
  G  jtest  : the same with elanc-go (Go compiler, spec S7; a go build cache in the
              bench work directory). Not in the default kinds.

ctest/actest/cctest (old C compiler) and pitest/pevaltest (partial evaluator)
have no script anymore and are not replayed.

Each test runs in a private copy of its application directory.

Two oracles:
  * historical: SAMPLES/OUT.out (some predate later library/printer changes)
  * snapshot  : tests/snapshots/<id>.out, the outputs of a trusted build
                (--save-snapshots); used to check that code changes preserve
                behaviour exactly. Status "SNAP-OK"/"SNAP-DIFF" is reported separately.

Usage:
  run_tests.py [--list] [--kinds I,A,J,JO,R,G] [--filter REGEX] [-j N] [--timeout S]
               [--save-baseline] [--long/--no-long]
Results are written to tests/results/<timestamp>/ (summary.tsv, one .diff per failure)
and compared with tests/baseline.tsv when it exists.
"""
import argparse, concurrent.futures as cf, datetime, difflib, os, re, shutil
import subprocess, sys, tempfile, time
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
ROOT = REPO / "legacy"                    # verbatim legacy sources (read only)
ELAN3 = ROOT / "elan3"
PREFIX = Path(os.environ.get("ELAN_PREFIX", REPO / "reference" / "install"))
OLDLIB_SRC = ROOT / "elan" / "src" / "elan-library" / "elanlib"
BASELINE = HERE / "baseline.tsv"
SNAPDIR = HERE / "snapshots"
WORK = HERE / "work"   # scratch copies of the application directories
REGRESSION = REPO / "tests" / "regression"
EXAMPLES = REPO / "examples"
EXCEPTIONS = HERE / "platform-exceptions.tsv"

# Files renamed so that no two paths differ only by case (CASE-RENAMES.tsv:
# stored path, original path). A test sees the original names: its private
# copy of the application gets them back when it uses one of them.
def load_case_renames(path=REPO / "CASE-RENAMES.tsv"):
    pairs = []
    for line in path.read_text().splitlines():
        if line and not line.startswith("#"):
            stored, orig = line.split("\t")
            pairs.append((REPO / stored, REPO / orig))
    return pairs


CASE_RENAMES = load_case_renames()
STORED_OF = {orig: stored for stored, orig in CASE_RENAMES}


def src_path(p):
    """Where the original file p is stored (checked before p itself: on a
    case-insensitive file system p would find its twin)."""
    return STORED_OF.get(p, p)


def restore_case(src, dst, lgi=None):
    """In dst, a copy of src, give the renamed files of src their original
    names: all of them when lgi is None, else only if src/lgi.lgi is one
    (the files of a case variant go together: Robot/Nim.lgi imports Nim,
    Space, State). On a case-insensitive file system a restored file
    replaces its twin, which the test does not use (removed first, or the
    twin's name would stay)."""
    under = [(s, o) for s, o in CASE_RENAMES if src in o.parents]
    if lgi is not None and src / f"{lgi}.lgi" not in STORED_OF:
        return
    for stored, orig in under:
        target = dst / orig.relative_to(src)
        for twin in os.listdir(target.parent):
            if twin != target.name and twin.lower() == target.name.lower():
                os.remove(target.parent / twin)
        os.replace(dst / stored.relative_to(src), target)


# Historical tst_all files still read in legacy/ (compiler test directories,
# BenchThesis: not copied), and the living copy of the ELAN 3 applications
# (spec S9 §8), whose tests are in TESTS files (tests/apps/README.md).
SUITES = [ROOT / "elan" / "sources"]
APPS = [REPO / "applications", REPO / "contributions"]

# gtest LGI SPC INP OUT (spec S9): elanc-go, the compiled program reading INP
KIND_OF_CMD = {"itest": ["I"], "aitest": ["A"], "jtest": ["J", "JO", "R", "G"],
               "gtest": ["G"]}
# kinds compiled by the new compilers (not in the default kinds): spec S6
# (elanc-rs, Rust) and S7 (elanc-go, Go)
COMPILER_KINDS = {"R": "elanc-rs", "G": "elanc-go"}


def env():
    e = dict(os.environ)
    java = os.environ.get("JAVA_HOME", "/opt/homebrew/opt/openjdk")
    e["ELANLIB"] = os.environ.get("ELAN_TEST_LIB", str(PREFIX))
    e["PATH"] = f"{PREFIX}/bin:{java}/bin:" + e.get("PATH", "")
    e.pop("SECONDELANLIB", None)
    # kind R: the programs built by elanc-rs share one cargo target dir in
    # the bench's work directory (git-ignored), not the user's cache
    e["ELANC_RS_TARGET_DIR"] = str(WORK / "elanc-rs-target")
    e["ELANC_GO_CACHE"] = str(WORK / "elanc-go-cache")
    return e


# ---------------------------------------------------------------- discovery
COND = re.compile(r"^\s*if\s*\((.*)\)\s*then\s*$")


def parse_tst_all(path):
    """Yield (cmd, args, long) for each test line, tracking `if ($1 =~ *L*)`."""
    stack = []
    for raw in path.read_text(errors="replace").splitlines():
        line = raw.split("#", 1)[0].strip()
        if not line:
            continue
        m = COND.match(line)
        if m:
            stack.append("*L*" in m.group(1) and "=~ *L*" in m.group(1))
            continue
        if line.startswith("endif"):
            if stack:
                stack.pop()
            continue
        if line.startswith("else"):
            continue
        words = line.split()
        if words[0] in KIND_OF_CMD:
            yield words[0], words[1:], any(stack)


def parse_tests(path):
    """Yield (cmd, args, long) for each line of a TESTS file
    (tests/apps/README.md); an unknown command is an error (a typo must not
    drop a test)."""
    for n, raw in enumerate(path.read_text(errors="replace").splitlines(), 1):
        words = raw.split("#", 1)[0].split()
        if not words:
            continue
        long_ = words[0] == "long"
        if long_:
            words = words[1:]
        if not words or words[0] not in KIND_OF_CMD:
            raise SystemExit(f"{path}:{n}: unknown test command: {raw.strip()}")
        yield words[0], words[1:], long_


def discover(suites=None, apps=None):
    """Tests of the historical tst_all files of the suites (ids relative to
    legacy/) and of the TESTS files of the living copy (spec S9 §8; ids
    relative to the repository, t["copy"] True)."""
    suites = SUITES if suites is None else suites
    apps = APPS if apps is None else apps
    tests, seen = [], set()

    def add(d, lines, rel, copy):
        for cmd, a, long_ in lines:
            if cmd == "jtest":
                if len(a) < 3:
                    continue
                lgi, spc, inp, out, flags = a[0], a[1], None, a[2], a[3:]
            else:
                if len(a) < 4:
                    continue
                lgi, spc, inp, out, flags = a[0], a[1], a[2], a[3], a[4:]
            for kind in KIND_OF_CMD[cmd]:
                tid = f"{rel}::{kind}::{lgi}:{spc}:{inp or '-'}:{out}"
                if tid in seen:
                    continue
                seen.add(tid)
                tests.append(dict(id=tid, dir=d, kind=kind, lgi=lgi, spc=spc,
                                  inp=inp, out=out, flags=flags, long=long_, copy=copy))

    for suite in suites:
        for tst in sorted(suite.rglob("tst_all")):
            if tst.parent != suite:          # top-level driver (tst_one ...)
                add(tst.parent, parse_tst_all(tst), tst.parent.relative_to(ROOT), False)
    for top in apps:
        # exact name from the listing: rglob("TESTS") would also return a data
        # file `tests` on a case-insensitive volume
        for tf in sorted(Path(d) / "TESTS" for d, _, fs in os.walk(top) if "TESTS" in fs):
            add(tf.parent, parse_tests(tf), tf.parent.relative_to(REPO).as_posix(), True)
    return tests


def discover_regression(root=REGRESSION):
    """tests/regression/<case>/{prog.lgi,*.eln,input.inp,expected.out}: one I test per case."""
    if not root.is_dir():
        return []
    tests = []
    for d in sorted(p for p in root.iterdir() if p.is_dir()):
        if (d / "prog.lgi").exists():
            tests.append(dict(id=f"regression/{d.name}::I::prog:no:input:expected", dir=d,
                              kind="I", lgi="prog", spc="no", inp="input", out="expected",
                              flags=[], long=False,
                              fixes_2004=(d / "fixes-2004").exists()))
    return tests


FIXES_2004 = HERE / "fixes-2004.tsv"
FIXED_CASES = frozenset()   # loaded by main()


def load_fixes_2004(path=FIXES_2004):
    """fixes-2004.tsv: test-id <TAB> reason: bench cases whose output changed
    on purpose when a bug of the 2004 system was fixed."""
    if not Path(path).exists():
        return set()
    return {line.split("\t")[0] for line in Path(path).read_text().splitlines()
            if line.strip() and not line.startswith("#")}


def modern_only(t, prefix, fixes=frozenset()):
    """A case that tests the fix of a bug of the 2004 system (a regression
    case with a fixes-2004 file, or a bench case listed in fixes-2004.tsv) is
    not run against the reference (reference/install)."""
    return (bool(t.get("fixes_2004")) or t.get("id") in fixes) and \
        Path(prefix).resolve() == (REPO / "reference" / "install").resolve()


def discover_examples(root=EXAMPLES):
    """examples/<name>/{<name>.lgi,*.eln,[one *.spc,]input.inp,expected.out}: one I test
    each; the specification, if any, is passed as `elan -b <name>.lgi <spec>.spc`."""
    if not root.is_dir():
        return []
    tests = []
    for d in sorted(p for p in root.iterdir() if p.is_dir()):
        if not (d / f"{d.name}.lgi").exists():
            continue
        specs = sorted(p.stem for p in d.glob("*.spc"))
        if len(specs) > 1:
            raise SystemExit(f"{d}: more than one .spc file")
        spc = specs[0] if specs else "no"
        tests.append(dict(id=f"examples/{d.name}::I::{d.name}:{spc}:input:expected", dir=d,
                          kind="I", lgi=d.name, spc=spc, inp="input", out="expected",
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


# ---------------------------------------------------------------- execution
def run(cmd, cwd, stdin=None, timeout=60):
    with open(stdin, "rb") if stdin else open(os.devnull, "rb") as fin:
        try:
            p = subprocess.run(cmd, cwd=cwd, stdin=fin, stdout=subprocess.PIPE,
                               stderr=subprocess.PIPE, env=env(), timeout=timeout)
        except OSError as e:   # program (or its #! interpreter) missing: one failed test
            return 127, "", f"cannot run {cmd[0]}: {e}"
    return p.returncode, p.stdout.decode("latin-1"), p.stderr.decode("latin-1")


def samples(d):
    for name in ("SAMPLES", "Sample", "Samples"):
        if (d / name).is_dir():
            return d / name
    return d


SANITIZER_MARKERS = ("ERROR: AddressSanitizer", "ERROR: LeakSanitizer", "runtime error:")


def sanitizer_report(err):
    """First line of an ASan/UBSan report in a program's stderr, or None."""
    for line in err.splitlines():
        if any(m in line for m in SANITIZER_MARKERS):
            return line.strip()
    return None


class Unrunnable(Exception):
    """A test that produced no output to compare: status, detail, output so far."""
    def __init__(self, status, detail, out=""):
        super().__init__(status)
        self.status, self.detail, self.out = status, detail, out


def locate(t, name):
    """(file, path relative to the test's directory) of its SAMPLES file
    `name` (stored names of legacy/: CASE-RENAMES.tsv), or (None, None)."""
    p = samples(t["dir"]) / name
    if src_path(p).exists():
        return src_path(p), p.relative_to(t["dir"])
    return None, None


def has_lgi(t):
    return src_path(t["dir"] / f"{t['lgi']}.lgi").exists()


def private_copy(t, wd):
    """wd: a copy of the test's directory (legacy/: case names restored)."""
    shutil.copytree(t["dir"], wd, symlinks=True,
                    ignore=shutil.ignore_patterns("CVS", "*.o", "a.out", ".elan.*"))
    restore_case(t["dir"], wd, t["lgi"])


def produce(t, timeout):
    """(stdout, stderr, rc) of test t run in a private copy with PREFIX's tools.
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
        return out, err, rc
    finally:
        shutil.rmtree(work, ignore_errors=True)


def run_test(t, timeout):
    if modern_only(t, PREFIX, FIXED_CASES):
        return "MODERN-ONLY", "fixes a bug of the 2004 reference (fixes-2004)", ""
    expected, _ = locate(t, f"{t['out']}.out")
    if expected is None:
        return "NOREF", f"missing {t['out']}.out", ""
    try:
        out, err, rc = produce(t, timeout)
    except Unrunnable as u:
        return u.status, u.detail, u.out
    except subprocess.TimeoutExpired:
        return "TIMEOUT", f"> {timeout}s", ""
    k = t["kind"]
    exp = expected.read_text(encoding="latin-1", errors="replace")
    if k in COMPILER_KINDS:   # rewrite_step counts depend on the compilation scheme
        out, exp = REWRITE_STEP.sub("", out), REWRITE_STEP.sub("", exp)
    if out == exp:
        return "PASS", "", out
    if norm(out) == norm(exp):
        return "PASS~", "differs only in white space", out
    if atoms(out) == atoms(exp):
        return "PASS≈", "same symbols/values in same order (printing differs)", out
    diff = "".join(difflib.unified_diff(exp.splitlines(True), out.splitlines(True),
                                        "expected", "got", n=2))
    tail = f"\n--- stderr:\n{err[-1500:]}" if err.strip() else f"\n--- rc={rc}" if rc != 0 else ""
    return "FAIL", diff[:20000] + tail, out


def snapshot_id(t):
    """Kind R (elanc-rs) is compared with the snapshot of the J case."""
    if t["kind"] in COMPILER_KINDS:
        return t["id"].replace(f"::{t['kind']}::", "::J::")
    return t["id"]


def snapshot_text(t, text):
    """The snapshot as compared with the output of t (kind R: without the
    rewrite_step line, whose count depends on the compilation scheme)."""
    return REWRITE_STEP.sub("", text) if t["kind"] in COMPILER_KINDS else text


def compares_snapshot(t, status):
    """Kind R: only an output of a program that ran is compared (elanc-rs
    refuses the constructs of later stages: UNSUPPORTED); a MODERN-ONLY case
    did not run."""
    if status == "MODERN-ONLY":
        return False
    return t["kind"] not in COMPILER_KINDS or status in ("PASS", "PASS~", "PASS≈", "FAIL")


def prune_results(root, keep=20):
    """Keep only the `keep` most recent result directories (named by date)."""
    runs = sorted(p for p in Path(root).iterdir() if p.is_dir())
    for old in runs[:-keep] if keep > 0 else runs:
        shutil.rmtree(old, ignore_errors=True)


def safe(tid):
    # + hash of the exact id: macOS file names are case-insensitive and some
    # applications have both Robot.lgi-based and robot.lgi-based tests
    import hashlib
    h = hashlib.sha1(tid.encode()).hexdigest()[:8]
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", tid) + "-" + h


REWRITE_STEP = re.compile(r"^rewrite_step = \d+\n", re.M)
STATS = re.compile(r"^\s*(rewrite_step|total time|average speed|nb_|time)\b.*$", re.M)
ATOM = re.compile(r"[A-Za-z_][A-Za-z0-9_']*|-?\d+|\"[^\"]*\"")  # 5.7.nil is a list, not 5.7


def atoms(s):
    """Weak oracle: identifiers/numbers/strings in order, ignoring punctuation,
    list constructors and statistics lines. Old reference outputs were produced
    with older libraries/printers (a.b.nil, a,b,nil, cons_X(a,b), h(,)(a,b), [](n))."""
    s = STATS.sub("", s)
    return [a for a in ATOM.findall(s) if not re.fullmatch(r"cons(_\w+)?", a)]


def norm(s):
    return " ".join(s.split())


# ---------------------------------------------------------------- main
def load_baseline():
    if not BASELINE.exists():
        return {}
    res = {}
    for line in BASELINE.read_text().splitlines():
        if line and not line.startswith("#"):
            tid, st = line.split("\t")[:2]
            res[tid] = st
    return res


RANK = {"PASS": 0, "PASS~": 1, "PASS≈": 2}


def judge(statuses, base, exceptions, new_ids=frozenset()):
    """Compare test statuses with the baseline: return (regressions, improvements),
    lists of (id, old, new). Cases of tests/regression must PASS exactly (their
    expected.out is the oracle, whatever the baseline says); tests listed in the
    platform exceptions are exempt. Tests of the living copy (spec S9 §8) not in
    the baseline must PASS (kind G: or UNSUPPORTED): the reference is their oracle."""
    regress, improve = [], []
    for tid, new in sorted(statuses.items()):
        if new == "SANITIZER":          # a sanitizer report is always a failure
            regress.append((tid, base.get(tid, "PASS"), new))
            continue
        if tid in exceptions or new == "MODERN-ONLY":   # not run (fixes-2004)
            continue
        if tid.startswith(("regression/", "examples/")):
            if new not in ("PASS", "MODERN-ONLY"):
                regress.append((tid, "PASS", new))
            continue
        if tid in new_ids and tid not in base:
            ok = ("PASS", "UNSUPPORTED") if "::G::" in tid else ("PASS",)
            if new not in ok:
                regress.append((tid, "PASS", new))
            continue
        if tid in base:
            old = base[tid]
            if RANK.get(new, 9) > RANK.get(old, 9):
                regress.append((tid, old, new))
            elif RANK.get(new, 9) < RANK.get(old, 9):
                improve.append((tid, old, new))
    return regress, improve


def new_tests(tests, base):
    """Ids of the tests of the living copy added since the baseline: neither
    the test nor its J case (kinds R, G: the baseline has the J case of a
    historical jtest line) is in it."""
    return {t["id"] for t in tests
            if t.get("copy") and t["id"] not in base and snapshot_id(t) not in base}


def run_repeated(t, timeout, repeat):
    """Run a test `repeat` times; outputs that vary between runs => FLAKY."""
    st, detail, out = run_test(t, timeout)
    outs = {out}
    for _ in range(repeat - 1):
        outs.add(run_test(t, timeout)[2])
    if len(outs) > 1:
        return "FLAKY", f"{len(outs)} different outputs in {repeat} runs\n" + detail, out
    return st, detail, out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--kinds", default="I,A,J,JO")
    ap.add_argument("--filter", default="")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("--timeout", type=int, default=120)
    ap.add_argument("--long", dest="long", action="store_true", default=True)
    ap.add_argument("--no-long", dest="long", action="store_false")
    ap.add_argument("--save-baseline", action="store_true")
    ap.add_argument("--save-snapshots", action="store_true")
    ap.add_argument("--repeat", type=int, default=1,
                    help="run each test N times and report FLAKY when outputs differ")
    ap.add_argument("--prefix", help="installed ELAN to test (default: reference/install)")
    ap.add_argument("--elanlib", help="prefix whose share/elanlib replaces the installed library")
    ap.add_argument("--oldlib", action="store_true",
                    help="use the 2001-2002 library of legacy/elan/src (closer to the reference outputs)")
    a = ap.parse_args()

    global PREFIX
    if a.prefix:
        PREFIX = Path(a.prefix).resolve()
    if a.oldlib:
        tmp = Path(tempfile.mkdtemp(prefix="elan-oldlib-"))
        (tmp / "share").mkdir()
        shutil.copytree(OLDLIB_SRC, tmp / "share" / "elanlib", symlinks=True)
        restore_case(OLDLIB_SRC, tmp / "share" / "elanlib")   # strategy/Any.eln
        a.elanlib = str(tmp)
    if a.elanlib:
        os.environ["ELAN_TEST_LIB"] = str(Path(a.elanlib).resolve())
    kinds = set(a.kinds.split(","))
    tests = [t for t in discover() + discover_regression() + discover_examples()
             if t["kind"] in kinds and (a.long or not t["long"]) and re.search(a.filter, t["id"])]
    if a.list:
        for t in tests:
            print(t["id"], "(long)" if t["long"] else "")
        print(len(tests), "tests")
        return 0

    exceptions = load_exceptions()
    global FIXED_CASES
    FIXED_CASES = frozenset(load_fixes_2004())

    stamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
    outdir = HERE / "results" / stamp
    outdir.mkdir(parents=True)
    prune_results(HERE / "results", keep=20)
    t0 = time.time()
    results, snapdiff = {}, []
    with cf.ThreadPoolExecutor(a.j) as ex:
        futs = {ex.submit(run_repeated, t, a.timeout, a.repeat): t for t in tests}
        for i, f in enumerate(cf.as_completed(futs), 1):
            t = futs[f]
            st, detail, out = f.result()
            snap = SNAPDIR / (safe(snapshot_id(t)) + ".out")
            if a.save_snapshots and st in ("PASS", "PASS~", "PASS≈", "FAIL") and t["kind"] not in COMPILER_KINDS:
                SNAPDIR.mkdir(exist_ok=True)
                snap.write_text(out, encoding="latin-1")
            sst = ""
            if snap.exists() and not a.save_snapshots and compares_snapshot(t, st):
                snapped = snapshot_text(t, snap.read_text(encoding="latin-1"))
                sst = "SNAP-OK" if snapped == out else "SNAP-DIFF"
                if sst == "SNAP-DIFF" and t["id"] in exceptions:
                    sst = "SNAP-EXC"
                if sst == "SNAP-DIFF":
                    snapdiff.append(t["id"])
                    (outdir / (safe(t["id"]) + ".snapdiff.txt")).write_text("".join(
                        difflib.unified_diff(snapped.splitlines(True),
                                             out.splitlines(True), "snapshot", "got", n=2)))
            results[t["id"]] = (st, detail)
            print(f"[{i}/{len(tests)}] {st:7} {sst:9} {t['id']}", flush=True)
            if st not in ("PASS",):
                fn = safe(t["id"]) + ".txt"
                (outdir / fn).write_text(detail)

    ids = sorted(results)
    with open(outdir / "summary.tsv", "w") as f:
        for tid in ids:
            f.write(f"{tid}\t{results[tid][0]}\n")
    counts = {}
    for st, _ in results.values():
        counts[st] = counts.get(st, 0) + 1
    print(f"\n{len(results)} tests in {time.time()-t0:.0f}s: " +
          ", ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    print(f"details: {outdir}")

    base = load_baseline()
    new_ids = new_tests(tests, base)
    regress, improve = judge({tid: results[tid][0] for tid in ids}, base, exceptions,
                             new_ids)
    if base or regress or new_ids:
        print(f"vs baseline: {len(regress)} regressions, {len(improve)} improvements")
        for tid, old, new in regress:
            print(f"  REGRESSION {tid}: {old} -> {new}")
    if SNAPDIR.exists() and not a.save_snapshots:
        print(f"vs snapshots: {len(snapdiff)} differences")
        for tid in snapdiff:
            print(f"  SNAP-DIFF {tid}")
    if a.save_baseline:
        with open(BASELINE, "w") as f:
            f.write("# test-id\tstatus (written by run_tests.py --save-baseline)\n")
            for tid in ids:
                f.write(f"{tid}\t{results[tid][0]}\n")
        print(f"baseline saved: {BASELINE}")
    return 1 if (regress or snapdiff) else 0


if __name__ == "__main__":
    sys.exit(main())
