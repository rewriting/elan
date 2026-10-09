/*
  
    ELAN

    Copyright (C) 1994-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
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

*/
/*
 *	defines
 */
#define SYM_TAB_SIZE	1000			/* symbol table size */
#ifndef NULL
#define NULL		0			/* null pointer */
#endif

/*
 *	macros
 */
#define MALLOC(t)       ((t *) salloc(sizeof(t)))
#define CALLOC(n, t)    ((t *) salloc((unsigned) (n) * sizeof(t)))
#define FREE(t)         (sfree((void *) t))
#define STRSAVE(s)      (strcpy((char *) salloc((unsigned) strlen(s)+1),s))
#define ASSERT(c, m)    if(!(c)) fatal("ASSERT failed: %s", (m))
#define REALLOC(p, n, t) ((t *) srealloc((void *) p, \
				(unsigned) (n) * sizeof(t)))

