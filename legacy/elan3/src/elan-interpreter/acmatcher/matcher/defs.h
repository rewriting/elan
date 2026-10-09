/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
#include <stdio.h>
//#include "minitools.h"

#define ASSERT(c, m)    if(!(c)) fatal("ASSERT failed: %s", (m))

#define MALLOC(t)       ((t *) salloc(sizeof(t)))
#define CALLOC(n, t)    ((t *) salloc(((unsigned) (n)) * sizeof(t)))
#define FREE(t)         (sfree((void *) t))
#define STRSAVE(s)      (strcpy((char *) salloc((unsigned) strlen(s)+1),s))
#define REALLOC(p, n, t) ((t *) srealloc((void *) p, ((unsigned) (n)) * sizeof(t)))

/*
#define MALLOC(t)       ((t *) intern_alloc(sizeof(t)))
#define CALLOC(n, t)    ((t *) intern_alloc(((unsigned) (n)) * sizeof(t)))
#define FREE(t)         (intern_free((long *) t))
#define STRSAVE(s)      (strcpy((char *) intern_alloc((unsigned) strlen(s)+1),s))
#define REALLOC(p, n, t) ((t *) \
   ADDSCALE(srealloc((void *) SUBSCALE(p), scale + ((unsigned) (n)) * sizeof(t)))) ;\
   SIZE(p)=MAGICNUMBER;
*/
