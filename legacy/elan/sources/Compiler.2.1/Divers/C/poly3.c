#include "poly3.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 241;
fsym fsymtab[241];
/* Declaration des pattern_list */
void init_pattern_list_234();
void delete_pattern_list_234();
void init_pattern_list_233();
void delete_pattern_list_233();
          void init_pattern_list_33_cons1_repeat1_one_233();
          void delete_pattern_list_33_cons1_repeat1_one_233();

/* Constantes */
struct term *con_229;
struct term *con_228;
struct term *con_226;
struct term *con_219;
struct term *con_240;
struct term *con_239;
struct term *con_230;
struct term *con_1;
struct term *con_0;

/* Redirection de built-ins */

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
#ifdef __cplusplus
typedef struct term* (*funTabType)(...);
#else
typedef struct term* (*funTabType)();
#endif

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
NULL, NULL, NULL, NULL, (funTabType) &fun_202, (funTabType) &fun_203, 
(funTabType) &fun_204, (funTabType) &fun_205, (funTabType) &fun_206, 
(funTabType) &fun_207, (funTabType) &fun_208, (funTabType) &fun_209, 
(funTabType) &fun_210, (funTabType) &fun_211, (funTabType) &fun_212, 
(funTabType) &fun_213, (funTabType) &fun_214, (funTabType) &fun_215, 
(funTabType) &fun_216, (funTabType) &fun_217, (funTabType) &fun_218, 
NULL, NULL, (funTabType) &fun_221, NULL, (funTabType) &fun_223, 
(funTabType) &fun_224, (funTabType) &fun_225, NULL, 
(funTabType) &fun_227, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_233, (funTabType) &fun_234, (funTabType) &fun_235, 
(funTabType) &fun_236, NULL, (funTabType) &fun_238, NULL, NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, (funTabType) &str_26, NULL, NULL, NULL, NULL, 
NULL, NULL, (funTabType) &str_33, NULL};

/* Initialisation des patterns AC */
TERM *EkerTerm[100];
static void EkerTermInit() {
  TERM_LIST *vlist[100];
  AC_LIST *acvlist[100];
  struct term *sv[100];
  /* (+)((*)(var3,var4),(*)(var3,var5)) */
    /* AC pattern construction phase */
  sv[8] = (struct term*) make_term(0,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[8],1,(AC_LIST *) NULL);
  sv[7] = (struct term*) make_term(1,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[7],1,acvlist[1]);
  sv[9] = (struct term*) make_ac_term(234,acvlist[1],ACFUNC);
  acvlist[1] = make_ac_list((TERM*) sv[9],1,(AC_LIST *) NULL);
  sv[8] = (struct term*) make_term(0,NULL,VAR);
  acvlist[2] = make_ac_list((TERM*) sv[8],1,(AC_LIST *) NULL);
  sv[5] = (struct term*) make_term(2,NULL,VAR);
  acvlist[2] = make_ac_list((TERM*) sv[5],1,acvlist[2]);
  sv[6] = (struct term*) make_ac_term(234,acvlist[2],ACFUNC);
  acvlist[1] = make_ac_list((TERM*) sv[6],1,acvlist[1]);
  sv[10] = (struct term*) make_ac_term(233,acvlist[1],ACFUNC);
  //eker_print_term((TERM*)sv[10]);
  //printf("\n");
  ac_sort((TERM*)sv[10]);
  EkerTerm[4] = (TERM*)sv[10];
  /* (+)(var5,var6) */
    /* AC pattern construction phase */
  sv[6] = (struct term*) make_term(0,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[6],1,(AC_LIST *) NULL);
  sv[5] = (struct term*) make_term(1,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[5],1,acvlist[1]);
  sv[7] = (struct term*) make_ac_term(233,acvlist[1],ACFUNC);
  //eker_print_term((TERM*)sv[7]);
  //printf("\n");
  ac_sort((TERM*)sv[7]);
  EkerTerm[2] = (TERM*)sv[7];
  /* (+)(var9,var10) */
    /* AC pattern construction phase */
  sv[11] = (struct term*) make_term(0,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[11],1,(AC_LIST *) NULL);
  sv[10] = (struct term*) make_term(1,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[10],1,acvlist[1]);
  sv[12] = (struct term*) make_ac_term(233,acvlist[1],ACFUNC);
  //eker_print_term((TERM*)sv[12]);
  //printf("\n");
  ac_sort((TERM*)sv[12]);
  EkerTerm[1] = (TERM*)sv[12];
  /* (+)(var7,var8) */
    /* AC pattern construction phase */
  sv[6] = (struct term*) make_term(0,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[6],1,(AC_LIST *) NULL);
  sv[5] = (struct term*) make_term(1,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[5],1,acvlist[1]);
  sv[7] = (struct term*) make_ac_term(233,acvlist[1],ACFUNC);
  //eker_print_term((TERM*)sv[7]);
  //printf("\n");
  ac_sort((TERM*)sv[7]);
  EkerTerm[0] = (TERM*)sv[7];
  /* (+)((*)(var3,(var4)),(*)(var3,(var5))) */
    /* AC pattern construction phase */
  sv[10] = (struct term*) make_term(0,NULL,VAR);
  acvlist[1] = make_ac_list((TERM*) sv[10],1,(AC_LIST *) NULL);
  sv[8] = (struct term*) make_term(1,NULL,VAR);
  vlist[2] = make_term_list((TERM*) sv[8],(TERM_LIST *) NULL);
  sv[9] = (struct term*) make_term(232,vlist[2],FUNC);
  acvlist[1] = make_ac_list((TERM*) sv[9],1,acvlist[1]);
  sv[11] = (struct term*) make_ac_term(234,acvlist[1],ACFUNC);
  acvlist[1] = make_ac_list((TERM*) sv[11],1,(AC_LIST *) NULL);
  sv[10] = (struct term*) make_term(0,NULL,VAR);
  acvlist[2] = make_ac_list((TERM*) sv[10],1,(AC_LIST *) NULL);
  sv[5] = (struct term*) make_term(2,NULL,VAR);
  vlist[3] = make_term_list((TERM*) sv[5],(TERM_LIST *) NULL);
  sv[6] = (struct term*) make_term(232,vlist[3],FUNC);
  acvlist[2] = make_ac_list((TERM*) sv[6],1,acvlist[2]);
  sv[7] = (struct term*) make_ac_term(234,acvlist[2],ACFUNC);
  acvlist[1] = make_ac_list((TERM*) sv[7],1,acvlist[1]);
  sv[12] = (struct term*) make_ac_term(233,acvlist[1],ACFUNC);
  //eker_print_term((TERM*)sv[12]);
  //printf("\n");
  ac_sort((TERM*)sv[12]);
  EkerTerm[3] = (TERM*)sv[12];
}

/* Query */
static struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  {
    /* deriv(,)((+)((*)((X),(Y),([](2))),(^)((X),[](2)),(^)((^)((Y),[](2)),[](2))),X) */
    struct term *tmp, *sv[6];
    TERM_ALLOC(sv[2],term1,code_231);
    sv[2]->sub[0] = con_228;
    TERM_ALLOC(sv[3],term1,code_231);
    sv[3]->sub[0] = con_229;
    TERM_ALLOC(sv[4],term1,code_201);
    sv[4]->sub[0] = (setIntegerTag(2));
    TERM_ALLOC(sv[1],term1,code_232);
    sv[1]->sub[0] = sv[4];
    TERM_ALLOC(sv[4],term2,code_234);
    term_add_onf_term_color(sv[4],sv[2],1);
    term_add_onf_term_color(sv[4],sv[3],2);
    term_add_onf_term_color(sv[4],sv[1],3);
    sv[4] = fun_234( sv[4] );
    TERM_ALLOC(sv[2],term1,code_231);
    sv[2]->sub[0] = con_228;
    TERM_ALLOC(sv[3],term1,code_201);
    sv[3]->sub[0] = (setIntegerTag(2));
    sv[1] = fun_235( sv[2],sv[3] );
    TERM_ALLOC(sv[3],term1,code_231);
    sv[3]->sub[0] = con_229;
    TERM_ALLOC(sv[5],term1,code_201);
    sv[5]->sub[0] = (setIntegerTag(2));
    sv[2] = fun_235( sv[3],sv[5] );
    TERM_ALLOC(sv[5],term1,code_201);
    sv[5]->sub[0] = (setIntegerTag(2));
    sv[3] = fun_235( sv[2],sv[5] );
    TERM_ALLOC(sv[2],term2,code_233);
    term_add_onf_term_color(sv[2],sv[4],1);
    term_add_onf_term_color(sv[2],sv[1],2);
    term_add_onf_term_color(sv[2],sv[3],3);
    sv[2] = fun_233( sv[2] );
    sv[3] = fun_236( sv[2],con_228 );
    res=str_33(sv[3]);
  }
  return res;
}

/* Procedure principale */
#ifndef CSETCHP
long *bp_main;
#endif
struct rusage before_self,after_self;
long diff_time_self;
int main(int argc,char **argv) {
#ifdef CSETCHP
  char bp;
#else
  long bp;
#endif
  struct term *res;
  int i,j;
  int queryMode=0;
  int REFMode=0;
#ifdef CSETCHP
  choice_init(&bp);
#else
  bp_main=&bp;
#endif
  GC_free_space_divisor=2;
  for(i=1 ; i<argc && *argv[i]=='-' ; i++) {
    if(!strcmp(argv[i],"-query")) {
      queryMode=1;
    }
    if(!strcmp(argv[i],"-REF")) {
      REFMode=1;
    }
    if(!strcmp(argv[i],"-debug")) {
      debugMode=1;
    }
  }
  init_alloc();
  for(i=0 ; i<fsymtabSize ; i++) {
    fsym_init(i,0,"nullString",0,0,NULL);
  }
  fsym_init(code_229,0,"Y",0,0, NULL);
  fsym_init(code_228,0,"X",0,0, NULL);
  fsym_init(code_227,0,"Vars",0,0, NULL);
  fsym_init(code_226,0,"listExtract",0,0, NULL);
  fsym_init(code_225,2,"ccat(,)",0,0, NULL);
  fsym_init(code_224,1,"size_of_identifier_list()",0,0, NULL);
  fsym_init(code_223,2,"-thelem()",0,0, NULL);
  fsym_init(code_222,1,"elem()",0,0, NULL);
  fsym_init(code_221,2,"@",0,0, NULL);
  fsym_init(code_220,2,".",0,0, NULL);
  fsym_init(code_219,0,"nil",0,0, NULL);
  fsym_init(code_218,1,"valueOf()",0,0, NULL);
  fsym_init(code_217,1,"itob_int()",0,0, NULL);
  fsym_init(code_216,1,"btoi_int()",0,0, NULL);
  fsym_init(code_215,2,"less_int(,)",0,0, NULL);
  fsym_init(code_214,2,"lesseq_int(,)",0,0, NULL);
  fsym_init(code_213,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_212,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_211,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_210,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_209,1,"umin()",0,0, NULL);
  fsym_init(code_208,2,"or(,)",0,0, NULL);
  fsym_init(code_207,2,"div(,)",0,0, NULL);
  fsym_init(code_33,2,"greatereq_variable(,)",33,0, NULL);
  fsym_init(code_206,2,"and(,)",0,0, NULL);
  fsym_init(code_32,2,"greater_variable(,)",32,0, NULL);
  fsym_init(code_205,2,"mod(,)",0,0, NULL);
  fsym_init(code_31,2,"lesseq_variable(,)",31,0, NULL);
  fsym_init(code_204,2,"time(,)",0,0, NULL);
  fsym_init(code_30,2,"less_variable(,)",30,0, NULL);
  fsym_init(code_203,2,"minus(,)",0,0, NULL);
  fsym_init(code_202,2,"plus(,)",0,0, NULL);
  fsym_init(code_201,1,"[]",0,0, NULL);
  fsym_init(code_200,1,"",0,0, NULL);
  fsym_init(code_79,1,"-",0,0, NULL);
  fsym_init(code_240,0,"simplify",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_68,1,"",0,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_239,0,"last_simplify",0,0, NULL);
  fsym_init(code_238,0,"go",0,0, NULL);
  fsym_init(code_237,1,"pp()",0,0, NULL);
  fsym_init(code_236,2,"deriv(,)",0,0, NULL);
  fsym_init(code_235,2,"(^)",0,0, NULL);
  fsym_init(code_234,-1,"(*)",0,0, NULL);
  fsym_init(code_233,-1,"(+)",0,0, NULL);
  fsym_init(code_232,1,"",0,0, NULL);
  fsym_init(code_231,1,"",0,0, NULL);
  fsym_init(code_230,0,"Z",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_19,2,"neq_variable(,)",19,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_18,2,"eq_variable(,)",18,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_15,2,"neq_ident(,)",15,0, NULL);
  fsym_init(code_14,2,"eq_ident(,)",14,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_229, code_229);
  TERM_CONST_ALLOC(con_228, code_228);
  TERM_CONST_ALLOC(con_226, code_226);
  TERM_CONST_ALLOC(con_219, code_219);
  TERM_CONST_ALLOC(con_240, code_240);
  TERM_CONST_ALLOC(con_239, code_239);
  TERM_CONST_ALLOC(con_230, code_230);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  /* Initialisation des pattern_list */
init_pattern_list_234();
init_pattern_list_233();
          init_pattern_list_33_cons1_repeat1_one_233();
  if(queryMode) {
    yyparse();
  }
  EkerTermInit();

  getrusage(RUSAGE_SELF, &before_self);
  backTrackInit();
  if (!setChoicePoint()) {
    if(queryMode) {
      printf("start with: "); term_printnl(stdout,query);
      res = normalise(query);
      res=str_33(res);
    } else {
      res=main_query();
    }
    if(!REFMode) {
      printf("\nresult = ");
    }
    if((long)res==0 || (long)res==1) {
      printf("%d\n",res);
    } else {
      if(REFMode) {
        term_printREFln(stdout,res);
      } else {
        term_printnl(stdout,res);
      }
    }
    if(!REFMode) {
      backStatistics();
      globalStatistics();
    }
    fail();
  }
end:
  getrusage(RUSAGE_SELF, &after_self);
  diff_time_self = ((after_self.ru_utime.tv_sec*1000) + (after_self.ru_utime.tv_usec/1000)) - ((before_self.ru_utime.tv_sec*1000) + (before_self.ru_utime.tv_usec/1000));
destruction:
  FREE(fsymtab[code_229].name);
  FREE(fsymtab[code_228].name);
  FREE(fsymtab[code_227].name);
  FREE(fsymtab[code_226].name);
  FREE(fsymtab[code_225].name);
  FREE(fsymtab[code_224].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_219].name);
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
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_79].name);
  FREE(fsymtab[code_240].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_20].name);
  FREE(fsymtab[code_68].name);
  FREE(fsymtab[code_67].name);
  FREE(fsymtab[code_239].name);
  FREE(fsymtab[code_238].name);
  FREE(fsymtab[code_237].name);
  FREE(fsymtab[code_236].name);
  FREE(fsymtab[code_235].name);
  FREE(fsymtab[code_234].name);
  FREE(fsymtab[code_233].name);
  FREE(fsymtab[code_232].name);
  FREE(fsymtab[code_231].name);
  FREE(fsymtab[code_230].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_15].name);
  FREE(fsymtab[code_14].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  TERM_FREE(con_229);
  TERM_FREE(con_228);
  TERM_FREE(con_226);
  TERM_FREE(con_219);
  TERM_FREE(con_240);
  TERM_FREE(con_239);
  TERM_FREE(con_230);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  /* Destruction des pattern_list */
delete_pattern_list_234();
delete_pattern_list_233();
          delete_pattern_list_33_cons1_repeat1_one_233();
#ifdef DEBUG
  backStatistics();
#endif
  if(!REFMode) {
    printf("\nrewrite_step  = %u\n",rewrite_step);
    printf("total time    = %.3f sec\n",((double)(diff_time_self))/1000.0);
    if(diff_time_self > 0) {
      printf("average speed = %d rwr/sec\n",(long)((1000.0*(double)rewrite_step)/(double)diff_time_self));
    }
  }
  exit(0);
}
#include "ac_tools.c"
