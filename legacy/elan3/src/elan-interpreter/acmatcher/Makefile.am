SUBDIRS       = matcher
EXTRA_DIST    = match.y match.l
bin_PROGRAMS  = match

match_SOURCES = defs.h externs.h main.c
match_LDADD   = lex.yy.o y.tab.o \
                -Lmatcher -lmatch

CFLAGS = -g -Wall -Wstrict-prototypes
CXXFLAGS = -g -Wall -Wstrict-prototypes

YFLAGS        = -d

CLEANFILES   = lex.yy.c y.tab.c y.tab.h idef2tif

y.tab.c y.tab.h: match.y
	$(YACC) $(AM_YFLAGS) $(YFLAGS) $<

lex.yy.c : match.l y.tab.h
	$(LEX) $(AM_LFLAGS) $(LFLAGS) $<

lex.yy.o: lex.yy.c matcher/sym_types.h externs.h matcher/defs.h \
        matcher/term_types.h matcher/functions.h y.tab.h
	$(COMPILE) -DYY_NO_UNPUT -c lex.yy.c

