ARCH := $(shell uname -m)
ifeq '$(ARCH)' "alpha"
CC = gcc
#LIBELAN = -lelanmv -lexc
LIBELAN = -lelan
FAST = 
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
OBJ = cs.o cs.split.fun_202.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) -o a.out $(OBJ) $(LIB)
cs.split.fun_202.o: cs.h
cs.o: cs.c cs.h
	$(CC) $(CC_OPT) -c cs.c
clean:
	/bin/rm -f cs.o cs.split.fun_202.o 
veryclean: clean 
	/bin/rm -f cs.c cs.h cs.split.fun_202.c cs.make cs.ref 
