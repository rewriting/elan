#!/bin/sh
# Tests of the guard check_lextrace_hooks.sh (plan S8a, Task 10 fix 2): it
# passes on the real mlstream.cc without a base reference, and fails on temp
# copies of it where a hook is placed outside #ifdef ELAN_LEXTRACE:
#   1. a declaration (a lextrace token in elan's preprocessed source);
#   2. a comment only (not in the token stream: the source-line invariant);
#   3. a hook in the #else branch of an #ifdef ELAN_LEXTRACE block;
# and on a copy without any hook (ELAN_LEXTRACE must change the tokens).
# Usage: tests/front/test_check_lextrace_hooks.sh [BUILD]
set -u
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
BUILD=${1:-build}
G=$HERE/check_lextrace_hooks.sh
F=$ROOT/src/interpreter/lex/mlstream.cc
W=$(mktemp -d "${TMPDIR:-/tmp}/test-lextrace-hooks.XXXXXX")
trap 'rm -rf "$W"' EXIT INT TERM
fail=0

expect() { # expect 0|1 NAME FILE
  "$G" --file "$3" "$BUILD" > "$W/out" 2>&1; st=$?
  if [ "$1" = 0 ] && [ $st -ne 0 ]; then echo "FAIL $2: guard failed"; sed 's/^/    /' "$W/out"; fail=$((fail + 1))
  elif [ "$1" = 1 ] && [ $st -eq 0 ]; then echo "FAIL $2: guard passed"; sed 's/^/    /' "$W/out"; fail=$((fail + 1))
  else echo "ok $2"; fi
}

expect 0 "real mlstream.cc" "$F"

{ cat "$F"; echo 'static int lextrace_leak;'; } > "$W/decl.cc"
expect 1 "unguarded declaration" "$W/decl.cc"

{ cat "$F"; echo '// a lextrace hook would go here'; } > "$W/comment.cc"
expect 1 "unguarded comment" "$W/comment.cc"

{ cat "$F"; printf '#ifdef ELAN_LEXTRACE\nstatic int a_guarded;\n#else\nstatic int lextrace_else;\n#endif\n'; } > "$W/else.cc"
expect 1 "hook in the #else branch" "$W/else.cc"

awk '/^#ifdef ELAN_LEXTRACE/ { s = 1; next } s && /^#endif/ { s = 0; next } !s' "$F" > "$W/nohook.cc"
expect 1 "no hook at all" "$W/nohook.cc"

if [ $fail -eq 0 ]; then echo "test_check_lextrace_hooks: ok"; exit 0; fi
echo "test_check_lextrace_hooks: $fail failure(s)"; exit 1
