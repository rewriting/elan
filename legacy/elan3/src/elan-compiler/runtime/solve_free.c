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
/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *	Solve free AC problem to produce variable bindings & stack
 *	of pure equations.
 */
#include "defs.h"
#include "term_types.h"
#include "free_types.h"
#include "pure_types.h"
#include "state_types.h"
#include "functions.h"

static BOOL advance_prob();
static BOOL clash_bindings();
static AC_LIST *find_unused();
static void delete_unused();
static BOOL advance_graph();

/*
 *	Find all solutions to free AC problem by iteratively backtracking
 *	over all solutions to AC subproblems.
 */
BOOL solve_free(obj, fp, reset)
MATCH_OBJECT *obj;
FREE_PROBLEM *fp;
BOOL reset;
{
  AC_PROB_LIST *p;

  if(reset){
    if(fp->bindings){
      if(clash_bindings(obj->bind_tab, fp->bindings))
        return(FALSE);
      assert_bindings(obj->bind_tab, fp->bindings);
    }
    p = fp->subproblems;
forward:
    for(; p; p = p->next_prob){
      if(!advance_prob(obj, p, TRUE)){
        p = p->prev_prob;
        goto backtrack;
      }
    }
    return(TRUE);
  }
  else{
    p = fp->subproblems_tail;
backtrack:
    for(; p; p = p->prev_prob){
      if(advance_prob(obj, p, FALSE)){
        p = p->next_prob;
        goto forward;
      }
    }
    if(fp->bindings)
      retract_bindings(obj->bind_tab, fp->bindings);
    return(FALSE);
  }
}

static BOOL clash_bindings(bind_tab, b)
BINDING bind_tab[];
BIND_LIST *b;
{
  TERM *t;

  for(; b; b = b->next_bound){
    t = bind_tab[b->var_num].value;
    if(t != NULL && eker_term_cmp(t, b->binding) != 0)
      return(TRUE);
  }
  return(FALSE);
}

/*
 *	Find all solutions to free AC problem with AC top symbol by
 *	iteratively backtracking over all solutions to graph subproblems.
 *	After a success, if there are variables under the AC top symbol,
 *	left over subject terms are gathered and forma pure AC equation
 *	which is pushed on the pure stack.
 */
static BOOL advance_prob(obj, p, reset)
MATCH_OBJECT *obj;
AC_PROB_LIST *p;
BOOL reset;
{
  GRAPH_LIST *g;
  PURE_LIST *t;

  if(reset){
    g = p->graph_problems;
forwards:
    for(; g; g = g->next_graph){
      if(!advance_graph(obj, g, TRUE)){
        g = g->prev_graph;
        goto backtrack;
      }
    }
    if(p->variables){	/* push pure equation on stack */
      t = EMALLOC(PURE_LIST);
      t->ac_sym = p->ac_sym;
      t->variables = p->variables;
      t->unused = find_unused(p);
      t->next_pure = obj->pure_stack;
      obj->pure_stack = t;
    }
    return(TRUE);
  }
  else{
    g = p->graph_problems_tail;
    if(p->variables){	/* remove pure equation from stack */
      delete_unused(obj->pure_stack->unused, p->unused);
      t = obj->pure_stack->next_pure;
      EFREE(obj->pure_stack);
      obj->pure_stack = t;
    }
backtrack:
    for(; g; g = g->prev_graph){
      if(advance_graph(obj, g, FALSE)){
        g = g->next_graph;
        goto forwards;
      }
    }
    return(FALSE);
  }
}

/*
 *	Gather up unused subject terms from graph problems and tack them
 *	on to the list of unused subject terms from the AC subproblem.
 */
static AC_LIST *find_unused(p)
AC_PROB_LIST *p;
{
  AC_LIST *head = p->unused, *t;
  int i, ns;
  SUBJECT *sa;
  GRAPH_LIST *g;

  for(g = p->graph_problems; g; g = g->next_graph){
    ns = g->n_subjects;
    sa = g->subjects;
    for(i = 0; i < ns; i++){
      if(sa[i].smult > 0){
        t = EMALLOC(AC_LIST);
        t->arg = sa[i].sub;
        t->mult = sa[i].smult;
        t->next_ac = head;
        head = t;
      }
    }
  }
  return(head);
}

/*
 *	Delete list of unused subterms gathered from graph problems but
 *	leave original list of unused subject terms from the AC subproblem
 *	untouched. Assumes pointer comparisions to heap objects are valid.
 */
static void delete_unused(temp, unused)
AC_LIST *temp, *unused;
{
   AC_LIST *q;

  /* 
   * 17/08/98 : modif de PEM
   * destruction des structures par le GC
   */
   //temp=NULL; return;

   if(temp == NULL)	/* initialisation case */
     return;
   for(; temp != unused; temp = q){
     q = temp->next_ac;
     EFREE(temp);
   }
}

/*
 *	Find all solutions to graph subproblem using brute force
 *	iterative bactracking search. Should really use Japanese
 *	algorithm in the case where all pattern nodes have
 *	multiplicity 1.
 */
static BOOL advance_graph(obj, g, reset)
MATCH_OBJECT *obj;
GRAPH_LIST *g;
BOOL reset;
{
  int i, pmult;
  MSUB_LIST *ms;
  int np = g->n_patterns;
  PATTERN *pat = g->patterns;
  SUBJECT *sub = g->subjects;

  if(reset){
    i = -1;
forward:
    for(i++; i < np; i++){
      pmult = pat[i].pmult;
      for(ms = pat[i].matching_subs; ms; ms = ms->next_msub){
        if(sub[ms->msub].smult >= pmult
          && (ms->result == NULL || solve_free(obj, ms->result, TRUE))){
            pat[i].current_sub = ms;
            sub[ms->msub].smult -= pmult;
            goto okay;
        }
      }
      goto backtrack;
okay:;
    }
    return(TRUE);
  }
  else{
    i = np;
backtrack:
    for(i--; i >= 0; i--){
      ms = pat[i].current_sub;
      if(ms->result != NULL && solve_free(obj, ms->result, FALSE))
        goto forward;
      pmult = pat[i].pmult;
      sub[ms->msub].smult += pmult;
      for(ms = ms->next_msub; ms; ms = ms->next_msub){
        if(sub[ms->msub].smult >= pmult
          && (ms->result == NULL || solve_free(obj, ms->result, TRUE))){
          pat[i].current_sub = ms;
          sub[ms->msub].smult -= pmult;
          goto forward;
        }
      }
    }
    return(FALSE);
  }
}

