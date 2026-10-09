#!/usr/bin/env python3
"""Architecture test: module dependency rules of the ELAN interpreter.

Each directory of src/interpreter is a module. A file may include headers of
its own module and of the modules its module depends on (RULES, from the S3a
spec, decision D2). Existing back-edges are listed, with a reason, in
allowed-exceptions.txt; that list may only shrink: an exception that is no
longer needed is reported as stale.

Usage: check_deps.py [--root DIR] [--exceptions FILE] [--list-violations]
Exit status: 0 if clean, 1 otherwise.
"""
import argparse
import re
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent.parent / "src" / "interpreter"

_CORE = ["lex", "base"]
RULES = {
    "base": [],
    "lex": ["base"],
    "parse": ["lex", "base"],
    "term": ["parse"] + _CORE,
    "match": ["term", "parse"] + _CORE,
    "rewrite": ["match", "term", "parse"] + _CORE,
    "load": ["rewrite", "match", "term", "parse"] + _CORE,
}
for _m in ("meta", "ref", "peval", "command", "compile"):
    RULES[_m] = ["load", "rewrite", "match", "term", "parse"] + _CORE
RULES["driver"] = sorted(set(RULES) - {"driver"})

SOURCE = re.compile(r"\.(c|cc|h|t|parser)$")
INCLUDE = re.compile(r'^\s*#\s*include\s*"([^"]+)"', re.M)
COMMENT = re.compile(r"/\*.*?\*/|//[^\n]*", re.S)


def sources(root, rules):
    for mod in rules:
        d = root / mod
        if d.is_dir():
            for p in sorted(d.rglob("*")):
                if p.is_file() and SOURCE.search(p.name):
                    yield p


def module_of(root, path):
    return path.relative_to(root).parts[0]


def resolve(root, rules, src, name):
    """Header as the compiler finds it: next to the includer, else in a module dir."""
    local = (src.parent / name).resolve()
    if local.is_file() and root.resolve() in local.parents:
        return root / local.relative_to(root.resolve())
    for mod in rules:
        for cand in (root / mod / name, *(root / mod).rglob(name)):
            if cand.is_file():
                return cand
    return None


def edges(root, rules):
    for src in sources(root, rules):
        text = COMMENT.sub("", src.read_text(errors="replace"))
        for name in INCLUDE.findall(text):
            hdr = resolve(root, rules, src, name)
            if hdr is not None:
                yield src, hdr


def rel(root, p):
    return p.relative_to(root).as_posix()


def violations(root, rules, exceptions):
    out = []
    for src, hdr in edges(root, rules):
        a, b = module_of(root, src), module_of(root, hdr)
        if a == b or b in rules.get(a, []):
            continue
        e = (rel(root, src), rel(root, hdr))
        if e not in exceptions and e not in out:
            out.append(e)
    return out


def stale_exceptions(root, rules, exceptions):
    used = {(rel(root, s), rel(root, h)) for s, h in edges(root, rules)}
    return sorted(e for e in exceptions if e not in used)


def load_exceptions(path):
    exc = set()
    if path.exists():
        for line in path.read_text().splitlines():
            line = line.split("#", 1)[0].strip()
            if line:
                a, b = (x.strip() for x in line.split("->"))
                exc.add((a, b))
    return exc


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", type=Path, default=ROOT)
    ap.add_argument("--exceptions", type=Path, default=HERE / "allowed-exceptions.txt")
    ap.add_argument("--list-violations", action="store_true",
                    help="print every forbidden edge, ignoring the exceptions file")
    a = ap.parse_args()
    exc = set() if a.list_violations else load_exceptions(a.exceptions)
    bad = violations(a.root, RULES, exc)
    stale = [] if a.list_violations else stale_exceptions(a.root, RULES, exc)
    for s, h in bad:
        print(f"FORBIDDEN {s} -> {h}  ({module_of(a.root, a.root / s)} may not depend on "
              f"{module_of(a.root, a.root / h)})")
    for s, h in stale:
        print(f"STALE exception (no longer needed, remove it): {s} -> {h}")
    print(f"architecture: {len(bad)} forbidden includes, {len(stale)} stale exceptions, "
          f"{len(exc)} listed exceptions")
    return 1 if (bad or stale) else 0


if __name__ == "__main__":
    sys.exit(main())
