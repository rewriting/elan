#!/usr/bin/env python3
"""The lexer corpus of S8a (plan finding 1): every .eln, .lgi and .spc file
under src/lib, examples and tests of REPO, without work/ directories and
without tests/front (its fixtures are run by test_lextrace_harness.sh and the
Go unit tests). One line per file: PATH<TAB>standalone|loaded, PATH relative
to REPO, sorted. `loaded`: a .eln whose text, comments and strings stripped,
contains FOR EACH or IF (their headers need the loader: actgram(), typet,
Earley), so elan-lextrace cannot read it standalone; it is covered by the
loader traces of test_front_load.sh instead.
Usage: list_files.py REPO"""
import re
import sys
from pathlib import Path

ROOTS = ("src/lib", "examples", "tests")
EXTS = (".eln", ".lgi", ".spc")
COMMENT = re.compile(rb'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\])*"', re.S)
NEEDS_LOADER = re.compile(rb"\bFOR\s+EACH\b|\bIF\b")


def kind(path):
    if path.suffix != ".eln":
        return "standalone"
    text = COMMENT.sub(b" ", path.read_bytes())
    return "loaded" if NEEDS_LOADER.search(text) else "standalone"


def files(repo):
    repo = Path(repo).resolve()
    out = []
    for root in ROOTS:
        for p in (repo / root).rglob("*"):
            rel = p.relative_to(repo)
            if p.suffix not in EXTS or not p.is_file():
                continue
            if "work" in rel.parts or rel.parts[:2] == ("tests", "front"):
                continue
            out.append((str(rel), kind(p)))
    return sorted(out)


def main(argv):
    if len(argv) != 2:
        sys.exit("usage: list_files.py REPO")
    for rel, k in files(argv[1]):
        print(f"{rel}\t{k}")


if __name__ == "__main__":
    main(sys.argv)
