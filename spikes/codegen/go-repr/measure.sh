#!/bin/sh
# Best of N runs (real, user) and largest max RSS (MiB), /usr/bin/time -l,
# for every Go variant under several GC settings, plus the first Go spike
# (../go/bin, strong table) and the Rust versions (default setting only).
# The rewrite_step values seen over the N runs are recorded (with the weak
# table and id-ordered AC they can vary with GC timing).
# Usage: measure.sh [N]  (default 5). Writes results.tsv here.
export LC_ALL=C
N=${1:-5}
HERE=$(cd "$(dirname "$0")" && pwd)
OUT=$HERE/results.tsv
TMP=${TMPDIR:-/tmp}/go-repr-measure.$$
mkdir -p "$TMP"
printf 'variant\tsetting\tprogram\tsize\treal_s\tuser_s\trss_mb\tsteps\n' > "$OUT"
run() { # run VARIANT SETTING PROGRAM SIZE BINARY
  best_real=999; best_user=999; rss=0; steps=""; i=0
  while [ $i -lt "$N" ]; do
    case $2 in
      default) /usr/bin/time -l "$5" "$4" > "$TMP/out" 2> "$TMP/time" ;;
      *) env $2 /usr/bin/time -l "$5" "$4" > "$TMP/out" 2> "$TMP/time" ;;
    esac
    r=$(awk '/ real /{print $1}' "$TMP/time" | tr , .)
    u=$(awk '/ real /{print $3}' "$TMP/time" | tr , .)
    m=$(awk '/maximum resident set size/{printf "%.1f", $1/1048576}' "$TMP/time")
    s=$(awk '/rewrite_step/{print $3}' "$TMP/out")
    best_real=$(echo "$r $best_real" | awk '{print ($1<$2)?$1:$2}')
    best_user=$(echo "$u $best_user" | awk '{print ($1<$2)?$1:$2}')
    rss=$(echo "$m $rss" | awk '{print ($1>$2)?$1:$2}')
    case " $steps " in *" $s "*) ;; *) steps="$steps $s" ;; esac
    i=$((i+1))
  done
  printf '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n' "$1" "$2" "$3" "$4" "$best_real" "$best_user" "$rss" "${steps# }" | tee -a "$OUT"
}
for spec in "nqueens 10" "nqueens 11" "nqueens 12" "efib 100" "efib 150" "efib 200" "propc 2" "propc 3"; do
  set -- $spec
  prog=$1 size=$2
  case $prog in
    nqueens) variants="gs g gn h u t" ;;
    *) variants="gs g t" ;;
  esac
  for setting in default GOGC=200 GOGC=400 GOMAXPROCS=1; do
    run go1 "$setting" $prog $size "$HERE/../go/bin/$prog"
    for v in $variants; do
      run $v "$setting" $prog $size "$HERE/bin/$v-$prog"
    done
  done
  run rust default $prog $size "$HERE/../rust/bin/$prog"
  [ -x "$HERE/../rust-enum/bin/$prog" ] && run rust-enum default $prog $size "$HERE/../rust-enum/bin/$prog"
done
rm -r "$TMP"
