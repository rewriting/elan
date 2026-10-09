/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *	Interface to matcher
 */
#include "defs.h"
#include "term_types.h"
#include "free_types.h"
#include "pure_types.h"
#include "state_types.h"
#include "functions.h"

static BOOL find_match();
static TERM *ac_uncompress();

/*
 *	Build a match object
 */
void *build_match(TERM *p, TERM *s, int tot_var) {
  MATCH_OBJECT *m = EMALLOC(MATCH_OBJECT);
  int i;
  BINDING *b;
  /* 
  flatten(p);
  flatten(s);
  ac_compress(p);
  ac_compress(s);
  */

  m->orig_pat = p;
  m->orig_sub = s;
  m->total_var = tot_var;
  m->bind_tab = b = CALLOC(tot_var, BINDING);
  for(i = 0; i < tot_var; i++){
    b[i].value = NULL;
    b[i].n_times = 0;
  }
  m->pure_stack = NULL;
  m->pure_prob = NULL; /* user might destoy_match() before solve_pure() */
  m->first = TRUE;
  if(!build_free(m, p, s, &(m->free_prob))){
    //destroy_term(p);
    destroy_term(s);
    EFREE(m->bind_tab);
    EFREE(m);
    return((void *) NULL);
  }
  return((void *) m);
}

/*
 *	Extract a match
 */
static BOOL find_match(m)
MATCH_OBJECT *m;
{
  if(m == NULL)
    return(FALSE);
  if(m->first){
    m->first = FALSE;
    if(m->free_prob == NULL){
      return(TRUE);
    }
    if(!solve_free(m, m->free_prob, TRUE))
      return(FALSE);
    if(build_pure(m)){
      if(m->pure_prob == NULL || solve_pure(m->pure_prob, TRUE))
        return(TRUE);
      destroy_pure(m->pure_prob);
    }
  }
  else{
    if(m->free_prob == NULL)
      return(FALSE);
    if(m->pure_prob != NULL && solve_pure(m->pure_prob, FALSE))
      return(TRUE);
    destroy_pure(m->pure_prob);
  }
  while(solve_free(m, m->free_prob, FALSE)){
    if(build_pure(m)){
      if(m->pure_prob == NULL || solve_pure(m->pure_prob, TRUE))
        return(TRUE);
      destroy_pure(m->pure_prob);
    }
  }
  m->pure_prob = NULL; /* don't want to free the same pure_prob twice */
  return(FALSE);
}

BOOL extract_match(vm, assignment)
void *vm;
TERM *assignment[];
{
  MATCH_OBJECT *m = (MATCH_OBJECT *) vm;
  int i, j, t, n_var, n_term, a;
  VAR_HEADER *var_tab;
  TERM_HEADER *term_tab;
  TERM *p;
  TERM_LIST *head, *tail;

  if(m == NULL) {
    return(FALSE);
  }
  if(!find_match(m)) {
    return(FALSE);
  }

  for(i = 0; i < m->total_var; i++)
    if(m->bind_tab[i].value != NULL)
      assignment[i] = ac_uncompress(m->bind_tab[i].value);
    else
      /* Laurent 31.12.97 : BUG found */
      assignment[i] = NULL;
  if(m->pure_prob == NULL)
    /* PEM 09.97 : BUG found: return; */
    return(TRUE);
  n_var = m->pure_prob->n_var;
  n_term = m->pure_prob->n_term;
  var_tab = m->pure_prob->var_tab;
  term_tab = m->pure_prob->term_tab;
  for(j = 0; j < n_var; j++){
    if(var_tab[j].owner == SHARED){
      assignment[var_tab[j].var] =
        ac_uncompress(term_tab[var_tab[j].ass.s.cur_ass].term);

    }
    else if(var_tab[j].ass.o.cur_size == 1){
      for(i = 0; i < n_term; i++){
        if(var_tab[j].ass.o.assign[i].cur != 0)
          break;
      }
      assignment[var_tab[j].var] = ac_uncompress(term_tab[i].term);
    }
    else{
      head = tail = NULL;
      a = 0;
      for(i = 0; i < n_term; i++){
        for(t = var_tab[j].ass.o.assign[i].cur; t; t--){
          if(tail == NULL)
            tail = head = EMALLOC(TERM_LIST);
          else
            tail = tail->next_arg = EMALLOC(TERM_LIST);
          tail->arg = ac_uncompress(term_tab[i].term);
          a++;
        }
      }
      tail->next_arg = NULL;
      p = EMALLOC(TERM);
      p->type = AC_NORMAL;
      p->sym = var_tab[j].owner;
      p->rest.f.list_len = a;
      p->rest.f.arg_list = head;
      p->rest.f.arg_tail = tail;
      assignment[var_tab[j].var] = p;
    }
  }
  return(TRUE);
}
   
static TERM *ac_uncompress(t)
TERM *t;
{
  TERM *p = EMALLOC(TERM);
  TERM_LIST *q, *head, *tail;
  AC_LIST *r;
  int i;

  switch(t->type){
  case VARIABLE:
  case CONSTANT:
  case BUILTIN:
    *p = *t;
    break;
  case FUNCTION:
  case AC_NORMAL:
    p->type = t->type;
    p->sym = t->sym;
    head = tail = NULL;
    for(q = t->rest.f.arg_list; q; q = q->next_arg){
      if(tail == NULL)
        tail = head = EMALLOC(TERM_LIST);
      else
        tail = tail->next_arg = EMALLOC(TERM_LIST);
      tail->arg = ac_uncompress(q->arg);
    }
    tail->next_arg = NULL;
    p->rest.f.list_len = t->rest.f.list_len;
    p->rest.f.arg_list = head;
    p->rest.f.arg_tail = tail;
    break;
  case AC_COMPRESSED:
    p->type = AC_NORMAL;
    p->sym = t->sym;
    head = tail = NULL;
    for(r = t->rest.a.ac_list; r; r = r->next_ac){
      for(i = r->mult; i; i--){
        if(tail == NULL)
          tail = head = EMALLOC(TERM_LIST);
        else
          tail = tail->next_arg = EMALLOC(TERM_LIST);
        tail->arg = ac_uncompress(r->arg);
      }
    }
    tail->next_arg = NULL;
    p->rest.f.list_len = t->rest.a.arg_count;
    p->rest.f.arg_list = head;
    p->rest.f.arg_tail = tail;
    break;
  }
  return(p);
}

/*
 *	destroy object and free storage
 */
void destroy_match(vm)
void *vm;
{
  MATCH_OBJECT *m = (MATCH_OBJECT *) vm;

  /* 
   * 17/08/98 : modif de PEM
   * destruction des structures par le GC
   */
  //m = NULL; return;

  if(m == NULL)
    return;
  destroy_free(m->free_prob);
  destroy_pure(m->pure_prob);
  //destroy_term(m->orig_pat);
  m->orig_pat=NULL;
  destroy_term(m->orig_sub);
  EFREE(m->bind_tab);
  EFREE(m);
}

