#!/bin/sh
# The BenchThesis applications of the 2000 compiler (ELAN 2.1), ported to
# ELAN 3 syntax in tests/compiler/benchthesis/ (see the README there):
# FastCompletion (Knuth-Bendix completion, 15 specifications) and MinelaComp
# (an ELAN-in-ELAN rewriting engine with proof terms, 6 specifications).
#
# Kinds, as in tests/legacy-bench/run_tests.py:
#   J   elanc -b -nosplit -quiet LGI SPC; make -f LGI.make; ./a.out ...
#   JO  same with -optimiseChoicePoint
#   I   elan -b LGI.lgi SPC.spc < SAMPLES/INP.inp
# A compiled case runs `./a.out -noInput -quiet` (the `start with` query of the
# program) when INP is `-`, else `./a.out -quiet < SAMPLES/INP.inp`.
#
# Oracle: benchthesis/<app>/expected/<KIND>-<OUT>.out, the stdout of the 2004
# reference system (reference/install) for the same commands, compared
# exactly (the historical outputs SAMPLES/<OUT>.out of 2000 differ in
# printing; the README records how).
#
# Usage: test_benchthesis.sh PREFIX     (an installed ELAN with elanc)
#        BENCHTHESIS_JOBS=n  parallel cases (default: number of CPUs)
#        BENCHTHESIS_SAVE=1  write the outputs as the expected ones
#                            (only with PREFIX = the reference install)
PREFIX=${1:?usage: test_benchthesis.sh PREFIX}
HERE=$(cd "$(dirname "$0")" && pwd)
SRC=$HERE/benchthesis
WORK=$HERE/work/benchthesis
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH

# one case: KIND APP LGI SPC INP OUT
if [ "$2" = --one ]; then
  shift 2
  kind=$1 app=$2 lgi=$3 spc=$4 inp=$5 out=$6
  name=$app-$kind-$out
  wd=$WORK/$name
  rm -rf "$wd"; mkdir -p "$wd"; cp -R "$SRC/$app/." "$wd/"
  exec > "$WORK/$name.result"
  cd "$wd" || exit 1
  start=$(date +%s)
  case $kind in
    I) elan -b "$lgi.lgi" "$spc.spc" < "SAMPLES/$inp.inp" > got.out 2> got.err ;;
    J|JO)
      opt=; [ "$kind" = JO ] && opt=-optimiseChoicePoint
      if ! elanc -b -nosplit -quiet $opt "$lgi" "$spc" > elanc.log 2>&1 \
         || ! make -f "$lgi.make" > make.log 2>&1 || [ ! -x a.out ]; then
        echo "FAIL $name (elanc or make failed, see $wd/*.log)"; exit 0
      fi
      if [ "$inp" = - ]; then ./a.out -noInput -quiet > got.out 2> got.err
      else ./a.out -quiet < "SAMPLES/$inp.inp" > got.out 2> got.err; fi
      status=$?
      if [ "$status" != 0 ]; then echo "FAIL $name (a.out status $status)"; exit 0; fi ;;
  esac
  secs=$(( $(date +%s) - start ))
  expected=$SRC/$app/expected/$kind-$out.out
  if [ -n "$BENCHTHESIS_SAVE" ]; then
    mkdir -p "$SRC/$app/expected"; cp got.out "$expected"; echo "saved $name (${secs}s)"
  elif [ ! -f "$expected" ]; then
    echo "FAIL $name (missing $expected)"
  elif ! cmp -s "$expected" got.out; then
    echo "FAIL $name (${secs}s): differs from $expected"
    diff "$expected" got.out | head -5 | cut -c1-200
  elif grep -q 'runtime error:' got.err; then
    echo "FAIL $name (${secs}s): sanitizer report"; grep 'runtime error:' got.err | head -3
  else
    echo "ok   $name (${secs}s)"
  fi
  exit 0
fi

cases() {
  for kind in J JO; do
    for spc in KNZ86 Zeh89 curry exa79 exa80 exa85 expf expoclos furtin gdiv group p2 p8 sample taussky; do
      echo "$kind FastCompletion ans $spc - j$spc"
    done
    # rewritingC.lgi: the query is the pair [term,proofnil] read from stdin
    for c in append:cappend congruence:ccong or:cor peano:cpeano primes:cprimes4 primes:cprimes8 sample:csample; do
      echo "$kind MinelaComp rewritingC ${c%%:*} ${c#*:} ${c#*:}"
    done
  done
  for spc in KNZ86 Zeh89 curry exa79 exa80 exa85 expf expoclos furtin gdiv group p2 sample taussky; do
    echo "I FastCompletion ans_completion $spc sat i$spc"
  done
  for c in append:iappend congruence:icong or:ior peano:ipeano primes:iprimes4 sample:isample; do
    echo "I MinelaComp rewriting ${c%%:*} ${c#*:} ${c#*:}"
  done
}

JOBS=${BENCHTHESIS_JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)}
rm -rf "$WORK"; mkdir -p "$WORK"
start=$(date +%s)
cases | sed "s|^|$PREFIX --one |" | xargs -P "$JOBS" -L 1 "$0"
cat "$WORK"/*.result
fails=$(cat "$WORK"/*.result | grep -c '^FAIL')
echo "benchthesis: $(cat "$WORK"/*.result | grep -c '^ok') ok, $fails failed, $(( $(date +%s) - start ))s"
[ "$fails" -eq 0 ] && rm -rf "$WORK"
[ "$fails" -eq 0 ]
