#ifndef _term_h
#define _term_h
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "builtin.h"
#include <stdarg.h>

//#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */

/************************************************************
 * Symbol Definition
 */
extern struct fsym fsymtab[];

typedef struct fsym {
  int arity;
  char *name;
  int semantic;
  int modulo;
  int defstrat;                          // for Peter
  struct term* (*semact)(struct term *); // for Peter
} fsym;

#define term_semantic(t) (fsymtab[getSymb(t)].semantic) 
#define term_arity(t)    (fsymtab[getSymb(t)].arity)
#define term_modulo(t)   (fsymtab[getSymb(t)].modulo)
#define term_name(t)     (fsymtab[getSymb(t)].name)
#define term_defstrat(t) (fsymtab[getSymb(t)].defstrat)
#define term_semact(t)   (fsymtab[getSymb(t)].semact)
#define symb_arity(s)    (fsymtab[s].arity)
#define symb_modulo(s)   (fsymtab[s].modulo)
#define symb_isAC(s)     (symb_arity(s) == -1)

extern void fsym_init(int code, int a, char *n,
		      int sem, int dstrat,
		      struct term* (*semacttion)(struct term *));

/************************************************************
 * Syntactic Term
 */
#ifdef HCODE
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
#else // HCODE

#define ACMASK     0x2000
#define REDUCEMASK 0x4000
#define SYMBMASK   0x0fff

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
#define setHcode(t,h)          (t->hcode=h)
#endif // HCODE

TERMSTR(term1,1);
TERMSTR(term2,2);

/*
 * General macro for MASK
 * MASK 0x0000ffff
 * SHIFT 16
 */

#define getValue(t,  MASK,SHIFT) (((t) & MASK) >> SHIFT)
#define setValue(t,v,MASK,SHIFT) ((t) = ((t) & (~MASK)) | ((v)<<SHIFT))

#ifndef HCODE

#define HCODEMASK  0x0ffff000
#define HCODESHIFT 12
#define setAC(t)   ((t)->symb |= ACMASK)
#define isAC(t)    ((t)->symb &  ACMASK) 

#define getSymb(t)             ((t)->symb & SYMBMASK)
#define setSymb(t,s)           ((t)->symb = (s))

#define getFreeSubterm(t,i)    ((t)->sub[i])
#define setFreeSubterm(t,i,st) ((t)->sub[i]=(st))

#define INTERN_HFUNCTION(t,st) 0

#else // HCODE

//#define getHcode(t)            (isTagged(t)?((unsigned int)t):(((unsigned int)t->symb&HCODEMASK)>>HCODESHIFT))
//#define setHcode(t,h)          ((t->symb)=(t->symb&~HCODEMASK)|(h<<HCODESHIFT))

#define setAC(t)   ((t)->symb |= ACMASK)
#define isAC(t)    ((t)->symb &  ACMASK) 

#define HNUMBER                65599
#define INTERN_HFUNCTION(t,st) (((getHcode(t)+getHcode(st))) % HNUMBER)
#define HFUNCTION(t,st)        (isTagged(st)?getHcode(t):INTERN_HFUNCTION(t,st))

#define getSymb(t)             ((t)->symb & SYMBMASK)
#define setSymb(t,s)           { (t)->symb = (s); \
                                  if(!isTagged(t)) {setHcode(t,s);} }

#define getFreeSubterm(t,i)    ((t)->sub[i])
#define setFreeSubterm(t,i,st) { ((t)->sub[i]=(st));\
                                 setHcode(t,HFUNCTION(t,st)); }
#endif // HCODE

// used  in trace.c
#define clearReduced(t)        ((t)->symb &= (~REDUCEMASK)) 
#define setReduced(t)          ((t)->symb |= REDUCEMASK)
#define isReduced(t)           ((t)->symb &  REDUCEMASK) 

extern struct term *term_build(int nbArg, int code, ...);
extern void term_alloc(struct term **ptr_dest,
		       int size_sname,
		       unsigned int funsym);
#define TERM_CONST_ALLOC(dest,funsym)\
        {(dest)=(struct term*) malloc(sizeof(struct term1));\
	 setSymb(dest,funsym);\
	}
//  TERM_ALLOC(dest,term1,symb)

#ifdef DEBUG
#define TERM_ALLOC(dest,sname,funsym)\
         term_alloc(&dest,sizeof(struct sname),funsym)
#else
#define TERM_ALLOC(dest,sname,funsym)\
        {(dest)=(struct term*) MALLOC(sizeof(struct sname));\
	 setSymb(dest,funsym);\
	}
#endif

#define TERM_ARITY_ALLOC(dest,arity,funsym)\
        {(dest)=(struct term*) \
	   MALLOC(sizeof(struct term1)+(arity-1)*sizeof(struct term*));\
	 setSymb(dest,funsym);\
	 if(symb_isAC(funsym)) {printf("TERM_ALLOC AC\n");}\
	}

#define TERM_FREE(packet) FREE((packet));
extern void term_copyTopSymbol(struct term *t,struct term *subterm);


/************************************************************
 * AC Term
 */

#ifndef HASHCODE
struct termac {
  unsigned int symb;     // info | hcode | symb
  unsigned int sizeinfo; // max nb of subterms | effective nb of subterms 
  struct term **subterm; // array of [color|mult] subterms 
};
#else // HCODE
struct termac {
  unsigned short int symb;  // info | symb
  unsigned short int hcode; // hcode
  unsigned int sizeinfo; // max nb of subterms | effective nb of subterms 
  struct term **subterm; // array of [color|mult] subterms 
};
#endif // HCODE


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

#define getMult(t,i)        getInternMult((unsigned int)((t)->subterm[(i)<<1]))
#define setMult(t,i,m)      setInternMult((unsigned int)((t)->subterm[(i)<<1]),(unsigned int)(m))
#define getSubterm(t,i)     (t->subterm[((i)<<1)+1])
#define setSubterm(t,i,st)  ((t->subterm[((i)<<1)+1]) = (st))

#define setColorMult(t,i,c,m) (((unsigned int)((t)->subterm[(i)<<1])) = (((c)<<16) | (m)))

#ifdef COLOR
#define getColor(t,i)       getInternColor((unsigned int)((t)->subterm[(i)<<1]))
#define setColor(t,i,c)     setInternColor((unsigned int)((t)->subterm[(i)<<1]),(unsigned int)(c))
#else
#define getColor(t,i)
#define setColor(t,i,c)
#endif

#define TERMAC_ALLOC(dest,size,funsym)\
         termac_alloc(((struct termac **)&dest),size,funsym)

extern void termac_alloc(struct termac **ptr_dest,
                         int size,
                         unsigned int funsym);

#ifdef NOTMACRO
extern int term_isAC(struct term *t);
extern void termac_add_lastColor(struct termac *t,struct term *subterm,
                                 int mult, int color);
#else
#define term_isAC(t) (isAC(t))
#define termac_add_lastColor(tac,subterm,mult,color) {\
  register int arity = getArity(tac);\
  if(arity == getSize(tac)) termac_resize(tac,2*arity);\
  setColorMult(tac,arity,color,mult);\
  setSubterm(tac,arity,subterm);\
  setHcode(tac,INTERN_HFUNCTION(tac,subterm));\
  setArity(tac,arity+1);}
#endif

#define termac_add_last(t,st,m) termac_add_lastColor(t,st,m,0)

extern void termac_resize(struct termac *t,int size);
extern void termac_copyTopSymbol(struct termac *tac,struct termac *subterm);
extern void termac_copyTopSymbolExcept(struct termac *tac,int i,struct termac *subterm);

 
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
#ifdef COLOR
//#define getColor(c)          ((int)(c)->color)
//#define setColor(c,m)        ((c)->color=((int)m))
extern int isMonoColor(struct term *t);
extern void setMonoColor(struct term *t);

#define term_add_onf_term_color(t1,fsym,t2,c) intern_term_add_onf_term(1,(struct termac*)t1,fsym,t2,c)
#define term_add_onf_term(t1,fsym,t2) intern_term_add_onf_term(1,(struct termac*)t1,fsym,t2,bicolor)

#define term_add_list_term(t1,fsymt2) intern_term_add_onf_term(0,(struct termac*)t1,fsym,t2,0)
extern struct termac *intern_term_add_onf_term(int isAC,
			 		       struct termac *tac,
                                               unsigned int fsym,
					       struct term *subterm,
                                               int color);
#else
//#define getColor(c)
//#define setColor(c,m)
#define isMonoColor(t) 0
#define term_add_onf_term_color(t1,fsym,t2,c) intern_term_add_onf_term(1,t1,fsym,t2)
#define term_add_onf_term(t1,fsym,t2) intern_term_add_onf_term(1,t1,fsym,t2)
#define term_add_list_term(t1,fsym,t2) intern_term_add_onf_term(0,t1,fsymt2)
extern struct termac *intern_term_add_onf_term(int isAC,
					       struct termac *tac,
                                               unsigned int fsym,
					       struct term *subterm);
#endif


/************************************************************
 * Functions
 */
extern void term_print(FILE *fich,struct term *t);
extern void internal_term_printnl(FILE *fich,struct term *t, int mode);

#define NO_IO       0
#define REF_IO      1
#define ELAN_IO     2
#define INTERNAL_IO 3
#define term_println(f,t) internal_term_println(f,t,INTERNAL_IO)
#define term_printnl(f,t) internal_term_println(f,t,INTERNAL_IO)

extern void term_printREF(FILE *fich,struct term *t);
extern void term_printREFln(FILE *fich,struct term *t);

extern struct term* normalise(struct term *res);
extern struct term* specialApply(struct term *res);
extern void term_flatten(struct term *t);
extern struct term *term_unflatten(struct term *t);
extern void term_onf(struct term *t);
extern int term_cmp(struct term *t, struct term *t2);
extern long term_notDestructEqual(struct term *t1,struct term *t2);
extern struct term *term_replace(struct term *t1,struct term *t2,struct term *t3);
extern struct term *term_rec_replace(struct term *t1,struct term *t2,struct term *t3);
extern int term_occur(struct term *t1,struct term *t2);
extern  struct term *term_removeTopSymbol(struct term *t);

extern struct term *term_metaApply(struct term *t);

#ifdef MEMORY_VERIFY

#ifdef PURIFY
#define DD(X) X
#else
#define DD(X) testpointer(X)
#endif

#else
#define DD(X) X
#endif

/************************************************************
 * Definition des builtins
 */

#define code_int -1

/************************************************************
 * List of term for RefParser
 */
extern int yyparse();

typedef struct listTerm {
  struct term *term;
  struct listTerm *next;
  struct listTerm *last;
} listTerm;

extern listTerm *listTermCreate(struct term *term);
extern listTerm *addTermListTerm(listTerm *list , struct term *term);

extern struct term *asfNull();
extern struct term *asfCons(struct term *t1,struct term *t2);
extern struct term *asfHead(struct term *t);
extern struct term *asfTail(struct term *t);
extern struct term *asfPrefix(struct term *t);
extern struct term *asfLast(struct term *t);
extern struct term *asfNotEmptyList(struct term *t);
extern struct term *asfIsSingleElement(struct term *t);

/*
 * CC_GC
 */

struct termac *CC_GC_encode(struct termac *tac);
struct termac *CC_GC_decode(struct termac *tac);

/*
 * Array
 */
extern struct term *term_newArray(int n, struct term *t);
extern struct term *term_getArray(struct term *array, int n);
extern struct term *term_setArray(struct term *array, int n, struct term *t);

/*
 * hashCode
 */
extern int hashTerm(struct term *t);




#define TERMAC(t) ((struct termac*)t)

#define genericGetArity(t) (!term_isAC(t))?(term_arity(t)):(getArity(TERMAC(t)))
#define genericGetColor(t,i) (!term_isAC(t))?(bicolor):(getColor(TERMAC(t),i))

#define genericTermAlloc(res,arity,symb) (!symb_isAC(symb))?(TERM_ARITY_ALLOC(res,arity,symb)):(TERMAC_ALLOC(res,arity,symb))

#define genericGetSubterm(t,i) (!term_isAC(t))?(getFreeSubterm(t,i):(getSubterm(TERMAC(t),i))

#define genericSetSubterm(t,i,st) if(!term_isAC(t)) { setFreeSubterm(t,i,st); } else \
{ term_add_onf_term_color(TERMAC(t),getSymb(t),(st),bicolor); }

#define genericGetMult(t,i) (!term_isAC(t))?(1):(getMult(TERMAC(t),i))

#define genericCopyTermAllocExcept(dest,index,source)\
 if(!term_isAC(source)) {\
   TERM_ARITY_ALLOC(dest,term_arity(source),getSymb(source));\
   term_copyTopSymbol(dest,source);\
 } else {\
   TERMAC_ALLOC(dest,getArity(TERMAC(source)),getSymb(source));\
   termac_copyTopSymbolExcept(TERMAC(dest),index,TERMAC(source));\
 } 


#endif


