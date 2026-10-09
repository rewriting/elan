#!/bin/sh
# Generate the parser sources of the ELAN interpreter, exactly as the
# $(generated_c) rule of elan-interpreter/src/Makefile.am (2003) did.
# Usage: gen-parsers.sh MACC MTOKDEF MABIDENT GRAMMARDIR OUTDIR
# (GRAMMARDIR = src/interpreter/parse/grammars)
set -e
MACC=$1; MTOKDEF=$2; MABIDENT=$3; S=$4; O=$5
mkdir -p "$O"; cd "$O"

gen() { # gen <grammar file> <table name> <keywords name>
  cp "$1" tmpgram.t; "$MACC" tmpgram.t; mv sttab.c "$2"; mv idtab.tid "$3"
}
gen "$S/ppexgram.t"  ppexparsertab.cc ppexrw
gen "$S/modgram.t"   mparsertab.cc    modrw
gen "$S/ldmodgram.t" ldparsertab.cc   ldrw
cat "$S/aterm.orig" "$S/aterm.ref"    > aterm.t;  gen aterm.t  atermparsertab.cc  atermrw
cat "$S/aterm.orig" "$S/aterm.reduce" > raterm.t; gen raterm.t reduceparsertab.cc reducerw

cat ldrw modrw ppexrw | "$MTOKDEF"  > commtokens.h
cat ldrw modrw ppexrw | "$MABIDENT" > tabofident.cc
"$MTOKDEF"  < atermrw  | sed -f "$S/toaterm" > acommtokens.h
"$MABIDENT" < atermrw  | sed -f "$S/toaterm" > atabofident.cc
"$MTOKDEF"  < reducerw | sed -f "$S/toaterm" > rcommtokens.h

cat "$S/ppexparser.h"   commtokens.h  ppexparsertab.cc   "$S/include.parser" > ppexparser.cc
cat "$S/mparser.h"      commtokens.h  mparsertab.cc      "$S/include.parser" > mparser.cc
cat "$S/atermparser.h"  acommtokens.h atermparsertab.cc  "$S/include.parser" > atermparser.cc
cat "$S/reduceparser.h" rcommtokens.h reduceparsertab.cc "$S/include.parser" > reduceparser.cc
cat "$S/ldparser.h"     commtokens.h  ldparsertab.cc     "$S/include.parser" > ldparser.cc

rm -f ppexparsertab.cc ppexrw mparsertab.cc modrw ldparsertab.cc ldrw \
      aterm.t atermparsertab.cc atermrw raterm.t reduceparsertab.cc reducerw \
      tmpgram.t acommtokens.h rcommtokens.h
