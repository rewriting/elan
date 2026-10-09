#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "prop.h"
#include "back.h"
#include "builtin.h"
#define bitSet_create2(bi,size) bitSet_stack_create(bi,size) 


/* Constantes d'execution */
int trace = 1;
long rewrite_step=0;
int indentlevel=0;

/* Table des symboles */
fsym fsymtab[256];
/* Declaration des pattern_list */
void init_pattern_list_40();
void delete_pattern_list_40();
void init_pattern_list_41();
void delete_pattern_list_41();

/* Constantes */
struct term *con_19;
struct term *con_18;
struct term *con_17;
struct term *con_16;
struct term *con_15;
struct term *con_14;
struct term *con_13;
struct term *con_12;
struct term *con_11;
struct term *con_2;
struct term *con_1;

/* Procedure principale */
long *bp_main;
main() {
  long bp;
  struct term *res;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  fsym_init(code_43,1,"not");
  fsym_init(code_42,2,"implies");
  fsym_init(code_41,-1,"xor");
  fsym_init(code_40,-1,"and");
  fsym_init(code_19,0,"a9");
  fsym_init(code_18,0,"a8");
  fsym_init(code_17,0,"a7");
  fsym_init(code_16,0,"a6");
  fsym_init(code_15,0,"a5");
  fsym_init(code_14,0,"a4");
  fsym_init(code_13,0,"a3");
  fsym_init(code_12,0,"a2");
  fsym_init(code_11,0,"a1");
  fsym_init(code_2,0,"false");
  fsym_init(code_1,0,"true");
  
/* Initialidsation des constantes */
  TERM_CONST_ALLOC(con_19, code_19);
  TERM_CONST_ALLOC(con_18, code_18);
  TERM_CONST_ALLOC(con_17, code_17);
  TERM_CONST_ALLOC(con_16, code_16);
  TERM_CONST_ALLOC(con_15, code_15);
  TERM_CONST_ALLOC(con_14, code_14);
  TERM_CONST_ALLOC(con_13, code_13);
  TERM_CONST_ALLOC(con_12, code_12);
  TERM_CONST_ALLOC(con_11, code_11);
  TERM_CONST_ALLOC(con_2, code_2);
  TERM_CONST_ALLOC(con_1, code_1);
  /* Initialisation des pattern_list */
  init_pattern_list_40();
  init_pattern_list_41();

  /* TERME DE DEPART */
  {
    /* implies(and(a1,implies(a1,a2),implies(a2,a3),implies(a3,a4),implies(a4,a5),implies(a5,a6),implies(a6,a7),implies(a7,a8),implies(a8,a9)),a9) */
  struct term *tmp, *sv0, *sv1, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10, *sv11, *sv12;
    sv5 = fun_42( con_18,con_19 );
    sv6 = fun_42( con_17,con_18 );
    sv7 = fun_42( con_16,con_17 );
    sv8 = fun_42( con_15,con_16 );
    sv9 = fun_42( con_14,con_15 );
    sv10 = fun_42( con_13,con_14 );
    sv11 = fun_42( con_12,con_13 );
    sv12 = fun_42( con_11,con_12 );
    TERM_ALLOC(sv4,term2,NULL,code_40);
    term_add_onf_term(sv4,con_11);
    term_add_onf_term(sv4,sv12);
    term_add_onf_term(sv4,sv11);
    term_add_onf_term(sv4,sv10);
    term_add_onf_term(sv4,sv9);
    term_add_onf_term(sv4,sv8);
    term_add_onf_term(sv4,sv7);
    term_add_onf_term(sv4,sv6);
    term_add_onf_term(sv4,sv5);
    sv4 = fun_40( sv4 );
    sv1 = fun_42( sv4,con_19 );
    res=sv1;
  }
end:
  printf("\nresult[%d] = ",rewrite_step);
  term_printnl(stdout,res);
destruction:
  FREE(fsymtab[code_43].name);
  FREE(fsymtab[code_42].name);
  FREE(fsymtab[code_41].name);
  FREE(fsymtab[code_40].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_17].name);
  FREE(fsymtab[code_16].name);
  FREE(fsymtab[code_15].name);
  FREE(fsymtab[code_14].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_2].name);
  FREE(fsymtab[code_1].name);
  TERM_FREE(con_19);
  TERM_FREE(con_18);
  TERM_FREE(con_17);
  TERM_FREE(con_16);
  TERM_FREE(con_15);
  TERM_FREE(con_14);
  TERM_FREE(con_13);
  TERM_FREE(con_12);
  TERM_FREE(con_11);
  TERM_FREE(con_2);
  TERM_FREE(con_1);
  /* Destruction des pattern_list */
  delete_pattern_list_40();
  delete_pattern_list_41();
#ifdef DEBUG
  print_space_usage();
#endif
  printf("\nrewrite_step = %d\n",rewrite_step);
}
int match_subterm_40(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_40(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_40;
static int no_pattern_40_niv_0;
static int nb_pattern_40_niv_0 = 4;
static int nb_pattern_40_niv_1 = 3;
#define max_nb_pattern_under_40 1

struct term* fun_40(struct term *v0 ) {
  bitSet *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet_create2(mask,4);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    term_printnl(stdout,v0);
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  bitSet_set(mask,1);
  bitSet_set(mask,2);
  bitSet_set(mask,3);
  /*  End syntactical matching */
  if( bitSet_get(mask,0) || bitSet_get(mask,1) || bitSet_get(mask,2) || bitSet_get(mask,3) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    mode=POSSIBLE_REST;
    ms=MS_create();
    MS_init(ms, match_subterm_40, no_pattern_40_niv_0, pattern_list_40, nb_pattern_40_niv_1, v0, necessary_link, max_nb_pattern_under_40);
    /* End AC matching */
  }
  if(ACPattern && ms!=NULL) {
    indice = MS_solve(ms,mode);
  } else {
    indice = 0;
  }
  if(bitSet_get(mask,0)) {
    if(indice>=0 && ms->no_rule==0) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] and(x,true) => x
 */
      struct term *tmp, *sv0;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_40);
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_40(sv0);
      else
        sv0=tmp;
      res = sv0 ;
      goto end;
    }
  myend0:
  }
  if(bitSet_get(mask,1)) {
    if(indice>=0 && ms->no_rule==1) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] and(x,false) => false
 */
      struct term *tmp, *sv0, *sv1;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_40);
      res = con_2 ;
      goto end;
    }
  myend1:
  }
  if(bitSet_get(mask,2)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] and(x[2],y) => and(x,y)
 */
      struct term *tmp, *sv0, *sv1, *sv2;
      for(i=0 ; i<nb_variable ; i++) {
        substitution[i]=v0;
      }
      sv1=substitution[0];
      sv0=substitution[1];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv1), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv1), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        indice=next_pe_extract(nb_arg_subject-1,E,sol,2);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv1,E,sol,2,&sv1,&sv0);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend2;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_40(sv0);
      else
        sv0=tmp;
      tmp = term_removeTopSymbol(sv1);
      if(tmp == NULL)
        sv1=fun_40(sv1);
      else
        sv1=tmp;
      TERM_ALLOC(sv2,term2,NULL,code_40);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_40( sv2 );
      res = sv2 ;
      goto end;
    }
  myend2:
  }
  if(bitSet_get(mask,3)) {
    if(indice>=0 && ms->no_rule==2) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      /* [] and(x,xor(y,z)) => xor(and(x,y),and(x,z))
 */
      struct term *tmp, *sv0, *sv2, *sv3, *sv4, *sv5, *sv6;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_40);
      sv3=substitution[1];
      sv0=substitution[2];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv3), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv3), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        indice=next_pe_extract(nb_arg_subject-1,E,sol,1);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv3,E,sol,1,&sv3,&sv0);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend3;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp != NULL)
        sv0=tmp;
      sv4=substitution[0];
      tmp = term_removeTopSymbol(sv4);
      if(tmp == NULL)
        sv4=fun_40(sv4);
      else
        sv4=tmp;
      TERM_ALLOC(sv2,term2,NULL,code_40);
      term_add_onf_term(sv2,sv4);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_40( sv2 );
      tmp = term_removeTopSymbol(sv3);
      if(tmp != NULL)
        sv3=tmp;
      TERM_ALLOC(sv5,term2,NULL,code_40);
      term_add_onf_term(sv5,sv4);
      term_add_onf_term(sv5,sv3);
      sv5 = fun_40( sv5 );
      TERM_ALLOC(sv6,term2,NULL,code_41);
      term_add_onf_term(sv6,sv5);
      term_add_onf_term(sv6,sv2);
      sv6 = fun_41( sv6 );
      res = sv6 ;
      goto end;
    }
  myend3:
  }
match_fail:
  res=v0;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet_delete(mask);
  if(ACPattern && ms!=NULL)
    MS_delete(ms);
  subindent();
  return res;
}

int match_subterm_40(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(v0->symb) {
    case code_41:
      /* AC case: Not tested */
      bitSet_set(mask,2);
    break;
    case code_2:
      bitSet_set(mask,1);
    break;
    case code_1:
      bitSet_set(mask,0);
    break;
  }
  return nb_bit;
}

void variable_extract_40(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* true */
    case 0:
      break;
    /* false */
    case 1:
      break;
    /* xor(y,z) */
    case 2:
      {
        /* Not tested */
        int nb_variable=2;
        int nb_variable_ac=1;
        struct term *substitution[2];
        LINK *link;
        match_state *msbg;
        int i;
        link=BG_link_get(ms->cbg,id_pattern);
        if(link==NULL) {
          for(i=0 ; i<nb_variable ; i++) {
            extract_substitution[*indice]=v0;
            (*indice)++;
          }
        } else {
          msbg=LINK_get(link,no_arg_subject);
          substitution_build(v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_40);
          for(i=0 ; i <nb_variable ; i++) {
            extract_substitution[*indice]=substitution[i];
            (*indice)++;
          }
        }
      }
      break;
    default:
      fprintf(stderr,"variable_extract_40: bad pattern number\n");
      exit(0);
  }
}

void init_pattern_list_40() {
  int pattern_tab[max_nb_pattern_under_40];
  no_pattern_40_niv_0=0;
  pattern_list_40=MS_pattern_list_create(nb_pattern_40_niv_0);
  /* and(x,true) */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_40,no_pattern_40_niv_0++,1,pattern_tab);
  /* and(x,false) */
  pattern_tab[0]=1;
  MS_pattern_list_init(pattern_list_40,no_pattern_40_niv_0++,1,pattern_tab);
  /* and(x[2],y) */
  /* and(x,xor(y,z)) */
  pattern_tab[0]=2;
  MS_pattern_list_init(pattern_list_40,no_pattern_40_niv_0++,1,pattern_tab);
}

void delete_pattern_list_40() {
  MS_pattern_list_free(pattern_list_40,no_pattern_40_niv_0);
}

struct term* fun_42(struct term *v1,struct term *v2 ) {
  bitSet *mask;
  struct term *res;
  bitSet_create2(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_42(");
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(v1->symb) {
    default:
      switch(v2->symb) {
        default:
          bitSet_set(mask,0);
      }
  }
  /*  End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* [] implies(x,y) => not(xor(x,and(x,y)))
 */
    struct term *tmp, *sv0, *sv1, *sv2;
    TERM_ALLOC(sv0,term2,NULL,code_40);
    term_add_onf_term(sv0,v1);
    term_add_onf_term(sv0,v2);
    sv0 = fun_40( sv0 );
    TERM_ALLOC(sv1,term2,NULL,code_41);
    term_add_onf_term(sv1,v1);
    term_add_onf_term(sv1,sv0);
    sv1 = fun_41( sv1 );
    sv0 = fun_43( sv1 );
    res = sv0 ;
    goto end;
  myend0:
  }
match_fail:
  TERM_ALLOC(res,term2,NULL, 42);
  res->sub[0] = v1;
  res->sub[1] = v2;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet_delete(mask);
  subindent();
  return res;
}

struct term* fun_43(struct term *v1 ) {
  bitSet *mask;
  struct term *res;
  bitSet_create2(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_43(");
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(v1->symb) {
    default:
      bitSet_set(mask,0);
  }
  /*  End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* [] not(x) => xor(x,true)
 */
    struct term *tmp, *sv0, *sv1;
    TERM_ALLOC(sv1,term2,NULL,code_41);
    term_add_onf_term(sv1,v1);
    term_add_onf_term(sv1,con_1);
    sv1 = fun_41( sv1 );
    res = sv1 ;
    goto end;
  myend0:
  }
match_fail:
  TERM_ALLOC(res,term1,NULL, 43);
  res->sub[0] = v1;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet_delete(mask);
  subindent();
  return res;
}
int match_subterm_41(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_41(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_41;
static int no_pattern_41_niv_0;
static int nb_pattern_41_niv_0 = 2;
static int nb_pattern_41_niv_1 = 1;
#define max_nb_pattern_under_41 1

struct term* fun_41(struct term *v0 ) {
  bitSet *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet_create2(mask,2);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    term_printnl(stdout,v0);
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  bitSet_set(mask,1);
  /*  End syntactical matching */
  if( bitSet_get(mask,0) || bitSet_get(mask,1) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    mode=POSSIBLE_REST;
    ms=MS_create();
    MS_init(ms, match_subterm_41, no_pattern_41_niv_0, pattern_list_41, nb_pattern_41_niv_1, v0, necessary_link, max_nb_pattern_under_41);
    /* End AC matching */
  }
  if(ACPattern && ms!=NULL) {
    indice = MS_solve(ms,mode);
  } else {
    indice = 0;
  }
  if(bitSet_get(mask,0)) {
    if(indice>=0 && ms->no_rule==0) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] xor(x,false) => x
 */
      struct term *tmp, *sv0;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_41);
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_41(sv0);
      else
        sv0=tmp;
      res = sv0 ;
      goto end;
    }
  myend0:
  }
  if(bitSet_get(mask,1)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] xor(x[2],y) => xor(y,false)
 */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3;
      for(i=0 ; i<nb_variable ; i++) {
        substitution[i]=v0;
      }
      sv0=substitution[0];
      sv2=substitution[1];
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv0), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        sol=(int*)IMALLOC(nb_arg_subject*sizeof(int));
        for(cell=term_first(sv0), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=cell_mult(cell);
          sol[no_arg_subject]=0;
        }
        indice=next_pe_extract(nb_arg_subject-1,E,sol,2);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv0,E,sol,2,&sv0,&sv2);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend1;
        }
      }
      tmp = term_removeTopSymbol(sv2);
      if(tmp == NULL)
        sv2=fun_41(sv2);
      else
        sv2=tmp;
      TERM_ALLOC(sv3,term2,NULL,code_41);
      term_add_onf_term(sv3,sv2);
      term_add_onf_term(sv3,con_2);
      sv3 = fun_41( sv3 );
      res = sv3 ;
      goto end;
    }
  myend1:
  }
match_fail:
  res=v0;
  goto end_no_rewrite;
end:
  rewrite_step++;
end_no_rewrite:
  if(trace>=1) {
    indent(indentlevel);
    printf("rewrite[%d] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet_delete(mask);
  if(ACPattern && ms!=NULL)
    MS_delete(ms);
  subindent();
  return res;
}

int match_subterm_41(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(v0->symb) {
    case code_2:
      bitSet_set(mask,0);
    break;
  }
  return nb_bit;
}

void variable_extract_41(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* false */
    case 0:
      break;
    default:
      fprintf(stderr,"variable_extract_41: bad pattern number\n");
      exit(0);
  }
}

void init_pattern_list_41() {
  int pattern_tab[max_nb_pattern_under_41];
  no_pattern_41_niv_0=0;
  pattern_list_41=MS_pattern_list_create(nb_pattern_41_niv_0);
  /* xor(x,false) */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_41,no_pattern_41_niv_0++,1,pattern_tab);
  /* xor(x[2],y) */
}

void delete_pattern_list_41() {
  MS_pattern_list_free(pattern_list_41,no_pattern_41_niv_0);
}
