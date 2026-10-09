
#ifdef BITSETMASK
#include "bitset_tab_mask.h"
#endif

#ifdef BITSETINT
#include "bitset_tab_int.h"
#endif

#ifdef BITSET32
#include "bitset_32.h"
#else

#define bitSet32 bitSet
#define bitSet32_create bitSet_create
#define bitSet32_stack_create(dest,size) bitSet *dest; bitSet_stack_create(dest,size)
#define bitSet32_get bitSet_get
#define bitSet32_delete bitSet_delete
#define bitSet32_stack_delete bitSet_stack_delete
#define bitSet32_init_clear bitSet_init_clear
#define bitSet32_set bitSet_set

#endif
