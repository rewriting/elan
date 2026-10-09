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
#define code_1 1
#define code_0 0
#define code_13 13
#define code_12 12
#define code_11 11
#define code_10 10
#define code_24 24
#define code_22 22
#define code_21 21
#define code_202 202
#define code_201 201
#define code_200 200
#define code_9 9
#define code_8 8

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
#define allocStable(x) MALLOC(x)

/* Entetes */
extern struct term *con_1;
extern struct term *con_0;
extern struct term* fun_202(struct term *v1,struct term *v2);
extern struct term *con_200;

/* Entetes */
#endif
