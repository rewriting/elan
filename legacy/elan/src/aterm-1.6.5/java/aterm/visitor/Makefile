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


SHELL = /bin/sh

srcdir = .
top_srcdir = ../../..
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

top_builddir = ../../..

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

javadir = ${prefix}/classes/aterm/visitor

java_JAVA = Visitable.java Visitor.java VisitFailure.java TopDown.java

JAVAROOT = ${top_builddir}/java/
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
JAVAC = javac
JAVACFLAGS = 
CLASSPATH_ENV = CLASSPATH=$(JAVAROOT):$(srcdir)/$(JAVAROOT):$$CLASSPATH
DIST_COMMON =  $(java_JAVA) Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu java/aterm/visitor/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status


mostlyclean-javaJAVA:

clean-javaJAVA:
	-rm *.class classjava.stamp

distclean-javaJAVA:

maintainer-clean-javaJAVA:

install-javaJAVA: classjava.stamp
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)$(javadir)
	for p in *.class; do \
	  echo " $(INSTALL_DATA) $$p $(DESTDIR)$(javadir)/$$p"; \
	  $(INSTALL_DATA) $$p $(DESTDIR)$(javadir)/$$p; \
	done

uninstall-javaJAVA:
	@$(NORMAL_UNINSTALL)
	for p in *.class; do \
	  rm -f $(DESTDIR)$(javadir)/$$p; \
	done
classjava.stamp: $(java_JAVA)
	$(CLASSPATH_ENV) $(JAVAC) -d $(JAVAROOT) $(JAVACFLAGS) $?
	echo timestamp > classjava.stamp
tags: TAGS
TAGS:


distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = java/aterm/visitor

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu java/aterm/visitor/Makefile
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
install-exec-am:
install-exec: install-exec-am

install-data-am: install-javaJAVA
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am: uninstall-javaJAVA
uninstall: uninstall-am
all-am: Makefile classjava.stamp
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:
	$(mkinstalldirs)  $(DESTDIR)$(javadir)


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-javaJAVA mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-javaJAVA clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-javaJAVA distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-javaJAVA maintainer-clean-generic \
		distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: mostlyclean-javaJAVA distclean-javaJAVA clean-javaJAVA \
maintainer-clean-javaJAVA uninstall-javaJAVA install-javaJAVA tags \
distdir info-am info dvi-am dvi check check-am installcheck-am \
installcheck install-exec-am install-exec install-data-am install-data \
install-am install uninstall-am uninstall all-redirect all-am all \
installdirs mostlyclean-generic distclean-generic clean-generic \
maintainer-clean-generic clean mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
