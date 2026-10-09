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
 *    flattening/sorting routines for AC terms
 */
#include "defs.h"
#include "term_types.h"
#include "free_types.h"
#include "pure_types.h"
#include "state_types.h"
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
        EFREE(p->arg);
        EFREE(p);
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
  BOOL ac=FALSE;
  TERM_LIST *p, *p2;
  AC_LIST *q;
  TERM *c;

  ASSERT(t->type != AC_COMPRESSED,
         "ac_compress() - argument not in normal form");
  switch(t->type){
  case VARIABLE:
  case CONSTANT:
  case BUILTIN:
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
    /*
     * PEM 31.10.98 : dangerous !!!
     */
    t->rest.a.arg_count = t->rest.f.list_len;
    t->rest.a.ac_list = (q = EMALLOC(AC_LIST));
    q->arg = (c = p->arg);
    q->mult = 1;
    p2 = p->next_arg;
    EFREE(p);
    for(p = p2; p; p = p2){
      if(eker_term_cmp(c, p->arg) == 0){
        (q->mult)++;
        destroy_term(p->arg);
      }
      else{
        q = (q->next_ac = EMALLOC(AC_LIST));
        q->arg = (c = p->arg);
        q->mult = 1;
      }
      p2 = p->next_arg;
      EFREE(p);
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
  if(eker_term_cmp(p->arg, p2->arg) >= 0){
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
    if(eker_term_cmp(p->arg, p2->arg) >= 0){
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
int eker_term_cmp(TERM *t, TERM *t2)
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
  if(type == CONSTANT || type == VARIABLE || type == BUILTIN)
    return(0);
  if(type == FUNCTION){ /* lexographic ordering on subterms */
    for(p = t->rest.f.arg_list, p2 = t2->rest.f.arg_list; ;
      p = p->next_arg, p2 = p2->next_arg){
        if(p == NULL)
          return(p2 == NULL ? 0 : (-1));
        if(p2 == NULL)
          return(1);
        r = eker_term_cmp(p->arg, p2->arg);
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
        r = eker_term_cmp(q->arg, q2->arg);
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

  /*
   * PEM 02/11/98 : ne pas detruire les termes
   */
    //return;

  switch(t->type){
  case VARIABLE:
  case CONSTANT:
  case BUILTIN:
    break;
  case FUNCTION:
  case AC_NORMAL:
    for(p = t->rest.f.arg_list; p; p = p2){
      destroy_term(p->arg);
      p2 = p->next_arg;
      EFREE(p);
    }
    break;
  case AC_COMPRESSED:
    for(q = t->rest.a.ac_list; q; q = q2){
      destroy_term(q->arg);
      q2 = q->next_ac;
      EFREE(q);
    }
    break;
  default:
    ASSERT(FALSE, "destroy_term(): unknown term type");
  }
  EFREE(t);
}


/*
 *	Sort argument lists in descending order
 */
static AC_LIST *ac_merge_sort(AC_LIST *p, int len)
{
  AC_LIST *p2, *q, *base;
  int i, l;

  if(len <= 1)
    return(p);
  l = len / 2;
  for(q = p, i = l - 1; i > 0; i--)
    q = q->next_ac;
  p2 = q->next_ac;
  q->next_ac = NULL;
  p = ac_merge_sort(p, l);
  p2 = ac_merge_sort(p2, len - l);
  if(eker_term_cmp(p->arg, p2->arg) >= 0){
    q = p;
    p = p->next_ac;
    if(p == NULL){
      q->next_ac = p2;
      return(q);
    }
  }
  else{
    q = p2;
    p2 = p2->next_ac;
    if(p2 == NULL){
      q->next_ac = p;
      return(q);
    }
  }
  base = q;
  for(;;){
    if(eker_term_cmp(p->arg, p2->arg) >= 0){
      q = (q->next_ac = p);
      p = p->next_ac;
      if(p == NULL){
        q->next_ac = p2;
        break;
      }
    }
    else{
      q = (q->next_ac = p2);
      p2 = p2->next_ac;
      if(p2 == NULL){
        q->next_ac = p;
        break;
      }
    }
  }
  return(base);
}

/*
 * permet de trier les arguments
 */
void ac_sort(TERM *t) {
  AC_LIST *p, *q;
  TERM_LIST *tl;
  int len;

  switch(t->type){
  case VARIABLE:
  case CONSTANT:
  case BUILTIN:
    return;
  case FUNCTION:
      for(tl=t->rest.f.arg_list; tl ; tl=tl->next_arg) {
	  ac_sort(tl->arg);
      }
      return;
  case AC_COMPRESSED:
      for(p=t->rest.a.ac_list; p ; p=p->next_ac) {
	  ac_sort(p->arg);
      }
        /*
         * len correspond au nombre de sous termes
         * sans prendre en compte les multiplicites
         */
      len = t->rest.a.arg_count;
      //printf("len=%d\n",len);
      p=ac_merge_sort(t->rest.a.ac_list, len);

      for(q=p, len=0 ; q->next_ac!=NULL ; q=q->next_ac) {
          /*
           * mise a jour de t->rest.a.arg_count
           */
        len+=q->mult;
      }
        // Ne pas oublier la derniere cellule !
      len+=q->mult;
        //printf("len=%d\n",len);
        /*
         * len correspond au nombre de sous termes
         * en prenant en compte les multiplicites
         */
      t->rest.a.arg_count=len;
      t->rest.a.ac_list = p;
      t->rest.a.ac_tail = q; /* do we ever use the tail? */
      return;
  default:
    fprintf(stderr,"Unknown type in flatten:ac_sort\n");
    exit(0);
  }
}
