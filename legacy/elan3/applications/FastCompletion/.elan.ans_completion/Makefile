ARCH := $(shell uname -s)
ifneq (,$(findstring CYGWIN,$(ARCH)))
CC = gcc -static -pipe
CXX = g++ -Wl,--stack,0x2000000
LIBELAN = -lchoice
FAST = -O2 -march=i686
YLIB = -lfl
DLLIB = 
else
CC = gcc -static -pipe
CXX = libtool --mode=link g++
LIBELAN = -lchoice
FAST = -O2
YLIB = -lfl
DLLIB = -ldl
endif
INC = -I"/users/protheo/moreau/tmp/include" -I"/users/protheo/moreau/tmp/include/elan-compiler"
ATERM_OPT = -DBASETERM
BASIC_OPT = -DCOMPUTE_HCODE -DHASHCODE -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(ATERM_OPT)
CC_OPT = -DCSETCHP -DBITSET32 $(BASIC_OPT) $(INC)
LIBCOQ=
LIB = $(LIBCOQ) -L/users/protheo/moreau/tmp/lib $(LIBELAN) -lruntime-base -learley $(YLIB) $(DLLIB) -lgc
OBJ = ans_completion.o ans_completion.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
all: a.out

a.out: $(OBJ)
	$(CXX) $(CC_OPT) -o a.out $(OBJ) $(LIB)
ans_completion.core.o: ans_completion.h
ans_completion.o: ans_completion.c ans_completion.h
	$(CC) $(CC_OPT) -c ans_completion.c
clean:
	/bin/rm -f a.out ans_completion.o ans_completion.core.o 
veryclean: clean 
	/bin/rm -f ans_completion.c ans_completion.h ans_completion.core.c ../ans_completion.make ../ans_completion.ref 
