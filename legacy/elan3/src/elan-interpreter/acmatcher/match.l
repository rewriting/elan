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
#include <string.h>
#include "matcher/sym_types.h"
#include "externs.h"
#include "matcher/defs.h"
#include "matcher/term_types.h"
#include "matcher/functions.h"
#include "y.tab.h"
%}

%%

[()=,.;:]		return(*yytext);
[A-Za-z][A-Za-z0-9]*	{
			  yylval.y_int = lookup(yytext);
			  return(IDENTIFIER);
			}
[ \t]			;
[\n\r\f]		lineno++;
"/*"			{
			  int i = 0;
			  do {
			    switch(input()){
			    case '*':
			      i = 1;
			      break;
			    case '/':
			      i += i;
			      break;
			    case '\n':
			    case '\r':
			    case '\f':
			      lineno++;
			      /* fall through */
			    default:
			      i = 0;
			    }
			  }while(i < 2);
			}
.			fatal("Illegal character `%s'", yytext);

%%

int lookup(text)
char *text;
{
  int hash = 0, i;
  char *p;

  for(p = text; *p; p++)
    hash += *p;
  i = hash %= SYM_TAB_SIZE;
  while(sym_tab[i].name){
    if(!strcmp(sym_tab[i].name, text))
      return(i);
    if(++i == SYM_TAB_SIZE)
      i = 0;
    if(i == hash)
      fatal("symbol table full.", "");
  }
  sym_tab[i].name = STRSAVE(text);
  return(i);
}

