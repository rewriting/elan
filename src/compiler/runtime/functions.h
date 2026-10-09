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
/*
 *	Interface functions
 */
extern void *salloc(unsigned n);
extern void *srealloc(void *old, unsigned size);
extern void sfree(void *p);
extern void fatal(char *s, char *a);
/*
 *	flatten.c functions
 */
extern void flatten(TERM *t);
extern void ac_compress(TERM *t);
extern int eker_term_cmp(TERM *t, TERM *t2);
extern void destroy_term(TERM *t);
extern void ac_sort(TERM *t);
/*
 *	build_match.c
 */
extern void *build_match(TERM *p, TERM *s, int tot_var);
extern BOOL extract_match(void *vm, TERM *assignment[]);
extern void destroy_match(void *vm);
extern void assert_bindings(BINDING bind_tab[], BIND_LIST *b);
extern void retract_bindings(BINDING bind_tab[], BIND_LIST *b);
/*
 *	build_free.c
 */
extern BOOL build_free(MATCH_OBJECT *obj,TERM *p,TERM *s,FREE_PROBLEM **dest);
extern void destroy_free(FREE_PROBLEM *fp);
/*
 *	solve_free.c
 */
extern BOOL solve_free(MATCH_OBJECT *obj, FREE_PROBLEM *fp, BOOL reset);
/*
 *	build_pure.c
 */
extern BOOL build_pure(MATCH_OBJECT *obj);
extern void destroy_pure(PURE_PROBLEM *prob);
/*
 *	solve_pure.c
 */
extern BOOL solve_pure(PURE_PROBLEM *prob, BOOL reset);

