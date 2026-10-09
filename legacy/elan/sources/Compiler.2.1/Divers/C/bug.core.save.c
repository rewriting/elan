#include "bug.h"
int match_subterm_210(struct term *v0,int no_arg_subject, int *mask, BG *cbg);
void variable_extract_210(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static int **pattern_list_210;
static int no_pattern_210_niv_0;
static int nb_pattern_210_niv_0 = 4;
static int nb_pattern_210_niv_1 = 3;
#define max_nb_pattern_under_210 1
int match_subterm_210_211(struct term *v0,int no_arg_subject, int *mask, BG *cbg);
void variable_extract_210_211(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static int **pattern_list_210_211;
static int no_pattern_210_211_niv_0;
static int nb_pattern_210_211_niv_0 = 2;
static int nb_pattern_210_211_niv_1 = 2;
#define max_nb_pattern_under_210_211 2

struct term* fun_210(struct term *v0 ) {
  struct term *v1,*v2;
  struct term *res;
  saveGlobalIndent()
  match_state *ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,4);
  bitSet32_init_clear(mask32);
  addindent();
  if(trace>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_210_%s_(",fsymtab[210].name);
    term_print(stdout,v0);
    printf(")\n");
  }
  if(term_first(v0)==term_last(v0) && getMult(term_first(v0))==1) {
    res=cell_t(term_first(v0));
    goto end_no_rewrite;
  }
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  bitSet32_set(mask32,1);
  bitSet32_set(mask32,2);
  bitSet32_set(mask32,3);
  /* End syntactical matching */
  if( bitSet32_get(mask32,0) || bitSet32_get(mask32,1) || bitSet32_get(mask32,2) || bitSet32_get(mask32,3) ) {
    int necessary_link;
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=1;
    indice = MS_init(&ms, match_subterm_210, no_pattern_210_niv_0, pattern_list_210, nb_pattern_210_niv_1,v0, necessary_link, max_nb_pattern_under_210);
    /* End AC matching */
  }
  if(bitSet32_get(mask32,0)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
    printf("try rule 0\n");
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend0;
    }
    if(ACPattern && MS_reinit(ms,v0,0)>0) {
      while(1) {
        if(MS_solve_rule(ms)<0) {
          goto myend0;
        }
        // printf("setChoicePoint\n");
        /* choicePoint AC matching */
        if(!setChoicePoint()) {
          break;
        }
        // printf("on revient d'un fail\n");
      }
    } else {
      goto myend0;
    }
    if(1) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      struct term *tmp, *sv[5];
      /* lhs: F(,)(var0,G(,)(f()(var1),f()(var2))) */
      /* To protect the term */
      setShared(v0);
      substitution_build_without_context(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_210,0);

      printf("var0 = "); term_printnl(stdout,substitution[0]);
      printf("var1 = "); term_printnl(stdout,substitution[1]);
      printf("var2 = "); term_printnl(stdout,substitution[2]);

      /* allDetEvaluation: multiDet */
      /* if eq_Foo(,)(var1,var2) */
      sv[1] = substitution[1];
      setShared(sv[1]);
      sv[2] = substitution[2];
      setShared(sv[2]);
      sv[3] = fun_18( sv[1],sv[2] );
      if( sv[3] != con_1 ) {
        fail();
      }
      /* rhs: rule0 */
      res = con_200 ;
      if(ACPattern)
        CUTCLOSE(); /* AC matching */
      goto end;
    } else {
      fail();
    }
    myend0:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
  if(bitSet32_get(mask32,1)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
     printf("try rule 1\n");
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend1;
    }
    if(ACPattern && MS_reinit(ms,v0,1)>0) {
      while(1) {
        if(MS_solve_rule(ms)<0) {
          goto myend1;
        }
        // printf("setChoicePoint\n");
        /* choicePoint AC matching */
        if(!setChoicePoint()) {
          break;
        }
        // printf("on revient d'un fail\n");
      }
    } else {
      goto myend1;
    }
    if(1) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      struct term *tmp, *sv[5];
      /* lhs: F(,)(var0,G(,)(g()(var1),g()(var2))) */
      /* To protect the term */
      setShared(v0);
      substitution_build_without_context(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_210,1);
      /* allDetEvaluation: multiDet */
      /* if eq_Foo(,)(var1,var2) */
      sv[1] = substitution[1];
      setShared(sv[1]);
      sv[2] = substitution[2];
      setShared(sv[2]);
      sv[3] = fun_18( sv[1],sv[2] );
      if( sv[3] != con_1 ) {
        fail();
      }
      /* rhs: rule1 */
      res = con_201 ;
      if(ACPattern)
        CUTCLOSE(); /* AC matching */
      goto end;
    } else {
      fail();
    }
    myend1:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
  if(bitSet32_get(mask32,2)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
     printf("try rule 2\n");
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend2;
    }
    if(1) {
      int nb_variable=2;
      int nb_variable_ac=2;
      struct term *substitution[2];
      int i;
      struct term *tmp, *sv[3];
      /* lhs: F(,)(var0,var1[2]) */
      for(i=0 ; i<nb_variable ; i++) {
        substitution[i]=v0;
      }
      sv[0]=substitution[0];
      setShared(sv[0]);
      sv[1]=substitution[1];
      setShared(sv[1]);
      {
        int *E,*sol;
        int nb_arg_subject;
        int no_arg_subject;
        int indice=0;
        struct cell_term *cell;
        struct cell_term *copy_cell;
        for(cell=term_first(sv[1]), nb_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), nb_arg_subject++);
        E=(int*)MALLOC(nb_arg_subject*sizeof(long));
        sol=(int*)MALLOC(nb_arg_subject*sizeof(long));
        for(cell=term_first(sv[1]), no_arg_subject=0 ; cell != NULL ; cell=cell_next(cell), no_arg_subject++) {
          E[no_arg_subject]=getMult(cell);
          sol[no_arg_subject]=0;
        }
        if(maximal_extract_fail(nb_arg_subject-1,E,sol,2)) {
          setShared(sv[1]);
          extract_xy_from_pe(sv[1],E,sol,2,&sv[1],&sv[0]);
          // printf("list_x = sv[1] = ");           term_printnl(stdout,sv[1]);
          // printf("list_y = sv[0] = ");           term_printnl(stdout,sv[0]);
        } else {
          /*  There is no more solution */
          fail();
        }
      }
      /* allDetEvaluation: det */
      /* rhs: rule2 */
      res = con_202 ;
      if(ACPattern)
        CUTCLOSE(); /* AC matching */
      goto end;
    } else {
      fail();
    }
    myend2:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
  if(bitSet32_get(mask32,3)) {
    if(ACPattern)
      CUTOPEN(); /* AC matching */
     printf("try rule 3\n");
    if(setChoicePoint()) {
      /* AC matching failed, try next rule */
      goto myend3;
    }
    if(ACPattern && MS_reinit(ms,v0,2)>0) {
      while(1) {
        if(MS_solve_rule(ms)<0) {
          goto myend3;
        }
        // printf("setChoicePoint\n");
        /* choicePoint AC matching */
        if(!setChoicePoint()) {
          break;
        }
        // printf("on revient d'un fail\n");
      }
    } else {
      goto myend3;
    }
    if(1) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      struct term *tmp, *sv[2];
      /* lhs: F(,)(var0,tt) */
      /* To protect the term */
      setShared(v0);
      substitution_build_without_context(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_210,2);
      /* allDetEvaluation: det */
      /* rhs: tt */
      res = con_205 ;
      if(ACPattern)
        CUTCLOSE(); /* AC matching */
      goto end;
    } else {
      fail();
    }
    myend3:;
    if(ACPattern)
      CUTCLOSE(); /* AC matching */
  }
match_fail:
  res=v0;
  tab_rewrite_step[0][210]++;
  goto end_no_rewrite;
end:
  rewrite_step++;
  tab_rewrite_step[1][210]++;
end_no_rewrite:
  if(trace>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  if(ACPattern && ms!=NULL)
    MS_delete(ms);
  subindent();
  restoreGlobalIndent();
  return res;
}

int match_subterm_210(struct term *v0,int no_arg_subject,int *mask,BG *cbg) {
  struct term *v1,*v2;
  int nb_bit=0;
  switch(getSymb(v0)) {
  case code_205: /* tt */
    mask[nb_bit++]=2;
    break;
  case code_211: /* G(,) */
    /* AC case: Not tested */
    nb_bit += match_subterm_AC(0, no_arg_subject, mask, cbg, match_subterm_210_211, no_pattern_210_211_niv_0, pattern_list_210_211, nb_pattern_210_211_niv_1,v0, max_nb_pattern_under_210_211);
    /* AC case: Not tested */
    nb_bit += match_subterm_AC(1, no_arg_subject, mask, cbg, match_subterm_210_211, no_pattern_210_211_niv_0, pattern_list_210_211, nb_pattern_210_211_niv_1,v0, max_nb_pattern_under_210_211);
    break;
  /* matching is not complete: jumpNode is null */
  }
  return nb_bit;
}

int match_subterm_210_211(struct term *v0,int no_arg_subject,int *mask,BG *cbg) {
  struct term *v1,*v2,*v3,*v4;
  int nb_bit=0;
  switch(getSymb(v0)) {
  case code_213: /* g() */
    v2=v0->sub[0];
    switch(getSymb(v2)) {
    default:
    label9:
      mask[nb_bit++]=1;
    }
    break;
  case code_212: /* f() */
    v3=v0->sub[0];
    switch(getSymb(v3)) {
    default:
    label7:
      mask[nb_bit++]=0;
    }
    break;
  /* matching is not complete: jumpNode is null */
  }
  return nb_bit;
}

void variable_extract_210(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* G(,)(f()(var1),f()(var2)) */
  case 0:
    {
      /* Not tested */
      int nb_variable=2;
      int nb_variable_ac=0;
      struct term *substitution[2];
      LINK *link;
      match_state *msbg;
      int i;
      printf("variable_extract_451: case 0\n");
      link=BG_link_get(ms->cbg,id_pattern);
      printf("id_pattern = %d\n",id_pattern);

      if(link==NULL) {
	printf("link == NULL\n");
        for(i=0 ; i<nb_variable ; i++) {
          extract_substitution[*indice]=v0;
          (*indice)++;
        }
      } else {
	printf("link != NULL\n");
        msbg=LINK_get(link,no_arg_subject);
        substitution_build(v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_210_211);
        for(i=0 ; i <nb_variable ; i++) {
          extract_substitution[*indice]=substitution[i];
          setShared(substitution[i]);
          (*indice)++;
        }
      }
    }
    break;
    /* G(,)(g()(var1),g()(var2)) */
  case 1:
    {
      /* Not tested */
      int nb_variable=2;
      int nb_variable_ac=0;
      struct term *substitution[2];
      LINK *link;
      match_state *msbg;
      int i;
      printf("variable_extract_451: case 1\n");
      link=BG_link_get(ms->cbg,id_pattern);
      printf("id_pattern = %d\n",id_pattern);

      if(link==NULL) {
	printf("link == NULL\n");
        for(i=0 ; i<nb_variable ; i++) {
          extract_substitution[*indice]=v0;
          (*indice)++;
        }
      } else {
	printf("link != NULL\n");
        msbg=LINK_get(link,no_arg_subject);
        substitution_build(v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_210_211);
        for(i=0 ; i <nb_variable ; i++) {
          extract_substitution[*indice]=substitution[i];
          setShared(substitution[i]);
          (*indice)++;
        }
      }
    }
    break;
    /* tt */
  case 2:
    break;
  default:
    fprintf(stderr,"variable_extract_210: bad pattern number\n");
    exit(0);
  }
}

void variable_extract_210_211(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* f()(var1) */
  case 0:
    extract_substitution[*indice]=v0->sub[0];
    (*indice)++;
    break;
    /* g()(var1) */
  case 1:
    extract_substitution[*indice]=v0->sub[0];
    (*indice)++;
    break;
  default:
    fprintf(stderr,"variable_extract_210_211: bad pattern number\n");
    exit(0);
  }
}

void init_pattern_list_210() {
  int pattern_tab[max_nb_pattern_under_210];
  no_pattern_210_niv_0=0;
  pattern_list_210=MS_pattern_list_create(nb_pattern_210_niv_0);
  /* F(,)(var0,G(,)(f()(var1),f()(var2))) */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_210,no_pattern_210_niv_0++,1,pattern_tab);
  /* F(,)(var0,G(,)(g()(var1),g()(var2))) */
  pattern_tab[0]=1;
  MS_pattern_list_init(pattern_list_210,no_pattern_210_niv_0++,1,pattern_tab);
  /* F(,)(var0,tt) */
  pattern_tab[0]=2;
  MS_pattern_list_init(pattern_list_210,no_pattern_210_niv_0++,1,pattern_tab);
}

void delete_pattern_list_210() {
  MS_pattern_list_free(pattern_list_210,no_pattern_210_niv_0);
}

void init_pattern_list_210_211() {
  int pattern_tab[max_nb_pattern_under_210_211];
  no_pattern_210_211_niv_0=0;
  pattern_list_210_211=MS_pattern_list_create(nb_pattern_210_211_niv_0);
  /* G(,)(f()(var1),f()(var2)) */
  pattern_tab[0]=0;
  pattern_tab[1]=0;
  MS_pattern_list_init(pattern_list_210_211,no_pattern_210_211_niv_0++,2,pattern_tab);
  /* G(,)(g()(var1),g()(var2)) */
  pattern_tab[0]=1;
  pattern_tab[1]=1;
  MS_pattern_list_init(pattern_list_210_211,no_pattern_210_211_niv_0++,2,pattern_tab);
}

void delete_pattern_list_210_211() {
  MS_pattern_list_free(pattern_list_210_211,no_pattern_210_211_niv_0);
}

struct term* fun_204( ) {
  struct term *v1,*v2;
  struct term *res;
  saveGlobalIndent()
  match_state *ms=NULL;
  int ACPattern=0;
  int indice=-1;
  bitSet32_stack_create(mask32,1);
  bitSet32_init_clear(mask32);
  addindent();
  if(trace>=2) {
    doindent(indentlevel);
    printf("start with: ");
    printf("fun_204_%s_(",fsymtab[204].name);
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet32_set(mask32,0);
  /* End syntactical matching */
  if(bitSet32_get(mask32,0)) {
    struct term *tmp, *sv[4];
    /* lhs: go */
    /* allDetEvaluation: det */
    CUTOPEN(); /* Wheres */
    if(setChoicePoint()) {
      /* local evaluations failed, try next rule */
      goto myend4;
    }
    /* rhs: F(,)(a,G(,)(g()(b),g()(c))) */
    TERM_ALLOC(sv[2],term1,code_213);
    sv[2]->sub[0] = con_207;
    TERM_ALLOC(sv[3],term1,code_213);
    sv[3]->sub[0] = con_208;
    TERM_ALLOC(sv[1],term2,code_211);
    term_add_onf_term_color(sv[1],sv[2],1);
    term_add_onf_term_color(sv[1],sv[3],2);
    TERM_ALLOC(sv[2],term2,code_210);
    term_add_onf_term_color(sv[2],con_206,1);
    term_add_onf_term_color(sv[2],sv[1],2);
    sv[2] = fun_210( sv[2] );
    res = sv[2] ;
    CUTCLOSE(); /* Wheres */
    goto end;
    myend4:;
    CUTCLOSE(); /* Wheres */
  }
match_fail:
  TERM_CONST_ALLOC(res,code_204);
  tab_rewrite_step[0][204]++;
  goto end_no_rewrite;
end:
  rewrite_step++;
  tab_rewrite_step[1][204]++;
end_no_rewrite:
  if(trace>=1) {
    doindent(indentlevel);
    printf("rewrite[%u] ",rewrite_step);
    term_printnl(stdout,res);
  }
  bitSet32_stack_delete(mask32);
  subindent();
  restoreGlobalIndent();
  return res;
}
