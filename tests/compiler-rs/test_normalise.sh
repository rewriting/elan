#!/bin/sh
# Normalisation by a new compiler (elanc-rs, S6a; elanc-go, S7a) compared
# with REM and with the interpreter.
#
# The compiler is $ELANC: elanc-rs (default) or elanc-go; below, "the
# compiler" is that one.
#
# Programs: programs/normalise/<name>.lgi (+ .eln), each with a start term
# `start with () t` and `query of sort bool`. For each program:
#  - the compiler builds it (shared build cache, see TARGET below; the REM
#    options of the bench, -b -nosplit -quiet, are ignored) and
#    `./a.out -noInput -quiet` prints `result = t`;
#  - REM: `elanc -b -nosplit -quiet` + make + `./a.out -noInput -quiet`:
#    the `result = ` lines must be byte-identical (the `rewrite_step` line
#    is not compared: it depends on the compilation scheme);
#  - the interpreter: `elan -b` with the query `true end` (ignored by the
#    start term) prints ` t end`: the results must be equal modulo the
#    printing of builtin strings (quoted by the interpreter, raw in
#    compiled programs, elan-runtime/src/print.rs, elanrt/print.go): `"`
#    is removed from both sides.
# Exceptions, each documented in its file: <name>.rem holds the expected
# REM result line when REM differs on purpose (the compiler's result is then
# compared with the interpreter only); <name>.no-interpreter says why the
# interpreter is not run, <name>.no-rem why REM is not run.
# Outputs are compared as bytes (LC_ALL=C): strings are bytes, and the
# sources of latin1*.eln are Latin-1.
# Programs programs/refused/<name>.lgi use constructs of later stages:
# the compiler must stop with status 2 and a "not supported yet (<stage>)"
# message, the stage being one of its own (S6b... for elanc-rs, S7b... for
# elanc-go), and remove the a.out left by a previous compilation.
#
# Usage: [ELANC=elanc-go] test_normalise.sh PREFIX [name...]   (an installed
# ELAN with the compiler; the names select programs, default all)
# The work directory is tests/compiler-rs/work/normalise.XXXXXX for
# elanc-rs, normalise-go.XXXXXX for elanc-go (kept, git-ignored); the build
# cache is shared between runs (the runtime is compiled once), one per
# compiler: the cargo target directory $ELANC_RS_TARGET_DIR, default
# tests/compiler-rs/work/target, or the go build cache $ELANC_GO_CACHE,
# default tests/compiler-rs/work/gocache. The status is non-zero on any
# failure.
PREFIX=${1:?usage: test_normalise.sh PREFIX}
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

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/normalise$SUFFIX.XXXXXX") || exit 2
echo "work directory: $RUN ($ELANC)"

HAVE_REM=; command -v elanc >/dev/null 2>&1 && HAVE_REM=1
fails=0 n=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }

shift
selected() {  # selected NAME: NAME is one of the names given
  for s in $SELECTION; do [ "$s" = "$1" ] && return 0; done
  return 1
}
SELECTION="$*"

for lgi in "$HERE"/programs/normalise/*.lgi; do
  name=$(basename "$lgi" .lgi)
  [ -z "$SELECTION" ] || selected "$name" || continue
  n=$((n+1))
  d=$RUN/$name
  mkdir "$d" "$d/$W" "$d/rem" "$d/int" || exit 2
  for w in $W rem int; do
    cp "$HERE/programs/normalise/$name.lgi" "$HERE/programs/normalise/$name.eln" "$d/$w/" || exit 2
  done

  # the compiler
  # the REM options of the bench command line are accepted and ignored
  if ! ( cd "$d/$W" && $ELANC -v -b -nosplit -quiet $CACHE_OPT "$TARGET" "$name" >$ELANC.log 2>&1 ); then
    fail "$name: $ELANC failed (see $d/$W/$ELANC.log)"; continue
  fi
  build=$(sed -n "s/^$ELANC: build time //p" "$d/$W/$ELANC.log")
  rs=$( cd "$d/$W" && ./a.out -noInput -quiet 2>"$d/$W/run.err"; echo "status $?" )
  status=$(printf '%s\n' "$rs" | sed -n 's/^status //p')
  rs=$(printf '%s\n' "$rs" | grep '^result = ')
  if [ "$status" != 0 ] || [ -z "$rs" ]; then
    fail "$name: the compiled program failed (status $status, see $d/$W/run.err)"; continue
  fi

  # REM
  if [ -z "$HAVE_REM" ]; then
    echo "skip $name vs REM (elanc not installed)"
  elif [ -f "$HERE/programs/normalise/$name.no-rem" ]; then
    echo "skip $name vs REM ($name.no-rem)"
  elif ( cd "$d/rem" && elanc -b -nosplit -quiet "$name" >elanc.log 2>&1 \
       && make -f "$name.make" >make.log 2>&1 ); then
    rem=$( cd "$d/rem" && ./a.out -noInput -quiet 2>&1 | grep '^result = ' )
    known=$HERE/programs/normalise/$name.rem
    if [ -f "$known" ]; then
      expected=$(grep -v '^#' "$known")
      if [ "$rem" = "$expected" ]; then
        ok "$name vs REM: known difference ($name.rem), $ELANC $rs (build $build)"
      else
        fail "$name vs REM: REM [$rem], expected [$expected] ($name.rem)"
      fi
    elif [ "$rs" = "$rem" ]; then
      ok "$name vs REM: $rs (build $build)"
    else
      fail "$name vs REM: $ELANC [$rs], REM [$rem]"
    fi
  else
    fail "$name: elanc or make failed (see $d/rem/*.log)"
  fi

  # interpreter
  if [ -f "$HERE/programs/normalise/$name.no-interpreter" ]; then
    echo "skip $name vs interpreter ($name.no-interpreter)"; continue
  fi
  int=$( cd "$d/int" && printf 'true end\n' | elan -b "$name.lgi" 2>&1 \
         | sed -n 's/^ \(.*\) end$/\1/p' | grep -v '^end$' )
  a=$(printf '%s\n' "$rs" | sed 's/^result = //' | tr -d '"')
  b=$(printf '%s\n' "$int" | tr -d '"')
  if [ -n "$b" ] && [ "$a" = "$b" ]; then
    ok "$name vs interpreter"
  else
    fail "$name vs interpreter: $ELANC [$a], interpreter [$b]"
  fi
done

# The same program compiled from another directory reuses its package in
# the shared build cache (elanc-rs: a cargo package named after the
# generated code, not after the work directory; elanc-go: go build
# -trimpath, independent of the directory): the cache does not grow.
packages() {  # packages NAME: the files of NAME's packages in $TARGET
  case $ELANC in
    elanc-rs)
      ls -a "$TARGET/release" "$TARGET/release/deps" "$TARGET/release/.fingerprint" \
        2>/dev/null | grep -c "^elan[-_]$1[-_]" ;;
    *) find "$TARGET" -type f | wc -l | tr -d ' ' ;;  # all its files
  esac
}
if [ -d "$RUN/peano/$W" ] && [ -x "$RUN/peano/$W/a.out" ]; then
  n=$((n+1))
  d=$RUN/peano/again
  mkdir "$d" && cp "$RUN/peano/$W/peano.lgi" "$RUN/peano/$W/peano.eln" "$d/" || exit 2
  before=$(packages peano)
  if ( cd "$d" && $ELANC $CACHE_OPT "$TARGET" peano >$ELANC.log 2>&1 ) \
     && [ "$(cd "$d" && ./a.out -noInput -quiet | grep '^result = ')" \
          = "$(cd "$RUN/peano/$W" && ./a.out -noInput -quiet | grep '^result = ')" ]; then
    after=$(packages peano)
    if [ "$before" = "$after" ]; then
      ok "peano from another directory: same package ($after files)"
    else
      fail "peano from another directory: $before files, then $after in $TARGET"
    fi
  else
    fail "peano from another directory: $ELANC failed or another result (see $d/$ELANC.log)"
  fi
fi

for lgi in "$HERE"/programs/refused/*.lgi; do
  name=$(basename "$lgi" .lgi)
  [ -z "$SELECTION" ] || selected "$name" || continue
  n=$((n+1))
  d=$RUN/refused-$name
  mkdir "$d" || exit 2
  cp "$HERE/programs/refused/$name.lgi" "$HERE/programs/refused/"*.eln "$d/" || exit 2
  echo stale >"$d/a.out" || exit 2  # a refused program leaves no a.out
  ( cd "$d" && $ELANC $CACHE_OPT "$TARGET" "$name" >$ELANC.log 2>&1 )
  status=$?
  msg=$(grep "not supported yet ($STAGE[b-e])" "$d/$ELANC.log")
  if [ "$status" = 2 ] && [ -n "$msg" ] && [ ! -e "$d/a.out" ]; then
    ok "refused $name: $msg"
  else
    fail "refused $name: status $status, expected 2 and a message ($STAGE...), no a.out (see $d/$ELANC.log)"
  fi
done

echo "$n programs, $fails failures"
[ "$fails" -eq 0 ]
