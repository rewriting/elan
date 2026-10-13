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
import re
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
        out, err, rc = rt.produce(t, timeout)
    except rt.Unrunnable as u:
        return u.status
    except subprocess.TimeoutExpired:
        return "TIMEOUT"
    finally:
        rt.PREFIX = old
    lines = [l.strip() for l in err.splitlines() if l.strip()]
    for i, line in enumerate(lines):
        if "[error]" in line or "fatal" in line:
            if re.search(r"\[error\]\s*:file=\S+\s+line=\d+", line) and i + 1 < len(lines):
                line += " " + lines[i + 1]   # a bare location: add its message
            return " ".join(line.split())[:160]
    return "ok" if rc == 0 else f"rc={rc}"


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
    modern_only = sum(1 for r in rows if r[1] != "ok" and r[2] == "ok")
    neither = sum(1 for r in rows if r[1] != "ok" and r[2] != "ok")
    print(f"triage: {len(rows)} programs: {both} load with both, {ref_only} with the reference "
          f"only, {modern_only} with the modern only, {neither} with neither; "
          f"report in {REPORT.name}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
