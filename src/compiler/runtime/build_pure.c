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
 *	Build structure for solving system of pure AC equations
 */
#include "defs.h"
#include "term_types.h"
#include "free_types.h"
#include "pure_types.h"
#include "state_types.h"
#include "functions.h"

static void free_index_list(INDEX_LIST *list);
static int pure_list_len(PURE_LIST *r);
static BOOL delete_bound(PURE_PROBLEM *prob, BINDING bind_tab[]);
static BOOL delete_binding(PURE_PROBLEM *prob, int vmult[], TERM *b);
static void delete_dead_vars(PURE_PROBLEM *prob);
static void delete_dead_terms(PURE_PROBLEM *prob);
static void build_assign(PURE_PROBLEM *prob);
static int var_cmp(const void *pi, const void *pj);
static void compute_index_lists(PURE_PROBLEM *prob);

/*
 *	Free index list
 */
static void free_index_list(INDEX_LIST *list)
{
  INDEX_LIST *p;

  while(list){
    p = list->next_index;
    EFREE(list);
    list = p;
  }
}

/*
 *	Build pure problem structure as follows:
 *	(1) For each pure AC equation:
 *		(a) insert its AC symbol into top_sym
 *		(b) insert its variables into var_tab
 *		(c) insert its subject terms into term_tab
 *	(2) Delete any bound variables together with their bindings.
 *	(3) Build structures for assignments and shared variables.
 */
BOOL build_pure(MATCH_OBJECT *obj)
{
  PURE_LIST *pure_stack = obj->pure_stack;
  int i, j, term_tab_size, *vmult;
  PURE_LIST *r;
  VAR_LIST *v;
  AC_LIST *t;
  PURE_PROBLEM *prob;
  int n_pure, n_var, n_term, *top_sym;
  VAR_HEADER *var_tab;
  TERM_HEADER *term_tab;

  n_pure = pure_list_len(pure_stack);
  if(n_pure == 0){
    obj->pure_prob = NULL;
    return(TRUE);
  }
  top_sym = CALLOC(n_pure, int);
  var_tab = CALLOC(obj->total_var, VAR_HEADER);
  term_tab_size = 1;
  term_tab = CALLOC(term_tab_size, TERM_HEADER);
  n_var = n_term = 0;

  for(i = 0, r = pure_stack; r; i++, r = r->next_pure){
    top_sym[i] = r->ac_sym;
    for(v = r->variables; v; v = v->next_var){	/* Insert variables */
      for(j = 0; j < n_var; j++){
        if(var_tab[j].var == v->var_num){
          var_tab[j].vmult[i] = v->var_mult;
          goto var_found;
        }
      }
      var_tab[n_var].var =  v->var_num;
      var_tab[n_var].vmult = vmult = CALLOC(n_pure, int);
      for(j = 0; j < n_pure; j++)
        vmult[j] = 0;
      var_tab[n_var].vmult[i] = v->var_mult;
      var_tab[n_var].owner = SHARED;	/* so we don't free nonexist assign */
      n_var++;
var_found:;
    }
    for(t = r->unused; t; t = t->next_ac){	/* Insert terms */
      for(j = 0; j < n_term; j++){
        if(eker_term_cmp(term_tab[j].term, t->arg) == 0){
          term_tab[j].tmult[i] = t->mult;
          goto term_found;
        }
      }
      if(n_term == term_tab_size){	/* Grow term_tab */
        TERM_HEADER *old_term_tab = term_tab;
        int i;
        term_tab_size *= 2;
          /*
            printf("term_tab_size=%d\tTERM_HEADER=%d\n",
            term_tab_size, sizeof(TERM_HEADER));
          */
          //term_tab = REALLOC(term_tab, term_tab_size, TERM_HEADER);
        term_tab = CALLOC(term_tab_size, TERM_HEADER);
        for(i=0 ; i<(term_tab_size/2) ; i++) {
          term_tab[i].term     = old_term_tab[i].term;
          term_tab[i].tmult    = old_term_tab[i].tmult;
          term_tab[i].subterms = old_term_tab[i].subterms;
        }
      }
      term_tab[n_term].term = t->arg;
      term_tab[n_term].tmult = CALLOC(n_pure, int);
      for(j = 0; j < n_pure; j++)
        term_tab[n_term].tmult[j] = 0;
      term_tab[n_term].tmult[i] = t->mult;
      term_tab[n_term].subterms = 0;	/* so we know if to free list */
      n_term++;
term_found:;
    }
  }
  prob = EMALLOC(PURE_PROBLEM);
  prob->n_pure = n_pure;
  prob->n_var = n_var;
  prob->n_term = n_term;
  prob->top_sym = top_sym;
  prob->var_tab = var_tab;
  prob->term_tab = term_tab;
  if(!delete_bound(prob, obj->bind_tab)){
    destroy_pure(prob);
    return(FALSE);
  }
  build_assign(prob);
  obj->pure_prob = prob;
  return(TRUE);
}

/*
 *	Return length of list of pure AC equations
 */
static int pure_list_len(PURE_LIST *r)
{
  int c;

  for(c = 0; r; r = r->next_pure)
    c++;
  return(c);
}

/*
 *	Delete bound variables and their bindings from var_tab and
 *	term_tab tables.
 */
static BOOL delete_bound(PURE_PROBLEM *prob, BINDING bind_tab[])
{
  VAR_HEADER *var_tab = prob->var_tab;
  int n_var = prob->n_var;
  int i;
  TERM *b;

  for(i = 0; i < n_var; i++){
    b = bind_tab[var_tab[i].var].value;
    if(b != NULL){
      if(!delete_binding(prob, var_tab[i].vmult, b))
        return(FALSE);
      var_tab[i].var = DEAD;
    }
  }
  delete_dead_vars(prob);
  delete_dead_terms(prob);
  if(prob->n_var == 0){
    if(prob->n_term != 0)
      return(FALSE); 	/* no variables to assign terms to */
  }
  else{
    if(prob->n_term == 0)
      return(FALSE);	/* no terms to assign to variables */
  }
  return(TRUE);
}

/*
 *	Delete terms bound to eleminated variable from term_tab
 */
static BOOL delete_binding(PURE_PROBLEM *prob, int vmult[], TERM *b)
{
  TERM_HEADER *term_tab = prob->term_tab;
  int *top_sym = prob->top_sym;
  int n_pure = prob->n_pure;
  int n_term = prob->n_term;
  int j, k, t, m, l;
  AC_LIST *p;
  BOOL ac_binding;
 
  ac_binding = (BOOL) (b->type == AC_COMPRESSED); /* beware of ac nesting */
  for(j = 0; j < n_term; j++){
    if(eker_term_cmp(term_tab[j].term, b) == 0)
      goto got_term;
  }
  if(!ac_binding)
    return(FALSE);
got_term:
  for(k = 0; k < n_pure; k++){
    m = vmult[k];
    if(m != 0){
      if(ac_binding){
        if(b->sym == top_sym[k]){	/* binding caused ac nesting */
          for(p = b->rest.a.ac_list; p; p = p->next_ac){
            for(l = 0; l < n_term; l++){
              if(eker_term_cmp(term_tab[l].term, p->arg) == 0)
                goto got_subterm;
            }
            return(FALSE);
got_subterm:
            t = term_tab[l].tmult[k] - m * p->mult;
            if(t < 0)
              return(FALSE);
            term_tab[l].tmult[k] = t;
          }
          continue;	/* next pure equation */
        }
        else{	/* ac binding but no ac nesting */
          if(j ==  n_term)
            return(FALSE);
        }
      }
      t = term_tab[j].tmult[k] - m;
      if(t < 0)
        return(FALSE);
      term_tab[j].tmult[k] = t;
    }
  }
  return(TRUE);
}

/*
 *	Remove dead columns from var_tab
 */
static void delete_dead_vars(PURE_PROBLEM *prob)
{
  VAR_HEADER *var_tab = prob->var_tab;
  int n_var = prob->n_var;
  int i, j;

  for(i = j =  0; i < n_var; i++){
    if(var_tab[i].var != DEAD){
      if(j != i)
        var_tab[j] = var_tab[i];
      j++;
    }
    else {
      EFREE(var_tab[i].vmult);
    }
  }
  prob->n_var = j;
}

/*
 *	Remove dead columns from term_tab
 */
static void delete_dead_terms(PURE_PROBLEM *prob)
{
  TERM_HEADER *term_tab = prob->term_tab;
  int n_pure = prob->n_pure;
  int n_term = prob->n_term;
  int i, j, k;

  for(i = j = 0; i < n_term; i++){
    for(k = 0; k < n_pure; k++){
      if(term_tab[i].tmult[k] != 0){
        if(j != i)
          term_tab[j] = term_tab[i];
        j++;
        goto not_dead;
      }
    }
    EFREE(term_tab[i].tmult);
not_dead:;
  }
  prob->n_term = j;
}

/*
 *	For each variable, check to see if it is uniquely owned
 *	by an AC function symbol or whether it is shared. Create
 *	assignment arrays for uniquely owned variables. If there
 *	are any shared variables compute index lists
 */
static void build_assign(PURE_PROBLEM *prob)
{
  VAR_HEADER *var_tab = prob->var_tab;
  int *top_sym = prob->top_sym;
  int n_pure = prob->n_pure;
  int n_var = prob->n_var;
  int n_term = prob->n_term;
  int i, j, k, *vmult;
  BOOL shared_vars;

  shared_vars = FALSE;
  for(i = 0; i < n_var; i++){
    k = UNUSED;
    vmult = var_tab[i].vmult;
    for(j = 0; j < n_pure; j++){
      if(vmult[j] != 0){
        if(k == UNUSED)
          k = top_sym[j];
        else if(k != top_sym[j]){
          k = SHARED;
          shared_vars = TRUE;
          break;
        }
      }
    }
    ASSERT(k != UNUSED, "build_assign(): variable unused");
    var_tab[i].owner = k;
  }
/*
 *	Find the largest multiplicity for each variable
 */
  for(i = 0; i < n_var; i++){
    k = 0;
    vmult = var_tab[i].vmult;
    for(j = 0; j < n_pure; j++){
      if(vmult[j] > k)
        k = vmult[j];
    }
    var_tab[i].max_mult = k;
  }
/*
 *	Sort variables so that shared variables are satisfied first,
 *	then those with greatest max multiplicity
 */
  if(n_var > 1)
    qsort((char *) var_tab, n_var, sizeof(VAR_HEADER), var_cmp);
/*
 *	Should optimize pure AC system here (& propogate constraints?)
 */
  for(i = 0; i < n_var; i++){
    if(var_tab[i].owner != SHARED)
      var_tab[i].ass.o.assign = CALLOC(n_term, ASSIGN);
  }
  if(shared_vars)
    compute_index_lists(prob);
}

/*
 *	Return -ve if we think variable *i is harder to satisfy than
 *	variable *j; +ve for vicervera and 0 for don't know.
 */
static int var_cmp(const void *pi, const void *pj)
{
  const VAR_HEADER *i = pi, *j = pj;
  if(i->owner == SHARED){
    if(j->owner != SHARED)
      return(-1);
  }
  else{
    if(j->owner == SHARED)
      return(1);
  }
  return(j->max_mult - i->max_mult);
}

/*
 *	For each term which in term_tab which has an AC top symbol
 *	compute a list of the indices (into term_tab) of its subterms
 *	if they all exist in term_tab.
 *	We need this list to find out what a shared variable should bind to
 *	in an AC equation with AC symbol foo if it gets bound to a term
 *	with top symbol foo in another AC equation with AC symbol bar.
 */
static void compute_index_lists(PURE_PROBLEM *prob)
{
  TERM_HEADER *term_tab = prob->term_tab;
  int n_term = prob->n_term;
  int i, j;
  AC_LIST *p;
  INDEX_LIST *list, *t;

  for(i = 0; i < n_term; i++){
    if(term_tab[i].term->type == AC_COMPRESSED){
      list = NULL;
      for(p = term_tab[i].term->rest.a.ac_list; p; p = p->next_ac){
        for(j = 0; j < n_term; j++){
          if(j != i && eker_term_cmp(term_tab[j].term, p->arg) == 0)
            goto got_subterm;
        }
        free_index_list(list);
        list = NULL;
        break;
got_subterm:
        t = EMALLOC(INDEX_LIST);
        t->next_index = list;
        t->index = j;
        t->smult = p->mult;
        list = t;
      }
      term_tab[i].subterms = list;
    }
  }
}

/*
 *	Free all storage allocted to pure problem structure
 */
void destroy_pure(PURE_PROBLEM *prob)
{
  VAR_HEADER *var_tab;
  TERM_HEADER *term_tab;
  int n_var, n_term, i;

  /* 
   * 17/08/98 : modif de PEM
   * destruction des structures par le GC
   */
  //prob=NULL; return;

  if(prob == NULL)
    return;
  var_tab = prob->var_tab;
  term_tab = prob->term_tab;
  n_var = prob->n_var;
  n_term = prob->n_term;
  for(i = 0; i < n_term; i++){
    EFREE(term_tab[i].tmult);
    if(term_tab[i].subterms)
      free_index_list(term_tab[i].subterms);
  }
  for(i = 0; i < n_var; i++){
    EFREE(var_tab[i].vmult);
    if(var_tab[i].owner != SHARED) {
      EFREE(var_tab[i].ass.o.assign);
    }
  }
  EFREE(term_tab);
  EFREE(var_tab);
  EFREE(prob->top_sym);
  EFREE(prob);
}

