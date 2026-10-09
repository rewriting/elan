/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *	Solve system of pure equations.
 */
#include "defs.h"
#include "term_types.h"
#include "pure_types.h"

static BOOL advance_shared(PURE_PROBLEM *prob, int v, BOOL reset);
static void update_shared(PURE_PROBLEM *prob, int v, int t, int d);
static BOOL shared_last(PURE_PROBLEM *prob, int v);
static BOOL in_index_list(INDEX_LIST *l, int i);
static BOOL advance_owned(PURE_PROBLEM *prob, int v, BOOL reset);
static BOOL advance_select(PURE_PROBLEM *prob, int v, int r, BOOL reset);
static BOOL owned_last(PURE_PROBLEM *prob, int v);

/*
 *	Find all matching assignments to pure AC system by iterative
 *	version of a backtracking search. Results are returned by side-effect
 *	in prob->ass. Assigned terms are subtracted from prob->term_tab on
 *	success and restored on failure (except for those assigned to last
 *	variable).
 */
BOOL solve_pure(PURE_PROBLEM *prob, BOOL reset)
{
  VAR_HEADER *var_tab = prob->var_tab;
  int n_var = prob->n_var;
  int i;
  BOOL r;

  if(reset){
    i = -1;
forward:
    for(i++; i < n_var; i++){
      r = (var_tab[i].owner == SHARED) ?
        advance_shared(prob, i, TRUE) : advance_owned(prob, i, TRUE);
      if(!r)
        goto backtrack;
    }
  return(TRUE);
  }
  else{
    i = n_var;
backtrack:
    for(i--; i >= 0; i--){
      r = (var_tab[i].owner == SHARED) ?
        advance_shared(prob, i, FALSE) : advance_owned(prob, i, FALSE);
      if(r)
        goto forward;
    }
    return(FALSE);
  }
}

/*
 *	Try to find assignment for shared variable v.
 */
static BOOL advance_shared(PURE_PROBLEM *prob, int v, BOOL reset)
{
  VAR_HEADER *var_tab = prob->var_tab;
  TERM_HEADER *term_tab = prob->term_tab;
  int *top_sym = prob->top_sym;
  int n_pure = prob->n_pure;
  int n_term = prob->n_term;
  int *vmult = var_tab[v].vmult;
  int cur, i, m, sym;
  INDEX_LIST *p;

  if(v == prob->n_var - 1)
    return(reset ? shared_last(prob, v) : FALSE);
  if(reset)
    cur = -1;
  else{
    cur = var_tab[v].ass.s.cur_ass;
    update_shared(prob, v, cur, 1);	/* retract old assignment */
  }

  for(cur++; cur < n_term; cur++){
    sym = term_tab[cur].term->sym;
    for(i = 0; i < n_pure; i++){
      m = vmult[i];
      if(m != 0){
        if(top_sym[i] == sym){	/* check for AC nesting */
          p = term_tab[cur].subterms;
          if(p == NULL)
            goto fail;
          for(; p; p = p->next_index){
            if(term_tab[p->index].tmult[i] < m * p->smult)
              goto fail;
          }
        }
        else{
          if(term_tab[cur].tmult[i] < m)
            goto fail;
        }
      }
    }
    update_shared(prob, v, cur, -1);
    var_tab[v].ass.s.cur_ass = cur;
    return(TRUE);
fail:;
  }
  return(FALSE);
}

/*
 *	Update term_tab multiplicities after assignment to shared variable
 *	made or retracted.
 */
static void update_shared(PURE_PROBLEM *prob, int v, int t, int d)
{
  TERM_HEADER *term_tab = prob->term_tab;
  int *top_sym = prob->top_sym;
  int *vmult = prob->var_tab[v].vmult;
  int n_pure = prob->n_pure;
  int sym = term_tab[t].term->sym;
  int i, m;
  INDEX_LIST *p;

  for(i = 0; i < n_pure; i++){
    m = vmult[i];
    if(m != 0){
      if(d < 0)
        m = -m;
      if(top_sym[i] == sym){	/* check for AC nesting */
        for(p = term_tab[t].subterms; p; p = p->next_index)
          term_tab[p->index].tmult[i] += m * p->smult;
      }
      else
        term_tab[t].tmult[i] += m;
    }
  }
}

/*
 *	See if there is a compatible assignment the final variable
 *	which happens to be shared (should be a rare occurrence).
 *	Used up terms are never subtracted from term_tab for last last
 *	variable as there is no need.
 */
static BOOL shared_last(PURE_PROBLEM *prob, int v)
{
  TERM_HEADER *term_tab = prob->term_tab;
  VAR_HEADER *var_tab = prob->var_tab;
  int *top_sym = prob->top_sym;
  int n_pure = prob->n_pure;
  int n_term = prob->n_term;
  int *vmult = var_tab[v].vmult;
  int i, j, cur, m, sym;
  INDEX_LIST *p, *indices;

  for(i = 0; i < n_pure; i++){
    if(vmult[i] == 0){
      for(j = 0; j < n_term; j++){
        if(term_tab[j].tmult[i] != 0)
          return(FALSE);
      }
    }
  }

  for(cur = 0; cur < n_term; cur++){
    sym = term_tab[cur].term->sym;
    for(i = 0; i < n_pure; i++){
      m = vmult[i];
      if(m != 0){
        if(top_sym[i] == sym){
          indices = term_tab[cur].subterms;
          if(indices == NULL)
            goto fail;
          for(p = indices; p; p = p->next_index){
            if(term_tab[p->index].tmult[i] != m * p->smult)
              goto fail;
          }
          for(j = 0; j < n_term; j++){
            if(!in_index_list(indices, j) && term_tab[j].tmult[i] != 0)
              goto fail;
          }
        }
        else{
          if(term_tab[cur].tmult[i] != m)
            goto fail;
          for(j = 0; j < n_term; j++){
            if(j != cur && term_tab[j].tmult[i] != 0)
              goto fail;
          }
        }
      }
    }
    var_tab[v].ass.s.cur_ass = cur;
    return(TRUE);
fail:;
  }
  return(FALSE);
}

/*
 *	Check to see if index i occurs in list l.
 */
static BOOL in_index_list(INDEX_LIST *l, int i)
{
  for(; l; l = l->next_index){
    if(l->index == i)
      return(TRUE);
  }
  return(FALSE);
}

#define UNDEF		(-1)

/*
 *	Try to find assignment for owned variable v.
 */
static BOOL advance_owned(PURE_PROBLEM *prob, int v, BOOL reset)
{
  VAR_HEADER *var_tab = prob->var_tab;
  TERM_HEADER *term_tab = prob->term_tab;
  int n_pure = prob->n_pure;
  int n_term = prob->n_term;
  ASSIGN *assign = var_tab[v].ass.o.assign;
  int *vmult = var_tab[v].vmult;
  int i, j, m, t, size, ass, cur_size;

  if(v == prob->n_var - 1)
    return(reset ? owned_last(prob, v) : FALSE);
  if(reset){
    size = 0;
    for(j = 0; j < n_term; j++){
      ass = UNDEF;
      for(i = 0; i < n_pure; i++){
        m = vmult[i];
        if(m != 0){
          t = term_tab[j].tmult[i] / m;
          if(t == 0){
            ass = 0;
            break;
          }
          if(ass == UNDEF || ass > t)
            ass = t;
        }
      }
      assign[j].sum_prev = size;
      assign[j].max = ass;
      size += ass;
    }
    if(size == 0)
      return(FALSE);
    var_tab[v].ass.o.max_size = size;
    cur_size = 0;
  }
  else{
    cur_size = var_tab[v].ass.o.cur_size;
    if(advance_select(prob, v, cur_size, FALSE))
      return(TRUE);
    size = var_tab[v].ass.o.max_size;
  }

  for(cur_size++; cur_size <= size; cur_size++){
    if(advance_select(prob, v, cur_size, TRUE)){
      var_tab[v].ass.o.cur_size = cur_size;
      return(TRUE);
    }
  }
  return(FALSE);
}

#define UPDATE(i, d)	for(j = 0; j < n_pure; j++) \
		 	  term_tab[i].tmult[j] += (d) * vmult[j]

/*
 *	Try to find next assignment of size r for owned variable v from
 *	the multiset of terms in prob->var_tab[v].assign[].max
 *	We use a nonrecursive selection from multiset algorithm.
 */
static BOOL advance_select(PURE_PROBLEM *prob, int v, int r, BOOL reset)
{
  VAR_HEADER *var_tab = prob->var_tab;
  TERM_HEADER *term_tab = prob->term_tab;
  int n_pure = prob->n_pure;
  int n_term = prob->n_term;
  int *vmult = var_tab[v].vmult;
  ASSIGN *assign = var_tab[v].ass.o.assign;
  int i, j, t;

  if(reset)
    i = n_term;
  else{
    r = assign[0].cur;
    UPDATE(0, r);	/* retract first part of old assignment */
    for(i = 1; i < n_term; i++){
      t = assign[i].cur;
      if(r > 0 && t < assign[i].max){
        assign[i].cur = t + 1;
        UPDATE(i, -1);	/* assert part of new assignment */
        r--;
        goto forward;
      }
      r += t;
      UPDATE(i, t);	/* retract next part of old assignment */
    }
    return(FALSE);
  }

forward:
  for(i--; i > 0 && assign[i].sum_prev >= r; i--)
    assign[i].cur = 0;
  t = -(assign[i].cur = r - assign[i].sum_prev);
  UPDATE(i, t);		/* assert part of new assignment */
  for(i--; i >= 0; i--){
    t = -(assign[i].cur = assign[i].max);
    UPDATE(i, t);	/* assert part of new assignment */
  }
  return(TRUE);
}

/*
 *	See if there is a compatible assignment to the final variable
 *	which happen to be owned (most usual case). Used up terms are
 *	not subtracted from term_tab.
 */
static BOOL owned_last(PURE_PROBLEM *prob, int v)
{
  VAR_HEADER *var_tab = prob->var_tab;
  TERM_HEADER *term_tab = prob->term_tab;
  int n_pure = prob->n_pure;
  int n_term = prob->n_term;
  int *vmult = var_tab[v].vmult;
  ASSIGN *assign = var_tab[v].ass.o.assign;
  int a, i, j, m, t, size, *tmult;

  size = 0;
  for(j = 0; j < n_term; j++){
    a = UNDEF;
    tmult = term_tab[j].tmult;
    for(i = 0; i < n_pure; i++){
      m = vmult[i];
      t = tmult[i];
      if(m == 0){
        if(t != 0)
          return(FALSE);
      }
      else{
        if((t % m) != 0)
          return(FALSE);
        t /= m;
        if(a == UNDEF)
          a = t;
        else if(a != t)
          return(FALSE);
      }
    }
    assign[j].cur = a;
    size += a;
  }
  if(size == 0)
    return(FALSE);
  var_tab[v].ass.o.cur_size = size;
  return(TRUE);
}

