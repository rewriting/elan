#ifndef _tools_h
#define _tools_h
#include <stdio.h>

#ifndef strdup
#define strdup(dest,source) strcpy(dest=(char*)Valloc(1+strlen(source)),source)
#define strndup(dest,source,n) strncpy(dest=(char*)Valloc(1+n),source,n)
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

#define MALLOC(n) intern_alloc(n)
#define FREE(p) intern_free((long*)(p))

void MsgErreur(char *message);

void init_alloc(void);
char* Valloc(int taille);
char *allocator(int n);
char *intern_alloc(int size);
void intern_free(long *p);
void print_space_usage(void);

int alloc_member(long *t);
void testalloc(long *t);
void testfree(long *t);


#endif
