#!/bin/bash
# Reference build of ELAN 3 (2004) on a modern system.
#
# Copies the untouched sources from ../legacy into build/, applies the minimal
# portability patches of patches/ (see README.md), and installs the interpreter
# `elan`, the compiler `elanc` (REM), the runtime libraries and the standard
# library into install/ (override with PREFIX=...).
#
# This build is the *behavioural reference* for the modern port: the test
# bench in ../tests/legacy-bench is validated against it.
#
# Requirements (Homebrew on macOS arm64): gcc (gcc-16/g++-16), openjdk,
# bdw-gc, bison, flex, libtool.  Must run on a case-sensitive file system.
#
# Usage: ./build.sh [component...]   components: prepare aterm cpl interpreter compiler library
#        (no argument = all, in dependency order)
set -euo pipefail

HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$HERE/.." && pwd)"
LEGACY="$REPO/legacy"
BUILD="$HERE/build"
SRC="$BUILD/elan3/src"
PREFIX="${PREFIX:-$HERE/install}"
LOGDIR="$BUILD/logs"

GCC="${ELAN_CC:-gcc-16}"
GXX="${ELAN_CXX:-g++-16}"
GC_PREFIX="${GC_PREFIX:-/opt/homebrew/opt/bdw-gc}"
JAVA_HOME="${JAVA_HOME:-/opt/homebrew/opt/openjdk}"
AUX="${AUX:-$(dirname "$(find /opt/homebrew/Cellar/libtool -name config.guess -path '*build-aux*' | head -1)")}"
export PATH="$JAVA_HOME/bin:$PATH"

# Pre-standard C: K&R prototypes, tentative definitions shared between objects
CC_FLAGS="$GCC -std=gnu89 -w -fcommon"
# Pre-standard C++: <iostream.h>, extra qualifications, ...
CXX_FLAGS="$GXX -std=gnu++98 -fpermissive -w -I$HERE/compat"
AR=/usr/bin/ar

log() { printf '\n=== %s\n' "$*"; }
run() { # run <logname> <cmd...>: quiet, show the log tail on failure
  local name=$1; shift
  if ! "$@" >>"$LOGDIR/$name.log" 2>&1; then
    echo "FAILED: $* (see $LOGDIR/$name.log)"; tail -30 "$LOGDIR/$name.log"; exit 1
  fi
}

case_sensitive_or_die() {
  local t; t=$(mktemp -d "$HERE/.casetest.XXXX")
  touch "$t/a" "$t/A"
  if [ "$(ls "$t" | wc -l)" -ne 2 ]; then
    rm -rf "$t"; echo "ERROR: $HERE is on a case-insensitive file system (see README.md)"; exit 1
  fi
  rm -rf "$t"
}

# Copies scramble timestamps: make would rerun aclocal/automake/autoconf 1.x,
# which are not installed. Order the generated files.
freeze_autotools() {
  touch aclocal.m4 2>/dev/null || true; sleep 1
  touch configure config.h.in 2>/dev/null || true
  find . -name Makefile.in -exec touch {} + ; sleep 1
  find . -name 'config.h.in' -exec touch {} + 2>/dev/null || true
}

build_prepare() {
  log "Preparing sources from legacy/ + patches/"
  case_sensitive_or_die
  rm -rf "$BUILD"; mkdir -p "$SRC" "$LOGDIR"
  cp -R "$LEGACY/elan3/src/." "$SRC/"
  cp -R "$LEGACY/elan/src/aterm-1.6.5" "$SRC/"
  for p in "$HERE"/patches/*.patch; do
    echo "  applying $(basename "$p")"
    run prepare patch -p1 -d "$BUILD" -i "$p"
  done
  # cpl: the CVS checkout lacks the generated Makefile.in; take them from the
  # cpl-0.6 distribution shipped next to it (same configure.in).
  (cd "$SRC/cpl" && tar xzf cpl-0.6.tar.gz --strip-components=1 \
     cpl-0.6/Makefile.in cpl-0.6/src/Makefile.in cpl-0.6/doc/Makefile.in cpl-0.6/test/Makefile.in)
  # Dangling symlinks to the autotools of 2003 (/sw/share/automake-1.7/...):
  # replace them with current copies. Old config.guess/sub do not know arm64.
  find "$SRC" -type l ! -exec test -e {} \; -print | while read -r l; do
    n=$(basename "$l"); rm "$l"
    if [ -f "$AUX/$n" ]; then cp "$AUX/$n" "$l"
    elif [ "$n" = mkinstalldirs ]; then printf '#!/bin/sh\nexec mkdir -p "$@"\n' >"$l"; chmod +x "$l"
    fi
  done
  for d in cpl elan-interpreter elan-compiler elan-library aterm-1.6.5; do
    for f in config.guess config.sub; do [ -f "$SRC/$d/$f" ] && cp "$AUX/$f" "$SRC/$d/$f"; done
  done
  # Objects/archives of the 2003 builds (i386 ELF) and stale configure caches
  find "$SRC" \( -name '*.o' -o -name '*.a' -o -name config.cache -o -name config.status \) -delete
  rm -f "$SRC/elan-compiler/scripts/elanc"   # generated in 2003 for another prefix
}

configure_clean() { [ -f Makefile ] && make distclean >/dev/null 2>&1 || true; freeze_autotools; }

build_aterm() {
  log "ATerm 1.6.5"
  cd "$SRC/aterm-1.6.5"; configure_clean
  run aterm env CC="$CC_FLAGS" AR=$AR ./configure --prefix="$PREFIX"
  # -DPROTOTYPES=1: md5.h otherwise declares K&R prototypes rejected by GCC 16.
  # The 'test' directory needs generated files that are not shipped: ignore it.
  make -k install CC="$CC_FLAGS -DPROTOTYPES=1" AR=$AR >>"$LOGDIR/aterm.log" 2>&1 || true
  [ -f "$PREFIX/lib/libATerm.a" ] || { echo "libATerm.a missing (see $LOGDIR/aterm.log)"; exit 1; }
}

build_cpl() {
  log "Choice-Point Library (cpl)"
  cd "$SRC/cpl"; configure_clean
  run cpl env CC="$CC_FLAGS" ./configure --prefix="$PREFIX" --with-aterm="$PREFIX"
  run cpl make install CC="$CC_FLAGS" CC_GCC="$CC_FLAGS" CC_DBG="$CC_FLAGS -g" \
      CC_PROF="$CC_FLAGS -pg" AR=$AR
}

build_interpreter() {
  log "ELAN interpreter"
  cd "$SRC/elan-interpreter"; configure_clean
  run interpreter env CC="$CC_FLAGS" CXX="$CXX_FLAGS" ./configure --prefix="$PREFIX"
  run interpreter make install CC="$CC_FLAGS" CXX="$CXX_FLAGS" AR=$AR
}

build_compiler() {
  log "ELAN compiler (REM + runtime)"
  cd "$SRC/elan-compiler"; configure_clean
  local inc="-I$GC_PREFIX/include -I$PREFIX/include"
  run compiler env CC="$CC_FLAGS" CXX="$CXX_FLAGS" ./configure --prefix="$PREFIX" \
      --with-cpl="$PREFIX" --with-aterm="$PREFIX" --with-gc="$GC_PREFIX"
  run compiler make install CC="$CC_FLAGS $inc" CXX="$CXX_FLAGS $inc" \
      CC_GCC="$CC_FLAGS $inc" CC_DBG="$CC_FLAGS $inc -g" CC_PROF="$CC_FLAGS $inc -pg" \
      CC_NS="$CC_FLAGS $inc" AR=$AR JAVAC="javac -nowarn -encoding ISO-8859-1"
}

build_library() {
  log "ELAN standard library (elanlib)"
  cd "$SRC/elan-library"; configure_clean
  run library ./configure --prefix="$PREFIX"
  run library make install
}

components=("$@")
[ ${#components[@]} -eq 0 ] && components=(prepare aterm cpl interpreter compiler library)
mkdir -p "$LOGDIR" "$PREFIX"
for c in "${components[@]}"; do "build_$c"; done

log "Done. Installed in $PREFIX"
echo "Use:  source $HERE/env.sh"
