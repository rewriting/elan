SUBDIRS = SmilesGasEl

# Common defines for all libraries
STDDEFS = $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)

## Library specific compiler flags
GCC_COMPILE    = $(CC_GCC)    $(CFLAGS_GCC)    $(DEFS_GCC)    $(STDDEFS) $(CC_OPT) 
DBG_COMPILE    = $(CC_DBG)    $(CFLAGS_DBG)    $(DEFS_DBG)    $(STDDEFS) $(CC_OPT) 
PROF_COMPILE   = $(CC_PROF)   $(CFLAGS_PROF)   $(DEFS_PROF)   $(STDDEFS) $(CC_OPT) 
NS_COMPILE     = $(CC_NS)     $(CFLAGS_NS)     $(DEFS_NS)     $(STDDEFS) $(CC_OPT) 
COMPILE        = $(CC)        $(CFLAGS)        $(DEFS)        $(STDDEFS) $(CC_OPT) 

## Rules for compiling C files to include in static libraries

%-base.o  : %.c $(ALLINCLUDES)
	$(COMPILE) -DBASETERM -c $< -o $@

%-base-gcc.o  : %.c $(ALLINCLUDES)
	$(GCC_COMPILE) -DBASETERM -c $< -o $@

%-base-dbg.o  : %.c $(ALLINCLUDES)
	$(DBG_COMPILE) -DBASETERM -c $< -o $@

%-base-prof.o : %.c $(ALLINCLUDES)
	$(PROF_COMPILE) -DBASETERM -c $< -o $@

%-aterm.o  : %.c $(ALLATERMINCLUDES)
	$(COMPILE) -DATERM -DNO_SHARING -c $< -o $@

%-aterm-gcc.o  : %.c $(ALLATERMINCLUDES)
	$(GCC_COMPILE) -DATERM -DNO_SHARING -c $< -o $@

%-aterm-dbg.o  : %.c $(ALLATERMINCLUDES)
	$(DBG_COMPILE) -DATERM -c $< -o $@

%-aterm-prof.o : %.c $(ALLATERMINCLUDES)
	$(PROF_COMPILE) -DATERM -c $< -o $@

%-aterm-ns.o : %.c $(ALLATERMINCLUDES)
	$(NS_COMPILE) -DATERM -DNO_SHARING -c $< -o $@

BASIC_OPT = -DHASHCODE -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR 
CC_OPT    = -DCOMPUTE_HCODE -DCSETCHP -DBITSET32 $(BASIC_OPT) 

INCLUDES  =  -I$(top_builddir)/runtime -I$(CPL)/include/elan-compiler -I$(GC-PATCHED)/include -I$(CPL)/include -I$(ATERM)/include

lib_LIBRARIES		= libruntime-base.a libruntime-aterm.a\
                          @RUNTIME_LIB_BASE_GCC@ @RUNTIME_LIB_BASE_DBG@ @RUNTIME_LIB_BASE_PROF@\
                          @RUNTIME_LIB_ATERM_GCC@ @RUNTIME_LIB_ATERM_DBG@\
                          @RUNTIME_LIB_ATERM_PROF@ @RUNTIME_LIB_ATERM_NS@

EXTRA_LIBRARIES		= libruntime-base-gcc.a libruntime-base-dbg.a libruntime-base-prof.a\
                          libruntime-aterm-gcc.a libruntime-aterm-dbg.a \
                          libruntime-aterm-prof.a libruntime-aterm-ns.a

#ACMDIR                  = acmatcher
ACMDIR                  = .

ACMATCHERSRC            = $(ACMDIR)/build_free.c $(ACMDIR)/build_pure.c $(ACMDIR)/flatten.c\
                          $(ACMDIR)/solve_free.c $(ACMDIR)/build_match.c $(ACMDIR)/eker_tools.c\
                          $(ACMDIR)/gccmatch.c $(ACMDIR)/solve_pure.c

ACMATCHERINCLUDES       = $(ACMDIR)/acmatchdefs.h $(ACMDIR)/eker_tools.h $(ACMDIR)/functions.h\
                          $(ACMDIR)/pure_types.h $(ACMDIR)/sym_types.h $(ACMDIR)/defs.h\
                          $(ACMDIR)/free_types.h $(ACMDIR)/minitools.h $(ACMDIR)/state_types.h\
                          $(ACMDIR)/term_types.h 

COMMONSRCS		= bgraph.c bitset.c bitset_32.c link.c match_state.c builtinMatching.c\
                          tools.c streval.c ac_tools.c thunklib.c builtin.c y.tab.c\
                          termCommon.c trace.c termIn.c termOut.c\
                          bitset_tab_mask.c bitset_tab_int.c y.tab.c

COMMONINCLUDES		= bgraph.h bitset.h bitset_32.h link.h match_state.h builtinMatching.h\
                          tools.h streval.h ac_tools.h builtin.h y.tab.h\
                          termCommon.h trace.h termIn.h termOut.h\
                          bitset_tab_mask.h bitset_tab_int.h codes.h\
			  main_skeleton.h main_skeleton.c\
			  main_coq_skeleton.h main_coq_skeleton.c\
			  main_skeleton_lib.h main_skeleton_lib.c

YFLAGS                  = -d

ALLSRCS                 = $(COMMONSRCS) $(ACMATCHERSRC) termBase.c 
ALLATERMSRCS            = $(COMMONSRCS) termATerm.c

ALLINCLUDES             = $(COMMONINCLUDES) $(ACMATCHERINCLUDES) termBase.h
ALLATERMINCLUDES        = $(COMMONINCLUDES) termATerm.h

EXTRA_DIST		= $(ALLSRCS) termATerm.c ref.yacc ref.lex

pkginclude_HEADERS         = $(ALLINCLUDES) termATerm.h 

libruntime_base_a_SOURCES       =
libruntime_base_gcc_a_SOURCES	=
libruntime_base_dbg_a_SOURCES	=
libruntime_base_prof_a_SOURCES  =

libruntime_base_a_LIBADD	= $(ALLSRCS:.c=-base.o)
libruntime_base_gcc_a_LIBADD	= $(ALLSRCS:.c=-base-gcc.o)
libruntime_base_dbg_a_LIBADD	= $(ALLSRCS:.c=-base-dbg.o)
libruntime_base_prof_a_LIBADD   = $(ALLSRCS:.c=-base-prof.o)

libruntime_aterm_a_SOURCES	=
libruntime_aterm_gcc_a_SOURCES	=
libruntime_aterm_dbg_a_SOURCES	=
libruntime_aterm_prof_a_SOURCES =
libruntime_aterm_ns_a_SOURCES   =

libruntime_aterm_a_LIBADD       = $(ALLATERMSRCS:.c=-aterm.o)
libruntime_aterm_gcc_a_LIBADD	= $(ALLATERMSRCS:.c=-aterm-gcc.o)
libruntime_aterm_dbg_a_LIBADD	= $(ALLATERMSRCS:.c=-aterm-dbg.o)
libruntime_aterm_prof_a_LIBADD  = $(ALLATERMSRCS:.c=-aterm-prof.o)
libruntime_aterm_ns_a_LIBADD    = $(ALLATERMSRCS:.c=-aterm-ns.o)

y.tab.c y.tab.h: ref.yacc lex.yy.c
	$(YACC) $(AM_YFLAGS) $(YFLAGS) $<

lex.yy.c : ref.lex y.tab.h
	$(LEX) $(AM_LFLAGS) $(LFLAGS) $<

CLEANFILES          = lex.yy.c y.tab.c y.tab.h $(ACMDIR)/*.o
