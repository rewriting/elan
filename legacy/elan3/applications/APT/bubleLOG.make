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
OBJ = bubleLOG.o bubleLOG.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
bubleLOG.core.o: bubleLOG.h
bubleLOG.o: bubleLOG.c bubleLOG.h
	$(CC) $(CC_OPT) -c bubleLOG.c
clean:
	/bin/rm -f bubleLOG.o bubleLOG.core.o 
veryclean: clean 
	/bin/rm -f bubleLOG.c bubleLOG.h bubleLOG.core.c 
