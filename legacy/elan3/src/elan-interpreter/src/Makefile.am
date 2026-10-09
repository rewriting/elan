SUBDIRS = doc

noinst_PROGRAMS = mtokdef mabident

bin_PROGRAMS = elan 

CFLAGSPERSO = -DALPHA -DCOMMAND -DPEM -DBORO -DANYS -DSYMBS -DEARLEY -DTO_BE_DISTRIBUTED -DMETA -DPEVAL -DPREFIX=\"${prefix}\"
CXXFLAGS  = -pipe -O2 $(CFLAGSPERSO)
CFLAGS = -g -Wall -Wstrict-prototypes

MACC     = ../parser/macc
EXEEXT =
MTOKDEF  = ./mtokdef
MABIDENT = ./mabident

COMMONSOURCES = commondefs.cc\
	        ochstream.cc\
		ichstream.cc\
		lstream.cc\
		stringtab.cc\
		mitab.cc\
		mallo.cc\
		misc.cc

#macc_SOURCES =  globdef.c rtglobde.c cmallo.c tiderr.c mlexan.c dump.c \
#                envir.c gen.c rwtab.c ingram.c init.c macc.c trapsa.c mh.c \
#                globdef.h globuse.h rtglobde.h

mtokdef_SOURCES = mtokdefmain.cc specials.cc $(COMMONSOURCES)

mabident_SOURCES = mtokmain.cc specials.cc $(COMMONSOURCES)

ALLINCLUDES = commondefs.h mitab.h compiledefs.h termdefs.h atermparser.h eker_term.h module.h\
	      codes.h ldparser.h mparser.h command.h lstream.h ppexparser.h strategy.h\
              mallo.h stringtab.h meta.h rtdatas.h sym_types.h\
	      acmatchdefs.h reduceparser.h 

# commtokens.h


generated_c = tabofident.cc\
              ppexparser.cc\
              mparser.cc\
              atermparser.cc\
	      reduceparser.cc\
              ldparser.cc\
	      atabofident.cc

generated_h = commtokens.h

elan_SOURCES = ldmain.cc msemact.cc msemact3.cc termcompile.cc matchcompile.cc compilemisc.cc strategy.cc grammar.cc earley.cc esemact.cc term.cc module.cc tmisc.cc rtmisc.cc match.cc trsystem.cc normalise.cc aterm.cc partial.cc stateofexecution.cc meta.cc lbuffer.cc mlstream.cc command.cc $(COMMONSOURCES) $(ALLINCLUDES)

# $(generated_c)

elan_LDADD = $(generated_c:.cc=.o) -lm -L../acmatcher/matcher -lmatch 

###################################################################
EXTRA_DIST  = ppexgram.t modgram.t ldmodgram.t aterm.orig aterm.ref\
	      aterm.reduce include.parser toaterm\
	      banner.cc files.cc process.cc

CLEANFILES   = $(generated_c) $(generated_h) rtabofident.cc f1.tmp f2.tmp

$(generated_c) $(generated_h): mtokdef mabident\
          stringtab.h commondefs.h include.parser\
	  ppexgram.t modgram.t ldmodgram.t aterm.orig aterm.ref aterm.reduce
#
	$(INSTALL) $(srcdir)/ppexgram.t tmpgram.t
	$(MACC) tmpgram.t
	mv sttab.c   ppexparsertab.cc
	mv idtab.tid ppexrw
#
	$(INSTALL) $(srcdir)/modgram.t tmpgram.t
	$(MACC) tmpgram.t
	mv sttab.c   mparsertab.cc
	mv idtab.tid modrw
#
	$(INSTALL) $(srcdir)/ldmodgram.t tmpgram.t
	$(MACC) tmpgram.t
	mv sttab.c   ldparsertab.cc
	mv idtab.tid ldrw
#
	cat $(srcdir)/aterm.orig $(srcdir)/aterm.ref > aterm.t
	$(MACC) aterm.t
	mv sttab.c   atermparsertab.cc
	mv idtab.tid atermrw
	rm -f aterm.t
#
	cat $(srcdir)/aterm.orig $(srcdir)/aterm.reduce > raterm.t
	$(MACC) raterm.t
	mv sttab.c   reduceparsertab.cc
	mv idtab.tid reducerw
	rm -f raterm.t
#
	rm -f commtokens.h tabofident.cc
	cat ldrw modrw ppexrw | $(MTOKDEF)  > commtokens.h
	cat ldrw modrw ppexrw | $(MABIDENT) > tabofident.cc
#
	rm -f acommtokens.h atabofident.cc
	rm -f tmpgram.t
	cat atermrw | $(MTOKDEF)   > tmpgram.t
	sed -f $(srcdir)/toaterm tmpgram.t   > acommtokens.h
	rm -f tmpgram.t
	cat atermrw | $(MABIDENT)  > tmpgram.t
	sed -f $(srcdir)/toaterm tmpgram.t   > atabofident.cc
#
	rm -f rcommtokens.h rtabofident.cc
	rm -f tmpgram.t
	cat reducerw | $(MTOKDEF)  > tmpgram.t
	sed -f $(srcdir)/toaterm tmpgram.t   > rcommtokens.h
	rm -f tmpgram.t
	cat reducerw | $(MABIDENT) > tmpgram.t
	sed -f $(srcdir)/toaterm tmpgram.t   > rtabofident.cc
#
	rm -f ppexparser.cc mparser.cc atermparser.cc reduceparser.cc ldparser.cc
	cat $(srcdir)/ppexparser.h   commtokens.h  ppexparsertab.cc   $(srcdir)/include.parser > ppexparser.cc
	cat $(srcdir)/mparser.h      commtokens.h  mparsertab.cc      $(srcdir)/include.parser > mparser.cc
	cat $(srcdir)/atermparser.h  acommtokens.h atermparsertab.cc  $(srcdir)/include.parser > atermparser.cc
	cat $(srcdir)/reduceparser.h rcommtokens.h reduceparsertab.cc $(srcdir)/include.parser > reduceparser.cc
	cat $(srcdir)/ldparser.h     commtokens.h  ldparsertab.cc     $(srcdir)/include.parser > ldparser.cc
#	touch $(generated_c)
	rm -f ppexparsertab.cc ppexrw mparsertab.cc modrw ldparsertab.cc ldrw\
              aterm.t atermparsertab.cc atermrw raterm.t reduceparsertab.cc reducerw\
              tmpgram.t acommtokens.h rcommtokens.h

###################################################################

