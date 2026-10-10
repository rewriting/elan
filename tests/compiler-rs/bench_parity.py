#!/usr/bin/env python3
"""The bench compiled by a new compiler against elanc (kind J): elanc-rs
(kind R) or elanc-go (kind G).

Runs tests/legacy-bench/run_tests.py --kinds R or G (which compares every
output with the J snapshot of the case, rewrite_step line excluded) and
fails when:
- the runner fails (an output differs from the J snapshot);
- a case has a worse status than the same J case in the baseline
  (PASS < PASS~ < PASS≈ < FAIL < ERROR);
- a case recorded in the expected file of the kind (the cases the
  compiler compiles, with their status) has a worse status, e.g. becomes
  UNSUPPORTED.
UNSUPPORTED cases not recorded (refused until S6b-S6d for elanc-rs,
S7b-S7d for elanc-go) are counted only.
Usage: bench_parity.py PREFIX [--kind R|G] [--save]
       --kind R (default): elanc-rs, expected cases in bench_r_expected.tsv;
       --kind G: elanc-go, expected cases in bench_g_expected.tsv;
       --save rewrites the expected file of the kind (bench_r_expected.tsv
       or bench_g_expected.tsv) with the cases of this run.
"""
import re, subprocess, sys
from collections import Counter
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
BENCH = REPO / "tests" / "legacy-bench"
HERE = Path(__file__).resolve().parent
RANK = {"PASS": 0, "PASS~": 1, "PASS≈": 2, "FAIL": 3, "ERROR": 4, "TIMEOUT": 5,
        "SANITIZER": 6}

COMPILER = {"R": "elanc-rs", "G": "elanc-go"}
USAGE = "usage: bench_parity.py PREFIX [--kind R|G] [--save]"

def parse(argv):
    """The command line: (prefix, kind, save); exits with a message (status
    2) when it is wrong."""
    prefix, kind, save = None, "R", False
    i = 0
    while i < len(argv):
        a = argv[i]
        if a == "--save":
            save = True
        elif a == "--kind":
            if i + 1 >= len(argv) or argv[i + 1].startswith("-"):
                print(f"bench_parity.py: --kind needs a value (R or G)\n{USAGE}", file=sys.stderr)
                sys.exit(2)
            i += 1
            kind = argv[i]
            if kind not in COMPILER:
                print(f"bench_parity.py: unknown kind {kind} (R or G)\n{USAGE}", file=sys.stderr)
                sys.exit(2)
        elif a.startswith("-") or prefix is not None:
            print(f"bench_parity.py: unexpected argument {a}\n{USAGE}", file=sys.stderr)
            sys.exit(2)
        else:
            prefix = a
        i += 1
    if prefix is None:
        print(USAGE, file=sys.stderr)
        sys.exit(2)
    return prefix, kind, save

def main():
    prefix, kind, save = parse(sys.argv[1:])
    EXPECTED = HERE / f"bench_{kind.lower()}_expected.tsv"
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
                        "--kinds", kind], capture_output=True, text=True)
    counts, problems, got = Counter(), [], {}
    for line in p.stdout.splitlines():
        m = re.match(r"\[\d+/\d+\]\s+(\S+)\s+(SNAP-\S+\s+)?(\S+::" + kind + r"::\S+)", line)
        if not m:
            continue
        st, snap, tid = m.group(1), (m.group(2) or "").strip(), m.group(3)
        counts[st] += 1
        got[tid] = st
        if snap == "SNAP-DIFF":
            problems.append(f"{tid}: output differs from the elanc (J) snapshot")
        j = base.get(tid.replace(f"::{kind}::", "::J::"))
        if st in RANK and j in RANK and RANK[st] > RANK[j]:
            problems.append(f"{tid}: {kind} {st}, J {j}")
    for tid, st in expected.items():
        now = got.get(tid, "MISSING")
        if RANK.get(now, 99) > RANK.get(st, 99):
            problems.append(f"{tid}: was {st}, now {now}")
    print(f"bench {kind}: " + ", ".join(f"{k}={v}" for k, v in sorted(counts.items())))
    if save:
        rows = sorted((t, s) for t, s in got.items() if s in RANK)
        EXPECTED.write_text(f"# {kind} cases compiled by {COMPILER[kind]} and their status "
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
