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
LIB = -L$(ELANLIB)/Compiler/$(ARCH) $(LIBELAN) -lRuntimeSupport -lACMatcher -lTermIO -learley $(YLIB) -lgc
OBJ = triplets.o triplets.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) -o a.out $(OBJ) $(LIB)
triplets.core.o: triplets.h
triplets.o: triplets.c triplets.h
	$(CC) $(CC_OPT) -c triplets.c
clean:
	/bin/rm -f triplets.o triplets.core.o 
veryclean: clean 
	/bin/rm -f triplets.c triplets.h triplets.core.c triplets.make triplets.ref 
