#include "natc10.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 255;
fsym fsymtab[255];
/* Declaration des pattern_list */

/* Constantes */
struct term *con_1;
struct term *con_0;
struct term *con_226;
struct term *con_225;
struct term *con_218;

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
(funTabType) &fun_227, (funTabType) &fun_228, NULL, 
(funTabType) &fun_230, NULL, (funTabType) &fun_232, NULL, 
(funTabType) &fun_234, NULL, (funTabType) &fun_236, NULL, 
(funTabType) &fun_238, NULL, (funTabType) &fun_240, NULL, 
(funTabType) &fun_242, NULL, (funTabType) &fun_244, NULL, 
(funTabType) &fun_246, NULL, NULL, (funTabType) &fun_249, 
(funTabType) &fun_250, (funTabType) &fun_251, (funTabType) &fun_252, 
(funTabType) &fun_253, (funTabType) &fun_254, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, (funTabType) &str_24, NULL};

/* Initialisation des patterns AC */
TERM *EkerTerm[100];
static void EkerTermInit() {
  TERM_LIST *vlist[100];
  AC_LIST *acvlist[100];
  struct term *sv[100];
}

/* Query */
static struct term *main_query() {
  struct term *res;
  /* TERME DE DEPART */
  printf("Cannot reduce an unground term\n");
  printf("Use the -query option to input the query\n");
  exit(0);
  return res;
}

/* Procedure principale */
#ifndef CSETCHP
long *bp_main;
#endif
struct rusage before_self,after_self;
long diff_sec,diff_usec;
double total_time;
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
  fsym_init(code_212,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_211,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_210,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_209,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_208,1,"umin()",0,0, NULL);
  fsym_init(code_207,2,"or(,)",0,0, NULL);
  fsym_init(code_206,2,"div(,)",0,0, NULL);
  fsym_init(code_205,2,"and(,)",0,0, NULL);
  fsym_init(code_204,2,"mod(,)",0,0, NULL);
  fsym_init(code_203,2,"time(,)",0,0, NULL);
  fsym_init(code_202,2,"minus(,)",0,0, NULL);
  fsym_init(code_201,2,"plus(,)",0,0, NULL);
  fsym_init(code_200,1,"[]",0,0, NULL);
  fsym_init(code_254,0,"l",0,0, NULL);
  fsym_init(code_253,1,"fib()",0,0, NULL);
  fsym_init(code_252,1,"fact()",0,0, NULL);
  fsym_init(code_251,1,"prec()",0,0, NULL);
  fsym_init(code_250,2,"*",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_249,2,"+",0,0, NULL);
  fsym_init(code_248,1,"mult_10()",0,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_247,1,"():",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_246,1,"mult_9()",0,0, NULL);
  fsym_init(code_245,1,"()9",0,0, NULL);
  fsym_init(code_244,1,"mult_8()",0,0, NULL);
  fsym_init(code_243,1,"()8",0,0, NULL);
  fsym_init(code_242,1,"mult_7()",0,0, NULL);
  fsym_init(code_241,1,"()7",0,0, NULL);
  fsym_init(code_240,1,"mult_6()",0,0, NULL);
  fsym_init(code_239,1,"()6",0,0, NULL);
  fsym_init(code_238,1,"mult_5()",0,0, NULL);
  fsym_init(code_237,1,"()5",0,0, NULL);
  fsym_init(code_236,1,"mult_4()",0,0, NULL);
  fsym_init(code_235,1,"()4",0,0, NULL);
  fsym_init(code_234,1,"mult_3()",0,0, NULL);
  fsym_init(code_233,1,"()3",0,0, NULL);
  fsym_init(code_232,1,"mult_2()",0,0, NULL);
  fsym_init(code_231,1,"()2",0,0, NULL);
  fsym_init(code_230,1,"mult_1()",0,0, NULL);
  fsym_init(code_229,1,"()1",0,0, NULL);
  fsym_init(code_228,1,"mult_0()",0,0, NULL);
  fsym_init(code_227,1,"()0",0,0, NULL);
  fsym_init(code_226,0,"d",0,0, NULL);
  fsym_init(code_225,0,"listExtract",0,0, NULL);
  fsym_init(code_224,2,"ccat(,)",0,0, NULL);
  fsym_init(code_223,1,"size_of_builtinInt_list()",0,0, NULL);
  fsym_init(code_222,2,"-thelem()",0,0, NULL);
  fsym_init(code_221,1,"elem()",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_220,2,"@",0,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_79,1,"-",0,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_219,2,".",0,0, NULL);
  fsym_init(code_218,0,"nil",0,0, NULL);
  fsym_init(code_217,1,"valueOf()",0,0, NULL);
  fsym_init(code_216,1,"itob_int()",0,0, NULL);
  fsym_init(code_215,1,"btoi_int()",0,0, NULL);
  fsym_init(code_214,2,"less_int(,)",0,0, NULL);
  fsym_init(code_213,2,"lesseq_int(,)",0,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_226, code_226);
  TERM_CONST_ALLOC(con_225, code_225);
  TERM_CONST_ALLOC(con_218, code_218);
  /* Initialisation des pattern_list */
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
destruction:
  FREE(fsymtab[code_212].name);
  FREE(fsymtab[code_211].name);
  FREE(fsymtab[code_210].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_67].name);
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
  FREE(fsymtab[code_254].name);
  FREE(fsymtab[code_253].name);
  FREE(fsymtab[code_252].name);
  FREE(fsymtab[code_251].name);
  FREE(fsymtab[code_250].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_249].name);
  FREE(fsymtab[code_248].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_247].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_246].name);
  FREE(fsymtab[code_245].name);
  FREE(fsymtab[code_244].name);
  FREE(fsymtab[code_243].name);
  FREE(fsymtab[code_242].name);
  FREE(fsymtab[code_241].name);
  FREE(fsymtab[code_240].name);
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
  FREE(fsymtab[code_229].name);
  FREE(fsymtab[code_228].name);
  FREE(fsymtab[code_227].name);
  FREE(fsymtab[code_226].name);
  FREE(fsymtab[code_225].name);
  FREE(fsymtab[code_224].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_79].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_20].name);
  FREE(fsymtab[code_219].name);
  FREE(fsymtab[code_218].name);
  FREE(fsymtab[code_217].name);
  FREE(fsymtab[code_216].name);
  FREE(fsymtab[code_215].name);
  FREE(fsymtab[code_214].name);
  FREE(fsymtab[code_213].name);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_226);
  TERM_FREE(con_225);
  TERM_FREE(con_218);
  /* Destruction des pattern_list */
#ifdef DEBUG
  backStatistics();
#endif
  if(!REFMode) {
    printf("\nrewrite_step  = %u\n",rewrite_step);
    diff_sec  = after_self.ru_utime.tv_sec  - before_self.ru_utime.tv_sec;
    diff_usec = after_self.ru_utime.tv_usec - before_self.ru_utime.tv_usec;
    total_time= (double)diff_sec + (((double)diff_usec)/1000000.0);
    printf("total time    = %.3f sec\n",total_time);
    if(diff_sec > 0) {
      printf("average speed = %d rwr/sec\n",(long)(((double)rewrite_step)/total_time));
    } else if(diff_usec > 0) {
      printf("average speed = %d rwr/sec\n",(long)(((double)1000000*rewrite_step)/((double)diff_usec)));
    }
  }
  exit(0);
}
#include "ac_tools.c"
