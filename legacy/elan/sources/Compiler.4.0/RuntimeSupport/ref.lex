%{
int no_line=1;
char instr[1000];
char *remove_backslash(char *source);
%}

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
  for(i=1,j=0 ; i<strlen(source)-1 ; i++)
    {
      if(source[i]=='\\' && source[i+1]=='"')
	i++;
      dest[j++]=source[i];
    }
  return dest;
}



