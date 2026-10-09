#include <stdlib.h>
#include "tools.h"

void MsgErreur(message)
 char *message;
{
  printf("%s",message);
}


/* ******************************* */
/*       memory management         */
/* ******************************* */

#define MEMCHUNKSIZE 100000    /* size of chunks allocated by malloc */
#define MAXABORTED 100         /* maximal space left free in chunk */

#define MAGICNUMBER ((long)0x123456789)

#define scale sizeof(long)
#define memoround(n) if((n)%scale) (n)+=(scale-((n)%scale));

static char *actchunk = NULL;
static char *actchunkend = NULL;

#define FREELIST_SIZE 100
#define MAX_SIZE_STRUCT (scale*FREELIST_SIZE)

#ifdef DEBUG
#define MAXALLOCPLACE 100000
static long *alloc_table[MAXALLOCPLACE];
static int alloc_table_index = 0;
static int position;
#endif

/*
 * Statistics
 */
/* static int total_mem; */
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

void init_alloc()
{
  int i;
  malloc_calls=0;
  free_calls=0;
  allocator_calls=0;
  for(i=0 ; i<FREELIST_SIZE ; i++)
    {
      nb_alloc[i]=0;
      nb_free[i]=0;
      nb_allocator[i]=0;
      length_freelist[i]=0;
      freelist[i]=NULL;
    }
#ifdef DEBUG
  for(i=0 ; i<MAXALLOCPLACE ; i++)
    alloc_table[i]=NULL;
#endif
}

char *Valloc(int taille)
{
  char *zone;
  ADDDEBUG(malloc_calls);
#ifdef DEBUG
  if(taille <= 0)
    {
      printf("taille = %d\n",taille);
      exit(1);
    }
#endif

  //fprintf(stderr,"debut malloc(%d)\n",taille);
  zone=(char*) malloc(taille);
  //fprintf(stderr,"fin malloc\n");

  if (zone==NULL)
    {
      printf("Echec du Valloc : pas assez de memoire disponible.\n");
      exit(0);
    }
  return (zone);
}

char *allocator(int n)
{ 
  char *res;

  memoround(n);
  ADDDEBUG(allocator_calls);
  if (actchunk + (n+scale)  >= actchunkend) 
    {
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

      if ((n+scale)>MEMCHUNKSIZE)
	{
	  fprintf(stderr,"\n\n[allocator] memory block too big\n\n");
	  exit(1);
	}

      actchunk = (char *) Valloc(MEMCHUNKSIZE);
      if (actchunk == NULL)
	{
	  fprintf(stderr,"\n\n[allocator] out of memory\n\n");
	  exit(1);
	}
      actchunkend = actchunk + MEMCHUNKSIZE;
    }

  res = ADDSCALE(actchunk);
  actchunk += (n+scale);
  SIZE(res)=n; /* on peut ajouter +scale */

  return res;
}

char *intern_alloc(int size)
{
  char *res;
  int indice;

#ifdef PURIFY
  //ADDDEBUG(malloc_calls);
  res=(char*)Valloc(size);
  testalloc((long*)res);
  return res;
#endif

  memoround(size);
  indice=size/scale;

  if(size >= MAX_SIZE_STRUCT)
    {
      //fprintf(stderr,"intern_alloc --> Valloc(%d)\n",size+scale);
      //ADDDEBUG(malloc_calls);
      res=Valloc(size+scale);
      res+=scale;
      SIZE(res)=size;
      goto fin;
    }
  ADDDEBUG(nb_alloc[indice]);
  if(freelist[indice] == NULL)
    {
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
  testalloc((long*)res);
#endif
  return res;
}

void intern_free(long *p)
{
  int size;

#ifdef PURIFY
  ADDDEBUG(free_calls);
  testfree(p);
  free(p);
  return;
#endif
  size=SIZE(p);

#ifdef DEBUG
  testfree(p);
  //printf("[intern_free] (%d)\tmysize=%d\n",p,size);
#endif
  // on suppose MAGICNUMBER > MAX_SIZE_STRUCT
  if(size >= MAX_SIZE_STRUCT)
    {
      ADDDEBUG(free_calls);
      free(TRUEADR(p));
    }
  else
    {
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
  int  i; /* , final; */

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
  printf("\n\n");
}



/* ******************************* */
/*       allocations debugging     */
/* ******************************* */

#ifdef DEBUG
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
