
#include <malloc.h>
#include <stdio.h>

/* Y'a un petit truc ici, il faut placer param en premier ... */
typedef struct Param
{
  long param[10];
  long num;
  long arity;
} Param;

void f1(long p1) {printf("f1(%d)\n",p1);}
void f2(long p1, long p2) {printf("f2(%d,%d)\n",p1,p2);}
void f3(long p1, long p2, long p3) {printf("f3(%d,%d,%d)\n",p1,p2,p3);}

void (*f[10])()={f1,f2,f3};

void appel(Param p)
{
  f[p.num](p);
}

main()
{
  Param p;
  
  p.num=0;
  p.arity=1;
  p.param[0]=0;
  appel(p);       
  
  p.num=1;
  p.arity=2;
  p.param[0]=1;
  p.param[1]=2;
  appel(p);

  p.num=2;
  p.arity=3;
  p.param[0]=3;
  p.param[1]=4;
  p.param[2]=5;
  appel(p);
}

