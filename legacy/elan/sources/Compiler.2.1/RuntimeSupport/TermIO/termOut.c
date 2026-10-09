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

#include "termOut.h"

#define TERMSIZE 1000000
#define TABOFGRAM_SIZE 50000

#define IDENTCODE 0
#define TYPECODE 1
#define CHARCODE 2
#define NUMCODE 3



extern int arity[] ;
extern int gram[] ;
extern char *tabIdent[] ;

int findTextForm(int code)
{
int cur  = 0 ;

while ((cur < TABOFGRAM_SIZE) && (gram[cur+2] != code)) 
    cur+= 5 + gram[cur+4] + gram[cur+4] ;

 if (cur >= TABOFGRAM_SIZE) { 
   fprintf(stderr,"code not found in grammar\n") ;
   cur = -1 ;
 }

return(cur) ;
}


int alnumF = 0 ;

int LetterOrDigit(c)
{

return 
(('a' <= c) && (c <= 'z')) ||
(('A' <= c) && (c <= 'Z')) ||
(('0' <= c) && (c <= '9')) ;

}


void printTextFormTerm(FILE *fich,int pos,struct term *t) 
{

 int i,j=0 ;
 

 char *s,c ;

 for (i=0;i<gram[pos+4];i++)
   switch(gram[pos+5+i*2]) {
   case TYPECODE: 
     termOut(fich,t->sub[j]) ;
     j++ ;
     break ;

   case IDENTCODE:

     s = tabIdent[gram[pos+5+i*2+1]] ;

     if (alnumF && LetterOrDigit(s[strlen(s)-1])) fprintf(fich," ") ;
     fprintf(fich,"%s",s) ;

     alnumF = LetterOrDigit(s[strlen(s)-1]) ;
     break ;
     
   case CHARCODE:

     c = gram[pos+5+i*2+1] ;

     if (alnumF && LetterOrDigit(c)) fprintf(fich," ") ;

     fprintf(fich,"%c",c) ;

     alnumF = LetterOrDigit(c) ;
     break;

   case NUMCODE:
    if (alnumF) fprintf(fich," ") ;

    fprintf(fich,"%d",gram[pos+5+i*2+1]) ;

    alnumF = 1 ;
    break ;
   } ;


}


void termOut(FILE *fich,struct term *t)
{
  int i;

  if(isIntegerTagged(t)) {
    fprintf(fich," %d",getInt(t));
    return;
  } else if(isIdentifierTagged(t)) {
    fprintf(fich,"IDENT(%d)",getIdentifier(t));
    // A REVOIR !!!
    return; 
  }

    /*
     * Avant d'etre affiche, le terme AC est transforme en terme syntaxique
     * il n'y a plus de traitement special
     */
  if(0 && term_isAC(t)) {
    fprintf(fich,"%s*(",term_name(t));
    if(term_first(t) != NULL) {
      struct cell_term *cell;
      for(cell=term_first(t) ; cell!=NULL ; cell=cell_next(cell)) {
        if(getMult(cell)>1) {
          int i;
          fprintf(fich,"[");
          for(i=0 ; i<getMult(cell) ; i++) {
            termOut(fich, cell_t(cell));
            if(i!=getMult(cell)-1) {
              fprintf(fich,",");
            }
          }
          fprintf(fich,"]");
        } else {
          termOut(fich, cell_t(cell));
	}
        if(cell != term_last(t)) {
          fprintf(fich,",");
        }
      }
    }
    
    fprintf(fich,")",term_name(t));
   }
   
   else {
     printTextFormTerm(fich,findTextForm(getSymb(t)),t) ;
  }
  
}
