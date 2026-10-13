#!/usr/bin/env python3
"""Write the missing expected outputs of the tests of the living copy (spec S9
§8) with the 2004 reference system: for an itest/aitest line, the reference's interpreter
(`elan -b LGI [SPC] < INP`); for a gtest line, the program compiled by the
reference's REM (`elanc-rem -b -nosplit -quiet LGI [SPC]; make; ./a.out
-quiet < INP`). The output goes to <samples>/<OUT>.out (SAMPLES/ or Sample/ of the test
dir if it has one, else the dir itself); an
existing output is kept unless --force.
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
    of the copy's tests (write_expected skips the existing ones): kind I for itest/aitest
    lines, J for gtest lines."""
    jobs, kinds = [], {}
    for t in tests:
        if not t.get("copy"):
            continue
        kind = "J" if t["kind"] == "G" else "I"
        key = (t["dir"], t["out"])
        if key in kinds:
            if kinds[key] != kind:
                raise SystemExit(f"{t['id']}: output {t['out']} is used by interpreter and "
                                 "compiled lines: give the gtest line its own output name")
            continue
        kinds[key] = kind
        jobs.append((dict(t, kind=kind), rt.samples(t["dir"]) / f"{t['out']}.out"))
    return jobs


def write_expected(jobs, force, timeout):
    """Run each job on rt.PREFIX; return (written, kept, failed)."""
    written = kept = failed = 0
    for t, target in jobs:
        if target.exists() and not force:
            kept += 1
            continue
        try:
            out, err, rc = rt.produce(t, timeout)
        except rt.Unrunnable as u:
            print(f"FAILED {t['id']}: {u.status}\n{u.detail[-800:]}")
            failed += 1
            continue
        except subprocess.TimeoutExpired:
            print(f"FAILED {t['id']}: TIMEOUT > {timeout}s")
            failed += 1
            continue
        if rc != 0:   # not an expected output: the reference did not run the program to the end
            print(f"FAILED {t['id']}: rc={rc}\n{err[-800:]}")
            failed += 1
            continue
        target.parent.mkdir(exist_ok=True)
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
