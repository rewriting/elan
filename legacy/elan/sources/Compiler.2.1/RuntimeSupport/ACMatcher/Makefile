# Copyright: LORIA: CNRS, INPL, INRIA, UHP, Universite Nancy 2    
# Copyright: See details in the copyright file of the distribution    
#
#	$Source: /users/protheo/pot/ELAN/CVSELAN/elan/sources/Makefile,v $
#	$Revision: 1.14 $
#	$Date: 1998/11/24 15:39:55 $
#	$Author: moreau $

export VERSION       = version.3.4.alpha
#export CC            = gcc
#export CPP           = g++
export CC            = egcs
export CPP           = egcs -x c++
export FAST          = -O2
export ARCH          := $(shell uname -m)

# where the current distribution of ELAN is installed 
export ELANDIR       = $(HOME)/ELAN/elan
#  where the library of the ELAN system should be installed
export ELANLIB       = $(ELANDIR)/elanlib
#export ELANLIB       = /local/protheo/pot/ELAN/$(VERSION)/elanlib
# where the `new' distribution is installed
export ELANDIST      = /local/protheo/pot/ELAN/Test/Distrib/elan/sources

export ARCHFLAG      =
export SPECIAL_FLAG  =

ifeq '$(ARCH)' "i686"
export LEX = flex
export YACC = bison
export YLIB = -lfl
export YACC_OPT = -d --yacc
else
export LEX = lex
export YACC = yacc
export YLIB = -ly -ll
export YACC_OPT = -vd #--yacc
endif

NOWEBDIR = /users/protheo/cirstea/bin/NoWeb/$(ARCH)
export NOTANGLE= $(NOWEBDIR)/notangle
export NOWEAVE = $(NOWEBDIR)/noweave
export NOHTML  = $(NOWEAVE) -html -index 
export NOTOC   = $(NOWEBDIR)/htmltoc
export NOWEAVEFLAGS = -index


MKINSTALLDIRS  = $(ELANDIR)/sources/Scripts/mkinstalldirs

include Makefile.common

install-dirs:
	@for i in $(MAKEINSTALLDIRS) ; do \
		parent=`echo $$i | sed -e 's@/[^/]*$$@@' | sed -e 's@^$$@/@'`;\
		if [ -d $$parent ] ; then true ; else mkdir $$parent ; fi ;\
		if [ ! -d $$i ] ; then \
			if $(MKINSTALLDIRS) $$i ; then \
				true ; \
			else \
				exit 1 ; \
			fi ; \
		else \
			true ; \
		fi ; \
	done

dist-dirs:
	@for i in $(MAKEDISTDIRS) ; do \
		parent=`echo $$i | sed -e 's@/[^/]*$$@@' | sed -e 's@^$$@/@'`;\
		if [ -d $$parent ] ; then true ; else mkdir $$parent ; fi ;\
		if [ ! -d $$i ] ; then \
			if $(MKINSTALLDIRS) $$i ; then \
				true ; \
			else \
				exit 1 ; \
			fi ; \
		else \
			true ; \
		fi ; \
	done
