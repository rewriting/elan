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
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
#include <stdio.h>
#include "gc.h"
#include "tools.h"
/*
 *	defines
 */

/*
 *	macros
 */
#define ASSERT(c, m)    if(!(c)) fatal("ASSERT failed: %s", (m))

#define EMALLOC(t)      ((t *) MALLOC(sizeof(t)))
#define EFREE(t)        FREE(t)
#define CALLOC(n, t)    ((t *) MALLOC(((unsigned) (n)) * sizeof(t)))
#define STRSAVE(s)      (strcpy((char *) MALLOC((unsigned) strlen(s)+1),s))
//#define REALLOC(p, n, t) ((t *) MREALLOC((void *) p, ((unsigned) (n)) * sizeof(t)))
