#!/bin/sh
# Performance of programs compiled by elanc-go (S7c task 3): the spike
# programs (spikes/codegen/programs: nqueens, efib, propc) with ground
# start terms, each built once and run 3 times (`./a.out -noInput -quiet`);
# the table gives the best real and user times, the largest maximum RSS,
# the rewrite steps (checked against the counts of elanc-go below, which
# count the builtin operations of nqueens unlike the spike: a
# different count is a FAIL) and the reference numbers of the spike
# (spikes/codegen/go-repr/NOTES.md: Go, generic nodes, weak table, default
# GOGC; the hybrid with native builtins for nqueens) and of REM
# (spikes/codegen/RESULTS.md, the S7c plan), and elanc-rs's time where it
# runs the program (Apple M4 Max). deeplist3m is the deep normalisation
# len(gen(3 x 10^6)) of programs/depth/depth.eln.
#
# Not part of `make check` (timings depend on the machine); run it by hand
# after a change of the runtime or of the generated code:
#
#   tests/compiler-rs/bench_perf.sh build/install [N] [--check]   (N runs, default 3)
#
# limit_s is criterion 4 of the S7 spec as a number (S7e task 1): 2x the Go
# spike figure (spikes/codegen/RESULTS.md) where there is one, else 1.5x the
# S7c final figure (plan S7c, Task 3). A case whose best real time exceeds
# its limit prints `SLOW <case>: <t> s > <limit> s`; with --check the exit
# status is then 1. Run it with --check, and tests/compiler-rs/bench_jg.py
# PREFIX --check (elanc-go against REM on the bench), after a change of the
# runtime or of the generated code.
#
# Environment: ELANC_GO_CACHE (the go build cache, default
# tests/compiler-rs/work/gocache); GOGC/GOMEMLIMIT are passed to the
# programs. The work directory tests/compiler-rs/work/perf-go is reused
# (overwritten) by every run.
CHECK=0 PREFIX= N=
for a in "$@"; do
  case $a in
    --check) CHECK=1 ;;
    *) if [ -z "$PREFIX" ]; then PREFIX=$a; else N=$a; fi ;;
  esac
done
[ -n "$PREFIX" ] || { echo "usage: bench_perf.sh PREFIX [N] [--check]" >&2; exit 2; }
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
N=${N:-3}
HERE=$(cd "$(dirname "$0")" && pwd)
SPIKE=$(cd "$HERE/../../spikes/codegen/programs" && pwd) || exit 2
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH LC_ALL=C
CACHE=${ELANC_GO_CACHE:-$HERE/work/gocache}
RUN=$HERE/work/perf-go
mkdir -p "$CACHE" "$RUN" || exit 2

TIME=${ELAN_TEST_TIME:-/usr/bin/time}
if "$TIME" -l true >/dev/null 2>&1; then TIMEFLAG=-l
elif "$TIME" -v true >/dev/null 2>&1; then TIMEFLAG=-v
else echo "bench_perf.sh: $TIME -l or -v needed" >&2; exit 2
fi

# case: name program start-term query-sort result-sort expected-steps limit spike REM elanc-rs
# limit (s): 2x the Go spike figure of RESULTS.md (nqueens 11/12: 0.20/1.20,
# efib 100/200: 0.01/0.07, propc 2/3: 0.13/0.64), else 1.5x S7c's final
# figure (nqueens 10: 0.03, efib 150: 0.04, propc 1: 0.01, deeplist3m: 1.25)
CASES='nqueens10 nqueens (queens_strat)queens(10) int list 18376901 0.05 0.03/0.01 - -
nqueens11 nqueens (queens_strat)queens(11) int list 105918861 0.40 0.17/0.08 22.2 -
nqueens12 nqueens (queens_strat)queens(12) int list 653494329 2.40 0.97/0.46 - 0.37
efib100 efib ()go(100) bool eInt 10193 0.02 0.01 9.1 -
efib150 efib ()go(150) bool eInt 22793 0.06 0.03 - -
efib200 efib ()go(200) bool eInt 40393 0.14 0.06 ~140 -
propc1 propc ()q1 Prop Prop 12841 0.02 - 0.21 -
propc2 propc ()q2 Prop Prop 218978 0.26 0.12 1.49 -
propc3 propc ()q3 Prop Prop 556880 1.28 0.68 9.7 -
deeplist3m depth ()len(gen(3000000)) bool int 15000003 1.9 - - 0.36'

fails=0 slow=
printf '%-10s %8s %8s %8s %11s %7s  %-9s %-6s %s\n' case real_s user_s rss_MB steps limit_s spike_s REM_s elanc-rs_s
while read -r name prog start sort rsort steps limit spike rem rs; do
  d=$RUN/$name
  mkdir -p "$d" || exit 2
  if [ "$prog" = depth ]; then cp "$HERE/programs/depth/depth.eln" "$d/" || exit 2
  else cp "$SPIKE/$prog.eln" "$d/" || exit 2
  fi
  strat="${start%%)*})"
  term=${start#*)}
  printf 'LPL %s description\n  query of sort %s\n  result of sort %s\n  import %s\n  start with %s %s\nend\n' \
    "$name" "$sort" "$rsort" "$prog" "$strat" "$term" > "$d/$name.lgi"
  if ! ( cd "$d" && elanc-go -b -nosplit -quiet --gocache "$CACHE" "$name" >elanc-go.log 2>&1 ); then
    echo "FAIL $name: elanc-go failed (see $d/elanc-go.log)"; fails=$((fails+1)); continue
  fi
  best_real=999999 best_user=999999 rss=0 got=
  i=0
  while [ $i -lt "$N" ]; do
    ( cd "$d" && "$TIME" $TIMEFLAG ./a.out -noInput -quiet >out 2>err ) || { echo "FAIL $name: status $? (see $d/err)"; fails=$((fails+1)); break; }
    got=$(sed -n 's/^rewrite_step = //p' "$d/out")
    case $TIMEFLAG in
      -l) r=$(awk '/ real /{print $1}' "$d/err"); u=$(awk '/ real /{print $3}' "$d/err")
          m=$(awk '/maximum resident set size/{printf "%.1f", $1/1048576}' "$d/err") ;;
      -v) r=$(awk -F': ' '/Elapsed \(wall clock\)/{n=split($2,a,":"); s=a[n]; if(n>1) s+=60*a[n-1]; print s}' "$d/err")
          u=$(awk -F': ' '/User time/{print $2}' "$d/err")
          m=$(awk -F: '/Maximum resident set size/{printf "%.1f", $2/1024}' "$d/err") ;;
    esac
    best_real=$(echo "$r $best_real" | awk '{print ($1<$2)?$1:$2}')
    best_user=$(echo "$u $best_user" | awk '{print ($1<$2)?$1:$2}')
    rss=$(echo "$m $rss" | awk '{print ($1>$2)?$1:$2}')
    i=$((i+1))
  done
  mark=
  if [ "$steps" != 0 ] && [ "$got" != "$steps" ]; then mark=" FAIL: steps $got, expected $steps"; fails=$((fails+1)); fi
  printf '%-10s %8s %8s %8s %11s %7s  %-9s %-6s %s%s\n' "$name" "$best_real" "$best_user" "$rss" "$got" "$limit" "$spike" "$rem" "$rs" "$mark"
  if [ "$best_real" != 999999 ] && awk -v t="$best_real" -v l="$limit" 'BEGIN { exit !(t > l) }'; then
    slow="${slow}SLOW $name: $best_real s > $limit s
"
  fi
done <<EOF
$CASES
EOF
if [ -n "$slow" ]; then
  printf '%s' "$slow"
  [ "$CHECK" = 1 ] && fails=$((fails+1))
fi
[ "$fails" -eq 0 ]
