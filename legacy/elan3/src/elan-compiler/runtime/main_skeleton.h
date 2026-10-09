/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
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
extern funTabType strTab[]; // [Huy: Oct 18 00] 

#endif
