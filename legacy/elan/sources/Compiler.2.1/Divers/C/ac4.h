
/* Codes */
#define code_44 44
#define code_43 43
#define code_42 42
#define code_41 41
#define code_40 40
#define code_18 18
#define code_17 17
#define code_16 16
#define code_15 15
#define code_14 14
#define code_13 13
#define code_12 12
#define code_11 11
#define code_69 69
#define code_68 68
#define code_67 67
#define code_66 66
#define code_65 65
#define code_3 3
#define code_64 64
#define code_2 2
#define code_63 63
#define code_1 1
#define code_62 62
#define code_61 61
#define code_60 60
#define code_75 75
#define code_74 74
#define code_73 73
#define code_72 72
#define code_71 71
#define code_70 70
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
extern struct term *fun_41(struct term *t);
extern struct term *fun_40(struct term *t);
extern struct term *con_18;
extern struct term *con_17;
extern struct term *con_16;
extern struct term *con_15;
extern struct term *con_14;
extern struct term *con_13;
extern struct term *con_12;
extern struct term *con_11;
extern struct term *con_69;
extern struct term *con_68;
extern struct term *con_67;
extern struct term *con_66;
extern struct term *con_65;
extern struct term *con_64;
extern struct term *con_2;
extern struct term *con_63;
extern struct term *con_1;
extern struct term *con_62;
extern struct term *con_61;
extern struct term *con_60;
extern struct term *con_75;
extern struct term *con_74;
extern struct term *con_73;
extern struct term *con_72;
extern struct term *con_71;
extern struct term *con_70;

/* Entetes */
extern struct term *str_74(struct term *t);
extern struct term *str_73(struct term *t);
extern struct term *str_72(struct term *t);
extern struct term *str_71(struct term *t);
extern struct term *str_70(struct term *t);
#include "ac_tools.c"
