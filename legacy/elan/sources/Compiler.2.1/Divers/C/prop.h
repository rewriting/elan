
/* Codes */
#define code_43 43
#define code_42 42
#define code_41 41
#define code_40 40
#define code_19 19
#define code_18 18
#define code_17 17
#define code_16 16
#define code_15 15
#define code_14 14
#define code_13 13
#define code_12 12
#define code_11 11
#define code_2 2
#define code_1 1

/* Structures */
TERMSTR(term1,1);
TERMSTR(term2,2);

/* Constantes d'execution */
extern int trace;
extern long rewrite_step;
extern int indentlevel;

/* Macros */
#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */
#define addindent() indentlevel++;
#define subindent() indentlevel--;

/* Entetes */
extern struct term* fun_43(struct term *v1);
extern struct term* fun_42(struct term *v1,struct term *v2);
extern struct term *fun_41(struct term *t);
extern struct term *fun_40(struct term *t);
extern struct term *con_19;
extern struct term *con_18;
extern struct term *con_17;
extern struct term *con_16;
extern struct term *con_15;
extern struct term *con_14;
extern struct term *con_13;
extern struct term *con_12;
extern struct term *con_11;
extern struct term *con_2;
extern struct term *con_1;
#include "ac_tools.c"
