#!/bin/sh
# Strategies compiled by the Rust compiler (elanc-rs, S6b) compared with the
# interpreter and with REM.
#
# Programs: programs/strategies/<name>.lgi, each with a ground start term
# `start with (S) t` (or `()` for the normalisation of rules whose `where`s
# use strategies) and `query of sort bool`; the modules are the .eln files
# of the directory (all copied next to each program). With `-d DIR`, the
# programs of programs/DIR instead (programs/strategy_terms: strategy terms
# evaluated at run time, the programs of tests/compiler/programs/
# runtime_strategies and start_term with the query replaced by 3, 95 and
# 200, `<case>_<n>.lgi`; direct_<n>: f(<n>, 4), go applying to f only);
# the query given to the
# interpreter is the content of programs/DIR/query when it exists (the
# start term ignores it, but it must parse), else `true end`. For each
# program:
#  - elanc-rs builds it (shared cargo target directory, see TARGET below)
#    and `./a.out -noInput -quiet` prints one `result = t` line per result;
#  - the interpreter: `elan -b` with the query `true end` (ignored by the
#    start term) prints ` t end` per result: the sequences of results must
#    be equal, in order (the leading spaces of the compiled programs'
#    token spacing are not compared, nor the quotes of strings);
#  - REM: `elanc -b -nosplit -quiet` + make + `./a.out -noInput -quiet`:
#    the `result = ` lines must be byte-identical (the `rewrite_step` line
#    is not compared: it depends on the compilation scheme).
# A program without result fails (both sides empty prove nothing), unless
# it has a <name>.none saying why no result is expected (it then fails if
# elanc-rs gives one).
# Exceptions, each documented in its file: <name>.no-rem says why REM is
# not run, <name>.no-interpreter why the interpreter is not run.
# Outputs are compared as bytes (LC_ALL=C).
#
# Usage: test_strategies.sh PREFIX [-d DIR] [name...]   (an installed ELAN
# with elanc-rs; the names select programs, default all)
# The work directory is tests/compiler-rs/work/DIR.XXXXXX (kept,
# git-ignored); the cargo target directory is shared between runs:
# $ELANC_RS_TARGET_DIR, default tests/compiler-rs/work/target. The status is
# non-zero on any failure.
PREFIX=${1:?usage: test_strategies.sh PREFIX}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
HERE=$(cd "$(dirname "$0")" && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH LC_ALL=C
TARGET=${ELANC_RS_TARGET_DIR:-$HERE/work/target}
shift
DIR=strategies
if [ "$1" = -d ]; then DIR=${2:?-d DIR}; shift 2; fi
PROGS=$HERE/programs/$DIR
QUERY='true end'
[ -f "$PROGS/query" ] && QUERY=$(cat "$PROGS/query")

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/$DIR.XXXXXX") || exit 2
echo "work directory: $RUN"

HAVE_REM=; command -v elanc >/dev/null 2>&1 && HAVE_REM=1
fails=0 n=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }

selected() {  # selected NAME: NAME is one of the names given
  for s in $SELECTION; do [ "$s" = "$1" ] && return 0; done
  return 1
}
SELECTION="$*"

# results on one line: `a | b | c`
oneline() { tr '\n' '|' | sed -e 's/|$//' -e 's/|/ | /g'; }

for lgi in "$PROGS"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  [ -z "$SELECTION" ] || selected "$name" || continue
  n=$((n+1))
  d=$RUN/$name
  mkdir "$d" "$d/rs" "$d/rem" "$d/int" || exit 2
  for w in rs rem int; do
    cp "$lgi" "$PROGS"/*.eln "$d/$w/" || exit 2
  done

  # elanc-rs
  if ! ( cd "$d/rs" && elanc-rs -v -b -nosplit -quiet --target-dir "$TARGET" "$name" >elanc-rs.log 2>&1 ); then
    fail "$name: elanc-rs failed (see $d/rs/elanc-rs.log)"; continue
  fi
  build=$(sed -n 's/^elanc-rs: build time //p' "$d/rs/elanc-rs.log")
  out=$( cd "$d/rs" && ./a.out -noInput -quiet 2>"$d/rs/run.err"; echo "status $?" )
  status=$(printf '%s\n' "$out" | sed -n 's/^status //p')
  rs=$(printf '%s\n' "$out" | grep '^result = ')
  if [ "$status" != 0 ] || ! printf '%s\n' "$out" | grep -q '^rewrite_step = '; then
    fail "$name: the compiled program failed (status $status, see $d/rs/run.err)"; continue
  fi
  count=$(printf '%s' "$rs" | grep -c '^result = ')
  if [ -f "$PROGS/$name.none" ]; then
    if [ "$count" != 0 ]; then
      fail "$name: $count results, none expected ($name.none)"; continue
    fi
  elif [ "$count" = 0 ]; then
    fail "$name: no result (vacuous comparison; add $name.none if intended)"; continue
  fi

  # interpreter
  if [ -f "$PROGS/$name.no-interpreter" ]; then
    echo "skip $name vs interpreter ($name.no-interpreter)"
  else
    iout=$( cd "$d/int" && printf '%s\n' "$QUERY" | elan -b "$name.lgi" 2>&1; echo "status $?" )
    istatus=$(printf '%s\n' "$iout" | sed -n 's/^status //p')
    int=$(printf '%s\n' "$iout" | sed -n 's/^ \(.*\) end$/\1/p' | grep -v '^end$')
    a=$(printf '%s\n' "$rs" | sed 's/^result = *//' | tr -d '"' | oneline)
    b=$(printf '%s\n' "$int" | tr -d '"' | oneline)
    if [ "$istatus" != 0 ] || printf '%s\n' "$iout" | grep -q '^\[error\]'; then
      fail "$name: the interpreter failed (status $istatus)"
    elif [ "$a" = "$b" ]; then
      ok "$name vs interpreter: $count results [$a] (build $build)"
    else
      fail "$name vs interpreter: elanc-rs [$a], interpreter [$b]"
    fi
  fi

  # REM
  if [ -z "$HAVE_REM" ]; then
    echo "skip $name vs REM (elanc not installed)"
  elif [ -f "$PROGS/$name.no-rem" ]; then
    echo "skip $name vs REM ($name.no-rem)"
  elif ( cd "$d/rem" && elanc -b -nosplit -quiet "$name" >elanc.log 2>&1 \
       && make -f "$name.make" >make.log 2>&1 ); then
    rout=$( cd "$d/rem" && ./a.out -noInput -quiet 2>&1; echo "status $?" )
    rstatus=$(printf '%s\n' "$rout" | sed -n 's/^status //p')
    rem=$(printf '%s\n' "$rout" | grep '^result = ')
    if [ "$rstatus" != 0 ]; then
      fail "$name: the REM program failed (status $rstatus)"
    elif [ "$rs" = "$rem" ]; then
      ok "$name vs REM: $count results"
    else
      fail "$name vs REM: elanc-rs [$(printf '%s\n' "$rs" | oneline)], REM [$(printf '%s\n' "$rem" | oneline)]"
    fi
  else
    fail "$name: elanc or make failed (see $d/rem/*.log)"
  fi
done

echo "$n programs, $fails failures"
[ "$fails" -eq 0 ]
