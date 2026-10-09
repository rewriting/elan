#ifndef _term_aterm_h
#define _term_aterm_h
#include "aterm1.h"
#include "aterm2.h"
#include "tools.h"

extern int MAXPOS;
extern struct TR_COQ *head_tr;
void Ginit_builtin();
extern int fsymtabSize;
int *tab_bijection;
extern void tab_bijection_init();

/*#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>*/
typedef struct _ATerm Gterm;
typedef ATermInt GtermInt;
typedef struct _ATermList GtermList;
#include "builtin.h"
//#include "builtin.c"
#include "trace.h"
#define GgetFirst(list) ATgetFirst(list)
#define GgetNext(list) ATgetNext(list)
typedef struct Gfsym {
  AFun afun;
  int arity;
  char *name;
  char *sort;
  int semantic;
  int modulo;
  int defstrat;               // for Peter
  Gterm * (*semact)(Gterm *); // for Peter
} Gfsym;
//#define term_arity(t)    (fsymtab[GgetSymb(t)].arity)
//#define code_arity(t)    (fsymtab[t].arity)
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
void intern_GsetSymb(Gterm **ptr_dest,unsigned int code);
void intern_GsetArgument(Gterm **ptr_dest,int pos, Gterm *t);
//#define GgetArgument(t,p) intern_GgetArgument((Gterm*)t,p)
#define GgetArgument(t,p) ATgetArgument(t,p)

Gterm *intern_GgetArgument(Gterm *v,int p);
void intern_GmakeAppl_Arity(Gterm **ptr_dest,int arity,int code);
void intern_GmakeAppl_Array(Gterm **ptr_dest,int symb,Gterm *ArrayArgs[]);


//void intern_GgetArguments_tab(Gterm ***ptr_arg[],Gterm* res);

GtermList *GlistTermCreate(Gterm *term);
GtermList *GaddTermListTerm(GtermList *list , Gterm *term);
Gterm *GlistGetHead(GtermList *list);
GtermList *GlistGetTail(GtermList *list);
int GlistIsEmpty(GtermList *list);


#define Gmake_const(dest,code)                  intern_Gmake_const(&(dest),code)

/*
#define GmakeAppl0(dest,code)                   intern_GmakeAppl0(&(dest),code)
#define GmakeAppl1(dest,code,s0)                intern_GmakeAppl1(&(dest),code,s0)
#define GmakeAppl2(dest,code,s0,s1)             intern_GmakeAppl2(&(dest),code,s0,s1)
#define GmakeAppl3(dest,code,s0,s1,s2)          intern_GmakeAppl3(&(dest),code,s0,s1,s2)
#define GmakeAppl4(dest,code,s0,s1,s2,s3)       intern_GmakeAppl4(&(dest),code,s0,s1,s2,s3)
#define GmakeAppl5(dest,code,s0,s1,s2,s3,s4)    intern_GmakeAppl5(&(dest),code,s0,s1,s2,s3,s4)
#define GmakeAppl6(dest,code,s0,s1,s2,s3,s4,s5) intern_GmakeAppl6(&(dest),code,s0,s1,s2,s3,s4,s5)
*/

#define GmakeAppl0(dest,code)                   (dest=(Gterm *)ATmakeAppl0(fsymtab[code].afun))
#define GmakeAppl1(dest,code,s0)                (dest=(Gterm *)ATmakeAppl1(fsymtab[code].afun,s0))
#define GmakeAppl2(dest,code,s0,s1)             (dest=(Gterm *)ATmakeAppl2(fsymtab[code].afun,s0,s1))
#define GmakeAppl3(dest,code,s0,s1,s2)          (dest=(Gterm *)ATmakeAppl3(fsymtab[code].afun,s0,s1,s2))
#define GmakeAppl4(dest,code,s0,s1,s2,s3)       (dest=(Gterm *)ATmakeAppl4(fsymtab[code].afun,s0,s1,s2,s3))
#define GmakeAppl5(dest,code,s0,s1,s2,s3,s4)    (dest=(Gterm *)ATmakeAppl5(fsymtab[code].afun,s0,s1,s2,s3,s4))
#define GmakeAppl6(dest,code,s0,s1,s2,s3,s4,s5) (dest=(Gterm *)ATmakeAppl6(fsymtab[code].afun,s0,s1,s2,s3,s4,s5))



#define GmakeApplArity(dest,arity,code)         intern_GmakeAppl_Arity(&(dest),arity,code)
#define GmakeApplArray(dest,t,ArrayArgs)        intern_GmakeAppl_Array((Gterm**)(&dest),t,ArrayArgs)
#define GsetArgument(dest,pos,t)                intern_GsetArgument((Gterm**)(&dest),pos,t)
//#define GgetArgument(v,pos)                     intern_GgetArgument(v,pos)
#define GgetArguments_tab(arg,res)              arg = malloc(sizeof(term_arity(res) * sizeof(Gterm *)));\
                                                for(i=0;i<term_arity(res);i++)\
                                                {\
                                                arg[i]=ATgetArgument((ATermAppl)res,i);\
                                                }
int intern_GgetSymb(Gterm *);


#define myoptatoi(s) (100*(*s) + 10*(*(s+1)) + (*(s+2)) - (111*'0'))

#define getCode(name) (myatoi(name))
//#define getCode(name) ( ((name[0]-1)<<8)+name[1] )
//#define getCode(name) (myoptatoi(name))


//#define GgetSymb(t)                     intern_GgetSymb((Gterm*)t)
//#define GgetSymb(t)                     getCode(ATgetName(ATgetAFun(((ATermAppl)t))))

#define GgetSymb(t)                     tab_bijection[ATgetAFun(((ATermAppl)t))]
#define GgetSymbAC(t) GgetSymb((Gterm*)t)
#define GsetSymb(dest,code)             intern_GsetSymb(&((Gterm*)dest),code)
#define GsetSymbAC(dest,code)

#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */
extern int Ginitialise_trace();

#define GclearReduced(t)        0//((t)->symb &= (~REDUCEMASK))
#define GsetReduced(t)          0//((t)->symb |= REDUCEMASK)
#define GisReduced(t)           0//((t)->symb &  REDUCEMASK)


#define term_notDestructEqual(t1,t2) ((t1) == (t2))













/*
HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
HHHHHHHHHHHHHHHHHHHHHHHHHHH DUPLICATION HHHHHHHHHHHHHHHHHHHHHH
HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
*/
#ifndef HASHCODE
#define ACMASK     0x20000000
#else // HCODE
#define ACMASK     0x2000
#endif

#define INTERN_HFUNCTION(t,st) 0
#define getHcode(t)
#define setHcode(t,h)



//#define GinitMinimalSize(s) ((1+((s)>>2))<<2)
#define GsetAC(t)   ((t)->symb |= ACMASK)
#define GnotYetImplemented(s) {\
    fprintf(stderr,"%s: not yet implemented\n",s); exit(1); }
#define isAC(t)    0

// A ENLEVER
#ifndef HASHCODE
#define TERMSTR(strname,arity)\
struct strname {\
  unsigned int symb;\
  Gterm *(sub[arity]);\
}
#else // HCODE
#define TERMSTR(strname,arity)\
struct strname {\
  unsigned short int symb;\
  unsigned short int hcode;\
  Gterm *(sub[arity]);\
}
#endif

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





















