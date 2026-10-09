#include <stdlib.h>
#include "eker_tools.h"
#include "builtin.h"
#include "defs.h"

TERM *toEkerForm(struct term *t) {
  int i,arity;
  TERM_LIST *tl;
  AC_LIST *acl=NULL;

  Verif_void(t,"toEkerFrom(t)");
  //printf("toEkerForm\n");

  if(isTagged(t)) {
    /*
     * on passe directement la valeur taggee
     */
    return make_term((long)t,NULL,BUILTIN);
  } 

  if((arity=term_arity(t))==0) {
    //printf("constant: (%d)\n",getSymb(t)); 
    return make_term(getSymb(t),NULL,CONSTAN);
  }

  if(!term_isAC(t)) {
    i=0;
    tl=make_term_list(toEkerForm(getFreeSubterm(t,i)),(TERM_LIST*) NULL);
    i++;
    while (i<arity) {
      tl = make_term_list(toEkerForm(getFreeSubterm(t,i)),tl);
      i++;
    }
    return make_term(getSymb(t),tl,FUNC);
  } else {
    struct termac *tac=(struct termac*)t;
    int i;
    
    if(getArity(tac)==0) {
      printf("Warning [toEkerForm]: empty term !\n");
      exit(0);
    } else {
      for(i=0, acl=(AC_LIST*)NULL ; i<getArity(tac) ; i++) {
        acl = make_ac_list(toEkerForm(getSubterm(tac,i)),(int)getMult(tac,i),acl);
      }
    }
    return make_ac_term(getSymb(t),acl,ACFUNC);
  }
}

struct term *fromEkerForm(TERM * t) {
  TERM_LIST *l;
  int i,arity;
  struct term *res;

  Verif_void(t,"fromEkerFrom(t)");

  switch(t->type) {
  case VARIABLE:
    printf("internal error: assignment is not a ground term !\n");
    exit(0);
    break; 
  case BUILTIN:
    /*
     * on recupere la valeur eventuellement taggee
     */
    res=(struct term*)t->sym;
    break;
    case CONSTANT:
      TERM_CONST_ALLOC(res,t->sym);
      break;
  case FUNCTION:
      arity = fsymtab[t->sym].arity;
      TERM_ARITY_ALLOC(res,arity,t->sym);
      l = t->rest.f.arg_list;
      for(i=arity-1 ; l != NULL ; l=l->next_arg, i--) {
	  setFreeSubterm(res,i,fromEkerForm(l->arg));
      }
      break;
  case AC_NORMAL: {
    struct termac *resac=NULL;
      // [pem: Jun 15 00] inutile avec le nouveau add_onf
      // TERMAC_ALLOC(resac,i,t->sym);
    l = t->rest.f.arg_list;
    for(i=0 ; l != NULL ; l=l->next_arg) {
      resac=term_add_onf_term_color(resac,t->sym,fromEkerForm(l->arg),i++);
    }
    res=(struct term*)resac;
    break;
  }
  default:
      fprintf(stderr,"internal error: non expected case in print_term");
      exit(0);
  }
  return res;
}

TERM *make_term(int id, TERM_LIST *args, SYM_TYPE type) {
  TERM *t;
  TERM_LIST *p;                              /*  tail of args  */
  int len;                                   /* lenght of args */

  t = EMALLOC(TERM);
  t->sym = id;
  if(args == NULL){
    if(type == VAR){
      t->rest.v.var_nr = id;               /*  to code variables 1..N, symbols N.. */
      t->type = VARIABLE;
    }
    else{
      if(type == BUILTIN) {
        t->type = BUILTIN;
      } else {
        t->type = CONSTANT;
      }
    }
  }
  else{
      for(p = args, len = 1; p->next_arg; p = p->next_arg) {
	len++;
      }
    t->rest.f.list_len = len;
    t->rest.f.arg_list = args;
    t->rest.f.arg_tail = p;
    if(type == FUNC) t->type = FUNCTION;
    else             t->type = AC_NORMAL;
  }
  return(t);
}


TERM_LIST *make_term_list(TERM *new, TERM_LIST *rest) {
  TERM_LIST *l = EMALLOC(TERM_LIST);
  l->arg = new;
  l->next_arg = rest;
  return(l);
}

TERM *make_ac_term(int id, AC_LIST *args,SYM_TYPE type) {
  TERM *t = EMALLOC(TERM);
  AC_LIST *p;                              /*  tail of args  */
  int len;                                   /* lenght of args */

  ASSERT(type != AC_NORMAL, "make_ac_term() - not an AC symbol");

  t->sym = id;
  t->type = AC_COMPRESSED;
  for(p = args, len=1; p->next_ac; p = p->next_ac) {
    len++;
  }
  t->rest.a.arg_count = len;
  t->rest.a.ac_list = args;
  t->rest.a.ac_tail = p;
  return(t);
}

AC_LIST *make_ac_list(TERM *new, int multiplicity, AC_LIST *rest) {
  AC_LIST *l = EMALLOC(AC_LIST);
  l->arg = new;
  l->mult = multiplicity;
  l->next_ac = rest;
  return(l);
}

void eker_print_term(TERM *t) {
  if(t == NULL){
    printf("(null ptr)");
    return;
  }
  switch(t->type) {
  case VARIABLE:
    printf("VAR(%d)", t->sym);
    break;
  case CONSTANT:
    printf("CST(%d)", t->sym);
    break;
  case BUILTIN:
    printf("BI(%d)", t->sym);
    break;

  case FUNCTION:
    printf("SYM(%d)(", t->sym);
    eker_print_tlist(t->rest.f.arg_list);
    printf(")");
    break;
  case AC_NORMAL:
    printf("ACSYM(%d)(", t->sym);
    eker_print_tlist(t->rest.f.arg_list);
    printf(")");
    break;
  case AC_COMPRESSED:
    //printf("ACC(%d)(", t->sym);
    printf("ACC(%d)[count=%d](", t->sym,t->rest.a.arg_count );
    eker_print_aclist(t->rest.a.ac_list);
    printf(")");
    break;
  }
}

void eker_print_tlist(TERM_LIST *l) {
  while(l){
    eker_print_term(l->arg);
    l = l->next_arg;
    if(l != NULL)
      printf(", ");
  }
}

void eker_print_aclist(AC_LIST *l) {
  while(l!=NULL) {
    printf("%d*", l->mult);
    eker_print_term(l->arg);
    l = l->next_ac;
    if(l != NULL)
      printf(", ");
  }
}
