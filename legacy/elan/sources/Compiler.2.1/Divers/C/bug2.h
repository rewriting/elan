#ifndef __main_header_h
#define __main_header_h
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "back.h"
#include "choice.h"
#include "builtin.h"
#include "builtinMatching.h"
#include "streval.h"
#include "gc.h"
#include "acmatchdefs.h"

/* Codes */
#define code_212 212
#define code_211 211
#define code_1 1
#define code_210 210
#define code_0 0
#define code_13 13
#define code_12 12
#define code_11 11
#define code_10 10
#define code_24 24
#define code_22 22
#define code_21 21
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

/* Structures */
TERMSTR(term3,3);

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
#define allocStable(x) MALLOC(x)

/* Entetes */
extern struct term *con_212;
extern struct term *con_211;
extern struct term *con_1;
extern struct term *con_210;
extern struct term *con_0;
extern struct term *con_209;
extern struct term *con_208;
extern struct term *fun_204(struct term *t);
extern struct term *con_203;
extern struct term *con_202;
extern struct term *con_201;
extern struct term* fun_200();

/* Entetes */
extern struct term *str_414(struct term *t);
#endif
