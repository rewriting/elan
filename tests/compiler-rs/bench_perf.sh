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
# (spikes/codegen/RESULTS.md, the S7c plan).
#
# Not part of `make check` (timings depend on the machine); run it by hand
# after a change of the runtime or of the generated code:
#
#   tests/compiler-rs/bench_perf.sh build/install [N]   (N runs, default 3)
#
# Environment: ELANC_GO_CACHE (the go build cache, default
# tests/compiler-rs/work/gocache); GOGC/GOMEMLIMIT are passed to the
# programs. The work directory tests/compiler-rs/work/perf-go is reused
# (overwritten) by every run.
PREFIX=${1:?usage: bench_perf.sh PREFIX [N]}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
N=${2:-3}
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

# case: name program start-term query-sort result-sort expected-steps spike REM
CASES='nqueens10 nqueens (queens_strat)queens(10) int list 18376901 0.03/0.01 -
nqueens11 nqueens (queens_strat)queens(11) int list 105918861 0.17/0.08 22.2
nqueens12 nqueens (queens_strat)queens(12) int list 653494329 0.97/0.46 -
efib100 efib ()go(100) bool eInt 10193 0.01 9.1
efib150 efib ()go(150) bool eInt 22793 0.03 -
efib200 efib ()go(200) bool eInt 40393 0.06 ~140
propc1 propc ()q1 Prop Prop 12841 - 0.21
propc2 propc ()q2 Prop Prop 218978 0.12 1.49
propc3 propc ()q3 Prop Prop 556880 0.68 9.7'

fails=0
printf '%-10s %8s %8s %8s %11s  %-9s %s\n' case real_s user_s rss_MB steps spike_s REM_s
while read -r name prog start sort rsort steps spike rem; do
  d=$RUN/$name
  mkdir -p "$d" || exit 2
  cp "$SPIKE/$prog.eln" "$d/" || exit 2
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
  printf '%-10s %8s %8s %8s %11s  %-9s %s%s\n' "$name" "$best_real" "$best_user" "$rss" "$got" "$spike" "$rem" "$mark"
done <<EOF
$CASES
EOF
[ "$fails" -eq 0 ]
