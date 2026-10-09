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
LIBELAN = -lCsetChoicePoint
FAST = -O3 -fomit-frame-pointer -march=i686
YLIB = -lfl
else
CC = gcc -pipe
LIBELAN = -lCsetChoicePoint
FAST = -O2
YLIB = -ll -ly
endif
endif
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiler/$(ARCH)
CC_OPT = -DCSETCHP -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) $(LIBELAN) -lRuntimeSupport -lACMatcher -lTermIO -learley $(YLIB) -lgc
OBJ = new_meta.o new_meta.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) -o a.out $(OBJ) $(LIB)
new_meta.core.o: new_meta.h
new_meta.o: new_meta.c new_meta.h
	$(CC) $(CC_OPT) -c new_meta.c
clean:
	/bin/rm -f new_meta.o new_meta.core.o 
veryclean: clean 
	/bin/rm -f new_meta.c new_meta.h new_meta.core.c new_meta.make new_meta.ref 
