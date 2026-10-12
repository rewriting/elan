#!/bin/sh
# Spike programs (bench_perf.sh cases) compiled by REM without and with -O
# (choice-point optimisation) and by elanc-go: best real time of N runs
# (one run when the first takes more than 10 s), max RSS, rewrite steps.
# Usage: spike_rem.sh PREFIX [N] > spike.tsv   (work dir: $SPIKE_REM_WORK)
PREFIX=$(cd "${1:?usage: spike_rem.sh PREFIX [N]}" && pwd) || exit 2
N=${2:-3}
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../../.." && pwd)
SPIKE=$REPO/spikes/codegen/programs
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH LC_ALL=C
RUN=${SPIKE_REM_WORK:-$REPO/tests/compiler-rs/work/spike-rem}
CACHE=$RUN/gocache
mkdir -p "$RUN" "$CACHE"

CASES='nqueens10 nqueens (queens_strat)queens(10) int list
nqueens11 nqueens (queens_strat)queens(11) int list
nqueens12 nqueens (queens_strat)queens(12) int list
efib100 efib ()go(100) bool eInt
efib150 efib ()go(150) bool eInt
efib200 efib ()go(200) bool eInt
propc1 propc ()q1 Prop Prop
propc2 propc ()q2 Prop Prop
propc3 propc ()q3 Prop Prop'

printf 'case\tvariant\treal_s\tuser_s\trss_mb\tsteps\n'
echo "$CASES" | while read -r name prog start sort rsort; do
  strat="${start%%)*})"; term=${start#*)}
  for v in J JO G; do
    d=$RUN/$name-$v
    rm -rf "$d"; mkdir -p "$d"; cp "$SPIKE/$prog.eln" "$d/"
    printf 'LPL %s description\n  query of sort %s\n  result of sort %s\n  import %s\n  start with %s %s\nend\n' \
      "$name" "$sort" "$rsort" "$prog" "$strat" "$term" > "$d/$name.lgi"
    case $v in
      J)  ( cd "$d" && elanc-rem -b -nosplit -quiet "$name" && make -f "$name.make" ) >"$d/build.log" 2>&1 ;;
      JO) ( cd "$d" && elanc-rem -b -nosplit -quiet -O "$name" && make -f "$name.make" ) >"$d/build.log" 2>&1 ;;
      G)  ( cd "$d" && elanc-go -b -nosplit -quiet --gocache "$CACHE" "$name" ) >"$d/build.log" 2>&1 ;;
    esac
    [ -x "$d/a.out" ] || { printf '%s\t%s\tBUILD-ERROR\t-\t-\t-\n' "$name" "$v"; continue; }
    best=999999 bestu=999999 rss=0 i=0
    while [ $i -lt "$N" ]; do
      ( cd "$d" && /usr/bin/time -l ./a.out -noInput -quiet >out 2>err )
      r=$(awk '/ real /{print $1}' "$d/err"); u=$(awk '/ real /{print $3}' "$d/err")
      m=$(awk '/maximum resident set size/{printf "%.1f", $1/1048576}' "$d/err")
      best=$(echo "$r $best" | awk '{print ($1<$2)?$1:$2}')
      bestu=$(echo "$u $bestu" | awk '{print ($1<$2)?$1:$2}')
      rss=$(echo "$m $rss" | awk '{print ($1>$2)?$1:$2}')
      i=$((i+1))
      awk -v t="$r" 'BEGIN { exit !(t > 10) }' && break
    done
    steps=$(sed -n 's/^rewrite_step = //p' "$d/out")
    printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$name" "$v" "$best" "$bestu" "$rss" "$steps"
  done
done
