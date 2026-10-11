#!/usr/bin/env python3
"""Prints stdin with REM's identifiers replaced by their names: a compiled
C program prints an identifier as IDENT(n), n its index in the Identifiers
table of the .ref file it was compiled from (src/compiler/runtime/
termOut.c); the interpreter and elanc-go print its name (S7d task 3).

Usage: rem_idents.py PROG.ref < output"""
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

def main():
    names = identifiers(sys.argv[1])
    data = sys.stdin.buffer.read()
    out = re.sub(rb"IDENT\((\d+)\)", lambda m: names.get(int(m.group(1)), m.group(0)), data)
    sys.stdout.buffer.write(out)

if __name__ == "__main__":
    main()
