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

# $Id: Makefile.am,v 1.16 2001/10/04 09:30:53 olivierp Exp $


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

# The following libraries are compiled to static libraries.
#
lib_LIBRARIES = libATerm.a libATerm-gcc.a libATerm-cc.a 			libATerm-dbg.a libATerm-prof.a 


EXTRA_LIBRARIES = libATerm-gcc.a libATerm-cc.a libATerm-dbg.a 			libATerm-prof.a libATerm-ns.a


ALLSRCS = aterm.c list.c make.c memory.c afun.c gc.c 			hash.c bafio.c byteio.c tafio.c version.c md5c.c


PUBLIC_INCL = abool.h atypes.h aterm1.h aterm2.h deprecated.h 			encoding.h afun.h


PRIVATE_INCL = _aterm.h _afun.h debug.h gc.h list.h make.h 			memory.h util.h bafio.h version.h tafio.h byteio.h 			md5.h


#CFLAGS			= -Wall -g -O2 -DDMALLOC -I/ufs/olivierp/include
CFLAGS = -Wall -g -O2

ALLINCLUDES = ${PUBLIC_INCL} ${PRIVATE_INCL}

include_HEADERS = $(PUBLIC_INCL)

EXTRA_DIST = $(ALLSRCS) $(PRIVATE_INCL)

# static only libraries
libATerm_a_SOURCES = 
libATerm_cc_a_SOURCES = 
libATerm_gcc_a_SOURCES = 
libATerm_dbg_a_SOURCES = 
libATerm_prof_a_SOURCES = 
libATerm_ns_a_SOURCES = 

libATerm_ns_a_CFLAGS = -DNO_SHARING

libATerm_a_LIBADD = $(ALLSRCS:.c=.o)
libATerm_cc_a_LIBADD = $(ALLSRCS:.c=-cc.o)
libATerm_gcc_a_LIBADD = $(ALLSRCS:.c=-gcc.o)
libATerm_dbg_a_LIBADD = $(ALLSRCS:.c=-dbg.o)
libATerm_prof_a_LIBADD = $(ALLSRCS:.c=-prof.o)
libATerm_ns_a_LIBADD = $(ALLSRCS:.c=-ns.o)
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
LIBRARIES =  $(lib_LIBRARIES)


DEFS =  -DPACKAGE=\"aterm\" -DVERSION=\"1.6.5\" -DHAVE_STRDUP=1 -DHAVE_LRAND48=1 -DHAVE_SRAND48=1  -I. -I$(srcdir) 
CPPFLAGS = 
LDFLAGS = 
LIBS = 
libATerm_gcc_a_DEPENDENCIES =  aterm-gcc.o list-gcc.o make-gcc.o \
memory-gcc.o afun-gcc.o gc-gcc.o hash-gcc.o bafio-gcc.o byteio-gcc.o \
tafio-gcc.o version-gcc.o md5c-gcc.o
libATerm_gcc_a_OBJECTS = 
libATerm_cc_a_DEPENDENCIES =  aterm-cc.o list-cc.o make-cc.o memory-cc.o \
afun-cc.o gc-cc.o hash-cc.o bafio-cc.o byteio-cc.o tafio-cc.o \
version-cc.o md5c-cc.o
libATerm_cc_a_OBJECTS = 
libATerm_dbg_a_DEPENDENCIES =  aterm-dbg.o list-dbg.o make-dbg.o \
memory-dbg.o afun-dbg.o gc-dbg.o hash-dbg.o bafio-dbg.o byteio-dbg.o \
tafio-dbg.o version-dbg.o md5c-dbg.o
libATerm_dbg_a_OBJECTS = 
libATerm_prof_a_DEPENDENCIES =  aterm-prof.o list-prof.o make-prof.o \
memory-prof.o afun-prof.o gc-prof.o hash-prof.o bafio-prof.o \
byteio-prof.o tafio-prof.o version-prof.o md5c-prof.o
libATerm_prof_a_OBJECTS = 
libATerm_ns_a_DEPENDENCIES =  aterm-ns.o list-ns.o make-ns.o memory-ns.o \
afun-ns.o gc-ns.o hash-ns.o bafio-ns.o byteio-ns.o tafio-ns.o \
version-ns.o md5c-ns.o
libATerm_ns_a_OBJECTS = 
libATerm_a_DEPENDENCIES =  aterm.o list.o make.o memory.o afun.o gc.o \
hash.o bafio.o byteio.o tafio.o version.o md5c.o
libATerm_a_OBJECTS = 
HEADERS =  $(include_HEADERS)

DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
SOURCES = $(libATerm_gcc_a_SOURCES) $(libATerm_cc_a_SOURCES) $(libATerm_dbg_a_SOURCES) $(libATerm_prof_a_SOURCES) $(libATerm_ns_a_SOURCES) $(libATerm_a_SOURCES)
OBJECTS = $(libATerm_gcc_a_OBJECTS) $(libATerm_cc_a_OBJECTS) $(libATerm_dbg_a_OBJECTS) $(libATerm_prof_a_OBJECTS) $(libATerm_ns_a_OBJECTS) $(libATerm_a_OBJECTS)

all: all-redirect
.SUFFIXES:
.SUFFIXES: .S .c .o .s
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) $(srcdir)/../make_rules
	cd $(top_srcdir) && $(AUTOMAKE) --gnu aterm/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-libLIBRARIES:

clean-libLIBRARIES:
	-test -z "$(lib_LIBRARIES)" || rm -f $(lib_LIBRARIES)

distclean-libLIBRARIES:

maintainer-clean-libLIBRARIES:

install-libLIBRARIES: $(lib_LIBRARIES)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(libdir)
	@list='$(lib_LIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(INSTALL_DATA) $$p $(DESTDIR)$(libdir)/$$p"; \
	    $(INSTALL_DATA) $$p $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done
	@$(POST_INSTALL)
	@list='$(lib_LIBRARIES)'; for p in $$list; do \
	  if test -f $$p; then \
	    echo " $(RANLIB) $(DESTDIR)$(libdir)/$$p"; \
	    $(RANLIB) $(DESTDIR)$(libdir)/$$p; \
	  else :; fi; \
	done

uninstall-libLIBRARIES:
	@$(NORMAL_UNINSTALL)
	list='$(lib_LIBRARIES)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(libdir)/$$p; \
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

libATerm-gcc.a: $(libATerm_gcc_a_OBJECTS) $(libATerm_gcc_a_DEPENDENCIES)
	-rm -f libATerm-gcc.a
	$(AR) cru libATerm-gcc.a $(libATerm_gcc_a_OBJECTS) $(libATerm_gcc_a_LIBADD)
	$(RANLIB) libATerm-gcc.a

libATerm-cc.a: $(libATerm_cc_a_OBJECTS) $(libATerm_cc_a_DEPENDENCIES)
	-rm -f libATerm-cc.a
	$(AR) cru libATerm-cc.a $(libATerm_cc_a_OBJECTS) $(libATerm_cc_a_LIBADD)
	$(RANLIB) libATerm-cc.a

libATerm-dbg.a: $(libATerm_dbg_a_OBJECTS) $(libATerm_dbg_a_DEPENDENCIES)
	-rm -f libATerm-dbg.a
	$(AR) cru libATerm-dbg.a $(libATerm_dbg_a_OBJECTS) $(libATerm_dbg_a_LIBADD)
	$(RANLIB) libATerm-dbg.a

libATerm-prof.a: $(libATerm_prof_a_OBJECTS) $(libATerm_prof_a_DEPENDENCIES)
	-rm -f libATerm-prof.a
	$(AR) cru libATerm-prof.a $(libATerm_prof_a_OBJECTS) $(libATerm_prof_a_LIBADD)
	$(RANLIB) libATerm-prof.a

libATerm-ns.a: $(libATerm_ns_a_OBJECTS) $(libATerm_ns_a_DEPENDENCIES)
	-rm -f libATerm-ns.a
	$(AR) cru libATerm-ns.a $(libATerm_ns_a_OBJECTS) $(libATerm_ns_a_LIBADD)
	$(RANLIB) libATerm-ns.a

libATerm.a: $(libATerm_a_OBJECTS) $(libATerm_a_DEPENDENCIES)
	-rm -f libATerm.a
	$(AR) cru libATerm.a $(libATerm_a_OBJECTS) $(libATerm_a_LIBADD)
	$(RANLIB) libATerm.a

install-includeHEADERS: $(include_HEADERS)
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(includedir)
	@list='$(include_HEADERS)'; for p in $$list; do \
	  if test -f "$$p"; then d= ; else d="$(srcdir)/"; fi; \
	  echo " $(INSTALL_DATA) $$d$$p $(DESTDIR)$(includedir)/$$p"; \
	  $(INSTALL_DATA) $$d$$p $(DESTDIR)$(includedir)/$$p; \
	done

uninstall-includeHEADERS:
	@$(NORMAL_UNINSTALL)
	list='$(include_HEADERS)'; for p in $$list; do \
	  rm -f $(DESTDIR)$(includedir)/$$p; \
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

subdir = aterm

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu aterm/Makefile
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
info-am:
info: info-am
dvi-am:
dvi: dvi-am
check-am: all-am
check: check-am
installcheck-am:
installcheck: installcheck-am
install-exec-am: install-libLIBRARIES
install-exec: install-exec-am

install-data-am: install-includeHEADERS
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-libLIBRARIES uninstall-includeHEADERS
uninstall: uninstall-am
all-am: Makefile $(LIBRARIES) $(HEADERS)
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(libdir) $(DESTDIR)$(includedir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-libLIBRARIES mostlyclean-compile \
		mostlyclean-tags mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-libLIBRARIES clean-compile clean-tags clean-generic \
		mostlyclean-am

clean: clean-am

distclean-am:  distclean-libLIBRARIES distclean-compile distclean-tags \
		distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-libLIBRARIES \
		maintainer-clean-compile maintainer-clean-tags \
		maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-libLIBRARIES distclean-libLIBRARIES \
clean-libLIBRARIES maintainer-clean-libLIBRARIES uninstall-libLIBRARIES \
install-libLIBRARIES mostlyclean-compile distclean-compile \
clean-compile maintainer-clean-compile uninstall-includeHEADERS \
install-includeHEADERS tags mostlyclean-tags distclean-tags clean-tags \
maintainer-clean-tags distdir info-am info dvi-am dvi check check-am \
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

version.o:      version.c $(ALLINCLUDES) $(ALLSRCS)
version-cc.o:   version.c $(ALLINCLUDES) $(ALLSRCS)
version-gcc.o:  version.c $(ALLINCLUDES) $(ALLSRCS)
version-dbg.o:  version.c $(ALLINCLUDES) $(ALLSRCS)
version-prof.o: version.c $(ALLINCLUDES) $(ALLSRCS)
version-ns.o:   version.c $(ALLINCLUDES) $(ALLSRCS)

# vim:ts=8:sw=8:sts=0

# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
