ARCH = `uname -m`
ifeq '$(ARCH)' "alpha"
CC = cc
LIBELAN = -lelanmv -lexc
FAST = -fast
else
CC = gcc
LIBELAN = -lelan
FAST = -O2
endif
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiled/$(ARCH)
CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) $(LIBELAN) -lRuntimeSupport -lgc
OBJ = queens.o queens.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
queens.core.o: queens.h
queens.o: queens.c queens.h
	$(CC) $(CC_OPT) -c queens.c
clean:
	/bin/rm -f queens.o queens.core.o 
veryclean: clean 
	/bin/rm -f queens.c queens.h queens.core.c 
