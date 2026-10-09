#
#    CPL - The Nancy ChoicePoint Library
#

include ../make_rules

lib_LIBRARIES		= libchoice.a @CHOICE_LIB_GCC@ @CHOICE_LIB_DBG@ @CHOICE_LIB_PROF@
EXTRA_LIBRARIES		= libchoice-gcc.a libchoice-dbg.a libchoice-prof.a

ALLSRCS			= choice.c
ALLINCLUDES	= choice.h

CFLAGS			= -Wall -O2

include_HEADERS		= $(ALLINCLUDES)

EXTRA_DIST		= $(ALLSRCS) $(ALLINCLUDES)

libchoice_a_SOURCES	= $(ALLSRCS)
libchoice_gcc_a_SOURCES	=
libchoice_dbg_a_SOURCES	=
libchoice_prof_a_SOURCES=

libchoice_gcc_a_LIBADD	= $(ALLSRCS:.c=-gcc.o)
libchoice_dbg_a_LIBADD	= $(ALLSRCS:.c=-dbg.o)
libchoice_prof_a_LIBADD	= $(ALLSRCS:.c=-prof.o)

