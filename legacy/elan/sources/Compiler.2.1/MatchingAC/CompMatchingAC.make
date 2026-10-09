ARCH = `uname -m`
CC = gcc
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiled/$(ARCH)
CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM $(INC) -DPDEBUGG -DDEBUG
LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) -lDBelanmv -lRuntimeSupport -lgc -lexc
OBJ = CompMatchingAC.o CompMatchingAC.core.o 
.c.o:
	$(CC) -O2 $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
CompMatchingAC.core.o: CompMatchingAC.h
CompMatchingAC.o: CompMatchingAC.c CompMatchingAC.h
	$(CC) $(CC_OPT) -c CompMatchingAC.c
clean:
	/bin/rm -f CompMatchingAC.o CompMatchingAC.core.o 
veryclean: clean 
	/bin/rm -f CompMatchingAC.c CompMatchingAC.h CompMatchingAC.core.c 
