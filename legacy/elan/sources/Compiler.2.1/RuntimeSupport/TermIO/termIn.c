#include <stdlib.h>

#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "back.h"
#include "builtin.h"
#include "streval.h"
#include "termIn.h"

/* #include"runtimeInit.h" */



#define IDENTCODE 0
#define TYPECODE 1
#define CHARCODE 2
#define NUMCODE 3
#define BLANKCODE 4

#define STRINGLENGTH 1000



/* sizes of arrays */

#define TABOFGRAM_SIZE 50000
#define TABOFARITY_SIZE 50000
#define TABOFIDENT_SIZE 3000
#define TABOFSORT_SIZE 500





/************************* les define ELAN *************************/

#define RGLOP 01   /* global rule */
#define RPRIORITYMSK 007777


#define NUMRULE            69
#define IDENTRULE          70
#define DEFAULTRULE        0

#ifdef STRINGS
#define STRINGRULE         82
#endif


/********************************************************************/


char buf[STRINGLENGTH] ;
int BGr=0 ;

int currentSort ;


extern int gram[] ;
extern int pos ; /* current pos in gram[] */

int arity[TABOFARITY_SIZE] ; /* upper bound ??? */


char *tabIdent[TABOFIDENT_SIZE] ;
char *tabSort[TABOFSORT_SIZE] ;

void initTab(char *s[],int n)
{
int i;

for(i=0;i<n;i++) s[i] = "" ;
}



void initTabIdent()
{
int i;

for(i=0;i<tabIdentSize ;i++) 
    {
   
   tabIdent[tabIdentIndex[i]] = tabIdentStr[i] ;
//   printf("tabIdent[%d] = %s\n",tabIdentIndex[i],tabIdent[tabIdentIndex[i]]) ;
    } 

}


void initTabSort()
{
int i;

for(i=0;i<tabSortSize ;i++) 
    {

	tabSort[tabSortIndex[i]] = tabSortStr[i] ;
//	printf("tabSort[%d] = %s\n",tabSortIndex[i],tabSort[tabSortIndex[i]]) ;
    }
}


int computeArity(int pos) 
{
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


void initArity(int pos)
{

int i=0 ;

while (i<pos) i = computeArity(i) ;
}




extern int earleyRes[] ;

extern int earleyKind[] ;

extern int earleyPos ;

/* earleyRes, earleyKind and earleyPos are initialized by esemact() */

#ifdef STRINGS
extern char *earleyString[] ;
#endif


// STRINGS: TO DO in computeTerm !!!

struct term *computeTerm(int *ppos, int *pcolor)
{

int code, kind,i ;

listTerm *list,*l;
struct term *t;



  (*ppos)-- ;

  code = earleyRes[*ppos] ;

  kind = earleyKind[*ppos] ;



  switch(kind) {

  case DEFAULTRULE: 

      if (arity[code] == 0) { 
        TERM_CONST_ALLOC(t,code); 
      }
      else { 

       TERM_ARITY_ALLOC(t,arity[code],code);

       list = listTermCreate(computeTerm(ppos,pcolor));
	

       for(i=arity[code]-1 ;i>=1;i--) {
	   list = addTermListTerm(list,computeTerm(ppos,pcolor));
       }

       list = addTermListTerm(list,NULL);


       for(l=list, i=0 ; l!=NULL ; l=l->next, i++) {

	   if(l->term != NULL) {

                  if(symb_isAC(code)) {
                    (*pcolor)++;

                    printf("termIn: add ");
                    term_print(stdout,l->term);
                    printf("\t color = %d\n",*pcolor);

                    
                    term_add_onf_term_color(t,l->term,*pcolor);
		      //term_add_onf_term(t,l->term);
                  } else {
				 
		      t->sub[i]=l->term; }
	   } 
       }
      }
    break ;

  case NUMRULE:

    t = setIntegerTag(code) ;
    break ;

  }

  return(t) ;

}

void initTabRef()

{
initArity(pos) ;

initTab(tabIdent,TABOFIDENT_SIZE) ;
initTab(tabSort,TABOFSORT_SIZE) ;

initTabIdent() ;
initTabSort() ;
}

int findTab(char *s[],int n,char *str)
{

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



struct term *EarleyParser(char *querySortName) {
  int e ;
  struct term *query;
  int color =0;
  
  tabofidentInit(tabIdent,TABOFIDENT_SIZE);
  typetInit(tabSort,TABOFSORT_SIZE);
  addstandards() ;
  grammarInit(gram,pos);
  earleyInit() ;

  if (*querySortName != 0) {
    earleyQuerySort = findTab(tabSort,TABOFSORT_SIZE,querySortName) ;
  }
  e = earleyCall(earleyQuerySort) ;
  if (e) {
    query = computeTerm(&earleyPos, &color) ;
    printf("\n") ;
    fflush(stdout) ;
  }
  else exit(1) ;
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
  fprintf(stderr,"Enter a query of the form: ([[<strat>:]<sort>]) <term> end\n");
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
        printf("':' expected\n",c);
        exit(1);
      }
      if(scanf(" %[^ )] %c",s2,&c) != 2) {
        printf("identifier expected\n");
      }
        /* ( id : id ) */
    }
  }
}

typedef struct term* (*funTabType)();
extern funTabType strTab[];

extern struct term *main_query();
extern struct term *query;
extern int earleyQueryStrategy;
extern char *sortName;
extern char *strategyName;

struct term *termParser(int queryMode, int evaluationMode) {
  struct term *res;
  
  switch(queryMode) {
      case 0: /* noInput */ 
        res=main_query(); 
        break;
      case 1: /* REFInput */ 
          //printf("query = "); term_println(stdout,query);
        res=normalise(query);
      
        if(earleyQueryStrategy!=0) {
          res = strTab[earleyQueryStrategy](res);
        }
        break;
      case 2: /* Elan Form */
        switch(evaluationMode) {
            case 0: /* sort and strategy from lgi file */
              printf("Enter a query term of sort '%s'\n", tabSort[earleyQuerySort]);

              res=EarleyParser("");
              res=normalise(res);
              if(earleyQueryStrategy!=0) {
                res = strTab[earleyQueryStrategy](res);
              }
              break;
            case 1: /* sort from shell line and no strategy */
              printf("Enter a query term of sort '%s'\n",sortName);
              res=normalise(EarleyParser(sortName));
              break;
            case 2: { /* strategy:sort from shell line */
              char c, sort[100], strat[100];
              int i,strIndex;
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
                printf("Strategy '%s' not found\n",strat);
                exit(1);
              }
              printf("Enter a query term of sort '%s'\n",sort);
              res=normalise(EarleyParser(sort));
              res=strTab[strIndex](res);
              break;
            }
        }
        break;
      case 3: { /* Command line */
        char s1[100], s2[100], *sort, *strat;
        int i,strIndex;
        prefixParser(s1,s2);
        sort=(strlen(s2)==0)?s1:s2;
        strat=(strlen(s2)!=0)?s1:s2;
        res = normalise(EarleyParser(sort));
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
            exit(1);
          }
          res = strTab[strIndex](res);
        } else if (strlen(sort)==0 && earleyQueryStrategy!=0) {
          res = strTab[earleyQueryStrategy](res);
        }
        break;
      }
  }
  return res;
}
