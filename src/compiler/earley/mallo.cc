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

#include "commondefs.h"


#define ADDDEBUG(n) 
#define SUBDEBUG(n) 

/* number of calls to malloc */
static int allo_calls=0;
/* number of calls to free */
static int fre_calls=0;

void *allo(unsigned n, unsigned s)
{
  void *p;

  if (n!=0)
    {
      ADDDEBUG(allo_calls);

      //p=intern_alloc(n*s);
      p=malloc(n*s);


      if (p==NULL)
	{
	  sterr << "\n\n[allo] sorry, no memory\n";
	  failexit();
	}
      return(p);
    }
  return(NULL);
}

void fre(void *p)
{ 
  if (p!=NULL)
    {
      ADDDEBUG(fre_calls);

      free((long*)p);
      //intern_free((long*)p);

    }
}

/* ******************************* */
/*       memory management         */
/* ******************************* */

#define MEMCHUNKSIZE 100000    /* size of chunks allocated by malloc */
#define MAXABORTED 100         /* maximal space left free in chunk */

#define scale sizeof(long)
#define memoround(n) if((n)%scale) (n)+=(scale-((n)%scale));

static char *actchunk = NULL;
static char *actchunkend = NULL;

#define FREELIST_SIZE 100
#define MAX_SIZE_STRUCT (scale*FREELIST_SIZE)

/*
 * Statistics
 */
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

#define MAXALLOCPLACE 100000
static long *alloc_table[MAXALLOCPLACE];
static int alloc_table_index = 0;
static int position;

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
  zone=(char*) malloc(taille);
  if (!zone) {
    printf("Echec du Valloc : pas assez de memoire disponible.\n");
    failexit();
  }
  return (zone);
}

char *allocator(long n)
{ 
  char *res;

  memoround(n);
  ADDDEBUG(allocator_calls);
  if (actchunk + (n+scale)  >= actchunkend) 
    {
      if ((n+scale)>MEMCHUNKSIZE)
	{
	  fprintf(stderr,"\n\n[allocator] memory block too big: %lu\n\n",n+scale);
	  failexit();
	}
      actchunk = (char *) malloc(MEMCHUNKSIZE);
      if (actchunk == NULL)
	{
	  fprintf(stderr,"\n\n[allocator] out of memory\n\n");
	  failexit();
	}
      actchunkend = actchunk + MEMCHUNKSIZE;
    }

  res = ADDSCALE(actchunk);
  actchunk += (n+scale);
  SIZE(res)=n; /* on peut ajouter +scale */


  return res;
}

char *intern_alloc(long size)
{
  char *res;

  memoround(size);
  if((size_t)size >= MAX_SIZE_STRUCT)
    {
      //fprintf(stderr,"intern_alloc --> Valloc(%d)\n",size+scale);
      res=Valloc(size+scale);
      res=ADDSCALE(res);
      SIZE(res)=size;
      goto fin;
    }

  ADDDEBUG(nb_alloc[size/scale]);
  if(freelist[size/scale] == NULL)
    {
      ADDDEBUG(nb_allocator[size/scale]);
      res=allocator(size);
      goto fin;
    }

  SUBDEBUG(length_freelist[size/scale]);
  res=(char*)freelist[size/scale];
  freelist[size/scale]= (long*) *(freelist[size/scale]);
fin:
  return res;
}

void intern_free(long *p)
{
  int size;

  size=SIZE(p);


  if((size_t)size >= MAX_SIZE_STRUCT)
    {
      ADDDEBUG(free_calls);
      free(TRUEADR(p));
    }
  else
    {
      ADDDEBUG(nb_free[size/scale]);
      ADDDEBUG(length_freelist[size/scale]);
      *p=(long) freelist[size/scale];
      freelist[size/scale]=p;
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
	  printf("freelist[%4lu]",i*scale);
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
  printf("allo_calls                  : %7d\n", allo_calls);
  printf("fre_calls                   : %7d\n", fre_calls);
  printf("\n\n");

}



/* ******************************* */
/*       allocations debugging     */
/* ******************************* */

int alloc_member(long *t)
{
  int i;
  for(i=0 ; i<alloc_table_index ; i++)
    if(alloc_table[i]==t)
      {
	position = i;
	return(1);
      }
  return(0);
}

void testalloc(long *t)
{ 
  if (alloc_member(t)) {
    fprintf(stderr,"two times allocated place t==%p\n\n",(void *)t); 
    failexit();
  }
  if (alloc_table_index >= MAXALLOCPLACE) {
    fprintf(stderr,"sorry alloc_table overflowed\n\n"); 
    failexit();
  }
  alloc_table[alloc_table_index++] = t;
}

void testfree(long *t)
{
  int i;
  if (!alloc_member(t))
    {
      fprintf(stderr,"two times freed the same place !!!\nt==%p\n",(void *)t); 
      fprintf(stderr,"position=%d\n",position);
      failexit();
    }
  /*
   * on decale tout d'un cran a gauche
   */
  for(i=position ; i<alloc_table_index-1 ; i++)
    alloc_table[i]=alloc_table[i+1];

  alloc_table_index--;
}


 
