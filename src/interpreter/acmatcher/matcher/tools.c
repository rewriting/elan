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

#define ADDDEBUG(n) 
#define SUBDEBUG(n) 

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
}

char *Valloc(int taille)
{
  char *zone;
  ADDDEBUG(malloc_calls);

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


  memoround(size);
  indice=size/scale;

  if((size_t)size >= MAX_SIZE_STRUCT)
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
  return res;
}

void intern_free(long *p)
{
  int size;

  size=SIZE(p);

  // on suppose MAGICNUMBER > MAX_SIZE_STRUCT
  if((size_t)size >= MAX_SIZE_STRUCT)
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
	  printf("freelist[%4d]",(int)(i*scale));
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

