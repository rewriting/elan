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

# 5. REM exits with a non-zero status on errors: missing .ref, parse error
fresh remstatus
( cd "$WORK/remstatus" && elanc -nosplit -quiet enum >/dev/null 2>&1 )
if ( cd "$WORK/remstatus" && "$JAVA" -cp "$PREFIX/classes" rem.REM missing.ref -quiet >/dev/null 2>&1 ); then
  fail "rem_fails_on_a_missing_ref"
else
  ok "rem_fails_on_a_missing_ref"
fi
echo "1" > "$WORK/remstatus/bad.ref"   # lexically valid: a ParseException
if ( cd "$WORK/remstatus" && "$JAVA" -cp "$PREFIX/classes" rem.REM bad.ref -quiet >/dev/null 2>&1 ); then
  fail "rem_fails_on_a_parse_error"
else
  ok "rem_fails_on_a_parse_error"
fi
if ( cd "$WORK/remstatus" && "$JAVA" -cp "$PREFIX/classes" rem.REM enum.ref -nosplit -nocode -quiet >/dev/null 2>&1 ); then
  ok "rem_succeeds_on_a_valid_ref"
else
  fail "rem_succeeds_on_a_valid_ref"
fi

# 6. elanc fails when REM fails (an option without its argument makes REM fail)
fresh elancrem
if ( cd "$WORK/elancrem" && elanc -nosplit -quiet enum -strategy >/dev/null 2>&1 ); then
  fail "elanc_fails_when_rem_fails"
else
  ok "elanc_fails_when_rem_fails"
fi

# 7. the generated Makefile compiles and links with $(ELAN_SANITIZE) (the
#    sanitizer options of a sanitizer build, empty otherwise)
mk="$WORK/gcprefix/.elan.enum/Makefile"
if [ "$(grep -c '^CC = $(ELAN_CC) .*$(ELAN_SANITIZE)$' "$mk")" -eq 2 ] \
   && [ "$(grep -c '^CXX = $(ELAN_CXX) .*$(ELAN_SANITIZE)$' "$mk")" -eq 2 ]; then
  ok "generated_makefile_uses_elan_sanitize"
else
  fail "generated_makefile_uses_elan_sanitize"
fi

# 5. integers print as the 2004 compiled code (and the interpreter) did: the
#    low 32 bits (65536*65536 -> 0), not as a 64-bit value
mkdir -p "$WORK/int32"
cat > "$WORK/int32/sq.eln" <<'ELN'
module sq
import global int; end
operators global sq(@) : (int) int; end
rules for int
  n : int;
global
  [] sq(n) => n*n end
end
end
ELN
cat > "$WORK/int32/sq.lgi" <<'LGI'
LPL sq description
  query of sort int
  result of sort int
  import sq
  start with () sq(65536)
end
LGI
out=$( cd "$WORK/int32" && elanc -nosplit -quiet sq >/dev/null 2>&1 && make -f sq.make >/dev/null 2>&1 && ./a.out -noInput -quiet )
if echo "$out" | grep -q '^result = 0$'; then ok "compiled_int_prints_low_32_bits"
else fail "compiled_int_prints_low_32_bits (got: $(echo "$out" | grep result))"; fi

rm -rf "$WORK"
# 8. the generated main Makefile fails when the build fails, and succeeds
#    with -output, a plain name or an absolute path (copied to the main
#    directory, as in 2004)
fresh makestatus
( cd "$WORK/makestatus" && elanc -nosplit -quiet enum >/dev/null 2>&1 )
echo "syntax error" >> "$WORK/makestatus/.elan.enum/enum.c"
if ( cd "$WORK/makestatus" && make -f enum.make >/dev/null 2>&1 ); then
  fail "generated_makefile_fails_when_the_build_fails"
else
  ok "generated_makefile_fails_when_the_build_fails"
fi
for out in prog "$WORK/outputabs/abs/prog"; do
  fresh outputabs; mkdir -p "$WORK/outputabs/abs"
  if ( cd "$WORK/outputabs" && elanc -nosplit -quiet -output "$out" enum >/dev/null 2>&1 \
       && make -f enum.make >/dev/null 2>&1 ) && [ -x "$WORK/outputabs/prog" ] \
     && ( cd "$WORK/outputabs" && [ -x "$out" ] ); then
    ok "generated_makefile_with_output $out"
  else
    fail "generated_makefile_with_output $out"
  fi
done

echo "$fails failed"
[ "$fails" -eq 0 ]
