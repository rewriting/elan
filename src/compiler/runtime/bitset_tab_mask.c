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
#include "bitset.h"
#include "tools.h"

bitSet *intern_bitSet_create(int size)
{
  bitSet *res;
  res=(bitSet*) AMALLOC((1+(size/NBITS)+1)*sizeof(bitSet_type));
  bitSet_init_size(res,size);
  return res;
}

bitSet *intern_bitSet_GC_create(int size)
{
  bitSet *res;
  res=(bitSet*) AMALLOC((1+(size/NBITS)+1)*sizeof(bitSet_type));
  bitSet_init_size(res,size);
  return res;
}

bitSet *bitSet_copy(b)
  bitSet *b;
{
  int i;
  bitSet *res;
  Verif_void(b,"bitSet_copy(b)");
  bitSet_create(res,bitSet_size(b));
  for(i=1 ; i <= bitSet_tab_size(b) ; i++)
    res[i] = b[i];
  return res;
}


void bitSet_print(b)
  bitSet *b;
{
  int i;
  int res=0;
  Verif_void(b,"bitSet_print(b)");
  printf("[%d] ", bitSet_size(b));
  for(i=0 ; i<bitSet_size(b) ; i++)
    {
      res|=bitSet_get(b,i);
      if(bitSet_get(b,i))
	printf("%d ",i);
    }
  if(res==0)
    printf("null");
}

void bitSet_or(b1,b2)
  bitSet *b1,*b2;
{
  int i;
  Verif_void(b1,"bitSet_or(b1)");
  Verif_void(b2,"bitSet_or(b2)");

  if(bitSet_size(b1) <= bitSet_size(b2))
    for(i=1 ; i <= bitSet_tab_size(b1) ; i++)
      b1[i] |= b2[i];
  else
    for(i=1 ; i <= bitSet_tab_size(b2) ; i++)
      b1[i] |= b2[i];
}

int bitSet_nb_bit(bitSet *b)
{
  int i;
  int res=0;
  for(i=0 ; i<bitSet_size(b) ; i++) {
    if(bitSet_get(b,i))
      res++;
  }
  return res;
}

int bitSet_isnull(bitSet *b) {
  int i;
  int isnull=1;
  Verif_void(b,"bitSet_isnull(b)");
  for(i=1 ; isnull && i <= bitSet_tab_size(b) ; i++) {
    isnull &= (b[i] == 0);
  }
  return isnull;
}

/*
void bitSet_not(bitSet *b1)
{
  int i;
  Verif_void(b1,"bitSet_not(b1)");
  for(i=0 ; i < bitSet_size(b1) ; i++)
    b1[1+i] = !b1[1+i];
}

int bitSet_isclear(bitSet *b)
{
  int i,clear;
  Verif_void(b,"bitSet_isclear(b)");
  for(i=0, clear=1 ; i<bitSet_size(b) && clear ; i++)
    clear &= (b[1+i]==0);
  return clear;
}


void bitSet_and(b1,b2)
  bitSet *b1,*b2;
{
  int i;
  Verif_void(b1,"bitSet_and(b1)");
  Verif_void(b2,"bitSet_and(b2)");

  if( bitSet_size(b1) <=  bitSet_size(b2))
    {
      for(i=0 ; i < bitSet_size(b1) ; i++)
	b1[1+i] &= b2[1+i];
    }
  else
    {
      for(i=0 ; i < bitSet_size(b2) ; i++)
	b1[1+i] &= b2[1+i];
      for(i=bitSet_size(b2) ; i < bitSet_size(b1) ; i++)
	b1[1+i] = 0;
    }
}

void bitSet_or(b1,b2)
  bitSet *b1,*b2;
{
  int i;
  Verif_void(b1,"bitSet_or(b1)");
  Verif_void(b2,"bitSet_or(b2)");

  if(bitSet_size(b1) <= bitSet_size(b2))
    for(i=0 ; i < bitSet_size(b1) ; i++)
      b1[1+i] |= b2[1+i];
  else
    for(i=0 ; i < bitSet_size(b2) ; i++)
      b1[1+i] |= b2[1+i];
}

int bitSet_equals(b1,b2)
  bitSet *b1,*b2;
{
  int i,res=1;
  Verif_void(b1,"bitSet_equals(b1)");
  Verif_void(b2,"bitSet_equals(b2)");
  if(bitSet_size(b1) != bitSet_size(b2))
    return 0;
  for(i=0 ; i < bitSet_size(b1) && res ; i++)
      res &= (b1[1+i] == b2[1+i]);
  return res;
}
*/
