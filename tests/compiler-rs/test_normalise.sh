#!/bin/sh
# Normalisation by the Rust compiler (elanc-rs, S6a) compared with REM and
# with the interpreter.
#
# Programs: programs/normalise/<name>.lgi (+ .eln), each with a start term
# `start with () t` and `query of sort bool`. For each program:
#  - elanc-rs builds it (shared cargo target directory, see TARGET below;
#    the REM options of the bench, -b -nosplit -quiet, are ignored) and
#    `./a.out -noInput -quiet` prints `result = t`;
#  - REM: `elanc -b -nosplit -quiet` + make + `./a.out -noInput -quiet`:
#    the `result = ` lines must be byte-identical (the `rewrite_step` line
#    is not compared: it depends on the compilation scheme);
#  - the interpreter: `elan -b` with the query `true end` (ignored by the
#    start term) prints ` t end`: the results must be equal modulo the
#    printing of builtin strings (quoted by the interpreter, raw in
#    compiled programs, elan-runtime/src/print.rs): `"` is removed from
#    both sides.
# Exceptions, each documented in its file: <name>.rem holds the expected
# REM result line when REM differs on purpose (the elanc-rs result is then
# compared with the interpreter only); <name>.no-interpreter says why the
# interpreter is not run.
# Programs programs/refused/<name>.lgi use constructs of later stages:
# elanc-rs must stop with status 2 and a "not supported yet" message, and
# remove the a.out left by a previous compilation.
#
# Usage: test_normalise.sh PREFIX [name...]   (an installed ELAN with
# elanc-rs; the names select programs, default all)
# The work directory is tests/compiler-rs/work/normalise.XXXXXX (kept,
# git-ignored); the cargo target directory is shared between runs:
# $ELANC_RS_TARGET_DIR, default tests/compiler-rs/work/target (the runtime
# crate is compiled once). The status is non-zero on any failure.
PREFIX=${1:?usage: test_normalise.sh PREFIX}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
HERE=$(cd "$(dirname "$0")" && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH
TARGET=${ELANC_RS_TARGET_DIR:-$HERE/work/target}

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/normalise.XXXXXX") || exit 2
echo "work directory: $RUN"

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
  mkdir "$d" "$d/rs" "$d/rem" "$d/int" || exit 2
  for w in rs rem int; do
    cp "$HERE/programs/normalise/$name.lgi" "$HERE/programs/normalise/$name.eln" "$d/$w/" || exit 2
  done

  # elanc-rs
  # the REM options of the bench command line are accepted and ignored
  if ! ( cd "$d/rs" && elanc-rs -v -b -nosplit -quiet --target-dir "$TARGET" "$name" >elanc-rs.log 2>&1 ); then
    fail "$name: elanc-rs failed (see $d/rs/elanc-rs.log)"; continue
  fi
  build=$(sed -n 's/^elanc-rs: build time //p' "$d/rs/elanc-rs.log")
  rs=$( cd "$d/rs" && ./a.out -noInput -quiet 2>"$d/rs/run.err"; echo "status $?" )
  status=$(printf '%s\n' "$rs" | sed -n 's/^status //p')
  rs=$(printf '%s\n' "$rs" | grep '^result = ')
  if [ "$status" != 0 ] || [ -z "$rs" ]; then
    fail "$name: the compiled program failed (status $status, see $d/rs/run.err)"; continue
  fi

  # REM
  if [ -z "$HAVE_REM" ]; then
    echo "skip $name vs REM (elanc not installed)"
  elif ( cd "$d/rem" && elanc -b -nosplit -quiet "$name" >elanc.log 2>&1 \
       && make -f "$name.make" >make.log 2>&1 ); then
    rem=$( cd "$d/rem" && ./a.out -noInput -quiet 2>&1 | grep '^result = ' )
    known=$HERE/programs/normalise/$name.rem
    if [ -f "$known" ]; then
      expected=$(grep -v '^#' "$known")
      if [ "$rem" = "$expected" ]; then
        ok "$name vs REM: known difference ($name.rem), elanc-rs $rs (build $build)"
      else
        fail "$name vs REM: REM [$rem], expected [$expected] ($name.rem)"
      fi
    elif [ "$rs" = "$rem" ]; then
      ok "$name vs REM: $rs (build $build)"
    else
      fail "$name vs REM: elanc-rs [$rs], REM [$rem]"
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
    fail "$name vs interpreter: elanc-rs [$a], interpreter [$b]"
  fi
done

# The same program compiled from another directory reuses its package in
# the shared target directory (named after the generated code, not after
# the work directory): the target directory does not grow.
packages() {  # packages NAME: the files of NAME's packages in $TARGET
  ls -a "$TARGET/release" "$TARGET/release/deps" "$TARGET/release/.fingerprint" \
    2>/dev/null | grep -c "^elan[-_]$1[-_]"
}
if [ -d "$RUN/peano/rs" ] && [ -x "$RUN/peano/rs/a.out" ]; then
  n=$((n+1))
  d=$RUN/peano/again
  mkdir "$d" && cp "$RUN/peano/rs/peano.lgi" "$RUN/peano/rs/peano.eln" "$d/" || exit 2
  before=$(packages peano)
  if ( cd "$d" && elanc-rs --target-dir "$TARGET" peano >elanc-rs.log 2>&1 ) \
     && [ "$(cd "$d" && ./a.out -noInput -quiet | grep '^result = ')" \
          = "$(cd "$RUN/peano/rs" && ./a.out -noInput -quiet | grep '^result = ')" ]; then
    after=$(packages peano)
    if [ "$before" = "$after" ]; then
      ok "peano from another directory: same package ($after files)"
    else
      fail "peano from another directory: $before files, then $after in $TARGET"
    fi
  else
    fail "peano from another directory: elanc-rs failed or another result (see $d/elanc-rs.log)"
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
  ( cd "$d" && elanc-rs --target-dir "$TARGET" "$name" >elanc-rs.log 2>&1 )
  status=$?
  msg=$(grep 'not supported yet' "$d/elanc-rs.log")
  if [ "$status" = 2 ] && [ -n "$msg" ] && [ ! -e "$d/a.out" ]; then
    ok "refused $name: $msg"
  else
    fail "refused $name: status $status, expected 2 and a message, no a.out (see $d/elanc-rs.log)"
  fi
done

echo "$n programs, $fails failures"
[ "$fails" -eq 0 ]
