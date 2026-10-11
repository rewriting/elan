#!/bin/sh
# Compiled programs that read their query from the standard input (S7d
# task 5: the Earley parser of REM's compiled programs ported to Go,
# elanrt/earley), compared query by query with REM and the interpreter.
#
# Programs (each with its queries, one per line, each run separately):
#  - tests/compiler/programs/start_term/*.lgi: queries 5, 7, 2147483647;
#  - tests/compiler/programs/runtime_strategies/*.lgi: 3, 95, 200;
#  - examples/*: the lines of input.inp (the .spc of the example passed);
#  - programs/query/<name>.lgi: the lines of <name>.queries (mixfix
#    operators, priorities and associativity, overloading, injections,
#    strings, numbers, identifiers, AC operators, syntax errors); a line
#    `<EOF>` is an empty standard input (not given to the interpreter,
#    which stops silently: no query).
# For each program and query, `./a.out -quiet < query`:
#  - the compiler ($ELANC, default elanc-go) and REM (`elanc -b -nosplit
#    -quiet`): the whole stdout byte-identical (REM's IDENT(n) printed by
#    name, rem_idents.py; the `rewrite_step` line excluded: the count
#    depends on the compilation scheme), the same stderr (syntax errors,
#    ambiguity warnings; the string lexems of an error message, which REM
#    prints as `(null)`, see remstrings) and the same exit status;
#  - the interpreter (`elan -b LGI [SPC]`): when the compiled program
#    succeeds, its `result =` lines equal to the interpreter's results
#    (` t end` lines; AC terms put in a canonical form, ac_canon.py, for the
#    programs listed in programs/query/canon); when it reports a syntax
#    error (status 1), the interpreter reports an error too.
# Exceptions, each with its reason: programs/query/<name>.no-rem,
# <name>.no-interpreter (then <name>.expected holds the expected stdout,
# rewrite_step excluded, one block per query separated by `==` lines).
#
# Usage: [ELANC=elanc-go] test_query.sh PREFIX [name...]  (an installed
# ELAN with the compiler and elanc; names select programs: start_term/x,
# runtime_strategies/x, examples/x, query/x). The go build cache is
# $ELANC_GO_CACHE, default tests/compiler-rs/work/gocache. The work
# directory tests/compiler-rs/work/query-go.XXXXXX is removed when all pass.
PREFIX=${1:?usage: test_query.sh PREFIX [name...]}
PREFIX=$(cd "$PREFIX" && pwd) || exit 2
shift
HERE=$(cd "$(dirname "$0")" && pwd)
ROOT=$(cd "$HERE/../.." && pwd)
export ELANLIB=$PREFIX PATH=$PREFIX/bin:$PATH LC_ALL=C
ELANC=${ELANC:-elanc-go}
[ "$ELANC" = elanc-go ] || { echo "test_query.sh: only elanc-go reads queries" >&2; exit 2; }
TARGET=${ELANC_GO_CACHE:-$HERE/work/gocache}
QDIR=$HERE/programs/query
mkdir -p "$HERE/work" "$TARGET" || exit 2
RUN=$(mktemp -d "$HERE/work/query-go.XXXXXX") || exit 2
echo "work directory: $RUN ($ELANC)"
SELECTION="$*"
selected() { [ -z "$SELECTION" ] && return 0; for s in $SELECTION; do [ "$s" = "$1" ] && return 0; done; return 1; }

fails=0 n=0 nq=0
ok()   { echo "ok   $1"; }
fail() { echo "FAIL $1"; fails=$((fails+1)); }
oneline() { tr '\n' '|' | sed -e 's/|$//' -e 's/|/ | /g'; }
# canon LABEL: the results in a canonical form modulo AC, for the programs
# listed in programs/query/canon (with the arguments of ac_canon.py)
canon() {
  args=$(awk -F '\t' -v l="$1" '$1 == l { print $2 }' "$QDIR/canon")
  if [ -n "$args" ]; then
    # shellcheck disable=SC2086
    set -f; python3 "$HERE/ac_canon.py" $args; set +f
  else
    cat
  fi
}

# remstrings: an error message of the compiled program as REM prints it:
# REM prints the string lexems (STRING, a string read) as `(null)` (it
# reads them past its table of identifiers: lexem::alfsy), elanc-go as
# STRING and the string quoted (Task 5 ruling)
remstrings() {
  sed -E -e "/^(\[earley\]|	one of|	in context)/s/'STRING'/'(null)'/g" \
         -e '/^(\[earley\]|	one of|	in context)/s/"([^"\\]|\\.)*"/(null)/g'
}

# program LABEL SRCDIR LGI SPC QUERIES: compile LGI (files of SRCDIR) with
# the compiler and REM, then run each line of the file QUERIES
program() {
  label=$1 src=$2 lgi=$3 spc=$4 queries=$5
  selected "$label" || return 0
  n=$((n+1))
  d=$RUN/$(echo "$label" | tr / _)
  mkdir -p "$d/go" "$d/rem" "$d/int" || exit 2
  for w in go rem int; do cp -R "$src"/. "$d/$w/" || exit 2; done
  spcfile=; [ -n "$spc" ] && spcfile=$spc.spc
  noint=; norem=; expected=
  base=$(basename "$label")
  if [ "${label%%/*}" = query ]; then
    [ -f "$QDIR/$base.no-rem" ] && norem=$QDIR/$base.no-rem
    [ -f "$QDIR/$base.no-interpreter" ] && noint=$QDIR/$base.no-interpreter
    [ -f "$QDIR/$base.expected" ] && expected=$QDIR/$base.expected
  fi
  if ! ( cd "$d/go" && $ELANC -v -b -nosplit -quiet --gocache "$TARGET" "$lgi" $spc >elanc-go.log 2>&1 ); then
    fail "$label: $ELANC failed (see $d/go/elanc-go.log)"; return
  fi
  build=$(sed -n 's/^elanc-go: build time //p' "$d/go/elanc-go.log")
  if [ -z "$norem" ] && ! ( cd "$d/rem" && elanc -b -nosplit -quiet "$lgi" $spc >elanc.log 2>&1 \
       && make -f "$lgi.make" >make.log 2>&1 ); then
    fail "$label: elanc or make failed (see $d/rem/*.log)"; return
  fi
  i=0
  while IFS= read -r q || [ -n "$q" ]; do
    i=$((i+1)) nq=$((nq+1))
    if [ "$q" = "<EOF>" ]; then printf '' > "$d/q$i"; else printf '%s\n' "$q" > "$d/q$i"; fi
    what="$label [$q]"
    ( cd "$d/go" && ./a.out -quiet < "$d/q$i" > "$d/go/out$i" 2> "$d/go/err$i" ); gs=$?
    grep -v '^rewrite_step = ' "$d/go/out$i" > "$d/go/cmp$i"
    good=1
    case $gs in 0|1) ;; *) fail "$what: status $gs ($(head -c 300 "$d/go/err$i" | tr '\n' ' '))"; continue ;; esac
    # REM
    if [ -n "$norem" ]; then
      if [ -n "$expected" ]; then
        awk -v k="$i" 'BEGIN{b=1} /^==$/{b++; next} b==k' "$expected" > "$d/exp$i"
        if ! cmp -s "$d/exp$i" "$d/go/cmp$i"; then
          fail "$what: stdout differs from $base.expected: $(diff "$d/exp$i" "$d/go/cmp$i" | head -4 | tr '\n' ' ')"; good=
        fi
      fi
    else
      ( cd "$d/rem" && ./a.out -quiet < "$d/q$i" > "$d/rem/raw$i" 2> "$d/rem/err$i" ); rs=$?
      # REM's identifiers by name: the names of the .ref, then those of the query
      python3 "$HERE/rem_idents.py" "$d/rem/$lgi.ref" --query "$d/q$i" < "$d/rem/raw$i" > "$d/rem/out$i"
      grep -v '^rewrite_step = ' "$d/rem/out$i" > "$d/rem/cmp$i"
      if [ "$rs" != "$gs" ]; then
        fail "$what vs REM: status $gs, REM $rs"; good=
      elif ! cmp -s "$d/go/cmp$i" "$d/rem/cmp$i"; then
        fail "$what vs REM: stdout $(diff "$d/rem/cmp$i" "$d/go/cmp$i" | head -4 | tr '\n' ' ')"; good=
      elif ! remstrings < "$d/go/err$i" | cmp -s - "$d/rem/err$i"; then
        fail "$what vs REM: stderr $(diff "$d/rem/err$i" "$d/go/err$i" | head -6 | tr '\n' ' ')"; good=
      fi
    fi
    # the interpreter
    # (an empty input is no query for the interpreter: it stops silently)
    if [ -z "$noint" ] && [ "$q" != "<EOF>" ]; then
      iout=$( cd "$d/int" && elan -b "$lgi.lgi" $spcfile < "$d/q$i" 2>&1 )
      if [ "$gs" = 1 ]; then
        if ! printf '%s\n' "$iout" | grep -q '^\[error\]'; then
          fail "$what: a syntax error for $ELANC, not for the interpreter"; good=
        fi
      else
        a=$(grep '^result = ' "$d/go/out$i" | sed 's/^result = *//' | tr -d '"' | canon "$label" | oneline)
        b=$(printf '%s\n' "$iout" | sed -n 's/^ *\(.*\) end$/\1/p' | grep -v '^end$' | tr -d '"' | canon "$label" | oneline)
        if [ "$a" != "$b" ]; then
          fail "$what vs interpreter: $ELANC [$a], interpreter [$b]"; good=
        fi
      fi
    fi
    if [ -n "$good" ]; then
      r=$(grep '^result = ' "$d/go/out$i" | sed 's/^result = *//' | oneline)
      [ "$gs" = 1 ] && r="syntax error: $(sed -n 's/^\[earley\] //p' "$d/go/err$i" | head -1)"
      ok "$what: [$r]${norem:+ (no REM: $(basename "$norem"))}${noint:+ (no interpreter: $(basename "$noint"))}"
    fi
  done < "$queries"
  echo "     $label: build $build"
}

numbers() { printf '%s end\n' "$@" > "$RUN/numbers.$1"; echo "$RUN/numbers.$1"; }
stdir=$ROOT/tests/compiler/programs/start_term
for lgi in "$stdir"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  program "start_term/$name" "$stdir" "$name" "" "$(numbers 5 7 2147483647)"
done
rsdir=$ROOT/tests/compiler/programs/runtime_strategies
for lgi in "$rsdir"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  program "runtime_strategies/$name" "$rsdir" "$name" "" "$(numbers 3 95 200)"
done
for d in "$ROOT"/examples/*/; do
  name=$(basename "$d")
  spc=$(cd "$d" && ls ./*.spc 2>/dev/null | head -1)
  spc=${spc#./}
  program "examples/$name" "$d" "$name" "${spc%.spc}" "$d/input.inp"
done
for lgi in "$QDIR"/*.lgi; do
  name=$(basename "$lgi" .lgi)
  program "query/$name" "$QDIR" "$name" "" "$QDIR/$name.queries"
done

echo "$n programs, $nq queries, $fails failures"
if [ "$fails" -eq 0 ]; then rm -rf "$RUN"; else echo "kept for inspection: $RUN"; fi
[ "$fails" -eq 0 ]
