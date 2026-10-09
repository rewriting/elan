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
OBJ = bubleLIN.o bubleLIN.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
bubleLIN.core.o: bubleLIN.h
bubleLIN.o: bubleLIN.c bubleLIN.h
	$(CC) $(CC_OPT) -c bubleLIN.c
clean:
	/bin/rm -f bubleLIN.o bubleLIN.core.o 
veryclean: clean 
	/bin/rm -f bubleLIN.c bubleLIN.h bubleLIN.core.c 
