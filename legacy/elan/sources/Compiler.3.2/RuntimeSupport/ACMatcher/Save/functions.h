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

