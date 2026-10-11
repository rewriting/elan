#!/bin/sh
# What bin/elanc is (S7e task 11): a relative symlink chosen by the CMake
# variable ELAN_ELANC, `go` (the default: elanc-go) or `rem` (elanc-rem, REM's
# driver). With ELAN_ELANC=go and elanc-go not built (no go >= 1.24,
# -DELAN_GO=OFF), CMake falls back to `rem` with a STATUS message.
#
# Usage: test_elanc_switch.sh PREFIX [go|rem]   (default go)
#   go:  bin/elanc -> elanc-go (or -> elanc-rem when elanc-go is not
#        installed: the fallback); elanc builds ./a.out itself; REM's
#        options that change the result are refused (status 2, naming
#        elanc-rem); elanc-rem and its make still work
#   rem: bin/elanc -> elanc-rem, REM's driver
# CMAKE_CHECK=1 also configures scratch build directories to check the
# fallback and the errors of ELAN_ELANC (cmake only, nothing built), the
# default with the go of the PATH, a fake go1.22 and no go.
PREFIX=${1:?usage: test_elanc_switch.sh PREFIX [go|rem]}
WANT=${2:-go}
HERE=$(cd "$(dirname "$0")" && pwd)
REPO=$(cd "$HERE/../.." && pwd)
WORK=$HERE/work/switch
PREFIX=$(cd "$PREFIX" && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH
ELANC="$PREFIX/bin/elanc"   # bare-elanc-ok: this test is about what bin/elanc is
SNAPSHOT=$REPO/tests/legacy-bench/snapshots/elan_sources_Compiler.4.0_Test_J_enum_no_-_enum-6acca3e8.out
fails=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }

fresh() { # fresh <name>: a work copy of the enum example
  rm -rf "$WORK/$1"; mkdir -p "$WORK/$1"
  cp "$REPO"/legacy/elan/sources/Compiler.4.0/Test/enum.* "$WORK/$1/"
}
# same_as_snapshot <dir>: ./a.out -noInput -quiet prints the J snapshot
# (rewrite_step excluded: the counts depend on the compilation scheme)
same_as_snapshot() {
  ( cd "$1" && ./a.out -noInput -quiet ) 2>/dev/null | grep -v '^rewrite_step = ' > "$1/out"
  grep -v '^rewrite_step = ' "$SNAPSHOT" | cmp -s - "$1/out"
}

case "$WANT" in
  go)  if [ -x "$PREFIX/bin/elanc-go" ]; then target=elanc-go; else target=elanc-rem; fi ;;
  rem) target=elanc-rem ;;
  *)   echo "test_elanc_switch.sh: go or rem, not '$WANT'"; exit 2 ;;
esac
echo "ELAN_ELANC=$WANT: bin/elanc should be a symlink to $target"  # bare-elanc-ok: the name in a message

# 1. bin/elanc is a relative symlink to the chosen compiler
if [ -L "$ELANC" ] && [ "$(readlink "$ELANC")" = "$target" ]; then
  ok "elanc_is_a_link_to_$target"
else
  fail "elanc_is_a_link_to_$target (got: $(ls -l "$ELANC" 2>&1))"
fi
if [ -x "$PREFIX/bin/elanc-rem" ] || [ ! -d "$PREFIX/classes" ]; then ok "elanc_rem_installed"
else fail "elanc_rem_installed"; fi

if [ "$target" = elanc-go ]; then
  # 2. its usage is elanc-go's, under the name elanc
  if "$ELANC" -help 2>&1 | head -1 | grep -q '^usage: elanc \[options\] program\[.lgi\] \[specification\[.spc\]\]$'; then  # bare-elanc-ok: the name in a message
    ok "elanc_usage_is_go"
  else
    fail "elanc_usage_is_go (got: $("$ELANC" -help 2>&1 | head -1))"
  fi
  # 3. `elanc -b enum` builds ./a.out itself (no .make), whose output is REM's
  fresh build
  if ( cd "$WORK/build" && "$ELANC" -b enum >/dev/null 2>&1 ) && [ -x "$WORK/build/a.out" ] \
     && [ ! -f "$WORK/build/enum.make" ] && same_as_snapshot "$WORK/build"; then
    ok "elanc_builds_a_out_as_rem"
  else
    fail "elanc_builds_a_out_as_rem"
  fi
  # 4. REM's common command line: options without effect ignored, -o/-output
  #    name the program, the .lgi suffix and a directory accepted
  fresh common
  if ( cd "$WORK/common" && "$ELANC" -nosplit -quiet -O -optimiseChoicePoint -strategy 0 enum.lgi >/dev/null 2>&1 ) \
     && same_as_snapshot "$WORK/common"; then
    ok "elanc_accepts_rems_common_options"
  else
    fail "elanc_accepts_rems_common_options"
  fi
  fresh output
  mkdir -p "$WORK/output/run"
  if ( cd "$WORK/output/run" && "$ELANC" -output enumprog ../enum >/dev/null 2>&1 && "$ELANC" -o other ../enum.lgi >/dev/null 2>&1 ) \
     && [ -x "$WORK/output/run/enumprog" ] && [ -x "$WORK/output/run/other" ] && [ ! -e "$WORK/output/run/a.out" ]; then
    ok "elanc_output_option"
  else
    fail "elanc_output_option"
  fi
  # 5. REM's options that change the result or the files written: refused,
  #    status 2, a message naming elanc-rem, no a.out left behind
  for opt in -coq -proofterm -lib -debug -onlyC -noC -nocode "-strategy 1" "-strategy 2"; do
    fresh refused
    touch "$WORK/refused/a.out"
    # shellcheck disable=SC2086
    msg=$( cd "$WORK/refused" && "$ELANC" -nosplit $opt enum 2>&1 >/dev/null ); st=$?
    if [ "$st" -eq 2 ] && [ "$msg" = "elanc: option $opt is REM's: use elanc-rem" ] && [ ! -e "$WORK/refused/a.out" ]; then  # bare-elanc-ok: the name in a message
      ok "elanc_refuses $opt"
    else
      fail "elanc_refuses $opt (status $st: $msg)"
    fi
  done
  # 6. REM is still there as elanc-rem: its .make, make, the same output
  if [ -x "$PREFIX/bin/elanc-rem" ]; then
    fresh rem
    if ( cd "$WORK/rem" && elanc-rem -nosplit -quiet enum >/dev/null 2>&1 && make -f enum.make >/dev/null 2>&1 ) \
       && same_as_snapshot "$WORK/rem"; then
      ok "elanc_rem_and_make_still_work"
    else
      fail "elanc_rem_and_make_still_work"
    fi
  fi
else
  # 2. bin/elanc is REM's driver: its usage, the .make it writes, make
  if "$ELANC" 2>&1 | grep -q 'Reduce ELAN Machine'; then ok "elanc_usage_is_rem"
  else fail "elanc_usage_is_rem"; fi
  fresh rem
  if ( cd "$WORK/rem" && "$ELANC" -nosplit -quiet enum >/dev/null 2>&1 && make -f enum.make >/dev/null 2>&1 ) \
     && same_as_snapshot "$WORK/rem"; then
    ok "elanc_is_rem_with_its_make"
  else
    fail "elanc_is_rem_with_its_make"
  fi
fi

# 7. CMake: the fallback to rem when elanc-go is not built, the errors
if [ "${CMAKE_CHECK:-}" = 1 ]; then
  configure() { # configure <name> <cmake options>: its output in $WORK/<name>.log
    n=$1; shift
    rm -rf "$WORK/cfg-$n"
    cmake -S "$REPO" -B "$WORK/cfg-$n" "$@" > "$WORK/$n.log" 2>&1
  }
  if configure nogo -DELAN_GO=OFF && grep -q 'ELAN_ELANC=go but elanc-go is not built: bin/elanc is elanc-rem' "$WORK/nogo.log"; then  # bare-elanc-ok: the name in a message
    ok "cmake_falls_back_to_rem_without_go"
  else
    fail "cmake_falls_back_to_rem_without_go"
  fi
  # the default (ELAN_ELANC=go): elanc-go when the go of the PATH is
  # >= 1.24, else the fallback; checked with the PATH as it is, with a
  # fake old go first in the PATH, and with a PATH without go (a directory
  # of links to the programs of the PATH, go and gofmt left out; CMake's
  # system prefixes not searched)
  mkdir -p "$WORK/oldgo" "$WORK/nogo"
  printf '#!/bin/sh\n[ "$1 $2" = "env GOVERSION" ] && echo go1.22.0\n' > "$WORK/oldgo/go"
  chmod +x "$WORK/oldgo/go"
  (IFS=:; for d in $PATH; do
     for f in "$d"/*; do
       b=${f##*/}
       case $b in go|gofmt) continue ;; esac
       [ -x "$f" ] && [ ! -d "$f" ] && [ ! -e "$WORK/nogo/$b" ] && ln -s "$f" "$WORK/nogo/$b"
     done
   done)
  go_ok() { # whether the go of the PATH is >= 1.24 (CMake's test)
    v=$(GOTOOLCHAIN=local go env GOVERSION 2>/dev/null) || return 1
    v=${v#go}; maj=${v%%.*}; min=${v#*.}; min=${min%%[!0-9]*}
    [ -n "$maj" ] && [ -n "$min" ] || return 1
    [ "$maj" -gt 1 ] || { [ "$maj" -eq 1 ] && [ "$min" -ge 24 ]; }
  }
  default_case() { # default_case <name> <PATH> <cmake options>
    n=$1 p=$2; shift 2
    if (PATH=$p; go_ok); then
      expect='bin/elanc is elanc-go (ELAN_ELANC=go)'  # bare-elanc-ok: the name in a message
    else
      expect='ELAN_ELANC=go but elanc-go is not built: bin/elanc is elanc-rem'  # bare-elanc-ok: the name in a message
    fi
    if (PATH=$p; configure "$n" "$@") && grep -qF "$expect" "$WORK/$n.log"; then
      ok "cmake_elanc_default_$n"
    else
      fail "cmake_elanc_default_$n (expected: $expect)"
    fi
  }
  default_case go "$PATH"
  default_case oldgo "$WORK/oldgo:$PATH"
  # (CMake's find_program also looks in its system prefixes: off)
  default_case nogo "$WORK/nogo" -DCMAKE_FIND_USE_CMAKE_SYSTEM_PATH=OFF
  if grep -q 'go1.22.0 is older than go1.24: elanc-go is not built' "$WORK/oldgo.log"; then ok "cmake_sees_an_old_go"
  else fail "cmake_sees_an_old_go"; fi
  if grep -q 'go not found: elanc-go is not built' "$WORK/nogo.log"; then ok "cmake_sees_no_go"
  else fail "cmake_sees_no_go"; fi
  if configure bogus -DELAN_ELANC=java; then fail "cmake_refuses_a_bad_value"
  elif grep -q 'ELAN_ELANC must be go or rem' "$WORK/bogus.log"; then ok "cmake_refuses_a_bad_value"
  else fail "cmake_refuses_a_bad_value (another error)"; fi
  if configure remoff -DELAN_ELANC=rem -DELAN_COMPILER=OFF; then fail "cmake_refuses_rem_without_rem"
  elif grep -q 'ELAN_ELANC=rem needs the REM compiler' "$WORK/remoff.log"; then ok "cmake_refuses_rem_without_rem"
  else fail "cmake_refuses_rem_without_rem (another error)"; fi
  if configure none -DELAN_GO=OFF -DELAN_COMPILER=OFF && grep -q 'no compiler is built: bin/elanc is not installed' "$WORK/none.log"; then  # bare-elanc-ok: the name in a message
    ok "cmake_no_elanc_without_compilers"
  else
    fail "cmake_no_elanc_without_compilers"
  fi
fi

rm -rf "$WORK"
echo "test_elanc_switch: $fails failure(s)"
[ "$fails" -eq 0 ]
