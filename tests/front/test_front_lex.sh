#!/bin/sh
# Acceptance of the Go lexer and preprocessor (S8a, tasks 5 and 7): for each
# file of list_files.py (src/lib, examples, tests; 619 at d9c73a8), in the
# file's directory, `elan-lextrace lex F` (the C++ oracle) and
# `front-trace lex F` (built here into the work directory); stdout compared
# by tracecmp.py --mode lex (positions included), stderr byte for byte,
# statuses equal. Files marked `loaded` (FOR EACH/IF) cannot be read
# standalone: they count as covered only when test_front_load.sh found them
# in a passing loader comparison (tests/front/work/loaded-files.txt), else
# they are listed as UNCOVERED and fail, unless unloadable.tsv says why no
# program elan loads can reach them (UNLOADABLE, reported).
# Usage: tests/front/test_front_lex.sh [PREFIX] [--only EXT,...]
#   PREFIX: an installed ELAN (default build/install); the harness is
#   $BUILD/src/interpreter/elan-lextrace, BUILD = dirname PREFIX unless set.
#   --only lgi,spc: only the files with these extensions.
#   FRONT_JOBS=n parallel runs (default: number of CPUs).
# The run fails unless it compared one result per standalone file and at
# least MIN_ALL files (MIN_LGI_SPC with --only lgi,spc, 1 with another
# --only), and, in a full run, covered at least MIN_COVERED loaded files.
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)
MIN_ALL=599 MIN_LGI_SPC=402 MIN_COVERED=15

if [ "${1:-}" = --one ]; then   # worker: --one RUN H FT N FILE
  RUN=$2 H=$3 FT=$4 n=$5 f=$6
  d=$REPO/$(dirname "$f"); b=$(basename "$f"); o=$RUN/out/$n
  ( cd "$d" && "$H" lex "$b" > "$o.c" 2> "$o.c.err" < /dev/null ); cs=$?
  ( cd "$d" && "$FT" lex "$b" > "$o.g" 2> "$o.g.err" < /dev/null ); gs=$?
  if python3 "$HERE/tracecmp.py" --mode lex "$o.c" "$o.g" --stderr "$o.c.err" "$o.g.err" \
       --status "$cs" "$gs" > "$o.cmp" 2>&1; then
    echo "ok $f" > "$RUN/res/$n"; rm -f "$o".*
  else
    { echo "FAIL $f"; head -30 "$o.cmp" | sed 's/^/    /'; } > "$RUN/res/$n"
  fi
  exit 0
fi

PREFIX=${1:-build/install}
case "$PREFIX" in /*) ;; *) PREFIX=$REPO/$PREFIX ;; esac
ONLY=
[ "${2:-}" = --only ] && ONLY=${3:?--only EXT,...}
BUILD=$(cd "${BUILD:-$(dirname "$PREFIX")}" && pwd) || exit 2   # absolute: used after cd
H=$BUILD/src/interpreter/elan-lextrace
[ -x "$H" ] || { echo "test_front_lex: $H not built"; exit 2; }
export ELANLIB=$PREFIX
JOBS=${FRONT_JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)}

mkdir -p "$HERE/work"
RUN=$(mktemp -d "$HERE/work/lex.XXXXXX") || exit 2
mkdir "$RUN/out" "$RUN/res"
FT=$RUN/front-trace
( cd "$REPO/src/compiler-go" && GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off \
    GOCACHE=${GOCACHE:-$BUILD/src/compiler-go/gocache} go build -o "$FT" ./front/cmd/front-trace ) || exit 2

python3 "$HERE/list_files.py" "$REPO" > "$RUN/files.tsv" || exit 2
if [ -n "$ONLY" ]; then
  pat=$(echo "$ONLY" | sed 's/,/|/g')
  grep -E "\.($pat)	" "$RUN/files.tsv" > "$RUN/sel.tsv"
else
  cp "$RUN/files.tsv" "$RUN/sel.tsv"
fi

TAB=$(printf '\t')
awk -F'\t' '$2 == "standalone" { print NR, $1 }' "$RUN/sel.tsv" \
  | sed "s|^|$0 --one $RUN $H $FT |" | xargs -P "$JOBS" -L 1 sh
total=0 ok=0 failed=0 covered=0 uncovered=0
for r in "$RUN"/res/*; do
  [ -f "$r" ] || continue
  total=$((total + 1))
  if head -1 "$r" | grep -q '^ok '; then ok=$((ok + 1)); else failed=$((failed + 1)); cat "$r"; fi
done
LOADED=$HERE/work/loaded-files.txt
unloadable=0
while IFS="$TAB" read -r f k; do
  [ "$k" = loaded ] || continue
  if [ -f "$LOADED" ] && grep -qxF "$f" "$LOADED"; then covered=$((covered + 1))
  elif grep -q "^$f$TAB" "$HERE/unloadable.tsv"; then echo "UNLOADABLE $f"; unloadable=$((unloadable + 1))
  else echo "UNCOVERED $f"; uncovered=$((uncovered + 1)); fi
done < "$RUN/sel.tsv"
echo "$total files: $ok ok, $failed failed; loaded files: $covered covered, $unloadable UNLOADABLE (unloadable.tsv), $uncovered UNCOVERED"
# The run must have compared something: one result per standalone file, and
# at least the S8a figures (599 standalone files, 402 with --only lgi,spc;
# 15 loaded files covered by test_front_load.sh in a full run).
nsel=$(awk -F'\t' '$2 == "standalone"' "$RUN/sel.tsv" | wc -l)
case "$ONLY" in
  "") min=$MIN_ALL ;;
  lgi,spc|spc,lgi) min=$MIN_LGI_SPC ;;
  *) min=1 ;;
esac
short=
[ "$total" -eq "$nsel" ] || short="$short $total results for $nsel standalone files;"
[ "$ok" -ge "$min" ] || short="$short ok $ok < $min;"
[ -n "$ONLY" ] || [ "$covered" -ge "$MIN_COVERED" ] || short="$short loaded files covered $covered < $MIN_COVERED;"
if [ -n "$short" ]; then echo "test_front_lex: FAIL: too little compared:$short"; failed=$((failed + 1)); fi
if [ $failed -eq 0 ] && [ $uncovered -eq 0 ]; then rm -rf "$RUN"; exit 0; fi
echo "work directory: $RUN"; exit 1
