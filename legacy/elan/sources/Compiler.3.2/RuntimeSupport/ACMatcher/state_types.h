/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *	Types for current state of matching problem
 */

/*
 *	Main match object
 */
typedef struct match_object_struct {
  TERM *orig_pat;		/* Original pattern */
  TERM *orig_sub;		/* Original subject */
  int total_var;		/* Total number of variables */
  FREE_PROBLEM *free_prob;		/* pointer to free problem structure */
  struct binding_struct *bind_tab;	/* table of variable bindings */
  struct pure_list_struct *pure_stack;	/* stack of pure AC equations */
  PURE_PROBLEM *pure_prob;	/* pointer to pure problem structure */
  BOOL first;			/* first match extracted ? */
} MATCH_OBJECT;

/*
 *	Variable binding
 */
typedef struct binding_struct {
  TERM *value;			/* term bound variable is bound to */
  int n_times;			/* number of times bound to this term */
} BINDING;

/*
 *	Linked list of pure AC equations
 */
typedef struct pure_list_struct {
  int ac_sym;                                   /* AC top symbol */
  struct ac_list_struct *unused;                /* unmatchable subjects */
  struct var_list_struct *variables;            /* variables under top sym */
  struct pure_list_struct *next_pure;           /* next_pure equation */
} PURE_LIST;

