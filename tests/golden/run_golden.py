#!/usr/bin/env python3
"""Golden tests of the ELAN interpreter: what the bench does not see.

The bench (tests/legacy-bench) compares the standard output of programs only.
These tests pin, byte for byte, outputs that it ignores:

  ref  the .ref files written by `elan --export` and `elan --cexport` (their
       numbers are stringtab hash positions and symbol codes: the REF
       contract, docs/ref-format.md);
  run  the standard output, the standard error and the exit status of one
       `elan` run: the -d dump, the -s/-S statistics, deliberate errors.

The cases are listed in cases.tsv (tab-separated, `#` comments):

  ref <TAB> id <TAB> dir <TAB> lgi <TAB> spc|-
      elan -b --export LGI.ref LGI.lgi [SPC.spc]  -> expected/<id>.ref
      elan -b --cexport LGI.ref LGI.lgi [SPC.spc] -> expected/<id>.cref
  run <TAB> id <TAB> dir <TAB> stdin|- <TAB> args
      elan ARGS < STDIN                           -> expected/<id>.txt

`dir` is relative to the repository; each case runs in a private copy of it
(under work/, which must be on a case-sensitive file system). When the copy
contains gen.py, `python3 -I gen.py` runs first in it (generated programs:
table overflows, deep includes...). Text outputs are normalised: the work
directory, the install prefix and the repository become <WORK>, <PREFIX> and
<REPO>; the times and speeds of the statistics become <T> and <N>.

Usage:
  run_golden.py [--prefix PREFIX] [--filter REGEX] [-j N] [--update]
--update rewrites the goldens from the current build (then review the diff
and record the generating commit in README.md).
"""
import argparse, concurrent.futures as cf, difflib, os, re, shutil, subprocess, sys, tempfile
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
CASES = HERE / "cases.tsv"
EXPECTED = HERE / "expected"
WORK = HERE / "work"
TIMEOUT = 300


class CaseError(Exception):
    pass


# ---------------------------------------------------------------- manifest
def parse_manifest(text):
    """Cases of a cases.tsv text: a list of dicts (kind, id, dir, ...)."""
    cases, seen = [], set()
    for n, raw in enumerate(text.splitlines(), 1):
        if not raw.strip() or raw.lstrip().startswith("#"):
            continue
        f = raw.split("\t")
        kind = f[0]
        if kind == "ref" and len(f) == 5:
            c = dict(kind="ref", id=f[1], dir=f[2], lgi=f[3], spc=None if f[4] == "-" else f[4])
        elif kind == "run" and len(f) == 5:
            c = dict(kind="run", id=f[1], dir=f[2], stdin=None if f[3] == "-" else f[3],
                     args=f[4].split())
        else:
            raise CaseError(f"cases.tsv:{n}: malformed line: {raw!r}")
        if not re.fullmatch(r"[A-Za-z0-9_.+-]+", c["id"]):
            raise CaseError(f"cases.tsv:{n}: bad id {c['id']!r}")
        if c["id"] in seen:
            raise CaseError(f"cases.tsv:{n}: duplicate id {c['id']!r}")
        seen.add(c["id"])
        cases.append(c)
    return cases


# ---------------------------------------------------------------- normalisation
TIME = re.compile(r"\(\d+\.\d+\+\d+\.\d+\)=\d+\.\d+ sec")
SPEED = re.compile(r"average speed = \S+ inf/sec")


def normalise(text, work, prefix):
    """Remove what varies between runs: paths, times, speeds."""
    subst = {}
    for path, name in ((REPO, "<REPO>"), (prefix, "<PREFIX>"), (work, "<WORK>")):
        for p in (str(path), os.path.realpath(path)):
            subst[p] = name
    for p in sorted(subst, key=len, reverse=True):   # longest first: /private/var/x before /var/x
        text = text.replace(p, subst[p])
    text = TIME.sub("(<T>+<T>)=<T> sec", text)
    return SPEED.sub("average speed = <N> inf/sec", text)


def render_run(args, out, err, rc):
    """The golden text of a `run` case."""
    def block(s):
        return s if s == "" or s.endswith("\n") else s + "\n"
    return (f"$ elan {' '.join(args)}\n--- stdout\n{block(out)}"
            f"--- stderr\n{block(err)}--- exit status {rc}\n")


# ---------------------------------------------------------------- execution
def env(prefix):
    e = dict(os.environ)
    e["ELANLIB"] = str(prefix)
    e["PATH"] = f"{prefix}/bin:" + e.get("PATH", "")
    e.pop("SECONDELANLIB", None)
    e["LC_ALL"] = "C"
    return e


def elan(prefix, args, cwd, stdin=None):
    with open(stdin, "rb") if stdin else open(os.devnull, "rb") as fin:
        p = subprocess.run([str(Path(prefix) / "bin" / "elan"), *args], cwd=cwd, stdin=fin,
                           stdout=subprocess.PIPE, stderr=subprocess.PIPE, env=env(prefix),
                           timeout=TIMEOUT)
    return p.returncode, p.stdout.decode("latin-1"), p.stderr.decode("latin-1")


def produce(case, prefix, work_root=WORK):
    """Run a case; return {golden file name: bytes}."""
    src = REPO / case["dir"]
    if not src.is_dir():
        raise CaseError(f"missing directory {case['dir']}")
    work_root.mkdir(parents=True, exist_ok=True)
    tmp = Path(tempfile.mkdtemp(prefix="golden-", dir=work_root))
    try:
        wd = tmp / src.name
        shutil.copytree(src, wd, symlinks=True,
                        ignore=shutil.ignore_patterns("CVS", "*.o", "a.out", ".elan.*", "*.ref"))
        if (wd / "gen.py").exists():
            subprocess.run([sys.executable, "-I", "gen.py"], cwd=wd, check=True)
        if case["kind"] == "ref":
            res = {}
            spc = [f"{case['spc']}.spc"] if case["spc"] else []
            for opt, ext in (("--export", "ref"), ("--cexport", "cref")):
                ref = wd / f"{case['lgi']}.ref"
                if ref.exists():
                    ref.unlink()
                rc, out, err = elan(prefix, ["-b", opt, ref.name, f"{case['lgi']}.lgi", *spc], wd)
                if rc != 0 or not ref.exists():
                    raise CaseError(f"elan {opt} failed (rc={rc})\n{out[-1500:]}{err[-1500:]}")
                res[f"{case['id']}.{ext}"] = ref.read_bytes()
            return res
        stdin = wd / case["stdin"] if case["stdin"] else None
        rc, out, err = elan(prefix, case["args"], wd, stdin)
        text = render_run(case["args"], normalise(out, wd, prefix), normalise(err, wd, prefix), rc)
        return {f"{case['id']}.txt": text.encode("latin-1")}
    finally:
        shutil.rmtree(tmp, ignore_errors=True)


def compare(produced, expected_dir=EXPECTED):
    """List of (file name, problem) for the produced files that differ from the goldens."""
    problems = []
    for name, data in sorted(produced.items()):
        g = expected_dir / name
        if not g.exists():
            problems.append((name, "no golden (run with --update)"))
        elif g.read_bytes() != data:
            old = g.read_bytes().decode("latin-1").splitlines(True)
            new = data.decode("latin-1").splitlines(True)
            diff = "".join(difflib.unified_diff(old, new, "golden", "got", n=2))
            problems.append((name, diff[:6000]))
    return problems


def case_sensitive(directory):
    directory.mkdir(parents=True, exist_ok=True)
    probe = Path(tempfile.mkdtemp(prefix="case-", dir=directory))
    try:
        (probe / "a").touch()
        (probe / "A").touch()
        return len(list(probe.iterdir())) == 2
    finally:
        shutil.rmtree(probe, ignore_errors=True)


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--prefix", default=str(REPO / "build" / "install"))
    ap.add_argument("--filter", default="")
    ap.add_argument("-j", type=int, default=os.cpu_count())
    ap.add_argument("--update", action="store_true")
    a = ap.parse_args(argv)
    prefix = Path(a.prefix).resolve()
    if not case_sensitive(WORK):
        print(f"ERROR: {WORK} is on a case-insensitive file system")
        return 2
    cases = [c for c in parse_manifest(CASES.read_text()) if re.search(a.filter, c["id"])]
    failed, names = 0, set()
    with cf.ThreadPoolExecutor(a.j) as ex:
        futs = {ex.submit(produce, c, prefix): c for c in cases}
        for f in cf.as_completed(futs):
            c = futs[f]
            try:
                produced = f.result()
            except (CaseError, subprocess.SubprocessError, OSError) as e:
                print(f"ERROR   {c['id']}: {e}")
                failed += 1
                continue
            names.update(produced)
            if a.update:
                EXPECTED.mkdir(exist_ok=True)
                for name, data in produced.items():
                    (EXPECTED / name).write_bytes(data)
                print(f"updated {c['id']}")
                continue
            problems = compare(produced)
            if problems:
                failed += 1
                for name, what in problems:
                    print(f"DIFF    {name}\n{what}")
            else:
                print(f"ok      {c['id']}")
    if not a.filter:
        for g in sorted(EXPECTED.glob("*")):
            if g.name not in names and g.name != "README.md":
                print(f"STALE   {g.name}: no case produces it (remove it)")
                failed += 1
    print(f"golden: {len(cases)} cases, {failed} failed")
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
