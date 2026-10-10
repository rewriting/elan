#!/bin/sh
# Strategies evaluated at run time by the compiled runtime (str_eval2,
# src/compiler/runtime/streval.c): dc, one (dc one) and
# `if S then S1 orelse S2 fi`, compared with the interpreter.
#
# `elanc -strategy 1` makes the generated `eval` strategy of strat[X] (the
# interpreter of strategy terms, elanlib/strategy/strat.eln) call the C
# function str_eval instead of the compiled ELAN rules (REM Strategy.isEval,
# BORO); str_eval passes the strategy terms built at run time to str_eval2.
#
# Each programs/runtime_strategies/<case>.lgi applies one strategy term to the
# query; for every query below, the sequence of results of the compiled
# program (`result = t` lines) must equal the interpreter's (`elan -b`, one
# ` t end` line per result).
# Usage: test_runtime_strategies.sh PREFIX     (an installed ELAN with elanc)
PREFIX=${1:?usage: test_runtime_strategies.sh PREFIX}
HERE=$(cd "$(dirname "$0")" && pwd)
WORK=$HERE/work/runtime-strategies
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH
QUERIES="3 95 200"
fails=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }

rm -rf "$WORK"; mkdir -p "$WORK"
cp "$HERE"/programs/runtime_strategies/* "$WORK/"

for lgi in "$WORK"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  if ! ( cd "$WORK" && rm -rf a.out ".elan.$name" \
         && elanc -nosplit -quiet -strategy 1 "$name" >"$name.elanc.log" 2>&1 \
         && make -f "$name.make" >"$name.make.log" 2>&1 ); then
    fail "$name (elanc or make failed, see $WORK/$name.*.log)"; continue
  fi
  # the test must go through str_eval, not the compiled strat[int] rules
  if ! grep -q 'return str_eval(v0);' "$WORK/.elan.$name/$name.core.c"; then
    fail "$name (the generated code does not call str_eval)"; continue
  fi
  for q in $QUERIES; do
    expected=$( cd "$WORK" && printf '%s end\n' "$q" | elan -b "$name.lgi" 2>&1 \
                | sed -n 's/^ \(.*\) end$/\1/p' | grep -v '^end$' | tr '\n' ' ' )
    got=$( cd "$WORK" && printf '%s end\n' "$q" | ./a.out -quiet 2>&1; echo "status $?" )
    status=$(echo "$got" | sed -n 's/^status //p')
    got=$(echo "$got" | sed -n 's/^result = //p' | tr '\n' ' ')
    if [ "$status" = 0 ] && [ "$got" = "$expected" ]; then
      ok "$name $q: $expected"
    else
      fail "$name $q: compiled [$got] (status $status), interpreter [$expected]"
    fi
  done
done

[ "$fails" -eq 0 ] && rm -rf "$WORK"
echo "$fails failed"
[ "$fails" -eq 0 ]
