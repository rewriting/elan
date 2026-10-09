/*
  
    ELAN

    Copyright (C) 1994-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "matcher/sym_types.h"
#include "externs.h"
#include "matcher/defs.h"
#include "matcher/term_types.h"
#include "matcher/functions.h"

#define NUMBER_OF_VARS	50

SYMBOL sym_tab[SYM_TAB_SIZE];
int lineno = 0; 
int translate[NUMBER_OF_VARS];
static TERM *assignment[NUMBER_OF_VARS];

int yyparse(void);

int main(int argc, char **argv)
{
  /* extern int yydebug; */
  /* extern FILE *yyin; */
  /* char *p = (char *) NULL; */
  int i;

  lineno = 1;
  printf("## \n"); fflush(stdout);
  if(yyparse() != 0)
    fatal("Unexpected end of input", "");

//	Free strings from symbol table for memory accounting purposes

  for(i = 0; i < SYM_TAB_SIZE; i++)
    if(sym_tab[i].name != NULL)
      sfree(sym_tab[i].name);
  return(0);
}


static void *match_state;
static int match_i;

void matchinit(TERM *p, TERM *s)
{
  match_state = build_match(p, s, NUMBER_OF_VARS);
  match_i = 0;
}

/* mutual calls, so we do need this prototype */
void print_term(TERM *t);

void print_tlist(TERM_LIST *l)
{
  while(l){
    print_term(l->arg);
    l = l->next_arg;
    if(l != NULL)
      printf(", ");
  }
}

void print_aclist(AC_LIST *l)
{
  while(l){
    printf("%d*", l->mult);
    print_term(l->arg);
    l = l->next_ac;
    if(l != NULL)
      printf(", ");
  }
}

void print_term(t)
TERM *t;
{
  if(t == NULL){
    printf("(null ptr)");
    return;
  }
  switch(t->type){
  case VARIABLE:
  case CONSTANT:
    printf("%s", sym_tab[t->sym].name);
    break;
  case FUNCTION:
  case AC_NORMAL:
    printf("%s(", sym_tab[t->sym].name);
    print_tlist(t->rest.f.arg_list);
    printf(")");
    break;
  case AC_COMPRESSED:
    printf("%s(", sym_tab[t->sym].name);
    print_aclist(t->rest.a.ac_list);
    printf(")");
    break;
  }
}

void nextsolution(void)
{ int j;
  if (extract_match(match_state, assignment)) {
      printf("%d\n**\n", ++ match_i);
      for(j = 0; j < NUMBER_OF_VARS; j++)
        if(assignment[j] != NULL){
          printf("%s  |-->  ", sym_tab[translate[j]].name);
          print_term(assignment[j]);
          printf("\n");
          destroy_term(assignment[j]);
          assignment[j] = NULL;
        }
      printf("##\n");
      fflush(stdout);
  } else {
    printf("END#\n");
    fflush(stdout);
  }
}

void allsolution(void) 
{
  while (extract_match(match_state, assignment))
    match_i++;
  
  printf("%d solutions\n**\n", match_i); 
  printf("END#\n");
  fflush(stdout);
  
}

void abortthisproblem(void)
{
  destroy_match(match_state);
}

void fatal(s,a)
char *s, *a;
{
  if(lineno)
    (void) fprintf(stderr, "Fatal error, line %d: ", lineno);
  else
    (void) fprintf(stderr, "Fatal error, command line: ");
  if(*a)
    (void) fprintf(stderr, s, a);
  else
    (void) fprintf(stderr, s);
  exit(1);
}

void *salloc(n)
unsigned n;
{
  char *t = (char*)malloc(n);

  if(t == NULL)
    fatal("salloc(): out of memory", "");
  return((void *) t);
}

void *srealloc(old, size)
void *old;
unsigned size;
{
  char *t = (char*)realloc(old,size);

  if(t == NULL)
    fatal("srealloc(): out of memory", "");
  return((void *) t);
}

void sfree(p)
void *p;
{
  free((char *) p);
}

