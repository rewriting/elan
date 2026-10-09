ARCH = `uname -m`
CC = gcc
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiled/$(ARCH)
CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) -lelan -lRuntimeSupport -lgc
OBJ = queensLIN.o queensLIN.core.o 
.c.o:
	$(CC) -O2 $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
queensLIN.core.o: queensLIN.h
queensLIN.o: queensLIN.c queensLIN.h
	$(CC) $(CC_OPT) -c queensLIN.c
clean:
	/bin/rm -f queensLIN.o queensLIN.core.o 
veryclean: clean 
	/bin/rm -f queensLIN.c queensLIN.h queensLIN.core.c 
