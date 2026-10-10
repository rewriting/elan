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
# `elanc -strategy 2` compiles instead the strategy terms known at compile
# time (REM StrategyEval: dc, dk, if-then-orelse, labels, defined strategies)
# and leaves the others to str_eval2. Every program is run at both levels.
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

rm -rf "$WORK"
for level in 1 2; do
 wd=$WORK/strategy$level
 mkdir -p "$wd"
 cp "$HERE"/programs/runtime_strategies/* "$wd/"
 for lgi in "$wd"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  label="-strategy $level $name"
  if ! ( cd "$wd" && rm -rf a.out ".elan.$name" \
         && elanc -nosplit -quiet -strategy $level "$name" >"$name.elanc.log" 2>&1 \
         && make -f "$name.make" >"$name.make.log" 2>&1 && [ -x a.out ] ); then
    fail "$label (elanc or make failed, see $wd/$name.*.log)"; continue
  fi
  # level 1 must go through str_eval, not the compiled strat[int] rules;
  # level 2 must compile the strategy term (StrategyEval)
  case $level in
    1) marker='return str_eval(v0);' ;;
    2) marker='- strategy term to compile' ;;
  esac
  if ! grep -q -e "$marker" "$wd/.elan.$name/$name.core.c"; then
    fail "$label (the generated code does not contain '$marker')"; continue
  fi
  for q in $QUERIES; do
    expected=$( cd "$wd" && printf '%s end\n' "$q" | elan -b "$name.lgi" 2>&1 \
                | sed -n 's/^ \(.*\) end$/\1/p' | grep -v '^end$' | tr '\n' ' ' )
    got=$( cd "$wd" && printf '%s end\n' "$q" | ./a.out -quiet 2>&1; echo "status $?" )
    status=$(echo "$got" | sed -n 's/^status //p')
    got=$(echo "$got" | sed -n 's/^result = //p' | tr '\n' ' ')
    if [ "$status" = 0 ] && [ "$got" = "$expected" ]; then
      ok "$label $q: $expected"
    else
      fail "$label $q: compiled [$got] (status $status), interpreter [$expected]"
    fi
  done
 done
done

[ "$fails" -eq 0 ] && rm -rf "$WORK"
echo "$fails failed"
[ "$fails" -eq 0 ]
