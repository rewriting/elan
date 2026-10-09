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
extern void *salloc();
extern void *srealloc();
extern void sfree();
extern void fatal();
/*
 *	flatten.c functions
 */
extern void flatten();
extern void ac_compress();
extern int eker_term_cmp();
extern void destroy_term();
extern void ac_sort();
/*
 *	build_match.c
 */
extern void *build_match();
extern BOOL extract_match();
extern void destroy_match();
extern void assert_bindings();
extern void retract_bindings();
/*
 *	build_free.c
 */
extern BOOL build_free();
extern void destroy_free();
/*
 *	solve_free.c
 */
extern BOOL solve_free();
/*
 *	build_pure.c
 */
extern BOOL build_pure();
extern void destroy_pure();
/*
 *	solve_pure.c
 */
extern BOOL solve_pure();

