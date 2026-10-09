/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *	Types for pure problem data structure
 */

/*
 *	Main structure
 */
typedef struct pure_problem_struct {
  int n_pure;			/* number of pure equations */
  int n_var;			/* number of variable */
  int n_term;			/* number of terms */
  int *top_sym;			/* AC top symbol array */
  struct var_header_struct *var_tab;	/* variable table */
  struct term_header_struct *term_tab;	/* term table */
} PURE_PROBLEM;

/*
 *	Structure for each variable
 */
typedef struct var_header_struct {
  int var;			/* variable number */
  int *vmult;			/* table of multiplicities */
  int max_mult;			/* maximum multiplicity (for sorting on) */
  int owner;			/* AC function symbol owning this variable */
  union {
    struct {
      int cur_size;		/* size of current assignment */
      int max_size;		/* size of maximum assignment */
      struct assign_struct *assign;	/* table of assignments */
    } o;
    struct {
      int cur_ass;
    } s;
  } ass;
} VAR_HEADER;

/*
 *	Special value for var field
 */
#define DEAD		(-1)

/*
 *	Special values for owner field
 */
#define SHARED		(-2)
#define UNUSED		(-1)

/*
 *	Structure for assignment of term to owned variable
 */
typedef struct assign_struct {
  int cur;			/* multiplicity of current assignment */
  int max;			/* maximum multiplicity of assignment */
  int sum_prev;			/* sum of previous multiplicities */
} ASSIGN;

/*
 *	Structure for each term
 */
typedef struct term_header_struct {
  TERM *term;			/* pointer to term tree */
  int *tmult;			/* table of multiplicities */
  struct index_list_struct *subterms;	/* list of indices for subterms */
} TERM_HEADER;

/*
 *	List of indices into term_tab for dealing with AC denesting
 *	on shared variables
 */
typedef struct index_list_struct {
  int index;			/* index of term corresponding to a subterm */
  int smult;			/* multiplicity of that term in original term */
  struct index_list_struct *next_index;	/* pointer to next list element */
} INDEX_LIST;

