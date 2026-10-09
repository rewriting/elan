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
 *    Routines to simplify and build data structure for free AC problem
 */
#include "defs.h"
#include "term_types.h"
#include "free_types.h"
#include "pure_types.h"
#include "state_types.h"
#include "functions.h"

static BOOL simplify(MATCH_OBJECT *obj, FREE_PROBLEM *fp, TERM *p, TERM *s);
static BOOL simplify_ac(MATCH_OBJECT *obj, FREE_PROBLEM *fp, int top, AC_LIST *pp, AC_LIST *ss);
static int count_same(AC_LIST *pp);
static void free_bind_list(BIND_LIST *l);
static void free_subprob_list(AC_PROB_LIST *l);
static void free_variables(VAR_LIST *l);
static void free_unused(AC_LIST *l);
static void free_graphs(GRAPH_LIST *l);
static void free_msub_list(MSUB_LIST *l);

/*
 *	Build structure for free AC problem by:
 *	  (1)	Call simplify() to match free symbol skeleton and
 *		generate a list of variable bindings an a list of AC
 *		subproblems.
 *	  (2)	For every graph problem occuring in an AC subproblem,
 *		for every possible matching pair of pattern and subject
 *		nodes, make a recursive call to build a structure for
 *		the induced free AC problem.
 *	If successful, pointer to structure copied through dest.
 */
BOOL build_free(MATCH_OBJECT *obj,TERM *p,TERM *s,FREE_PROBLEM **dest) {
  FREE_PROBLEM *fp, *new_fp;
  AC_PROB_LIST *a;
  GRAPH_LIST *g;
  int i, j, np, ns, pmult;
  MSUB_LIST *sub_head, *sub_tail;
  PATTERN *pa;
  SUBJECT *sa;
  TERM *pat;

  fp = EMALLOC(FREE_PROBLEM);
  fp->bindings = NULL;
  fp->subproblems = NULL;
  fp->subproblems_tail = NULL;
  if(!simplify(obj, fp, p, s))
    goto clean_up2;
  if(fp->bindings != NULL)
    assert_bindings(obj->bind_tab, fp->bindings);
  for(a = fp->subproblems; a; a = a->next_prob){ /* For each AC subproblem */
    for(g = a->graph_problems; g; g = g->next_graph){ /* For each graph */
      np = g->n_patterns; pa = g->patterns;
      ns = g->n_subjects; sa = g->subjects;
      for(i = 0; i < np; i++){ /* For each pattern node */
        sub_head = sub_tail = NULL;
        pat = pa[i].pat; pmult = pa[i].pmult;
        for(j = 0; j < ns; j++){ /* For each subject node */
          if(sa[j].smult >= pmult){
            if(build_free(obj, pat, sa[j].sub, &new_fp)){
              if(sub_tail)
                sub_tail = (sub_tail->next_msub) = EMALLOC(MSUB_LIST);
              else
                sub_tail = sub_head = EMALLOC(MSUB_LIST);
              sub_tail->msub = j;
              sub_tail->result = new_fp;
            }
          }
        }
        if(!sub_tail) /* No match for pattern node */
          goto clean_up;
        sub_tail->next_msub = NULL;
        pa[i].matching_subs = sub_head;
      }
    }
  }
  if(fp->bindings != NULL)
    retract_bindings(obj->bind_tab, fp->bindings);
  else{
    if(fp->subproblems == NULL){
      EFREE(fp);
      fp = NULL;
    }
  }
  *dest = fp;
  return(TRUE);
/*
 *	Tidy up after failure
 */
clean_up:
  if(fp->bindings != NULL)
    retract_bindings(obj->bind_tab, fp->bindings);
clean_up2:
  destroy_free(fp);
  return(FALSE);
}

/*
 *	Simplify subproblem depending on type of pattern symbol:
 *	  Constant:	Check for equality with subject symbol
 *	  Variable:	Check for clash with bindings from above skeletons;
 *			Check for clash with bindings for this skeleton;
 *			If not yet bound, insert binding.
 *	  Function:	Simplify arguments.
 *	  AC Function:	Build complicated structure to hold list of graph
 *			problems, top level variables and unmatchable
 *			(except by variables) subject terms.
 */
static BOOL simplify(MATCH_OBJECT *obj, FREE_PROBLEM *fp, TERM *p, TERM *s)
{
  int t;
  BIND_LIST *q, *qf;
  TERM_LIST *pp, *ss;
  TERM *b;

  switch(p->type){
  case CONSTANT:
  case BUILTIN:
    return((BOOL) (p->sym == s->sym));
  case VARIABLE:
    t = p->rest.v.var_nr;
    b = obj->bind_tab[t].value;
    if(b)
      return((BOOL) (eker_term_cmp(b, s) == 0));	/* bound in above skeleton */
    for(qf = NULL, q = fp->bindings; q ; qf = q, q = q->next_bound){
      if(q->var_num == t)
        return((BOOL) (eker_term_cmp(q->binding, s) == 0));
      if(q->var_num > t)
        break;
    }
    if(qf)
      qf = (qf->next_bound = EMALLOC(BIND_LIST));
    else
      qf = (fp->bindings = EMALLOC(BIND_LIST));
    qf->var_num = t;
    qf->binding = s;
    qf->next_bound = q;
    break;
  case FUNCTION:
    if(p->sym != s->sym)
      return(FALSE);
    for(pp = p->rest.f.arg_list, ss = s->rest.f.arg_list ; pp;
      pp = pp->next_arg, ss = ss->next_arg)
        if(!simplify(obj, fp, pp->arg, ss->arg))
          return(FALSE);
    break;
  case AC_COMPRESSED:
    if(p->sym != s->sym)
      return(FALSE);
    if(p->rest.a.arg_count > s->rest.a.arg_count)
      return(FALSE);
    return(simplify_ac(obj, fp, p->sym, p->rest.a.ac_list, s->rest.a.ac_list));
  default:
    ASSERT(FALSE, "simplify(): unknown term type");
  }
  return(TRUE);
}

/*
 *	list building macros
 */
#define U_APPEND(a, m)	((u_tail = (u_tail ? \
			(u_tail->next_ac = EMALLOC(AC_LIST)) : \
			(u_head = EMALLOC(AC_LIST)) )), \
                        u_tail->arg = (a), u_tail->mult = (m))
#define G_APPEND(n, p, m, s) \
			((g_temp = (g_tail ? \
			(g_tail->next_graph = EMALLOC(GRAPH_LIST)) : \
			(g_head = EMALLOC(GRAPH_LIST)) )), \
                        g_temp->prev_graph = g_tail, g_tail = g_temp, \
                       	g_tail->n_patterns = (n), g_tail->patterns = (p), \
			g_tail->n_subjects = (m), g_tail->subjects = (s))
#define V_APPEND(v, m)	((v_tail = (v_tail ? \
			(v_tail->next_var = EMALLOC(VAR_LIST)) : \
			(v_head = EMALLOC(VAR_LIST)) )), \
                        v_tail->var_num = (v), v_tail->var_mult = (m))


#define NONE		-1

/*
 *	Simplify subproblem with AC top symbol
 */
static BOOL simplify_ac(MATCH_OBJECT *obj, FREE_PROBLEM *fp, int top, AC_LIST *pp, AC_LIST *ss)
{
  int i, diff, np, ns, g_left_overs, g_diff;
  TERM *p, *s;
  PATTERN *pa;
  SUBJECT *sa;
  AC_PROB_LIST *r;
  AC_LIST *u_head = NULL, *u_tail = NULL; /* list of unused subjects */
  GRAPH_LIST *g_head = NULL, *g_tail = NULL; /* list of graph problemss */
  VAR_LIST *v_head = NULL, *v_tail = NULL; /* list of variables */
  GRAPH_LIST *g_temp;

  g_left_overs = 0;
  while(pp){
    p = pp->arg;
    switch(p->type){
    case CONSTANT:
    case BUILTIN:
      for(;; ss = ss->next_ac){
        if(ss == NULL)
          goto clean_up; /* didn't find any subject with same top sym */
        s = ss->arg;
        if(s->sym == p->sym)
          break;
        U_APPEND(s, ss->mult);
      }
      diff = ss->mult - pp->mult;
      if(diff < 0)
        goto clean_up; /* not enough constants in subject */
      if(diff > 0)
        U_APPEND(s, diff);
      ss = ss->next_ac; /* skip over constant */
      pp = pp->next_ac;
      break;
    case FUNCTION:
    case AC_COMPRESSED:
      for(;; ss = ss->next_ac){
        if(ss == NULL)
          goto clean_up; /* didn't find any subject with same top sym */
        s = ss->arg;
        if(s->sym == p->sym)
          break;
        U_APPEND(s, ss->mult);
      }
      np = count_same(pp);
      pa = CALLOC(np, PATTERN);
      g_diff = 0;
      for(i = 0; i < np; i++, pp = pp->next_ac){
        pa[i].pat = pp->arg;
        pa[i].pmult = pp->mult;
        g_diff -= pp->mult;
        pa[i].matching_subs = NULL;	/* so we can destroy struct easily */
      }
      ns = count_same(ss);
      sa = CALLOC(ns, SUBJECT);
      for(i = 0; i < ns; i++, ss = ss->next_ac){
        sa[i].sub = ss->arg;
        sa[i].smult = ss->mult;
        g_diff += ss->mult;
      }
      G_APPEND(np, pa, ns, sa);
      if(g_diff < 0) /* more patterns than subjects in graph */
        goto clean_up;
      g_left_overs += g_diff;
      break;
    case VARIABLE:
      V_APPEND(p->rest.v.var_nr, pp->mult);
      pp = pp->next_ac;
      break;
    default:
      ASSERT(FALSE, "simplify_ac(): unknown term type");
    }
  }
  for(; ss; ss = ss->next_ac)
    U_APPEND(ss->arg, ss->mult);
/*
 *	Heuristic checks
 */
  if((u_tail || g_left_overs >  0) && !v_tail)
    goto clean_up; /* unused stuff with no variables to assign it to */
  if(!u_tail && !v_tail && !g_tail)
    return(TRUE); /* nothing left - don't build ac subproblem */
/*
 *	Terminate lists
 */
  if(u_tail)
    u_tail->next_ac = NULL;
  if(g_tail)
    g_tail->next_graph = NULL;
  if(v_tail)
    v_tail->next_var = NULL;
/*
 *	Add simplified problem to list of AC subproblems
 */
  r = EMALLOC(AC_PROB_LIST);
  r->ac_sym = top;
  r->unused = u_head;
  r->graph_problems = g_head;
  r->graph_problems_tail = g_tail;
  r->variables = v_head;
  r->prev_prob = fp->subproblems_tail;
  r->next_prob = NULL;
  if(fp->subproblems_tail)
    fp->subproblems_tail->next_prob = r;
  else
    fp->subproblems = r;
  fp->subproblems_tail = r;
  return(TRUE);
/*
 *	Tidy up after failure
 */
clean_up:
  if(u_tail){
    u_tail->next_ac = NULL;
    free_unused(u_head);
  }
  if(g_tail){
    g_tail->next_graph = NULL;
    free_graphs(g_head);
  }
  if(v_tail){
    v_tail->next_var = NULL;
    free_variables(v_head);
  }
  return(FALSE);
}

/*
 *	Count number of subterms with the same top symbol as the first
 *	on the list.
 */
static int count_same(AC_LIST *pp)
{
  int count = 1;
  int sym = pp->arg->sym;

  for(pp = pp->next_ac; pp; pp = pp->next_ac){
    if(pp->arg->sym == sym)
      count++;
    else
      break;
  }
  return(count);
}

void assert_bindings(BINDING bind_tab[], BIND_LIST *b)
{
  int i;

  for(; b; b = b->next_bound){
    i = b->var_num;
    if(bind_tab[i].value == NULL){
      bind_tab[i].value = b->binding;
      bind_tab[i].n_times = 1;
    }
    else
      bind_tab[i].n_times++;
  }
}

void retract_bindings(BINDING bind_tab[], BIND_LIST *b)
{
  int i;

  for(; b; b = b->next_bound){
    i = b->var_num;
    if(--bind_tab[i].n_times == 0)
      bind_tab[i].value = NULL;
  }
}

void destroy_free(FREE_PROBLEM *fp)
{
  /* 
   * 17/08/98 : modif de PEM
   * destruction des structures par le GC
   */
    //  fp=NULL; return;

  if(fp == NULL)
    return;
  free_bind_list(fp->bindings);
  free_subprob_list(fp->subproblems);
  EFREE(fp);
}

static void free_bind_list(BIND_LIST *l)
{
  BIND_LIST *next;

  for(; l; l = next){
    next = l->next_bound;
    EFREE(l);
  }
}

static void free_subprob_list(AC_PROB_LIST *l)
{
  AC_PROB_LIST *next;

  for(; l; l = next){
    next = l->next_prob;
    free_variables(l->variables);
    free_unused(l->unused);
    free_graphs(l->graph_problems);
    EFREE(l);
  }
}

static void free_variables(VAR_LIST *l)
{
  VAR_LIST *next;

  for(; l; l = next){
    next = l->next_var;
    EFREE(l);
  }
}

static void free_unused(AC_LIST *l)
{
  AC_LIST *next;

  for(; l; l = next){
    next = l->next_ac;
    EFREE(l);
  }
}

static void free_graphs(GRAPH_LIST *l)
{
  GRAPH_LIST *next;
  int i, np;
  PATTERN *pa;

  for(; l; l = next){
    next = l->next_graph;
    np = l->n_patterns; pa = l->patterns;
    for(i = 0; i < np; i++)
      free_msub_list(pa[i].matching_subs);
    EFREE(l->patterns);
    EFREE(l->subjects);
    EFREE(l);
  }
}

static void free_msub_list(MSUB_LIST *l)
{
  MSUB_LIST *next;

  for(; l; l = next){
    next = l->next_msub;
    destroy_free(l->result);
    EFREE(l);
  }
}

