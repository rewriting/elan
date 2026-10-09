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

  if(t == NULL) {
    printf("srealloc size = %d\n",size);
    fatal("srealloc(): out of memory", "");
  }
  return((void *) t);
}

void sfree(p)
void *p;
{
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


TERM *make_term(id, args,type,len,p)
int id;
TERM_LIST *args;
int len;                                   /* lenght of args */
SYM_TYPE type;
TERM_LIST *p;                              /*  tail of args  */
{
  TERM *t = EMALLOC(TERM);

  t->sym = id;
  if(args == NULL) {
    if(type == VAR) {
      t->rest.v.var_nr = id;               /*  to code variables 1..N, symbols N.. */
      t->type = VARIABLE;
    } else {
        t->type = CONSTANT;
    }
  } else {
    t->rest.f.list_len = len;
    t->rest.f.arg_list = args;
    t->rest.f.arg_tail = p;
    if(type == FUNC) t->type = FUNCTION;
    else             t->type = AC_NORMAL;
  }
  return(t);
}

TERM_LIST *make_term_list(new, rest)
TERM *new;
TERM_LIST *rest;
{
  TERM_LIST *l = EMALLOC(TERM_LIST);

  l->arg = new;
  l->next_arg = rest;
  return(l);
}
