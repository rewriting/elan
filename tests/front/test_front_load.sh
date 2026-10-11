#!/bin/sh
# Acceptance of the Go loader skeleton (S8a, task 8): for each program of
# list_programs.py, in a private copy of its directory ($SECONDELANLIB is the
# original parent directory, so the `..` lookup of the C++ is unchanged):
#   elan --export /dev/null ARGS (no -b): the lines "  handling module N" /
#       "  end of N" of stdout are the C++ module tree; a status != 0 is
#       NOLOAD (reported, not compared: the first error is in a module body);
#   elan-lextrace load TRACE --export /dev/null -b ARGS: the C++ trace;
#   front-trace load --tree --trace TRACE ARGS: the Go tree and trace.
# The trees are compared after removing the strconc[...] subtrees of the C++
# side (counted), the traces by tracecmp.py --mode load (FOR EACH bodies and
# strconc streams of the C++ side excluded, counted; positions dropped), the
# statuses of elan and front-trace must be equal, the harness's status 0, and
# so must stderr, byte for byte, except elan's warnings about module bodies
# (S8b: Earley ambiguities, the 'semantic' option of an operator; matched by
# their exact shape, removed on the C++ side only and counted). Files of passing programs are written, as paths relative
# to the repository, to tests/front/work/loaded-files.txt (test_front_lex.sh
# counts the `loaded` files found there as covered).
# The run fails unless it compared at least MIN_OK programs, with at most
# MAX_NOLOAD NOLOAD, MIN_STRCONC strconc subtrees and MIN_FOREACH FOR EACH
# regions removed, and one result per program (the S8a figures).
# Usage: tests/front/test_front_load.sh PREFIX     (FRONT_JOBS=n: parallel runs)
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)
TREE='^ +(handling module|end of) '
MIN_OK=538 MAX_NOLOAD=30 MIN_STRCONC=102 MIN_FOREACH=371

if [ "${1:-}" = --one ]; then   # worker: --one RUN H FT N KIND DIR LGI SPC
  RUN=$2 H=$3 FT=$4 n=$5 kind=$6 dir=$7 lgi=$8 spc=$9
  name="$kind $dir $lgi $spc"
  app=$RUN/apps/$n o=$RUN/out/$n
  cp -R "$REPO/$dir" "$app" || { echo "FAIL $name: copy" > "$RUN/res/$n"; exit 0; }
  set -- "$lgi.lgi"; [ "$spc" != - ] && set -- "$lgi.lgi" "$spc.spc"
  export SECONDELANLIB=$REPO/$dir/..
  ( cd "$app" && elan --export /dev/null "$@" ) > "$o.elan" 2> "$o.elan.err" < /dev/null; es=$?
  if [ $es -ne 0 ]; then
    echo "NOLOAD $name (elan status $es)" > "$RUN/res/$n"; rm -rf "$app" "$o".*; exit 0
  fi
  ( cd "$app" && "$H" load "$o.c" --export /dev/null -b "$@" ) > /dev/null 2> "$o.c.err" < /dev/null; hs=$?
  ( cd "$app" && "$FT" load --tree --trace "$o.g" "$@" ) > "$o.go" 2> "$o.go.err" < /dev/null; gs=$?
  [ -f "$o.g" ] || : > "$o.g"
  # C++ tree without the strconc subtrees
  grep -E "$TREE" "$o.elan" | awk '
    s { if ($0 ~ /handling module /) s++; else if ($0 ~ /end of /) s--; next }
    /handling module strconc\[/ { s = 1; n++; next }
    { print }
    END { print n + 0 > "/dev/stderr" }' > "$o.ctree" 2> "$o.nstrconc"
  grep -E "$TREE" "$o.go" > "$o.gtree"
  # elan's stderr without the warnings of module bodies (S8b), counted: an
  # Earley ambiguity (empty line, header, the string, `between operators :`,
  # the operators, each line starting with a tab) or the 'semantic' option of
  # an operator (empty line, position line, message)
  python3 - "$o.elan.err" "$o.elan.err.cmp" "$o.nbodywarn" <<'EOF'
import re, sys
text = open(sys.argv[1], encoding="latin-1").read()
text, n1 = re.subn(r"\n\[warning\]earley: ambiguity can't be solved in string:\n[^\n]*\n"
                   r"between operators :\n(?:\t[^\n]*\n)*", "", text)
text, n2 = re.subn(r"\n\[warning\] :file=[^\n]*\n"
                   r"\[warning\] using the 'semantic' option in user's module\n", "", text)
open(sys.argv[2], "w", encoding="latin-1").write(text)
open(sys.argv[3], "w").write("%d\n" % (n1 + n2))
EOF
  msg=
  if [ $hs -ne 0 ]; then msg="harness status $hs ($(head -c 200 "$o.c.err"))"
  elif [ $es -ne $gs ]; then msg="status: elan $es, front-trace $gs ($(head -c 200 "$o.go.err"))"
  elif ! cmp -s "$o.ctree" "$o.gtree"; then msg="module tree differs: $(diff "$o.ctree" "$o.gtree" | head -6 | tr '\n' ' ')"
  elif ! python3 "$HERE/tracecmp.py" --mode load "$o.c" "$o.g" > "$o.cmp" 2>&1; then
    msg="trace differs:
$(head -30 "$o.cmp" | sed 's/^/    /')"
  elif ! cmp -s "$o.elan.err.cmp" "$o.go.err"; then
    msg="stderr differs: $(diff "$o.elan.err.cmp" "$o.go.err" | head -6 | tr '\n' ' ')"
  fi
  if [ -n "$msg" ]; then
    echo "FAIL $name: $msg" > "$RUN/res/$n"; rm -rf "$app"; exit 0
  fi
  # files of this program, as the C++ lookup finds them (ichstream.cc)
  grep '^@begin ' "$o.c" | sed 's/^@begin //' | sort -u | while read -r m; do
    for p in "$REPO/$dir/$m" "$REPO/$dir/../$m" "$REPO/src/lib/elanlib/common/$m" \
             "$REPO/src/lib/elanlib/noquote/$m" "$REPO/src/lib/elanlib/strategy/$m" \
             "$REPO/src/lib/elanlib/ref/$m"; do
      if [ -f "$p" ] && ls "$(dirname "$p")" | grep -qxF "$m"; then
        python3 -c 'import os,sys; print(os.path.relpath(os.path.realpath(sys.argv[1]), sys.argv[2]))' "$p" "$REPO"
        break
      fi
    done
  done > "$RUN/files/$n"
  echo "ok $name $(tail -1 "$o.cmp") strconc-tree=$(cat "$o.nstrconc") body-warnings=$(cat "$o.nbodywarn")" > "$RUN/res/$n"
  rm -rf "$app" "$o".*
  exit 0
fi

PREFIX=${1:?usage: test_front_load.sh PREFIX}
case "$PREFIX" in /*) ;; *) PREFIX=$REPO/$PREFIX ;; esac
BUILD=${BUILD:-$(dirname "$PREFIX")}
H=$BUILD/src/interpreter/elan-lextrace
[ -x "$H" ] || { echo "test_front_load: $H not built"; exit 2; }
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH
JOBS=${FRONT_JOBS:-$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)}

mkdir -p "$HERE/work"
RUN=$(mktemp -d "$HERE/work/load.XXXXXX") || exit 2
mkdir "$RUN/apps" "$RUN/out" "$RUN/res" "$RUN/files"
FT=$RUN/front-trace
( cd "$REPO/src/compiler-go" && GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off \
    GOCACHE=${GOCACHE:-$BUILD/src/compiler-go/gocache} go build -o "$FT" ./front/cmd/front-trace ) || exit 2
python3 "$HERE/list_programs.py" "$REPO" > "$RUN/programs.tsv" || exit 2

awk -F'\t' '{ print NR, $1, $2, $3, $4 }' "$RUN/programs.tsv" \
  | sed "s|^|$0 --one $RUN $H $FT |" | xargs -P "$JOBS" -L 1 sh
total=0 ok=0 failed=0 noload=0 foreach=0 strconc=0 bodywarn=0
for r in "$RUN"/res/*; do
  total=$((total + 1))
  case $(head -1 "$r") in
    ok*) ok=$((ok + 1))
         foreach=$((foreach + $(sed -n 's/.* foreach=\([0-9]*\).*/\1/p' "$r")))
         strconc=$((strconc + $(sed -n 's/.* strconc-tree=\([0-9]*\).*/\1/p' "$r")))
         bodywarn=$((bodywarn + $(sed -n 's/.* body-warnings=\([0-9]*\).*/\1/p' "$r"))) ;;
    NOLOAD*) noload=$((noload + 1)); cat "$r" ;;
    *) failed=$((failed + 1)); cat "$r" ;;
  esac
done
cat "$RUN"/files/* 2>/dev/null | sort -u > "$HERE/work/loaded-files.txt"
echo "$total programs: $ok ok, $failed failed, $noload NOLOAD; strconc subtrees removed: $strconc; FOR EACH regions excluded: $foreach; body warnings excluded from stderr: $bodywarn"
# The run must have compared something: a broken ELANLIB or elan makes every
# program NOLOAD, which compares nothing (minimums: the figures of 4785698).
nprog=$(wc -l < "$RUN/programs.tsv")
short=
[ "$total" -eq "$nprog" ] || short="$short $total results for $nprog programs;"
[ "$ok" -ge "$MIN_OK" ] || short="$short ok $ok < $MIN_OK;"
[ "$noload" -le "$MAX_NOLOAD" ] || short="$short NOLOAD $noload > $MAX_NOLOAD;"
[ "$strconc" -ge "$MIN_STRCONC" ] || short="$short strconc subtrees $strconc < $MIN_STRCONC;"
[ "$foreach" -ge "$MIN_FOREACH" ] || short="$short FOR EACH regions $foreach < $MIN_FOREACH;"
if [ -n "$short" ]; then
  echo "test_front_load: FAIL: too little compared:$short (ELANLIB=$ELANLIB)"; failed=$((failed + 1))
fi
if [ $failed -eq 0 ]; then rm -rf "$RUN"; exit 0; fi
echo "work directory: $RUN"; exit 1
