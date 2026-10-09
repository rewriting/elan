#include "bitset_32.h"
#include "tools.h"

bitSet32 *intern_bitSet32_create(int size)
{
  bitSet32 *res;
  res=(bitSet32*) AMALLOC(sizeof(bitSet32_type));
  return res;
}

bitSet32 *bitSet32_copy(b)
  bitSet32 *b;
{
  bitSet32 *res;
  Verif_void(b,"bitSet32_copy(b)");
  bitSet32_create(res,bitSet32_size(b));
  res[0] = b[0];
  return res;
}

void bitSet32_print(b)
  bitSet32 *b;
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

#ifdef NOTMACRO
extern void bitSet32_init_size(bitSet32 *b,int size)
{
  Verif_void(b,"bitSet32_init_size(b)");
  b[0]=32;
}

void bitSet32_delete(b) 
  bitSet32 *b;
{
  Verif_void(b,"bitSet32_delete(b)");
  IFREE(b);
}

int bitSet32_size(b)
  bitSet32 *b;
{
  Verif_void(b,"bitSet32_size(b)");
  return 32;
}

int bitSet32_tab_size(b)
  bitSet32 *b;
{
  Verif_void(b,"bitSet32_tab_size(b)");
  return 1;
}

void bitSet32_set(b,bit)
  bitSet32 *b;
  int bit;
{
  Verif_void(b,"bitSet32_set(b)");
#ifdef DEBUG
  if(bit<0 || bit>bitSet32_size(b))
    {
      fprintf(stderr,"bitSet32_set error : bit<0 || bit>size\n");
      exit(0);
    }
#endif
  SETBIT32(b,bit);
}

void bitSet32_clear(b,bit)
  bitSet32 *b;
  int bit;
{
  Verif_void(b,"bitSet32_clear(b)");
#ifdef DEBUG
  if(bit<0 || bit>bitSet32_size(b))
    {
      fprintf(stderr,"bitSet32_clear error : bit<0 || bit>size\n");
      exit(0);
    }
#endif
  NULLBIT32(b,bit);
}

int bitSet32_get(b,bit)
  bitSet32 *b;
  int bit;
{
  Verif_void(b,"bitSet32_get(b)");
#ifdef DEBUG
  if(bit<0 || bit>bitSet32_size(b))
    {
      fprintf(stderr,"bitSet32_get error : bit<0 || bit>size\n");
      exit(0);
    }
#endif
  return ISSETBIT32(b,bit);
}

void bitSet32_init_set(b)
  bitSet32 *b;
{
  int i;
  Verif_void(b,"bitSet32_init_set(b)");
  b[0] = BITALL32;
}

void bitSet32_init_clear(b)
  bitSet32 *b;
{
  register int i;
  Verif_void(b,"bitSet32_init_clear(b)");
  b[0] = 0;
}

#endif

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
