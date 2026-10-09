#!/usr/bin/env python3
"""Contract test: the compiler's copies of the interpreter's limits and codes agree.

The compiled runtime (src/compiler/runtime) and its Earley parser
(src/compiler/earley) read what `elan --cexport` writes: identifier and sort
codes are hash positions in tables of MAXNOFIDENT and NTYPES entries, symbol
codes are the interpreter's (docs/ref-format.md). They keep their own copies
of these constants; this test fails when a copy disagrees.

Usage: check_limits.py [--src DIR]      Exit status: 0 if they agree, 1 otherwise.
"""
import argparse
import re
import sys
from pathlib import Path

SRC = Path(__file__).resolve().parent.parent.parent / "src"

DEFINE = re.compile(r"^\s*#\s*define\s+(\w+)\s+(.+?)\s*(?://.*|/\*.*)?$")


def defines(text):
    """{name: value text} of the active #defines (first definition wins)."""
    d = {}
    for line in text.splitlines():
        m = DEFINE.match(line)
        if m:
            d.setdefault(m.group(1), m.group(2).strip())
    return d


def evaluate(name, d, depth=0):
    """Integer value of a #define made of integers, names and + - ( )."""
    expr = d[name]
    if depth > 20 or not re.fullmatch(r"[\w\s()+-]+", expr):
        raise ValueError(f"{name}: cannot evaluate {expr!r}")
    expr = re.sub(r"\b[A-Za-z_]\w*\b", lambda m: f"({evaluate(m.group(0), d, depth + 1)})", expr)
    return int(eval(expr, {"__builtins__": {}}))  # digits, + - ( ) only


def stringtab_sizes(text):
    """{table: size} of the `stringtab name(size)` definitions."""
    return {m.group(1): int(m.group(2))
            for m in re.finditer(r"^\s*stringtab\s+(\w+)\s*\(\s*(\d+)\s*\)", text, re.M)}


def check(src=SRC):
    """List of problems (empty when the copies agree)."""
    interp = defines((src / "interpreter/base/constants.h").read_text())
    problems = []

    def want(where, what, got, expected):
        if got != expected:
            problems.append(f"{where}: {what} = {got}, the interpreter has {expected}")

    ident, ntypes = evaluate("MAXNOFIDENT", interp), evaluate("NTYPES", interp)

    rt = defines((src / "compiler/runtime/termIn.h").read_text())
    want("runtime/termIn.h", "TABOFIDENT_SIZE", evaluate("TABOFIDENT_SIZE", rt), ident)
    want("runtime/termIn.h", "TABOFSORT_SIZE", evaluate("TABOFSORT_SIZE", rt), ntypes)

    tabs = stringtab_sizes((src / "compiler/earley/runtimeInit.cc").read_text())
    for t, size in (("tabofident", ident), ("atabofident", ident), ("typet", ntypes)):
        want("earley/runtimeInit.cc", f"stringtab {t}", tabs.get(t), size)

    ear = defines((src / "compiler/earley/commondefs.h").read_text())
    for name in ("MAXNOFIDENT", "NTYPES", "MAXNFSYM", "IDLEN", "NNONTERMINALS", "FSYMTABSIZE",
                 "IDENT", "BOFIDENT", "MAXNOFSTRING", "STRING", "BOFSTRING", "JUSTNUMBER",
                 "BOFTYPES"):
        want("earley/commondefs.h", name, evaluate(name, ear), evaluate(name, interp))
    # FSYMCODESBEG is a stale copy (200, the interpreter has 300): it must stay unused
    for f in sorted((src / "compiler").rglob("*")):
        if f.suffix in (".c", ".cc", ".h", ".java", ".tpl", ".jj") and f.name != "commondefs.h":
            for n, line in enumerate(f.read_text(errors="replace").splitlines(), 1):
                code = line.split("//")[0].split("/*")[0]
                if re.search(r"\bFSYMCODESBEG\b", code):
                    problems.append(f"{f.relative_to(src)}:{n}: uses FSYMCODESBEG "
                                    "(the copy in earley/commondefs.h is stale)")

    # built-in symbol codes
    codes = defines((src / "interpreter/term/codes.h").read_text())
    for copy in ("compiler/runtime/codes.h", "compiler/earley/codes.h"):
        other = defines((src / copy).read_text())
        for name in sorted(set(codes) & set(other)):
            if codes[name] != other[name]:
                problems.append(f"{copy}: {name} = {other[name]}, the interpreter has {codes[name]}")
    return problems


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--src", type=Path, default=SRC)
    a = ap.parse_args(argv)
    problems = check(a.src)
    for p in problems:
        print(p)
    print(f"check_limits: {len(problems)} problems")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
