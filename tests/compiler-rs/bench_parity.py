#!/usr/bin/env python3
"""The bench compiled by elanc-rs (kind R) against elanc (kind J).

Runs tests/legacy-bench/run_tests.py --kinds R and checks that no R case
that elanc-rs compiles has a worse status than the same J case in the
baseline (PASS < PASS~ < PASS≈ < FAIL < ERROR). UNSUPPORTED cases (refused
by elanc-rs until S6b-S6d) are counted, not failed.
Usage: bench_parity.py PREFIX
"""
import re, subprocess, sys
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
BENCH = REPO / "tests" / "legacy-bench"
RANK = {"PASS": 0, "PASS~": 1, "PASS≈": 2, "FAIL": 3, "ERROR": 4, "TIMEOUT": 5,
        "SANITIZER": 6}

def main():
    prefix = sys.argv[1]
    base = {}
    for line in (BENCH / "baseline.tsv").read_text().splitlines():
        if line and not line.startswith("#"):
            tid, st = line.split("\t")[:2]
            base[tid] = st
    p = subprocess.run([sys.executable, str(BENCH / "run_tests.py"), "--prefix", prefix,
                        "--kinds", "R"], capture_output=True, text=True)
    counts, worse = Counter(), []
    for line in p.stdout.splitlines():
        m = re.match(r"\[\d+/\d+\]\s+(\S+)\s.*?(\S+::R::\S+)", line)
        if not m:
            continue
        st, tid = m.groups()
        counts[st] += 1
        j = base.get(tid.replace("::R::", "::J::"))
        if st in RANK and j in RANK and RANK[st] > RANK[j]:
            worse.append(f"{tid}: R {st}, J {j}")
    print("bench R: " + ", ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    for w in worse:
        print("WORSE THAN elanc: " + w)
    return 1 if worse or not counts else 0

if __name__ == "__main__":
    sys.exit(main())
