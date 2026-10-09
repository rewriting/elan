ARCH := $(shell uname -m)
CC = gcc -static -pipe
LIBELAN = -lchoice
FAST = -O2 -march=i686
YLIB = -lfl

INC = -I$(ELANLIB)/include/elan-compiler -I$(ELANLIB)/include
ATERM_OPT = -DBASETERM
BASIC_OPT = -DCOMPUTE_HCODE -DHASHCODE -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(ATERM_OPT)
CC_OPT = -DCSETCHP -DBITSET32 $(BASIC_OPT) $(INC)
LIBCOQ=
LIB = -L$(ELANLIB)/lib $(LIBCOQ) $(LIBELAN) -lruntime-base -learley $(YLIB) -lgc

all: server

server.o: server.c server.h
	$(CC) -I.elan.fac $(FAST) $(CC_OPT) -c server.c

server: server.o
	$(CC) $(CC_OPT) -o server server.o -L. -lfac $(LIB)
