#!/bin/sh
# Tests of the harness elan-lextrace (S8a task 1).
#  1. `elan-lextrace lex F` on each tests/front/fixtures/F that has an
#     F.expected (stdout, then "@@stderr", stderr, then "@@status N"), run in
#     the fixtures directory; any difference fails. With --save, the fixtures
#     without an .expected get one (to be read before committing it).
#  2. `lex` refuses an IF (its condition needs booltype, set by the loader):
#     status 3, the message `[elan-lextrace] IF needs the loader`, no crash
#     (it was a bus error).
#  3. Loader mode on examples/unification: status 0, @begin/@end balanced,
#     25 module streams (= the "handling module" lines of elan without -b;
#     the plan said 32, a miscount: its 8 717 lines are found), 10 FOR EACH regions, each @foreach-begin closed by
#     an @foreach-end (regions may nest) before the @end of its module.
# Usage: tests/front/test_lextrace_harness.sh [BUILD] [--save]
set -u
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
BUILD=${1:-build}
SAVE=${2:-}
case "$BUILD" in /*) ;; *) BUILD=$ROOT/$BUILD ;; esac
H=$BUILD/src/interpreter/elan-lextrace
FIX=$ROOT/tests/front/fixtures
export ELANLIB=${ELANLIB:-$BUILD/install}
if [ ! -x "$H" ]; then echo "test_lextrace_harness: FAIL: $H not built"; exit 1; fi

mkdir -p "$ROOT/tests/front/work"
W=$(mktemp -d "$ROOT/tests/front/work/harness.XXXXXX")
fail=0; n=0

render() { # render FILE: the harness output in the .expected format
  ( cd "$FIX" && "$H" lex "$1" > "$W/out" 2> "$W/err" < /dev/null ); st=$?
  cat "$W/out"; echo "@@stderr"; cat "$W/err"; echo "@@status $st"
}

for f in $(cd "$FIX" && ls | grep -v '\.expected$'); do
  if [ -f "$FIX/$f.expected" ]; then
    n=$((n + 1))
    render "$f" > "$W/got"
    if ! cmp -s "$W/got" "$FIX/$f.expected"; then
      echo "FAIL lex $f"; diff "$FIX/$f.expected" "$W/got" | head -10; fail=$((fail + 1))
    fi
  elif [ "$SAVE" = "--save" ]; then
    render "$f" > "$FIX/$f.expected"; echo "saved $f.expected"
  fi
done
echo "lex: $n fixtures, $fail failed"

# lex refuses IF (standalone, booltype is unset: a bus error before)
for src in 'module m IF 1 THEN { a } end' 'module m { b }~(1) IF x : { a } end'; do
  printf '%s\n' "$src" > "$W/if.eln"
  ( cd "$W" && "$H" lex if.eln > "$W/if.out" 2> "$W/if.err" < /dev/null ); st=$?
  if [ $st -ne 3 ] || ! grep -q '^\[elan-lextrace\] IF needs the loader' "$W/if.err"; then
    echo "FAIL lex refuses IF ($src): status $st"; head -3 "$W/if.err"; fail=$((fail + 1))
  fi
done

# loader mode
cp -R "$ROOT/examples/unification" "$W/unif"
( cd "$W/unif" && "$H" load "$W/trace" --export /dev/null -b unification.lgi simplesig.spc \
    < /dev/null > "$W/load.out" 2> "$W/load.err" ); st=$?
if [ $st -ne 0 ]; then
  echo "FAIL load unification: status $st"; head -5 "$W/load.err"; fail=$((fail + 1))
else
  if ! awk '
      /^@begin / { d++; nb++; fe[d] = 0; next }
      /^@end$/   { if (d < 1 || fe[d]) bad = 1; d--; next }
      /^@foreach-begin$/ { fe[d]++; nf++; next }
      /^@foreach-end$/   { if (fe[d] < 1) bad = 1; fe[d]--; next }
      END { if (d != 0 || bad || nb != 25 || nf != 10) {
              printf "begin=%d foreach=%d depth=%d bad=%d\n", nb, nf, d, bad; exit 1 }
            printf "load unification: %d lines, %d modules, %d FOR EACH regions\n", NR, nb, nf }
    ' "$W/trace"; then
    echo "FAIL load unification: unbalanced or unexpected counts"; fail=$((fail + 1))
  fi
fi

if [ -x "$BUILD/install/bin/elan" ]; then
  nh=$(cd "$W/unif" && "$BUILD/install/bin/elan" --export /dev/null unification.lgi simplesig.spc \
         < /dev/null 2>&1 | grep -c 'handling module')
  nb=$(grep -c '^@begin ' "$W/trace")
  if [ "$nh" != "$nb" ]; then
    echo "FAIL load unification: $nb @begin, elan prints $nh 'handling module'"; fail=$((fail + 1))
  fi
fi

if [ $fail -eq 0 ]; then rm -rf "$W"; echo "test_lextrace_harness: ok"; exit 0; fi
echo "test_lextrace_harness: $fail failure(s), work directory $W"; exit 1
