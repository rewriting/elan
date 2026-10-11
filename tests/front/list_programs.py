#!/usr/bin/env python3
"""The program corpus of S8a (plan finding 7, criterion 1 of spec S8, loading
only), one program per line: KIND<TAB>DIR<TAB>LGI<TAB>SPC|-  (DIR relative to
REPO, LGI and SPC without extension), deduplicated by (DIR, LGI, SPC), first
kind wins, in this order:
  bench        the J cases of the legacy bench (tests/compiler-rs/list_j_programs.py)
  golden       the `ref` cases of tests/golden/cases.tsv
  example      examples/<name>/<name>.lgi [+ its .spc]
  program      every .lgi of tests/compiler-rs/programs and tests/compiler/programs
  benchthesis  test_benchthesis.sh: FastCompletion ans x 15 specifications,
               MinelaComp rewritingC x 7 (6 distinct)
  fixture      tests/front/programs/<name>/<name>.lgi
Usage: list_programs.py REPO"""
import subprocess
import sys
from pathlib import Path

FAST = ("KNZ86 Zeh89 curry exa79 exa80 exa85 expf expoclos furtin gdiv group "
        "p2 p8 sample taussky").split()
MINELA = "append congruence or peano primes primes sample".split()


def programs(repo):
    repo = Path(repo).resolve()
    out, seen = [], set()

    def add(kind, d, lgi, spc):
        d = Path(d)
        rel = str(d.resolve().relative_to(repo)) if d.is_absolute() else str(d)
        key = (rel, lgi, spc)
        if key not in seen and (repo / rel / f"{lgi}.lgi").exists():
            seen.add(key)
            out.append((kind, rel, lgi, spc))

    j = subprocess.run([sys.executable, str(repo / "tests/compiler-rs/list_j_programs.py")],
                       check=True, capture_output=True, text=True).stdout
    for line in j.splitlines():
        d, lgi, spc = line.split("\t")
        add("bench", d, lgi, "-" if spc == "no" else spc)
    for line in (repo / "tests/golden/cases.tsv").read_text().splitlines():
        f = line.split("\t")
        if f[0] == "ref":
            add("golden", f[2], f[3], f[4])
    for d in sorted(p for p in (repo / "examples").iterdir() if p.is_dir()):
        specs = sorted(p.stem for p in d.glob("*.spc"))
        add("example", d, d.name, specs[0] if specs else "-")
    for root in ("tests/compiler-rs/programs", "tests/compiler/programs"):
        for lgi in sorted((repo / root).rglob("*.lgi")):
            if "work" not in lgi.relative_to(repo).parts:
                add("program", lgi.parent, lgi.stem, "-")
    bt = repo / "tests/compiler/benchthesis"
    for spc in FAST:
        add("benchthesis", bt / "FastCompletion", "ans", spc)
    for spc in MINELA:
        add("benchthesis", bt / "MinelaComp", "rewritingC", spc)
    for d in sorted(p for p in (repo / "tests/front/programs").iterdir() if p.is_dir()):
        add("fixture", d, d.name, "-")
    return out


def main(argv):
    if len(argv) != 2:
        sys.exit("usage: list_programs.py REPO")
    for row in programs(argv[1]):
        print("\t".join(row))


if __name__ == "__main__":
    main(sys.argv)
