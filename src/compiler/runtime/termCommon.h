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
#ifndef _term_common_h
#define _term_common_h

#include <assert.h>
#include <stdio.h>

//#include "Back.h"
#ifdef CSETCHP
#include "choice.h"
#endif

#define MAX_NBR_ARG 16
//temporaire


#define term_arity(t)    (fsymtab[GgetSymb(t)].arity)
#define code_arity(t)    (fsymtab[(t)].arity)

#include "termBase.h"



#ifdef __cplusplus
typedef Gterm* (*funTabType)(...);
#else
typedef Gterm* (*funTabType)();
#endif       
extern funTabType funTab[];
extern funTabType strTab[];
/* the strategies of strTab are Gterm *str(Gterm *) */
typedef Gterm* (*strTabFunType)(Gterm *);
extern char *tabIdent[];

/*
HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
HHHHHHHHHHHHHHHHHHHHHHHHHHH DUPLICATION HHHHHHHHHHHHHHHHHHHHHH
HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
*/
/* fsymtab is declared after struct Gfsym (termBase.h / termATerm.h) */

/************************************************************
 * Symbol Definition
 */
//temporaire
//typedef signed char multiplicityType;
typedef int multiplicityType;
#define term_semantic(t) (fsymtab[GgetSymb(t)].semantic)

#define term_modulo(t)   (fsymtab[GgetSymbAC(t)].modulo)
#define term_name(t)     (fsymtab[GgetSymb(t)].name)
#define term_defstrat(t) (fsymtab[GgetSymb(t)].defstrat)
#define term_semact(t)   (fsymtab[GgetSymb(t)].semact)
#define symb_arity(s)    (fsymtab[s].arity)
#define symb_modulo(s)   (fsymtab[s].modulo)
#define symb_isAC(s)     (symb_arity(s) == -1)
extern void Gfsym_init(int code, int a, char *n, char *sort,
		      int sem, int dstrat,
		      Gterm * (*semacttion)(Gterm *));


extern void term_print(FILE *fich,Gterm *t);
extern void internal_term_printnl(FILE *fich,Gterm *t, int mode);
extern void internal_term_print(FILE *fich,Gterm *t, int mode);
extern void internal_term_println(FILE *fich,Gterm *t, int mode);

#define NO_IO       0
#define REF_IO      1
#define ELAN_IO     2
#define INTERNAL_IO 3
#define term_println(f,t) internal_term_println(f,t,INTERNAL_IO)
#define term_printnl(f,t) internal_term_println(f,t,INTERNAL_IO)

extern void term_printREF(FILE *fich,Gterm *t);
extern void term_printREFln(FILE *fich,Gterm *t);
extern Gterm *term_build(int nbArg, int code, ...);

/************************************************************		
 * Symbol Definition
 */


/************************************************************		
 * AC
 ************************************************************/

//#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */

struct termac {
  unsigned short int symb;  // info | symb
  unsigned short int hcode; // hcode
  unsigned int sizeinfo; // max nb of subterms | effective nb of subterms 
  Gterm **subterm; // array of [color|mult] subterms
};


#define SIZEMASK  0xffff0000
#define ARITYMASK 0x0000ffff
#define getArity(t)         ((t)->sizeinfo  & ARITYMASK)
#define setArity(t,n)       ((t)->sizeinfo  = ((t)->sizeinfo & SIZEMASK) | (n))
#define getSize(t)          (((t)->sizeinfo & SIZEMASK) >> 16)
#define setSize(t,n)        ((t)->sizeinfo  = ((t)->sizeinfo & ARITYMASK) | ((n) << 16))

#define setSizeArity(t,s,a) ((t)->sizeinfo = ((s)<<16) | (a))


#define COLORMASK  0xffff0000
#define COLORSHIFT 16
#define MULTMASK   0x0000ffff

#define getInternMult(t)    ((multiplicityType)((t) & MULTMASK))
#define setInternMult(t,n)  ((t)  = ((t) & ~MULTMASK) | (n))
#define getInternColor(t)   (((t) & COLORMASK) >> COLORSHIFT)
#define setInternColor(t,n) ((t)  = ((t) & ~COLORMASK) | ((n) << COLORSHIFT))

#define getMult(t,i)        getInternMult((unsigned long)((t)->subterm[(i)<<1]))
#define setMult(t,i,m)      setInternMult(*(unsigned long *)&((t)->subterm[(i)<<1]),(unsigned int)(m))
#define getSubterm(t,i)     (t->subterm[((i)<<1)+1])
#define setSubterm(t,i,st)  ((t->subterm[((i)<<1)+1]) = (st))

#define setColorMult(t,i,c,m) ((*(unsigned long *)&((t)->subterm[(i)<<1])) = (((c)<<16) | (m)))

#define getColor(t,i)       getInternColor((unsigned long)((t)->subterm[(i)<<1]))
#define setColor(t,i,c)     setInternColor(*(unsigned long *)&((t)->subterm[(i)<<1]),(unsigned int)(c))

#define TERMAC_ALLOC(dest,size,funsym)\
         termac_alloc(((struct termac **)&(dest)),size,funsym)

extern void termac_alloc(struct termac **ptr_dest,
                         int size,
                         unsigned int funsym);

#define term_isAC(t) isAC(t)
// TO BE IMPLEMENTED
//(isAC(t))



#define termac_add_lastColor(tac,subterm,mult,color) {\
  register int arity = getArity(tac);\
  if(arity == getSize(tac)) termac_resize(tac,2*arity);\
  setColorMult(tac,arity,color,mult);\
  setSubterm(tac,arity,subterm);\
  setArity(tac,arity+1);}

//  setHcode(tac,INTERN_HFUNCTION(tac,subterm));


#define termac_add_last(t,st,m) termac_add_lastColor(t,st,m,0)

extern void termac_resize(struct termac *t,int size);
extern void termac_copyTopSymbol(struct termac *tac,struct termac *subterm);
extern void termac_copyTopSymbolExcept(struct termac *tac,int i,struct termac *subterm);

/************************************************************
 * Functions
 */
extern Gterm *term_replace(Gterm *t1,Gterm *t2,Gterm *t3);
extern Gterm *term_rec_replace(Gterm *t1,Gterm *t2,Gterm *t3);


 
/************************************************************
 * Color
 */

#define bicolor 0xff
#define MULTMASK   0x0000ffff
#define COLORMASK  0xffff0000

//#define getMult(c)          ((c)->mult & MULTMASK)
//#define setMult(c,m)        ((c)->mult = ((c)->mult & COLORMASK) | (m))
//#define getColor(c)         ((c)->mult >> 4)
//#define setColor(c,color)   ((c)->mult = ( ((c)->mult & MULTMASK) | ((color)<<4) ))
//#define getColor(c)          ((int)(c)->color)
//#define setColor(c,m)        ((c)->color=((int)m))
extern int isMonoColor(Gterm *t);
extern void setMonoColor(Gterm *t);

#define term_add_onf_term_color(t1,fsym,t2,c) intern_term_add_onf_term(1,(struct termac*)t1,fsym,t2,c)
#define term_add_onf_term(t1,fsym,t2) intern_term_add_onf_term(1,(struct termac*)t1,fsym,t2,bicolor)

#define term_add_list_term(t1,fsymt2) intern_term_add_onf_term(0,(struct termac*)t1,fsym,t2,0)
extern struct termac *intern_term_add_onf_term(int isAC,
			 		       struct termac *tac,
                                               unsigned int fsym,
					       Gterm *subterm,
                                               int color);

extern Gterm *specialApply(Gterm *res);
extern Gterm *term_metaApply(Gterm *t);
extern Gterm *term_rewriteStep(int strIndex,Gterm *t);

#define GspecialApply(t) specialApply(t)
/************************************************************
 * List of term for RefParser
 */
extern int yyparse();

extern GtermList *listTermCreate(Gterm *term);
extern GtermList *addTermListTerm(GtermList *list , Gterm *term);

extern Gterm *asfNull();
extern Gterm *asfCons(Gterm *t1,Gterm *t2);
extern Gterm *asfHead(Gterm *t);
extern Gterm *asfTail(Gterm *t);
extern Gterm *asfPrefix(Gterm *t);
extern Gterm *asfLast(Gterm *t);
extern Gterm *asfNotEmptyList(Gterm *t);
extern Gterm *asfIsSingleElement(Gterm *t);

/*
 * CC_GC
 */

struct termac *CC_GC_encode(struct termac *tac);
struct termac *CC_GC_decode(struct termac *tac);


GtermList *GlistTermCreate(Gterm *term);
GtermList *GaddTermListTerm(GtermList *list , Gterm *term);


/*
 * hashCode
 */

extern int hashTerm(Gterm *t);

/*
 * Array
 */
extern Gterm *term_newArray(int n, Gterm *t);
extern Gterm *term_getArray(Gterm *array, int n);
extern Gterm *term_setArray(Gterm *array, int n, Gterm *t);
extern int term_getLength(Gterm *t);

/*
 * String
 */
extern Gterm *term_newString(char *s);
extern char  *term_getString(Gterm *t);
extern char *build_string(int n);
extern char *ccat(char *s1, char *s2);
extern char *findIdent(unsigned long n);
extern int selectChar(char *string,int n);
extern char *substitute(char *string,int n1, int n2);
extern char *subString(char *string,int i,int l);

/*
 * I/O
 */

extern int open_file(char *file, char *mode);
extern int close_file(int pid);
extern int flush_file(int pid);
extern int Getc(int pid);
extern int Putc(int pid, int c);

#endif // end of file



















