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
#include "termIn.h"
#include "termOut.h"

/* Codes */
#define code_1 1
#define code_0 0
#define code_19 19
#define code_18 18
#define code_15 15
#define code_14 14
#define code_68 68
#define code_13 13
#define code_12 12
#define code_11 11
#define code_10 10
#define code_24 24
#define code_22 22
#define code_21 21
#define code_205 205
#define code_33 33
#define code_204 204
#define code_32 32
#define code_203 203
#define code_31 31
#define code_202 202
#define code_30 30
#define code_201 201
#define code_200 200
#define code_9 9
#define code_8 8

/* Structures */

/* Constantes d'execution */
/* 0: no trace, 1: result, 2: start with */
#define trace 0
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
#define allocStable(x) MALLOC(x)

/* Entetes */
extern struct term *con_1;
extern struct term *con_0;
extern struct term *con_205;
extern struct term *con_204;
extern struct term* fun_203(struct term *v1);
extern struct term *con_202;

/* Entetes */
#endif
