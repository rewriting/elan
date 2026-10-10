#!/bin/sh
# Uniform measurement of the spike binaries: for each program and size, the
# best of N runs (real and user seconds) and the largest maximum RSS (MB).
# Usage: measure.sh [N]     (from spikes/codegen; default N=5)
N=${1:-5}
HERE=$(cd "$(dirname "$0")" && pwd)
OUT=$HERE/results.tsv
printf 'variant\tprogram\tsize\treal_s\tuser_s\trss_mb\n' > "$OUT"

run() { # run VARIANT PROGRAM SIZE BINARY [ENV]
  best_real=999; best_user=999; rss=0
  i=0
  while [ $i -lt "$N" ]; do
    if [ -n "$5" ]; then
      env "$5" /usr/bin/time -l "$4" "$3" > /dev/null 2> "$HERE/.time"
    else
      /usr/bin/time -l "$4" "$3" > /dev/null 2> "$HERE/.time"
    fi
    r=$(awk '/ real /{print $1}' "$HERE/.time")
    u=$(awk '/ real /{print $3}' "$HERE/.time")
    m=$(awk '/maximum resident set size/{printf "%.1f", $1/1048576}' "$HERE/.time")
    best_real=$(echo "$r $best_real" | awk '{print ($1<$2)?$1:$2}')
    best_user=$(echo "$u $best_user" | awk '{print ($1<$2)?$1:$2}')
    rss=$(echo "$m $rss" | awk '{print ($1>$2)?$1:$2}')
    i=$((i+1))
  done
  printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$1" "$2" "$3" "$best_real" "$best_user" "$rss" | tee -a "$OUT"
}

for spec in "nqueens 10" "nqueens 11" "nqueens 12" "efib 100" "efib 150" "efib 200" \
            "propc 2" "propc 3"; do
  set -- $spec
  prog=$1 size=$2
  run c-gc     "$prog" "$size" "$HERE/c/bin/$prog"
  run c-arena  "$prog" "$size" "$HERE/c/bin/$prog-arena"
  run rust     "$prog" "$size" "$HERE/rust/bin/$prog"
  run go       "$prog" "$size" "$HERE/go/bin/$prog"
  run go-1core "$prog" "$size" "$HERE/go/bin/$prog" GOMAXPROCS=1
done
