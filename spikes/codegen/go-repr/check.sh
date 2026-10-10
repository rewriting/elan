#!/bin/sh
# Checks every variant against the oracle: nqueens result lines (order
# included) = work-current/nqueens/out{8,10,11}.txt and ../rust/bin/nqueens 12;
# efib values; propc = t. Prints the rewrite_step of each run.
cd "$(dirname "$0")"
TMP=${TMPDIR:-/tmp}/go-repr-check.$$
mkdir -p "$TMP"
fail=0
for n in 8 10 11 12; do
  if [ $n = 12 ]; then ../rust/bin/nqueens 12 | grep '^result' > "$TMP/exp"
  else grep '^result' ../work-current/nqueens/out$n.txt | sed 's/ *= */ = /' > "$TMP/exp"; fi
  for v in g gs gn h u t; do
    bin/$v-nqueens $n > "$TMP/out"
    if grep '^result' "$TMP/out" | cmp -s - "$TMP/exp"; then r=ok; else r=FAIL; fail=1; fi
    echo "nqueens $n $v $r $(tail -1 "$TMP/out")"
  done
done
for n in 50:11011074 100:35084101 150:60386349 200:83233826; do
  for v in g gs gn t; do
    bin/$v-efib ${n%:*} > "$TMP/out"
    if grep -q "^result = ${n#*:}\$" "$TMP/out"; then r=ok; else r=FAIL; fail=1; fi
    echo "efib ${n%:*} $v $r $(tail -1 "$TMP/out")"
  done
done
for q in 1 2 3; do
  for v in g gs gn t; do
    bin/$v-propc $q > "$TMP/out"
    if grep -q '^result = t$' "$TMP/out"; then r=ok; else r=FAIL; fail=1; fi
    echo "propc $q $v $r $(tail -1 "$TMP/out")"
  done
done
rm -r "$TMP"
[ $fail = 0 ] && echo "ALL OK" || echo "FAILURES"
