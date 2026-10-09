/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/

#include "pure_types.h"
#include "free_types.h"
#include "state_types.h"

/*
 *	Interface functions
 */
void *salloc(unsigned n);
void *srealloc(void *old, unsigned size);
void sfree(void *p);
void fatal(char *s, char *a);
/*
 *	flatten.c functions
 */
void flatten(TERM *t);
void ac_compress(TERM *t);
int term_cmp(TERM *t, TERM *t2);
void destroy_term(TERM *t);
/*
 *	build_match.c
 */
void *build_match(TERM *p, TERM *s, int tot_var);
BOOL extract_match(void *vm, TERM **assignment);
void destroy_match(void *vm);
/*
 *	build_free.c
 */
void assert_bindings(BINDING *bind_tab, BIND_LIST *b);
void retract_bindings(BINDING *bind_tab, BIND_LIST *b);
BOOL build_free(MATCH_OBJECT *obj, TERM *p, TERM *s, FREE_PROBLEM **dest);
void destroy_free(FREE_PROBLEM *fp);
/*
 *	solve_free.c
 */
BOOL solve_free(MATCH_OBJECT *obj, FREE_PROBLEM *fp, BOOL reset);
/*
 *	build_pure.c
 */
BOOL build_pure(MATCH_OBJECT *obj);
void destroy_pure(PURE_PROBLEM *prob);
/*
 *	solve_pure.c
 */
BOOL solve_pure(PURE_PROBLEM *prob, BOOL reset);
