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

#include <stdlib.h>

#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "termCommon.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "builtin.h"
#include "streval.h"
#include "termIn.h"
#include "trace.h"

extern int gram[] ;
extern int globalGramSize;
int BGr=0 ;

void initTab(char *s[],int n) {
  int i;
  for(i=0;i<n;i++) {
    s[i] = "" ;
  }
}

void initTabIdent() {
  int i;
  for(i=0;i<tabIdentSize ;i++) {
    tabIdent[tabIdentIndex[i]] = tabIdentStr[i] ;
      // printf("tabIdent[%d] = %s\n",tabIdentIndex[i],tabIdent[tabIdentIndex[i]]) ;
  } 
}

void initTabSort() {
  int i;
  for(i=0;i<tabSortSize ;i++) {
    tabSort[tabSortIndex[i]] = tabSortStr[i] ;
      //	printf("tabSort[%d] = %s\n",tabSortIndex[i],tabSort[tabSortIndex[i]]) ;
  }
}

int computeArity(int pos) {
    int srulenumber,ar,posRHS,i ;
    srulenumber = gram[pos+2] ;
    posRHS = pos+5 ;
    ar = 0 ; /* arity */
    for (i=0;i<gram[pos+4];i++) {
	if (gram[pos+5+i*2] == TYPECODE) ar++ ;
	posRHS+=2 ;
    }
    // printf("arity[%d] = %d\n",srulenumber,ar) ;
    arity[srulenumber] = ar ;
    return(posRHS) ;
}


void initArity() {
    int i=0 ;
    while (i<globalGramSize-5) {
	i = computeArity(i);
    }
}


/*
 * earleyRes, earleyKind and earleyPos are initialized in runtimeInit.cc
 */
extern int earleyRes[];
extern int earleyKind[];
extern int earleyPos;
extern char *earleyString[] ;


// STRINGS: TO DO in computeTerm !!!

Gterm *computeTerm(int *ppos) {
  char *string;
  int code, kind,i ;
  GtermList *list, *l;
  Gterm *t;
  Gterm *tmp;
  (*ppos)-- ;
  code = earleyRes[*ppos] ;
  kind = earleyKind[*ppos] ;
  string = earleyString[*ppos] ;
  switch(kind) {
      case DEFAULTRULE: 
/*
        printf("code=%d: %20s    (AC=%d, arity=%d)\tppos=%d\n",
               code,
               fsymtab[code].name,
               symb_isAC(code),
               arity[code],*ppos
               );
*/  
        if(arity[code] == 0) {
          if(code == 0) {
            return con_0;
          } else if(code == 1) {
            return con_1;
          } else {
            GmakeAppl0(t,code);
          }
        } else {
          if(symb_isAC(code)) {
            TERMAC_ALLOC(t,arity[code],code);
          } else {
            GmakeApplArity(t,arity[code],code);
            //TERM_ARITY_ALLOC(t,arity[code],code);
          }

            /* create a list of terms */
          tmp = computeTerm(ppos);
          list = (GtermList*)GlistTermCreate(tmp);
          for(i=arity[code]-1 ;i>=1;i--) {
            tmp = computeTerm(ppos);
            list = GaddTermListTerm(list,tmp);
          }
            //ATprintf("*** list = %l\n",list);
          for(l=list, i=0 ; !GlistIsEmpty(l) ; l=GlistGetTail(l), i++) {
            if(symb_isAC(code)) {
              t=(Gterm*)term_add_onf_term((struct termac*)t,code,GlistGetHead(l));
            } else {
                //ATprintf("l->term = %t\n",GlistGetHead(l));
              GsetArgument(t,i,GlistGetHead(l));
            }
          }
        }
        break ;
      case NUMRULE:
        t = GsetIntegerTag(code) ;
        break;
      case IDENTRULE:
        t = GsetIdentifierTag(code) ;
        break;
      case STRINGRULE:
        t = (Gterm*) GsetStringTag(string);
          //printf("computeTerm: STRINGRULE not yet implemented\n");
        break;
      default:
        printf("computeTerm: case not matched\n");
        exit(1);
  }
    //ATprintf("*** term = %t\n",t);
     //printf("*** term = "); term_println(stdout,t);
  return(t);
}

void initTabRef()

{
initArity() ;

initTab(tabIdent,TABOFIDENT_SIZE) ;
initTab(tabSort,TABOFSORT_SIZE) ;

initTabIdent() ;
initTabSort() ;
}

int findTab(char *s[],int n,char *str) {
  int i=0 ;
  while ((i<n) && (strcmp(s[i],str)!=0)) {
      // fprintf(stderr,"compare sorts %s %s\n",s[i],str);
    i++ ;
  }
  if (i==n) { 
    fprintf(stderr,"\nUnknown sort\n") ;
    i = -1 ;
  }
return i ;
}

void EarleyParserInit() {
  tabofidentInit(tabIdent,TABOFIDENT_SIZE);
  typetInit(tabSort,TABOFSORT_SIZE);
  addstandards() ;
  grammarInit(gram,globalGramSize);
}

Gterm *EarleyParser(char *querySortName) {
  int e ;
  Gterm *query;

  earleyInit() ;
  if (*querySortName != 0) {
    earleyQuerySort = findTab(tabSort,TABOFSORT_SIZE,querySortName);
    if(earleyQuerySort == -1) {
      exit(1);
    }
  }
    /*
     * [pem: Oct 20 00] C'est quoi ce truc ?
     * e = earleyCall(NULL,earleyQuerySort) ; // [Huy: Oct 17 00] 
     */
  e = earleyCall(NULL,earleyQuerySort) ; 
  if(e) {
    query = computeTerm(&earleyPos) ;
    printf("\n") ;
    fflush(stdout) ;
  } else {
    exit(1);
  }
  return(query) ;
}

/*
 * parser for the following grammar (3 cases)
 * ()
 * ( id )
 * ( id : id )
 */

void prefixParser(char *s1, char *s2) {
  char c;
  fprintf(stdout,"Enter a query of the form: ([[<strat>:]<sort>]) <term> end\n");
  fflush(stdout);
  s1[0]='\0';
  s2[0]='\0';
  if(scanf(" ( %c",&c) != 1) {
    printf("'(' expected\n");
    exit(1);
  }
  if(c==')') {
      /* () */;
  } else {
    ungetc(c,stdin);
    if(scanf(" %[^ :)] %c",s1,&c) != 2) {
      printf("identifier expected\n");
      exit(1);
    }
    
    if(c==')') {
        /* ( id ) */;
    } else {
      if(c!=':') {
        printf("':' expected\n");
        exit(1);
      }
      if(scanf(" %[^ )] %c",s2,&c) != 2) {
        printf("identifier expected\n");
      }
        /* ( id : id ) */
    }
  }
}

//typedef Gterm* (*funTabType)();
extern funTabType strTab[];

extern Gterm *main_query();
extern Gterm *query;
//extern int earleyQueryStrategy;
extern char *sortName;
extern char *strategyName;

Gterm *termParser(int queryMode, int evaluationMode) {
  Gterm *res;
  int i;
  FILE *fp_trace;

  //printf("\n*************** in termParser ****************\n");
  MAXPOS = 0; // [Huy: May  1 00] 
  for(i=0; i<sizeof(POS)/sizeof(int); i++) {
    position[i]=0;
  }
  if(coqMode) {
    Ginitialise_trace();
  }
  strCall = earleyQueryStrategy;

  //printf("queryMode = %d\n",queryMode);
  
  switch(queryMode) {
      case 0: /* noInput */

        res=main_query();
        break;
      case 1: /* REFInput */ 

        res=(Gterm*)normalise(query);
        if(earleyQueryStrategy!=0) {
          res = strTab[earleyQueryStrategy](res);
        }
        break;
      case 2: /* Elan Form */

        switch(evaluationMode) {
            case 0: /* sort and strategy from lgi file */

              fprintf(stdout,"Enter a query term of sort '%s'\n", tabSort[earleyQuerySort]);
	      fflush(stdout);
	      if(coqMode) { /* [Huy: Oct  6 00] Coq case */

                if((res = (Gterm*)GcoqEarleyParser("")) == NULL) {
		    //printf("\nintermIN.c ligne 326 return NULL");
                  return NULL;
                }
		/*--------------------- for trace AC only -------------------*/
		if (coqMode){ //[NGUYEN: May  8 01] to be removed
		    file_query = fopen(query_file,"w");
		}
		if (testAC){ 
		    if (file_query !=NULL){
			//fprintf(file_query,"Trace(");
			//termOut(file_query,term_unflatten(res));
			fprintf(file_query,"trace2pi(");
		    }
		        //fprintf(stderr,"Trace(");
			//termOut(stderr,term_unflatten(res));
			fprintf(stderr,"trace2pi(");
		}
		/*----------------------------------------------------------*/

                //printf("\nintermIN.c ligne 329 return not NULL");
                GsetTermNoReduced(res);
	      } else {
	
	        res = EarleyParser("");
	        //printf("\nin EarleyParser res");term_print(stdout,res);
              }
                //ATprintf("\n before nomalise res = %t\n",res);
              res=(Gterm*)normalise(res);
                //ATprintf("\n after nomalise res = %t\n",res);
              //res=normalise(EarleyParser(""));
	      
              if(earleyQueryStrategy!=0) {
                res = strTab[earleyQueryStrategy](res);
              }

	      /*--------------------- for trace AC only -------------------*/
	      if (testAC){
		  if (file_query !=NULL){
		      fprintf(file_query,")");
		  }
		  fprintf(stderr,")");    
	      }
	      /*---------------------------------------------------------*/  
              break;
            case 1: /* sort from shell line and no strategy */
              //printf("\n*************** case 2 case 1 ****************\n");
              fprintf(stdout,"Enter a query term of sort '%s'\n",sortName);
              fprintf(stdout,"\ncase 1 sort = %s\n",sortName);
	      fflush(stdout);
              res=(Gterm*)normalise(EarleyParser(sortName));
              break;
            case 2: { /* strategy:sort from shell line */
	      char c, sort[100], strat[100];
              int i,strIndex;
              //printf("\n*************** case 2 case 2 ****************\n");
              if(sscanf(strategyName," %[^ :] %c %s",strat,&c,sort) != 3) {
                printf("the '-strategy' option should be followed by:\n\t<strategy>:<sort>\n");
                exit(1);
              }
              for(i=0, strIndex=0 ; strIndex==0 && i<tabStrategySize ; i++) {
                if(!strncmp(strategyName,tabStrategyStr[i],strlen(strategyName))) {
                  strIndex = tabStrategyIndex[i];
                }
              }
              if(strIndex==0) {
                printf("Strategy '%s' or sort '%s' not found\n",strat,sort); 
                exit(1);
              }
              fprintf(stdout,"Enter a query term of sort '%s'\n",sort);
	      fflush(stdout);
              strCall = strIndex; // [Huy: May 22 00] 
              printf("\ncase 2 sort = %s\n ",sort);
              res=(Gterm*)normalise(EarleyParser(sort));
              res=strTab[strIndex](res);
              break;
            }
        }
        break;
      case 3: { /* Command line */
        char *sort, *strat;
        char s1[100], s2[100];
        int i,strIndex;

	if(coqMode) { /*  [Huy: Oct  6 00]  Coq case */
          if(GcoqprefixParser(s1,s2) != 0) {
            return NULL;
          }
          sort = tabSort[earleyQuerySort]; /* Sorte de .lgi */
          strat = (strlen(s2)!=0)?s1:s2;
	  if ((res = (Gterm*)GcoqEarleyParser(sort))==NULL) { // [Huy: Oct 22 00]
	      return NULL;
	  }
          res = (Gterm*)normalise(res);
	} else {   
	    prefixParser(s1,s2);
	    sort=(strlen(s2)==0)?s1:s2;
	    strat=(strlen(s2)!=0)?s1:s2;
	    res = (Gterm*)normalise(EarleyParser(sort));
        }
        if(strlen(strat) > 0) {
	    strcat(strat,":");
	    strcat(strat,sort);
	    for(i=0, strIndex=0 ; strIndex==0 && i<tabStrategySize ; i++) {
		if(!strncmp(strat,tabStrategyStr[i],strlen(strat))) {
		    strIndex = tabStrategyIndex[i];
		}
	    }
	    if(strIndex==0) {
		printf("Strategy '%s' not found\n",strat);
		if(coqMode) {
		    return NULL;
		} else {
		    exit(1);
		}
	    }
	    strCall = strIndex;                     // [Huy: May 22 00]
	    res = strTab[strIndex](res);
        } else if (strlen(sort)==0 && earleyQueryStrategy!=0) {
	    res = strTab[earleyQueryStrategy](res);
        }
        break;
      }
  }
  if(printMode) {
      if ((fp_trace = fopen(trace_file,"w")) != NULL){
	  Gtrace_pretty_print(head_tr,1,fp_trace);
	  fclose(fp_trace);
      }
  }
  return res;
}

Gterm *term_read(int fileNumber, Gterm *t) {
  int e ;
  char *sort = NULL;
  Gterm *input;
  int earleySort;
  
    //printf("coucou \n");
  
  sort = fsymtab[GgetSymb(t)].sort ;

    //printf("sort = '%s'\n",sort);
  
  earleyInit() ;
  if(sort == NULL) {
    printf("term_read error\n");
    exit(1);
  } else {
    earleySort = findTab(tabSort,TABOFSORT_SIZE,sort);
      //printf("earleySort = %d\n",earleySort);
    
    if(earleySort == -1) {
      printf("term_read error: sort not found\n");
      exit(1);
    }
  }
  
  e = earleyCall(tabfile->files[fileNumber],earleySort) ;
  
  if(e) {
    input = computeTerm(&earleyPos) ;
      //printf("input = "); term_println(stdout,input);
    printf("\n") ;
  } else {     
    printf("term_read error: earleyCall failed\n");
    exit(1);
  }
  
  return(input) ;
}

