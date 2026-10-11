#!/bin/sh
# Normalisation by a new compiler (elanc-rs, S6a; elanc-go, S7a) compared
# with REM and with the interpreter.
#
# The compiler is $ELANC: elanc-go (default since S7e) or elanc-rs (built
# only with -DELAN_RUST=ON); below, "the
# compiler" is that one.
#
# Programs: programs/normalise/<name>.lgi (+ .eln), each with a start term
# `start with () t` and `query of sort bool`. For each program:
#  - the compiler builds it (shared build cache, see TARGET below; the REM
#    options of the bench, -b -nosplit -quiet, are ignored) and
#    `./a.out -noInput -quiet` prints `result = t`;
#    with elanc-go, `go vet` on the generated module (.elan-go.<name>)
#    must report nothing;
#  - REM: `elanc-rem -b -nosplit -quiet` + make + `./a.out -noInput -quiet`:
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
# interpreter is not run, <name>.no-rem why REM is not run,
# <name>.no-<compiler> (e.g. .no-elanc-rs) why the program is not run
# with that compiler (normalised or refused programs); <name>.expected
# holds the expected result line of a program that neither oracle runs
# (its .no-rem and .no-interpreter say why); <name>.stdout the whole
# expected output of `./a.out -noInput -quiet` (status 0), compared with
# the compiler's and REM's (the interpreter has no -noInput: .no-interpreter).
# REM prints an identifier as IDENT(n), n its index in the .ref file: its
# output is compared with the names put back (rem_idents.py).
# Outputs are compared as bytes (LC_ALL=C): strings are bytes, and the
# sources of latin1*.eln are Latin-1.
# Programs programs/refused/<name>.lgi use constructs the compiler refuses:
# it must stop with status 2 and a "not supported (not planned)" message
# (refusals kept by design, docs/followups.md; since S7e the only kind for
# elanc-go) or, for elanc-rs, a "not supported yet (<stage>)" message, the
# stage being one of its later ones (S6c, S6d, S6e),
# and remove the a.out left by a previous compilation.
#
# Usage: [ELANC=elanc-rs] test_normalise.sh PREFIX [name...]   (an installed
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
ELANC=${ELANC:-elanc-go}
# per compiler: the build cache and its option, the refusal messages,
# the suffix of the work directory, the subdirectory of each program
case $ELANC in
  elanc-rs)
    TARGET=${ELANC_RS_TARGET_DIR:-$HERE/work/target}
    CACHE_OPT=--target-dir SUFFIX= W=rs
    REFUSED='not supported (yet \(S6[c-e]\)|\(not planned\))' ;;
  elanc-go)
    TARGET=${ELANC_GO_CACHE:-$HERE/work/gocache}
    # since S7e every refusal of elanc-go is kept by design
    CACHE_OPT=--gocache SUFFIX=-go W=go REFUSED='not supported \(not planned\)' ;;
  *) echo "unknown compiler ELANC=$ELANC (elanc-rs or elanc-go)" >&2; exit 2 ;;
esac

mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/normalise$SUFFIX.XXXXXX") || exit 2
echo "work directory: $RUN ($ELANC)"

HAVE_REM=; command -v elanc-rem >/dev/null 2>&1 && HAVE_REM=1
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

shift
selected() {  # selected NAME: NAME is one of the names given
  for s in $SELECTION; do [ "$s" = "$1" ] && return 0; done
  return 1
}
SELECTION="$*"

for lgi in "$HERE"/programs/normalise/*.lgi; do
  name=$(basename "$lgi" .lgi)
  [ -z "$SELECTION" ] || selected "$name" || continue
  if [ -f "$HERE/programs/normalise/$name.no-$ELANC" ]; then
    echo "skip $name ($name.no-$ELANC)"; continue
  fi
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
  vet "$name" "$d/$W" || continue
  build=$(sed -n "s/^$ELANC: build time //p" "$d/$W/$ELANC.log")
  # a program with an expected whole output (<name>.stdout): the
  # compiler's and REM's, byte for byte, status 0
  stdout=$HERE/programs/normalise/$name.stdout
  if [ -f "$stdout" ]; then
    ( cd "$d/$W" && ./a.out -noInput -quiet >run.out 2>run.err )
    status=$?
    if [ "$status" = 0 ] && cmp -s "$d/$W/run.out" "$stdout"; then
      ok "$name: the output of $name.stdout"
    else
      fail "$name: status $status, output $(od -c "$d/$W/run.out" | head -3 | tr '\n' ' ') (expected $name.stdout)"
    fi
    if [ -n "$HAVE_REM" ] && [ ! -f "$HERE/programs/normalise/$name.no-rem" ]; then
      if ( cd "$d/rem" && elanc-rem -b -nosplit -quiet "$name" >elanc.log 2>&1 \
           && make -f "$name.make" >make.log 2>&1 && ./a.out -noInput -quiet >run.out 2>&1 ) \
         && cmp -s "$d/rem/run.out" "$stdout"; then
        ok "$name vs REM: the output of $name.stdout"
      else
        fail "$name vs REM: not the output of $name.stdout (see $d/rem)"
      fi
    fi
    continue
  fi
  rs=$( cd "$d/$W" && ./a.out -noInput -quiet 2>"$d/$W/run.err"; echo "status $?" )
  status=$(printf '%s\n' "$rs" | sed -n 's/^status //p')
  rs=$(printf '%s\n' "$rs" | grep '^result = ')
  if [ "$status" != 0 ] || [ -z "$rs" ]; then
    fail "$name: the compiled program failed (status $status, see $d/$W/run.err)"; continue
  fi

  # the expected result, when no oracle runs the program
  if [ -f "$HERE/programs/normalise/$name.expected" ]; then
    expected=$(grep -v '^#' "$HERE/programs/normalise/$name.expected")
    if [ "$rs" = "$expected" ]; then
      ok "$name: $rs ($name.expected)"
    else
      fail "$name: $ELANC [$rs], expected [$expected] ($name.expected)"
    fi
  fi

  # REM
  if [ -z "$HAVE_REM" ]; then
    echo "skip $name vs REM (elanc-rem not installed)"
  elif [ -f "$HERE/programs/normalise/$name.no-rem" ]; then
    echo "skip $name vs REM ($name.no-rem)"
  elif ( cd "$d/rem" && elanc-rem -b -nosplit -quiet "$name" >elanc.log 2>&1 \
       && make -f "$name.make" >make.log 2>&1 ); then
    rem=$( cd "$d/rem" && ./a.out -noInput -quiet 2>&1 | python3 "$HERE/rem_idents.py" "$name.ref" \
           | grep '^result = ' )
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
    fail "$name: elanc-rem or make failed (see $d/rem/*.log)"
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
    # (an empty count would say nothing: the cache is elsewhere)
    if [ "$before" = "$after" ] && [ "$after" -gt 0 ]; then
      ok "peano from another directory: same package ($after files)"
    else
      fail "peano from another directory: $before files, then $after in $TARGET"
    fi
  else
    fail "peano from another directory: $ELANC failed or another result (see $d/$ELANC.log)"
  fi
fi

# elanc-go builds with -buildvcs=false: the work directories are in a git
# work tree, and a VCS stamp would run git and make the build depend on
# the state of the repository.
if [ "$ELANC" = elanc-go ] && [ -x "$RUN/peano/go/a.out" ]; then
  n=$((n+1))
  if ! info=$(${GO:-go} version -m "$RUN/peano/go/a.out" 2>&1); then
    fail "peano: go version -m failed: $info"
  elif printf '%s\n' "$info" | grep -q 'vcs'; then
    fail "peano: the program has VCS stamps: $(printf '%s\n' "$info" | grep vcs | tr '\n' ' ')"
  else
    ok "peano: no VCS stamp in the program"
  fi
fi

for lgi in "$HERE"/programs/refused/*.lgi; do
  name=$(basename "$lgi" .lgi)
  [ -z "$SELECTION" ] || selected "$name" || continue
  if [ -f "$HERE/programs/refused/$name.no-$ELANC" ]; then
    echo "skip refused $name ($name.no-$ELANC)"; continue
  fi
  n=$((n+1))
  d=$RUN/refused-$name
  mkdir "$d" || exit 2
  cp "$HERE/programs/refused/$name.lgi" "$HERE/programs/refused/"*.eln "$d/" || exit 2
  echo stale >"$d/a.out" || exit 2  # a refused program leaves no a.out
  ( cd "$d" && $ELANC $CACHE_OPT "$TARGET" "$name" >$ELANC.log 2>&1 )
  status=$?
  msg=$(grep -E "$REFUSED" "$d/$ELANC.log")
  if [ "$status" = 2 ] && [ -n "$msg" ] && [ ! -e "$d/a.out" ]; then
    ok "refused $name: $msg"
  else
    fail "refused $name: status $status, expected 2 and a message ($REFUSED), no a.out (see $d/$ELANC.log)"
  fi
done

echo "$n programs, $fails failures"
# the run directory is kept only when something failed (to look at it)
if [ "$fails" -eq 0 ]; then rm -rf "$RUN"; else echo "kept for inspection: $RUN"; fi
[ "$fails" -eq 0 ]
