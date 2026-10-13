#!/usr/bin/env python3
"""Rename the bench ids of the ELAN 3 applications once, when the bench
switched from legacy/elan3 to the living copy (spec S9 §8):
elan3/applications/X::K::... -> applications/X::K::..., Robot's capitalised
variants renamed as in import_legacy.RENAMES. Rewrites baseline.tsv,
fixes-2004.tsv, platform-exceptions.tsv, bench_g_expected.tsv,
bench_r_expected.tsv, and renames the snapshot files.
Usage: migrate_ids.py"""
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO = HERE.parent.parent
sys.path.insert(0, str(REPO / "tests" / "legacy-bench"))
import run_tests as rt  # noqa: E402
from import_legacy import RENAMES  # noqa: E402

FILES = [rt.BASELINE, rt.FIXES_2004, rt.EXCEPTIONS,
         REPO / "tests" / "compiler-rs" / "bench_g_expected.tsv",
         REPO / "tests" / "compiler-rs" / "bench_r_expected.tsv"]


def new_id(old):
    if not old.startswith(("elan3/applications/", "elan3/contributions/")):
        return old
    path, kind, fields = old[len("elan3/"):].split("::")
    lgi, spc, inp, out = fields.split(":")
    if path == "applications/Robot":
        lgi, inp, out = (RENAMES.get(w, w) for w in (lgi, inp, out))
    return f"{path}::{kind}::{lgi}:{spc}:{inp}:{out}"


def migrate_file(path):
    lines = path.read_text().splitlines(True)
    out = []
    for line in lines:
        if line.startswith("#") or not line.strip():
            out.append(line)
            continue
        tid, sep, rest = line.partition("\t")
        out.append(new_id(tid) + sep + rest)
    path.write_text("".join(out))


def migrate_snapshots(old_ids):
    moved = 0
    for old in old_ids:
        new = new_id(old)
        src = rt.SNAPDIR / (rt.safe(old) + ".out")
        if new != old and src.exists():
            src.rename(rt.SNAPDIR / (rt.safe(new) + ".out"))
            moved += 1
    return moved


def main():
    old_ids = {line.split("\t")[0] for line in rt.BASELINE.read_text().splitlines()
               if line.strip() and not line.startswith("#")}
    moved = migrate_snapshots(old_ids)
    files = [f for f in FILES if f.exists()]
    for f in files:
        migrate_file(f)
    print(f"migrate_ids: {len(files)} files rewritten, {moved} snapshots renamed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
