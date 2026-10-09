#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "bool3.h"
#include "back.h"
#include "builtin.h"
#include "streval.h"

/* Constantes d'execution */
int trace = 0; /* 1 : result, 2 : start with */
long rewrite_step=0;
int indentlevel=0;

/* Table des symboles */
fsym fsymtab[217];
/* Declaration des pattern_list */
void init_pattern_list_211();
void delete_pattern_list_211();
void init_pattern_list_212();
void delete_pattern_list_212();

/* Constantes */
struct term *con_210;
struct term *con_1;
struct term *con_0;
struct term *con_209;
struct term *con_208;
struct term *con_207;
struct term *con_206;
struct term *con_205;
struct term *con_204;
struct term *con_203;
struct term *con_201;
struct term *con_200;

/* Redirection de built-ins */

#ifdef BORO

/* substrategies for streval */

/* defined strategies */

#endif

/* Procedure principale */
long *bp_main;
main() {
  long bp;
  struct term *res;
  bp_main=&bp;
  backTrackInit();
  init_alloc();
  fsym_init(code_216,0,"start",0,0, NULL);
  fsym_init(code_215,1,"not",0,0, NULL);
  fsym_init(code_19,2,"neq_Bool3(,)",19,0, NULL);
  fsym_init(code_214,2,"or",0,0, NULL);
  fsym_init(code_18,2,"eq_Bool3(,)",18,0, NULL);
  fsym_init(code_213,2,"and",0,0, NULL);
  fsym_init(code_212,-1,"m",0,0, NULL);
  fsym_init(code_211,-1,"p",0,0, NULL);
  fsym_init(code_210,0,"a8",0,0, NULL);
  fsym_init(code_13,2,"greatereq_bool(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_bool(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_bool(,)",11,0, NULL);
  fsym_init(code_10,2,"less_bool(,)",10,0, NULL);
  fsym_init(code_9,2,"neq_bool(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_bool(,)",8,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_33,2,"greatereq_Bool3(,)",33,0, NULL);
  fsym_init(code_209,0,"a7",0,0, NULL);
  fsym_init(code_32,2,"greater_Bool3(,)",32,0, NULL);
  fsym_init(code_208,0,"a6",0,0, NULL);
  fsym_init(code_31,2,"lesseq_Bool3(,)",31,0, NULL);
  fsym_init(code_207,0,"a5",0,0, NULL);
  fsym_init(code_30,2,"less_Bool3(,)",30,0, NULL);
  fsym_init(code_206,0,"a4",0,0, NULL);
  fsym_init(code_205,0,"a3",0,0, NULL);
  fsym_init(code_204,0,"a2",0,0, NULL);
  fsym_init(code_203,0,"a1",0,0, NULL);
  fsym_init(code_202,0,"b2",0,0, NULL);
  fsym_init(code_201,0,"b1",0,0, NULL);
  fsym_init(code_200,0,"b0",0,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  
/* Initialidsation des constantes */
  TERM_CONST_ALLOC(con_210, code_210);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_209, code_209);
  TERM_CONST_ALLOC(con_208, code_208);
  TERM_CONST_ALLOC(con_207, code_207);
  TERM_CONST_ALLOC(con_206, code_206);
  TERM_CONST_ALLOC(con_205, code_205);
  TERM_CONST_ALLOC(con_204, code_204);
  TERM_CONST_ALLOC(con_203, code_203);
  TERM_CONST_ALLOC(con_201, code_201);
  TERM_CONST_ALLOC(con_200, code_200);
  /* Initialisation des pattern_list */
  init_pattern_list_211();
  init_pattern_list_212();

  if (!setChoicePoint()) {
    /* TERME DE DEPART */
    {
      /* start[var=0, subst=-1, NOSHARE] */
      struct term *tmp, *sv0;
      sv0 = fun_216(  );
      res=sv0;
    }
    printf("\nresult[%d] = ",rewrite_step);
    if((long)res==0 || (long)res==1) {
      printf("%d\n",res);
    } else {
      term_printnl(stdout,res);
    }
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_213].name);
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  TERM_FREE(con_210);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_209);
  TERM_FREE(con_208);
  TERM_FREE(con_207);
  TERM_FREE(con_206);
  TERM_FREE(con_205);
  TERM_FREE(con_204);
  TERM_FREE(con_203);
  TERM_FREE(con_201);
  TERM_FREE(con_200);
  /* Destruction des pattern_list */
  delete_pattern_list_211();
  delete_pattern_list_212();
#ifdef DEBUG
  print_space_usage();
#endif
  printf("\nrewrite_step = %d\n",rewrite_step);
}

struct term* fun_214(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_214(");
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
    default:
      switch(getSymb(v2)) {
        default:
          bitSet_set(mask,0);
      }
  }
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: or(var0[mv][var=1, subst=-1, NOSHARE, 4],var1[mv][var=2, subst=-1, NOSHARE, 4])[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9;

    setShared(v1);
    setShared(v2);

    /* where var2[var=0, subst=-1, NOSHARE, 2] := m(,)[var0[2][var=-1, subst=-1, PERFECTSHARE]][var=1, subst=-1, NOSHARE] */
    TERM_ALLOC(sv1,termac,code_212);
    term_add_onf_term(sv1,v1);
    term_add_onf_term(sv1,v1);
    sv1 = fun_212( sv1 );
    sv0 = sv1;

    setShared(sv0);

    /* where var3[var=2, subst=-1, NOSHARE, 2] := m(,)[var1[2][var=-1, subst=-1, PERFECTSHARE]][var=3, subst=-1, NOSHARE] */
    TERM_ALLOC(sv3,termac,code_212);
    term_add_onf_term(sv3,v2);
    term_add_onf_term(sv3,v2);
    sv3 = fun_212( sv3 );
    sv2 = sv3;

    setShared(sv2);

    /* rhs: p(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE].m(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE]][var=8, subst=-1, NOSHARE].m(,)[var0[var=-1, subst=-1, PERFECTSHARE].var3[var=-1, subst=-1, PERFECTSHARE]][var=7, subst=-1, NOSHARE].m(,)[var1[var=-1, subst=-1, PERFECTSHARE].var2[var=-1, subst=-1, PERFECTSHARE]][var=6, subst=-1, NOSHARE].m(,)[var2[var=-1, subst=-1, PERFECTSHARE].var3[var=-1, subst=-1, PERFECTSHARE].b2[var=4, subst=-1, NOSHARE]][var=5, subst=-1, NOSHARE]][var=9, subst=-1, NOSHARE] */
    sv4 = fun_202(  );
    TERM_ALLOC(sv5,termac,code_212);
    term_add_onf_term(sv5,sv0);
    term_add_onf_term(sv5,sv2);
    term_add_onf_term(sv5,sv4);
    sv5 = fun_212( sv5 );
    TERM_ALLOC(sv6,termac,code_212);
    term_add_onf_term(sv6,v2);
    term_add_onf_term(sv6,sv0);
    sv6 = fun_212( sv6 );
    TERM_ALLOC(sv7,termac,code_212);
    term_add_onf_term(sv7,v1);
    term_add_onf_term(sv7,sv2);
    sv7 = fun_212( sv7 );
    TERM_ALLOC(sv8,termac,code_212);
    term_add_onf_term(sv8,v1);
    term_add_onf_term(sv8,v2);
    sv8 = fun_212( sv8 );
    TERM_ALLOC(sv9,termac,code_211);
    term_add_onf_term(sv9,v1);
    term_add_onf_term(sv9,v2);
    term_add_onf_term(sv9,sv8);
    term_add_onf_term(sv9,sv7);
    term_add_onf_term(sv9,sv6);
    term_add_onf_term(sv9,sv5);
    sv9 = fun_211( sv9 );
    res = sv9 ;
    goto end;
    myend0:;
  }
match_fail:
  TERM_ALLOC(res,term2, 214);
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

struct term* fun_216( ) {
  struct term *v1,*v2;
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
    /* lhs: start[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10, *sv11, *sv12, *sv13, *sv14, *sv15, *sv16, *sv17, *sv18, *sv19, *sv20, *sv21, *sv22, *sv23, *sv24, *sv25, *sv26, *sv27, *sv28, *sv29, *sv30, *sv31, *sv32, *sv33, *sv34, *sv35, *sv36, *sv37, *sv38, *sv39;
    /* rhs: eq_Bool3(and(and(and(a1[var=35, subst=-1, NOSHARE],a2[var=34, subst=-1, NOSHARE])[var=36, subst=-1, NOSHARE],and(a3[var=32, subst=-1, NOSHARE],a4[var=31, subst=-1, NOSHARE])[var=33, subst=-1, NOSHARE])[var=37, subst=-1, NOSHARE],and(and(a5[var=28, subst=-1, NOSHARE],a6[var=27, subst=-1, NOSHARE])[var=29, subst=-1, NOSHARE],and(a7[var=25, subst=-1, NOSHARE],a8[var=24, subst=-1, NOSHARE])[var=26, subst=-1, NOSHARE])[var=30, subst=-1, NOSHARE])[var=38, subst=-1, NOSHARE],not(or(or(or(not(a1[var=18, subst=-1, NOSHARE])[var=19, subst=-1, NOSHARE],not(a2[var=16, subst=-1, NOSHARE])[var=17, subst=-1, NOSHARE])[var=20, subst=-1, NOSHARE],or(not(a3[var=13, subst=-1, NOSHARE])[var=14, subst=-1, NOSHARE],not(a4[var=11, subst=-1, NOSHARE])[var=12, subst=-1, NOSHARE])[var=15, subst=-1, NOSHARE])[var=21, subst=-1, NOSHARE],or(or(not(a5[var=7, subst=-1, NOSHARE])[var=8, subst=-1, NOSHARE],not(a6[var=5, subst=-1, NOSHARE])[var=6, subst=-1, NOSHARE])[var=9, subst=-1, NOSHARE],or(not(a7[var=2, subst=-1, NOSHARE])[var=3, subst=-1, NOSHARE],not(a8[var=0, subst=-1, NOSHARE])[var=1, subst=-1, NOSHARE])[var=4, subst=-1, NOSHARE])[var=10, subst=-1, NOSHARE])[var=22, subst=-1, NOSHARE])[var=23, subst=-1, NOSHARE])[var=39, subst=-1, NOSHARE] */
    
    sv1 = fun_215( con_210 );
    sv3 = fun_215( con_209 );
    sv4 = fun_214( sv3,sv1 );
    sv6 = fun_215( con_208 );
    sv8 = fun_215( con_207 );
    sv9 = fun_214( sv8,sv6 );
    sv10 = fun_214( sv9,sv4 );
    sv12 = fun_215( con_206 );
    sv14 = fun_215( con_205 );
    sv15 = fun_214( sv14,sv12 );
    sv17 = fun_215( con_204 );
    sv19 = fun_215( con_203 );
    sv20 = fun_214( sv19,sv17 );
    sv21 = fun_214( sv20,sv15 );
    sv22 = fun_214( sv21,sv10 );
    sv23 = fun_215( sv22 );
    
    sv26 = fun_213( con_209,con_210 );
    sv29 = fun_213( con_207,con_208 );
    sv30 = fun_213( sv29,sv26 );
    sv33 = fun_213( con_205,con_206 );
    sv36 = fun_213( con_203,con_204 );
    sv37 = fun_213( sv36,sv33 );
    sv38 = fun_213( sv37,sv30 );
    sv39 = fun_18( sv38,sv23 );
    res = sv39;
    

    /*
    sv26 = fun_213( con_209,con_210 );
    sv29 = fun_213( con_207,con_208 );
    sv30 = fun_213( sv29,sv26 );
    sv33 = fun_213( con_205,con_206 );
    sv36 = fun_213( con_203,con_204 );
    sv37 = fun_213( sv36,sv33 );
    sv38 = fun_213( sv37,sv30 );
    res = sv38;
    */

    /*    
    sv36 = fun_213( con_203,con_204 );
    res = fun_213( con_205,sv36);
    */

    goto end;
    myend1:; 
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
int match_subterm_211(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_211(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static int **pattern_list_211;
static int no_pattern_211_niv_0;
static int nb_pattern_211_niv_0 = 2;
static int nb_pattern_211_niv_1 = 1;
#define max_nb_pattern_under_211 1

struct term* fun_211(struct term *v0 ) {
  struct term *v1,*v2;
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
      /* [] p(,)[var0[var=0, subst=0, NOSHARE, 1].b0[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => var0[var=-1, subst=-1, PERFECTSHARE] */
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
    myend2:;
  }
  if(bitSet_get(mask,1)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] p(,)[var0[3][var=0, subst=0, NOSHARE].var1[var=2, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] => p(,)[var1[var=-1, subst=-1, PERFECTSHARE].b0[var=1, subst=-1, NOSHARE]][var=3, subst=-1, NOSHARE] */
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
        indice=next_pe_extract(nb_arg_subject-1,E,sol,3);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv0,E,sol,3,&sv0,&sv2);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend3;
        }
      }
      tmp = term_removeTopSymbol(sv2);
      if(tmp == NULL)
        sv2=fun_211(sv2);
      else
        sv2=tmp;
      TERM_ALLOC(sv3,termac,code_211);
      term_add_onf_term(sv3,sv2);
      term_add_onf_term(sv3,con_200);
      sv3 = fun_211( sv3 );
      res = sv3 ;
      goto end;
    }
    myend3:;
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
  struct term *v1,*v2;
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(getSymb(v0)) {
    case code_200:
      bitSet_set(mask,0);
    break;
  }
  return nb_bit;
}

void variable_extract_211(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* b0[var=0, subst=-1, NOSHARE] */
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
  /* p(,)[var0[var=0, subst=0, NOSHARE, 1].b0[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_211,no_pattern_211_niv_0++,1,pattern_tab);
  /* p(,)[var0[3][var=0, subst=0, NOSHARE].var1[var=2, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] */
}

void delete_pattern_list_211() {
  MS_pattern_list_free(pattern_list_211,no_pattern_211_niv_0);
}

struct term* fun_213(struct term *v1,struct term *v2 ) {
  struct term *v3,*v4,*v5,*v6;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_213(");
    term_print(stdout,v1);
    printf(",");
    term_print(stdout,v2);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
    default:
      switch(getSymb(v2)) {
        default:
          bitSet_set(mask,0);
      }
  }
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: and(var0[mv][var=1, subst=-1, NOSHARE, 3],var1[mv][var=2, subst=-1, NOSHARE, 3])[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5, *sv6, *sv7, *sv8, *sv9, *sv10, *sv11; 

    /*    
    printf("coucou1\n");
    setShared(v1);
    setShared(v2);
    printf("coucou2\n");
    printf("v1 = "); term_printnl(stdout,v1);
    printf("v2 = "); term_printnl(stdout,v2);
    */

    /* where var2[var=0, subst=-1, NOSHARE, 2] := m(,)[var0[2][var=-1, subst=-1, PERFECTSHARE]][var=1, subst=-1, NOSHARE] */
    TERM_ALLOC(sv1,termac,code_212);
    term_add_onf_term(sv1,v1);
    term_add_onf_term(sv1,v1);
    sv1 = fun_212( sv1 );
    sv0 = sv1;

    //printf("coucou3\n");

    setShared(sv0);

    /* where var3[var=2, subst=-1, NOSHARE, 2] := m(,)[var1[2][var=-1, subst=-1, PERFECTSHARE]][var=3, subst=-1, NOSHARE] */
    TERM_ALLOC(sv3,termac,code_212);
    term_add_onf_term(sv3,v2);
    term_add_onf_term(sv3,v2);
    sv3 = fun_212( sv3 );
    sv2 = sv3;

    setShared(sv2);

    /*
    printf("sv1 = "); term_printnl(stdout,sv1);
    printf("sv2 = "); term_printnl(stdout,sv2);
    */

    /* rhs: p(,)[m(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE].b2[var=9, subst=-1, NOSHARE]][var=10, subst=-1, NOSHARE].m(,)[var0[var=-1, subst=-1, PERFECTSHARE].var3[var=-1, subst=-1, PERFECTSHARE].b2[var=7, subst=-1, NOSHARE]][var=8, subst=-1, NOSHARE].m(,)[var1[var=-1, subst=-1, PERFECTSHARE].var2[var=-1, subst=-1, PERFECTSHARE].b2[var=5, subst=-1, NOSHARE]][var=6, subst=-1, NOSHARE].m(,)[var2[var=-1, subst=-1, PERFECTSHARE].var3[var=-1, subst=-1, PERFECTSHARE]][var=4, subst=-1, NOSHARE]][var=11, subst=-1, NOSHARE] */
    TERM_ALLOC(sv4,termac,code_212);
    term_add_onf_term(sv4,sv0);
    term_add_onf_term(sv4,sv2);
    sv4 = fun_212( sv4 );
    sv5 = fun_202(  );
    TERM_ALLOC(sv6,termac,code_212);
    term_add_onf_term(sv6,v2);
    term_add_onf_term(sv6,sv0);
    term_add_onf_term(sv6,sv5);
    sv6 = fun_212( sv6 );
    sv7 = fun_202(  );
    TERM_ALLOC(sv8,termac,code_212);
    term_add_onf_term(sv8,v1);
    term_add_onf_term(sv8,sv2);
    term_add_onf_term(sv8,sv7);
    sv8 = fun_212( sv8 );
    sv9 = fun_202(  );
    TERM_ALLOC(sv10,termac,code_212);
    term_add_onf_term(sv10,v1);
    term_add_onf_term(sv10,v2);
    term_add_onf_term(sv10,sv9);
    sv10 = fun_212( sv10 );
    TERM_ALLOC(sv11,termac,code_211);
    term_add_onf_term(sv11,sv10);
    term_add_onf_term(sv11,sv8);
    term_add_onf_term(sv11,sv6);
    term_add_onf_term(sv11,sv4);
    sv11 = fun_211( sv11 );
    res = sv11 ;
    goto end;
    myend4:;
  }
match_fail:
  TERM_ALLOC(res,term2, 213);
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

struct term* fun_215(struct term *v1 ) {
  struct term *v2,*v3,*v4;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_215(");
    term_print(stdout,v1);
    printf(")\n");
  }
  /* Begin syntactical matching */
  switch(getSymb(v1)) {
    default:
      bitSet_set(mask,0);
  }
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: not(var0[mv][var=1, subst=-1, NOSHARE, 1])[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1, *sv2, *sv3;
    /* rhs: p(,)[b1[var=2, subst=-1, NOSHARE].m(,)[var0[var=-1, subst=-1, PERFECTSHARE].b2[var=0, subst=-1, NOSHARE]][var=1, subst=-1, NOSHARE]][var=3, subst=-1, NOSHARE] */
    sv0 = fun_202(  );
    TERM_ALLOC(sv1,termac,code_212);
    term_add_onf_term(sv1,v1);
    term_add_onf_term(sv1,sv0);
    sv1 = fun_212( sv1 );
    TERM_ALLOC(sv3,termac,code_211);
    term_add_onf_term(sv3,con_201);
    term_add_onf_term(sv3,sv1);
    sv3 = fun_211( sv3 );
    res = sv3 ;
    goto end;
    myend5:;
  }
match_fail:
  TERM_ALLOC(res,term1, 215);
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
int match_subterm_212(struct term *v0,int no_arg_subject, bitSet *mask, BG *cbg);
void variable_extract_212(struct term *v0,int id_pattern,struct term *extract_substitution[],int *indice,struct match_state *ms,int no_arg_subject,int no_pattern);
static int **pattern_list_212;
static int no_pattern_212_niv_0;
static int nb_pattern_212_niv_0 = 4;
static int nb_pattern_212_niv_1 = 3;
#define max_nb_pattern_under_212 1

struct term* fun_212(struct term *v0 ) {
  struct term *v1,*v2;
  bitSet *mask;
  struct term *res;
  match_state *ms=NULL;
  int mode;
  int necessary_link;
  int indice=-1;
  int ACPattern=0;
  bitSet_create(mask,4);
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
  bitSet_set(mask,3);
  /* End syntactical matching */
  if( bitSet_get(mask,0) || bitSet_get(mask,1) || bitSet_get(mask,2) || bitSet_get(mask,3) ) {
    ACPattern=1;
    /* Begin AC matching */
    necessary_link=0;
    mode=POSSIBLE_REST;
    ms=MS_create();
    MS_init(ms, match_subterm_212, no_pattern_212_niv_0, pattern_list_212, nb_pattern_212_niv_1, v0, necessary_link, max_nb_pattern_under_212);
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
      /* [] m(,)[var0[var=0, subst=0, NOSHARE].b0[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => b0[var=1, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_212);
      res = con_200 ;
      goto end;
    }
    myend6:;
  }
  if(bitSet_get(mask,1)) {
    if(indice>=0 && ms->no_rule==1) {
      int nb_variable=1;
      int nb_variable_ac=1;
      struct term *substitution[1];
      int i;
      /* [] m(,)[var0[var=0, subst=0, NOSHARE, 1].b1[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => var0[var=-1, subst=-1, PERFECTSHARE] */
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
    myend7:;
  }
  if(bitSet_get(mask,2)) {
    if(indice>=0 && ms->no_rule==2) {
      int nb_variable=3;
      int nb_variable_ac=1;
      struct term *substitution[3];
      int i;
      /* [] m(,)[var2[var=0, subst=0, NOSHARE, 2].p(,)[var0[var=3, subst=1, NOSHARE, 1].var1[var=1, subst=2, NOSHARE, 1]][var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] => p(,)[m(,)[var0[var=-1, subst=-1, PERFECTSHARE].var2[var=-1, subst=-1, PERFECTSHARE]][var=4, subst=-1, NOSHARE].m(,)[var1[var=-1, subst=-1, PERFECTSHARE].var2[var=-1, subst=-1, PERFECTSHARE]][var=2, subst=-1, NOSHARE]][var=5, subst=-1, NOSHARE] */
      struct term *tmp, *sv0, *sv1, *sv2, *sv3, *sv4, *sv5;
      substitution_build(v0,ms,nb_variable,substitution,nb_variable_ac,variable_extract_212);
      sv3=substitution[1];
      sv1=substitution[2];
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
          extract_xy_from_pe(sv3,E,sol,1,&sv3,&sv1);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend8;
        }
      }
      sv0=substitution[0];
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_212(sv0);
      else
        sv0=tmp;

      setShared(sv0);

      tmp = term_removeTopSymbol(sv1);
      if(tmp != NULL)
        sv1=tmp;
      TERM_ALLOC(sv2,termac,code_212);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_212( sv2 );
      tmp = term_removeTopSymbol(sv3);
      if(tmp != NULL)
        sv3=tmp;
      TERM_ALLOC(sv4,termac,code_212);
      term_add_onf_term(sv4,sv3);
      term_add_onf_term(sv4,sv0);
      sv4 = fun_212( sv4 );
      TERM_ALLOC(sv5,termac,code_211);
      term_add_onf_term(sv5,sv4);
      term_add_onf_term(sv5,sv2);
      sv5 = fun_211( sv5 );
      res = sv5 ;
      goto end;
    }
    myend8:;
  }
  if(bitSet_get(mask,3)) {
    {
      int nb_variable=2;
      int nb_variable_ac=1;
      struct term *substitution[2];
      int i;
      /* [] m(,)[var0[3][var=1, subst=0, NOSHARE, 1].var1[var=0, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] => m(,)[var0[var=-1, subst=-1, PERFECTSHARE].var1[var=-1, subst=-1, PERFECTSHARE]][var=2, subst=-1, NOSHARE] */
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
        indice=next_pe_extract(nb_arg_subject-1,E,sol,3);
        if(indice >= 0 ) {
          int i;
          extract_xy_from_pe(sv1,E,sol,3,&sv1,&sv0);
          IFREE(E);
          IFREE(sol);
        } else {
          IFREE(E);
          IFREE(sol);
          goto myend9;
        }
      }
      tmp = term_removeTopSymbol(sv0);
      if(tmp == NULL)
        sv0=fun_212(sv0);
      else
        sv0=tmp;
      tmp = term_removeTopSymbol(sv1);
      if(tmp == NULL)
        sv1=fun_212(sv1);
      else
        sv1=tmp;
      TERM_ALLOC(sv2,termac,code_212);
      term_add_onf_term(sv2,sv1);
      term_add_onf_term(sv2,sv0);
      sv2 = fun_212( sv2 );
      res = sv2 ;
      goto end;
    }
    myend9:;
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
  struct term *v1,*v2;
  int nb_bit=0;
  bitSet_init_clear(mask);
  switch(getSymb(v0)) {
    case code_211:
      /* AC case: Not tested */
      bitSet_set(mask,2);
    break;
    case code_201:
      bitSet_set(mask,1);
    break;
    case code_200:
      bitSet_set(mask,0);
    break;
  }
  return nb_bit;
}

void variable_extract_212(struct term *v0, int id_pattern, struct term *extract_substitution[], int *indice, struct match_state *ms, int no_arg_subject, int no_pattern) {
  switch(id_pattern) {
    /* b0[var=0, subst=-1, NOSHARE] */
  case 0:
    break;
    /* b1[var=0, subst=-1, NOSHARE] */
  case 1:
    break;
    /* p(,)[var0[var=3, subst=1, NOSHARE, 1].var1[var=1, subst=2, NOSHARE, 1]][var=0, subst=-1, NOSHARE] */
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
        substitution_build(v0,msbg,nb_variable,substitution,nb_variable_ac,variable_extract_212);
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

void init_pattern_list_212() {
  int pattern_tab[max_nb_pattern_under_212];
  no_pattern_212_niv_0=0;
  pattern_list_212=MS_pattern_list_create(nb_pattern_212_niv_0);
  /* m(,)[var0[var=0, subst=0, NOSHARE].b0[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=0;
  MS_pattern_list_init(pattern_list_212,no_pattern_212_niv_0++,1,pattern_tab);
  /* m(,)[var0[var=0, subst=0, NOSHARE, 1].b1[var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=1;
  MS_pattern_list_init(pattern_list_212,no_pattern_212_niv_0++,1,pattern_tab);
  /* m(,)[var2[var=0, subst=0, NOSHARE, 2].p(,)[var0[var=3, subst=1, NOSHARE, 1].var1[var=1, subst=2, NOSHARE, 1]][var=0, subst=-1, NOSHARE]][mv][var=0, subst=-1, NOSHARE] */
  pattern_tab[0]=2;
  MS_pattern_list_init(pattern_list_212,no_pattern_212_niv_0++,1,pattern_tab);
  /* m(,)[var0[3][var=1, subst=0, NOSHARE, 1].var1[var=0, subst=1, NOSHARE, 1]][mv][var=0, subst=-1, NOSHARE] */
}

void delete_pattern_list_212() {
  MS_pattern_list_free(pattern_list_212,no_pattern_212_niv_0);
}

struct term* fun_202( ) {
  struct term *v1,*v2;
  bitSet *mask;
  struct term *res;
  bitSet_create(mask,1);
  bitSet_init_clear(mask);
  addindent();
  if(trace>=2) {
    indent(indentlevel);
    printf("start with: ");
    printf("fun_202(");
    printf(")\n");
  }
  /* Begin syntactical matching */
  bitSet_set(mask,0);
  /* End syntactical matching */
  if(bitSet_get(mask,0)) {
    /* lhs: b2[mv][var=0, subst=-1, NOSHARE] */
    struct term *tmp, *sv0, *sv1;
    /* rhs: p(,)[b1[2][var=0, subst=-1, NOSHARE]][var=1, subst=-1, NOSHARE] */
    TERM_ALLOC(sv1,termac,code_211);
    term_add_onf_term(sv1,con_201);
    term_add_onf_term(sv1,con_201);
    sv1 = fun_211( sv1 );
    res = sv1 ;
    goto end;
    myend10:;
  }
match_fail:
  fprintf(stderr,"Match Fail error in fun_202\n");
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
