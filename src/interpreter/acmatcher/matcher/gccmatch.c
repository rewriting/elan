/*
		(c) 	Marian Vittek, 1994
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/

/* this file provides a g++ interface for ACmatching written in C */


#include "sym_types.h"
//#include "externs.h"
#include "defs.h"
#include "term_types.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

void *salloc(unsigned n)
{
  char *t = (char*) malloc(n);

  if(t == NULL)
    fatal("salloc(): out of memory", "");
  return((void *) t);
}

void *srealloc(void *old, unsigned size)
{
  char *t = realloc((char *) old, size);

  if(t == NULL)
    fatal("srealloc(): out of memory", "");
  return((void *) t);
}

void sfree(void *p)
{
  free((char *) p);
}


void fatal(char *s, char *a)
{
  fprintf(stderr,"[error] ACmatch error ");
  if(*a)
    (void) fprintf(stderr, s, a);
  else
    (void) fprintf(stderr, "%s", s);
  exit(1);
}


TERM *make_term(int id, TERM_LIST *args, SYM_TYPE type, int len, 
                TERM_LIST *p /* tail of args */)
{
  TERM *t = MALLOC(TERM);

  t->sym = id;
  if(args == NULL){
    if(type == VAR){
      t->rest.v.var_nr = id;               /*  to code variables 1..N, symbols N.. */
      t->type = VARIABLE;
    }
    else{
        t->type = CONSTANT;
    }
  }
  else{
    t->rest.f.list_len = len;
    t->rest.f.arg_list = args;
    t->rest.f.arg_tail = p;
    if(type == FUNC) t->type = FUNCTION;
    else             t->type = AC_NORMAL;
  }
  return(t);
}

TERM_LIST *make_term_list(TERM *new_t, TERM_LIST *rest)
{
  TERM_LIST *l = MALLOC(TERM_LIST);

  l->arg = new_t;
  l->next_arg = rest;
  return(l);
}
