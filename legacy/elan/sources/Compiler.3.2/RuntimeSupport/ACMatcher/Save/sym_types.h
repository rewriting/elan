/*
 *	typedefs
 */

/*
 *	enumerations
 */
typedef enum {UNDEF, VAR, ACFUNC, FUNC, CONSTAN} SYM_TYPE;

/*
 *	symbol table entries
 */
typedef struct symbol_struct {
  SYM_TYPE type;
  char *name;
  int arity;
  int vmark;			/* give each variable a unique number */
} SYMBOL;

