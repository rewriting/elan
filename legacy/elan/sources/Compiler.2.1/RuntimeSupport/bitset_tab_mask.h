#ifndef _bitset_h
#define _bitset_h
#include "tools.h"

#define bitSet_type unsigned int
typedef bitSet_type bitSet; 

#define NBITS (sizeof(bitSet_type)*8)
#define BITALL 0xffffffff
//#define ISSETBIT(b,bit) ((b[1+(bit/NBITS)] >> (bit%NBITS))&1)
/*
#define ISSETBIT(b,bit) (b[1+(bit/NBITS)]  &  1<<(bit%NBITS))
#define SETBIT(b,bit)   { b[1+(bit/NBITS)] |= 1<<(bit%NBITS);}
#define NULLBIT(b,bit)  { b[1+(bit/NBITS)] &= ~(1<<(bit%NBITS));}
*/
#define ISSETBIT(b,bit) ( b[1+(bit>>5)]  &  1<<(bit&0x1f))
#define SETBIT(b,bit)   { b[1+(bit>>5)] |= 1<<(bit&0x1f);}
#define NULLBIT(b,bit)  { b[1+(bit>>5)] &= ~(1<<(bit&0x1f)); }

#define HALFSHIFT NBITS/2
#define HALFBITALL 0x0000ffff
//#define HALFBITALL 0x00000000ffffffff

#ifdef NOTMACRO
extern bitSet *intern_bitSet_create(int size);
extern bitSet *intern_bitSet_GC_create(int size);
#define bitSet_create(dest,size) dest=intern_bitSet_create(size);
#define bitSet_GC_create(dest,size) dest=intern_bitSet_GC_create(size);
#define bitSet_stack_create(dest,size) dest=intern_bitSet_create(size);
extern void bitSet_init_size(bitSet *b,int size);
extern void bitSet_delete();
extern void bitSet_GC_delete();
#define bitSet_stack_delete(b) bitSet_delete(b)
extern void bitSet_set();
extern void bitSet_clear();

extern bitSet *bitSet_copy(bitSet *b);
extern void bitSet_print();

extern int  bitSet_get();
extern void bitSet_and();
extern void bitSet_or();
extern int  bitSet_size();
extern int  bitSet_tab_size();
extern int  bitSet_equals();
extern void bitSet_init_set();
extern void bitSet_init_clear();
void bitSet_not(bitSet *b1);
int bitSet_isclear(bitSet *b);

#else

extern bitSet *intern_bitSet_create(int size);
extern bitSet *intern_bitSet_GC_create(int size);
#define bitSet_init_size(b,size) b[0]= (size<<HALFSHIFT) + (1+(size/NBITS));

#define bitSet_create(b,size) {\
  b=(bitSet*) AMALLOC((1+(size/NBITS)+1)*sizeof(bitSet_type));\
  bitSet_init_size(b,size); }
#define bitSet_GC_create(b,size) {\
  b=(bitSet*) AMALLOC((1+(size/NBITS)+1)*sizeof(bitSet_type));\
  bitSet_init_size(b,size); }
#define bitSet_stack_create(b,size) {\
  bitSet b[(1+(size/NBITS)+1)];\
   bitSet_init_size(b,size); }
#define bitSet_delete(b)    IFREE(b)
#define bitSet_GC_delete(b)    FREE(b)
#define bitSet_stack_delete(b) 
#define bitSet_size(b)      (b[0]>>HALFSHIFT)
#define bitSet_tab_size(b)  (b[0] & HALFBITALL)
//((b[0]<<HALFSHIFT)>>HALFSHIFT)

#define bitSet_set(b,bit)   SETBIT(b,bit)
#define bitSet_clear(b,bit) NULLBIT(b,bit)
#define bitSet_get(b,bit)   ISSETBIT(b,bit)
#define bitSet_init_set(b) {\
  int i; for(i=1 ; i <= bitSet_tab_size(b) ; i++) b[i] = BITALL;}
#define bitSet_init_clear(b) {\
  int i; for(i=1 ; i <= bitSet_tab_size(b) ; i++) b[i] = 0; }

extern bitSet *bitSet_copy(bitSet *b);
extern void bitSet_print();

/*
void bitSet_not(bitSet *b1);
int bitSet_isclear(bitSet *b);
extern void bitSet_and();
extern void bitSet_or();
extern int  bitSet_equals();
*/
#endif

extern void bitSet_or(bitSet *b1,bitSet *b2);
extern int bitSet_nb_bit(bitSet *b);
extern int bitSet_isnull(bitSet *b);

#endif
