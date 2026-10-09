#ifndef __main_skeleton_h
#define __main_skeleton_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include "tools.h"
#include "ac_tools.h"
#include "termCommon.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "builtin.h"
#include "builtinMatching.h"
#include "streval.h"
#include "gc.h"
#include "eker_tools.h"
#include "acmatchdefs.h"
#include "termIn.h"
#include "termOut.h"
#include "trace.h"

//#include "Back.h"
#ifdef CSETCHP
#include "choice.h"
#endif

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

// ehm modification a faire ac
extern TERM *EkerTerm[];
extern int traceLevel;
extern char trace_file[];
extern char query_file[];
extern char query_sort_file[];

/* Macros */
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
/*#ifdef __cplusplus
typedef Gterm* (*funTabType)(...);
#else
typedef Gterm* (*funTabType)();
#endif*/

/*
 * function defined in xxx.c
 */
//modification a faire ac
extern void EkerTermInit();
extern void symbol_init();
extern int strCall;
//extern funTabType strTab[]; // [Huy: Oct 18 00] 
 /*
  * Library
  */
extern int initElanLib(long *bp); 
extern int getStrategyIndex(char *strategyName);
extern int getSymbolIndex(char *symbolName);
extern Gterm *makeConstructor(int code);

#endif
