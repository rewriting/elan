/*
		(c) 	Steven Eker, 1992
			INRIA-Lorraine & CRIN
        		615, rue du Jardin Botanique, BP 101
		        54602 Villers-les-Nancy
			France
*/
/*
 *    flattening/sorting routines for AC terms
 */
#include "defs.h"
#include "term_types.h"
#include "functions.h"

static TERM_LIST *merge_sort(TERM_LIST *p, int len);

/*
 *	flatten a term
 */
void flatten(TERM *t)
{
  TERM_LIST *p, *active;
  int count;
  TERM_TYPE type = t->type;

  ASSERT(type != AC_COMPRESSED, "flatten() - argument not in normal form");
  if(type != FUNCTION && type != AC_NORMAL)
    return;
  for(p = t->rest.f.arg_list; p; p = p->next_arg)
    flatten(p->arg);
  if(type != AC_NORMAL)
    return;
  count = 0;
  active = NULL;
  for(p = t->rest.f.arg_list; p; p = p->next_arg)
    if(p->arg->sym == t->sym){
      count += p->arg->rest.f.list_len;
        if(active != NULL)
          active->next_arg = p->arg->rest.f.arg_list;
        else
          t->rest.f.arg_list = p->arg->rest.f.arg_list;
        active = p->arg->rest.f.arg_tail;
        active->next_arg = p->next_arg;
        FREE(p->arg);
        FREE(p);
        p = active;	/* subtle - but correct! */
    }
    else{
      count++;
      active = p;
    }
  t->rest.f.arg_tail = active;
  t->rest.f.list_len = count;
}

void ac_compress(TERM *t)
{
  /* initialised to avoid warning */
  BOOL ac = FALSE;
  TERM_LIST *p, *p2;
  AC_LIST *q;
  TERM *c;

  ASSERT(t->type != AC_COMPRESSED,
         "ac_compress() - argument not in normal form");
  switch(t->type){
  case VARIABLE:
  case CONSTANT:
  case AC_COMPRESSED: /* added to avoid warning */
    return;
  case FUNCTION:
    ac = FALSE;
    break;
  case AC_NORMAL:
    ac = TRUE;
    break;
  }
  for(p = t->rest.f.arg_list; p; p = p->next_arg)
    ac_compress(p->arg);
  if(ac){
    p = merge_sort(t->rest.f.arg_list, t->rest.f.list_len);
    t->rest.a.arg_count = t->rest.f.list_len;
    t->rest.a.ac_list = (q = MALLOC(AC_LIST));
    q->arg = (c = p->arg);
    q->mult = 1;
    p2 = p->next_arg;
    FREE(p);
    for(p = p2; p; p = p2){
      if(term_cmp(c, p->arg) == 0){
        (q->mult)++;
        destroy_term(p->arg);
      }
      else{
        q = (q->next_ac = MALLOC(AC_LIST));
        q->arg = (c = p->arg);
        q->mult = 1;
      }
      p2 = p->next_arg;
      FREE(p);
    }
    q->next_ac = NULL;
    t->rest.a.ac_tail = q; /* do we ever use the tail? */
    t->type = AC_COMPRESSED;
  }
}

/*
 *	Sort argument lists in descending order
 */
static TERM_LIST *merge_sort(TERM_LIST *p, int len)
{
  TERM_LIST *p2, *q, *base;
  int i, l;

  if(len <= 1)
    return(p);
  l = len / 2;
  for(q = p, i = l - 1; i > 0; i--)
    q = q->next_arg;
  p2 = q->next_arg;
  q->next_arg = NULL;
  p = merge_sort(p, l);
  p2 = merge_sort(p2, len - l);
  if(term_cmp(p->arg, p2->arg) >= 0){
    q = p;
    p = p->next_arg;
    if(p == NULL){
      q->next_arg = p2;
      return(q);
    }
  }
  else{
    q = p2;
    p2 = p2->next_arg;
    if(p2 == NULL){
      q->next_arg = p;
      return(q);
    }
  }
  base = q;
  for(;;){
    if(term_cmp(p->arg, p2->arg) >= 0){
      q = (q->next_arg = p);
      p = p->next_arg;
      if(p == NULL){
        q->next_arg = p2;
        break;
      }
    }
    else{
      q = (q->next_arg = p2);
      p2 = p2->next_arg;
      if(p2 == NULL){
        q->next_arg = p;
        break;
      }
    }
  }
  return(base);
}

/*
 *	Compare flattened/sorted terms using lexicographic order for
 *	argument list of free function symbols and multiset order for
 *	argument lists of AC function symbols.
 */
int term_cmp(TERM *t, TERM *t2)
{
  TERM_TYPE type = t->type, type2 = t2->type;
  TERM_LIST *p, *p2;
  AC_LIST *q, *q2;
  int r;
  
  if(((int) type) < ((int) type2))
    return(-1);
  if(((int) type) > ((int) type2))
    return(1);
  if(t->sym < t2->sym)
    return(-1);
  if(t->sym > t2->sym)
    return(1);
  if(type == CONSTANT || type == VARIABLE)
    return(0);
  if(type == FUNCTION){ /* lexographic ordering on subterms */
    for(p = t->rest.f.arg_list, p2 = t2->rest.f.arg_list; ;
      p = p->next_arg, p2 = p2->next_arg){
        if(p == NULL)
          return(p2 == NULL ? 0 : (-1));
        if(p2 == NULL)
          return(1);
        r = term_cmp(p->arg, p2->arg);
        if(r != 0)
          return(r);
    }
  }
  else{ /* multiset ordering on subterms */
    for(q = t->rest.a.ac_list, q2 = t2->rest.a.ac_list; ;
      q = q->next_ac, q2 = q2->next_ac){
        if(q == NULL)
          return(q2 == NULL ? 0 : (-1));
        if(q2 == NULL)
          return(1);
        r = term_cmp(q->arg, q2->arg);
        if(r != 0)
          return(r);
        if(q->mult < q2->mult)
          return(-1);
        if(q->mult > q2->mult)
          return(1);
    }
  }
}

void destroy_term(TERM *t)
{
  TERM_LIST *p, *p2;
  AC_LIST *q, *q2;

  switch(t->type){
  case VARIABLE:
  case CONSTANT:
    break;
  case FUNCTION:
  case AC_NORMAL:
    for(p = t->rest.f.arg_list; p; p = p2){
      destroy_term(p->arg);
      p2 = p->next_arg;
      FREE(p);
    }
    break;
  case AC_COMPRESSED:
    for(q = t->rest.a.ac_list; q; q = q2){
      destroy_term(q->arg);
      q2 = q->next_ac;
      FREE(q);
    }
    break;
  default:
    ASSERT(FALSE, "destroy_term(): unknown term type");
  }
  FREE(t);
}

