ARCH := $(shell uname -m)
ifeq '$(ARCH)' "alpha"
CC = cc
#LIBELAN = -lelanmv -lexc
LIBELAN = -lelan
FAST = -fast
YLIB = -ll -ly
else
ifneq (,$(findstring 86,$(ARCH)))
CC = gcc -pipe
LIBELAN = -lelan
FAST = 
YLIB = -lfl
else
CC = gcc -pipe
LIBELAN = -lelan
FAST = -O2
YLIB = -ll -ly
endif
endif
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiler/$(ARCH)
CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) $(LIBELAN) -lRuntimeSupport -lACMatcher $(YLIB) -lgc
OBJ = nsh.o nsh.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) -o a.out $(OBJ) $(LIB)
nsh.core.o: nsh.h
nsh.o: nsh.c nsh.h
	$(CC) $(CC_OPT) -c nsh.c
clean:
	/bin/rm -f nsh.o nsh.core.o 
veryclean: clean 
	/bin/rm -f nsh.c nsh.h nsh.core.c nsh.make nsh.ref 
