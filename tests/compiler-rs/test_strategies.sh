#!/bin/sh
# Strategies compiled by a new compiler (elanc-rs, S6b; elanc-go, S7b)
# compared with the interpreter and with REM.
#
# The compiler is $ELANC: elanc-rs (default) or elanc-go; below, "the
# compiler" is that one.
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
#  - the compiler builds it (shared build cache, see TARGET below) and `./a.out -noInput -quiet` prints one `result = t` line per result;
#    with elanc-go, `go vet` on the generated module (.elan-go.<name>)
#    must report nothing;
#  - the interpreter: `elan -b` with the query `true end` (ignored by the
#    start term) prints ` t end` per result: the sequences of results must
#    be equal, in order (the leading spaces of the compiled programs'
#    token spacing are not compared, nor the quotes of strings);
#  - REM: `elanc -b -nosplit -quiet` + make + `./a.out -noInput -quiet`:
#    the `result = ` lines must be byte-identical (the `rewrite_step` line
#    is not compared: it depends on the compilation scheme).
# A program without result fails (both sides empty prove nothing), unless
# it has a <name>.none saying why no result is expected (it then fails if
# the compiler gives one).
# Exceptions, each documented in its file: <name>.no-rem says why REM is
# not run, <name>.no-interpreter why the interpreter is not run,
# <name>.no-<compiler> (e.g. .no-elanc-go) why the program is not run with
# that compiler. A program that the compiler refuses (status 2, "not
# supported yet (<stage>)", the stage being one of its later ones: S6c-S6e
# for elanc-rs, S7c-S7e for elanc-go) fails, the refusal quoted.
# Outputs are compared as bytes (LC_ALL=C).
#
# Usage: [ELANC=elanc-go] test_strategies.sh PREFIX [-d DIR] [name...]   (an
# installed ELAN with the compiler; the names select programs, default all)
# The work directory is tests/compiler-rs/work/DIR.XXXXXX for elanc-rs,
# DIR-go.XXXXXX for elanc-go (kept, git-ignored); the build cache is shared
# between runs, one per compiler: the cargo target directory
# $ELANC_RS_TARGET_DIR, default tests/compiler-rs/work/target, or the go
# build cache $ELANC_GO_CACHE, default tests/compiler-rs/work/gocache. The
# status is non-zero on any failure.
PREFIX=${1:?usage: test_strategies.sh PREFIX}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
HERE=$(cd "$(dirname "$0")" && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH LC_ALL=C
ELANC=${ELANC:-elanc-rs}
# per compiler: the build cache and its option, the stage of the refusals,
# the suffix of the work directory, the subdirectory of each program
case $ELANC in
  elanc-rs)
    TARGET=${ELANC_RS_TARGET_DIR:-$HERE/work/target}
    CACHE_OPT=--target-dir STAGE=S6 SUFFIX= W=rs ;;
  elanc-go)
    TARGET=${ELANC_GO_CACHE:-$HERE/work/gocache}
    CACHE_OPT=--gocache STAGE=S7 SUFFIX=-go W=go ;;
  *) echo "unknown compiler ELANC=$ELANC (elanc-rs or elanc-go)" >&2; exit 2 ;;
esac
shift
DIR=strategies
if [ "$1" = -d ]; then DIR=${2:?-d DIR}; shift 2; fi
PROGS=$HERE/programs/$DIR
QUERY='true end'
[ -f "$PROGS/query" ] && QUERY=$(cat "$PROGS/query")

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/$DIR$SUFFIX.XXXXXX") || exit 2
echo "work directory: $RUN ($ELANC)"

HAVE_REM=; command -v elanc >/dev/null 2>&1 && HAVE_REM=1
fails=0 n=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }
# vet NAME DIR: with elanc-go, go vet on the generated module
# DIR/.elan-go.NAME must report nothing (e.g. no unreachable code); the go
# command is the one elanc-go uses (share/elan-go/go-command, else go).
GO=go
[ -x "$(cat "$PREFIX/share/elan-go/go-command" 2>/dev/null)" ] && GO=$(cat "$PREFIX/share/elan-go/go-command")
vet() {
  [ "$ELANC" = elanc-go ] || return 0
  if v=$(cd "$2/.elan-go.$1" && GOTOOLCHAIN=local GOFLAGS=-mod=mod GOPROXY=off GOWORK=off \
         GOCACHE="$TARGET" "$GO" vet . 2>&1); then
    return 0
  fi
  fail "$1: go vet: $(printf '%s\n' "$v" | grep -v '^#' | head -3 | tr '\n' ' ')(module in $2/.elan-go.$1)"
  return 1
}

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
  if [ -f "$PROGS/$name.no-$ELANC" ]; then
    echo "skip $name ($name.no-$ELANC)"; continue
  fi
  n=$((n+1))
  d=$RUN/$name
  mkdir "$d" "$d/$W" "$d/rem" "$d/int" || exit 2
  for w in $W rem int; do
    cp "$lgi" "$PROGS"/*.eln "$d/$w/" || exit 2
  done

  # the compiler
  ( cd "$d/$W" && $ELANC -v -b -nosplit -quiet $CACHE_OPT "$TARGET" "$name" >$ELANC.log 2>&1 )
  cstatus=$?
  if [ "$cstatus" != 0 ]; then
    refusal=$(grep -E "not supported yet \($STAGE[c-e]\)" "$d/$W/$ELANC.log" | head -1)
    if [ "$cstatus" = 2 ] && [ -n "$refusal" ]; then
      fail "$name: refused by $ELANC: $refusal"
    else
      fail "$name: $ELANC failed (status $cstatus, see $d/$W/$ELANC.log)"
    fi
    continue
  fi
  vet "$name" "$d/$W" || continue
  build=$(sed -n "s/^$ELANC: build time //p" "$d/$W/$ELANC.log")
  out=$( cd "$d/$W" && ./a.out -noInput -quiet 2>"$d/$W/run.err"; echo "status $?" )
  status=$(printf '%s\n' "$out" | sed -n 's/^status //p')
  rs=$(printf '%s\n' "$out" | grep '^result = ')
  if [ "$status" != 0 ] || ! printf '%s\n' "$out" | grep -q '^rewrite_step = '; then
    fail "$name: the compiled program failed (status $status, see $d/$W/run.err)"; continue
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
      fail "$name vs interpreter: $ELANC [$a], interpreter [$b]"
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
      fail "$name vs REM: $ELANC [$(printf '%s\n' "$rs" | oneline)], REM [$(printf '%s\n' "$rem" | oneline)]"
    fi
  else
    fail "$name: elanc or make failed (see $d/rem/*.log)"
  fi
done

echo "$n programs, $fails failures"
# the run directory is kept only when something failed (to look at it)
if [ "$fails" -eq 0 ]; then rm -rf "$RUN"; else echo "kept for inspection: $RUN"; fi
[ "$fails" -eq 0 ]
