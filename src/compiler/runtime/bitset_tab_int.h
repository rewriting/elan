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

#define NBITS 1

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
