ARCH := $(shell uname -s)
ifneq (,$(findstring CYGWIN,$(ARCH)))
CC = gcc -static -pipe
CXX = g++ -Wl,--stack,0x2000000
LIBELAN = -lchoice
FAST = -O2 -march=i686
YLIB = -lfl
else
CC = gcc -static -pipe
CXX = libtool --mode=link g++
LIBELAN = -lchoice
FAST = -O2
YLIB = -ll -ly
endif
INC = -I"/local/protheo/pot/ELAN/elan/serveurtx/include" -I"/local/protheo/pot/ELAN/elan/serveurtx/include/elan-compiler"
ATERM_OPT = -DBASETERM
BASIC_OPT = -DCOMPUTE_HCODE -DHASHCODE -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(ATERM_OPT)
CC_OPT = -DCSETCHP -DBITSET32 $(BASIC_OPT) $(INC)
LIBCOQ=
LIB = $(LIBCOQ) -L/local/protheo/pot/ELAN/elan/serveurtx/lib $(LIBELAN) -lruntime-base -learley $(YLIB) -lgc
OBJ = fib.o fib.core.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
all: a.out

a.out: $(OBJ)
	$(CXX) $(CC_OPT) -o a.out $(OBJ) $(LIB)
fib.core.o: fib.h
fib.o: fib.c fib.h
	$(CC) $(CC_OPT) -c fib.c
clean:
	/bin/rm -f a.out fib.o fib.core.o 
veryclean: clean 
	/bin/rm -f fib.c fib.h fib.core.c ../fib.make ../fib.ref 
