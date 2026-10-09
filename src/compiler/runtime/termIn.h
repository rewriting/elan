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
#ifndef _termin_h
#define _termin_h

#include "termCommon.h"

/*
 * Some constants
 */
#define IDENTCODE 0
#define TYPECODE  1
#define CHARCODE  2
#define NUMCODE   3
#define BLANKCODE 4
#define STRINGLENGTH 1000

/* sizes of arrays */

#define TABOFGRAM_SIZE  50000
#define TABOFARITY_SIZE 50000
#define TABOFIDENT_SIZE 3000
#define TABOFSORT_SIZE  500


#define RGLOP              01   /* global rule */
#define RPRIORITYMSK       007777
#define NUMRULE            69
#define IDENTRULE          70
#define DEFAULTRULE        0

extern char buf[STRINGLENGTH];
extern int  currentSort ;
extern int  arity[TABOFARITY_SIZE] ; /* upper bound ??? */
extern char *tabIdent[TABOFIDENT_SIZE] ;
extern char *tabSort[TABOFSORT_SIZE] ;


extern Gterm *termParser(int queryMode, int evaluationMode);
extern Gterm *EarleyParser(char *querySortName);
extern void EarleyParserInit();
extern int findTab(char *s[],int n,char *str);
extern void addstandards(void);

extern void esemactinit();
extern void tabofidentInit(char *tab[],int max);
extern void typetInit(char *tab[],int max);
extern void grammarInit(int gram[],int max);
extern void earleyInit();
extern int earleyCall(FILE *fp,int sort);
extern void prefixParser(char *s1, char *s2);

extern char *tabIdentStr[];
extern int tabIdentIndex[];
extern int tabIdentSize;

extern char *tabSortStr[];
extern int tabSortIndex[];
extern int tabSortSize;

extern char *tabStrategyStr[];
extern int tabStrategyIndex[];
extern int tabStrategySize;

extern int earleyQuerySort ; 
extern int earleyQueryStrategy ; 
extern int coqMode;
extern int printMode;
extern int strCall;
extern int testAC;  //[NGUYEN: May 20 01] 
extern FILE *file_query; //NGUYEN
extern Gterm *term_read(int i, Gterm *t);
#endif

