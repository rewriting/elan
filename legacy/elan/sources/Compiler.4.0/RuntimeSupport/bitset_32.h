#ifndef _bitset_32_h
#define _bitset_32_h
#include "tools.h"

#define bitSet32_type unsigned int
typedef bitSet32_type bitSet32; 

#define NBITS32 (sizeof(bitSet32_type)*8)
#define BITALL32 0xffffffff
#define ISSETBIT32(b,bit) ((b[0] >> bit)&1)
#define SETBIT32(b,bit)   b[0] |= 1<< bit;
#define NULLBIT32(b,bit)  b[0] &= ~(1<<bit);

extern bitSet32 *intern_bitSet32_create(int size);
extern bitSet32 *bitSet32_copy(bitSet32 *b);
extern void bitSet32_print();

#ifdef NOTMACRO
#define bitSet32_create(dest,size) dest=intern_bitSet32_create(size);
#define bitSet32_stack_create(dest,size) bitSet *dest; dest=intern_bitSet32_create(size);
extern void bitSet32_init_size(bitSet32 *b,int size);
extern void bitSet32_delete();
#define bitSet32_stack_delete(b) bitSet32_delete(b)
extern void bitSet32_set();
extern void bitSet32_clear();
extern int  bitSet32_get();
extern void bitSet32_and();
extern void bitSet32_or();
extern int  bitSet32_size();
extern int  bitSet32_tab_size();
extern int  bitSet32_equals();
extern void bitSet32_init_set();
extern void bitSet32_init_clear();
void bitSet32_not(bitSet32 *b1);
int bitSet32_isclear(bitSet32 *b);

#else

extern bitSet32 *intern_bitSet32_create(int size);
#define bitSet32_init_size(b,size) 
#define bitSet32_create(b,size) b=(bitSet32*) AMALLOC(sizeof(bitSet32_type));
#define bitSet32_stack_create(b,size) bitSet32 b[1];
#define bitSet32_delete(b)    IFREE(b)
#define bitSet32_stack_delete(b) 
#define bitSet32_size(b)      32
#define bitSet32_tab_size(b)  1
#define bitSet32_set(b,bit)   SETBIT32(b,bit)
#define bitSet32_clear(b,bit) NULLBIT32(b,bit)
#define bitSet32_get(b,bit)   ISSETBIT32(b,bit)
#define bitSet32_init_set(b)  b[0] = BITALL32;
#define bitSet32_init_clear(b) b[0] = 0; 

/*
void bitSet32_not(bitSet32 *b1);
int bitSet32_isclear(bitSet32 *b);
extern void bitSet32_and();
extern void bitSet32_or();
extern int  bitSet32_equals();
*/
#endif

#endif
