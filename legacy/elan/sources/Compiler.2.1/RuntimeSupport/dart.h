#ifndef _dart_h
#define _dart_h

/*
 * constantes de fsymtab
 */  
#define code_empty   1
#define code_set     2
#define code_p1      3
#define code_p2      4
#define code_m2      5

#ifdef PEANO
#define code_o       10
#define code_s       11
#define code_plus    12
#define code_mult    13
#endif

/*
 * Constantes d'execution
 */
extern int trace;
extern int onfterm;
extern long rewrite_step;
extern int indentlevel;
extern struct term *const_empty;

#define MAX_TERM_SIZE 1000 // nb max du sous-termes d'un symbole AC
#define MAX_CBG_SIZE  100  // nb max de patterns dans un CBG

#define addindent() indentlevel++;
#define subindent() indentlevel--;

extern int next_pe_extract(int pos,int E[], int sol[],int multiplicity);
extern int next_pe_extract2(int total, int pos,int E[], int sol[],int multiplicity);
extern struct term *rest_extract(struct term *t, match_state *ms);
extern void extract_xy_from_pe(struct term *t, int *E, int *sol,
			       int multiplicity,
			       struct term **ptr_list_x,
			       struct term **ptr_list_y);

extern void indent();

extern void substitution_build(struct term *t, match_state *ms,
			       int nb_variable, struct term *substitution[],
			       int nb_variable_ac,
			       void (*variable_extract)()
			       );


#ifdef PEANO
extern struct term *fun_plus(struct term *t1, struct term *t2);
extern struct term *fun_mult(struct term *t1, struct term *t2);
#endif

#endif
