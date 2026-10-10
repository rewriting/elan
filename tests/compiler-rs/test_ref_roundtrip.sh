#!/bin/sh
# Round trip of the .ref reader/printer (src/compiler-rs/elan-ref) on the
# exports of the legacy bench: for every program of a J case
# (tests/legacy-bench/run_tests.py), `elan --cexport` as elanc runs it, then
# the `roundtrip` example of elan-ref (parse, print, same tokens modulo white
# space and comments, reparse to the same model).
# Usage: test_ref_roundtrip.sh PREFIX     (an installed ELAN)
# Programs whose .lgi is missing are skipped (NOLGI in the bench). Programs
# that the interpreter rejects cannot be exported: they are listed as
# NOEXPORT and do not fail the test (today the 30 `ans` programs of
# elan3/applications/BCompletion and Compiler.2.1/BenchThesis/FastCompletion:
# `switch` rules and `rewrite` used as a name, ERROR/NOREF in the bench).
# The status is non-zero when a .ref does not round-trip.
# The exports stay in tests/compiler-rs/work/roundtrip.XXXXXX/exports.
PREFIX=${1:?usage: test_ref_roundtrip.sh PREFIX}
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH
export CARGO_TARGET_DIR=${CARGO_TARGET_DIR:-$REPO/build/src/compiler-rs/target}

mkdir -p "$HERE/work"
RUN=$(mktemp -d "$HERE/work/roundtrip.XXXXXX") || exit 2
mkdir "$RUN/exports" "$RUN/apps"
echo "work directory: $RUN"

( cd "$REPO/src/compiler-rs" && cargo build --offline --quiet -p elan-ref --example roundtrip ) || exit 2
ROUNDTRIP=$CARGO_TARGET_DIR/debug/examples/roundtrip

python3 "$HERE/list_j_programs.py" > "$RUN/programs.tsv" || exit 2

n=0 ok=0 failed=0 noexport=0
TAB=$(printf '\t')
while IFS="$TAB" read -r dir lgi spc; do
  n=$((n+1))
  # one private copy of each application directory (elan may write in it)
  app=$RUN/apps/$(printf '%s' "$dir" | sed "s|^$REPO/||; s|/|_|g")
  if [ ! -d "$app" ]; then
    cp -R "$dir" "$app" || exit 2
  fi
  ref=$RUN/exports/$n-$(basename "$dir")-$lgi-$spc.ref
  if [ "$spc" = no ]; then
    ( cd "$app" && elan --cexport "$ref" -b "$lgi.lgi" ) > "$ref.log" 2>&1 </dev/null
  else
    ( cd "$app" && elan --cexport "$ref" -b "$lgi.lgi" "$spc.spc" ) > "$ref.log" 2>&1 </dev/null
  fi
  if [ $? -ne 0 ] || [ ! -s "$ref" ]; then
    echo "NOEXPORT $dir $lgi $spc (see $ref.log)"
    noexport=$((noexport+1))
    continue
  fi
  if "$ROUNDTRIP" "$ref" > "$ref.roundtrip" 2>&1; then
    ok=$((ok+1))
  else
    cat "$ref.roundtrip"
    failed=$((failed+1))
  fi
done < "$RUN/programs.tsv"

echo "$n programs: $ok ok, $failed failed, $noexport not exported"
[ "$failed" -eq 0 ]
