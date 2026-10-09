# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4-p5 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999, 2001 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.

#
# $Id: Makefile.am,v 1.11 2001/08/14 13:59:20 jong Exp $
#
# vim:ts=8:sw=8


#
# $Id: make_rules,v 1.11 2001/10/04 09:30:40 olivierp Exp $
#
# Common make rules for compiling the ATerm and related static/dynamic
# libraries.
#

# Common defines for all libraries


SHELL = /bin/sh

srcdir = .
top_srcdir = ..
prefix = /usr/local
exec_prefix = ${prefix}

bindir = ${exec_prefix}/bin
sbindir = ${exec_prefix}/sbin
libexecdir = ${exec_prefix}/libexec
datadir = ${prefix}/share
sysconfdir = ${prefix}/etc
sharedstatedir = ${prefix}/com
localstatedir = ${prefix}/var
libdir = ${exec_prefix}/lib
infodir = ${prefix}/info
mandir = ${prefix}/man
includedir = ${prefix}/include
oldincludedir = /usr/include

DESTDIR =

pkgdatadir = $(datadir)/aterm
pkglibdir = $(libdir)/aterm
pkgincludedir = $(includedir)/aterm

top_builddir = ..

ACLOCAL = aclocal
AUTOCONF = autoconf
AUTOMAKE = automake
AUTOHEADER = autoheader

INSTALL = /usr/bin/install -c
INSTALL_PROGRAM = ${INSTALL} $(AM_INSTALL_PROGRAM_FLAGS)
INSTALL_DATA = ${INSTALL} -m 644
INSTALL_SCRIPT = ${INSTALL_PROGRAM}
transform = s,x,x,

NORMAL_INSTALL = :
PRE_INSTALL = :
POST_INSTALL = :
NORMAL_UNINSTALL = :
PRE_UNINSTALL = :
POST_UNINSTALL = :
AR = ar
ATERM_LIB_CC = libATerm-cc.a
ATERM_LIB_DBG = libATerm-dbg.a
ATERM_LIB_GCC = libATerm-gcc.a
ATERM_LIB_NS = 
ATERM_LIB_PROF = libATerm-prof.a
CC = gcc
CC_CC = cc
CC_DBG = gcc
CC_GCC = gcc
CC_NS = gcc
CC_PROF = gcc
CFLAGS_CC = -O
CFLAGS_DBG = -Wall -g
CFLAGS_GCC = -Wall -O4
CFLAGS_NS = -Wall -g -O2
CFLAGS_PROF = -Wall -pg -g
DEFS_CC = $(DEFS) -DNDEBUG
DEFS_DBG = $(DEFS)
DEFS_GCC = $(DEFS) -DNDEBUG
DEFS_NS = $(DEFS) -DNO_SHARING
DEFS_PROF = $(DEFS)
INCL_CC = 
INCL_DBG = 
INCL_GCC = 
INCL_NS = 
INCL_PROF = 
JAVA_API_PS = 
JAVA_SUBDIR = 
LFLAGS_CC = 
LFLAGS_DBG = -g
LFLAGS_GCC = 
LFLAGS_NS = 
LFLAGS_PROF = -pg
MAKEINFO = makeinfo
PACKAGE = aterm
RANLIB = ranlib
TEX_DOCLET = 
VERSION = 1.6.5
VISITOR_PKG = aterm.visitor
XDEFS = 

STDDEFS = $(INCLUDES) $(AM_CPPFLAGS) $(CPPFLAGS)

GCC_COMPILE = $(CC_GCC)    $(CFLAGS_GCC)    $(DEFS_GCC)    $(STDDEFS)
CC_COMPILE = $(CC_CC)     $(CFLAGS_CC)     $(DEFS_CC)     $(STDDEFS)
DBG_COMPILE = $(CC_DBG)    $(CFLAGS_DBG)    $(DEFS_DBG)    $(STDDEFS)
PROF_COMPILE = $(CC_PROF)   $(CFLAGS_PROF)   $(DEFS_PROF)   $(STDDEFS)
CC64_COMPILE = $(CC_CC64)   $(CFLAGS_CC64)   $(DEFS_CC64)   $(STDDEFS)
SHARED_COMPILE = $(CC_SHARED) $(CFLAGS_SHARED) $(DEFS_SHARED) $(STDDEFS)
COMPILE = $(CC)        $(CFLAGS)        $(DEFS)        $(STDDEFS)
NS_COMPILE = $(CC_NS)     $(CFLAGS_NS)     $(DEFS_NS)     $(STDDEFS)

CFLAGS = -Wall -g -O2
EXTRA_DIST = $(bin_SCRIPTS) $(noinst_SCRIPTS)

INCLUDES = -I$(top_srcdir)/aterm

LDFLAGS = -L$(top_builddir)/aterm/

LDLIBS = -lATerm

bin_PROGRAMS = termsize baffle dicttoc atsum atdiff atrmannos

termsize_SOURCES = termsize.c
termsize_LDADD = $(LDLIBS)

baffle_SOURCES = baffle.c
baffle_LDADD = $(LDLIBS)

dicttoc_SOURCES = dicttoc.c
dicttoc_LDADD = $(LDLIBS)

atsum_SOURCES = atsum.c
atsum_LDADD = $(LDLIBS)

atdiff_SOURCES = atdiff.c
atdiff_LDADD = $(LDLIBS)

atrmannos_SOURCES = atrmannos.c
atrmannos_LDADD = $(LDLIBS)

bin_SCRIPTS = baf2trm baf2taf trm2baf trm2taf taf2trm taf2baf 			  trmcat

noinst_SCRIPTS = genpkg.sh
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
PROGRAMS =  $(bin_PROGRAMS)


DEFS =  -DPACKAGE=\"aterm\" -DVERSION=\"1.6.5\" -DHAVE_STRDUP=1 -DHAVE_LRAND48=1 -DHAVE_SRAND48=1  -I. -I$(srcdir) 
CPPFLAGS = 
LIBS = 
termsize_OBJECTS =  termsize.o
termsize_DEPENDENCIES = 
termsize_LDFLAGS = 
baffle_OBJECTS =  baffle.o
baffle_DEPENDENCIES = 
baffle_LDFLAGS = 
dicttoc_OBJECTS =  dicttoc.o
dicttoc_DEPENDENCIES = 
dicttoc_LDFLAGS = 
atsum_OBJECTS =  atsum.o
atsum_DEPENDENCIES = 
atsum_LDFLAGS = 
atdiff_OBJECTS =  atdiff.o
atdiff_DEPENDENCIES = 
atdiff_LDFLAGS = 
atrmannos_OBJECTS =  atrmannos.o
atrmannos_DEPENDENCIES = 
atrmannos_LDFLAGS = 
SCRIPTS =  $(bin_SCRIPTS) $(noinst_SCRIPTS)

CCLD = $(CC)
LINK = $(CCLD) $(AM_CFLAGS) $(CFLAGS) $(LDFLAGS) -o $@
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
DEP_FILES =  .deps/atdiff.P .deps/atrmannos.P .deps/atsum.P \
.deps/baffle.P .deps/dicttoc.P .deps/termsize.P
SOURCES = $(termsize_SOURCES) $(baffle_SOURCES) $(dicttoc_SOURCES) $(atsum_SOURCES) $(atdiff_SOURCES) $(atrmannos_SOURCES)
OBJECTS = $(termsize_OBJECTS) $(baffle_OBJECTS) $(dicttoc_OBJECTS) $(atsum_OBJECTS) $(atdiff_OBJECTS) $(atrmannos_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) $(srcdir)/../make_rules
	cd $(top_srcdir) && $(AUTOMAKE) --gnu utils/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-binPROGRAMS:

clean-binPROGRAMS:
	-test -z "$(bin_PROGRAMS)" || rm -f $(bin_PROGRAMS)

distclean-binPROGRAMS:

maintainer-clean-binPROGRAMS:

install-binPROGRAMS: $(bin_PROGRAMS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_PROGRAMS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo "  $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`"; \
	     $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	  else :; fi; \
	done

uninstall-binPROGRAMS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_PROGRAMS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed 's/$(EXEEXT)$$//'|sed '$(transform)'|sed 's/$$/$(EXEEXT)/'`; \
	done

.s.o:
	$(COMPILE) -c $<

.S.o:
	$(COMPILE) -c $<

mostlyclean-compile:
	-rm -f *.o core *.core

clean-compile:

distclean-compile:
	-rm -f *.tab.c

maintainer-clean-compile:

termsize: $(termsize_OBJECTS) $(termsize_DEPENDENCIES)
	@rm -f termsize
	$(LINK) $(termsize_LDFLAGS) $(termsize_OBJECTS) $(termsize_LDADD) $(LIBS)

baffle: $(baffle_OBJECTS) $(baffle_DEPENDENCIES)
	@rm -f baffle
	$(LINK) $(baffle_LDFLAGS) $(baffle_OBJECTS) $(baffle_LDADD) $(LIBS)

dicttoc: $(dicttoc_OBJECTS) $(dicttoc_DEPENDENCIES)
	@rm -f dicttoc
	$(LINK) $(dicttoc_LDFLAGS) $(dicttoc_OBJECTS) $(dicttoc_LDADD) $(LIBS)

atsum: $(atsum_OBJECTS) $(atsum_DEPENDENCIES)
	@rm -f atsum
	$(LINK) $(atsum_LDFLAGS) $(atsum_OBJECTS) $(atsum_LDADD) $(LIBS)

atdiff: $(atdiff_OBJECTS) $(atdiff_DEPENDENCIES)
	@rm -f atdiff
	$(LINK) $(atdiff_LDFLAGS) $(atdiff_OBJECTS) $(atdiff_LDADD) $(LIBS)

atrmannos: $(atrmannos_OBJECTS) $(atrmannos_DEPENDENCIES)
	@rm -f atrmannos
	$(LINK) $(atrmannos_LDFLAGS) $(atrmannos_OBJECTS) $(atrmannos_LDADD) $(LIBS)

install-binSCRIPTS: $(bin_SCRIPTS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@list='$(bin_SCRIPTS)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_SCRIPT) $$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`"; \
	    $(INSTALL_SCRIPT) $$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`; \
	  else if test -f $(srcdir)/$$p; then \
	    echo " $(INSTALL_SCRIPT) $(srcdir)/$$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`"; \
	    $(INSTALL_SCRIPT) $(srcdir)/$$p $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`; \
	  else :; fi; fi; \
	done

uninstall-binSCRIPTS:
	@$(NORMAL_UNINSTALL)
	list='$(bin_SCRIPTS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(bindir)/`echo $$p|sed '$(transform)'`; \
	done

tags: TAGS

ID: $(HEADERS) $(SOURCES) $(LISP)
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	here=`pwd` && cd $(srcdir) \
	  && mkid -f$$here/ID $$unique $(LISP)

TAGS:  $(HEADERS) $(SOURCES)  $(TAGS_DEPENDENCIES) $(LISP)
	tags=; \
	here=`pwd`; \
	list='$(SOURCES) $(HEADERS)'; \
	unique=`for i in $$list; do echo $$i; done | \
	  awk '    { files[$$0] = 1; } \
	       END { for (i in files) print i; }'`; \
	test -z "$(ETAGS_ARGS)$$unique$(LISP)$$tags" \
	  || (cd $(srcdir) && etags $(ETAGS_ARGS) $$tags  $$unique $(LISP) -o $$here/TAGS)

mostlyclean-tags:

clean-tags:

distclean-tags:
	-rm -f TAGS ID

maintainer-clean-tags:

distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = utils

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu utils/Makefile
	@for file in $(DISTFILES); do \
	  d=$(srcdir); \
	  if test -d $$d/$$file; then \
	    cp -pr $$d/$$file $(distdir)/$$file; \
	  else \
	    test -f $(distdir)/$$file \
	    || ln $$d/$$file $(distdir)/$$file 2> /dev/null \
	    || cp -p $$d/$$file $(distdir)/$$file || :; \
	  fi; \
	done

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)

-include $(DEP_FILES)

mostlyclean-depend:

clean-depend:

distclean-depend:
	-rm -rf .deps

maintainer-clean-depend:

%.o: %.c
	@echo '$(COMPILE) -c $<'; \
	$(COMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-cp .deps/$(*F).pp .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm .deps/$(*F).pp

%.lo: %.c
	@echo '$(LTCOMPILE) -c $<'; \
	$(LTCOMPILE) -Wp,-MD,.deps/$(*F).pp -c $<
	@-sed -e 's/^\([^:]*\)\.o[ 	]*:/\1.lo \1.o :/' \
	  < .deps/$(*F).pp > .deps/$(*F).P; \
	tr ' ' '\012' < .deps/$(*F).pp \
	  | sed -e 's/^\\$$//' -e '/^$$/ d' -e '/:$$/ d' -e 's/$$/ :/' \
	    >> .deps/$(*F).P; \
	rm -f .deps/$(*F).pp
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-binPROGRAMS install-binSCRIPTS
install-exec: install-exec-am

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-binPROGRAMS uninstall-binSCRIPTS
uninstall: uninstall-am
all-am: Makefile $(PROGRAMS) $(SCRIPTS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(bindir) $(DESTDIR)$(bindir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-binPROGRAMS mostlyclean-compile \
		mostlyclean-tags mostlyclean-depend mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-binPROGRAMS clean-compile clean-tags clean-depend \
		clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-binPROGRAMS distclean-compile distclean-tags \
		distclean-depend distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-binPROGRAMS \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-depend maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-binPROGRAMS distclean-binPROGRAMS clean-binPROGRAMS \
maintainer-clean-binPROGRAMS uninstall-binPROGRAMS install-binPROGRAMS \
mostlyclean-compile distclean-compile clean-compile \
maintainer-clean-compile uninstall-binSCRIPTS install-binSCRIPTS tags \
mostlyclean-tags distclean-tags clean-tags maintainer-clean-tags \
distdir mostlyclean-depend distclean-depend clean-depend \
maintainer-clean-depend info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


%-cc.o   : %.c $(ALLINCLUDES)
	CFLAGS= $(CC_COMPILE) -c $< -o $@

%-cc64.o : %.c $(ALLINCLUDES)
	$(CC64_COMPILE) -c $< -o $@

%-gcc.o  : %.c $(ALLINCLUDES)
	$(GCC_COMPILE) -c $< -o $@

%-dbg.o  : %.c $(ALLINCLUDES)
	$(DBG_COMPILE) -c $< -o $@

%-prof.o : %.c $(ALLINCLUDES)
	$(PROF_COMPILE) -c $< -o $@

%-ns.o   : %.c $(ALLINCLUDES)
	$(NS_COMPILE) -c $< -o $@

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
