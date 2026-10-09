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
OBJ = cs3.o cs3.split.fun_203.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) -o a.out $(OBJ) $(LIB)
cs3.split.fun_203.o: cs3.h
cs3.o: cs3.c cs3.h
	$(CC) $(CC_OPT) -c cs3.c
clean:
	/bin/rm -f cs3.o cs3.split.fun_203.o 
veryclean: clean 
	/bin/rm -f cs3.c cs3.h cs3.split.fun_203.c cs3.make cs3.ref 
