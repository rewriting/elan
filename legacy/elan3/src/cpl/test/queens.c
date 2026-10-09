/*
 * CPL - The Nancy ChoicePoint Library
 */

#include <stdio.h>
#include <stdlib.h>
#include <choice.h>
//------------------------------------------------------------------

int from1ton(int n) {
  int i;
  for(i=1;i<n; i++) {
    if(setChoicePoint()==0) {
        // set a choice point and return i
      return(i);
    }
  }
  return(i);
}
//-----------------------------------------------------------------

int array[20];

queens(int n) {
  int i,j,k;
  for(i=1 ; i<=n ; i++) {
    j=from1ton(n); /* choose a j */
      //printf("j=%d\n",j);
    for(k=1 ; k<i ; k++) {
      // int diff = array[k]-j;
      if(array[k]==j || array[k]-j==k-i || array[k]-j==i-k) {
	fail(); /* choose another j */
      }
    }
    array[i] = j;
  }

  fprintf(stdout,"result: ");
  for (i=1;i<=n;i++) fprintf(stdout," %d",array[i]);
  fprintf(stdout,"\n");

}
//-----------------------------------------------------------------
void tendigit(int N) {
  int i,j,k,l,count,sum;
  sum=0;
  for(i=0 ; i<=N ; i++) {
    j=from1ton(1 + N+1 - sum)-1; /* choose a j */
    array[i]=j;
    sum+=j;
  }

  if(sum != N+1)
    fail(); /* choose another j */

  for(k=0 ; k<=N ; k++) {
    count=0;
    for(l=0 ; l<=N ; l++) {
      if(array[l]==k) {
	count++;
      }
      if(!(array[k]>=count)) 
	fail(); /* choose another j */
    }
    if(array[k]!=count)
      fail(); /* choose another j */
  }
  for (i=0;i<=N;i++) fprintf(stdout," %d",array[i]);
  fprintf(stdout,"\n");
}

//-----------------------------------------------------------------

void move(int x, int *pty) {
  int i;
  i=from1ton(9);
  (*pty)=x-i;
  if( (*pty)<0 ) fail();
}

int win(int *ptx) {
  int y;
  move(*ptx,&y);
  //if( win(&y) ) fail();
  return 1;
}

//-----------------------------------------------------------------
#define LIMIT 12
long D[LIMIT];
int compteur=0;

long okay(long i, long j) {
  long k;
  for (k=0;k<i;k++) 
    if (D[k]==j || D[k]-j==i-k || j-D[k]==i-k) 
      return 0;
  return 1;
}

void Ndamy() {
  long i,j;
  for(i=0;i<LIMIT;i++) {
    for(j=0;j<LIMIT;j++) {
      if(!setChoicePoint()) {
	if(okay(i,j)) {
          D[i] = j;
          goto next;
        } else {
	  fail();
        }
      } else {
	continue;
      }
    }
    fail();
  next:;
  }
  compteur++;
  //   printf("Solution: "); for(k=0; k<LIMIT; k++) printf("%2d.",D[k]); printf("\n");
  return;
}

void Ndamy2() {
  long i,j,k;
  for(i=0;i<LIMIT;i++) {
    for(j=0;j<LIMIT;j++) {
      if(!setChoicePoint()) {
	if(okay(i,j)) {
          D[i] = j;
          goto next;
        } else {
	  fail();
        }
      } else {
	continue;
      }
    }
    fail();
  next:;
  }
  compteur++;
  printf("Solution: "); for(k=0; k<LIMIT; k++) printf("%2d.",D[k]); printf("\n");
  if(setChoicePoint) {
    fail();
  } else {
    return;
  }
}

//-----------------------------------------------------------------   
/*
int * ff(int arin, int &arout)
{ int i;
  i=0; arout=0;
  fprintf(stdout,"[ff1] arin==%d, i==%d, arout==%d\n",arin,i,arout);
  if (!setChoicePoint()) return(&i);
  i++; arout++;
  fprintf(stdout,"[ff2] arin==%d, i==%d, arout==%d\n",arin,i,arout);
  return(&i);
} 
*/  
//-----------------------------------------------------------------
int tvar=0;
int indvar;
   
void testchp(int n)
{ int i;
  i=0;    tvar++;  i++;
  if (n==0) {
    if (!setChoicePoint()) {
          fprintf(stdout,"setting chp i==%d, tvar==%d\n",i,tvar);
        } else {
          fprintf(stdout,"after fail  i==%d, tvar==%d\n",i,tvar);
        } 
  } else {
    testchp(n-1);
  }
  tvar --;      i--;  
} 
//-----------------------------------------------------------------
int testfail(int n)
{ int res;
  res = 0;
  if (n==0) {
    fail();
  } else {
    res = res + testfail(n-1);
  }
  return(res);
}
//-----------------------------------------------------------------
int goo()                             
{
  if(!setChoicePoint()) 
    return 2222; else return 3333;
}
//-----------------------------------------------------------------
int ttvar;
int testscp()
{
  int b;
  int i=0;
  ttvar=0;
  printf("debut testscp\n");
  b=setChoicePoint();
  printf("b=%d\ti=%d\t ttvar=%d\n",b,i,ttvar);
  i++;
  ttvar++;
  
  b=setChoicePoint(); 
  printf("b=%d\ti=%d\t ttvar=%d\n",b,i,ttvar); 
  i++; 
  ttvar++; 
  printf("fin testscp\n");
}


//----------------------------------------------------------------- 
// Eelco
void eelco_g(int *ptr_x);
void eelco_f() {
  int x = 0;

  if(localSetChoicePoint() == 0) {
    printf("x1 = %d\n",x);
  } else {
    printf("back1\n");
    printf("x1 = %d\n",x);
  }
  eelco_g(&x);
  if(localSetChoicePoint() == 0) {
    printf("x2 = %d\n",x);
  } else {
    printf("back2\n");
    printf("x2 = %d\n",x);
  }
  fail();
}

void eelco_g(int *ptr_x) {
  *ptr_x = 1;
 
}


//-----------------------------------------------------------------
main()
{
  long bp;
//  int a,j,i;
//  unsigned ii;
  //#ifdef CSETCHP  
//  jmp_buf tmp;
  CPL_init_malloc_protect(malloc);
  CPL_init_malloc(malloc);
  CPL_init_realloc_protect(realloc);
  CPL_init_realloc(realloc);

  choice_init(&bp);


//    choice_init(get_sp(0));
//  setjmp(tmp);
  
  //#else
  //  backTrackInit();
  //#endif
  /*
  if(setChoicePoint())
    {
      printf("No more ChoicePoint\n");
      backStatistics();
      exit(0);
    }
    */
  // Ex.1 queens(8);fail();
  // Ex.2 ff(11,a);

  // Ex.3 for(i=0;i<10;i++) { testchp(2000); }
  // Ex.4 testfail(100);
  // Ex.5 

    queens(10);
    //eelco_f();
  
    //Ndamy();
    //Ndamy2();
  //tendigit(12);
  /*
  for(i=11 ; i<=21 ; i++) {
    j=i;
    if(win(&j)) printf("%d\n",j);
  }
  */
  //testscp();
  fail();
  exit(1);
}


