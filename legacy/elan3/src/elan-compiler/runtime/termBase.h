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
#ifndef _term_base_h
#define _term_base_h

#include <stdarg.h>

#ifdef UPDATE_HCODE
#undef HASHCODE
#define HASHCODE
#endif

#ifdef COMPUTE_HCODE
#undef HASHCODE
#define HASHCODE
#endif

#ifndef HASHCODE

#define ACMASK     0x20000000
#define REDUCEMASK 0x40000000
#define SYMBMASK   0x00000fff

struct term {
  unsigned int symb;       // info | hcode | symb
  struct term *(sub[2]);   // array of subterms 
};

#define TERMSTR(strname,arity)\
struct strname {\
  unsigned int symb;\
  struct term *(sub[arity]);\
}
#define getHcode(t)
#define setHcode(t,h)
#else // HASHCODE

#define ACMASK     0x2000
#define REDUCEMASK 0x4000
#define SYMBMASK   0x0fff
#define HASHMASK   0x0000FFFF

struct term {
  unsigned short int symb;   // info | symb
  unsigned short int hcode;  // hcode
  struct term *(sub[2]);     // array of subterms 
};

#define TERMSTR(strname,arity)\
struct strname {\
  unsigned short int symb;\
  unsigned short int hcode;\
  struct term *(sub[arity]);\
}
#define getHcode(t)            ((unsigned int)t->hcode)
#define setHcode(t,h)          ((t)->hcode= (h&HASHMASK))
#endif // HASHCODE

TERMSTR(term1,1);
TERMSTR(term2,2);
TERMSTR(term0,1);
TERMSTR(term3,3);


typedef struct term Gterm;

typedef struct GtermList {
  Gterm *term;
  struct GtermList *next;
  struct GtermList *last;
} GtermList;

#include "tools.h"
#include "builtin.h"
//#include "Back.h"
#ifdef CSETCHP
#include "choice.h"
#endif

void intern_Gmake_const(Gterm **ptr_dest,int code);
void intern_GmakeAppl0(Gterm **ptr_dest,int code);
void intern_GmakeAppl1(Gterm **ptr_dest,int code,Gterm *subterm0);
void intern_GmakeAppl2(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1);
void intern_GmakeAppl3(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2);
void intern_GmakeAppl4(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2
                       ,Gterm *subterm3);
void intern_GmakeAppl5(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2
                       ,Gterm *subterm3,Gterm *subterm4);
void intern_GmakeAppl6(Gterm **ptr_dest,int code,Gterm *subterm0
                       ,Gterm *subterm1,Gterm *subterm2
                       ,Gterm *subterm3,Gterm *subterm4
                       ,Gterm *subterm5);
void GmakeAppl(Gterm **ptr_dest,int code,int arity,...);
void intern_GsetArgument(Gterm **ptr_dest,int pos, Gterm *t);
void intern_GmakeAppl_Arity(Gterm **ptr_dest,int arity,int code);
void intern_GmakeAppl_Array(Gterm **ptr_dest,int t,Gterm *ArrayArgs[]);
int intern_GgetSymb(Gterm *t);
void intern_GsetSymb(Gterm *t, int symb);


#define GgetSymbAC(t) GgetSymb((Gterm*)t)

GtermList *GlistTermCreate(Gterm *term);
GtermList *GaddTermListTerm(GtermList *list , Gterm *term);
Gterm *GlistGetHead(GtermList *list);
GtermList *GlistGetTail(GtermList *list);
int GlistIsEmpty(GtermList *list);
Gterm *intern_GgetArgument(Gterm *v,int p);

int Gterm_cmp(register Gterm *t1, register Gterm *t2);

#define GmakeApplArray(dest,t,ArrayArgs)        intern_GmakeAppl_Array(&(dest),t,ArrayArgs)
#define GgetArguments_tab(arg,res)              arg=res->sub
#define GgetFirst(list) (list)->term
#define GgetNext(list) (list)->next

#ifdef NOTMACRO
#define Gmake_const(dest,code)                  intern_Gmake_const(&(dest),code)
#define GmakeAppl0(dest,code)                   intern_GmakeAppl0(&(dest),code)
#define GmakeAppl1(dest,code,s0)                intern_GmakeAppl1(&(dest),code,s0)
#define GmakeAppl2(dest,code,s0,s1)             intern_GmakeAppl2(&(dest),code,s0,s1)
#define GmakeAppl3(dest,code,s0,s1,s2)          intern_GmakeAppl3(&(dest),code,s0,s1,s2)
#define GmakeAppl4(dest,code,s0,s1,s2,s3)       intern_GmakeAppl4(&(dest),code,s0,s1,s2,s3)
#define GmakeAppl5(dest,code,s0,s1,s2,s3,s4)    intern_GmakeAppl5(&(dest),code,s0,s1,s2,s3,s4)
#define GmakeAppl6(dest,code,s0,s1,s2,s3,s4,s5) intern_GmakeAppl6(&(dest),code,s0,s1,s2,s3,s4,s5)
#define GmakeApplArity(dest,arity,code)         intern_GmakeAppl_Arity(&(dest),arity,code)
#define GsetArgument(dest,pos,t)                intern_GsetArgument((&(dest)),pos,t)
#define GgetArgument(term,pos)                  intern_GgetArgument(term,pos)
#define GgetSymb(term)                          intern_GgetSymb(term)
#define GsetSymb(term,symb)                     intern_GsetSymb((Gterm*)term,symb)
#else
#define GgetSymb(t)                             getSymb(t)
#define GsetSymb(t,symb)                        setSymb(t,symb)
#define GsetArgument(dest,pos,t)                setFreeSubterm(dest,pos,t)
#define GgetArgument(t,pos)                     getFreeSubterm(t,pos)
#define GmakeApplArity(dest,arity,code)         TERM_ARITY_ALLOC(dest,arity,code)    
#define Gmake_const(dest,code)                  TERM_CONST_ALLOC(dest,code)
#define GmakeAppl0(dest,code)                   TERM_ALLOC(dest,term0,code)
#define GmakeAppl1(dest,code,s0)                { TERM_ALLOC(dest,term1,code);\
                                                  GsetArgument(dest,0,s0); }
#define GmakeAppl2(dest,code,s0,s1)             { TERM_ALLOC(dest,term2,code);\
                                                  GsetArgument(dest,0,s0);\
                                                  GsetArgument(dest,1,s1); }
#define GmakeAppl3(dest,code,s0,s1,s2)          { TERM_ARITY_ALLOC(dest,3,code);\
                                                  GsetArgument(dest,0,s0);\
                                                  GsetArgument(dest,1,s1);\
                                                  GsetArgument(dest,2,s2); }
#define GmakeAppl4(dest,code,s0,s1,s2,s3)       { TERM_ARITY_ALLOC(dest,4,code);\
                                                  GsetArgument(dest,0,s0);\
                                                  GsetArgument(dest,1,s1);\
                                                  GsetArgument(dest,2,s2);\
                                                  GsetArgument(dest,3,s3);\
                                                }
#define GmakeAppl5(dest,code,s0,s1,s2,s3,s4)    { TERM_ARITY_ALLOC(dest,5,code);\
                                                  GsetArgument(dest,0,s0);\
                                                  GsetArgument(dest,1,s1);\
                                                  GsetArgument(dest,2,s2);\
                                                  GsetArgument(dest,3,s3);\
                                                  GsetArgument(dest,4,s4);\
                                                }
#define GmakeAppl6(dest,code,s0,s1,s2,s3,s4,s5) { TERM_ARITY_ALLOC(dest,6,code);\
                                                  GsetArgument(dest,0,s0);\
                                                  GsetArgument(dest,1,s1);\
                                                  GsetArgument(dest,2,s2);\
                                                  GsetArgument(dest,3,s3);\
                                                  GsetArgument(dest,4,s4);\
                                                  GsetArgument(dest,5,s5);\
                                                 }
#endif

#define GsetSymbAC(dest,code) GsetSymb(dest,code)

typedef struct Gfsym {
  int arity;
  char *name;
  char *sort;
  int semantic;
  int modulo;
  int defstrat;                          // for Peter
  Gterm * (*semact)(Gterm *); // for Peter
  int prec; //[QUANG: Sep 19 01] precedence used in Gterm_cmp
} Gfsym;

/*
 * General macro for MASK
 * MASK 0x0000ffff
 * SHIFT 16
 */

#define getValue(t,  MASK,SHIFT) (((t) & MASK) >> SHIFT)
#define setValue(t,v,MASK,SHIFT) ((t) = ((t) & (~MASK)) | ((v)<<SHIFT))


#ifdef UPDATE_HCODE

#define setAC(t)   ((t)->symb |= ACMASK)
#define isAC(t)    ((t)->symb &  ACMASK) 

#define HNUMBER                65599
#define INTERN_HFUNCTION(t,st) ( getHcode(t) + getHcode(st) )
#define COMPUTE_HFUNCTION(t) 0
#define HFUNCTION(t,st)        (isTagged(st)?getHcode(t):(INTERN_HFUNCTION(t,st)% HNUMBER))

#define getSymb(t)             ((t)->symb & SYMBMASK)
#define setSymb(t,s)           { (t)->symb = (s); setHcode(t,s); }

#define getFreeSubterm(t,i)    ((t)->sub[i])
#define setFreeSubterm(t,i,st) { ((t)->sub[i]=(st)); setHcode(t,HFUNCTION(t,st)); }

#else
#ifdef COMPUTE_HCODE
              
#define setAC(t)   ((t)->symb |= ACMASK)
#define isAC(t)    ((t)->symb &  ACMASK) 

#define getSymb(t)             ((t)->symb & SYMBMASK)
#define setSymb(t,s)           { (t)->symb = (s); setHcode(t,HASHMASK); }

#define getFreeSubterm(t,i)    ((t)->sub[i])
#define setFreeSubterm(t,i,st) ((t)->sub[i]=(st))

#define INTERN_HFUNCTION(t,st) 0
#define COMPUTE_HFUNCTION(t) hashTerm(t)

#else 

#define setAC(t)   ((t)->symb |= ACMASK)
#define isAC(t)    ((t)->symb &  ACMASK) 

#define getSymb(t)             ((t)->symb & SYMBMASK)
#define setSymb(t,s)           ((t)->symb = (s))

#define getFreeSubterm(t,i)    ((t)->sub[i])
#define setFreeSubterm(t,i,st) ((t)->sub[i]=(st))

#define INTERN_HFUNCTION(t,st) 0
#define COMPUTE_HFUNCTION(t) 0

#endif // UPDATE_HCODE
#endif // COMPUTE_HCODE

#define GsetAC(t)   (setAC(t))


// used  in trace.c
#define GclearReduced(t)        ((t)->symb &= (~REDUCEMASK))
#define GsetReduced(t)          ((t)->symb |= REDUCEMASK)
#define GisReduced(t)           ((t)->symb &  REDUCEMASK)

extern void term_alloc(Gterm **ptr_dest,
		       int size_sname,
		       unsigned int funsym);
#define TERM_CONST_ALLOC(dest,funsym)\
        {(dest)=(Gterm*) malloc(sizeof(struct term1));\
	 GsetSymb(dest,funsym);\
	}
//  TERM_ALLOC(dest,term1,symb)

#ifdef DEBUG
#define TERM_ALLOC(dest,sname,funsym)\
         term_alloc(&dest,sizeof(struct sname),funsym)
#else
#define TERM_ALLOC(dest,sname,funsym)\
        {(dest)=(Gterm*) MALLOC(sizeof(struct sname));\
	 GsetSymb(dest,funsym);\
	}
#endif

#define TERM_ARITY_ALLOC(dest,arity,funsym)\
        {(dest)=(Gterm*) \
	   MALLOC(sizeof(struct term1)+(arity-1)*sizeof(Gterm*));\
	 GsetSymb(dest,funsym);\
	 if(symb_isAC(funsym)) {printf("TERM_ALLOC AC\n");}\
	}

#define TERM_FREE(packet) FREE((packet));
extern void term_copyTopSymbol(Gterm *t,Gterm *subterm);


/************************************************************
 * AC Term
 */

/************************************************************
 * Functions
 */
//extern void term_print(FILE *fich, Gterm *t);
//extern void internal_term_printnl(FILE *fich,Gterm *t, int mode);

extern struct term* normalise(Gterm *res);
extern void term_flatten(Gterm *t);
extern Gterm *term_unflatten(Gterm *t);
extern void term_onf(Gterm *t);
extern int term_cmp(Gterm *t, Gterm *t2);
extern long term_notDestructEqual(Gterm *t1,Gterm *t2);
extern int term_occur(Gterm *t1,Gterm *t2);
extern  Gterm *term_removeTopSymbol(Gterm *t);




/************************************************************
 * Definition des builtins
 */

#define code_int -1

/*
 * hashCode
 */
extern int doobs_hfunction(struct term *t);

#define TERMAC(t) ((struct termac*)t)
#define genericGetArity(t) (!term_isAC(t))?(term_arity(t)):(getArity(TERMAC(t)))
#define genericGetColor(t,i) (!term_isAC(t))?(bicolor):(getColor(TERMAC(t),i))

#define genericTermAlloc(res,arity,symb) (!symb_isAC(symb))?(GmakeApplArity(res,arity,symb)):(TERMAC_ALLOC(res,arity,symb))

#define genericGetSubterm(t,i) (!term_isAC(t))?(GgetArgument(t,i)):(getSubterm(TERMAC(t),i))

#define genericSetSubterm(t,i,st) if(!term_isAC(t)) { GsetArgument(t,i,st); } else \
{ term_add_onf_term_color(TERMAC(t),GgetSymb(t),(st),bicolor); }

#define genericGetMult(t,i) (!term_isAC(t))?(1):(getMult(TERMAC(t),i))

#define genericCopyTermAllocExcept(dest,index,source)\
 if(!term_isAC(source)) {\
   GmakeApplArity(dest,term_arity(source),GgetSymb(source));\
   term_copyTopSymbol(dest,source);\
 } else {\
   TERMAC_ALLOC(dest,getArity(TERMAC(source)),GgetSymb(source));\
   termac_copyTopSymbolExcept(TERMAC(dest),index,TERMAC(source));\
 } 



#endif

