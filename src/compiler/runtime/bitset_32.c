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
#include "bitset_32.h"
#include "tools.h"

bitSet32 *intern_bitSet32_create(int size)
{
  bitSet32 *res;
  res=(bitSet32*) AMALLOC(sizeof(bitSet32_type));
  return res;
}

bitSet32 *bitSet32_copy(bitSet32 *b)
{
  bitSet32 *res;
  Verif_void(b,"bitSet32_copy(b)");
  bitSet32_create(res,bitSet32_size(b));
  res[0] = b[0];
  return res;
}

void bitSet32_print(bitSet32 *b)
{
  int i;
  int res=0;
  Verif_void(b,"bitSet32_print(b)");
  printf("[%d] ", bitSet32_size(b));
  for(i=0 ; i<bitSet32_size(b) ; i++)
    {
      res|=bitSet32_get(b,i);
      if(bitSet32_get(b,i))
	printf("%d ",i);
    }
  if(res==0)
    printf("null");
}


/*
void bitSet32_not(bitSet32 *b1)
{
  int i;
  Verif_void(b1,"bitSet32_not(b1)");
  for(i=0 ; i < bitSet32_size(b1) ; i++)
    b1[1+i] = !b1[1+i];
}

int bitSet32_isclear(bitSet32 *b)
{
  int i,clear;
  Verif_void(b,"bitSet32_isclear(b)");
  for(i=0, clear=1 ; i<bitSet32_size(b) && clear ; i++)
    clear &= (b[1+i]==0);
  return clear;
}


void bitSet32_and(b1,b2)
  bitSet32 *b1,*b2;
{
  int i;
  Verif_void(b1,"bitSet32_and(b1)");
  Verif_void(b2,"bitSet32_and(b2)");

  if( bitSet32_size(b1) <=  bitSet32_size(b2))
    {
      for(i=0 ; i < bitSet32_size(b1) ; i++)
	b1[1+i] &= b2[1+i];
    }
  else
    {
      for(i=0 ; i < bitSet32_size(b2) ; i++)
	b1[1+i] &= b2[1+i];
      for(i=bitSet32_size(b2) ; i < bitSet32_size(b1) ; i++)
	b1[1+i] = 0;
    }
}

void bitSet32_or(b1,b2)
  bitSet32 *b1,*b2;
{
  int i;
  Verif_void(b1,"bitSet32_or(b1)");
  Verif_void(b2,"bitSet32_or(b2)");

  if(bitSet32_size(b1) <= bitSet32_size(b2))
    for(i=0 ; i < bitSet32_size(b1) ; i++)
      b1[1+i] |= b2[1+i];
  else
    for(i=0 ; i < bitSet32_size(b2) ; i++)
      b1[1+i] |= b2[1+i];
}

int bitSet32_equals(b1,b2)
  bitSet32 *b1,*b2;
{
  int i,res=1;
  Verif_void(b1,"bitSet32_equals(b1)");
  Verif_void(b2,"bitSet32_equals(b2)");
  if(bitSet32_size(b1) != bitSet32_size(b2))
    return 0;
  for(i=0 ; i < bitSet32_size(b1) && res ; i++)
      res &= (b1[1+i] == b2[1+i]);
  return res;
}
*/
