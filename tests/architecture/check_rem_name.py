#!/usr/bin/env python3
"""No test, script or CI step calls the bare name `elanc` (S7e task 9).

REM's driver is installed as `elanc` and as `elanc-rem`; the tests call it
`elanc-rem` (and the Go compiler `elanc-go`), so that no test depends on what
the name `elanc` designates (it may become the Go compiler, S7e task 10).

Scanned: the scripts (*.sh, *.py, Makefile, *.yml, Dockerfile*) under tests/,
ci/, .github/, reference/ and the top-level Makefile. A line is a comment
when its first non-blank character is `#`, and a `#` after a blank starts a
comment to the end of the line; comments are not checked. Any
other line naming `elanc` as a word (not `elanc-go`, `elanc.log`, `$ELANC`,
`.elan.x`), a path such as `bin/elanc` included, is reported, unless it
carries the marker `bare-elanc-ok` (with the reason next to it).

Usage: check_rem_name.py [--root DIR]   Exit status: 0 if none, 1 otherwise.
"""
import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
DIRS = ["tests", "ci", ".github", "reference"]
SKIP = {"work", "results", "programs", "__pycache__", "build", "install"}
# this checker and its tests name `elanc` on purpose
SELF = {"check_rem_name.py", "test_check_rem_name.py"}
MARKER = "bare-elanc-ok"
# `elanc` as a word: not preceded by a letter, digit, _, -, . or $ (but a
# `/` may: bin/elanc), not followed by a letter, digit, _, - or .
BARE = re.compile(r"(?<![\w.$-])elanc(?![\w.-])")
COMMENT = re.compile(r"\s#.*$")


def is_script(p):
    return (p.suffix in (".sh", ".py", ".yml", ".yaml") or p.name == "Makefile"
            or p.name.startswith("Dockerfile"))


def scripts(root):
    """The files to check, relative to root, sorted."""
    out = [root / "Makefile"] if (root / "Makefile").exists() else []
    for d in DIRS:
        base = root / d
        if not base.is_dir():
            continue
        for p in base.rglob("*"):
            rel = p.relative_to(root)
            if (p.is_file() and is_script(p) and p.name not in SELF
                    and not SKIP.intersection(rel.parts[:-1])):
                out.append(p)
    return sorted(out)


def bare_calls(text):
    """[(line number, line)] of the lines naming the bare `elanc`."""
    found = []
    for i, line in enumerate(text.splitlines(), 1):
        s = line.strip()
        if s.startswith("#") or MARKER in line:
            continue
        if BARE.search(COMMENT.sub("", line)):
            found.append((i, s))
    return found


def check(root):
    problems = []
    for p in scripts(root):
        for n, line in bare_calls(p.read_text(errors="replace")):
            problems.append(f"{p.relative_to(root)}:{n}: bare elanc (use elanc-rem or elanc-go): {line}")
    return problems


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("--root", type=Path, default=ROOT)
    a = ap.parse_args(argv)
    problems = check(a.root)
    for p in problems:
        print(p)
    print(f"check_rem_name: {len(problems)} bare calls of elanc")
    return 1 if problems else 0


if __name__ == "__main__":
    sys.exit(main())
