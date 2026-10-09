# Generated automatically from Makefile.in by configure.
# Makefile.in generated automatically by automake 1.4 from Makefile.am

# Copyright (C) 1994, 1995-8, 1999 Free Software Foundation, Inc.
# This Makefile.in is free software; the Free Software Foundation
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY, to the extent permitted by law; without
# even the implied warranty of MERCHANTABILITY or FITNESS FOR A
# PARTICULAR PURPOSE.


SHELL = /bin/sh

srcdir = .
top_srcdir = ../../../..
prefix = /local/protheo/pot/ELAN/elan3/mandrake-2.2.18
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

pkgdatadir = $(datadir)/elan-compiler
pkglibdir = $(libdir)/elan-compiler
pkgincludedir = $(includedir)/elan-compiler

top_builddir = ../../../..

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
build_alias = @build_alias@
build_triplet = @build@
host_alias = @host_alias@
host_triplet = @host@
target_alias = @target_alias@
target_triplet = @target@
ATERM = /local/protheo/pot/ELAN/elan3/mandrake-2.2.18
CC = gcc
CC_DBG = gcc
CC_GCC = gcc
CC_NS = gcc
CC_PROF = gcc
CFLAGS_DBG = -g
CFLAGS_GCC = -O2
CFLAGS_NS = -O2
CFLAGS_PROF = -pg
CLASSPATH = .:/users/protheo/moreau/Java/classes:/users/protheo/moreau/ELAN/classes:/users/protheo/pot/Tools/XML/xerces-1_2_1/xerces.jar:/users/protheo/pot/Tools/XML/swing-1.1.1fcs/swing.jar:/users/protheo/pot/Tools/XML/xerces-1_2_1/xercesSamples.jar:/users/protheo/moreau/Java/soot-1.2.0/soot/classes:/users/protheo/moreau/Java/soot-1.2.0/jasmin/classes:/users/protheo/moreau/ELAN/classes
CPL = /local/protheo/pot/ELAN/elan3/mandrake-2.2.18
CPP = gcc -E
CXX = c++
DEFS_DBG = $(DEFS) -DDEBUG
DEFS_GCC = $(DEFS)
DEFS_NS = $(DEFS) -DNO_SHARING
DEFS_PROF = $(DEFS)
EXEEXT = @EXEEXT@
GC = /local/protheo/pot/ELAN/elan3/mandrake-2.2.18
INCL_DBG = 
INCL_GCC = 
INCL_NS = 
INCL_PROF = 
JAR = @JAR@
JAVA = java
JAVAC = javac
JAVACC = javacc
JAVACFLAGS = 
JAVADOC = @JAVADOC@
JAVAFLAGS = 
JAVAH = @JAVAH@
JAVAPREFIX = 
JAVA_JUNIT = 
JUNIT = 
LEX = flex
LFLAGS_DBG = -g
LFLAGS_GCC = 
LFLAGS_NS = 
LFLAGS_PROF = -pg
MAKEINFO = makeinfo
PACKAGE = elan-compiler
RANLIB = ranlib
RUNTIME_LIB_ATERM_DBG = libruntime-aterm-dbg.a
RUNTIME_LIB_ATERM_GCC = libruntime-aterm-gcc.a
RUNTIME_LIB_ATERM_NS = libruntime-aterm-ns.a
RUNTIME_LIB_ATERM_PROF = libruntime-aterm-prof.a
RUNTIME_LIB_BASE_DBG = libruntime-base-dbg.a
RUNTIME_LIB_BASE_GCC = libruntime-base-gcc.a
RUNTIME_LIB_BASE_PROF = libruntime-base-prof.a
TESTS_JUNIT = 
VERSION = 4.3f
YACC = bison -y
YACCLIB = 
translit = @translit@
uudecode = yes

FILES = $(wildcard *.[ch])

EXTRA_DIST = $(FILES)
mkinstalldirs = $(SHELL) $(top_srcdir)/mkinstalldirs
CONFIG_CLEAN_FILES = 
DIST_COMMON =  Makefile.am Makefile.in


DISTFILES = $(DIST_COMMON) $(SOURCES) $(HEADERS) $(TEXINFOS) $(EXTRA_DIST)

TAR = gtar
GZIP_ENV = --best
all: all-redirect
.SUFFIXES:
$(srcdir)/Makefile.in: Makefile.am $(top_srcdir)/configure.in $(ACLOCAL_M4) 
	cd $(top_srcdir) && $(AUTOMAKE) --gnu runtime/SmilesGasEl/algo/huxu/Makefile

Makefile: $(srcdir)/Makefile.in  $(top_builddir)/config.status $(BUILT_SOURCES)
	cd $(top_builddir) \
	  && CONFIG_FILES=$(subdir)/$@ CONFIG_HEADERS= $(SHELL) ./config.status

tags: TAGS
TAGS:


distdir = $(top_builddir)/$(PACKAGE)-$(VERSION)/$(subdir)

subdir = runtime/SmilesGasEl/algo/huxu

distdir: $(DISTFILES)
	here=`cd $(top_builddir) && pwd`; \
	top_distdir=`cd $(top_distdir) && pwd`; \
	distdir=`cd $(distdir) && pwd`; \
	cd $(top_srcdir) \
	  && $(AUTOMAKE) --include-deps --build-dir=$$here --srcdir-name=$(top_srcdir) --output-dir=$$top_distdir --gnu runtime/SmilesGasEl/algo/huxu/Makefile
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

install-data-am:
install-data: install-data-am

install-am: all-am
	@$(MAKE) $(AM_MAKEFLAGS) install-exec-am install-data-am
install: install-am
uninstall-am:
uninstall: uninstall-am
all-am: Makefile
all-redirect: all-am
install-strip:
	$(MAKE) $(AM_MAKEFLAGS) AM_INSTALL_PROGRAM_FLAGS=-s install
installdirs:


mostlyclean-generic:

clean-generic:

distclean-generic:
	-rm -f Makefile $(CONFIG_CLEAN_FILES)
	-rm -f config.cache config.log stamp-h stamp-h[0-9]*

maintainer-clean-generic:
mostlyclean-am:  mostlyclean-generic

mostlyclean: mostlyclean-am

clean-am:  clean-generic mostlyclean-am

clean: clean-am

distclean-am:  distclean-generic clean-am

distclean: distclean-am

maintainer-clean-am:  maintainer-clean-generic distclean-am
	@echo "This command is intended for maintainers to use;"
	@echo "it deletes files that may require special tools to rebuild."

maintainer-clean: maintainer-clean-am

.PHONY: tags distdir info-am info dvi-am dvi check check-am \
installcheck-am installcheck install-exec-am install-exec \
install-data-am install-data install-am install uninstall-am uninstall \
all-redirect all-am all installdirs mostlyclean-generic \
distclean-generic clean-generic maintainer-clean-generic clean \
mostlyclean distclean maintainer-clean


# Tell versions [3.59,3.63) of GNU make to not export all variables.
# Otherwise a system limit (for SysV at least) may be exceeded.
.NOEXPORT:
