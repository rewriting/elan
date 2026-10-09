#include "queensAC.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 232;
fsym fsymtab[232];
/* Declaration des pattern_list */

/* Constantes */
struct term *con_218;
struct term *con_231;
struct term *con_229;
struct term *con_226;
struct term *con_225;
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
NULL, NULL, NULL, (funTabType) &fun_201, (funTabType) &fun_202, 
(funTabType) &fun_203, (funTabType) &fun_204, (funTabType) &fun_205, 
(funTabType) &fun_206, (funTabType) &fun_207, (funTabType) &fun_208, 
(funTabType) &fun_209, (funTabType) &fun_210, (funTabType) &fun_211, 
(funTabType) &fun_212, (funTabType) &fun_213, (funTabType) &fun_214, 
(funTabType) &fun_215, (funTabType) &fun_216, (funTabType) &fun_217, 
NULL, NULL, (funTabType) &fun_220, NULL, (funTabType) &fun_222, 
(funTabType) &fun_223, (funTabType) &fun_224, NULL, NULL, 
(funTabType) &fun_227, NULL, NULL, NULL, NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_10, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_70, NULL};

/* Initialisation des patterns AC */
TERM *EkerTerm[100];
static void EkerTermInit() {
  TERM_LIST *vlist[100];
  AC_LIST *acvlist[100];
  struct term *sv[100];
  /* U(var11,(var10)) */
    /* AC pattern construction phase */
  sv[29] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[29],1,(AC_LIST *) NULL);
  sv[27] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[27],(TERM_LIST *) NULL);
  sv[28] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[28],1,acvlist[0]);
  sv[30] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[30]);
  //printf("\n");
  ac_sort((TERM*)sv[30]);
  EkerTerm[3] = (TERM*)sv[30];
  /* U(var14,(var13)) */
    /* AC pattern construction phase */
  sv[38] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[38],1,(AC_LIST *) NULL);
  sv[36] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[36],(TERM_LIST *) NULL);
  sv[37] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[37],1,acvlist[0]);
  sv[39] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[39]);
  //printf("\n");
  ac_sort((TERM*)sv[39]);
  EkerTerm[4] = (TERM*)sv[39];
  /* U(var8,(var7)) */
    /* AC pattern construction phase */
  sv[20] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[20],1,(AC_LIST *) NULL);
  sv[18] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[18],(TERM_LIST *) NULL);
  sv[19] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[19],1,acvlist[0]);
  sv[21] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[21]);
  //printf("\n");
  ac_sort((TERM*)sv[21]);
  EkerTerm[2] = (TERM*)sv[21];
  /* U(var22,(var0)) */
    /* AC pattern construction phase */
  sv[65] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[65],1,(AC_LIST *) NULL);
  sv[63] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[63],(TERM_LIST *) NULL);
  sv[64] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[64],1,acvlist[0]);
  sv[66] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[66]);
  //printf("\n");
  ac_sort((TERM*)sv[66]);
  EkerTerm[7] = (TERM*)sv[66];
  /* U(var20,(var19)) */
    /* AC pattern construction phase */
  sv[56] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[56],1,(AC_LIST *) NULL);
  sv[54] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[54],(TERM_LIST *) NULL);
  sv[55] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[55],1,acvlist[0]);
  sv[57] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[57]);
  //printf("\n");
  ac_sort((TERM*)sv[57]);
  EkerTerm[6] = (TERM*)sv[57];
  /* U(var5,(var4)) */
    /* AC pattern construction phase */
  sv[10] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[10],1,(AC_LIST *) NULL);
  sv[8] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[8],(TERM_LIST *) NULL);
  sv[9] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[9],1,acvlist[0]);
  sv[11] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[11]);
  //printf("\n");
  ac_sort((TERM*)sv[11]);
  EkerTerm[1] = (TERM*)sv[11];
  /* U(var17,(var16)) */
    /* AC pattern construction phase */
  sv[47] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[47],1,(AC_LIST *) NULL);
  sv[45] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[45],(TERM_LIST *) NULL);
  sv[46] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[46],1,acvlist[0]);
  sv[48] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[48]);
  //printf("\n");
  ac_sort((TERM*)sv[48]);
  EkerTerm[5] = (TERM*)sv[48];
  /* U(var3,(var2)) */
    /* AC pattern construction phase */
  sv[4] = (struct term*) make_term(0,NULL,VAR);
  acvlist[0] = make_ac_list((TERM*) sv[4],1,(AC_LIST *) NULL);
  sv[2] = (struct term*) make_term(1,NULL,VAR);
  vlist[1] = make_term_list((TERM*) sv[2],(TERM_LIST *) NULL);
  sv[3] = (struct term*) make_term(230,vlist[1],FUNC);
  acvlist[0] = make_ac_list((TERM*) sv[3],1,acvlist[0]);
  sv[5] = (struct term*) make_ac_term(228,acvlist[0],ACFUNC);
  //eker_print_term((TERM*)sv[5]);
  //printf("\n");
  ac_sort((TERM*)sv[5]);
  EkerTerm[0] = (TERM*)sv[5];
}

/* Query */
static struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  {
    /* queens */
    struct term *tmp, *sv[1];
    res=str_10(con_226);
  }
  return res;
}

/* Procedure principale */
long *bp_main;
int main(int argc,char **argv) {
  long bp;
  struct term *res;
  int i,j;
  int queryMode=0;
  int REFMode=0;
  bp_main=&bp;
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
  fsym_init(code_218,0,"nil",0,0, NULL);
  fsym_init(code_217,1,"valueOf()",0,0, NULL);
  fsym_init(code_216,1,"itob_int()",0,0, NULL);
  fsym_init(code_215,1,"btoi_int()",0,0, NULL);
  fsym_init(code_214,2,"less_int(,)",0,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_213,2,"lesseq_int(,)",0,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_212,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_211,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_210,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_231,0,"queens",0,0, NULL);
  fsym_init(code_230,1,"",0,0, NULL);
  fsym_init(code_209,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_208,1,"umin()",0,0, NULL);
  fsym_init(code_207,2,"or(,)",0,0, NULL);
  fsym_init(code_206,2,"div(,)",0,0, NULL);
  fsym_init(code_205,2,"and(,)",0,0, NULL);
  fsym_init(code_204,2,"mod(,)",0,0, NULL);
  fsym_init(code_203,2,"time(,)",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_229,0,"empty",0,0, NULL);
  fsym_init(code_202,2,"minus(,)",0,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_228,-1,"U",0,0, NULL);
  fsym_init(code_201,2,"plus(,)",0,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_227,3,"ok(,,)",0,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_200,1,"[]",0,0, NULL);
  fsym_init(code_226,0,"queens",0,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_225,0,"listExtract",0,0, NULL);
  fsym_init(code_224,2,"ccat(,)",0,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_223,1,"size_of_int_list()",0,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_222,2,"-thelem()",0,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_221,1,"elem()",0,0, NULL);
  fsym_init(code_220,2,"@",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_219,2,".",0,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_218, code_218);
  TERM_CONST_ALLOC(con_231, code_231);
  TERM_CONST_ALLOC(con_229, code_229);
  TERM_CONST_ALLOC(con_226, code_226);
  TERM_CONST_ALLOC(con_225, code_225);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  /* Initialisation des pattern_list */
  if(queryMode) {
    yyparse();
  }
  EkerTermInit();

  backTrackInit();
  if (!setChoicePoint()) {
    if(queryMode) {
      res = normalise(query);
      res=str_10(res);
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
    }
    fail();
  }
end:
destruction:
  FREE(fsymtab[code_218].name);
  FREE(fsymtab[code_217].name);
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_213].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_231].name);
  FREE(fsymtab[code_230].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_229].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_228].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_227].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_226].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_225].name);
  FREE(fsymtab[code_224].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_20].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_219].name);
  TERM_FREE(con_218);
  TERM_FREE(con_231);
  TERM_FREE(con_229);
  TERM_FREE(con_226);
  TERM_FREE(con_225);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  /* Destruction des pattern_list */
#ifdef DEBUG
  backStatistics();
#endif
  if(!REFMode) {
    printf("\nrewrite_step = %u\n",rewrite_step);
  }
  exit(0);
}
#include "ac_tools.c"
