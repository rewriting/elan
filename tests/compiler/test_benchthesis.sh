#!/bin/sh
# The BenchThesis applications of the 2000 compiler (ELAN 2.1), ported to
# ELAN 3 syntax in tests/compiler/benchthesis/ (see the README there):
# FastCompletion (Knuth-Bendix completion, 15 specifications) and MinelaComp
# (an ELAN-in-ELAN rewriting engine with proof terms, 6 specifications).
#
# Kinds, as in tests/legacy-bench/run_tests.py:
#   J   elanc-rem -b -nosplit -quiet LGI SPC; make -f LGI.make; ./a.out ...
#   JO  same with -optimiseChoicePoint
#   I   elan -b LGI.lgi SPC.spc < SAMPLES/INP.inp
#   G   elanc-go -v -b LGI SPC (go build cache $ELANC_GO_CACHE, default
#       tests/compiler/work/gocache, shared between runs), then as J; the
#       oracle is expected/J-OUT.out without its `rewrite_step = N` line
#       (the count depends on the compilation scheme, as in the bench); the
#       steps and the compile time of elanc-go are written on the result line
# A compiled case runs `./a.out -noInput -quiet` (the `start with` query of the
# program) when INP is `-`, else `./a.out -quiet < SAMPLES/INP.inp`.
#
# Oracle: benchthesis/<app>/expected/<KIND>-<OUT>.out, the stdout of the 2004
# reference system (reference/install) for the same commands, compared
# exactly (the historical outputs SAMPLES/<OUT>.out of 2000 differ in
# printing; the README records how).
#
# Usage: test_benchthesis.sh PREFIX     (an installed ELAN with elanc-rem)
#        BENCHTHESIS_KINDS="J JO I"  the kinds run (default J JO I; G is
#                            run by make check-go)
#        BENCHTHESIS_SKIP="name..."  cases not run (names APP-KIND-OUT as
#                            printed, e.g. MinelaComp-G-cappend)
#        BENCHTHESIS_JOBS=n  parallel cases (default: number of CPUs)
#        BENCHTHESIS_SAVE=1  write the outputs as the expected ones
#                            (only with PREFIX = the reference install)
PREFIX=${1:?usage: test_benchthesis.sh PREFIX}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
HERE=$(cd "$(dirname "$0")" && pwd)
SRC=$HERE/benchthesis
WORK=$HERE/work/benchthesis
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH

# one case: KIND APP LGI SPC INP OUT
if [ "$2" = --one ]; then
  shift 2
  kind=$1 app=$2 lgi=$3 spc=$4 inp=$5 out=$6
  name=$app-$kind-$out
  for s in $BENCHTHESIS_SKIP; do
    [ "$s" = "$name" ] && { echo "skip $name" > "$WORK/$name.result"; exit 0; }
  done
  wd=$WORK/$name
  rm -rf "$wd"; mkdir -p "$wd"; cp -R "$SRC/$app/." "$wd/"
  exec > "$WORK/$name.result"
  cd "$wd" || exit 1
  start=$(date +%s)
  case $kind in
    I) elan -b "$lgi.lgi" "$spc.spc" < "SAMPLES/$inp.inp" > got.out 2> got.err ;;
    J|JO)
      opt=; [ "$kind" = JO ] && opt=-optimiseChoicePoint
      if ! elanc-rem -b -nosplit -quiet $opt "$lgi" "$spc" > elanc.log 2>&1 \
         || ! make -f "$lgi.make" > make.log 2>&1 || [ ! -x a.out ]; then
        echo "FAIL $name (elanc-rem or make failed, see $wd/*.log)"; exit 0
      fi
      if [ "$inp" = - ]; then ./a.out -noInput -quiet > got.out 2> got.err
      else ./a.out -quiet < "SAMPLES/$inp.inp" > got.out 2> got.err; fi
      status=$?
      if [ "$status" != 0 ]; then echo "FAIL $name (a.out status $status)"; exit 0; fi ;;
    G)
      if ! elanc-go -v -b -nosplit -quiet --gocache "$GOCACHE_DIR" "$lgi" "$spc" > elanc-go.log 2>&1 \
         || [ ! -x a.out ]; then
        echo "FAIL $name (elanc-go failed: $(grep -m1 'not supported' elanc-go.log), see $wd/elanc-go.log)"; exit 0
      fi
      build=$(sed -n 's/^elanc-go: build time //p' elanc-go.log)
      if [ "$inp" = - ]; then ./a.out -noInput -quiet > got.full 2> got.err
      else ./a.out -quiet < "SAMPLES/$inp.inp" > got.full 2> got.err; fi
      status=$?
      if [ "$status" != 0 ]; then echo "FAIL $name (a.out status $status)"; exit 0; fi
      steps=$(sed -n 's/^rewrite_step = //p' got.full)
      grep -v '^rewrite_step = ' got.full > got.out ;;
  esac
  secs=$(( $(date +%s) - start ))
  expected=$SRC/$app/expected/$kind-$out.out
  if [ "$kind" = G ]; then
    grep -v '^rewrite_step = ' "$SRC/$app/expected/J-$out.out" > expected.out
    expected=$wd/expected.out
    jsteps=$(sed -n 's/^rewrite_step = //p' "$SRC/$app/expected/J-$out.out")
    secs="${secs}s, build $build, steps $steps (J $jsteps)"
  else
    secs=${secs}s
  fi
  if [ -n "$BENCHTHESIS_SAVE" ] && [ "$kind" != G ]; then
    mkdir -p "$SRC/$app/expected"; cp got.out "$expected"; echo "saved $name ($secs)"
  elif [ ! -f "$expected" ]; then
    echo "FAIL $name (missing $expected)"
  elif ! cmp -s "$expected" got.out; then
    echo "FAIL $name ($secs): differs from $expected"
    diff "$expected" got.out | head -5 | cut -c1-200
  elif grep -q 'runtime error:' got.err; then
    echo "FAIL $name ($secs): sanitizer report"; grep 'runtime error:' got.err | head -3
  else
    echo "ok   $name ($secs)"
  fi
  exit 0
fi

KINDS=${BENCHTHESIS_KINDS:-J JO I}
has_kind() { case " $KINDS " in *" $1 "*) return 0;; esac; return 1; }
cases() {
  for kind in J JO G; do
    has_kind $kind || continue
    for spc in KNZ86 Zeh89 curry exa79 exa80 exa85 expf expoclos furtin gdiv group p2 p8 sample taussky; do
      echo "$kind FastCompletion ans $spc - j$spc"
    done
    # rewritingC.lgi: the query is the pair [term,proofnil] read from stdin
    for c in append:cappend congruence:ccong or:cor peano:cpeano primes:cprimes4 primes:cprimes8 sample:csample; do
      echo "$kind MinelaComp rewritingC ${c%%:*} ${c#*:} ${c#*:}"
    done
  done
  has_kind I || return 0
  for spc in KNZ86 Zeh89 curry exa79 exa80 exa85 expf expoclos furtin gdiv group p2 sample taussky; do
    echo "I FastCompletion ans_completion $spc sat i$spc"
  done
  for c in append:iappend congruence:icong or:ior peano:ipeano primes:iprimes4 sample:isample; do
    echo "I MinelaComp rewriting ${c%%:*} ${c#*:} ${c#*:}"
  done
}

export GOCACHE_DIR=${ELANC_GO_CACHE:-$HERE/work/gocache}
JOBS=${BENCHTHESIS_JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)}
rm -rf "$WORK"; mkdir -p "$WORK"
start=$(date +%s)
cases | sed "s|^|$PREFIX --one |" | xargs -P "$JOBS" -L 1 "$0"
cat "$WORK"/*.result
fails=$(cat "$WORK"/*.result | grep -c '^FAIL')
echo "benchthesis: $(cat "$WORK"/*.result | grep -c '^ok') ok, $fails failed, $(( $(date +%s) - start ))s"
[ "$fails" -eq 0 ] && rm -rf "$WORK"
[ "$fails" -eq 0 ]
