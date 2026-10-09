#
#    CPL - The Nancy ChoicePoint Library
#

ATERM = @ATERM@

TESTS = queens 

INCLUDES = -I$(top_srcdir)/src 
#           -I$(ATERM)/include 

LDFLAGS = -L$(top_builddir)/src
# -L$(ATERM)/lib 
LDLIBS  = -lchoice -lm
#-lATerm

check_PROGRAMS = queens 
#match-choice

queens_SOURCES = queens.c
queens_LDADD   = $(LDLIBS)


#match-choice

#EXTRA_DIST 		= 

