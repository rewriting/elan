#!/bin/sh
# Deep recursions in programs compiled by a new compiler, $ELANC: elanc-rs
# (default; spec S6 D3) or elanc-go (spec S7, G4): a program never
# overflows its stack. Either the computation succeeds or the program
# stops with
#   elan: recursion too deep (stack of 1024 MiB exhausted); ...
# on the standard error and the exit status 3 (elan-runtime/src/depth.rs,
# elanrt/depth.go).
#
# Programs: programs/depth/<name>.lgi (start terms),
# the modules being the .eln files of the directory; each has a
# <name>.expect, lines:
#   status N        exit status of `./a.out -noInput -quiet` (0 or 3)
#   result T        the results, in order (`result = T` lines; none when
#                   there is no `result` line)
#   stderr TEXT     a text the standard error contains
#   maxrss MB       maximum resident set size, measured by /usr/bin/time
#                   (BSD/macOS `-l`, GNU `-v`; $ELAN_TEST_TIME overrides
#                   the path): the program fails when it cannot be measured
#   small BIG SMALL [BIG2 SMALL2...]
#                   the program with BIG replaced by SMALL (and BIG2 by
#                   SMALL2...) in its start term
#                   is compiled too and its results compared with the
#                   interpreter's (`elan -b`); the interpreter cannot run the large sizes
#                   (it overflows its stack or is quadratic: tail, ndrec
#                   and where at 10^5 end with a segmentation fault)
#   query TEXT      the query given to the interpreter (default `true end`;
#                   ignored, but it must parse)
#   compilers C...  the compilers that run the program (default elanc-rs):
#                   all list elanc-go but those with strategy terms built
#                   at run time (S7b task 2 of elanc-go)
# Lines starting with `#` are comments.
#
# Usage: [ELANC=elanc-go] test_depth.sh PREFIX [name...]   (an installed
# ELAN with the compiler; the names select programs, default all)
# The work directory is tests/compiler-rs/work/depth.XXXXXX for elanc-rs,
# depth-go.XXXXXX for elanc-go (kept, git-ignored); the build cache is
# shared between runs: the cargo target directory $ELANC_RS_TARGET_DIR,
# default tests/compiler-rs/work/target, or the go build cache
# $ELANC_GO_CACHE, default tests/compiler-rs/work/gocache. Prints the time
# and RSS of each run; the status is non-zero on any failure.
PREFIX=${1:?usage: test_depth.sh PREFIX [name...]}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
HERE=$(cd "$(dirname "$0")" && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH LC_ALL=C
ELANC=${ELANC:-elanc-rs}
case $ELANC in
  elanc-rs) TARGET=${ELANC_RS_TARGET_DIR:-$HERE/work/target} CACHE_OPT=--target-dir SUFFIX= ;;
  elanc-go) TARGET=${ELANC_GO_CACHE:-$HERE/work/gocache} CACHE_OPT=--gocache SUFFIX=-go ;;
  *) echo "unknown compiler ELANC=$ELANC (elanc-rs or elanc-go)" >&2; exit 2 ;;
esac
shift
PROGS=$HERE/programs/depth
SELECTION="$*"

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/depth$SUFFIX.XXXXXX") || exit 2
echo "work directory: $RUN ($ELANC)"

# how /usr/bin/time reports the maximum RSS
TIME=${ELAN_TEST_TIME:-/usr/bin/time}
TIMEFLAG=
if "$TIME" -l true 2>/dev/null >/dev/null; then TIMEFLAG=-l
elif "$TIME" -v true 2>/dev/null >/dev/null; then TIMEFLAG=-v
fi

fails=0 n=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }

selected() {  # selected NAME: NAME is one of the names given
  for s in $SELECTION; do [ "$s" = "$1" ] && return 0; done
  return 1
}

# results on one line: `a | b | c`
oneline() { tr '\n' '|' | sed -e 's/|$//' -e 's/|/ | /g'; }

# build DIR NAME: the compiler in DIR (holding NAME.lgi and the modules)
build() {
  ( cd "$1" && $ELANC -v -b -nosplit -quiet $CACHE_OPT "$TARGET" "$2" >$ELANC.log 2>&1 )
}

for lgi in "$PROGS"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  [ -z "$SELECTION" ] || selected "$name" || continue
  exp=$PROGS/$name.expect
  if [ ! -f "$exp" ]; then fail "$name: no $name.expect"; continue; fi
  compilers=$(sed -n 's/^compilers //p' "$exp"); compilers=${compilers:-elanc-rs}
  case " $compilers " in
    *" $ELANC "*) ;;
    *) echo "skip $name (not for $ELANC: compilers $compilers)"; continue ;;
  esac
  n=$((n+1))
  d=$RUN/$name
  mkdir "$d" "$d/big" || exit 2
  cp "$lgi" "$PROGS"/*.eln "$d/big/" || exit 2
  want_status=$(sed -n 's/^status //p' "$exp")
  want_results=$(sed -n 's/^result //p' "$exp" | oneline)
  want_stderr=$(sed -n 's/^stderr //p' "$exp")
  maxrss=$(sed -n 's/^maxrss //p' "$exp")
  small=$(sed -n 's/^small //p' "$exp")
  query=$(sed -n 's/^query //p' "$exp"); query=${query:-true end}

  # the large size: status, message, results, RSS
  if ! build "$d/big" "$name"; then
    fail "$name: $ELANC failed (see $d/big/$ELANC.log)"; continue
  fi
  if [ -n "$TIMEFLAG" ]; then
    ( cd "$d/big" && "$TIME" $TIMEFLAG ./a.out -noInput -quiet >out 2>err ); status=$?
  else
    ( cd "$d/big" && ./a.out -noInput -quiet >out 2>err ); status=$?
  fi
  results=$(sed -n 's/^result = //p' "$d/big/out" | oneline)
  case $TIMEFLAG in
    -l) rss=$(awk '/maximum resident set size/ { print int($1 / 1048576) }' "$d/big/err")
        secs=$(awk '/ real / { print $1 }' "$d/big/err") ;;
    -v) rss=$(awk -F: '/Maximum resident set size/ { print int($2 / 1024) }' "$d/big/err")
        secs=$(awk -F': ' '/Elapsed \(wall clock\)/ { print $2 }' "$d/big/err") ;;
    *)  rss= secs= ;;
  esac
  info="status $status, ${secs:-?} s, ${rss:-?} MB"
  if [ "$status" != "$want_status" ]; then
    fail "$name: status $status, expected $want_status (see $d/big/err)"
  elif [ "$results" != "$want_results" ]; then
    fail "$name: results [$results], expected [$want_results]"
  elif [ -n "$want_stderr" ] && ! grep -qF "$want_stderr" "$d/big/err"; then
    fail "$name: the standard error does not contain '$want_stderr' (see $d/big/err)"
  elif grep -q 'overflowed its stack\|Segmentation\|panicked\|fatal error\|^goroutine ' "$d/big/err"; then
    fail "$name: crash (see $d/big/err)"
  elif [ -n "$maxrss" ] && [ -z "$rss" ]; then
    fail "$name: maxrss $maxrss expected but the RSS cannot be measured ($TIME -l or -v missing: install GNU time)"
  elif [ -n "$maxrss" ] && [ "$rss" -gt "$maxrss" ]; then
    fail "$name: $rss MB of RSS, expected at most $maxrss MB"
  else
    ok "$name: $info [$results]"
  fi

  # a small size vs the interpreter
  [ -n "$small" ] || continue
  set -- $small
  label=$2 subst=
  while [ $# -ge 2 ]; do subst="$subst -e /start.with/s/$1/$2/"; shift 2; done
  mkdir "$d/small" "$d/int" || exit 2
  for w in small int; do
    sed $subst "$lgi" >"$d/$w/$name.lgi" || exit 2
    cp "$PROGS"/*.eln "$d/$w/" || exit 2
  done
  if ! build "$d/small" "$name"; then
    fail "$name ($label): $ELANC failed (see $d/small/$ELANC.log)"; continue
  fi
  rs=$(cd "$d/small" && ./a.out -noInput -quiet 2>&1 | sed -n 's/^result = *//p' | oneline)
  iout=$( cd "$d/int" && printf '%s\n' "$query" | elan -b "$name.lgi" 2>&1; echo "status $?" )
  istatus=$(printf '%s\n' "$iout" | sed -n 's/^status //p')
  int=$(printf '%s\n' "$iout" | sed -n 's/^ \(.*\) end$/\1/p' | grep -v '^end$' | oneline)
  if [ "$istatus" != 0 ] || printf '%s\n' "$iout" | grep -q '^\[error\]'; then
    fail "$name ($label): the interpreter failed (status $istatus)"
  elif [ "$rs" = "$int" ]; then
    ok "$name ($label) vs interpreter: [$rs]"
  else
    fail "$name ($label) vs interpreter: $ELANC [$rs], interpreter [$int]"
  fi
done

echo "$n programs, $fails failures"
[ "$fails" -eq 0 ]
