#!/usr/bin/env python3
"""The bench compiled by elanc-rs (kind R) against elanc (kind J).

Runs tests/legacy-bench/run_tests.py --kinds R (which compares every R
output with the J snapshot of the case, rewrite_step line excluded) and
fails when:
- the runner fails (an R output differs from the J snapshot);
- an R case has a worse status than the same J case in the baseline
  (PASS < PASS~ < PASS≈ < FAIL < ERROR);
- a case recorded in bench_r_expected.tsv (the R cases elanc-rs compiles,
  with their status) has a worse status, e.g. becomes UNSUPPORTED.
UNSUPPORTED cases not recorded (refused until S6b-S6d) are counted only.
Usage: bench_parity.py PREFIX [--save]   (--save rewrites bench_r_expected.tsv)
"""
import re, subprocess, sys
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
BENCH = REPO / "tests" / "legacy-bench"
EXPECTED = Path(__file__).resolve().parent / "bench_r_expected.tsv"
RANK = {"PASS": 0, "PASS~": 1, "PASS≈": 2, "FAIL": 3, "ERROR": 4, "TIMEOUT": 5,
        "SANITIZER": 6}

def main():
    prefix = sys.argv[1]
    save = "--save" in sys.argv[2:]
    base = {}
    for line in (BENCH / "baseline.tsv").read_text().splitlines():
        if line and not line.startswith("#"):
            tid, st = line.split("\t")[:2]
            base[tid] = st
    expected = {}
    if EXPECTED.exists():
        for line in EXPECTED.read_text().splitlines():
            if line and not line.startswith("#"):
                tid, st = line.split("\t")[:2]
                expected[tid] = st
    p = subprocess.run([sys.executable, str(BENCH / "run_tests.py"), "--prefix", prefix,
                        "--kinds", "R"], capture_output=True, text=True)
    counts, problems, got = Counter(), [], {}
    for line in p.stdout.splitlines():
        m = re.match(r"\[\d+/\d+\]\s+(\S+)\s+(SNAP-\S+\s+)?(\S+::R::\S+)", line)
        if not m:
            continue
        st, snap, tid = m.group(1), (m.group(2) or "").strip(), m.group(3)
        counts[st] += 1
        got[tid] = st
        if snap == "SNAP-DIFF":
            problems.append(f"{tid}: output differs from the elanc (J) snapshot")
        j = base.get(tid.replace("::R::", "::J::"))
        if st in RANK and j in RANK and RANK[st] > RANK[j]:
            problems.append(f"{tid}: R {st}, J {j}")
    for tid, st in expected.items():
        now = got.get(tid, "MISSING")
        if RANK.get(now, 99) > RANK.get(st, 99):
            problems.append(f"{tid}: was {st}, now {now}")
    print("bench R: " + ", ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    if save:
        rows = sorted((t, s) for t, s in got.items() if s in RANK)
        EXPECTED.write_text("# R cases compiled by elanc-rs and their status "
                            "(written by bench_parity.py --save)\n"
                            + "".join(f"{t}\t{s}\n" for t, s in rows))
        print(f"saved {len(rows)} cases in {EXPECTED.name}")
    for prob in problems:
        print("PARITY: " + prob)
    if p.returncode != 0 and not problems:
        problems.append(f"run_tests.py exit status {p.returncode}")
        print(p.stdout[-3000:])
    return 1 if problems or not counts else 0


if __name__ == "__main__":
    sys.exit(main())
