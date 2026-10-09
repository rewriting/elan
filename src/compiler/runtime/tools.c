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
#include <stdlib.h>
#include "tools.h"


#define scale sizeof(long)
#define memoround(n) if((n)%scale) (n)+=(scale-((n)%scale));

void MsgErreur(char *message)
{
  printf("%s",message);
}

void indent(int deep) {
  int i;
  printf("|");
  for(i=0 ; i<deep ; i++)
    printf(" ");
}


/* ******************************* */
/*       memory management         */
/* ******************************* */

#define MEMCHUNKSIZE 100000    /* size of chunks allocated by malloc */

#define MAXABORTED 100         /* maximal space left free in chunk */


#define FREELIST_SIZE 4096
#define MAX_SIZE_STRUCT (scale*FREELIST_SIZE)

static char *actchunk=NULL;
static char *actchunkend=NULL;


/*
 * Statistics
 */
static int total_mem;
/* number of calls to malloc */
static int malloc_calls;
/* number of calls to free */
static int free_calls;
/* number of calls to allocator */
static int allocator_calls;

/* how many times a type has been allocated */
static int  nb_alloc[FREELIST_SIZE];
/* how many times a type has been reclaimed */
static int  nb_free[FREELIST_SIZE];
/* how many times allocator() is called  */
static int  nb_allocator[FREELIST_SIZE]; 
/* the lengths of freelist[] lists */
static int  length_freelist[FREELIST_SIZE]; 
/* the available lists */
static long *freelist[FREELIST_SIZE];

#ifdef DEBUG
#define ADDDEBUG(n) (n)++;
#define SUBDEBUG(n) (n)--;
#else
#define ADDDEBUG(n) 
#define SUBDEBUG(n) 
#endif

#define TRUEADR(adr) ((long*)(((char*)adr)-scale))
#define SIZE(adr)    (*(TRUEADR(adr)))
#define ADDSCALE(adr) ((char*)(((char*)adr)+scale))

void init_alloc() {
  int i;
  malloc_calls=0;
  free_calls=0;
  allocator_calls=0;
  for(i=0 ; i<FREELIST_SIZE ; i++) {
    nb_alloc[i]=0;
    nb_free[i]=0;
    nb_allocator[i]=0;
    length_freelist[i]=0;
    freelist[i]=NULL;
  }
}

char *Valloc(int taille) {
  char *zone;
  ADDDEBUG(malloc_calls);
#ifdef DEBUG
  if(taille <= 0) {
    printf("taille = %d\n",taille);
    exit(1);
  }
#endif
  zone=(char*) malloc(taille);
  if (zone==NULL) {
    printf("Echec du Valloc : pas assez de memoire disponible.\n");
    exit(0);
  }
  return (zone);
}

char *allocator(int size) { 
  char *res;
  int indice = size/scale;

  //memoround(size); /* size est deja aligne' */
  ADDDEBUG(allocator_calls);
  if (actchunk + (size+scale)  >= actchunkend) {
    /*
      // On peut ajouter un magic number
      if (n>MAXABORTED)
      {
      res = (char*) malloc(n);
      ADDDEBUG(malloc_calls);
      if (res == NULL)
      {
      fprintf(stderr,"\n\n[allocator] out of memory\n\n");
      exit(1);
      }
      return(res);
      } 
      */
    
    if ((size+scale)>MEMCHUNKSIZE) {
      fprintf(stderr,"\n\n[allocator] memory block too big\n\n");
      exit(1);
    }
    actchunk = (char *) Valloc(MEMCHUNKSIZE);
    if(actchunk == NULL) {
      fprintf(stderr,"\n\n[allocator] out of memory\n\n");
      exit(1);
    }
    actchunkend = actchunk + MEMCHUNKSIZE;
  }

  res = ADDSCALE(actchunk);
  actchunk += (size+scale);

  SIZE(res)=size; /* on peut ajouter +scale */
  return res;
}

char *intern_alloc(int size) {
  char *res;
  int indice;
  

  memoround(size);
  indice=size/scale;

  if(size >= MAX_SIZE_STRUCT) {
    //fprintf(stderr,"intern_alloc --> Valloc(%d)\n",size+scale);
    //ADDDEBUG(malloc_calls);
    res=Valloc(size+scale);
    res+=scale;
    SIZE(res)=size;
    goto fin;
  }
  ADDDEBUG(nb_alloc[indice]);
  if(freelist[indice] == NULL) {
    ADDDEBUG(nb_allocator[indice]);
    res=allocator(size);
    goto fin;
  }

  SUBDEBUG(length_freelist[indice]);
  res=(char*)freelist[indice];
  freelist[indice]= (long*) *(freelist[indice]);
fin:
#ifdef DEBUG
  if(SIZE(res)!=size)
    {
      fprintf(stderr,"\n\n[intern alloc] error \n\n");
      fprintf(stderr,"size=%d\tmysize=%d\n",size,SIZE(res));
      exit(1);
    }
  //printf("[intern_alloc] (%d)\tsize=%d\n",res,SIZE(res));
#endif


  return res;
}

void intern_free(long *p) {
  int size;

  size=SIZE(p);
  

  if(size >= MAX_SIZE_STRUCT) {
    ADDDEBUG(free_calls);
    free(TRUEADR(p));
  } else {
    int indice=size/scale;
    ADDDEBUG(nb_free[indice]);
    ADDDEBUG(length_freelist[indice]);
    *p = (long) freelist[indice];
    freelist[indice]=p;
  }
}


void print_space_usage()
{
  int  total_tp = 0;
  int  i;

  printf("                 gets   frees     diff   allocator  lengths\n");
  for (i=0; i<FREELIST_SIZE; i++)
    {
      if(freelist[i])
	{
	  printf("freelist[%4d]",i*scale);
	  printf("%7d ",   nb_alloc[i]);
	  printf("%7d ",   nb_free[i]);
	  printf("%7d ",   nb_alloc[i] - nb_free[i]);
	  printf("%8d ",   nb_allocator[i]);
	  printf("%10d \n", length_freelist[i]);
	  total_tp += nb_allocator[i];
	}
    }
  printf("\n");
  printf("allocator_calls             : %7d\n", allocator_calls);
  printf("allocator_calls listed above: %7d\n", total_tp);
  printf("tp calls unaccounted for    : %7d\n", allocator_calls - total_tp);
  printf("malloc_calls                : %7d\n", malloc_calls);
  printf("free_calls                  : %7d\n", free_calls);
  printf("\n\n");
}



/* ******************************* */
/*       allocations debugging     */
/* ******************************* */





char *intern_alloc2(int size, int mode) {
  char *res;
  int indice;
  

  memoround(size);
  indice=size/scale;

  if(size >= MAX_SIZE_STRUCT) {
    //fprintf(stderr,"intern_alloc --> Valloc(%d)\n",size+scale);
    //ADDDEBUG(malloc_calls);
    res=Valloc(size+scale);
    res+=scale;
    SIZE(res)=size;
    goto fin;
  }
  ADDDEBUG(nb_alloc[indice]);
  if(freelist[indice] == NULL) {
    ADDDEBUG(nb_allocator[indice]);
    if(mode==0)
      res=allocator(size);
    else
      res=NULL;
    goto fin;
  }

  SUBDEBUG(length_freelist[indice]);
  res=(char*)freelist[indice];
  freelist[indice]= (long*) *(freelist[indice]);
fin:
#ifdef DEBUG
  if(SIZE(res)!=size)
    {
      fprintf(stderr,"\n\n[intern alloc] error \n\n");
      fprintf(stderr,"size=%d\tmysize=%d\n",size,SIZE(res));
      exit(1);
    }
  //printf("[intern_alloc] (%d)\tsize=%d\n",res,SIZE(res));
#endif


  return res;
}

unsigned long cptMalloc=0;
unsigned long cptAMalloc=0;
unsigned long cptTermAlloc=0;
unsigned long cptTermacAlloc=0;
unsigned long cptCmpEqual=0;
unsigned long cptCmpTotal=0;
void globalStatistics() {
#ifdef DEBUG
  printf("\nStatistics:\n");
  printf("\tcptMalloc      = %u bytes (%f Mb)\n",cptMalloc,
         ((double)cptMalloc)/(1024*1024));
  printf("\tcptAMalloc     = %u bytes (%f Mb)\n",cptAMalloc,
         ((double)cptAMalloc)/(1024*1024));
  printf("\tcptTermAlloc   = %u\n",cptTermAlloc);
  printf("\tcptTermacAlloc = %u\n",cptTermacAlloc);
  printf("\tcptCmpEqual    = %u\n",cptCmpEqual);
  printf("\tcptCmpTotal    = %u\n",cptCmpTotal);
  printf("\n");
#endif
}

