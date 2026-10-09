#ifndef _tools_h
#define _tools_h
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef GCMEM
#include "gc.h"
#endif

#ifndef strdup
#define strdup(dest,source) strcpy(dest=(char*)GC_MALLOC_ATOMIC(1+strlen(source)),source)
#define strndup(dest,source,n) strncpy(dest=(char*)GC_MALLOC_ATOMIC(1+n),source,n)
#endif

#ifdef DEBUG
#define Debug(nom,msg)\
if(nom) printf("\t " #nom " : %s",msg)
#else
#define Debug(nom,msg)
#endif

#ifdef DEBUG
#define Verif_void(objet,chaine)\
  if(objet==0)\
    {\
      printf("%s est a void\n",chaine);\
      exit(1);\
    }
#else
#define Verif_void(objet,chaine)
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef DEEP
#define DEEP 1
#endif


#ifdef NOTMACRO
extern char* MALLOC(int n);
#else
#ifdef DEBUG
/* on suppose que (cptAlloc+=n)-cptAlloc = 0 */
//#define MALLOC(n) GC_DEBUG_MALLOC(n + (cptAlloc+=n) - cptAlloc)
#define MALLOC(n) GC_MALLOC(n)
#else
#define MALLOC(n) GC_MALLOC(n)
#endif
#endif
#define IMALLOC(n) MALLOC(n)

#ifdef DEBUG
//#define AMALLOC(n) GC_DEBUG_MALLOC_ATOMIC(n + (cptAlloc+=n) - cptAlloc)
#define AMALLOC(n) GC_MALLOC_ATOMIC(n)
#else
#define AMALLOC(n) GC_MALLOC_ATOMIC(n)
#endif

#define MREALLOC(p,n) GC_REALLOC(p,n)
#define FREE(p) /* do nothing */
#define IFREE(p) FREE(p)

extern void MsgErreur();
extern void indent(int deep);
extern void init_alloc();
extern char* Valloc();
extern char *allocator(int n);
extern char *intern_alloc(int size);
extern void intern_free(long *p);
extern void print_space_usage();

extern unsigned long cptAlloc;
extern unsigned long cptTermAlloc;
extern void globalStatistics();

#ifdef MEMORY_VERIFY
extern int alloc_member(long *t);
extern void testalloc(long *t);
extern void testfree(long *t);
#endif

extern char *intern_alloc2(int size, int mode);

#endif // #ifndef _tools_h
