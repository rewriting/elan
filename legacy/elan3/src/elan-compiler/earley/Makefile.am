CFLAGSPERSO   = -DCOMMAND -DBINS -DPEM -DBORO -DANYS -DSYMBS -DSELECT -DCAPAT -DLIN -DEARLEY -DSTRINGS -DMETA -DSEMANTIC -DIOS -DNORM -DEXTEND -DATERM -DCSTRAT -DPARTE -DCOMPRIM -DRUNTIME

AM_CXXFLAGS = -O2 -w $(CFLAGSPERSO)

lib_LIBRARIES = libearley.a

libearley_a_SOURCES =\
		commondefs.cc lbuffer.cc misc.cc stringtab.cc\
		earley.cc grammar.cc lstream.cc ochstream.cc\
		esemact.cc ichstream.cc mallo.cc runtimeInit.cc\
		RTCommons.h command.h lstream.h mitab.h stringtab.h\
		codes.h commondefs.h mallo.h runtimeInit.h termdefs.h

EXTRA_DIST	= files.c
