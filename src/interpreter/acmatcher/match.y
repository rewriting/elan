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
%{
#include <stdlib.h>
#include <ctype.h>

#include "matcher/sym_types.h"
#include "externs.h"
#include "matcher/defs.h"
#include "matcher/term_types.h"
#include "matcher/functions.h"

TERM *make_term(int id, TERM_LIST *args);
TERM_LIST *make_term_list(TERM *new_t, TERM_LIST *rest);

void yyerror(char *s);
int yylex(void);

void matchinit(TERM *p, TERM* s);
void nextsolution(void);
void allsolution(void);
void abortthisproblem(void);
%}

%union {
  int y_int;
  TERM *y_term;
  TERM_LIST *y_term_list;
}

%type <y_term> term
%type <y_term_list> term_list
%token <y_int> IDENTIFIER
%nonassoc '='
%start top

%%

top		:	equation instructions
		|	top equation instructions
		;

equation	:	term '=' term
			{
			  matchinit($1, $3);
			}
		;

term		:	IDENTIFIER
			{ $$ = make_term($1, (TERM_LIST *) NULL); }
		|	IDENTIFIER '(' term_list ')'
			{ $$ = make_term($1, $3); } 
		;

term_list	:	term ',' term_list
			{ $$ = make_term_list($1, $3); }
		|	term
			{ $$ = make_term_list($1, (TERM_LIST *) NULL); }
		;

instructions    :       instruction
                |       instructions instruction
                ;
instruction     :       '.'
                        { nextsolution(); }
                |       ':'
                        { allsolution(); }
                |       ';'
                        { abortthisproblem(); }
                ;
%%

void yyerror(s)
char *s;
{
  fatal("Yacc problem: %s", s);
}

TERM *make_term(int id, TERM_LIST *args)
{
  TERM *t = MALLOC(TERM);
  TERM_LIST *p;
  SYM_TYPE type;
  int len;
  static int vmark = 0;

  t->sym = id;
  if(args == NULL){
    type = isupper(sym_tab[id].name[0]) ? VAR : CONSTAN;
    if(type == VAR){
      if(sym_tab[id].vmark == 0){
        sym_tab[id].vmark = ++vmark;
        t->rest.v.var_nr = vmark;
        translate[vmark] = id;
      } else{
        t->rest.v.var_nr = sym_tab[id].vmark;
      }
      printf("var_nr = %d\n",t->rest.v.var_nr);

      t->type = VARIABLE;
    } else{
      t->type = CONSTANT;
    }
  }
  else{
    type = isupper(sym_tab[id].name[0]) ? ACFUNC : FUNC;
    for(p = args, len = 1; p->next_arg; p = p->next_arg)
      len++;
    t->rest.f.list_len = len;
    t->rest.f.arg_list = args;
    t->rest.f.arg_tail = p;
    if(type == FUNC){
      if(sym_tab[id].type == FUNC){
        if(sym_tab[id].arity != len){
          fatal("Free function `%s' has variable number of arguments",
            sym_tab[id].name);
        }
      }
      else
        sym_tab[id].arity = len;
      t->type = FUNCTION;
    }
    else
      t->type = AC_NORMAL;
  }
  if(sym_tab[id].type != UNDEF && sym_tab[id].type != type)
    fatal("Inconsistant use of identifier `%s'", sym_tab[id].name);
  sym_tab[id].type = type;
  return(t);
}

TERM_LIST *make_term_list(TERM *new_t, TERM_LIST *rest)
{
  TERM_LIST *l = MALLOC(TERM_LIST);

  l->arg = new_t;
  l->next_arg = rest;
  return(l);
}

#ifndef HAVE_YYWRAP
int yywrap(void)
{
  return 1;
}
#endif
