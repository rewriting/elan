/*
		(c) 	Marian Vittek, 1994
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/

/* this file provides a g++ interface for ACmatching written in C */


/*
#include "externs.h"
*/
#include "sym_types.h"
#include "defs.h"
#include "term_types.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

void *salloc(n)
unsigned n;
{
  char *t = (char*) malloc(n);
  //printf("salloc size = %d\n",n);

  printf("salloc should not be used\n");
  exit(1);

  if(t == NULL) {
    printf("salloc size = %d\n",n);
    fatal("salloc(): out of memory", "");
  }
  return((void *) t);
}

void *srealloc(old, size)
void *old;
unsigned size;
{
  char *t = realloc((char *) old, size);

  printf("srealloc should not be used\n");
  exit(1);

  if(t == NULL) {
    printf("srealloc size = %d\n",size);
    fatal("srealloc(): out of memory", "");
  }
  return((void *) t);
}

void sfree(p)
void *p;
{
  printf("sfree should not be used\n");
  exit(1);

  free((char *) p);
}


void fatal(s,a)
char *s, *a;
{
  fprintf(stderr,"[error] ACmatch error ");
  if(*a)
    (void) fprintf(stderr, s, a);
  else
    (void) fprintf(stderr, s);
  (void) fprintf(stderr,"\n");
  exit(1);
}

