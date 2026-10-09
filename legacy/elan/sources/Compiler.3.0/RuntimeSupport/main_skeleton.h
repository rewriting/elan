#ifndef __main_skeleton_h
#define __main_skeleton_h

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
#include "builtin.h"
#include "builtinMatching.h"
#include "streval.h"
#include "gc.h"
#include "acmatchdefs.h"
#include "termIn.h"
#include "termOut.h"
#include "trace.h"

#include "Back.h"
#include "choice.h"

/* Constantes d'execution */
extern int debugMode;
extern int quietMode;
extern int resultMode;
extern unsigned long rewrite_step;
extern unsigned long rewrite_label_step;
extern unsigned long rewrite_real_step;
extern int traceLevel;
extern int coqMode;
extern int printMode;

extern int global_indentlevel;
extern TERM *EkerTerm[];
extern int traceLevel;

/* Macros */
#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */
#ifdef DEBUG
/*
  #define declareIndentLevel() int indentlevel;
#define addindent() global_indentlevel++;
#define subindent() indentlevel--;
#define doindent(deep) indent(deep);
#define saveGlobalIndent() indentlevel=global_indentlevel;
#define restoreGlobalIndent() global_indentlevel=indentlevel;
*/
#define declareIndentLevel()
#define addindent()
#define subindent()
#define doindent(deep) indent(stack_ptr);
#define saveGlobalIndent()
#define restoreGlobalIndent()

#else
#define declareIndentLevel()
#define addindent()
#define subindent()
#define doindent(deep)
#define saveGlobalIndent()
#define restoreGlobalIndent()
#endif
// [pem: Sep  6 00]: 2*x pour corriger un bug du GC
#define allocStable(x) MALLOC(2*x)

#ifdef __cplusplus
typedef struct term* (*funTabType)(...);
#else
typedef struct term* (*funTabType)();
#endif

/*
 * function defined in xxx.c
 */
extern void EkerTermInit();
extern void symbol_init();
extern int strCall;


#endif
