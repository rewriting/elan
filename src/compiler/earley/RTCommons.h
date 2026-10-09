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
    Christophe Ringeissen	e-mail: Christophe.Ringeissen@loria.fr
    Marian Vittek		e-mail: vittek@guma.ii.fmph.uniba.sk

*/
#ifndef __RTCommons_h
#define __RTCommons_h

#define META

#ifndef RUNTIME 
// begin RUNTIME
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/resource.h>

#ifdef GCMEM
#include "gc.h"
#endif
/*
// suppose to use C++ only to link with SOLVER

// Here you need to include All solver files
*/

#ifdef SOLVER
#include <ilsolver/control.h>
#include <ilsolver/ilcint.h>
#include <ilsolver/ilcfloat.h>

extern IlcFloatExpI * ElanDeriveI(IlcFloatExpI* fun, IlcFloatExpI* var);
extern IlcFloatExpI * ElanSimplifyI(IlcFloatExpI* fun);

#define ElanDerive(ftext,xtext) ElanDeriveI((ftext).getImpl(), (xtext).getImpl())
#define ElanSimplify(ftext) ElanSimplifyI((ftext).getImpl())

#endif

#ifdef CSCP
#include "choice.h"
#include "Back.h"
//#define allocStable(x) malloc(x)
#else
#include "Back.h"
#endif

#ifdef PDEBUGG
#define DD(XX) testpointer(XX)
#else
#define DD(XX) XX
#endif


#define MEMCHUNKSIZE 100000    /* size of chunks allocated by malloc */
#define MAXABORTED 100         /* maximal space left free in chunk */
#define MAXARITY 500

#define DOUBLECONSTLIST f2list
#define DOUBLECONST 49

#define BUILTINTERM   1
#define EPM 32000             /* ENDFPROFILMARK */

#endif 
// end RUNTIME

#define STANDARDTERM 0


#define TERMSTR(strname,arity) struct strname {\
  int counter;\
  struct term **myfreelist;\
  int fs;\
  struct term *(sub[arity]);\
}

struct term {
  int counter; /* reference counter */
  struct term **myfreelist;
  int fs;
  struct term *(sub[2]);
};

#ifndef RUNTIME 
// begin RUNTIME


struct fsym {
  int arity;
};

#endif // RUNTIME

#ifdef GCMEM
#define ALLOC0(dest,sname,freelist,funsym) {\
	      dest = GC_malloc(sizeof(struct sname)); }
#define ALLOCaS0(dest,sou,arity) {\
      dest = GC_malloc(sizeof(struct term)+(arity-2)*sizeof(struct term*)); }
#else
#define ALLOC0(dest,sname,freelist,funsym) {\
  if (freelist==NULL) {\
    dest = allocator(sizeof(struct sname),&(freelist));\
  } else {\
    dest = freelist; freelist = freelist->sub[0];\
  }\
}

/* 
  I have to change this, it is not portable, depends on compiler !!!!!!!!!!!!!
  something like term##arity would be of interest
*/
#define ALLOCaS0(dest,sou,arity) {\
  if (*(sou->myfreelist) == NULL) {\
    dest = allocator(\
	   sizeof(struct term)+(arity-2)*sizeof(struct term*),\
	   sou->myfreelist);\
  } else {\
    dest = *(sou->myfreelist); *(sou->myfreelist) = dest->sub[0];\
  }\
}
#endif

#ifdef GCMEM
#define FREE0
#else
#define FREE0(packet) {\
  register struct term **tmp;\
  tmp = packet->myfreelist;\
  packet->sub[0] = *tmp; *tmp = packet;\
}
#endif

#ifdef PDEBUGG
#define ALLOC(dest,sname,freelist,funsym) {ALLOC0(dest,sname,freelist,funsym);testalloc(dest);}
#define ALLOCaS(dest,sou,arity) {ALLOCaS0(dest,sou,arity);testalloc(dest);}
#define FREE(packet) {testfree(packet); FREE0(packet);}
#else
#define ALLOC(dest,sname,freelist,funsym) ALLOC0(dest,sname,freelist,funsym)
#define ALLOCaS(dest,sou,arity) ALLOCaS0(dest,sou,arity)
#define FREE(packet) FREE0(packet)
#endif

#ifndef RUNTIME 
// begin RUNTIME

#define INTDIFF(a,b) (((int) a)-((int) b))

#ifdef BINS
#define getInt(n) (((int) n)/2)
#define setTag(n) ((struct term*)((n)*2+1))
/* REDOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOo */
#define fun3(a,b) ((struct term*)(((((int) a)/2)+(((int) b)/2))*2+1))
#define fun4(a,b) ((struct term*)(((((int) a)/2)-(((int) b)/2))*2+1))
#define fun5(a,b) ((struct term*)(((((int) a)/2)*(((int) b)/2))*2+1))
#define fun6(a,b) ((struct term*)(((((int) a)/2)/(((int) b)/2))*2+1))
/* ......#define fun7(a,b) ((struct term*)(((int) a)+((int) b))) */
#define fun8(a,b) ((struct term*)((((int) a) == ((int) b))*2+1))
#define fun9(a,b) ((struct term*)((((int) a) != ((int) b))*2+1))

#define fun10(a,b) ((struct term*)(((int) a) < ((int) b)))
#define fun11(a,b) ((struct term*)(((int) a) <= ((int) b)))
#define fun12(a,b) ((struct term*)(((int) a) >  ((int) b)))
#define fun13(a,b) ((struct term*)(((int) a) >= ((int) b)))

/*** IDENT ??? */
#define fun15(a,b) ((struct term*)(((int) a) != ((int) b)))
#define fun16(a,b,c) (replace(c,a,b,0))
#define fun17(a,b) ((struct term*) (occur(b,a,0)))
#define fun18(a,b) ((struct term*) (tcmp(a,b,0,0)==0))
#define fun19(a,b) ((struct term*) (tcmp(a,b,0,0)!=0))

#define fun20(a) ((struct term*)(((-(((int) a)/2)))*2+1))
#define fun21(a,b) ((struct term*)((((int) a) && ((int) b))*2+1))
#define fun22(a,b) ((struct term*)((((int) a) || ((int) b))*2+1))

#define fun24(a) ((struct term*)(((!(((int) a)/2)))*2+1))
#define fun25(a) (a)
#define fun26(a) (a)

#define fun27(a,b) ((struct term*)((((int) a) % ((int) b))*2+1))
#define fun28(a,b) ((struct term*)((((int) a) & ((int) b))*2+1))
#define fun29(a,b) ((struct term*)((((int) a) | ((int) b))*2+1))

#define fun30(a,b) ((struct term*) (tcmp(a,b,0,0)<0))
#define fun31(a,b) ((struct term*) (tcmp(a,b,0,0)<=0))
#define fun32(a,b) ((struct term*) (tcmp(a,b,0,0)>0))
#define fun33(a,b) ((struct term*) (tcmp(a,b,0,0)>=0))
#else //---------------------------------------------------- Bins = 0

#define getInt(n) ((int)(n))
#define setTag(n) ((struct term*)(n))
#define isTagged(n) (1)

//#define getInt(n) (((int)(n))/2)
//#define setTag(n) ((struct term*)((n)*2+1))
//#define isTagged(n) (((int)n)&1)

#define fun3(a,b) setTag(getInt(a) + getInt(b))
#define fun4(a,b) setTag(getInt(a) - getInt(b))
#define fun5(a,b) setTag(getInt(a) * getInt(b))
#define fun6(a,b) setTag(getInt(a) / getInt(b))
/* #define fun7(a,b) ... */
#define fun8(a,b) setTag(getInt(a) == getInt(b)) 
#define fun9(a,b) setTag(getInt(a) != getInt(b))
#define fun10(a,b) setTag(getInt(a) <  getInt(b))
#define fun11(a,b) setTag(getInt(a) <= getInt(b))
#define fun12(a,b) setTag(getInt(a) >  getInt(b))
#define fun13(a,b) setTag(getInt(a) >= getInt(b))

/*** IDENT ??? */
#define fun15(a,b) setTag(getInt(a) != getInt(b))
#define fun16(a,b,c) (replace(c,a,b,0))

#define fun17(a,b) setTag(occur(b,a,0))
#define fun18(a,b) setTag(tcmp(a,b,0,0)==0)
#define fun19(a,b) setTag(tcmp(a,b,0,0)!=0)

#define fun20(a) setTag(-getInt(a))
#define fun21(a,b) setTag(getInt(a) && getInt(b))
#define fun22(a,b) setTag(getInt(a) || getInt(b))

#define fun24(a) setTag(!getInt(a))
#define fun25(a) (a)
#define fun26(a) (a)

#define fun27(a,b) setTag(getInt(a) % getInt(b))
#define fun28(a,b) setTag(getInt(a) & getInt(b))
#define fun29(a,b) setTag(getInt(a) | getInt(b))

#define fun30(a,b) setTag(tcmp(a,b,0,0)<0)
#define fun31(a,b) setTag(tcmp(a,b,0,0)<=0)
#define fun32(a,b) setTag(tcmp(a,b,0,0)>0)
#define fun33(a,b) setTag(tcmp(a,b,0,0)>=0)
 
/*
#define fun3(a,b) setTag(getInt(a) + getInt(b))
//#define fun3(a,b) ((struct term*)(((int) a)+((int) b)))

#define fun4(a,b) setTag(getInt(a) - getInt(b))
//#define fun4(a,b) ((struct term*)(((int) a)-((int) b)))

#define fun5(a,b) ((struct term*)(((int) a)*((int) b)))
#define fun6(a,b) ((struct term*)(((int) a)/((int) b)))
// #define fun7(a,b) ((struct term*)(((int) a)+((int) b))) 
#define fun8(a,b) ((struct term*)(((int) a) == ((int) b)))
#define fun9(a,b) ((struct term*)(((int) a) != ((int) b)))
#define fun10(a,b) ((struct term*)(((int) a) < ((int) b)))
#define fun11(a,b) ((struct term*)(((int) a) <= ((int) b)))

#define fun12(a,b) setTag(getInt(a) > getInt(b))
//#define fun12(a,b) ((struct term*)(((int) a) >  ((int) b)))

#define fun13(a,b) ((struct term*)(((int) a) >= ((int) b)))

#define fun15(a,b) ((struct term*)(((int) a) != ((int) b)))
#define fun16(a,b,c) (replace(c,a,b,0))
#define fun17(a,b) ((struct term*) (occur(b,a,0)))
#define fun18(a,b) ((struct term*) (tcmp(a,b,0,0)==0))
#define fun19(a,b) ((struct term*) (tcmp(a,b,0,0)!=0))

#define fun20(a) ((struct term*)(-((int) a)))
#define fun21(a,b) ((struct term*)(((int) a) && ((int) b)))
#define fun22(a,b) ((struct term*)(((int) a) || ((int) b)))

#define fun24(a) ((struct term*)(! ((int) a)))
#define fun25(a) (a)
#define fun26(a) (a)
#define fun27(a,b) ((struct term*)(((int) a) % ((int) b)))
#define fun28(a,b) ((struct term*)(((int) a) & ((int) b)))
#define fun29(a,b) ((struct term*)(((int) a) | ((int) b)))

#define fun30(a,b) ((struct term*) (tcmp(a,b,0,0)<0))
#define fun31(a,b) ((struct term*) (tcmp(a,b,0,0)<=0))
#define fun32(a,b) ((struct term*) (tcmp(a,b,0,0)>0))
#define fun33(a,b) ((struct term*) (tcmp(a,b,0,0)>=0))
*/

#endif //----------------------------------------------------

#endif RUNTIME 
// end RUNTIME
#ifdef __cplusplus

extern "C" struct term * crDouble(double);
extern "C" double getDouble(struct term*vv);

#ifdef EARLEY
extern "C" struct term * earley_fun3(struct term*,struct term*);
extern "C" struct term * earley_fun4(struct term*,struct term*);
extern "C" struct term * earley_fun5(struct term*,struct term*);
extern "C" struct term * earley_fun6(struct term*,struct term*);
// 7
extern "C" struct term * earley_fun8(struct term*,struct term*);
extern "C" struct term * earley_fun9(struct term*,struct term*);
extern "C" struct term * earley_fun10(struct term*,struct term*);
extern "C" struct term * earley_fun11(struct term*,struct term*);
extern "C" struct term * earley_fun12(struct term*,struct term*);
extern "C" struct term * earley_fun13(struct term*,struct term*);
// 14
extern "C" struct term * earley_fun15(struct term*,struct term*);
extern "C" struct term * earley_fun16(struct term*,struct term*,struct term*);
extern "C" struct term * earley_fun17(struct term*,struct term*);
extern "C" struct term * earley_fun18(struct term*,struct term*);
extern "C" struct term * earley_fun19(struct term*,struct term*);
extern "C" struct term * earley_fun20(struct term*);
extern "C" struct term * earley_fun21(struct term*,struct term*);
extern "C" struct term * earley_fun22(struct term*,struct term*);
// 23
extern "C" struct term * earley_fun24(struct term*);
extern "C" struct term * earley_fun25(struct term*);
extern "C" struct term * earley_fun26(struct term*);
extern "C" struct term * earley_fun27(struct term*,struct term*);
extern "C" struct term * earley_fun28(struct term*,struct term*);
extern "C" struct term * earley_fun29(struct term*,struct term*);
extern "C" struct term * earley_fun30(struct term*,struct term*);
extern "C" struct term * earley_fun31(struct term*,struct term*);
extern "C" struct term * earley_fun32(struct term*,struct term*);
extern "C" struct term * earley_fun33(struct term*,struct term*);
#endif

extern "C" struct term * fun38(struct term*,struct term*);
extern "C" struct term * fun39(struct term*,struct term*);
extern "C" struct term * fun40(struct term*,struct term*);
extern "C" struct term * fun41(struct term*,struct term*);
extern "C" struct term * fun42(struct term*);
extern "C" struct term * fun43(struct term*,struct term*);
extern "C" struct term * fun44(struct term*,struct term*);
extern "C" struct term * fun45(struct term*,struct term*);
extern "C" struct term * fun46(struct term*,struct term*);
extern "C" struct term * fun47(struct term*,struct term*);
extern "C" struct term * fun48(struct term*,struct term*);


extern "C" struct term * fun50(struct term*);
extern "C" struct term * fun51(struct term*);
extern "C" struct term * fun52(struct term*);
extern "C" struct term * fun53(struct term*);
extern "C" struct term * fun54(struct term*);
extern "C" struct term * fun55(struct term*);
extern "C" struct term * fun56(struct term*,struct term*);
extern "C" struct term * fun57(struct term*);
extern "C" struct term * fun58(struct term*);
extern "C" struct term * fun59(struct term*);
extern "C" struct term * fun60(struct term*);
extern "C" struct term * fun61(struct term*);
extern "C" struct term * fun62(struct term*);
extern "C" struct term * fun63(struct term*);
extern "C" struct term * fun78(struct term*);

//#ifdef IOS
extern "C" struct term * earley_fun113(struct term*,struct term*); // getc
extern "C" struct term * earley_fun114(struct term*); // putc
extern "C" struct term * earley_fun115(struct term*); // create
extern "C" struct term * earley_fun116(struct term*,struct term*); // open
extern "C" struct term * earley_fun118(struct term*); // close
extern "C" struct term * earley_fun119(struct term*,struct term*); // write
extern "C" struct term * earley_fun120(struct term*); // write

extern "C" struct term * earley_fun121(struct term*);
extern "C" struct term * earley_fun122(struct term*);
extern "C" struct term * earley_fun123(struct term*);

//#endif

//#ifdef STRINGS
extern "C" struct term * earley_fun150(struct term*);  // length
extern "C" struct term * earley_fun151(struct term*,struct term*);  //append
extern "C" struct term * earley_fun152(struct term*,struct term*);  // indexing
extern "C" struct term * earley_fun153(struct term*,struct term*,struct term*); //@[@<-@]
extern "C" struct term * earley_fun154(struct term*,struct term*,struct term*); //substr
extern "C" struct term * earley_fun156(struct term*,struct term*);  // strspn
extern "C" struct term * earley_fun157(struct term*,struct term*);  //strcmp
extern "C" struct term * earley_fun158(struct term*);  //string
//#endif

/* some functions in library */

extern "C" void termwrite(struct term *,int);
extern "C" void termwriter(struct term *,int);
extern "C" void freeterm(struct term *);
extern "C" int occur(struct term*,struct term*,int);
extern "C" struct term * replace(struct term*,struct term*,struct term*,int);
extern "C" int tcmp(struct term*,struct term*,int,int);
extern "C" struct term * allocator(int ,struct term **);
extern "C" void timestart();
extern "C" void timestop();

extern "C" struct term *testpointer(struct term*);
#ifdef META
extern "C" struct term * fun129(struct term*, struct term*);   /* Meta_apply*/
extern "C" struct term * fun130(struct term*, struct term*, int);  /* Meta_a*/
#endif

extern "C"  struct term *fun180(int code, struct term *s, struct term *t);
extern "C"  struct term *fun181(int code, struct term *s);
extern "C"  struct term *fun182(int code, struct term *s);
extern "C"  struct term *fun183(int code);
extern "C"  struct term *fun186(int code);
extern "C"  struct term *fun184(int code, struct term *b, struct term *s, struct term *t);
extern "C"  struct term *fun185(int code, struct term *s, struct term *t);
extern "C"  struct term *fun187(int code, struct term *b, struct term *s, struct term *t);
extern "C"  struct term *fun188(int code, struct term *s, struct term *t);

#else


extern struct term * crDouble();
extern double getDouble();

#ifdef EARLEY
extern  struct term * earley_fun3();
extern  struct term * earley_fun4();
extern  struct term * earley_fun5();
extern  struct term * earley_fun6();
// 7
extern  struct term * earley_fun8();
extern  struct term * earley_fun9();
extern  struct term * earley_fun10();
extern  struct term * earley_fun11();
extern  struct term * earley_fun12();
extern  struct term * earley_fun13();
// 14
extern  struct term * earley_fun15();
extern  struct term * earley_fun16();
extern  struct term * earley_fun17();
extern  struct term * earley_fun18();
extern  struct term * earley_fun19();
extern  struct term * earley_fun20();
extern  struct term * earley_fun21();
extern  struct term * earley_fun22();
// 23
extern  struct term * earley_fun24();
extern  struct term * earley_fun25();
extern  struct term * earley_fun26();
extern  struct term * earley_fun27();
extern  struct term * earley_fun28();
extern  struct term * earley_fun29();
extern  struct term * earley_fun30();
extern  struct term * earley_fun31();
extern  struct term * earley_fun32();
extern  struct term * earley_fun33();
#endif


extern struct term * fun38();
extern struct term * fun39();
extern struct term * fun40();
extern struct term * fun41();
extern struct term * fun42();
extern struct term * fun43();
extern struct term * fun44();
extern struct term * fun45();
extern struct term * fun46();
extern struct term * fun47();
extern struct term * fun48();


extern struct term * fun50();
extern struct term * fun51();
extern struct term * fun52();
extern struct term * fun53();
extern struct term * fun54();
extern struct term * fun55();
extern struct term * fun56();
extern struct term * fun57();
extern struct term * fun58();
extern struct term * fun59();
extern struct term * fun60();
extern struct term * fun61();
extern struct term * fun62();
extern struct term * fun63();
extern struct term * fun78();

//#ifdef IOS
extern struct term * earley_fun113(); // getc
extern  struct term * earley_fun114(); // putc
extern struct term * earley_fun115(); // create
extern struct term * earley_fun116(); // open
extern struct term * earley_fun118(); // close
extern struct term * earley_fun119(); // write
extern  struct term * earley_fun120(); // write

extern  struct term * earley_fun121();
extern  struct term * earley_fun122();
extern  struct term * earley_fun123();
//#endif

//#ifdef STRINGS
extern struct term * earley_fun150();  // length
extern struct term * earley_fun151();  //append
extern struct term * earley_fun152();  // indexing
extern struct term * earley_fun153(); //@[@<-@]
extern struct term * earley_fun154(); //substr
extern struct term * earley_fun156();  // strspn
extern struct term * earley_fun157();  //strcmp
extern struct term * earley_fun158();  //string
//#endif

/* some functions in library */

extern void termdump();
extern void termwriter();
extern void termwrite();
extern void freeterm();
extern int occur();
extern struct term * replace();
extern int tcmp();
extern struct term * allocator();
extern exitnorule();
extern struct term *testpointer();
extern statistics();
extern void timestart();
extern void timestop();
#ifdef META
extern struct term * fun129();
extern struct term * fun130();
#endif

extern struct term *fun180();
extern struct term *fun181();
extern struct term *fun182();
extern struct term *fun183();
extern struct term *fun186();
extern struct term *fun184();
extern struct term *fun185();
extern struct term *fun187();
extern struct term *fun188();

#endif
#ifndef RUNTIME 
// begin RUNTIME
/* statistics */
extern int  anofreduction[];
extern int  anofsreduction[];
extern int  anofrt[];
extern int  anofr2[];
extern int  anofsrt[];
extern void rulestatistics();/* int level */
extern char *RNAMES[];
extern int FSYMTABSIZE;
extern int RTABSIZE;

extern void timestart();
extern void timestop();
extern struct rusage before;
extern struct rusage after;
/* statistics */

/* table des arites */
extern struct fsym fsymtab[];

#endif 
// end RUNTIME

/* pour chaque fsym,
 * une liste de flags dit si le nieme argument est builtin
 */
extern unsigned *sprofil[];
extern unsigned *real_sprofil[];

#ifndef RUNTIME 
// begin RUNTIME

extern struct term * DOUBLECONSTLIST;
extern int Bins;

 /* for Meta_applies */
#ifdef META
#define META0LIST f1list
#define META1LIST f1list
#define META2LIST f2list
#define META3LIST f3list
extern struct term *META0LIST;
extern struct term *META1LIST;
extern struct term *META2LIST;
extern struct term *META3LIST;
#endif

#endif 
// end RUNTIME

#endif
