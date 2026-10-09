#!/bin/sh
# Tests of the compiler driver elanc and of the Makefile REM generates.
# Usage: test_elanc.sh PREFIX     (an installed ELAN with elanc)
PREFIX=${1:?usage: test_elanc.sh PREFIX}
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)
WORK=$HERE/work
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH
fails=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }

fresh() { # fresh <name>: a work copy of the enum example
  rm -rf "$WORK/$1"; mkdir -p "$WORK/$1"
  cp "$REPO"/legacy/elan/sources/Compiler.4.0/Test/enum.* "$WORK/$1/"
}

# 1. a front-end (elan --cexport) error makes elanc fail, even with a stale .ref
fresh frontend
( cd "$WORK/frontend" && elanc -nosplit -quiet enum >/dev/null 2>&1 )
echo "this is not ELAN" >> "$WORK/frontend/enum.eln"
if ( cd "$WORK/frontend" && elanc -nosplit -quiet enum >/dev/null 2>&1 ); then
  fail "elanc_fails_when_the_export_fails"
else
  ok "elanc_fails_when_the_export_fails"
fi

# 2. the Linux branch of the generated Makefile uses GC_PREFIX like the Darwin one
fresh gcprefix
( cd "$WORK/gcprefix" && elanc -nosplit -quiet enum >/dev/null 2>&1 )
mk="$WORK/gcprefix/.elan.enum/Makefile"
if grep -q '^CC = $(ELAN_CC)' "$mk" && ! grep '^CC = $(ELAN_CC)' "$mk" | grep -vq -- '-I$(GC_PREFIX)/include' \
   && grep -q '^CXX = $(ELAN_CXX)' "$mk" && ! grep '^CXX = $(ELAN_CXX)' "$mk" | grep -vq -- '-L$(GC_PREFIX)/lib'; then
  ok "generated_linux_makefile_uses_gc_prefix"
else
  fail "generated_linux_makefile_uses_gc_prefix"
fi

# 3. the ATerm runtime is rejected
if elanc -aterm enum >/dev/null 2>&1; then fail "elanc_rejects_aterm"; else ok "elanc_rejects_aterm"; fi

# 4. REM itself rejects the ATerm runtime options and no longer advertises them
JAVA=$(sed -n 's/^JAVA="\(.*\)"$/\1/p' "$PREFIX/bin/elanc")
fresh rem
( cd "$WORK/rem" && elanc -nosplit -quiet enum >/dev/null 2>&1 )
if ( cd "$WORK/rem" && "$JAVA" -cp "$PREFIX/classes" rem.REM enum.ref -aterm >/dev/null 2>&1 ); then
  fail "rem_rejects_aterm"
elif "$JAVA" -cp "$PREFIX/classes" rem.REM 2>&1 | grep -q aterm; then
  fail "rem_rejects_aterm (still in the usage text)"
else
  ok "rem_rejects_aterm"
fi

rm -rf "$WORK"
echo "$fails failed"
[ "$fails" -eq 0 ]
