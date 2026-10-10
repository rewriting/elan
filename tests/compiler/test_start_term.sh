#!/bin/sh
# The `start with (S) t` query of a compiled program that reads its query
# from stdin: S is applied to t, where the variable `query` is bound to the
# term read, as the interpreter does (in 2004 the compiled program applied S
# to the term read and ignored t).
#
# Programs: programs/start_term/*.lgi (t contains `query` once, twice, not at
# all, or is `query` itself) and the MinelaComp application of BenchThesis
# (rewriting.lgi: `start with (s_rewrite) [query,proofnil]`).
# For each program and query, the results of the compiled program
# (`result = t` lines) must equal the interpreter's (`elan -b`, one ` t end`
# line per result).
# Usage: test_start_term.sh PREFIX     (an installed ELAN with elanc)
PREFIX=${1:?usage: test_start_term.sh PREFIX}
HERE=$(cd "$(dirname "$0")" && pwd)
WORK=$HERE/work/start-term
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH
fails=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }

# check DIR NAME LGI SPC INPUT: compile LGI (with SPC, may be empty) in DIR,
# run it on INPUT and compare with the interpreter
check() {
  dir=$1 label=$2 lgi=$3 spc=$4 input=$5
  if ! ( cd "$dir" && rm -rf a.out ".elan.$lgi" \
         && elanc -b -nosplit -quiet "$lgi" $spc >"$lgi.elanc.log" 2>&1 \
         && make -f "$lgi.make" >"$lgi.make.log" 2>&1 ); then
    fail "$label (elanc or make failed, see $dir/$lgi.*.log)"; return
  fi
  spcfile=; [ -n "$spc" ] && spcfile=$spc.spc
  expected=$( cd "$dir" && printf '%s\n' "$input" | elan -b "$lgi.lgi" $spcfile 2>&1 \
              | sed -n 's/^ *\(.*\) end$/\1/p' | grep -v '^end$' | tr '\n' ' ' )
  got=$( cd "$dir" && printf '%s\n' "$input" | ./a.out -quiet 2>&1; echo "status $?" )
  status=$(echo "$got" | sed -n 's/^status //p')
  got=$(echo "$got" | sed -n 's/^result = //p' | tr '\n' ' ')
  if [ "$status" = 0 ] && [ "$got" = "$expected" ]; then
    ok "$label: [$expected]"
  else
    fail "$label: compiled [$got] (status $status), interpreter [$expected]"
  fi
}

rm -rf "$WORK"; mkdir -p "$WORK/programs" "$WORK/minela"
cp "$HERE"/programs/start_term/* "$WORK/programs/"
cp -R "$HERE"/benchthesis/MinelaComp/. "$WORK/minela/"

for lgi in "$WORK"/programs/*.lgi; do
  name=$(basename "$lgi" .lgi)
  for q in 5 7; do
    check "$WORK/programs" "$name $q" "$name" "" "$q end"
  done
done
for spc in peano append; do
  check "$WORK/minela" "MinelaComp rewriting $spc" rewriting "$spc" \
        "$(cat "$WORK/minela/SAMPLES/i$spc.inp")"
done

[ "$fails" -eq 0 ] && rm -rf "$WORK"
echo "$fails failed"
[ "$fails" -eq 0 ]
