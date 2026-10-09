
/* Codes */
#define code_211 211
#define code_210 210
#define code_13 13
#define code_12 12
#define code_11 11
#define code_10 10
#define code_9 9
#define code_8 8
#define code_1 1
#define code_0 0
#define code_209 209
#define code_208 208
#define code_207 207
#define code_206 206
#define code_205 205
#define code_204 204
#define code_203 203
#define code_202 202
#define code_201 201
#define code_200 200
#define code_24 24
#define code_22 22
#define code_21 21

/* Structures */

/* Constantes d'execution */
extern int trace;
extern long rewrite_step;
extern int global_indentlevel;

/* Macros */
#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */
#define addindent() global_indentlevel++;
#define subindent() global_indentlevel--;

/* Entetes */
extern struct term *con_211;
extern struct term *con_210;
extern struct term *con_1;
extern struct term *con_0;
extern struct term *con_209;
extern struct term *con_208;
extern struct term *con_207;
extern struct term *con_206;
extern struct term *con_205;
extern struct term *con_204;
extern struct term *fun_202(struct term *t);
extern struct term *fun_201(struct term *t);

/* Entetes */
extern struct term *str_58(struct term *t);
extern struct term *str_57(struct term *t);
#include "ac_tools.c"
