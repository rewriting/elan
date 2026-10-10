#!/usr/bin/env python3
"""Prints the programs of the J cases of the legacy bench, one per line:
DIRECTORY<TAB>LGI<TAB>SPC (SPC is "no" without specification). The J and JO
cases export the same program (-optimiseChoicePoint is a REM option), so the
J cases are enough. Used by test_ref_roundtrip.sh."""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent.parent / "legacy-bench"))
import run_tests  # noqa: E402

seen = set()
for t in run_tests.discover() + run_tests.discover_regression() + run_tests.discover_examples():
    if t["kind"] != "J":
        continue
    if [f for f in t["flags"] if f]:
        sys.exit(f"{t['id']}: elan flags are not supported by test_ref_roundtrip.sh")
    key = (str(t["dir"]), t["lgi"], t["spc"])
    if key not in seen and (t["dir"] / f"{t['lgi']}.lgi").exists():
        seen.add(key)
        print("\t".join(key))
