## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign dejagnu
RUNTESTDEFAULTFLAGS = 

SUFFIXES = .eln .lgi
bin_PROGRAMS = test_bool

test_bool_SOURCES = test_bool.lgi test.eln

test_bool : $(test_bool_SOURCES)

# % : %.lgi
#	$(ELANC) $(ELANCFLAGS) -nocode -output $* $*.lgi $*.spc

% : %.lgi
	-rm -f $*.ref
	$(ELANC) $(ELANCFLAGS) -l $(top_srcdir)/src -output $* $*.lgi
	$(MAKE) -f $*.make

mostlyclean-compile:
	-rm -f *.o core *.core
	-rm -f *.ref *.make
	-rm -rf .elan.*
