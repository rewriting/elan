#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "sdart.h"
#include "back.h"
#include "builtin.h"

/* Constantes d'execution */
int trace = 0; /* 1 : result, 2 : start with */
long rewrite_step=0;
int indentlevel=0;

static int isGreedyRule_211[2] = {0,0};
static int isGreedyRule_212[3] = {0,0,0};
static int isGreedyRule_213[3] = {0,0,0};

/* Table des symboles */
fsym fsymtab[219];
/* Declaration des pattern_list */
void init_pattern_list_211();
void delete_pattern_list_211();
void init_pattern_list_213();
void delete_pattern_list_213();
void init_pattern_list_213_211();
void delete_pattern_list_213_211();
void init_pattern_list_212();
void delete_pattern_list_212();
void init_pattern_list_212_211();
void delete_pattern_list_212_211();

/* Constantes */
struct term *con_209;
struct term *con_200;

/* Procedure principale */
long *bp_main;
main() {
  long bp;
  struct term *res;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  fsym_init(code_218,0,"finish",0);
  fsym_init(code_217,0,"all",0);
  fsym_init(code_216,0,"triples",0);
  fsym_init(code_215,0,"doubles",0);
  fsym_init(code_214,0,"singles",0);
  fsym_init(code_213,-1,"m2(,)",0);
  fsym_init(code_212,-1,"p2(,)",0);
  fsym_init(code_211,-1,"+",0);
  fsym_init(code_210,1,"set()",0);
  fsym_init(code_209,0,"empty",0);
  fsym_init(code_208,0,"fifty",0);
  fsym_init(code_207,0,"twentyfive",0);
  fsym_init(code_206,0,"fifteen",0);
  fsym_init(code_205,0,"ten",0);
  fsym_init(code_204,0,"five",0);
  fsym_init(code_203,2,"mult(,)",0);
  fsym_init(code_202,2,"plus(,)",0);
  fsym_init(code_201,1,"s()",0);
  fsym_init(code_200,0,"o",0);
  
/* Initialidsation des constantes */
  TERM_CONST_ALLOC(con_209, code_209);
  TERM_CONST_ALLOC(con_200, code_200);
  /* Initialisation des pattern_list */
  init_pattern_list_211();
  init_pattern_list_213();
  init_pattern_list_213_211();
  init_pattern_list_212();
  init_pattern_list_212_211();

  if (!setChoicePoint()) {
    /* TERME DE DEPART */
    {
      /* finish */
      struct term *tmp, *sv0;
      sv0 = fun_218(  );
      res=sv0;
    }
    printf("\nresult[%d] = ",rewrite_step);
    term_printnl(stdout,res);
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_218].name);
  FREE(fsymtab[code_217].name);
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_213].name);
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_200].name);
  TERM_FREE(con_209);
  TERM_FREE(con_200);
  /* Destruction des pattern_list */
  delete_pattern_list_211();
  delete_pattern_list_213();
  delete_pattern_list_213_211();
  delete_pattern_list_212();
  delete_pattern_list_212_211();
#ifdef DEBUG
  print_space_usage();
#endif
  printf("\nrewrite_step = %d\n",rewrite_step);
}
int match_subterm_211(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_211(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_211;
static int no_pattern_211_niv_0;
static int nb_pattern_211_niv_0 = 2;
static int nb_pattern_211_niv_1 = 1;
#define max_nb_pattern_under_211 1

struct term* fun_211(struct term *v0 ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet_create(mask,2);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_211(");
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  bitSet_set(mask,1);
  /* End syntactical matching */
  if( bitSet_get(mask,0) || bitSet_get(mask,1) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    mode=POSSIBLE_REST;
    ms=MS_create();
    MS_init(ms, match_subterm_211, no_pattern_211_niv_0, pattern_list_211, nb_pattern_211_niv_1, v0, necessary_link, max_nb_pattern_under_211);
    /* End AC matching */
  }
  if(ACPattern && ms!=NULL) {
    indice = MS_solve(ms,mode,isGreedyRule_211);
  } else {
    indice = 0;
  }
  if(bitSet_get(mask,0)) {
    if(indice>=0 && ms->no_rule==0) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] +[var0.empty] => var0 */
      struct term *tmp, *sv0;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_211);
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_211(sv0);
      else
        sv0=tmp;
      res = sv0 ;
      goto end;
    }
    myend0:;
  }
  if(bitSet_get(mask,1)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] +[var0[2].var1] => +[var0.var1] */
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
          goto myend1;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_211(sv0);
      else
        sv0=tmp;
      tmp = term_removeTopSymbol(sv1);
      if(tmp == NULL)
        sv1=fun_211(sv1);
      else
        sv1=tmp;
      TERM_ALLOC(sv2,term2,code_211);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_211( sv2 );
      res = sv2 ;
      goto end;
    }
    myend1:;
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

int match_subterm_211(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
  struct term *v1;
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(v0->symb) {
    case code_209:
      bitSet_set(mask,0);
    break;
  }
  return nb_bit;
}

void variable_extract_211(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* empty */
  case 0:
    break;
  default:
    fprintf(stderr,"variable_extract_211: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_211() {
  int pattern_tab[max_nb_pattern_under_211];
  no_pattern_211_niv_0=0;
  pattern_list_211=MS_pattern_list_create(nb_pattern_211_niv_0);
  /* +[var0.empty] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_211,no_pattern_211_niv_0++,1,pattern_tab);
  /* +[var0[2].var1] */
}

void delete_pattern_list_211() {
  MS_pattern_list_free(pattern_list_211,no_pattern_211_niv_0);
}
int match_subterm_213(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_213(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_213;
static int no_pattern_213_niv_0;
static int nb_pattern_213_niv_0 = 3;
static int nb_pattern_213_niv_1 = 3;
#define max_nb_pattern_under_213 2
int match_subterm_213_211(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_213_211(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_213_211;
static int no_pattern_213_211_niv_0;
static int nb_pattern_213_211_niv_0 = 1;
static int nb_pattern_213_211_niv_1 = 1;
#define max_nb_pattern_under_213_211 1

struct term* fun_213(struct term *v0 ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet_create(mask,3);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_213(");
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  bitSet_set(mask,1);
  bitSet_set(mask,2);
  /* End syntactical matching */
  if( bitSet_get(mask,0) || bitSet_get(mask,1) || bitSet_get(mask,2) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=1;
    mode=POSSIBLE_REST;
    ms=MS_create();
    MS_init(ms, match_subterm_213, no_pattern_213_niv_0, pattern_list_213, nb_pattern_213_niv_1, v0, necessary_link, max_nb_pattern_under_213);
    /* End AC matching */
  }
  if(ACPattern && ms!=NULL) {
    indice = MS_solve(ms,mode,isGreedyRule_213);
  } else {
    indice = 0;
  }
  if(bitSet_get(mask,0)) {
    if(indice>=0 && ms->no_rule==0) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] m2(,)[var0.empty] => var0 */
      struct term *tmp, *sv0;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_213);
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_213(sv0);
      else
        sv0=tmp;
      res = sv0 ;
      goto end;
    }
    myend2:;
  }
  if(bitSet_get(mask,1)) {
    if(indice>=0 && ms->no_rule==1) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      /* [] m2(,)[var2.set(var0).set(var1)] => m2(,)[var2.set(mult(var0,var1))] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_213);
      sv0=substitution[2];
      sv1=substitution[1];
      sv2 = fun_203( sv1,sv0 );
      TERM_ALLOC(sv3,term1,code_210);
      sv3->sub[0] = sv2;
      sv4=substitution[0];
      tmp = term_removeTopSymbol(sv4);
      if(tmp == NULL)
        sv4=fun_213(sv4);
      else
        sv4=tmp;
      TERM_ALLOC(sv5,term2,code_213);
      term_add_onf_term(sv5,sv4);
      term_add_onf_term(sv5,sv3);
      sv5 = fun_213( sv5 );
      res = sv5 ;
      goto end;
    }
    myend3:;
  }
  if(bitSet_get(mask,2)) {
    if(indice>=0 && ms->no_rule==2) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      /* [] m2(,)[var2.+[var1.set(var0)]] => +[m2(,)[var1.var2].m2(,)[var2.set(var0)]] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_213);
      sv0=substitution[2];
      TERM_ALLOC(sv1,term1,code_210);
      sv1->sub[0] = sv0;
      sv2=substitution[0];
      tmp = term_removeTopSymbol(sv2);
      if(tmp == NULL)
        sv2=fun_213(sv2);
      else
        sv2=tmp;
      TERM_ALLOC(sv3,term2,code_213);
      term_add_onf_term(sv3,sv2);
      term_add_onf_term(sv3,sv1);
      sv3 = fun_213( sv3 );
      sv4=substitution[1];
      tmp = term_removeTopSymbol(sv4);
      if(tmp != NULL)
        sv4=tmp;
      TERM_ALLOC(sv5,term2,code_213);
      term_add_onf_term(sv5,sv4);
      term_add_onf_term(sv5,sv2);
      sv5 = fun_213( sv5 );
      TERM_ALLOC(sv6,term2,code_211);
      term_add_onf_term(sv6,sv5);
      term_add_onf_term(sv6,sv3);
      sv6 = fun_211( sv6 );
      res = sv6 ;
      goto end;
    }
    myend4:;
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

int match_subterm_213(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
  struct term *v1;
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(v0->symb) {
    case code_211:
      /* AC case: Not tested */
      nb_bit = match_subterm_AC(2, no_arg_subject, mask, cbg, match_subterm_213_211, no_pattern_213_211_niv_0, pattern_list_213_211, nb_pattern_213_211_niv_1, v0, max_nb_pattern_under_213_211);
    break;
    case code_210:
      v1=v0->sub[0];
      switch(v1->symb) {
        default:
          bitSet_set(mask,1);
      }
    break;
    case code_209:
      bitSet_set(mask,0);
    break;
  }
  return nb_bit;
}

int match_subterm_213_211(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
  struct term *v1;
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(v0->symb) {
    case code_210:
      switch(v0->symb) {
        default:
          bitSet_set(mask,0);
      }
    break;
  }
  return nb_bit;
}

void variable_extract_213(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* empty */
  case 0:
    break;
    /* set(var0) */
  case 1:
    extract_substitution[*indice]=v0->sub[0];
    (*indice)++;
    break;
    /* +[var1.set(var0)] */
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
        substitution_build(v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_213_211);
        for(i=0 ; i <nb_variable ; i++) {
          extract_substitution[*indice]=substitution[i];
          (*indice)++;
        }
      }
    }
    break;
  default:
    fprintf(stderr,"variable_extract_213: bad pattern number\n");
    exit(0);
  }
}

void variable_extract_213_211(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* set(var0) */
  case 0:
    extract_substitution[*indice]=v0->sub[0];
    (*indice)++;
    break;
  default:
    fprintf(stderr,"variable_extract_213_211: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_213() {
  int pattern_tab[max_nb_pattern_under_213];
  no_pattern_213_niv_0=0;
  pattern_list_213=MS_pattern_list_create(nb_pattern_213_niv_0);
  /* m2(,)[var0.empty] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_213,no_pattern_213_niv_0++,1,pattern_tab);
  /* m2(,)[var2.set(var0).set(var1)] */
  pattern_tab[0]=1;
  pattern_tab[1]=1;
  MS_pattern_list_init(pattern_list_213,no_pattern_213_niv_0++,2,pattern_tab);
  /* m2(,)[var2.+[var1.set(var0)]] */
  pattern_tab[0]=2;
  MS_pattern_list_init(pattern_list_213,no_pattern_213_niv_0++,1,pattern_tab);
}

void delete_pattern_list_213() {
  MS_pattern_list_free(pattern_list_213,no_pattern_213_niv_0);
}

void init_pattern_list_213_211() {
  int pattern_tab[max_nb_pattern_under_213_211];
  no_pattern_213_211_niv_0=0;
  pattern_list_213_211=MS_pattern_list_create(nb_pattern_213_211_niv_0);
  /* +[var1.set(var0)] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_213_211,no_pattern_213_211_niv_0++,1,pattern_tab);
}

void delete_pattern_list_213_211() {
  MS_pattern_list_free(pattern_list_213_211,no_pattern_213_211_niv_0);
}

struct term* fun_217( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_217(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: all */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9;
    /* rhs: +[set(o).set(twentyfive).set(fifty).singles.doubles.triples] */
    sv0 = fun_216(  );
    sv1 = fun_215(  );
    sv2 = fun_214(  );
    sv3 = fun_208(  );
    TERM_ALLOC(sv4,term1,code_210);
    sv4->sub[0] = sv3;
    sv5 = fun_207(  );
    TERM_ALLOC(sv6,term1,code_210);
    sv6->sub[0] = sv5;
    TERM_ALLOC(sv8,term1,code_210);
    sv8->sub[0] = con_200;
    TERM_ALLOC(sv9,term2,code_211);
    term_add_onf_term(sv9,sv8);
    term_add_onf_term(sv9,sv6);
    term_add_onf_term(sv9,sv4);
    term_add_onf_term(sv9,sv2);
    term_add_onf_term(sv9,sv1);
    term_add_onf_term(sv9,sv0);
    sv9 = fun_211( sv9 );
    res = sv9 ;
    goto end;
    myend5:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_217\n");
  exit(0);
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

struct term* fun_202(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,2);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_202(");
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(v1->symb) {
    default:
      switch(v2->symb) {
        case code_200:
          bitSet_set(mask,1);
        break;
        case code_201:
          v3=v2->sub[0];
          switch(v3->symb) {
            default:
              bitSet_set(mask,0);
          }
        break;
      }
  }
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: plus(var0,s(var1)) */
    struct term *tmp, *sv0, *sv1, *sv3;
    /* rhs: s(plus(var0,var1)) */
    sv0 = fun_202( v1,v3 );
    TERM_ALLOC(sv1,term1,code_201);
    sv1->sub[0] = sv0;
    res = sv1 ;
    goto end;
    myend6:;
  }
  if(bitSet_get(mask,1)) {
    /* lhs: plus(var0,o) */
    struct term *tmp, *sv1;
    /* rhs: var0 */
    res = v1 ;
    goto end;
    myend7:;
  }
match_fail:
  TERM_ALLOC(res,term2, 202);
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

struct term* fun_204( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_204(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: five */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
    /* rhs: s(s(s(s(s(o))))) */
    TERM_ALLOC(sv1,term1,code_201);
    sv1->sub[0] = con_200;
    TERM_ALLOC(sv2,term1,code_201);
    sv2->sub[0] = sv1;
    TERM_ALLOC(sv3,term1,code_201);
    sv3->sub[0] = sv2;
    TERM_ALLOC(sv4,term1,code_201);
    sv4->sub[0] = sv3;
    TERM_ALLOC(sv5,term1,code_201);
    sv5->sub[0] = sv4;
    res = sv5 ;
    goto end;
    myend8:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_204\n");
  exit(0);
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

struct term* fun_208( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_208(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: fifty */
    struct term *tmp, *sv0, *sv1, *sv2;
    /* rhs: plus(twentyfive,twentyfive) */
    sv0 = fun_207(  );
    sv1 = fun_207(  );
    sv2 = fun_202( sv1,sv0 );
    res = sv2 ;
    goto end;
    myend9:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_208\n");
  exit(0);
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

struct term* fun_216( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_216(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: triples */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6;
    /* rhs: m2(,)[set(s(s(s(o)))).singles] */
    sv0 = fun_214(  );
    TERM_ALLOC(sv2,term1,code_201);
    sv2->sub[0] = con_200;
    TERM_ALLOC(sv3,term1,code_201);
    sv3->sub[0] = sv2;
    TERM_ALLOC(sv4,term1,code_201);
    sv4->sub[0] = sv3;
    TERM_ALLOC(sv5,term1,code_210);
    sv5->sub[0] = sv4;
    TERM_ALLOC(sv6,term2,code_213);
    term_add_onf_term(sv6,sv5);
    term_add_onf_term(sv6,sv0);
    sv6 = fun_213( sv6 );
    res = sv6 ;
    goto end;
    myend10:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_216\n");
  exit(0);
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

struct term* fun_203(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,2);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_203(");
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(v1->symb) {
    default:
      switch(v2->symb) {
        case code_201:
          v3=v2->sub[0];
          switch(v3->symb) {
            default:
              bitSet_set(mask,1);
          }
        break;
        case code_200:
          bitSet_set(mask,0);
        break;
      }
  }
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: mult(var0,o) */
    struct term *tmp, *sv0;
    /* rhs: o */
    res = con_200 ;
    goto end;
    myend11:;
  }
  if(bitSet_get(mask,1)) {
    /* lhs: mult(var0,s(var1)) */
    struct term *tmp, *sv0, *sv1, *sv3;
    /* rhs: plus(mult(var0,var1),var0) */
    sv0 = fun_203( v1,v3 );
    sv1 = fun_202( sv0,v1 );
    res = sv1 ;
    goto end;
    myend12:;
  }
match_fail:
  TERM_ALLOC(res,term2, 203);
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

struct term* fun_207( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_207(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: twentyfive */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10;
    /* rhs: s(s(s(s(s(s(s(s(s(s(fifteen)))))))))) */
    sv0 = fun_206(  );
    TERM_ALLOC(sv1,term1,code_201);
    sv1->sub[0] = sv0;
    TERM_ALLOC(sv2,term1,code_201);
    sv2->sub[0] = sv1;
    TERM_ALLOC(sv3,term1,code_201);
    sv3->sub[0] = sv2;
    TERM_ALLOC(sv4,term1,code_201);
    sv4->sub[0] = sv3;
    TERM_ALLOC(sv5,term1,code_201);
    sv5->sub[0] = sv4;
    TERM_ALLOC(sv6,term1,code_201);
    sv6->sub[0] = sv5;
    TERM_ALLOC(sv7,term1,code_201);
    sv7->sub[0] = sv6;
    TERM_ALLOC(sv8,term1,code_201);
    sv8->sub[0] = sv7;
    TERM_ALLOC(sv9,term1,code_201);
    sv9->sub[0] = sv8;
    TERM_ALLOC(sv10,term1,code_201);
    sv10->sub[0] = sv9;
    res = sv10 ;
    goto end;
    myend13:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_207\n");
  exit(0);
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

struct term* fun_215( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_215(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: doubles */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
    /* rhs: m2(,)[set(s(s(o))).singles] */
    sv0 = fun_214(  );
    TERM_ALLOC(sv2,term1,code_201);
    sv2->sub[0] = con_200;
    TERM_ALLOC(sv3,term1,code_201);
    sv3->sub[0] = sv2;
    TERM_ALLOC(sv4,term1,code_210);
    sv4->sub[0] = sv3;
    TERM_ALLOC(sv5,term2,code_213);
    term_add_onf_term(sv5,sv4);
    term_add_onf_term(sv5,sv0);
    sv5 = fun_213( sv5 );
    res = sv5 ;
    goto end;
    myend14:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_215\n");
  exit(0);
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

struct term* fun_206( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_206(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: fifteen */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
    /* rhs: s(s(s(s(s(ten))))) */
    sv0 = fun_205(  );
    TERM_ALLOC(sv1,term1,code_201);
    sv1->sub[0] = sv0;
    TERM_ALLOC(sv2,term1,code_201);
    sv2->sub[0] = sv1;
    TERM_ALLOC(sv3,term1,code_201);
    sv3->sub[0] = sv2;
    TERM_ALLOC(sv4,term1,code_201);
    sv4->sub[0] = sv3;
    TERM_ALLOC(sv5,term1,code_201);
    sv5->sub[0] = sv4;
    res = sv5 ;
    goto end;
    myend15:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_206\n");
  exit(0);
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
int match_subterm_212(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_212(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_212;
static int no_pattern_212_niv_0;
static int nb_pattern_212_niv_0 = 3;
static int nb_pattern_212_niv_1 = 3;
#define max_nb_pattern_under_212 2
int match_subterm_212_211(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_212_211(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static **pattern_list_212_211;
static int no_pattern_212_211_niv_0;
static int nb_pattern_212_211_niv_0 = 1;
static int nb_pattern_212_211_niv_1 = 1;
#define max_nb_pattern_under_212_211 1

struct term* fun_212(struct term *v0 ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet_create(mask,3);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_212(");
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && cell_mult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  bitSet_set(mask,1);
  bitSet_set(mask,2);
  /* End syntactical matching */
  if( bitSet_get(mask,0) || bitSet_get(mask,1) || bitSet_get(mask,2) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=1;
    mode=POSSIBLE_REST;
    ms=MS_create();
    MS_init(ms, match_subterm_212, no_pattern_212_niv_0, pattern_list_212, nb_pattern_212_niv_1, v0, necessary_link, max_nb_pattern_under_212);
    /* End AC matching */
  }
  if(ACPattern && ms!=NULL) {
    indice = MS_solve(ms,mode,isGreedyRule_212);
  } else {
    indice = 0;
  }
  if(bitSet_get(mask,0)) {
    if(indice>=0 && ms->no_rule==0) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] p2(,)[var0.empty] => var0 */
      struct term *tmp, *sv0;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_212);
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_212(sv0);
      else
        sv0=tmp;
      res = sv0 ;
      goto end;
    }
    myend16:;
  }
  if(bitSet_get(mask,1)) {
    if(indice>=0 && ms->no_rule==1) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      /* [] p2(,)[var2.set(var0).set(var1)] => p2(,)[var2.set(plus(var0,var1))] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_212);
      sv0=substitution[2];
      sv1=substitution[1];
      sv2 = fun_202( sv1,sv0 );
      TERM_ALLOC(sv3,term1,code_210);
      sv3->sub[0] = sv2;
      sv4=substitution[0];
      tmp = term_removeTopSymbol(sv4);
      if(tmp == NULL)
        sv4=fun_212(sv4);
      else
        sv4=tmp;
      TERM_ALLOC(sv5,term2,code_212);
      term_add_onf_term(sv5,sv4);
      term_add_onf_term(sv5,sv3);
      sv5 = fun_212( sv5 );
      res = sv5 ;
      goto end;
    }
    myend17:;
  }
  if(bitSet_get(mask,2)) {
    if(indice>=0 && ms->no_rule==2) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      /* [] p2(,)[var2.+[var1.set(var0)]] => +[p2(,)[var1.var2].p2(,)[var2.set(var0)]] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_212);
      sv0=substitution[2];
      TERM_ALLOC(sv1,term1,code_210);
      sv1->sub[0] = sv0;
      sv2=substitution[0];
      tmp = term_removeTopSymbol(sv2);
      if(tmp == NULL)
        sv2=fun_212(sv2);
      else
        sv2=tmp;
      TERM_ALLOC(sv3,term2,code_212);
      term_add_onf_term(sv3,sv2);
      term_add_onf_term(sv3,sv1);
      sv3 = fun_212( sv3 );
      sv4=substitution[1];
      tmp = term_removeTopSymbol(sv4);
      if(tmp != NULL)
        sv4=tmp;
      TERM_ALLOC(sv5,term2,code_212);
      term_add_onf_term(sv5,sv4);
      term_add_onf_term(sv5,sv2);
      sv5 = fun_212( sv5 );
      TERM_ALLOC(sv6,term2,code_211);
      term_add_onf_term(sv6,sv5);
      term_add_onf_term(sv6,sv3);
      sv6 = fun_211( sv6 );
      res = sv6 ;
      goto end;
    }
    myend18:;
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

int match_subterm_212(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
  struct term *v1;
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(v0->symb) {
    case code_211:
      /* AC case: Not tested */
      nb_bit = match_subterm_AC(2, no_arg_subject, mask, cbg, match_subterm_212_211, no_pattern_212_211_niv_0, pattern_list_212_211, nb_pattern_212_211_niv_1, v0, max_nb_pattern_under_212_211);
    break;
    case code_210:
      v1=v0->sub[0];
      switch(v1->symb) {
        default:
          bitSet_set(mask,1);
      }
    break;
    case code_209:
      bitSet_set(mask,0);
    break;
  }
  return nb_bit;
}

int match_subterm_212_211(struct term *v0,int no_arg_subject,bitSet *mask,BG *cbg) {
  struct term *v1;
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(v0->symb) {
    case code_210:
      switch(v0->symb) {
        default:
          bitSet_set(mask,0);
      }
    break;
  }
  return nb_bit;
}

void variable_extract_212(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* empty */
  case 0:
    break;
    /* set(var0) */
  case 1:
    extract_substitution[*indice]=v0->sub[0];
    (*indice)++;
    break;
    /* +[var1.set(var0)] */
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
        substitution_build(v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_212_211);
        for(i=0 ; i <nb_variable ; i++) {
          extract_substitution[*indice]=substitution[i];
          (*indice)++;
        }
      }
    }
    break;
  default:
    fprintf(stderr,"variable_extract_212: bad pattern number\n");
    exit(0);
  }
}

void variable_extract_212_211(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* set(var0) */
  case 0:
    extract_substitution[*indice]=v0->sub[0];
    (*indice)++;
    break;
  default:
    fprintf(stderr,"variable_extract_212_211: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_212() {
  int pattern_tab[max_nb_pattern_under_212];
  no_pattern_212_niv_0=0;
  pattern_list_212=MS_pattern_list_create(nb_pattern_212_niv_0);
  /* p2(,)[var0.empty] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_212,no_pattern_212_niv_0++,1,pattern_tab);
  /* p2(,)[var2.set(var0).set(var1)] */
  pattern_tab[0]=1;
  pattern_tab[1]=1;
  MS_pattern_list_init(pattern_list_212,no_pattern_212_niv_0++,2,pattern_tab);
  /* p2(,)[var2.+[var1.set(var0)]] */
  pattern_tab[0]=2;
  MS_pattern_list_init(pattern_list_212,no_pattern_212_niv_0++,1,pattern_tab);
}

void delete_pattern_list_212() {
  MS_pattern_list_free(pattern_list_212,no_pattern_212_niv_0);
}

void init_pattern_list_212_211() {
  int pattern_tab[max_nb_pattern_under_212_211];
  no_pattern_212_211_niv_0=0;
  pattern_list_212_211=MS_pattern_list_create(nb_pattern_212_211_niv_0);
  /* +[var1.set(var0)] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_212_211,no_pattern_212_211_niv_0++,1,pattern_tab);
}

void delete_pattern_list_212_211() {
  MS_pattern_list_free(pattern_list_212_211,no_pattern_212_211_niv_0);
}

struct term* fun_214( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_214(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: singles */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10, *sv11, *sv12, *sv13, *sv14, *sv15, *sv16, *sv17, *sv18, *sv19, *sv20, *sv21, *sv22, *sv23, *sv24, *sv25, *sv26, *sv27, *sv28, *sv29, *sv30, *sv31, *sv32, *sv33, *sv34, *sv35, *sv36, *sv37, *sv38, *sv39, *sv40;
    /* rhs: +[set(s(o)).set(s(s(o))).set(s(s(s(o)))).set(s(s(s(s(o))))).set(s(s(s(s(five))))).set(s(s(s(five)))).set(s(s(five))).set(s(five)).set(five).set(ten)] */
    sv0 = fun_205(  );
    TERM_ALLOC(sv1,term1,code_210);
    sv1->sub[0] = sv0;
    sv2 = fun_204(  );
    TERM_ALLOC(sv3,term1,code_210);
    sv3->sub[0] = sv2;
    sv4 = fun_204(  );
    TERM_ALLOC(sv5,term1,code_201);
    sv5->sub[0] = sv4;
    TERM_ALLOC(sv6,term1,code_210);
    sv6->sub[0] = sv5;
    sv7 = fun_204(  );
    TERM_ALLOC(sv8,term1,code_201);
    sv8->sub[0] = sv7;
    TERM_ALLOC(sv9,term1,code_201);
    sv9->sub[0] = sv8;
    TERM_ALLOC(sv10,term1,code_210);
    sv10->sub[0] = sv9;
    sv11 = fun_204(  );
    TERM_ALLOC(sv12,term1,code_201);
    sv12->sub[0] = sv11;
    TERM_ALLOC(sv13,term1,code_201);
    sv13->sub[0] = sv12;
    TERM_ALLOC(sv14,term1,code_201);
    sv14->sub[0] = sv13;
    TERM_ALLOC(sv15,term1,code_210);
    sv15->sub[0] = sv14;
    sv16 = fun_204(  );
    TERM_ALLOC(sv17,term1,code_201);
    sv17->sub[0] = sv16;
    TERM_ALLOC(sv18,term1,code_201);
    sv18->sub[0] = sv17;
    TERM_ALLOC(sv19,term1,code_201);
    sv19->sub[0] = sv18;
    TERM_ALLOC(sv20,term1,code_201);
    sv20->sub[0] = sv19;
    TERM_ALLOC(sv21,term1,code_210);
    sv21->sub[0] = sv20;
    TERM_ALLOC(sv23,term1,code_201);
    sv23->sub[0] = con_200;
    TERM_ALLOC(sv24,term1,code_201);
    sv24->sub[0] = sv23;
    TERM_ALLOC(sv25,term1,code_201);
    sv25->sub[0] = sv24;
    TERM_ALLOC(sv26,term1,code_201);
    sv26->sub[0] = sv25;
    TERM_ALLOC(sv27,term1,code_210);
    sv27->sub[0] = sv26;
    TERM_ALLOC(sv29,term1,code_201);
    sv29->sub[0] = con_200;
    TERM_ALLOC(sv30,term1,code_201);
    sv30->sub[0] = sv29;
    TERM_ALLOC(sv31,term1,code_201);
    sv31->sub[0] = sv30;
    TERM_ALLOC(sv32,term1,code_210);
    sv32->sub[0] = sv31;
    TERM_ALLOC(sv34,term1,code_201);
    sv34->sub[0] = con_200;
    TERM_ALLOC(sv35,term1,code_201);
    sv35->sub[0] = sv34;
    TERM_ALLOC(sv36,term1,code_210);
    sv36->sub[0] = sv35;
    TERM_ALLOC(sv38,term1,code_201);
    sv38->sub[0] = con_200;
    TERM_ALLOC(sv39,term1,code_210);
    sv39->sub[0] = sv38;
    TERM_ALLOC(sv40,term2,code_211);
    term_add_onf_term(sv40,sv39);
    term_add_onf_term(sv40,sv36);
    term_add_onf_term(sv40,sv32);
    term_add_onf_term(sv40,sv27);
    term_add_onf_term(sv40,sv21);
    term_add_onf_term(sv40,sv15);
    term_add_onf_term(sv40,sv10);
    term_add_onf_term(sv40,sv6);
    term_add_onf_term(sv40,sv3);
    term_add_onf_term(sv40,sv1);
    sv40 = fun_211( sv40 );
    res = sv40 ;
    goto end;
    myend19:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_214\n");
  exit(0);
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

struct term* fun_218( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_218(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: finish */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
    /* rhs: p2(,)[+[set(fifty).doubles].all[2]] */
    sv0 = fun_217(  );
    sv1 = fun_215(  );
    sv2 = fun_208(  );
    TERM_ALLOC(sv3,term1,code_210);
    sv3->sub[0] = sv2;
    TERM_ALLOC(sv4,term2,code_211);
    term_add_onf_term(sv4,sv3);
    term_add_onf_term(sv4,sv1);
    sv4 = fun_211( sv4 );
    TERM_ALLOC(sv5,term2,code_212);
    term_add_onf_term(sv5,sv4);
    term_add_onf_term(sv5,sv0);
    term_add_onf_term(sv5,sv0);
    sv5 = fun_212( sv5 );
    res = sv5 ;
    goto end;
    myend20:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_218\n");
  exit(0);
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

struct term* fun_205( ) {
  struct term *v1;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_205(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: ten */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
    /* rhs: s(s(s(s(s(five))))) */
    sv0 = fun_204(  );
    TERM_ALLOC(sv1,term1,code_201);
    sv1->sub[0] = sv0;
    TERM_ALLOC(sv2,term1,code_201);
    sv2->sub[0] = sv1;
    TERM_ALLOC(sv3,term1,code_201);
    sv3->sub[0] = sv2;
    TERM_ALLOC(sv4,term1,code_201);
    sv4->sub[0] = sv3;
    TERM_ALLOC(sv5,term1,code_201);
    sv5->sub[0] = sv4;
    res = sv5 ;
    goto end;
    myend21:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_205\n");
  exit(0);
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
