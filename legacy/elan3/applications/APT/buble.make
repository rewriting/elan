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
OBJ = buble.o buble.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
buble.core.o: buble.h
buble.o: buble.c buble.h
	$(CC) $(CC_OPT) -c buble.c
clean:
	/bin/rm -f buble.o buble.core.o 
veryclean: clean 
	/bin/rm -f buble.c buble.h buble.core.c 
