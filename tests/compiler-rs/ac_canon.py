#!/usr/bin/env python3
"""Prints each term read on stdin (one per line) in a canonical form modulo
associativity-commutativity, to compare the results of a compiled program
with the interpreter's: the interpreter prints an AC term as it was built
(F(F(c,b),a)), REM and elanc-go flattened and sorted (F(a,F(b,c)); S7c
plan, investigation).

Usage: ac_canon.py SYMBOL...   (the AC symbols: a prefix one is printed
F(x,y), an infix one x U y; terms are identifiers with arguments, ints,
and at most one infix AC symbol, of the lowest priority)

A canonical term is the prefix form with the arguments of each AC symbol
flattened and sorted (as text): F(F(c,b),a) and F(a,F(b,c)) both give
F(a,b,c). A line that does not parse is printed unchanged."""
import re, sys

TOKEN = re.compile(r"\s*(-?[0-9]+|[A-Za-z_][A-Za-z0-9_']*|.)")

def tokens(s):
    out, i = [], 0
    while i < len(s):
        m = TOKEN.match(s, i)
        if not m or not m.group(1).strip():
            break
        out.append(m.group(1))
        i = m.end()
    return out

def parse(toks, ac):
    pos = 0
    def term():
        nonlocal pos
        t = primary()
        items = [t]
        # (a symbol after a term is infix: a prefix one is followed by "(")
        while pos < len(toks) and toks[pos] in ac:
            op = toks[pos]
            pos += 1
            items.append(primary())
        if len(items) == 1:
            return t
        return (op, items)
    def primary():
        nonlocal pos
        name = toks[pos]
        pos += 1
        if pos < len(toks) and toks[pos] == "(":
            pos += 1
            args = [term()]
            while toks[pos] == ",":
                pos += 1
                args.append(term())
            if toks[pos] != ")":
                raise ValueError("expected )")
            pos += 1
            return (name, args)
        return (name, [])
    t = term()
    if pos != len(toks):
        raise ValueError("trailing tokens")
    return t

def canon(t, ac):
    name, args = t
    args = [canon(a, ac) for a in args]
    if name in ac:
        flat = []
        for a in args:
            flat.extend(a[1] if a[0] == name else [a])
        args = sorted(flat, key=text)
    return (name, args)

def text(t):
    name, args = t
    return name + ("(" + ",".join(text(a) for a in args) + ")" if args else "")

def main():
    ac = set(sys.argv[1:])
    for line in sys.stdin:
        line = line.rstrip("\n")
        try:
            print(text(canon(parse(tokens(line), ac), ac)))
        except (ValueError, IndexError):
            print(line)

if __name__ == "__main__":
    main()
