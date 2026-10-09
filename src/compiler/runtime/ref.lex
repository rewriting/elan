/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
%{
int no_line=1;
char instr[1000];
char *remove_backslash(char *source);
%}

/* yyunput and input are not used */
%option nounput noinput

%%
[ \t]+			{ strcat(instr," "); }
\n			{ instr[0]='\0' ; no_line++; }
\%.*$			{ /* do nothing */ }


end			{ /*printf("c_end\n");*/
                          strcat(instr,yytext) ; return c_end; }
nil			{
                          strcat(instr,yytext) ; return c_nil; }
VAR			{ /*printf("c_VAR\n");*/
			  strcat(instr,yytext) ; return c_VAR; }
INT			{ /*printf("c_INT\n");*/
			  strcat(instr,yytext) ; return c_INT; }
IDENT			{ /*printf("c_IDENT\n");*/
			  strcat(instr,yytext) ; return c_IDENT; }
STRING  		{ /*printf("c_STRING\n");*/
			  strcat(instr,yytext) ; return c_STRING; }
FSYM			{ /*printf("c_FSYM\n");*/
			  strcat(instr,yytext) ; return c_FSYM; }

[0-9]+		{
			yylval.integer = atoi(yytext);			
			strcat(instr,yytext);
		        /*printf("c_integer=%d\n",yylval.integer);*/
			return c_integer;
		}




\"([^"]|\\\")*\" {
			strcat(instr,yytext);
                        yylval.string=remove_backslash((char*)yytext);
                        /*strdup(yylval.string,yytext);*/
		        /*printf("c_string=%s\n",yylval.string);*/
			return c_string;
     		}

\-			{ strcat(instr,yytext) ; return c_minus; }
\(			{ strcat(instr,yytext) ; return c_lbrace; }
\)			{ strcat(instr,yytext) ; return c_rbrace; }
\,			{ strcat(instr,yytext) ; return c_comma; }
\.			{ strcat(instr,yytext) ; return c_dot; }

\_			{ strcat(instr,yytext) ; return c_underscore; }
\{			{ strcat(instr,yytext) ; return c_laccol; }
\}			{ strcat(instr,yytext) ; return c_raccol; }

[a-zA-Z_][a-zA-Z0-9_\-]* { 
			strcat(instr,yytext);
			/*strdup(yylval.string,yytext);*/
			return c_idf;
			}

.			{
			strcat(instr,yytext);
			printf("lexical error : char '%c' unknown\n",yytext[0]);
			printf("Stop at line %d after '%s'\n",no_line,instr);
			exit(1);
			}
%%


char *remove_backslash(char *source) {
  int i,j;
  char *dest=(char*)AMALLOC(1+strlen(source));
  for(i=1,j=0 ; (size_t)i<strlen(source)-1 ; i++)
    {
      if(source[i]=='\\' && source[i+1]=='"')
	i++;
      dest[j++]=source[i];
    }
  return dest;
}



