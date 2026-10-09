#ifndef _bitset_h
#define _bitset_h
#include "tools.h"

#define bitSet_type unsigned int
typedef bitSet_type bitSet;

#define NBITS 1

#ifdef NOTMACRO
extern bitSet *intern_bitSet_create(int size);
#define bitSet_create(dest,size) dest=intern_bitSet_create(size);
#define bitSet_stack_create(dest,size) dest=intern_bitSet_create(size);
extern void bitSet_init_size(bitSet *b,int size);
extern void bitSet_delete();
#define bitSet_stack_delete(b) bitSet_delete(b)
extern void bitSet_set();
extern void bitSet_clear();
extern int  bitSet_get();
extern void bitSet_and();
extern void bitSet_or();
extern int  bitSet_size();
extern int  bitSet_equals();
extern void bitSet_print();
extern bitSet *bitSet_copy();
extern void bitSet_init_set();
extern void bitSet_init_clear();
void bitSet_not(bitSet *b1);
int bitSet_isclear(bitSet *b);

#else
extern bitSet *intern_bitSet_create(int size);
#define bitSet_create(b,size) {\
  b=(bitSet*)  AMALLOC((1+size)*sizeof(bitSet_type));\
  b[0]=size;}
#define bitSet_stack_create(b,size) bitSet b[1+size]; b[0]=size;
#define bitSet_init_size(b,size) b[0]=size;
#define bitSet_delete(b) IFREE(b)
#define bitSet_stack_delete(b) 
#define bitSet_set(b,bit) (b[1+bit]=1)
#define bitSet_clear(b,bit) (b[1+bit]=0)
#define bitSet_get(b,bit) (b[1+bit])
#define bitSet_size(b) (b[0])
extern void bitSet_print();
extern bitSet *bitSet_copy();
#define bitSet_init_set(b) {\
  int i; for(i=1 ; i <= bitSet_size(b) ; i++) b[i] = 1; }
#define bitSet_init_clear(b) {\
  int i; for(i=1 ; i <= bitSet_size(b) ; i++) b[i] = 0; }
void bitSet_not(bitSet *b1);
int bitSet_isclear(bitSet *b);
extern void bitSet_and();
extern void bitSet_or();
extern int  bitSet_equals();

#endif


#endif
