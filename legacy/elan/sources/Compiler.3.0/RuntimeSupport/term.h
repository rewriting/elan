#ifndef _term_h
#define _term_h
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include <stdarg.h>

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
struct term {
  unsigned int symb;
  struct term *(sub[2]);
};

#define TERMSTR(strname,arity)\
struct strname {\
  unsigned int symb;\
  struct term *(sub[arity]);\
}
TERMSTR(term1,1);
TERMSTR(term2,2);

//#define SHAREMASK  0x10000000
#define ACMASK     0x20000000
#define REDUCEMASK 0x40000000
#define SYMBMASK   0x0000ffff
#define setAC(t)   ((t)->symb |= ACMASK)
#define isAC(t)    ((t)->symb &  ACMASK) 

// not used 
#define clearReduced(t)  ((t)->symb &= (~REDUCEMASK)) 
#define setReduced(t)    ((t)->symb |= REDUCEMASK)
#define isReduced(t)     ((t)->symb &  REDUCEMASK) 

#define getSymb(t)       ((t)->symb & SYMBMASK)
#define setSymb(t,s)     ((t)->symb = (s))

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


/************************************************************
 * AC Term
 */

struct termac {
  unsigned int symb;
  unsigned int sizeinfo; // max nb of subterms | effective nb of subterms 
  struct term **subterm; // array of [color|mult] subterms 
};

#define SIZEMASK  0xffff0000
#define ARITYMASK 0x0000ffff
#define getArity(t)         ((t)->sizeinfo  & ARITYMASK)
#define setArity(t,n)       ((t)->sizeinfo  = ((t)->sizeinfo & SIZEMASK) | (n))
#define getSize(t)          (((t)->sizeinfo & SIZEMASK) >> 16)
#define setSize(t,n)        ((t)->sizeinfo  = ((t)->sizeinfo & ARITYMASK) | ((n) << 16))

#define setSizeArity(t,s,a) ((t)->sizeinfo = ((s)<<16) | (a))


#define COLORMASK 0xffff0000
#define MULTMASK  0x0000ffff
//typedef signed char multiplicityType;
typedef int multiplicityType;
#define getInternMult(t)    ((multiplicityType)((t) & MULTMASK))
#define setInternMult(t,n)  ((t)  = ((t) & COLORMASK) | (n))
#define getInternColor(t)   (((t) & COLORMASK) >> 16)
#define setInternColor(t,n) ((t)  = ((t) & MULTMASK) | ((n) << 16))

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
  setArity(tac,arity+1);}
#endif

#define termac_add_last(t,st,m) termac_add_lastColor(t,st,m,0)

extern void termac_resize(struct termac *t,int size);
extern void termac_copyTopSymbol(struct termac *tac,struct termac *subterm);

 
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


#endif


