#!/bin/sh
# Guard of decision D2 (spec S8, S8-11): the trace hooks of elan-lextrace in
# src/interpreter/lex/mlstream.cc must not change elan. The working copy and
# BASE's copy (default: main) are preprocessed WITHOUT ELAN_LEXTRACE, with the
# include directories of the build, and compared token by token (the hooks are
# all under #ifdef ELAN_LEXTRACE). As a self-check, the working copy
# preprocessed WITH the macro must differ (else the guard proves nothing).
# Usage: tests/front/check_lextrace_hooks.sh [BUILD] [BASE]
set -eu
ROOT=$(cd "$(dirname "$0")/../.." && pwd)
BUILD=${1:-build}
BASE=${2:-main}
case "$BUILD" in /*) ;; *) BUILD=$ROOT/$BUILD ;; esac
CXX=${CXX:-c++}
F=src/interpreter/lex/mlstream.cc
GEN=$BUILD/src/interpreter/generated
[ -d "$GEN" ] || { echo "check_lextrace_hooks: $GEN missing (run make first)"; exit 1; }

W=$(mktemp -d "${TMPDIR:-/tmp}/lextrace-hooks.XXXXXX")
trap 'rm -rf "$W"' EXIT INT TERM
git -C "$ROOT" show "$BASE:$F" > "$W/base.cc"

INC="-I$GEN"
for m in base lex parse term match rewrite load meta ref peval command compile driver; do
  INC="$INC -I$ROOT/src/interpreter/$m"
done
# shellcheck disable=SC2086
pp() { # pp FILE [extra flags]: the token stream, one token-ish per line
  f=$1; shift
  $CXX -std=c++17 -fsigned-char -E -P -DPREFIX='"/prefix"' -DPACKAGE='"elan-interpreter"' \
       -DVERSION='"3.6g"' -DYYTEXT_POINTER=1 $INC "$@" -x c++ "$f" | tr -s '[:space:]' '\n'
}
pp "$W/base.cc" > "$W/base.tok"
pp "$ROOT/$F" > "$W/work.tok"
pp "$ROOT/$F" -DELAN_LEXTRACE > "$W/trace.tok"

if ! cmp -s "$W/base.tok" "$W/work.tok"; then
  echo "check_lextrace_hooks: FAIL: $F without ELAN_LEXTRACE differs from $BASE's:"
  diff "$W/base.tok" "$W/work.tok" | head -20
  exit 1
fi
if cmp -s "$W/work.tok" "$W/trace.tok"; then
  echo "check_lextrace_hooks: FAIL: ELAN_LEXTRACE changes nothing (are the hooks there?)"
  exit 1
fi
echo "check_lextrace_hooks: ok ($F without ELAN_LEXTRACE is token-identical to $BASE's)"
