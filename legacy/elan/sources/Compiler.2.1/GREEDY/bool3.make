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
CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM\
	 -DGREEDY211 -DGREEDY212 $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) $(LIBELAN) -lRuntimeSupport -ll -ly -lgc
OBJ = bool3.o bool3.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)
bool3.core.o: bool3.h
bool3.o: bool3.c bool3.h
	$(CC) $(CC_OPT) -c bool3.c
clean:
	/bin/rm -f bool3.o bool3.core.o 
veryclean: clean 
	/bin/rm -f bool3.c bool3.h bool3.core.c 
