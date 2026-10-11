#!/usr/bin/env python3
"""Prints each term read on stdin (one per line) in a canonical form modulo
associativity-commutativity, to compare the results of a compiled program
with the interpreter's: the interpreter prints an AC term as it was built
(F(F(c,b),a)), REM and elanc-go flattened and sorted (F(a,F(b,c)); S7c
plan, investigation).

Usage: ac_canon.py SYMBOL...   (the AC symbols: a prefix one is printed
F(x,y), an infix one x U y; terms are identifiers with arguments, ints,
and at most one infix AC symbol, of the lowest priority)
       ac_canon.py --infix OP... -- SYMBOL...   (the infix operators of the
terms, AC or not, from the lowest priority to the highest; parentheses
allowed: (2+(X*6)) and 2+X*6 give the same canonical form)

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

def parse_infix(toks, ac, infix):
    """Precedence climbing over the infix operators (lowest first); an
    operator used several times at one level gives one n-ary node."""
    pos = 0
    def expr(level):
        nonlocal pos
        if level == len(infix):
            return primary()
        items = [expr(level + 1)]
        while pos < len(toks) and toks[pos] == infix[level]:
            pos += 1
            items.append(expr(level + 1))
        return items[0] if len(items) == 1 else (infix[level], items)
    def primary():
        nonlocal pos
        name = toks[pos]
        pos += 1
        if name == "(":
            t = expr(0)
            if toks[pos] != ")":
                raise ValueError("expected )")
            pos += 1
            return t
        if pos < len(toks) and toks[pos] == "(":
            pos += 1
            args = [expr(0)]
            while toks[pos] == ",":
                pos += 1
                args.append(expr(0))
            if toks[pos] != ")":
                raise ValueError("expected )")
            pos += 1
            return (name, args)
        return (name, [])
    t = expr(0)
    if pos != len(toks):
        raise ValueError("trailing tokens")
    return t

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
    args = sys.argv[1:]
    infix = None
    if args and args[0] == "--infix":
        end = args.index("--")
        infix, args = args[1:end], args[end + 1:]
    ac = set(args)
    for line in sys.stdin:
        line = line.rstrip("\n")
        try:
            if infix is None:
                t = parse(tokens(line), ac)
            else:
                # a flattened n-ary node is canonical only for AC symbols
                t = parse_infix(tokens(line), ac, infix)
            print(text(canon(t, ac)))
        except (ValueError, IndexError):
            print(line)

if __name__ == "__main__":
    main()
