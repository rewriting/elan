#!/bin/sh
# Guard of decision D2 (spec S8, S8-11): the trace hooks of elan-lextrace in
# src/interpreter/lex/mlstream.cc must not change elan. Stable invariants,
# true on any branch, before and after a merge:
#   1. mlstream.cc preprocessed WITHOUT ELAN_LEXTRACE, with the flags elan's
#      own compilation uses (its entry in BUILD/compile_commands.json if there
#      is one, else CMake's flags.make of elan_core, else the CMake defaults
#      below), has no token containing "lextrace" (any case);
#   2. every source line mentioning lextrace (any case, comments included) is
#      inside an `#ifdef ELAN_LEXTRACE` block (not in its #else branch), or is
#      that #ifdef line itself;
#   3. self-check: preprocessed WITH -DELAN_LEXTRACE the token stream differs
#      (else the guard proves nothing).
# Optional: with BASE (a git ref), BASE's copy preprocessed the same way must
# be token-identical to the working copy (only meaningful while BASE has the
# same code outside the hooks: a review aid, not a stable invariant).
# Usage: tests/front/check_lextrace_hooks.sh [--file F] [BUILD] [BASE]
#   --file F: check F instead of src/interpreter/lex/mlstream.cc (tests).
set -eu
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
REL=src/interpreter/lex/mlstream.cc
F=$ROOT/$REL
if [ "${1:-}" = --file ]; then F=${2:?--file F}; shift 2; fi
BUILD=${1:-build}
BASE=${2:-}
case "$BUILD" in /*) ;; *) BUILD=$ROOT/$BUILD ;; esac
GEN=$BUILD/src/interpreter/generated
[ -d "$GEN" ] || { echo "check_lextrace_hooks: $GEN missing (run make first)"; exit 1; }

W=$(mktemp -d "${TMPDIR:-/tmp}/lextrace-hooks.XXXXXX")
trap 'rm -rf "$W"' EXIT INT TERM

# The compiler and flags of elan's mlstream.o, one argument per line in
# $W/flags (without -c, -o and the source file).
CC_JSON=$BUILD/compile_commands.json
FLAGS_MAKE=$BUILD/src/interpreter/CMakeFiles/elan_core.dir/flags.make
if [ -f "$CC_JSON" ] && python3 - "$CC_JSON" "$REL" > "$W/flags" <<'EOF'
import json, shlex, sys
for e in json.load(open(sys.argv[1])):
    args = e.get("arguments") or shlex.split(e["command"])
    if e["file"].endswith(sys.argv[2]) and "-DELAN_LEXTRACE" not in args:
        print(args[0])
        skip = False
        for a in args[1:]:
            if skip:
                skip = False
                continue
            if a in ("-o", "-MF", "-MT", "-MQ"):
                skip = True
                continue
            if a in ("-c", "-MD", "-MMD") or a.endswith(sys.argv[2]):
                continue
            print(a)
        sys.exit(0)
sys.exit(1)
EOF
then SRC="compile_commands.json"
elif [ -f "$FLAGS_MAKE" ]; then
  SRC="flags.make of elan_core"
  cxx=$(sed -n 's/^# compile CXX with //p' "$FLAGS_MAKE")
  defs=$(sed -n 's/^CXX_DEFINES = //p' "$FLAGS_MAKE")
  incs=$(sed -n 's/^CXX_INCLUDES = //p' "$FLAGS_MAKE")
  cxxf=$(sed -n 's/^CXX_FLAGS = //p' "$FLAGS_MAKE")
  eval "set -- $cxx $defs $incs $cxxf"
  for a in "$@"; do echo "$a"; done > "$W/flags"
else
  SRC="CMake defaults"
  { echo "${CXX:-c++}"
    printf '%s\n' -std=c++17 -fsigned-char -DPREFIX='"/prefix"' -DPACKAGE='"elan-interpreter"' \
      -DVERSION='"3.6g"' -DYYTEXT_POINTER=1 "-I$GEN"
    for m in base lex parse term match rewrite load meta ref peval command compile driver; do
      echo "-I$ROOT/src/interpreter/$m"; done; } > "$W/flags"
fi
if grep -q -- '-DELAN_LEXTRACE' "$W/flags"; then
  echo "check_lextrace_hooks: FAIL: elan's flags ($SRC) define ELAN_LEXTRACE"; exit 1
fi

pp() { # pp FILE [extra flags]: the token stream, one token-ish per line
  f=$1; shift
  ( IFS='
'
    # shellcheck disable=SC2046
    set -- $(cat "$W/flags") "$@"; "$@" -E -P -x c++ "$f" ) | tr -s '[:space:]' '\n'
}
pp "$F" > "$W/work.tok"
pp "$F" -DELAN_LEXTRACE > "$W/trace.tok"
[ -s "$W/work.tok" ] || { echo "check_lextrace_hooks: FAIL: empty preprocessor output"; exit 1; }

fail=0
# 1. no lextrace token in elan's preprocessed source
if grep -in lextrace "$W/work.tok" > "$W/leak"; then
  echo "check_lextrace_hooks: FAIL: $F preprocessed without ELAN_LEXTRACE ($SRC) has lextrace tokens:"
  head -5 "$W/leak"; fail=1
fi
# 2. every line mentioning lextrace is inside #ifdef ELAN_LEXTRACE
if ! awk '
  function guarded(  i) { for (i = 1; i <= d; i++) if (g[i]) return 1; return 0 }
  /^[ \t]*#[ \t]*if/ {
    d++
    g[d] = ($0 ~ /^[ \t]*#[ \t]*ifdef[ \t]+ELAN_LEXTRACE([ \t]|\/|$)/ || \
            $0 ~ /^[ \t]*#[ \t]*if[ \t]+defined[ \t]*\(?[ \t]*ELAN_LEXTRACE[ \t]*\)?([ \t]|\/|$)/)
    if (g[d]) next
  }
  /^[ \t]*#[ \t]*(else|elif)/ { if (d > 0) g[d] = 0 }
  /^[ \t]*#[ \t]*endif/ {
    if (tolower($0) ~ /lextrace/ && !guarded()) bad[++nb] = FNR ": " $0
    d--; next
  }
  tolower($0) ~ /lextrace/ && !guarded() { bad[++nb] = FNR ": " $0 }
  END { for (i = 1; i <= nb; i++) print bad[i]; exit nb > 0 }' "$F" > "$W/unguarded"; then
  echo "check_lextrace_hooks: FAIL: lines of $F mentioning lextrace outside #ifdef ELAN_LEXTRACE:"
  head -5 "$W/unguarded"; fail=1
fi
# 3. the hooks are there
if cmp -s "$W/work.tok" "$W/trace.tok"; then
  echo "check_lextrace_hooks: FAIL: ELAN_LEXTRACE changes nothing in $F (are the hooks there?)"; fail=1
fi
# optional: token-identical to BASE's copy
if [ -n "$BASE" ]; then
  git -C "$ROOT" show "$BASE:$REL" > "$W/base.cc"
  pp "$W/base.cc" > "$W/base.tok"
  if ! cmp -s "$W/base.tok" "$W/work.tok"; then
    echo "check_lextrace_hooks: FAIL: $F without ELAN_LEXTRACE differs from $BASE's:"
    diff "$W/base.tok" "$W/work.tok" | head -20; fail=1
  fi
fi
[ $fail -eq 0 ] || exit 1
extra=; [ -z "$BASE" ] || extra=", token-identical to $BASE's copy"
echo "check_lextrace_hooks: ok ($F, flags from $SRC: no lextrace token without ELAN_LEXTRACE, every lextrace line guarded$extra)"
