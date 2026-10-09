ATERMDIST=$(HOME)/ELAN/Tom/Aterm
ATERMLIB=${ATERMDIST}/lib
ATERMINC=${ATERMDIST}/include

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
FAST = -O3 -march=i686
YLIB = -lfl
else
CC = gcc -pipe
LIBELAN = -lCsetChoicePoint
FAST = -O2
YLIB = -ll -ly
endif
endif
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiler/$(ARCH) -I$(ATERMINC)
BASIC_OPT = -DCOMPUTE_HCODE -DHASHCODE -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR
CC_OPT = -DCSETCHP -DBITSET32 $(BASIC_OPT) $(INC)
LIBCOQ=
LIB = $(LIBCOQ) -L$(ELANLIB)/Compiler/$(ARCH) $(LIBELAN) -lTermIO -lRuntimeSupport -lACMatcher -learley $(YLIB) -lgc -L${ATERMLIB} -lATerm
OBJ = ans.o ans.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
all: a.out

a.out: $(OBJ)
	ar r a.out $(OBJ)
	ranlib a.out
ans.core.o: ans.h
ans.o: ans.c ans.h
	$(CC) $(CC_OPT) -c ans.c
clean:
	/bin/rm -f a.out ans.o ans.core.o 
veryclean: clean 
	/bin/rm -f ans.c ans.h ans.core.c ../ans.make ../ans.ref 
