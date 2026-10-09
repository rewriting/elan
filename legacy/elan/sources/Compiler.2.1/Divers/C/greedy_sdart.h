
/* Codes */
#define code_218 218
#define code_217 217
#define code_216 216
#define code_215 215
#define code_214 214
#define code_213 213
#define code_212 212
#define code_211 211
#define code_210 210
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

/* Structures */

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
extern struct term* fun_218();
extern struct term* fun_217();
extern struct term* fun_216();
extern struct term* fun_215();
extern struct term* fun_214();
extern struct term *fun_213(struct term *t);
extern struct term *fun_212(struct term *t);
extern struct term *fun_211(struct term *t);
extern struct term *con_209;
extern struct term* fun_208();
extern struct term* fun_207();
extern struct term* fun_206();
extern struct term* fun_205();
extern struct term* fun_204();
extern struct term* fun_203(struct term *v1,struct term *v2);
extern struct term* fun_202(struct term *v1,struct term *v2);
extern struct term *con_200;

/* Entetes */
#include "ac_tools.c"
