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
CC_OPT = -DNOTMACRO -DNEW_EXTRACT\
	-DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM\
	-DGREEDY202 -DGREEDY203  $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) $(LIBELAN) -lRuntimeSupport -lgc
OBJ = propc.o propc.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)

	$(CC) $(CC_OPT) $(OBJ) $(LIB)
#	quantify -best-effort $(CC) $(CC_OPT) $(OBJ) $(LIB)
	$(CC) $(CC_OPT) $(OBJ) $(LIB)

propc.core.o: propc.h
propc.o: propc.c propc.h ac_tools.c
	$(CC) $(CC_OPT) -c propc.c
clean:
	/bin/rm -f propc.o propc.core.o 
veryclean: clean 
	/bin/rm -f propc.c propc.h propc.core.c 
