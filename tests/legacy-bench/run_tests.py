#!/usr/bin/env python3
"""ELAN regression test bench.

Replays the historical test bench of the ELAN project (the `tst_all` files that
drive the csh scripts `itest`, `aitest` and `jtest` from elan/sources/Scripts)
against an installed ELAN system: by default the reference build
(reference/install, see reference/build.sh); use --prefix to test another one
(e.g. the modern port).

Test kinds (same semantics as the original scripts):
  I  itest  : elan -b LGI.lgi [SPC.spc] < SAMPLES/INP.inp       == SAMPLES/OUT.out
  A  aitest : elan -b --export LGI.ref LGI.lgi [SPC.spc];
              elan -b --import LGI.ref < SAMPLES/INP.inp         == SAMPLES/OUT.out
  J  jtest  : elanc -b -nosplit -quiet LGI [SPC]; make; a.out -noInput -quiet == SAMPLES/OUT.out
              (-noInput = evaluate the compiled `start with` query; it was the default
              when the reference outputs were produced)
  JO jtest  : same with -optimiseChoicePoint (second half of the original jtest)

ctest/actest/cctest (old C compiler) and pitest/pevaltest (partial evaluator)
have no script anymore and are not replayed.

Each test runs in a private copy of its application directory.

Two oracles:
  * historical: SAMPLES/OUT.out (some predate later library/printer changes)
  * snapshot  : tests/snapshots/<id>.out, the outputs of a trusted build
                (--save-snapshots); used to check that code changes preserve
                behaviour exactly. Status "SNAP-OK"/"SNAP-DIFF" is reported separately.

Usage:
  run_tests.py [--list] [--kinds I,A,J,JO] [--filter REGEX] [-j N] [--timeout S]
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
EXCEPTIONS = HERE / "platform-exceptions.tsv"

# Directories whose tst_all files are harvested
SUITES = [ELAN3 / "applications", ELAN3 / "contributions", ROOT / "elan" / "sources"]

KIND_OF_CMD = {"itest": ["I"], "aitest": ["A"], "jtest": ["J", "JO"]}


def env():
    e = dict(os.environ)
    java = os.environ.get("JAVA_HOME", "/opt/homebrew/opt/openjdk")
    e["ELANLIB"] = os.environ.get("ELAN_TEST_LIB", str(PREFIX))
    e["PATH"] = f"{PREFIX}/bin:{java}/bin:" + e.get("PATH", "")
    e.pop("SECONDELANLIB", None)
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


def discover():
    tests, seen = [], set()
    for suite in SUITES:
        for tst in sorted(suite.rglob("tst_all")):
            d = tst.parent
            if d == suite:          # top-level driver (tst_one ...)
                continue
            for cmd, a, long_ in parse_tst_all(tst):
                if cmd == "jtest":
                    if len(a) < 3:
                        continue
                    lgi, spc, inp, out, flags = a[0], a[1], None, a[2], a[3:]
                else:
                    if len(a) < 4:
                        continue
                    lgi, spc, inp, out, flags = a[0], a[1], a[2], a[3], a[4:]
                for kind in KIND_OF_CMD[cmd]:
                    rel = d.relative_to(ROOT)
                    tid = f"{rel}::{kind}::{lgi}:{spc}:{inp or '-'}:{out}"
                    if tid in seen:
                        continue
                    seen.add(tid)
                    tests.append(dict(id=tid, dir=d, kind=kind, lgi=lgi, spc=spc,
                                      inp=inp, out=out, flags=flags, long=long_))
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


# ---------------------------------------------------------------- execution
def run(cmd, cwd, stdin=None, timeout=60):
    with open(stdin, "rb") if stdin else open(os.devnull, "rb") as fin:
        p = subprocess.run(cmd, cwd=cwd, stdin=fin, stdout=subprocess.PIPE,
                           stderr=subprocess.PIPE, env=env(), timeout=timeout)
    return p.returncode, p.stdout.decode("latin-1"), p.stderr.decode("latin-1")


def samples(d):
    for name in ("SAMPLES", "Sample", "Samples"):
        if (d / name).is_dir():
            return d / name
    return d


def run_test(t, timeout):
    expected = samples(t["dir"]) / f"{t['out']}.out"
    if not expected.exists():
        return "NOREF", f"missing {expected}", ""
    if not (t["dir"] / f"{t['lgi']}.lgi").exists():
        return "NOLGI", f"missing {t['lgi']}.lgi", ""
    spc = [] if t["spc"] == "no" else [f"{t['spc']}.spc"]
    flags = [f for f in t["flags"] if f]
    WORK.mkdir(exist_ok=True)
    work = Path(tempfile.mkdtemp(prefix="elantest-", dir=WORK))  # case-sensitive volume
    try:
        wd = work / t["dir"].name
        shutil.copytree(t["dir"], wd, symlinks=True,
                        ignore=shutil.ignore_patterns("CVS", "*.o", "a.out", ".elan.*"))
        inp = samples(wd) / f"{t['inp']}.inp" if t["inp"] else None
        if inp is not None and not inp.exists():
            return "NOINP", f"missing {inp.name}", ""
        k = t["kind"]
        if k == "I":
            rc, out, err = run(["elan", "-b", *flags, f"{t['lgi']}.lgi", *spc], wd, inp, timeout)
        elif k == "A":
            rc, o1, e1 = run(["elan", "-b", *flags, "--export", f"{t['lgi']}.ref",
                              f"{t['lgi']}.lgi", *spc], wd, None, timeout)
            if rc != 0:
                return "ERROR", f"export rc={rc}\n{e1[-2000:]}", o1
            rc, out, err = run(["elan", "-b", "--import", f"{t['lgi']}.ref"], wd, inp, timeout)
        else:  # J, JO
            opts = ["-b", "-nosplit", "-quiet"] + (["-optimiseChoicePoint"] if k == "JO" else [])
            rc, o1, e1 = run(["elanc", *opts, *flags, t["lgi"], *spc], wd, None, timeout)
            mk = wd / f"{t['lgi']}.make"
            if not mk.exists():
                return "ERROR", f"elanc rc={rc}\n{o1[-1500:]}\n{e1[-1500:]}", ""
            rc, o2, e2 = run(["make", "-f", mk.name], wd, None, timeout * 3)
            if rc != 0 or not (wd / "a.out").exists():
                return "ERROR", f"make rc={rc}\n{(o2 + e2)[-3000:]}", ""
            rc, out, err = run(["./a.out", "-noInput", "-quiet"], wd, None, timeout)
        exp = expected.read_text(encoding="latin-1", errors="replace")
        if out == exp:
            return "PASS", "", out
        if norm(out) == norm(exp):
            return "PASS~", "differs only in white space", out
        if atoms(out) == atoms(exp):
            return "PASS≈", "same symbols/values in same order (printing differs)", out
        diff = "".join(difflib.unified_diff(exp.splitlines(True), out.splitlines(True),
                                            "expected", "got", n=2))
        tail = f"\n--- rc={rc} stderr:\n{err[-1500:]}" if (rc or err.strip()) else ""
        return "FAIL", diff[:20000] + tail, out
    except subprocess.TimeoutExpired:
        return "TIMEOUT", f"> {timeout}s", ""
    finally:
        shutil.rmtree(work, ignore_errors=True)


def safe(tid):
    # + hash of the exact id: macOS file names are case-insensitive and some
    # applications have both Robot.lgi-based and robot.lgi-based tests
    import hashlib
    h = hashlib.sha1(tid.encode()).hexdigest()[:8]
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", tid) + "-" + h


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


def judge(statuses, base, exceptions):
    """Compare test statuses with the baseline: return (regressions, improvements),
    lists of (id, old, new). Cases of tests/regression must PASS exactly (their
    expected.out is the oracle, whatever the baseline says); tests listed in the
    platform exceptions are exempt."""
    regress, improve = [], []
    for tid, new in sorted(statuses.items()):
        if tid in exceptions:
            continue
        if tid.startswith("regression/"):
            if new != "PASS":
                regress.append((tid, "PASS", new))
            continue
        if tid in base:
            old = base[tid]
            if RANK.get(new, 9) > RANK.get(old, 9):
                regress.append((tid, old, new))
            elif RANK.get(new, 9) < RANK.get(old, 9):
                improve.append((tid, old, new))
    return regress, improve


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
        (tmp / "share" / "elanlib").symlink_to(OLDLIB_SRC)
        a.elanlib = str(tmp)
    if a.elanlib:
        os.environ["ELAN_TEST_LIB"] = str(Path(a.elanlib).resolve())
    kinds = set(a.kinds.split(","))
    tests = [t for t in discover() + discover_regression()
             if t["kind"] in kinds and (a.long or not t["long"]) and re.search(a.filter, t["id"])]
    if a.list:
        for t in tests:
            print(t["id"], "(long)" if t["long"] else "")
        print(len(tests), "tests")
        return 0

    if not case_sensitive(WORK):
        print(f"ERROR: {WORK} is on a case-insensitive file system "
              "(ELAN sources contain files differing only by case; see README)")
        return 2
    exceptions = load_exceptions()

    stamp = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
    outdir = HERE / "results" / stamp
    outdir.mkdir(parents=True)
    t0 = time.time()
    results, snapdiff = {}, []
    with cf.ThreadPoolExecutor(a.j) as ex:
        futs = {ex.submit(run_repeated, t, a.timeout, a.repeat): t for t in tests}
        for i, f in enumerate(cf.as_completed(futs), 1):
            t = futs[f]
            st, detail, out = f.result()
            snap = SNAPDIR / (safe(t["id"]) + ".out")
            if a.save_snapshots and st in ("PASS", "PASS~", "PASS≈", "FAIL"):
                SNAPDIR.mkdir(exist_ok=True)
                snap.write_text(out, encoding="latin-1")
            sst = ""
            if snap.exists() and not a.save_snapshots:
                sst = "SNAP-OK" if snap.read_text(encoding="latin-1") == out else "SNAP-DIFF"
                if sst == "SNAP-DIFF" and t["id"] in exceptions:
                    sst = "SNAP-EXC"
                if sst == "SNAP-DIFF":
                    snapdiff.append(t["id"])
                    (outdir / (safe(t["id"]) + ".snapdiff.txt")).write_text("".join(
                        difflib.unified_diff(snap.read_text(encoding="latin-1").splitlines(True),
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
    regress, improve = judge({tid: results[tid][0] for tid in ids}, base, exceptions)
    if base or regress:
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
