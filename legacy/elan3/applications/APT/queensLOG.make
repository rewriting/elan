ARCH = `uname -m`
CC = gcc
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiled/$(ARCH)
CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) -lelan -lRuntimeSupport -lgc
OBJ = queensLOG.o queensLOG.core.o 
.c.o:
	$(CC) -O2 $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
queensLOG.core.o: queensLOG.h
queensLOG.o: queensLOG.c queensLOG.h
	$(CC) $(CC_OPT) -c queensLOG.c
clean:
	/bin/rm -f queensLOG.o queensLOG.core.o 
veryclean: clean 
	/bin/rm -f queensLOG.c queensLOG.h queensLOG.core.c 
