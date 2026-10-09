/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
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
