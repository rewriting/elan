#include "bitset.h"
#include "tools.h"

bitSet *intern_bitSet_create(int size)
{
  bitSet *res;
  res=(bitSet*) IMALLOC((1+(size/NBITS)+1)*sizeof(bitSet_type));
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

#ifdef NOTMACRO
extern void bitSet_init_size(bitSet *b,int size)
{
  Verif_void(b,"bitSet_init_size(b)");
  b[0]=size;
}

void bitSet_delete(b)
  bitSet *b;
{
  Verif_void(b,"bitSet_delete(b)");
  IFREE(b);
}

int bitSet_size(b)
  bitSet *b;
{
  Verif_void(b,"bitSet_size(b)");
  return b[0];
}

void bitSet_set(b,bit)
  bitSet *b;
  int bit;
{
  Verif_void(b,"bitSet_set(b)");
#ifdef DEBUG
  if(bit<0 || bit>bitSet_size(b))
    {
      fprintf(stderr,"bitSet_set error : bit<0 || bit>size\n");
      exit(0);
    }
#endif
  SETBIT(b,bit);
}

void bitSet_clear(b,bit)
  bitSet *b;
  int bit;
{
  Verif_void(b,"bitSet_clear(b)");
#ifdef DEBUG
  if(bit<0 || bit>bitSet_size(b))
    {
      fprintf(stderr,"bitSet_clear error : bit<0 || bit>size\n");
      exit(0);
    }
#endif
  NULLBIT(b,bit);
}

int bitSet_get(b,bit)
  bitSet *b;
  int bit;
{
  Verif_void(b,"bitSet_get(b)");
#ifdef DEBUG
  if(bit<0 || bit>bitSet_size(b))
    {
      fprintf(stderr,"bitSet_get error : bit<0 || bit>size\n");
      exit(0);
    }
#endif
  return ISSETBIT(b,bit);
}

void bitSet_init_set(b)
  bitSet *b;
{
  int i;
  Verif_void(b,"bitSet_init_set(b)");
  for(i=1 ; i <=  1+(bitSet_size(b)/NBITS) ; i++)
    b[i] = BITALL;
}

void bitSet_init_clear(b)
  bitSet *b;
{
  register int i;
  Verif_void(b,"bitSet_init_clear(b)");
  for(i=1 ; i <= 1+(bitSet_size(b)/NBITS) ; i++)
    b[i] = 0;
}

#endif

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
