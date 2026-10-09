#ifndef __main_header_h
#define __main_header_h
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include "tools.h"
#include "ac_tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#ifndef CSETCHP
#include "Back.h"
#else
#include "choice.h"
#endif
#include "builtin.h"
#include "builtinMatching.h"
#include "streval.h"
#include "gc.h"
#include "acmatchdefs.h"
#include "termIn.h"
#include "termOut.h"

/* Codes */
#define code_212 212
#define code_211 211
#define code_210 210
#define code_15 15
#define code_14 14
#define code_13 13
#define code_12 12
#define code_11 11
#define code_68 68
#define code_10 10
#define code_67 67
#define code_158 158
#define code_157 157
#define code_156 156
#define code_154 154
#define code_153 153
#define code_152 152
#define code_151 151
#define code_209 209
#define code_150 150
#define code_208 208
#define code_207 207
#define code_206 206
#define code_205 205
#define code_204 204
#define code_203 203
#define code_202 202
#define code_201 201
#define code_200 200
#define code_144 144
#define code_143 143
#define code_142 142
#define code_141 141
#define code_140 140
#define code_9 9
#define code_8 8
#define code_6 6
#define code_5 5
#define code_4 4
#define code_3 3
#define code_1 1
#define code_0 0
#define code_247 247
#define code_246 246
#define code_245 245
#define code_244 244
#define code_243 243
#define code_242 242
#define code_241 241
#define code_240 240
#define code_139 139
#define code_138 138
#define code_137 137
#define code_136 136
#define code_135 135
#define code_134 134
#define code_133 133
#define code_132 132
#define code_131 131
#define code_130 130
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
#define code_129 129
#define code_128 128
#define code_81 81
#define code_177 177
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
#define code_220 220
#define code_28 28
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
TERMSTR(term3,3);
TERMSTR(term4,4);

/* Constantes d'execution */
/* 0: no trace, 1: result, 2: start with */
#define traceLevel 0
extern int debugMode;
extern int quietMode;
extern unsigned long rewrite_step;
extern int global_indentlevel;
extern TERM *EkerTerm[];

/* Macros */
#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */
#ifdef DEBUG
#define declareIndentLevel() int indentlevel;
#define addindent() global_indentlevel++;
#define subindent() indentlevel--;
#define doindent(deep) indent(deep);
#define saveGlobalIndent() indentlevel=global_indentlevel;
#define restoreGlobalIndent() global_indentlevel=indentlevel;
#else
#define declareIndentLevel()
#define addindent()
#define subindent()
#define doindent(deep)
#define saveGlobalIndent()
#define restoreGlobalIndent()
#endif
#define allocStable(x) MALLOC(2*x)

/* Entetes */
extern struct term* fun_212(struct term *v1,struct term *v2);
extern struct term* fun_211(struct term *v1,struct term *v2);
extern struct term* fun_210(struct term *v1,struct term *v2);
extern struct term* fun_209(struct term *v1,struct term *v2);
extern struct term* fun_208(struct term *v1);
extern struct term* fun_207(struct term *v1,struct term *v2);
extern struct term* fun_206(struct term *v1,struct term *v2);
extern struct term* fun_205(struct term *v1,struct term *v2);
extern struct term* fun_204(struct term *v1,struct term *v2);
extern struct term* fun_203(struct term *v1,struct term *v2);
extern struct term* fun_202(struct term *v1,struct term *v2);
extern struct term* fun_201(struct term *v1,struct term *v2);
extern struct term *con_1;
extern struct term *con_0;
extern struct term *con_247;
extern struct term *con_246;
extern struct term *con_245;
extern struct term *con_242;
extern struct term *con_241;
extern struct term *con_240;
extern struct term *con_239;
extern struct term *con_238;
extern struct term *con_237;
extern struct term *fun_236(struct term *v0, struct term *v1, struct term *v2, struct term *v3);
extern struct term* fun_235(struct term *v1,struct term *v2,struct term *v3,struct term *v4);
extern struct term* fun_234(struct term *v1,struct term *v2,struct term *v3,struct term *v4);
extern struct term* fun_233(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_232(struct term *v1,struct term *v2);
extern struct term *fun_231(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_230(struct term *v0, struct term *v1);
extern struct term* fun_229(struct term *v1);
extern struct term* fun_228(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_227(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_226(struct term *v1,struct term *v2);
extern struct term *con_225;
extern struct term* fun_224(struct term *v1,struct term *v2);
extern struct term* fun_223(struct term *v1);
extern struct term* fun_222(struct term *v1,struct term *v2);
extern struct term* fun_220(struct term *v1,struct term *v2);
extern struct term *con_218;
extern struct term* fun_217(struct term *v1);
extern struct term* fun_216(struct term *v1);
extern struct term* fun_215(struct term *v1);
extern struct term* fun_214(struct term *v1,struct term *v2);
extern struct term* fun_213(struct term *v1,struct term *v2);

/* Entetes */
extern struct term *str_303(struct term *t);
extern struct term *str_465(struct term *t);
extern struct term *str_305(struct term *t);
extern struct term *str_464(struct term *t);
extern struct term *str_104(struct term *t);
extern struct term *str_491(struct term *t);
extern struct term *str_307(struct term *t);
extern struct term *str_449(struct term *t);
extern struct term *str_492(struct term *t);
extern struct term *str_306(struct term *t);
extern struct term *str_304(struct term *t);
extern struct term *str_466(struct term *t);
#endif
