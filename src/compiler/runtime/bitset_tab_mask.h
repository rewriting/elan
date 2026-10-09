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
/* 1u: 1<<31 overflows an int (the bits are the same) */
#define ISSETBIT(b,bit) ( b[1+(bit>>5)]  &  1u<<(bit&0x1f))
#define SETBIT(b,bit)   { b[1+(bit>>5)] |= 1u<<(bit&0x1f);}
#define NULLBIT(b,bit)  { b[1+(bit>>5)] &= ~(1u<<(bit&0x1f)); }

#define HALFSHIFT NBITS/2
#define HALFBITALL 0x0000ffff
//#define HALFBITALL 0x00000000ffffffff


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
  int i; for(i=1 ; (unsigned)i <= bitSet_tab_size(b) ; i++) b[i] = 0; }

extern bitSet *bitSet_copy(bitSet *b);
extern void bitSet_print(bitSet *b);

/*
void bitSet_not(bitSet *b1);
int bitSet_isclear(bitSet *b);
extern void bitSet_and(bitSet *b1, bitSet *b2);
extern void bitSet_or(bitSet *b1, bitSet *b2);
extern int  bitSet_equals(bitSet *b1, bitSet *b2);
*/

extern void bitSet_or(bitSet *b1,bitSet *b2);
extern int bitSet_nb_bit(bitSet *b);
extern int bitSet_isnull(bitSet *b);

#endif
