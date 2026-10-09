#include <stdlib.h>
#include "eker_tools.h"
#include "builtin.h"

TERM *toEkerForm(struct term *t) {
  int i,arity, mult;
  TERM_LIST *l1,*l2;
  struct cell_term *p;

  Verif_void(t,"toEkerFrom(t)");
  //printf("toEkerForm\n");

  //printf("t = "); term_printnl(stdout,t);
  if(isTagged(t)) {
    // TODO
    //printf("*** toEkerForm(builtin) not yet implemented\n");
    //exit(0);
    /*
     * on passe directement la valeur taggee
     */
    printf("value = %d\n",(long)t);
    return make_term((long)t,NULL,CONSTAN,0,NULL);
  } 

  if((arity=term_arity(t))==0) {
    //printf("constant: (%d)\n",getSymb(t)); 
    return make_term(getSymb(t),NULL,CONSTAN,0,NULL);
  }

  if(!term_isAC(t)) {
    i=0;
    l1=l2=make_term_list(toEkerForm(t->sub[i++]),(TERM_LIST*) NULL);
    while (i<arity) {
      l2 = make_term_list(toEkerForm(t->sub[i++]),l2);
    }
    return make_term(getSymb(t),l2,FUNC,arity,l1);
  } else {
    p=term_first(t);
    if(p!=NULL) {
      arity=1;
      mult=getMult(p);
      //printf("mult = %d\n",mult);
      l1=l2=make_term_list(toEkerForm(cell_t(p)),(TERM_LIST*) NULL);
      mult--;
      while(mult>0) {
	l2 = make_term_list(toEkerForm(cell_t(p)),l2);
	mult--;
	arity++;
      }
      p=cell_next(p);
    } else {
      printf("Warning [toEkerForm]: p is NULL !\n");
      exit(0);
    }
    while(p!=NULL) {
      mult=getMult(p);
      //printf("mult = %d\n",mult);
      while(mult>0) {
	l2 = make_term_list(toEkerForm(cell_t(p)),l2);
	mult--;
	arity++;
      }
      p=cell_next(p);
    }
    //printf("arity = %d\n",arity);
    return make_term(getSymb(t),l2,ACFUNC,arity,l1);
  }
}

struct term *fromEkerForm(TERM * t) {
  TERM_LIST *l;
  int i,arity;
  struct term *res;

  Verif_void(t,"fromEkerFrom(t)");

  switch(t->type) {
  case VARIABLE:
    printf("internal error in fromEkerForm: not a ground term !\n");
    exit(0);
    break; 
  case CONSTANT:
    // TODO
      //printf("*** fromEkerForm(builtin) not yet implemented\n");
      //exit(0);
    /*
     * on recupere la valeur eventuellement taggee
     */
      TERM_CONST_ALLOC(res,t->sym);
      break;
  case FUNCTION:
      arity = fsymtab[t->sym].arity;
      TERM_ARITY_ALLOC(res,arity,t->sym);
      l = t->rest.f.arg_list;
      for(i=0 ; l != NULL ; l=l->next_arg) {
	  res->sub[i++]=fromEkerForm(l->arg);
      }
      break;
  case AC_NORMAL:
    TERM_ALLOC(res,term2,t->sym);
    l = t->rest.f.arg_list;
    for(i=0 ; l != NULL ; l=l->next_arg) {
	term_add_onf_term_color(res,fromEkerForm(l->arg),i++);
    }
    break;
  default:
      fprintf(stderr,"internal error: non expected case in print_term");
      exit(0);
  }
  //  printf("res = "); term_printnl(stdout,res);
  return res;
}

print_term(t)
TERM *t;
{
  if(t == NULL){
    printf("(null ptr)");
    return;
  }
  switch(t->type){
  case VARIABLE:
    printf("VAR(%d)", t->sym);
    break;
  case CONSTANT:
    printf("CST(%d)", t->sym);
    break;
  case FUNCTION:
    printf("%d(", t->sym);
    print_tlist(t->rest.f.arg_list);
    printf(")");
    break;
  case AC_NORMAL:
    printf("[%d](", t->sym);
    print_tlist(t->rest.f.arg_list);
    printf(")");
    break;
  case AC_COMPRESSED:
    printf("{%d}(", t->sym);
    print_aclist(t->rest.a.ac_list);
    printf(")");
    break;
  }
}

print_tlist(l)
TERM_LIST *l;
{
  while(l){
    print_term(l->arg);
    l = l->next_arg;
    if(l != NULL)
      printf(", ");
  }
}

print_aclist(l)
AC_LIST *l;
{
  while(l){
    printf("%d*", l->mult);
    print_term(l->arg);
    l = l->next_ac;
    if(l != NULL)
      printf(", ");
  }
}
