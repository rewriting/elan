#include "ans.h"
#include "main_skeleton.c"

/* Constantes d'execution */
unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];
int fsymtabSize = FSYM_TAB_SIZE;

/* Table des symboles */
fsym fsymtab[FSYM_TAB_SIZE];
/* Declaration des pattern_list */
void init_pattern_list_333();
void delete_pattern_list_333();
void init_pattern_list_334();
void delete_pattern_list_334();
          void init_pattern_list_413_tsome1_dk_334();
          void delete_pattern_list_413_tsome1_dk_334();

/* Constantes */
struct term *con_319;
struct term *con_1;
struct term *con_0;
struct term *con_341;
struct term *con_340;
struct term *con_339;
struct term *con_338;
struct term *con_337;
struct term *con_330;
struct term *con_329;
struct term *con_328;
struct term *con_326;

/* Redirection de built-ins */

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType funTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_302, 
(funTabType) &fun_303, (funTabType) &fun_304, (funTabType) &fun_305, 
(funTabType) &fun_306, (funTabType) &fun_307, (funTabType) &fun_308, 
(funTabType) &fun_309, (funTabType) &fun_310, (funTabType) &fun_311, 
(funTabType) &fun_312, (funTabType) &fun_313, (funTabType) &fun_314, 
(funTabType) &fun_315, (funTabType) &fun_316, (funTabType) &fun_317, 
(funTabType) &fun_318, NULL, NULL, (funTabType) &fun_321, NULL, 
(funTabType) &fun_323, (funTabType) &fun_324, (funTabType) &fun_325, 
NULL, (funTabType) &fun_327, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_333, (funTabType) &fun_334, (funTabType) &fun_335, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, (funTabType) &str_14, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_26, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_35, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, (funTabType) &str_240, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_341, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_413, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int strTabSize = 828;

/* Initialisation des patterns AC */
TERM *EkerTerm[100];
void EkerTermInit() {
  TERM_LIST *vlist[100];
  AC_LIST *acvlist[100];
  struct term *sv[100];
  /* *(var0,var1) */
    /* AC pattern construction phase */
  sv[5] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[5],1,(AC_LIST *) NULL);
  sv[4] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[4],1,acvlist[0]);
  sv[6] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[6]);
  //printf("\n");
  ac_sort((TERM*)sv[6]);
  EkerTerm[4] = (TERM*)sv[6];
  /* *(var1,var3) */
    /* AC pattern construction phase */
  sv[11] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[11],1,(AC_LIST *) NULL);
  sv[10] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[10],1,acvlist[0]);
  sv[12] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[12]);
  //printf("\n");
  ac_sort((TERM*)sv[12]);
  EkerTerm[12] = (TERM*)sv[12];
  /* +(var0,var5,var7) */
    /* AC pattern construction phase */
  sv[6] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[6],1,(AC_LIST *) NULL);
  sv[5] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[5],1,acvlist[0]);
  sv[4] = (struct term*) make_term(2,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[4],1,acvlist[0]);
  sv[7] = (struct term*) make_ac_term(333,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[7]);
  //printf("\n");
  ac_sort((TERM*)sv[7]);
  EkerTerm[8] = (TERM*)sv[7];
  /* +(var2,var3) */
    /* AC pattern construction phase */
  sv[5] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[5],1,(AC_LIST *) NULL);
  sv[4] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[4],1,acvlist[0]);
  sv[6] = (struct term*) make_ac_term(333,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[6]);
  //printf("\n");
  ac_sort((TERM*)sv[6]);
  EkerTerm[0] = (TERM*)sv[6];
  /* *(var6,(var3)) */
    /* AC pattern construction phase */
  sv[18] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[18],1,(AC_LIST *) NULL);
  sv[16] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[16],(TERM_LIST *) NULL);
  sv[17] = (struct term*) make_term(332,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[17],1,acvlist[0]);
  sv[19] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[19]);
  //printf("\n");
  ac_sort((TERM*)sv[19]);
  EkerTerm[10] = (TERM*)sv[19];
  /* *(var5,var2) */
    /* AC pattern construction phase */
  sv[16] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[16],1,(AC_LIST *) NULL);
  sv[15] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[15],1,acvlist[0]);
  sv[17] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[17]);
  //printf("\n");
  ac_sort((TERM*)sv[17]);
  EkerTerm[13] = (TERM*)sv[17];
  /* *(var3,+(var1,var2)) */
    /* AC pattern construction phase */
  sv[8] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[8],1,(AC_LIST *) NULL);
  sv[6] = (struct term*) make_term(1,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[6],1,(AC_LIST *) NULL);
  sv[5] = (struct term*) make_term(2,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[5],1,acvlist[1]);
  sv[7] = (struct term*) make_ac_term(333,acvlist[1],ACFUNC);
  acvlist[0] = make_ac_list((TERM*) sv[7],1,acvlist[0]);
  sv[9] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[9]);
  //printf("\n");
  ac_sort((TERM*)sv[9]);
  EkerTerm[7] = (TERM*)sv[9];
  /* *(var2,(var1)) */
    /* AC pattern construction phase */
  sv[12] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[12],1,(AC_LIST *) NULL);
  sv[10] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[10],(TERM_LIST *) NULL);
  sv[11] = (struct term*) make_term(332,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[11],1,acvlist[0]);
  sv[13] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[13]);
  //printf("\n");
  ac_sort((TERM*)sv[13]);
  EkerTerm[9] = (TERM*)sv[13];
  /* +(var0,var1) */
    /* AC pattern construction phase */
  sv[10] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[10],1,(AC_LIST *) NULL);
  sv[9] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[9],1,acvlist[0]);
  sv[11] = (struct term*) make_ac_term(333,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[11]);
  //printf("\n");
  ac_sort((TERM*)sv[11]);
  EkerTerm[1] = (TERM*)sv[11];
  /* *(var1,(var0)) */
    /* AC pattern construction phase */
  sv[6] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[6],1,(AC_LIST *) NULL);
  sv[4] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[4],(TERM_LIST *) NULL);
  sv[5] = (struct term*) make_term(332,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[5],1,acvlist[0]);
  sv[7] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[7]);
  //printf("\n");
  ac_sort((TERM*)sv[7]);
  EkerTerm[2] = (TERM*)sv[7];
  /* +(var0,var4,var6) */
    /* AC pattern construction phase */
  sv[6] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[6],1,(AC_LIST *) NULL);
  sv[5] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[5],1,acvlist[0]);
  sv[4] = (struct term*) make_term(2,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[4],1,acvlist[0]);
  sv[7] = (struct term*) make_ac_term(333,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[7]);
  //printf("\n");
  ac_sort((TERM*)sv[7]);
  EkerTerm[11] = (TERM*)sv[7];
  /* *(var5,(var2)) */
    /* AC pattern construction phase */
  sv[12] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[12],1,(AC_LIST *) NULL);
  sv[10] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[10],(TERM_LIST *) NULL);
  sv[11] = (struct term*) make_term(332,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[11],1,acvlist[0]);
  sv[13] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[13]);
  //printf("\n");
  ac_sort((TERM*)sv[13]);
  EkerTerm[3] = (TERM*)sv[13];
  /* *(var4,var2) */
    /* AC pattern construction phase */
  sv[10] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[10],1,(AC_LIST *) NULL);
  sv[9] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[9],1,acvlist[0]);
  sv[11] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[11]);
  //printf("\n");
  ac_sort((TERM*)sv[11]);
  EkerTerm[5] = (TERM*)sv[11];
  /* *(+(var1,var2),+(var3,var4)) */
    /* AC pattern construction phase */
  sv[10] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[10],1,(AC_LIST *) NULL);
  sv[9] = (struct term*) make_term(1,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[9],1,acvlist[0]);
  sv[11] = (struct term*) make_ac_term(333,acvlist[0],ACFUNC);
  acvlist[0] = make_ac_list((TERM*) sv[11],1,(AC_LIST *) NULL);
  sv[7] = (struct term*) make_term(2,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[7],1,(AC_LIST *) NULL);
  sv[6] = (struct term*) make_term(3,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[6],1,acvlist[1]);
  sv[8] = (struct term*) make_ac_term(333,acvlist[1],ACFUNC);
  acvlist[0] = make_ac_list((TERM*) sv[8],1,acvlist[0]);
  sv[12] = (struct term*) make_ac_term(334,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[12]);
  //printf("\n");
  ac_sort((TERM*)sv[12]);
  EkerTerm[6] = (TERM*)sv[12];
}

/* Query */
struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  printf("No ground term is defined\n");
  printf("Do not use -noInput in this case\n");
  exit(0);
  return res;
}
void symbol_init() {
  int i;
  //init_alloc();
  for(i=0 ; i<FSYM_TAB_SIZE ; i++) {
    fsym_init(i,0,"nullString",0,0,NULL);
  }
  fsym_init(code_19,2,"neq_variable(,)",19,0, NULL);
  fsym_init(code_18,2,"eq_variable(,)",18,0, NULL);
  fsym_init(code_15,2,"neq_ident(,)",15,0, NULL);
  fsym_init(code_14,2,"eq_ident(,)",14,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_68,1,"",0,0, NULL);
  fsym_init(code_319,0,"nil",0,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_318,1,"valueOf()",0,0, NULL);
  fsym_init(code_317,1,"itob_int()",0,0, NULL);
  fsym_init(code_316,1,"btoi_int()",0,0, NULL);
  fsym_init(code_315,2,"less_int(,)",0,0, NULL);
  fsym_init(code_314,2,"lesseq_int(,)",0,0, NULL);
  fsym_init(code_313,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_312,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_311,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_310,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_309,1,"umin()",0,0, NULL);
  fsym_init(code_308,2,"or(,)",0,0, NULL);
  fsym_init(code_307,2,"div(,)",0,0, NULL);
  fsym_init(code_306,2,"and(,)",0,0, NULL);
  fsym_init(code_305,2,"mod(,)",0,0, NULL);
  fsym_init(code_304,2,"time(,)",0,0, NULL);
  fsym_init(code_303,2,"minus(,)",0,0, NULL);
  fsym_init(code_302,2,"plus(,)",0,0, NULL);
  fsym_init(code_301,1,"[]",0,0, NULL);
  fsym_init(code_300,1,"",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_341,0,"tdfactorize",0,0, NULL);
  fsym_init(code_340,0,"sfactorize",0,0, NULL);
  fsym_init(code_33,2,"greatereq_variable(,)",33,0, NULL);
  fsym_init(code_32,2,"greater_variable(,)",32,0, NULL);
  fsym_init(code_31,2,"lesseq_variable(,)",31,0, NULL);
  fsym_init(code_30,2,"less_variable(,)",30,0, NULL);
  fsym_init(code_339,0,"tdexpand",0,0, NULL);
  fsym_init(code_338,0,"sexpand",0,0, NULL);
  fsym_init(code_337,0,"simplify",0,0, NULL);
  fsym_init(code_336,1,"pp()",0,0, NULL);
  fsym_init(code_335,2,"deriv(,)",0,0, NULL);
  fsym_init(code_334,-1,"*",0,0, NULL);
  fsym_init(code_333,-1,"+",0,0, NULL);
  fsym_init(code_332,1,"",0,0, NULL);
  fsym_init(code_331,1,"",0,0, NULL);
  fsym_init(code_330,0,"Z",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_79,1,"-",0,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_329,0,"Y",0,0, NULL);
  fsym_init(code_328,0,"X",0,0, NULL);
  fsym_init(code_327,0,"Vars",0,0, NULL);
  fsym_init(code_326,0,"listExtract",0,0, NULL);
  fsym_init(code_325,2,"ccat(,)",0,0, NULL);
  fsym_init(code_324,1,"size_of_identifier_list()",0,0, NULL);
  fsym_init(code_323,2,"-thelem()",0,0, NULL);
  fsym_init(code_322,1,"elem()",0,0, NULL);
  fsym_init(code_321,2,"@",0,0, NULL);
  fsym_init(code_320,2,",",0,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_319, code_319);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_341, code_341);
  TERM_CONST_ALLOC(con_340, code_340);
  TERM_CONST_ALLOC(con_339, code_339);
  TERM_CONST_ALLOC(con_338, code_338);
  TERM_CONST_ALLOC(con_337, code_337);
  TERM_CONST_ALLOC(con_330, code_330);
  TERM_CONST_ALLOC(con_329, code_329);
  TERM_CONST_ALLOC(con_328, code_328);
  TERM_CONST_ALLOC(con_326, code_326);
  /* Initialisation des pattern_list */
init_pattern_list_333();
init_pattern_list_334();
          init_pattern_list_413_tsome1_dk_334();
}
int gram[] = {
1,4596,3,58,3,1,58,2,43,1,58,
1,4696,4,58,3,1,58,2,45,1,58,
1,4796,5,58,3,1,58,2,42,1,58,
1,4896,6,58,3,1,58,2,47,1,58,
1,4896,27,58,3,1,58,2,37,1,58,
1,4896,28,58,3,1,58,2,38,1,58,
1,4896,29,58,3,1,58,2,124,1,58,
1,900,20,58,2,2,45,1,58,
1,4596,3,58,5,2,40,1,58,2,43,1,58,2,41,
1,4696,4,58,5,2,40,1,58,2,45,1,58,2,41,
1,4796,5,58,5,2,40,1,58,2,42,1,58,2,41,
1,4896,6,58,5,2,40,1,58,2,47,1,58,2,41,
1,4896,27,58,5,2,40,1,58,2,37,1,58,2,41,
1,4896,28,58,5,2,40,1,58,2,38,1,58,2,41,
1,4896,29,58,5,2,40,1,58,2,124,1,58,2,41,
1,900,20,58,4,2,40,2,45,1,58,2,41,
1,32768,3,58,6,0,452,2,40,1,58,2,44,1,58,2,41,
1,32768,4,58,6,0,556,2,40,1,58,2,44,1,58,2,41,
1,32768,5,58,6,0,642,2,40,1,58,2,44,1,58,2,41,
1,32768,6,58,6,0,323,2,40,1,58,2,44,1,58,2,41,
1,32768,27,58,6,0,531,2,40,1,58,2,44,1,58,2,41,
1,32768,28,58,6,0,518,2,40,1,58,2,44,1,58,2,41,
1,32768,29,58,6,0,225,2,40,1,58,2,44,1,58,2,41,
1,0,20,58,4,0,441,2,40,1,58,2,41,
1,32768,20,58,4,0,1594,2,40,1,58,2,41,
1,900,25,58,4,0,430,2,40,1,428,2,41,
1,33668,25,58,4,0,1583,2,40,1,428,2,41,
1,33668,318,58,4,0,722,2,40,1,331,2,41,
1,32768,300,59,1,1,32,
1,32768,322,59,4,0,419,2,40,1,187,2,41,
1,32768,323,59,7,1,331,2,45,0,220,0,419,2,40,1,187,2,41,
1,49152,337,133,1,0,1088,
1,49152,338,133,1,0,755,
1,49152,339,133,1,0,1278,
1,49152,340,133,1,0,1082,
1,49152,341,133,1,0,1183,
1,0,319,187,1,0,1688,
1,32768,319,187,1,0,534,
1,0,320,187,3,1,59,2,46,1,187,
1,0,320,187,6,0,435,2,40,1,59,2,44,1,187,2,41,
1,0,320,187,2,1,59,1,187,
1,32768,320,187,3,1,59,2,44,1,187,
1,0,321,187,6,0,632,2,40,1,187,2,44,1,187,2,41,
1,32768,321,187,3,1,187,2,64,1,187,
1,0,325,187,6,0,833,2,40,1,331,2,42,1,187,2,41,
1,32768,325,187,6,0,833,2,40,1,331,2,44,1,187,2,41,
1,32768,327,187,1,0,1045,
1,33768,301,331,1,1,58,
1,4596,302,331,3,1,331,2,43,1,331,
1,4696,303,331,3,1,331,2,45,1,331,
1,4796,304,331,3,1,331,2,42,1,331,
1,4896,305,331,3,1,331,2,37,1,331,
1,4896,306,331,3,1,331,2,38,1,331,
1,4896,307,331,3,1,331,2,47,1,331,
1,4896,308,331,3,1,331,2,124,1,331,
1,900,309,331,2,2,45,1,331,
1,500,302,331,5,2,40,1,331,2,43,1,331,2,41,
1,600,303,331,5,2,40,1,331,2,45,1,331,2,41,
1,700,304,331,5,2,40,1,331,2,42,1,331,2,41,
1,800,307,331,5,2,40,1,331,2,47,1,331,2,41,
1,800,305,331,5,2,40,1,331,2,37,1,331,2,41,
1,800,306,331,5,2,40,1,331,2,38,1,331,2,41,
1,800,308,331,5,2,40,1,331,2,124,1,331,2,41,
1,900,309,331,4,2,40,2,45,1,331,2,41,
1,33268,302,331,6,0,452,2,40,1,331,2,44,1,331,2,41,
1,33368,303,331,6,0,556,2,40,1,331,2,44,1,331,2,41,
1,33468,304,331,6,0,642,2,40,1,331,2,44,1,331,2,41,
1,33568,307,331,6,0,323,2,40,1,331,2,44,1,331,2,41,
1,33568,305,331,6,0,531,2,40,1,331,2,44,1,331,2,41,
1,33568,306,331,6,0,518,2,40,1,331,2,44,1,331,2,41,
1,33568,308,331,6,0,225,2,40,1,331,2,44,1,331,2,41,
1,33668,309,331,4,0,441,2,40,1,331,2,41,
1,900,316,331,4,0,430,2,40,1,428,2,41,
1,33668,316,331,4,0,856,2,40,1,428,2,41,
1,0,324,331,4,0,443,2,40,1,187,2,41,
1,32768,324,331,4,0,2444,2,40,1,187,2,41,
1,32768,328,338,1,0,88,
1,32768,329,338,1,0,89,
1,32768,330,338,1,0,90,
1,49152,326,347,1,0,1175,
1,32768,1,428,1,0,448,
1,32768,0,428,1,0,734,
1,4196,21,428,3,1,428,0,518,1,428,
1,4196,22,428,3,1,428,0,225,1,428,
1,36964,21,428,5,2,40,1,428,0,518,1,428,2,41,
1,36964,22,428,5,2,40,1,428,0,225,1,428,2,41,
1,200,24,428,4,0,337,2,40,1,428,2,41,
1,200,24,428,2,0,337,1,428,
1,200,24,428,4,2,40,0,337,1,428,2,41,
1,32968,24,428,6,2,40,0,337,2,40,1,428,2,41,2,41,
1,4296,8,428,4,1,428,2,61,2,61,1,428,
1,4296,9,428,4,1,428,2,33,2,61,1,428,
1,4296,10,428,3,1,428,2,60,1,428,
1,4296,11,428,4,1,428,2,60,2,61,1,428,
1,4296,12,428,3,1,428,2,62,1,428,
1,4296,13,428,4,1,428,2,62,2,61,1,428,
1,37064,8,428,6,0,737,2,40,1,428,2,44,1,428,2,41,
1,37064,9,428,6,0,847,2,40,1,428,2,44,1,428,2,41,
1,37064,10,428,6,0,962,2,40,1,428,2,44,1,428,2,41,
1,37064,11,428,6,0,1176,2,40,1,428,2,44,1,428,2,41,
1,37064,12,428,6,0,1269,2,40,1,428,2,44,1,428,2,41,
1,37064,13,428,6,0,1483,2,40,1,428,2,44,1,428,2,41,
1,0,14,428,4,1,32,2,61,2,61,1,32,
1,0,15,428,4,1,32,2,33,2,61,1,32,
1,32768,14,428,6,0,841,2,40,1,32,2,44,1,32,2,41,
1,32768,15,428,6,0,951,2,40,1,32,2,44,1,32,2,41,
1,0,18,428,4,1,59,2,61,2,61,1,59,
1,0,19,428,4,1,59,2,33,2,61,1,59,
1,0,18,428,6,0,1368,2,40,1,59,2,44,1,59,2,41,
1,0,19,428,6,0,1478,2,40,1,59,2,44,1,59,2,41,
1,300,8,428,4,1,58,2,61,2,61,1,58,
1,300,9,428,4,1,58,2,33,2,61,1,58,
1,300,10,428,3,1,58,2,60,1,58,
1,300,11,428,4,1,58,2,60,2,61,1,58,
1,300,12,428,3,1,58,2,62,1,58,
1,300,13,428,4,1,58,2,62,2,61,1,58,
1,300,8,428,6,0,1367,2,40,1,58,2,44,1,58,2,41,
1,300,9,428,6,0,1477,2,40,1,58,2,44,1,58,2,41,
1,300,10,428,6,0,1592,2,40,1,58,2,44,1,58,2,41,
1,300,11,428,6,0,1806,2,40,1,58,2,44,1,58,2,41,
1,300,12,428,6,0,1899,2,40,1,58,2,44,1,58,2,41,
1,300,13,428,6,0,2113,2,40,1,58,2,44,1,58,2,41,
1,900,26,428,4,0,1063,2,40,1,58,2,41,
1,33668,26,428,4,0,1794,2,40,1,58,2,41,
1,300,310,428,4,1,331,2,61,2,61,1,331,
1,300,311,428,4,1,331,2,33,2,61,1,331,
1,300,312,428,3,1,331,2,62,1,331,
1,300,313,428,4,1,331,2,62,2,61,1,331,
1,300,314,428,4,1,331,2,60,2,61,1,331,
1,300,315,428,3,1,331,2,60,1,331,
1,33068,310,428,6,0,640,2,40,1,331,2,44,1,331,2,41,
1,33068,311,428,6,0,961,2,40,1,331,2,44,1,331,2,41,
1,33068,315,428,6,0,865,2,40,1,331,2,44,1,331,2,41,
1,33068,314,428,6,0,1079,2,40,1,331,2,44,1,331,2,41,
1,33068,312,428,6,0,1172,2,40,1,331,2,44,1,331,2,41,
1,33068,313,428,6,0,1386,2,40,1,331,2,44,1,331,2,41,
1,900,317,428,4,0,1063,2,40,1,331,2,41,
1,33668,317,428,4,0,1067,2,40,1,331,2,41,
1,0,18,428,4,1,338,2,61,2,61,1,338,
1,0,19,428,4,1,338,2,33,2,61,1,338,
1,32768,18,428,6,0,1147,2,40,1,338,2,44,1,338,2,41,
1,32768,19,428,6,0,1257,2,40,1,338,2,44,1,338,2,41,
1,33768,331,452,1,1,338,
1,33768,332,452,1,1,331,
1,8193,333,452,5,2,40,1,452,2,43,1,452,2,41,
1,40961,333,452,3,1,452,2,43,1,452,
1,8194,334,452,5,2,40,1,452,2,42,1,452,2,41,
1,40962,334,452,3,1,452,2,42,1,452,
1,32768,335,452,6,0,538,2,40,1,452,2,44,1,338,2,41,
1,32768,336,452,4,0,224,2,40,1,452,2,41,
1,1000,301,331,3,2,91,1,58,2,93,
1,0,18,428,4,1,59,2,61,2,61,1,59,
1,0,19,428,4,1,59,2,33,2,61,1,59,
1,0,30,428,3,1,59,2,60,1,59,
1,0,31,428,4,1,59,2,60,2,61,1,59,
1,0,32,428,3,1,59,2,62,1,59,
1,0,33,428,4,1,59,2,62,2,61,1,59,
1,0,18,428,6,0,1368,2,40,1,59,2,44,1,59,2,41,
1,0,19,428,6,0,1478,2,40,1,59,2,44,1,59,2,41,
1,0,30,428,6,0,1593,2,40,1,59,2,44,1,59,2,41,
1,0,31,428,6,0,1807,2,40,1,59,2,44,1,59,2,41,
1,0,32,428,6,0,1900,2,40,1,59,2,44,1,59,2,41,
1,0,33,428,6,0,2114,2,40,1,59,2,44,1,59,2,41,
1,0,18,428,4,1,338,2,61,2,61,1,338,
1,0,19,428,4,1,338,2,33,2,61,1,338,
1,0,30,428,3,1,338,2,60,1,338,
1,0,31,428,4,1,338,2,60,2,61,1,338,
1,0,32,428,3,1,338,2,62,1,338,
1,0,33,428,4,1,338,2,62,2,61,1,338,
1,0,18,428,6,0,1147,2,40,1,338,2,44,1,338,2,41,
1,0,19,428,6,0,1257,2,40,1,338,2,44,1,338,2,41,
1,32768,30,428,6,0,1372,2,40,1,338,2,44,1,338,2,41,
1,32768,31,428,6,0,1586,2,40,1,338,2,44,1,338,2,41,
1,32768,32,428,6,0,1679,2,40,1,338,2,44,1,338,2,41,
1,32768,33,428,6,0,1893,2,40,1,338,2,44,1,338,2,41,
1,36768,68,32,1,1,220,
1,36768,67,58,1,1,19,
1,36768,79,58,2,2,45,1,19,
0};
int pos = 2376;
int earleyQuerySort = 452;
int earleyQueryStrategy = 413;
char *tabIdentStr[] = {
"pp",
"int",
"greatereq_bool",
"builtin",
"th",
"part",
"tone",
"dont",
"sexpand",
"specification",
"identifier",
"cons",
"builtinInt",
"size_of",
"iterate",
"then",
"less_variable",
"factorize",
"btoi",
"n",
"module",
"greater_bool",
"for",
"lesseq_variable",
"less_bool",
"neq_int",
"size_of_identifier",
"greatereq",
"neq",
"time",
"btoi_builtinInt",
"div",
"repeat",
"implicit",
"one",
"eq_int",
"dkconcur",
"cmp",
"where",
"greater_identifier",
"btoi_int",
"deriv",
"neq_identifier",
"neq_builtinInt",
"itob_builtinInt",
"choose",
"nil",
"expand",
"END",
"inline",
"eq",
"ident",
"of",
"mod",
"AND",
"tall",
"nil_identifier",
"bool",
"greater",
"eq_identifier",
"l",
"self",
"eq_builtinInt",
"bs",
"assocRight",
"anyInteger",
"Vars",
"greater_builtinInt",
"public",
"e",
"d",
"c",
"b",
"greatereq_variable",
"neq_variable",
"neq_ident",
"a",
"append",
"dcconcur",
"extractrule2",
"end",
"extractrule1",
"neq_bool",
"Z",
"dk",
"firstOne",
"eq_variable",
"eq_ident",
"id",
"local",
"alias",
"ee",
"greater_variable",
"elem",
"eq_bool",
"if",
"false",
"someVariables",
"anyIdentifier",
"fail",
"Epsilon",
"code",
"global",
"Y",
"SUCH",
"X",
"call",
"THAT",
"care",
"variable",
"dc",
"and",
"handline",
"P",
"case",
"size_of_identifier_list",
"ccat",
"check",
"valueOf",
"declare",
"dkcall",
"META",
"assocLeft",
"dccall",
"poly5",
"greatereq_identifier",
"greatereq_builtinInt",
"defined",
"Id",
"EACH",
"dcOne",
"p4",
"p3",
"p2",
"p1",
"n2",
"n3",
"hardAlias",
"n1",
"l2",
"l1",
"definedAs",
"topfactorize",
"normout",
"simplify",
"stratop",
"IF",
"strategies",
"sfactorize",
"export",
"operators",
"ANDIF",
"result",
"try",
"query",
"strategy",
"description",
"tdfactorize",
"sort",
"lesseq_int",
"plus",
"AC",
"otherwise",
"import",
"poly",
"queryend",
"start",
"minus",
"rules",
"spart",
"lesseq_bool",
"tsome",
"listExtract",
"LPL",
"FOR",
"greater_int",
"true",
"know",
"first",
"greatereq_int",
"with",
"itob_int",
"size",
"topexpand",
"umin",
"normalize",
"normin",
"itob",
"y",
"switch",
"x",
"source",
"private",
"not",
"list",
"tdexpand",
"oneconcur",
"lesseq",
"umin_builtinInt",
"normalise",
"less_identifier",
"less_builtinInt",
"less",
"pri",
"explicit",
"less_int",
"lesseq_identifier",
"lesseq_builtinInt",
"or",
""};
int tabIdentIndex[] = {
224,
542,
1483,
759,
220,
439,
438,
437,
755,
1377,
1059,
435,
1058,
751,
750,
431,
1372,
967,
430,
110,
646,
1269,
327,
1586,
962,
961,
1905,
960,
324,
642,
1583,
323,
641,
859,
322,
640,
857,
320,
539,
1900,
856,
538,
1478,
1477,
1794,
852,
534,
851,
215,
850,
214,
532,
213,
531,
211,
429,
1688,
428,
746,
1368,
108,
426,
1367,
424,
1047,
1046,
1045,
1899,
639,
101,
100,
99,
98,
1893,
1257,
951,
97,
632,
849,
1253,
311,
1252,
847,
90,
207,
842,
1147,
841,
205,
523,
522,
202,
1679,
419,
737,
418,
734,
1357,
1355,
412,
730,
411,
625,
89,
307,
88,
623,
305,
622,
838,
199,
518,
835,
80,
834,
2444,
833,
510,
722,
720,
619,
295,
932,
611,
505,
2114,
2113,
719,
173,
273,
489,
164,
163,
162,
161,
160,
372,
905,
159,
158,
157,
899,
1306,
788,
1088,
781,
143,
1083,
1082,
674,
991,
354,
671,
351,
566,
883,
1188,
1183,
456,
1079,
452,
132,
986,
667,
663,
877,
558,
556,
555,
554,
1176,
552,
1175,
232,
231,
1172,
448,
447,
763,
1386,
444,
1067,
443,
979,
441,
977,
659,
1063,
121,
658,
120,
657,
974,
337,
655,
1278,
972,
653,
1594,
970,
1593,
1592,
650,
331,
866,
865,
1807,
1806,
225,
0};
int tabIdentSize = 209;
char *tabSortStr[] = {
"<identifier->identifier>",
"ident",
"poly",
"intern string",
"bool",
"identifier",
"builtinInt",
"string",
"intern int",
"variable",
"intern ident",
"int",
"<poly->poly>",
"list[identifier]",
""};
int tabSortIndex[] = {
347,
32,
452,
351,
428,
59,
58,
163,
19,
338,
220,
331,
133,
187,
0};
int tabSortSize = 14;
char *tabStrategyStr[] = {
"tdfactorize:poly/poly5[Vars]",
"simplify:poly/poly5[Vars]",
"sfactorize:poly/poly5[Vars]",
"sexpand:poly/poly5[Vars]",
"tdexpand:poly/poly5[Vars]",
"listExtract:identifier/list[identifier]",
""};
int tabStrategyIndex[] = {
341,
35,
240,
413,
14,
26,
0};
int tabStrategySize = 6;
