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
OBJ = lpo.o lpo.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) -o a.out $(OBJ) $(LIB)
lpo.core.o: lpo.h
lpo.o: lpo.c lpo.h
	$(CC) $(CC_OPT) -c lpo.c
clean:
	/bin/rm -f lpo.o lpo.core.o 
veryclean: clean 
	/bin/rm -f lpo.c lpo.h lpo.core.c lpo.make lpo.ref 
