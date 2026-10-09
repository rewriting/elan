/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *	Types for free AC matching problem data structure
 */

/*
 *	Structure for free AC matching problem
 */
typedef struct free_problem_struct {
  struct bind_list_struct *bindings;		/* list of variable bindings */
  struct ac_prob_list_struct *subproblems;	/* list of subproblems */
  struct ac_prob_list_struct *subproblems_tail; /* tail of subproblems list */
} FREE_PROBLEM;

/*
 *	Linked list of bound variables
 */
typedef struct bind_list_struct {
  int var_num;					/* variable's number */
  struct term_struct *binding;			/* term that it's bound to */
  struct bind_list_struct *next_bound;		/* next bound variable */
} BIND_LIST;

/*
 *	Linked list of subproblems with AC top symbols
 */
typedef struct ac_prob_list_struct {
  int ac_sym;					/* AC top symbol */
  struct ac_list_struct *unused;		/* unmatchable subjects */
  struct var_list_struct *variables;		/* variables under top sym */
  struct graph_list_struct *graph_problems;	/* list of graph problems */
  struct graph_list_struct *graph_problems_tail;/* tail of graph list */
  struct ac_prob_list_struct *next_prob;	/* next subproblem */
  struct ac_prob_list_struct *prev_prob;	/* previous subproblem */
} AC_PROB_LIST;

/*
 *	Linked list of variable with multiplicities
 */
typedef struct var_list_struct {
  int var_num;					/* variable's number */
  int var_mult;					/* variable's multiplicity */
  struct var_list_struct *next_var;		/* next variable */
} VAR_LIST;

/*
 *	Linked list of graph problems
 */
typedef struct graph_list_struct {
  int n_patterns;				/* number of pattern nodes */
  struct pattern_struct *patterns;		/* array of pattern nodes */
  int n_subjects;				/* number of subject nodes */
  struct subject_struct *subjects;		/* array of subject nodes */
  struct graph_list_struct *next_graph;		/* next graph problem */
  struct graph_list_struct *prev_graph;		/* previous graph problem */
} GRAPH_LIST;
  
/*
 *	Pattern node
 */
typedef struct pattern_struct {
  struct term_struct *pat;			/* pattern term */
  int pmult;					/* pattern multiplicity */
  struct msub_list_struct *matching_subs;	/* list of matching subjects */
/* added for generator */
  struct msub_list_struct *current_sub;		/* current substitution */
} PATTERN;

/*
 *	Subject nodes
 */
typedef struct subject_struct {
  struct term_struct *sub;			/* subject term */
  int smult;					/* subject multiplicity */
} SUBJECT;

/*
 *	Linked list of matching subjects
 */
typedef struct msub_list_struct {
  int msub;					/* index of matching subject */
  struct free_problem_struct *result;		/* subproblems from match */
  struct msub_list_struct *next_msub;		/* next matching subject */
} MSUB_LIST;

