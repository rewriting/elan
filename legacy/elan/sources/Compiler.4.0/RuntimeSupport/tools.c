#include <stdlib.h>
#include "tools.h"


#define scale sizeof(long)
#define memoround(n) if((n)%scale) (n)+=(scale-((n)%scale));

void MsgErreur(message)
 char *message;
{
  printf("%s",message);
}

void indent(int deep) {
  int i;
  printf("|");
  for(i=0 ; i<deep ; i++)
    printf(" ");
}

#ifdef NOTMACRO

//#define CHUNKSIZE 65536
#define CHUNKSIZE 96
static char *freeArea=NULL;
static int freeSize=0;
static int nbChunk=0;

static long GC_counter=0;
void FREE(char *t) {
#ifdef ATERM
    //free(t);
#else
// do nothing
#endif
}

char *MALLOC(int n) {
  char *res;

    //printf("GC_counter = %d\n",GC_counter++);
#ifdef DEBUG
  cptMalloc+=n;
//  printf("gmalloc %d\n",n);
#endif
    //return GC_debug_malloc(n,"malloc",0);

  VERIF_STRANGE_ADDRESS(1);

    //return malloc(n);
  

#ifdef ATERM
    //printf("Warning: MALLOC\n");
  res = GC_malloc(n);
#else
  res = GC_malloc(n);
#endif
  return res;

    /*
    //memoround(n);
  if(n>freeSize) {
    if(n>CHUNKSIZE) {
      return GC_malloc(n);
    } else {
        //printf("chunk no %d\n",++nbChunk);
      
      freeArea=GC_malloc(CHUNKSIZE);
      freeSize=CHUNKSIZE;
    }
  }
  res=freeArea;
  freeSize-=n;
  freeArea+=n;
  return res;
    */
}

char *AMALLOC(int n) {
  char *res;
  
  //  printf("GC_counter = %d\n",GC_counter++);
#ifdef DEBUG
  cptAMalloc+=n;
//  printf("amalloc %d\n",n);
#endif
    //  return GC_debug_malloc(n,"amalloc",0);

  VERIF_STRANGE_ADDRESS(1);

    //return malloc(n);
  
#ifdef ATERM
    //printf("Warning: AMALLOC\n");
  res = GC_malloc(n);
#else
  res = GC_malloc(n);
#endif
  return res;
    /*
  if(n>freeSize) {
    if(n>CHUNKSIZE) {
      return GC_malloc(n);
    } else {
      freeArea=GC_malloc(CHUNKSIZE);
      freeSize=CHUNKSIZE;
    }
  }
  res=freeArea;
  freeSize-=n;
  freeArea+=n;
  return res;
    */
}
#endif

/* ******************************* */
/*       memory management         */
/* ******************************* */

#ifdef NEWGC
#define MEMCHUNKSIZE 4096      /* size of chunks allocated by malloc */
#else
#define MEMCHUNKSIZE 100000    /* size of chunks allocated by malloc */
#endif

#define MAXABORTED 100         /* maximal space left free in chunk */


#define FREELIST_SIZE 4096
#define MAX_SIZE_STRUCT (scale*FREELIST_SIZE)

#ifdef NEWGC
static char *actchunk[FREELIST_SIZE];
static char *actchunkend[FREELIST_SIZE];
#else
static char *actchunk=NULL;
static char *actchunkend=NULL;
#endif

#ifdef MEMORY_VERIFY
#define MAXALLOCPLACE 100000
static long *alloc_table[MAXALLOCPLACE];
static int alloc_table_index = 0;
static int position;
#endif

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
#ifdef NEWGC
    actchunk[i]=NULL;
    actchunkend[i]=NULL;
#endif
  }
#ifdef MEMORY_VERIFY
  for(i=0 ; i<MAXALLOCPLACE ; i++)
    alloc_table[i]=NULL;
#endif
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
#ifdef NEWGC
  if (actchunk[indice] + (size+scale)  >= actchunkend[indice]) {
#else
  if (actchunk + (size+scale)  >= actchunkend) {
#endif
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
#ifdef NEWGC
    actchunk[indice] = (char *) Valloc(MEMCHUNKSIZE);
    if(actchunk[indice] == NULL) {
      fprintf(stderr,"\n\n[allocator] out of memory\n\n");
      exit(1);
    }
    actchunkend[indice] = actchunk[indice] + MEMCHUNKSIZE;
  }

  res = ADDSCALE(actchunk[indice]);
  actchunk[indice] += (size+scale);
#else
    actchunk = (char *) Valloc(MEMCHUNKSIZE);
    if(actchunk == NULL) {
      fprintf(stderr,"\n\n[allocator] out of memory\n\n");
      exit(1);
    }
    actchunkend = actchunk + MEMCHUNKSIZE;
  }

  res = ADDSCALE(actchunk);
  actchunk += (size+scale);
#endif

  SIZE(res)=size; /* on peut ajouter +scale */
  return res;
}

char *intern_alloc(int size) {
  char *res;
  int indice;
  
#ifdef PURIFY
  //ADDDEBUG(malloc_calls);
  res=(char*)Valloc(size);
  //testalloc((long*)res);
  return res;
#endif

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

#ifdef MEMORY_VERIFY
  testalloc((long*)res);
#endif

  return res;
}

void intern_free(long *p) {
  int size;

#ifdef PURIFY
  ADDDEBUG(free_calls);
  //testfree(p);
  free(p);
  return;
#endif
  size=SIZE(p);
  
#ifdef MEMORY_VERIFY
  testfree(p);
  //printf("[intern_free] (%d)\tmysize=%d\n",p,size);
#endif

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

#ifdef MEMORY_VERIFY
int alloc_member(long *t)
{
  int i;
  for(i=0 ; i<alloc_table_index ; i++)
    if(alloc_table[i] == t)
      {
	position = i;
	return(1);
      }
  return(0);
}

void testalloc(long *t)
{ 
  if (alloc_member(t))
    {
      fprintf(stderr,"two times allocated place t==%d\n\n",t); 
      exit(1);
    }
  if (alloc_table_index >= MAXALLOCPLACE)
    {
      fprintf(stderr,"sorry alloc_table overflowed\n\n"); 
      exit(1);
    }
  alloc_table[alloc_table_index++] = t;
}

void testfree(long *t)
{
  int i;
  if (!alloc_member(t))
    {
      fprintf(stderr,"two times freed the same place !!!\nt==%d\n",t); 
      exit(1);
    }
  /*
   * on decale tout d'un cran a gauche
   */
  for(i=position ; i<alloc_table_index-1 ; i++ )
    alloc_table[i] = alloc_table[i+1];

  alloc_table_index--;
}

#endif




char *intern_alloc2(int size, int mode) {
  char *res;
  int indice;
  
#ifdef PURIFY
  //ADDDEBUG(malloc_calls);
  res=(char*)Valloc(size);
  //testalloc((long*)res);
  return res;
#endif

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

#ifdef MEMORY_VERIFY
  testalloc((long*)res);
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

