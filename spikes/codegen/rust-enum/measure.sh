#!/bin/sh
# Best of N runs (real, user) and largest max RSS (MB), /usr/bin/time -l,
# for rust-enum and the first rust version (../rust/bin), same session.
# Usage: measure.sh [N]   (default 5). Writes results.tsv here.
export LC_ALL=C
N=${1:-5}
HERE=$(cd "$(dirname "$0")" && pwd)
OUT=$HERE/results.tsv
TMP=$HERE/.time
printf 'variant\tprogram\tsize\treal_s\tuser_s\trss_mb\n' > "$OUT"
run() { # run VARIANT PROGRAM SIZE BINARY
  best_real=999; best_user=999; rss=0; i=0
  while [ $i -lt "$N" ]; do
    /usr/bin/time -l "$4" "$3" > /dev/null 2> "$TMP"
    r=$(awk '/ real /{print $1}' "$TMP" | tr , .)
    u=$(awk '/ real /{print $3}' "$TMP" | tr , .)
    m=$(awk '/maximum resident set size/{printf "%.1f", $1/1048576}' "$TMP")
    best_real=$(echo "$r $best_real" | awk '{print ($1<$2)?$1:$2}')
    best_user=$(echo "$u $best_user" | awk '{print ($1<$2)?$1:$2}')
    rss=$(echo "$m $rss" | awk '{print ($1>$2)?$1:$2}')
    i=$((i+1))
  done
  printf '%s\t%s\t%s\t%s\t%s\t%s\n' "$1" "$2" "$3" "$best_real" "$best_user" "$rss" | tee -a "$OUT"
}
for spec in "nqueens 11" "nqueens 12" "propc 2" "propc 3"; do
  set -- $spec
  run rust      "$1" "$2" "$HERE/../rust/bin/$1"
  run rust-enum "$1" "$2" "$HERE/bin/$1"
done
rm -f "$TMP"
