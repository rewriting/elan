#!/bin/sh
# Builds bin/<prog> (Boehm GC) and bin/<prog>-arena (bump arena) at -O2.
set -e
cd "$(dirname "$0")"
GC=/opt/homebrew/opt/bdw-gc
CC=${CC:-cc}
CFLAGS="-O2 -std=c11 -Wall -Wextra -Wno-unused-parameter"
mkdir -p bin
for p in ${PROGS:-nqueens efib propc}; do
  $CC $CFLAGS -I$GC/include -o bin/$p runtime.c $p.c $GC/lib/libgc.a -Wl,-w
  $CC $CFLAGS -DARENA -o bin/$p-arena runtime.c $p.c
done
echo "built: $(ls bin | tr '\n' ' ')"
