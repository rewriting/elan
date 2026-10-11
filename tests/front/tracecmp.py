#!/usr/bin/env python3
"""Compares a C++ lexem trace (elan-lextrace) with a Go one (front-trace).

  tracecmp.py --mode lex|load C GO [--stderr CERR GOERR] [--status CS GS]

lex mode: the traces must be equal line for line (positions included).
load mode: three exclusions, each counted (plan S8a, finding 6), all on the
C++ side except the third:
  * the lines strictly inside an outermost @foreach-begin/@foreach-end region
    (the bodies of FOR EACH/IF: S8a does not evaluate them; the markers stay);
  * the subtrees `@begin strconc.eln` ... `@end` (strconc[X,X,X] is loaded by
    trclos from a module body, S8b);
  * the position fields of `id <pos> <text>` and `str <i> <text>` lines, on
    both sides (numbering is S8b's).
Nothing else is removed: any other difference (a lexem, a marker, a
@begin/@end, a strconc subtree on the Go side) fails. With --stderr, the two
files must be equal; with --status, the two statuses.
On a difference: the first one, with 5 lines of context on each side and the
module stack (@begin names) at that point. On success, one summary line
`ok lines=<n> foreach=<n> strconc=<n>`. Exit status 0 (equal) or 1."""
import argparse
import re
import sys

POS = re.compile(r"^(id|str) -?\d+ ")


def read_lines(path):
    with open(path, "rb") as f:
        text = f.read().decode("latin-1")
    lines = text.split("\n")
    return lines[:-1] if lines and lines[-1] == "" else lines


def filter_cxx(lines):
    """C++ side of load mode: (kept [(lineno, text)], foreach, strconc)."""
    kept, foreach, strconc = [], 0, 0
    fe = 0           # depth inside @foreach regions
    sc = 0           # depth inside a strconc subtree (counting @begin/@end)
    for n, line in enumerate(lines, 1):
        if sc:
            if line.startswith("@begin "):
                sc += 1
            elif line == "@end":
                sc -= 1
            continue
        if line == "@begin strconc.eln" and fe == 0:
            strconc += 1
            sc = 1
            continue
        if line == "@foreach-begin":
            fe += 1
            if fe == 1:
                foreach += 1
                kept.append((n, line))
            continue
        if line == "@foreach-end" and fe > 0:
            fe -= 1
            if fe == 0:
                kept.append((n, line))
            continue
        if fe == 0:
            kept.append((n, line))
    return kept, foreach, strconc


def strip_pos(entries):
    return [(n, POS.sub(r"\1 ", t)) for n, t in entries]


def module_stack(entries, upto):
    stack = []
    for _, t in entries[:upto]:
        if t.startswith("@begin "):
            stack.append(t[7:])
        elif t == "@end" and stack:
            stack.pop()
    return stack


def report(c, g, i, cname, gname, out):
    out.write(f"first difference at entry {i + 1} "
              f"(C++ line {c[i][0] if i < len(c) else 'EOF'}, "
              f"Go line {g[i][0] if i < len(g) else 'EOF'})\n")
    out.write("module stack (C++): " + " > ".join(module_stack(c, i)) + "\n")
    lo = max(0, i - 5)
    for name, side in ((cname, c), (gname, g)):
        out.write(f"--- {name}\n")
        for j in range(lo, min(len(side), i + 6)):
            mark = ">" if j == i else " "
            out.write(f"{mark} {side[j][0]:>7}: {side[j][1]}\n")
        if i >= len(side):
            out.write(">     EOF\n")


def compare(mode, cpath, gpath, out=sys.stdout):
    craw, graw = read_lines(cpath), read_lines(gpath)
    if mode == "lex":
        c = list(enumerate(craw, 1))
        g = list(enumerate(graw, 1))
        foreach = strconc = 0
    else:
        c, foreach, strconc = filter_cxx(craw)
        c = strip_pos(c)
        g = strip_pos(list(enumerate(graw, 1)))
    for i in range(max(len(c), len(g))):
        if i >= len(c) or i >= len(g) or c[i][1] != g[i][1]:
            report(c, g, i, "C++ " + cpath, "Go " + gpath, out)
            return 1, foreach, strconc
    return 0, foreach, strconc


def main(argv, out=sys.stdout):
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--mode", choices=("lex", "load"), required=True)
    ap.add_argument("cxx")
    ap.add_argument("go")
    ap.add_argument("--stderr", nargs=2, metavar=("CERR", "GOERR"))
    ap.add_argument("--status", nargs=2, metavar=("CS", "GS"))
    a = ap.parse_args(argv)
    st, foreach, strconc = compare(a.mode, a.cxx, a.go, out)
    if st:
        return 1
    if a.stderr:
        ce, ge = (open(p, "rb").read() for p in a.stderr)
        if ce != ge:
            out.write("stderr differs\n--- C++\n" + ce.decode("latin-1")
                      + "--- Go\n" + ge.decode("latin-1"))
            return 1
    if a.status and a.status[0] != a.status[1]:
        out.write(f"status differs: C++ {a.status[0]}, Go {a.status[1]}\n")
        return 1
    out.write(f"ok lines={len(read_lines(a.cxx))} foreach={foreach} strconc={strconc}\n")
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
