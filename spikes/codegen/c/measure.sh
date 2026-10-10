#!/bin/bash
# Runs each binary once under /usr/bin/time -l; prints a markdown table row per run.
cd "$(dirname "$0")"
T=$(mktemp -d)
echo "| run | real s | user s | sys s | max RSS MB | rewrite_step |"
echo "|---|---|---|---|---|---|"
for spec in "nqueens 8" "nqueens 10" "nqueens 11" "efib 50" "efib 100" "propc 1" "propc 2" "propc 3"; do
  set -- $spec
  for v in "" -arena; do
    /usr/bin/time -l ./bin/$1$v $2 > $T/o.txt 2> $T/t.txt
    st=$(awk '/rewrite_step/{print $3}' $T/o.txt)
    awk -v p="$1$v $2" -v s="$st" '/ real/{r=$1;u=$3;sy=$5} /maximum resident/{m=$1/1048576} END{printf "| %s | %s | %s | %s | %.1f | %s |\n",p,r,u,sy,m,s}' $T/t.txt
  done
done
