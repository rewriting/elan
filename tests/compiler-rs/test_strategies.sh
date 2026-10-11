#!/bin/sh
# Strategies compiled by a new compiler (elanc-rs, S6b; elanc-go, S7b)
# compared with the interpreter and with REM.
#
# The compiler is $ELANC: elanc-go (default since S7e) or elanc-rs (built
# only with -DELAN_RUST=ON); below, "the
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
#  - REM: `elanc-rem -b -nosplit -quiet` + make + `./a.out -noInput -quiet`:
#    the `result = ` lines must be byte-identical; the `rewrite_step` line
#    is compared only when DIR has a file rem-steps (programs/ac: elanc-go
#    tries the same redexes in the same order as REM), except for a
#    program with a <name>.steps saying why its count differs (elsewhere
#    it depends on the compilation scheme).
# Both compiled programs read /dev/null as their standard input. When DIR
# has a file `output` (programs/io), the whole output of each program
# (standard output without its rewrite_step line, and standard error) is
# compared with REM's byte for byte, and the files listed in <name>.files
# (written by the program in its directory) with REM's; the files of
# DIR/data are copied next to each program. A program with a
# <name>.status (a run-time error of REM's: arrays out of range) must stop
# with that status, REM's program too; their whole outputs are then
# compared (DIR/output), nothing else.
# A program without result fails (both sides empty prove nothing), unless
# it has a <name>.none saying why no result is expected (it then fails if
# the compiler gives one).
# Exceptions, each documented in its file: <name>.no-rem says why REM is
# not run, <name>.no-interpreter why the interpreter is not run (when
# neither runs the program right, <name>.expected holds its expected
# `result = ` lines, `#` lines being comments),
# <name>.no-<compiler> (e.g. .no-elanc-go) why the program is not run with
# that compiler; no-<compiler> in DIR why no program of DIR is (e.g.
# programs/ac/no-elanc-rs: elanc-rs refuses AC by design).
# AC terms (programs/ac, S7c): the interpreter prints an AC term as it was
# built, REM and the compilers flattened and sorted; when DIR has a file
# ac-symbols (the AC symbols), the results compared with the
# interpreter's are first put in a canonical form modulo AC
# (ac_canon.py); with <name>.multiset (saying why: the oracles enumerate
# AC solutions in different orders), as multisets (sorted). The results
# are compared with REM's byte for byte, in order. A program that the compiler refuses (status 2, "not
# supported yet (<stage>)", the stage being one of its later ones: S6c-S6e
# for elanc-rs, S7c-S7e for elanc-go) fails, the refusal quoted.
# Outputs are compared as bytes (LC_ALL=C).
#
# Usage: [ELANC=elanc-rs] test_strategies.sh PREFIX [-d DIR] [name...]   (an
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
ELANC=${ELANC:-elanc-go}
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
if [ -f "$PROGS/no-$ELANC" ]; then
  echo "skip $DIR ($DIR/no-$ELANC: $(head -1 "$PROGS/no-$ELANC"))"; exit 0
fi
# canon: the canonical form modulo AC of the result lines (identity when
# DIR has no ac-symbols)
canon() {
  if [ -f "$PROGS/ac-symbols" ]; then
    # shellcheck disable=SC2046
    python3 "$HERE/ac_canon.py" $(cat "$PROGS/ac-symbols")
  else
    cat
  fi
}

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/$DIR$SUFFIX.XXXXXX") || exit 2
echo "work directory: $RUN ($ELANC)"

HAVE_REM=; command -v elanc-rem >/dev/null 2>&1 && HAVE_REM=1
# the programs read an empty standard input (getc(stdin): -1)
NULL=/dev/null
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

# same_output NAME D: with DIR/output, the whole output of the compiled
# program in D/$W (standard output without its rewrite_step line, standard
# error) and the files of NAME.files are REM's (in D/rem), byte for byte
same_output() {
  [ -f "$PROGS/output" ] || return 0
  same=1
  grep -v '^rewrite_step = ' "$2/$W/run.out" >"$2/$W/run.out.cmp"
  grep -v '^rewrite_step = ' "$2/rem/run.out" >"$2/rem/run.out.cmp"
  for f in run.out.cmp run.err $(cat "$PROGS/$1.files" 2>/dev/null); do
    if ! cmp -s "$2/$W/$f" "$2/rem/$f"; then
      fail "$1 vs REM: $f differs (see $2)"; same=
    fi
  done
  files=
  [ -f "$PROGS/$1.files" ] && files=" and files ($(tr '\n' ' ' <"$PROGS/$1.files" | sed 's/ $//'))"
  [ -n "$same" ] && ok "$1 vs REM: the same output, error output$files"
  return 0
}

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
    if [ -d "$PROGS/data" ]; then cp "$PROGS"/data/* "$d/$w/" || exit 2; fi
  done

  # the compiler
  ( cd "$d/$W" && $ELANC -v -b -nosplit -quiet $CACHE_OPT "$TARGET" "$name" >$ELANC.log 2>&1 )
  cstatus=$?
  if [ "$cstatus" != 0 ]; then
    refusal=$(grep -E "not supported (yet \($STAGE[c-e]\)|\(not planned\))" "$d/$W/$ELANC.log" | head -1)
    if [ "$cstatus" = 2 ] && [ -n "$refusal" ]; then
      fail "$name: refused by $ELANC: $refusal"
    else
      fail "$name: $ELANC failed (status $cstatus, see $d/$W/$ELANC.log)"
    fi
    continue
  fi
  vet "$name" "$d/$W" || continue
  build=$(sed -n "s/^$ELANC: build time //p" "$d/$W/$ELANC.log")
  out=$( cd "$d/$W" && ./a.out -noInput -quiet <"$NULL" >run.out 2>run.err; s=$?; cat run.out; echo; echo "status $s" )
  status=$(printf '%s\n' "$out" | sed -n 's/^status //p')
  rs=$(printf '%s\n' "$out" | grep '^result = ')
  # a program stopped by a run-time error of REM's (<name>.status): the
  # status, and the whole output as REM's
  if [ -f "$PROGS/$name.status" ]; then
    want=$(cat "$PROGS/$name.status")
    if [ "$status" != "$want" ]; then
      fail "$name: status $status, expected $want ($name.status, see $d/$W)"; continue
    fi
    ok "$name: status $want ($name.status)"
    if [ -n "$HAVE_REM" ] && [ ! -f "$PROGS/$name.no-rem" ]; then
      if ( cd "$d/rem" && elanc-rem -b -nosplit -quiet "$name" >elanc.log 2>&1 \
           && make -f "$name.make" >make.log 2>&1 ); then
        ( cd "$d/rem" && ./a.out -noInput -quiet <"$NULL" >run.out 2>run.err )
        rstatus=$?
        if [ "$rstatus" != "$want" ]; then
          fail "$name: the REM program's status $rstatus, expected $want"
        else
          same_output "$name" "$d"
        fi
      else
        fail "$name: elanc-rem or make failed (see $d/rem/*.log)"
      fi
    fi
    continue
  fi
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

  # the expected results, when no oracle runs the program right
  if [ -f "$PROGS/$name.expected" ]; then
    expected=$(grep -v '^#' "$PROGS/$name.expected")
    if [ "$rs" = "$expected" ]; then
      ok "$name: [$(printf '%s\n' "$rs" | oneline)] ($name.expected)"
    else
      fail "$name: $ELANC [$(printf '%s\n' "$rs" | oneline)], expected [$(printf '%s\n' "$expected" | oneline)] ($name.expected)"
    fi
  fi

  # interpreter
  if [ -f "$PROGS/$name.no-interpreter" ]; then
    echo "skip $name vs interpreter ($name.no-interpreter)"
  else
    iout=$( cd "$d/int" && printf '%s\n' "$QUERY" | elan -b "$name.lgi" 2>&1; echo "status $?" )
    istatus=$(printf '%s\n' "$iout" | sed -n 's/^status //p')
    int=$(printf '%s\n' "$iout" | sed -n 's/^ \(.*\) end$/\1/p' | grep -v '^end$')
    order=cat how=
    if [ -f "$PROGS/$name.multiset" ]; then order=sort how=" (as multisets, $name.multiset)"; fi
    a=$(printf '%s\n' "$rs" | sed 's/^result = *//' | tr -d '"' | canon | $order | oneline)
    b=$(printf '%s\n' "$int" | tr -d '"' | canon | $order | oneline)
    if [ "$istatus" != 0 ] || printf '%s\n' "$iout" | grep -q '^\[error\]'; then
      fail "$name: the interpreter failed (status $istatus)"
    elif [ "$a" = "$b" ]; then
      ok "$name vs interpreter$how: $count results [$a] (build $build)"
    else
      fail "$name vs interpreter: $ELANC [$a], interpreter [$b]"
    fi
  fi

  # REM
  if [ -z "$HAVE_REM" ]; then
    echo "skip $name vs REM (elanc-rem not installed)"
  elif [ -f "$PROGS/$name.no-rem" ]; then
    echo "skip $name vs REM ($name.no-rem)"
  elif ( cd "$d/rem" && elanc-rem -b -nosplit -quiet "$name" >elanc.log 2>&1 \
       && make -f "$name.make" >make.log 2>&1 ); then
    rout=$( cd "$d/rem" && ./a.out -noInput -quiet <"$NULL" >run.out 2>run.err; s=$?; cat run.out run.err; echo; echo "status $s" )
    rstatus=$(printf '%s\n' "$rout" | sed -n 's/^status //p')
    rem=$(printf '%s\n' "$rout" | grep '^result = ')
    if [ "$rstatus" != 0 ]; then
      fail "$name: the REM program failed (status $rstatus)"
    elif [ "$rs" != "$rem" ]; then
      fail "$name vs REM: $ELANC [$(printf '%s\n' "$rs" | oneline)], REM [$(printf '%s\n' "$rem" | oneline)]"
    elif [ -f "$PROGS/rem-steps" ] && [ ! -f "$PROGS/$name.steps" ]; then
      csteps=$(printf '%s\n' "$out" | sed -n 's/^rewrite_step = //p')
      rsteps=$(printf '%s\n' "$rout" | sed -n 's/^rewrite_step = //p')
      if [ "$csteps" = "$rsteps" ]; then
        ok "$name vs REM: $count results, $csteps rewrite steps"
      else
        fail "$name vs REM: rewrite steps $ELANC $csteps, REM $rsteps"
      fi
    else
      ok "$name vs REM: $count results"
    fi
    if [ "$rstatus" = 0 ]; then same_output "$name" "$d"; fi
  else
    fail "$name: elanc-rem or make failed (see $d/rem/*.log)"
  fi
done

echo "$n programs, $fails failures"
# the run directory is kept only when something failed (to look at it)
if [ "$fails" -eq 0 ]; then rm -rf "$RUN"; else echo "kept for inspection: $RUN"; fi
[ "$fails" -eq 0 ]
