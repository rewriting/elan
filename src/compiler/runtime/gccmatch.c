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
/*
		(c) 	Marian Vittek, 1994
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/

/* this file provides a g++ interface for ACmatching written in C */


/*
#include "externs.h"
*/
#include "sym_types.h"
#include "defs.h"
#include "term_types.h"
#include "free_types.h"
#include "pure_types.h"
#include "state_types.h"
#include "functions.h"
#include <stdio.h>
#include <stdlib.h>

void *salloc(unsigned n)
{
  char *t = (char*) malloc(n);
  //printf("salloc size = %d\n",n);

  printf("salloc should not be used\n");
  exit(1);

  if(t == NULL) {
    printf("salloc size = %d\n",n);
    fatal("salloc(): out of memory", "");
  }
  return((void *) t);
}

void *srealloc(void *old, unsigned size)
{
  char *t = realloc((char *) old, size);

  printf("srealloc should not be used\n");
  exit(1);

  if(t == NULL) {
    printf("srealloc size = %d\n",size);
    fatal("srealloc(): out of memory", "");
  }
  return((void *) t);
}

void sfree(void *p)
{
  printf("sfree should not be used\n");
  exit(1);

  free((char *) p);
}


void fatal(char *s, char *a)
{
  fprintf(stderr,"[error] ACmatch error ");
  if(*a)
    (void) fprintf(stderr, s, a);
  else
    (void) fprintf(stderr, s);
  (void) fprintf(stderr,"\n");
  exit(1);
}

