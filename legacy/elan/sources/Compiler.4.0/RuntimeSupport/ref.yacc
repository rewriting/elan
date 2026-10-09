%{
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "termCommon.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "builtin.h"
#include "streval.h"

/*
#ifdef __cplusplus
typedef Gterm* (*funTabType)(...);
#else
typedef Gterm* (*funTabType)();
#endif

funTabType funTab[] = {};
funTabType strTab[] = {};
Gfsym fsymtab[];
*/

/*extern int yylex();
extern int no_line;
extern char *yytext;
extern char instr[];*/

// flex
//extern char *yytext;
// lex
//extern char yytext[];

extern char instr[];


GtermList *list;
Gterm *query;
int lookup(char *string);
%}


%union
{
        int integer;
	char *string;
        Gterm *term;
        GtermList *listTerm;
}

%token c_end
%token c_VAR c_INT c_IDENT c_STRING c_FSYM
%token <integer> c_integer
%token <string> c_string
%token c_minus c_lbrace c_rbrace c_comma c_dot
%token <term> c_nil

%token <string> c_idf
%token c_underscore c_laccol c_raccol

%type <term> START TERM
%type <integer> INTEGER
%type <listTerm> LIST_TERM
%type <string> ASFIDF

%start START

%%
START   :   TERM c_end
          {
	    query=$1;
	    /*
            printf("term = ");
            term_printnl(stdout,query);
            printf("end\n");
	    */
          }
          | TERM
          { 
	    query=$1;
          }	  

TERM    :   c_INT c_lbrace INTEGER c_rbrace
          {
	    //printf("GsetIntegerTag\n");
            $$=(Gterm*)GsetIntegerTag($3);
          }
          | c_STRING c_lbrace c_string c_rbrace
          {
            printf("Not yet implemented\n");
            exit(1);
          }
          | c_IDENT c_lbrace INTEGER c_rbrace
          {
	    //printf("setIdentifierTag\n");
            $$=GsetIdentifierTag($3);
          }
          | c_VAR c_lbrace INTEGER c_comma INTEGER c_rbrace
          {
            printf("The query must be a GROUND term\n");
            exit(1);
          }
          | c_FSYM c_lbrace LIST_TERM c_comma INTEGER c_rbrace
          {
            int arity=0;
            int i;
 	    for(list=$3 ; list!=NULL ; list=GgetNext(list)) {
              arity++;
            }  
	    arity--;
	    /*
            printf("code  = %d\t",$5);
            printf("arity = %d\n",arity);
	    */
/*
	    if(symb_isAC($5)) {
	      printf("AC symbol not yet implemented\n");
	      exit(1);
	    }
*/

            if(arity==0) {
              Gmake_const($$,$5);
            } else {
	      int isAC = symb_isAC($5);
              GmakeApplArity($$,arity,$5);
              for(list=$3, i=0 ; list!=NULL ; list=GgetNext(list), i++) {
                if(GgetFirst(list) != NULL) {
                  /*
		  printf("sub[%d] = ",i);
                  term_printnl(stdout,list->term);
                  */
                  if(isAC) {
                    term_add_onf_term($$,$5,GgetFirst(list));
                  } else {
                    GsetArgument($$,i,GgetFirst(list));
                  }
                  
                }
              }
            }  

	    /*
	    printf("term  = "); term_printnl(stdout,$$); 
	    $$=specialApply($$);
	    printf("after specialApply : "); term_printnl(stdout,$$); 	
	    */

          }
          | c_nil
          {
            $$=NULL;
          }

/*
 * Pour ASF+SDF
 */

          | INTEGER
          {
	    /*printf("integer = %d\n",$1);*/
            $$=(Gterm*)GsetIntegerTag($1);
          }
          | ASFIDF
          {
            int code = lookup($1);
            /*printf("code = %d\n",code);*/
            ($$,code);
            $$=(Gterm*)GspecialApply($$);
          }
          | ASFIDF c_lbrace LIST_TERM c_rbrace
          {
            int code = lookup($1);
            int arity=0;
            int i;


            if(code==-1) {
              /* printf("Symbol '%s' not found\n",$1);*/
	      exit(1);
            }

 	    for(list=$3 ; list!=NULL ; list=GgetNext(list)) {
              arity++;
            }  
/*
            printf("arity = %d\n",arity);
            printf("code  = %d\n",code);
*/
            GmakeApplArity ($$,arity,code);
            for(list=$3, i=0 ; list!=NULL ; list=GgetNext(list), i++) {
              if(GgetFirst(list) != NULL) {
                GsetArgument($$,i,GgetFirst(list));
              }
            }
            $$=(Gterm*)GspecialApply($$);
          }
          | error
          {
            printf("syntax error near : '%s'\n",instr);
            exit(1);
          }

INTEGER : c_integer
          {
            $$=$1;
	    //printf("INTEGER = %d\n",$$);
          }
          | c_minus c_integer
          {
            $$=-$2;
	    //printf("INTEGER = %d\n",$$);
          }

LIST_TERM : LIST_TERM c_dot TERM
          {
            (GtermList *)$$=GaddTermListTerm((GtermList *)$1,$3);
          }
          | LIST_TERM c_comma TERM
          {
            (GtermList *)$$=GaddTermListTerm((GtermList *)$1,$3);
          }
          | TERM
          {
            (GtermList *)$$=GlistTermCreate($1);
          }

ASFIDF    :
/*
 * Pour pouvour detecter les erreurs de syntaxe
           c_idf
          {
	    $$=$1;
          }
	  |
	  */
          c_string
	  {
	    $$=$1;
	  }
%%
#include "lex.yy.c"

int yyerror() {}

/*extern fsym fsymtab[];*/
extern int fsymtabSize;

int lookup(char *string) {
  int i;
  if(string[0]=='"') {
    string[strlen(string)-1]='\0';
    string++;
  }

  printf("lookup '%s'\n",string);

  printf("TO BE IMPLEMENTED (ref.yacc)\n");
  exit(1);
/*
  for(i=0 ; i<fsymtabSize ; i++) {
    if(strncmp(string,fsymtab[i].name,strlen(string))==0)
      return i;
      // printf("'%s'\n",fsymtab[i].name);

  }
*/
  printf("Symbol '%s' not found\n",string);	
  return -1;
}



