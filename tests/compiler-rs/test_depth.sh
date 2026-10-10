#!/bin/sh
# Deep recursions in programs compiled by the Rust compiler (elanc-rs, spec
# S6 D3): a program never overflows its stack. Either the computation
# succeeds or the program stops with
#   elan: recursion too deep (stack of 1024 MiB exhausted); ...
# on the standard error and the exit status 3 (elan-runtime/src/depth.rs).
#
# Programs: programs/depth/<name>.lgi (start terms),
# the modules being the .eln files of the directory; each has a
# <name>.expect, lines:
#   status N        exit status of `./a.out -noInput -quiet` (0 or 3)
#   result T        the results, in order (`result = T` lines; none when
#                   there is no `result` line)
#   stderr TEXT     a text the standard error contains
#   maxrss MB       maximum resident set size (checked when /usr/bin/time
#                   measures it: BSD/macOS `-l`, GNU `-v`)
#   small BIG SMALL the program with BIG replaced by SMALL in its start term
#                   is compiled too and its results compared with the
#                   interpreter's (`elan -b`); the interpreter cannot run the large sizes
#                   (it overflows its stack or is quadratic: tail, ndrec
#                   and where at 10^5 end with a segmentation fault)
#   query TEXT      the query given to the interpreter (default `true end`;
#                   ignored, but it must parse)
# Lines starting with `#` are comments.
#
# Usage: test_depth.sh PREFIX [name...]   (an installed ELAN with elanc-rs;
# the names select programs, default all)
# The work directory is tests/compiler-rs/work/depth.XXXXXX (kept,
# git-ignored); the cargo target directory is shared between runs:
# $ELANC_RS_TARGET_DIR, default tests/compiler-rs/work/target. Prints the
# time and RSS of each run; the status is non-zero on any failure.
PREFIX=${1:?usage: test_depth.sh PREFIX [name...]}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
HERE=$(cd "$(dirname "$0")" && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH LC_ALL=C
TARGET=${ELANC_RS_TARGET_DIR:-$HERE/work/target}
shift
PROGS=$HERE/programs/depth
SELECTION="$*"

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/depth.XXXXXX") || exit 2
echo "work directory: $RUN"

# how /usr/bin/time reports the maximum RSS
TIMEFLAG=
if /usr/bin/time -l true 2>/dev/null >/dev/null; then TIMEFLAG=-l
elif /usr/bin/time -v true 2>/dev/null >/dev/null; then TIMEFLAG=-v
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

# build DIR NAME: elanc-rs in DIR (holding NAME.lgi and the modules)
build() {
  ( cd "$1" && elanc-rs -v -b -nosplit -quiet --target-dir "$TARGET" "$2" >elanc-rs.log 2>&1 )
}

for lgi in "$PROGS"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  [ -z "$SELECTION" ] || selected "$name" || continue
  exp=$PROGS/$name.expect
  if [ ! -f "$exp" ]; then fail "$name: no $name.expect"; continue; fi
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
    fail "$name: elanc-rs failed (see $d/big/elanc-rs.log)"; continue
  fi
  if [ -n "$TIMEFLAG" ]; then
    ( cd "$d/big" && /usr/bin/time $TIMEFLAG ./a.out -noInput -quiet >out 2>err ); status=$?
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
  elif grep -q 'overflowed its stack\|Segmentation\|panicked' "$d/big/err"; then
    fail "$name: crash (see $d/big/err)"
  elif [ -n "$maxrss" ] && [ -n "$rss" ] && [ "$rss" -gt "$maxrss" ]; then
    fail "$name: $rss MB of RSS, expected at most $maxrss MB"
  else
    ok "$name: $info [$results]"
  fi

  # a small size vs the interpreter
  [ -n "$small" ] || continue
  set -- $small
  mkdir "$d/small" "$d/int" || exit 2
  for w in small int; do
    sed "/start with/s/$1/$2/" "$lgi" >"$d/$w/$name.lgi" || exit 2
    cp "$PROGS"/*.eln "$d/$w/" || exit 2
  done
  if ! build "$d/small" "$name"; then
    fail "$name ($2): elanc-rs failed (see $d/small/elanc-rs.log)"; continue
  fi
  rs=$(cd "$d/small" && ./a.out -noInput -quiet 2>&1 | sed -n 's/^result = *//p' | oneline)
  iout=$( cd "$d/int" && printf '%s\n' "$query" | elan -b "$name.lgi" 2>&1; echo "status $?" )
  istatus=$(printf '%s\n' "$iout" | sed -n 's/^status //p')
  int=$(printf '%s\n' "$iout" | sed -n 's/^ \(.*\) end$/\1/p' | grep -v '^end$' | oneline)
  if [ "$istatus" != 0 ] || printf '%s\n' "$iout" | grep -q '^\[error\]'; then
    fail "$name ($2): the interpreter failed (status $istatus)"
  elif [ "$rs" = "$int" ]; then
    ok "$name ($2) vs interpreter: [$rs]"
  else
    fail "$name ($2) vs interpreter: elanc-rs [$rs], interpreter [$int]"
  fi
done

echo "$n programs, $fails failures"
[ "$fails" -eq 0 ]
