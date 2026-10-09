
/* Codes */
#define code_44 44
#define code_43 43
#define code_42 42
#define code_41 41
#define code_40 40
#define code_5 5
#define code_4 4
#define code_3 3
#define code_2 2
#define code_1 1
#define code_34 34
#define code_33 33
#define code_32 32
#define code_31 31
#define code_30 30
#define code_49 49
#define code_48 48
#define code_47 47
#define code_46 46
#define code_45 45

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
extern struct term* fun_44();
extern struct term* fun_43();
extern struct term* fun_42();
extern struct term* fun_41();
extern struct term* fun_40();
extern struct term* fun_4(struct term *v1,struct term *v2);
extern struct term* fun_3(struct term *v1,struct term *v2);
extern struct term *con_1;
extern struct term *fun_34(struct term *t);
extern struct term *fun_33(struct term *t);
extern struct term *fun_32(struct term *t);
extern struct term *con_30;
extern struct term* fun_49();
extern struct term* fun_48();
extern struct term* fun_47();
extern struct term* fun_46();
extern struct term* fun_45();

// TODO
//#include "outils.c"
