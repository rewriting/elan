#ifndef __main_header_h
#define __main_header_h
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "back.h"
#include "builtin.h"
#include "builtinMatching.h"
#include "streval.h"
#include "gc.h"
#include "acmatchdefs.h"

/* Codes */
#define code_212 212
#define code_19 19
#define code_211 211
#define code_18 18
#define code_210 210
#define code_15 15
#define code_14 14
#define code_13 13
#define code_12 12
#define code_11 11
#define code_68 68
#define code_10 10
#define code_67 67
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
#define code_9 9
#define code_8 8
#define code_6 6
#define code_5 5
#define code_4 4
#define code_3 3
#define code_1 1
#define code_0 0
#define code_243 243
#define code_242 242
#define code_241 241
#define code_240 240
#define code_239 239
#define code_238 238
#define code_237 237
#define code_236 236
#define code_235 235
#define code_234 234
#define code_233 233
#define code_232 232
#define code_231 231
#define code_230 230
#define code_33 33
#define code_32 32
#define code_31 31
#define code_30 30
#define code_229 229
#define code_228 228
#define code_227 227
#define code_226 226
#define code_225 225
#define code_224 224
#define code_223 223
#define code_222 222
#define code_29 29
#define code_221 221
#define code_28 28
#define code_220 220
#define code_27 27
#define code_26 26
#define code_25 25
#define code_24 24
#define code_22 22
#define code_79 79
#define code_21 21
#define code_20 20
#define code_219 219
#define code_218 218
#define code_217 217
#define code_216 216
#define code_215 215
#define code_214 214
#define code_213 213

/* Structures */

/* Constantes d'execution */
/* 0: no trace, 1: result, 2: start with */
#define trace 0
extern int debugMode;
extern unsigned long rewrite_step;
extern int global_indentlevel;
extern TERM *EkerTerm[];

/* Macros */
#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */
#ifdef DEBUG
#define addindent() global_indentlevel++;
#define subindent() global_indentlevel--;
#define doindent(deep) indent(deep);
#define saveGlobalIndent() int indentlevel=global_indentlevel;
#define restoreGlobalIndent() global_indentlevel=indentlevel;
#else
#define addindent()
#define subindent()
#define doindent(deep)
#define saveGlobalIndent()
#define restoreGlobalIndent()
#endif

/* Entetes */
extern struct term* fun_212(struct term *v1,struct term *v2);
extern struct term* fun_211(struct term *v1,struct term *v2);
extern struct term* fun_210(struct term *v1,struct term *v2);
extern struct term* fun_209(struct term *v1);
extern struct term* fun_208(struct term *v1,struct term *v2);
extern struct term* fun_207(struct term *v1,struct term *v2);
extern struct term* fun_206(struct term *v1,struct term *v2);
extern struct term* fun_205(struct term *v1,struct term *v2);
extern struct term* fun_204(struct term *v1,struct term *v2);
extern struct term* fun_203(struct term *v1,struct term *v2);
extern struct term* fun_202(struct term *v1,struct term *v2);
extern struct term *con_1;
extern struct term *con_0;
extern struct term *con_243;
extern struct term *con_242;
extern struct term* fun_239(struct term *v1,struct term *v2);
extern struct term *fun_238(struct term *t);
extern struct term *fun_237(struct term *t);
extern struct term *con_234;
extern struct term *con_233;
extern struct term *con_232;
extern struct term* fun_231(struct term *v1);
extern struct term* fun_230(struct term *v1);
extern struct term* fun_229(struct term *v1);
extern struct term* fun_227();
extern struct term *con_226;
extern struct term* fun_225(struct term *v1,struct term *v2);
extern struct term* fun_224(struct term *v1);
extern struct term* fun_223(struct term *v1,struct term *v2);
extern struct term* fun_221(struct term *v1,struct term *v2);
extern struct term *con_219;
extern struct term* fun_218(struct term *v1);
extern struct term* fun_217(struct term *v1);
extern struct term* fun_216(struct term *v1);
extern struct term* fun_215(struct term *v1,struct term *v2);
extern struct term* fun_214(struct term *v1,struct term *v2);
extern struct term* fun_213(struct term *v1,struct term *v2);

/* Entetes */
extern struct term *str_335(struct term *t);
extern struct term *str_234(struct term *t);
extern struct term *str_107(struct term *t);
extern struct term *str_46(struct term *t);
extern struct term *str_36(struct term *t);
extern struct term *str_233(struct term *t);
extern struct term *str_229(struct term *t);
extern struct term *str_222(struct term *t);
extern struct term *str_463(struct term *t);
extern struct term *str_334(struct term *t);
extern struct term *str_336(struct term *t);
extern struct term *str_228(struct term *t);
extern struct term *str_223(struct term *t);
extern struct term *str_205(struct term *t);
extern struct term *str_204(struct term *t);
extern struct term *str_114(struct term *t);
extern struct term *str_108(struct term *t);
extern struct term *str_26(struct term *t);
extern struct term *str_464(struct term *t);
extern struct term *str_259(struct term *t);
extern struct term *str_115(struct term *t);
#endif
