#!/bin/sh
# The S8a error corpus (plan task 9): each case through the C++ and through
# front-trace; stdout, stderr and status compared byte for byte.
#   errors/lex/<case>.eln|.lgi|.spc   `elan-lextrace lex` vs `front-trace lex`
#   errors/load/<case>/               a program, its `args` (elan options and
#                                     files), run in a private copy whose
#                                     parent is empty (the `..` lookup):
#                                     `elan ARGS` vs `front-trace load ARGS`
# The expected output of a case is the C++'s, checked in as
# errors/lex/<case>.expected or errors/load/<case>/expected (stdout, then
# "@@stderr", stderr, then "@@status N"); --save (re)writes them from the C++.
# Generated cases (sources made here, expected checked in): lex
# strings10001.eln (too many string constants), idents3000.eln (tabofident
# overflow); load importoverflow (a chain of 200 modules: the import table).
# A case whose directory has a file `earley` stops its stderr comparison at
# the line `[earley] syntax error on symbol ...` (Earley's expected-symbol
# list needs the grammar: S8b, plan task 9 ruling).
# Usage: tests/front/test_front_errors.sh [PREFIX] [--save]
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)
PREFIX=${1:-build/install}
SAVE=${2:-}
case "$PREFIX" in /*) ;; *) PREFIX=$REPO/$PREFIX ;; esac
BUILD=$(cd "${BUILD:-$(dirname "$PREFIX")}" && pwd) || exit 2   # absolute: used after cd
H=$BUILD/src/interpreter/elan-lextrace
ELAN=$PREFIX/bin/elan
[ -x "$H" ] || { echo "test_front_errors: $H not built"; exit 2; }
[ -x "$ELAN" ] || { echo "test_front_errors: $ELAN not installed"; exit 2; }
export ELANLIB=$PREFIX
unset SECONDELANLIB

mkdir -p "$HERE/work"
RUN=$(mktemp -d "$HERE/work/errors.XXXXXX") || exit 2
FT=$RUN/front-trace
( cd "$REPO/src/compiler-go" && GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off \
    GOCACHE=${GOCACHE:-$BUILD/src/compiler-go/gocache} go build -o "$FT" ./front/cmd/front-trace ) || exit 2

# generated sources
mkdir -p "$RUN/lex" "$RUN/gen"
cp "$HERE"/errors/lex/*.eln "$HERE"/errors/lex/*.lgi "$HERE"/errors/lex/*.spc "$RUN/lex/" 2>/dev/null
python3 - "$RUN" <<'EOF'
import os, sys
run = sys.argv[1]
with open(os.path.join(run, "lex", "strings10001.eln"), "w") as f:
    f.write("module m\n" + "\n".join('"s%d"' % i for i in range(10001)) + "\nend\n")
with open(os.path.join(run, "lex", "idents3000.eln"), "w") as f:
    f.write("module m\n" + "\n".join("i%d" % i for i in range(3000)) + "\nend\n")
d = os.path.join(run, "gen", "importoverflow")
os.makedirs(d)
with open(os.path.join(d, "p.lgi"), "w") as f:
    f.write("LPL p description\n  query of sort s\n  result of sort s\n"
            "  import m0\n  start with () c\nend\n")
with open(os.path.join(d, "bool.eln"), "w") as f:
    f.write("module bool\nend\n")
for i in range(200):
    with open(os.path.join(d, "m%d.eln" % i), "w") as f:
        f.write("module m%d\nimport global m%d; end\nend\n" % (i, i + 1) if i < 199
                else "module m199\nend\n")
with open(os.path.join(d, "args"), "w") as f:
    f.write("-b --export /dev/null p.lgi\n")
EOF

render() { # render OUT ERR STATUS: the .expected format
  cat "$1"; echo "@@stderr"; cat "$2"; echo "@@status $3"
}

# earley_cut FILE: the .expected text up to the `[earley] syntax error` line
earley_cut() {
  awk '/^@@stderr$/ { s = 1 } { print } s && /^\[earley\] syntax error on symbol / { exit }' "$1"
}

n=0 fail=0
check() { # check NAME EXPECTED GOT [earley]
  n=$((n + 1))
  if [ "$SAVE" = --save ]; then return; fi
  if [ ! -f "$2" ]; then echo "FAIL $1: no expected output (run with --save)"; fail=$((fail + 1)); return; fi
  if [ -n "${4:-}" ]; then
    earley_cut "$2" > "$RUN/exp.cut"; earley_cut "$3" > "$RUN/got.cut"
    tail -1 "$2" > "$RUN/exp.st"; tail -1 "$3" > "$RUN/got.st"
    if cmp -s "$RUN/exp.cut" "$RUN/got.cut" && cmp -s "$RUN/exp.st" "$RUN/got.st"; then return; fi
  elif cmp -s "$2" "$3"; then return
  fi
  echo "FAIL $1"; diff "$2" "$3" | head -12 | sed 's/^/    /'; fail=$((fail + 1))
}

for f in "$RUN"/lex/*; do
  b=$(basename "$f")
  exp=$HERE/errors/lex/$b.expected
  if [ "$SAVE" = --save ]; then
    ( cd "$RUN/lex" && "$H" lex "$b" > "$RUN/o" 2> "$RUN/e" < /dev/null ); st=$?
    render "$RUN/o" "$RUN/e" $st > "$exp"; echo "saved errors/lex/$b.expected"
  fi
  ( cd "$RUN/lex" && "$FT" lex "$b" > "$RUN/o" 2> "$RUN/e" < /dev/null ); st=$?
  render "$RUN/o" "$RUN/e" $st > "$RUN/got"
  check "lex $b" "$exp" "$RUN/got"
done

for d in "$HERE"/errors/load/*/ "$RUN"/gen/*/; do
  [ -f "$d/args" ] || continue
  c=$(basename "$d")
  exp=$HERE/errors/load/$c/expected
  [ -d "$HERE/errors/load/$c" ] || mkdir -p "$HERE/errors/load/$c"
  w=$RUN/load/$c; mkdir -p "$w"; cp -R "$d" "$w/$c"; rm -f "$w/$c/expected"
  args=$(cat "$d/args")
  if [ "$SAVE" = --save ]; then
    ( cd "$w/$c" && $ELAN $args > "$RUN/o" 2> "$RUN/e" < /dev/null ); st=$?
    render "$RUN/o" "$RUN/e" $st > "$exp"; echo "saved errors/load/$c/expected"
  fi
  ( cd "$w/$c" && "$FT" load $args > "$RUN/o" 2> "$RUN/e" < /dev/null ); st=$?
  render "$RUN/o" "$RUN/e" $st > "$RUN/got"
  if [ -f "$d/earley" ]; then check "load $c" "$exp" "$RUN/got" earley
  else check "load $c" "$exp" "$RUN/got"; fi
  rm -rf "$w"
done

echo "test_front_errors: $n cases, $fail failed"
if [ $fail -eq 0 ]; then rm -rf "$RUN"; exit 0; fi
echo "work directory: $RUN"; exit 1
