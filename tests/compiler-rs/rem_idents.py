#!/usr/bin/env python3
"""Prints stdin with REM's identifiers replaced by their names: a compiled
C program prints an identifier as IDENT(n), n its index in the Identifiers
table of the .ref file it was compiled from (src/compiler/runtime/
termOut.c); the interpreter and elanc-go print its name (S7d task 3).

With --query FILE (a query read by the program, S7d task 5), the
identifiers of the query that the table does not have get the position
REM's runtime gives them (src/compiler/earley/stringtab.cc: the sum of the
bytes, as signed C chars, modulo 3000, a collision probed by steps of
211), in the order they are read.

Usage: rem_idents.py PROG.ref [--query FILE] < output"""
import re, sys

def identifiers(path):
    names, inside = {}, False
    with open(path, "rb") as f:
        for line in f:
            line = line.rstrip(b"\n")
            if line == b"Identifiers":
                inside = True
                continue
            if inside:
                m = re.match(rb'(\d+):"(.*)"\.$', line)
                if not m:
                    break
                names[int(m.group(1))] = m.group(2)
    return names

def query_identifiers(src):
    """The identifiers of a query, in reading order (earley/lstream.cc:
    a letter then letters, digits and _, the first 49 characters; strings
    and comments skipped)."""
    out, i = [], 0
    while i < len(src):
        c = src[i:i+1]
        if src.startswith(b"//", i):
            j = src.find(b"\n", i)
            i = len(src) if j < 0 else j
        elif src.startswith(b"/*", i):
            j = src.find(b"*/", i + 2)
            i = len(src) if j < 0 else j + 2
        elif c == b'"':
            i += 1
            while i < len(src) and src[i:i+1] != b'"':
                i += 2 if src[i:i+1] == b"\\" else 1
            i += 1
        elif c.isalpha() and c.isascii():
            j = i + 1
            while j < len(src) and (src[j:j+1].isalnum() and src[j:j+1].isascii() or src[j:j+1] == b"_"):
                j += 1
            out.append(src[i:j][:49])
            i = j
        else:
            i += 1
    return out

def add(names, name):
    """stringtab::addstr: the position of name, added when absent."""
    if name in names.values():
        return
    pos = sum(b - 256 if b > 127 else b for b in name) % 3000
    while pos in names:
        pos = (pos + 211) % 3000
    names[pos] = name

def main():
    names = identifiers(sys.argv[1])
    if len(sys.argv) > 3 and sys.argv[2] == "--query":
        with open(sys.argv[3], "rb") as f:
            for n in query_identifiers(f.read()):
                add(names, n)
    data = sys.stdin.buffer.read()
    out = re.sub(rb"IDENT\((\d+)\)", lambda m: names.get(int(m.group(1)), m.group(0)), data)
    sys.stdout.buffer.write(out)

if __name__ == "__main__":
    main()
