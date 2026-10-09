#ifndef _tools_h
#define _tools_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "gc.h"

// [pem: Sep  6 00]: 2*x pour corriger un bug du GC
#ifdef ATERM
//#define allocStable(x) AT_allocate(2*x)
#define allocStable(x) allocStablePointer(x)
#else
//#define allocStable(x) MALLOC(2*x)
#define allocStable(x) allocStablePointer(x)
#endif

#define GnotYetImplemented(s) {\
    fprintf(stderr,"%s: not yet implemented\n",s); exit(1); }

#ifndef strdup
#define strdup(dest,source) strcpy(dest=(char*)AMALLOC(1+strlen(source)),source)
#define strndup(dest,source,n) strncpy(dest=(char*)AMALLOC(1+n),source,n)
#endif

#ifdef DEBUG
#define Debug(nom,msg)\
if(nom) printf("\t " #nom " : %s",msg)
#else
#define Debug(nom,msg)
#endif

/*
 * Pour surveiller une adresse modifiee
 * ajouter les lignes suivantes
 *   STRANGE_ADDRESS = &(variable);
 *   printf("allocatedBug (%d)\n",STRANGE_ADDRESS);
 *   allocatedBug=1;
 */

extern int allocatedBug;
#ifdef DEBUG_STRANGE
extern int *STRANGE_ADDRESS;

#define VERIF_STRANGE_ADDRESS(x) if(allocatedBug==1) if((*(int*)STRANGE_ADDRESS)==(x)) { printf("STRANGE\n"); assert(0); } 
#else
#define VERIF_STRANGE_ADDRESS(x)
#endif

#ifdef DEBUG
#define Verif_void(objet,chaine) {\
  VERIF_STRANGE_ADDRESS(1)\
  if(objet==0)\
    {\
      printf("%s est a void\n",chaine);\
      assert(0);\
    }\
}
#else
#define Verif_void(objet,chaine)
#endif

// if(allocatedBug==1) if((*(int*)156372512)!=1) { assert(1); }\

#ifndef NULL
#define NULL 0
#endif

#ifndef DEEP
#define DEEP 1
#endif

#define IMALLOC(n) MALLOC(n)

#ifdef NOTMACRO
extern char* MALLOC(int n);
extern char* AMALLOC(int n);
#else
#ifdef DEBUG
#define MALLOC(n)  GC_MALLOC(n)
#define AMALLOC(n) GC_MALLOC(n)
#else
#define MALLOC(n)  GC_MALLOC(n)
#define AMALLOC(n) GC_MALLOC_ATOMIC(n)
#endif
#endif

#ifdef NOTMACRO
extern void FREE(char *t);
#else
#define FREE(p) /* do nothing */
#endif

#define IFREE(p) FREE((char*)p)
#define AFREE(p) FREE((char*)p)

//#define MREALLOC(p,n) GC_REALLOC(p,n)

extern void MsgErreur();
extern void indent(int deep);
extern void init_alloc();
extern char* Valloc();
extern char *allocator(int n);
extern char *intern_alloc(int size);
extern void intern_free(long *p);
extern void print_space_usage();

extern unsigned long cptMalloc;
extern unsigned long cptAMalloc;
extern unsigned long cptTermAlloc;
extern unsigned long cptTermacAlloc;
extern unsigned long cptCmpEqual;
extern unsigned long cptCmpTotal;
extern void globalStatistics();

#ifdef MEMORY_VERIFY
extern int alloc_member(long *t);
extern void testalloc(long *t);
extern void testfree(long *t);
#endif

extern char *intern_alloc2(int size, int mode);

#endif // #ifndef _tools_h
