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
#include <stdio.h>
#include <string.h>

#include "tools.h"
#include "termCommon.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "builtin.h"
#include "streval.h"

#include "termOut.h"

#define TERMSIZE 1000000
//#define TABOFGRAM_SIZE 50000

#define IDENTCODE 0
#define TYPECODE 1
#define CHARCODE 2
#define NUMCODE 3



extern int arity[] ;
extern int gram[] ;
extern char *tabIdent[] ;
extern int globalGramSize;

int findTextForm(int code) {
  int cur   = 0;
  int found = -1;


  //printf("globalGramSize = %d\n",globalGramSize);
  while(cur<globalGramSize-5) {
      //printf("code = %d\tgram[%d+2] = %d\n",code,cur,gram[cur+2]);

      if(gram[cur+2] == code) {
	  found = cur;
      }
      cur+= (5 + (2*gram[cur+4]));
  }
 
  if( found==-1 ) {
      fprintf(stderr,"code  %d not found in grammar: cur = %d\n",code,cur);
      cur   = -1;
      found = -1;
      assert(0);
      //exit(1);
  }

  //printf("code = %d\tfound = %d\n",code,found);
  

  return found;
}


int alnumF = 0 ;

int LetterOrDigit(int c) {
  return 
    (('a' <= c) && (c <= 'z')) ||
    (('A' <= c) && (c <= 'Z')) ||
    (('0' <= c) && (c <= '9')) ;
}


void printTextFormTerm(FILE *fich, int pos, Gterm *t) {
  int i,j=0;
  char *s,c ;

  for(i=0;i<gram[pos+4];i++) {
      switch(gram[pos+5+i*2]) {
      case TYPECODE:
	  //printf("\nin termOut.c line 65 j = %d t =",j);term_print(stdout,t);printf("\n");
          termOut(fich,GgetArgument(t,j));
	  //printf("\n");
          j++;
          break;
      case IDENTCODE:
          s = tabIdent[gram[pos+5+i*2+1]];
          if(alnumF && LetterOrDigit(s[0])) {
	      fprintf(fich," ");
          }
          fprintf(fich,"%s",s);
          alnumF = LetterOrDigit(s[strlen(s)-1]);
          break;
      case CHARCODE:
          c = gram[pos+5+i*2+1];
          if(alnumF && LetterOrDigit(c)) {
	      fprintf(fich," ");
          }
          fprintf(fich,"%c",c);
          alnumF = LetterOrDigit(c);
          break;
      case NUMCODE:
          if(alnumF) {
	      fprintf(fich," ");
          }
          fprintf(fich,"%d",gram[pos+5+i*2+1]);
          alnumF = 1 ;
          break ;
      default:   //[HUY: Sep 11 01] 
	  fprintf(stderr,"Code type not found\n");
	  exit(1);
      }
  }
}


void termOut(FILE *fich,Gterm *t) {

  if(GisIntegerTagged(t)) {
    fprintf(fich,"%d",(int)GgetInt(t));   /* low 32 bits, as in 2004 and the interpreter */
    //printf("termOut GisIntegerTagged\n");
    return;
  } else if(GisIdentifierTagged(t)) {
    fprintf(fich,"IDENT(%lu)",GgetIdentifier(t));
    // A REVOIR !!!
    //printf("termOut GisIdentifierTagged\n");
    return;
  } else if(GisStringTagged(t)) {  // [hassen: Jun 28 01]
      //printf("termOut GisIntegerTagged\n");
    term_print(fich,t);
    return;
  } else if(GisArrayTagged(t)) {
    term_print(fich,t);
    return;
  }
  
  //printf("termOut: getSymb = %d\n",GgetSymb(t));
  //printf("t = "); term_println(stdout,t);


  printTextFormTerm(fich,findTextForm(GgetSymb(t)),t) ;
  
}

Gterm *term_write(int pid, Gterm *t) {
    //printf("tabfile->files[%d] = %d\n",i,tabfile->files[i]);
    //term_println(stdout,t);
  internal_term_print(tabfile->files[pid],t,ELAN_IO);
  return t ;
}
