#ifndef _term_h
#define _term_h
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include <stdarg.h>

extern struct fsym fsymtab[];

/* ------------------------------------------------------------ */

struct cell_term
{
  int mult; // multiplicity 
#ifdef COLOR
  int color;
#endif
  struct term *(sub[2]);
};

#ifdef COLOR
#define MULTMASK   0x0000ffff
#define COLORMASK  0xffff0000
//#define getMult(c)          ((c)->mult & MULTMASK)
//#define setMult(c,m)        ((c)->mult = ((c)->mult & COLORMASK) | (m))
//#define getColor(c)         ((c)->mult >> 4)
//#define setColor(c,color)   ((c)->mult = ( ((c)->mult & MULTMASK) | ((color)<<4) ))
#define getMult(c)           ((int)(c)->mult)
#define setMult(c,m)         ((c)->mult=((int)m))
#define getColor(c)          ((int)(c)->color)
#define setColor(c,m)        ((c)->color=((int)m))

#else
#define getMult(c)          ((int)(c)->mult)
#define setMult(c,m)        ((c)->mult=((int)m))
#define getColor(c)
#define setColor(c,m)
#endif

#define cell_t(t)  ((struct term*)(t)->sub[0])
#define cell_next(t)  ((struct cell_term*)(t)->sub[1])

#ifdef COLOR
//#define getColor(cell)     ((cell)->color)
//#define setColor(cell,col) ((cell)->color = (col))
extern int isMonoColor(struct term *t);
#else
//#define getColor(cell)
//#define setColor(cell,col)
#define isMonoColor(t) 0
#endif

//#define bicolor 0
#define bicolor 0xff

extern struct cell_term *cell_create();

#ifdef NOTMACRO
extern void cell_free(struct cell_term *cell);
extern void cell_add_last(struct cell_term *cell, struct term *t);
extern void cell_delete(struct cell_term *last_cell,
			struct cell_term *cell,
			struct term *t);
#else
#define cell_free(cell) FREE(cell);
#define cell_add_last(cell,t)\
  { if(term_first(t)==NULL) term_first(t)=term_last(t)=cell;\
  else { cell_next(term_last(t))=cell; term_last(t)=cell; }}
#define cell_delete(last_cell,cell,t)\
        if(cell==term_first(t))\
	  if(cell==term_last(t))\
	    term_first(t)=term_last(t)=NULL;\
          else\
	    term_first(t)=cell_next(cell);\
        else if(cell==term_last(t)) {\
	  cell_next(last_cell)=NULL; term_last(t)=last_cell; }\
        else cell_next(last_cell)=cell_next(cell);
#endif

extern void cell_insert(struct term *t, struct cell_term *cell,
			struct cell_term *first, struct cell_term *second);

/* ------------------------------------------------------------ */

struct term {
  unsigned int symb;
#if defined GCGEN1
  struct term *next;
#elif defined GCGEN2
  struct chunk *chunk;
#endif
  struct term *(sub[2]);
};

#if defined GCGEN1
#define TERMSTR(strname,arity)\
struct strname {\
  unsigned int symb;\
  struct term *next;\
  struct term *(sub[arity]);\
}
#elif defined GCGEN2
#define TERMSTR(strname,arity)\
struct strname {\
  unsigned int symb;\
  struct chunk *chunk;\
  struct term *(sub[arity]);\
}
#else
#define TERMSTR(strname,arity)\
struct strname {\
  unsigned int symb;\
  struct term *(sub[arity]);\
}
#endif

TERMSTR(term1,1);
TERMSTR(term2,2);

#define term_first(t) ((struct cell_term*)(((struct term*)(t))->sub[0]))
#define term_last(t)  ((struct cell_term*)(((struct term*)(t))->sub[1]))

struct termac {
  unsigned int symb;
#if defined  GCGEN1
  struct term *next;
#elif defined GCGEN2
  struct chunk *chunk;
#endif
  struct term *(sub[2]);
};

#define SHAREMASK  0x10000000
#define ACMASK     0x20000000
#define REDUCEMASK 0x40000000
#define SYMBMASK   0x0000ffff
#define setAC(t)         ((t)->symb |= ACMASK)
#define isAC(t)          ((t)->symb &  ACMASK) 

#define clearShared(t) 
//#define setShared(t)     ((t)->symb |= SHAREMASK)
//#define isShared(t)      ((t)->symb &  SHAREMASK)
#define setShared(t)
#define isShared(t)      (1)


#define clearReduced(t)  ((t)->symb &= (~REDUCEMASK)) 
#define setReduced(t)    ((t)->symb |= REDUCEMASK)
#define isReduced(t)     ((t)->symb &  REDUCEMASK) 

// ((t)->symb &= (!SHAREMASK)) 
#define getSymb(t)       ((t)->symb & SYMBMASK)
#define setSymb(t,s)     ((t)->symb = (s))

#ifdef GCMEM
#define addcounter(t) 
#define subcounter(t) 
#else
#define addcounter(t) ((t)->counter++)
#define subcounter(t) ((t)->counter--)
#endif

#define term_semantic(t) (fsymtab[getSymb(t)].semantic) 
#define term_arity(t) (fsymtab[getSymb(t)].arity)
#define term_name(t) (fsymtab[getSymb(t)].name)
#define term_defstrat(t) (fsymtab[getSymb(t)].defstrat)
#define term_semact(t) (fsymtab[getSymb(t)].semact)

/*
#define fsym_init(code,a,n,sem,dstrat,semaction) {\
	      fsymtab[code].arity=(a);\
	      strdup(fsymtab[code].name,(n));\
	      fsymtab[code].semantic=(sem);\
	      fsymtab[code].defstrat=(dstrat);\
	      fsymtab[code].semact=(semaction);\
              }
	      */

extern void fsym_init(int code, int a, char *n,
		      int sem, int dstrat,
		      struct term* (*semacttion)(struct term *));

extern struct term *term_build(int nbArg, int code, ...);
extern void term_alloc(struct term **ptr_dest,
		       int size_sname,
		       unsigned int funsym);

typedef struct fsym {
  int arity;
  char *name;
  int semantic;
  int defstrat;       // Peter
  struct term* (*semact)(struct term *);      // Peter
} fsym;


extern struct term *term_add_first(struct term *t, struct term *subterm);
extern struct term *term_add_last(struct term *t, struct term *subterm);

#ifdef COLOR
#define term_add_onf_term_color(t1,t2,c) intern_term_add_onf_term(1,t1,t2,c)
#define term_add_onf_term(t1,t2) intern_term_add_onf_term(1,t1,t2,0)
#define term_add_list_term(t1,t2) intern_term_add_onf_term(0,t1,t2,0)
extern struct term *intern_term_add_onf_term(int isAC,
					     struct term *t,
					     struct term *subterm,
					     int color);
#else
#define term_add_onf_term_color(t1,t2,c) intern_term_add_onf_term(1,t1,t2)
#define term_add_onf_term(t1,t2) intern_term_add_onf_term(1,t1,t2)
#define term_add_list_term(t1,t2) intern_term_add_onf_term(0,t1,t2)
extern struct term *intern_term_add_onf_term(int isAC,
					     struct term *t,
					     struct term *subterm);
#endif


extern void term_print(FILE *fich,struct term *t);
extern void term_printnl(FILE *fich,struct term *t);
#define term_println(f,t) term_printnl(f,t)

extern void term_printREF(FILE *fich,struct term *t);
extern void term_printREFln(FILE *fich,struct term *t);


#define symb_arity(s) (fsymtab[s].arity)
#define symb_isAC(s) (symb_arity(s) == -1)

#ifdef NOTMACRO
extern int term_isAC(struct term *t);
#else
#define term_isAC(t) (isAC(t))
#endif


/*
#ifdef NOTMACRO
extern int term_isAC(struct term *t);
#else
#define term_isAC(t) (term_arity(t) == -1)
#endif
*/
extern struct term* normalise(struct term *res);
extern struct term* specialApply(struct term *res);
extern void term_flatten(struct term *t);
extern struct term *term_unflatten(struct term *t);
extern void term_onf(struct term *t);
extern int term_cmp(struct term *t, struct term *t2);
extern long term_notDestructEqual(struct term *t1,struct term *t2);
extern struct term *term_replace(struct term *t1,struct term *t2,struct term *t3);
extern int term_occur(struct term *t1,struct term *t2);
extern struct term *term_copyTopSymbol(struct term *t);
extern  struct term *term_removeTopSymbol(struct term *t);
extern  struct term *term_removePossibleTopSymbol(struct term *t);

extern void freeterm(struct term *t);
extern struct term *term_metaApply(struct term *t);

/* how to get the first argument of a term t */

#define first_arg(t)            t->first

/* how to get t's argument to the immediately right of s.  for example,
   if t = f(a,b,c) and s=b, then term_Sib(t, s) will give the argument c */
#define next_arg(c)             c->next


#define CELL_ALLOC(dest) {(dest)=(struct cell_term*)MALLOC(sizeof(struct cell_term)); setColor(dest,bicolor);}
#define CELL_INIT(dest) {cell_next(dest)=NULL; setColor(dest,bicolor);}



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
	 clearShared(dest);\
	 if(symb_isAC(funsym)) {setAC(dest);\
	 term_first(dest)=(struct cell_term*)NULL;\
	 term_last(dest)=(struct cell_term*)NULL;}\
	}
#endif

#define TERM_ARITY_ALLOC(dest,arity,funsym)\
        {(dest)=(struct term*) \
	   MALLOC(sizeof(struct term1)+(arity-1)*sizeof(struct term*));\
	 setSymb(dest,funsym);\
	 clearShared(dest);\
	 if(symb_isAC(funsym)) {setAC(dest);\
	 term_first(dest)=NULL; term_last(dest)=NULL;}\
	}

#define TERM_FREE(packet) FREE((packet));


#ifdef MEMORY_VERIFY

#ifdef PURIFY
#define DD(X) X
#else
#define DD(X) testpointer(X)
#endif

#else
#define DD(X) X
#endif

/*
 * Definition des builtins
 */

#define code_int -1

/*
 * List of term for RefParser
 */
extern int yyparse();

typedef struct listTerm
{
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
#endif


