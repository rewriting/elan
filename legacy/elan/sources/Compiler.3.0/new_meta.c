#include "new_meta.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;
int quietMode=0;
char *sortName=NULL;
char *strategyName=NULL;

/* Table des symboles */
int fsymtabSize = 248;
fsym fsymtab[248];
/* Declaration des pattern_list */

/* Constantes */
struct term *con_1;
struct term *con_0;
struct term *con_247;
struct term *con_246;
struct term *con_245;
struct term *con_242;
struct term *con_241;
struct term *con_240;
struct term *con_239;
struct term *con_238;
struct term *con_237;
struct term *con_225;
struct term *con_218;

/* Redirection de built-ins */
struct term *fun_236(struct term *v0, struct term *v1, struct term *v2, struct term *v3)
 { return fun_128(236, v0, v1, v2, v3); }

struct term *fun_231(struct term *v0, struct term *v1, struct term *v2)
 { return fun_130(231, v0, v1, v2); }

struct term *fun_230(struct term *v0, struct term *v1)
 { return fun_129(230, v0, v1); }


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
(funTabType) &fun_223, (funTabType) &fun_224, NULL, 
(funTabType) &fun_226, (funTabType) &fun_227, (funTabType) &fun_228, 
(funTabType) &fun_229, NULL, NULL, (funTabType) &fun_232, 
(funTabType) &fun_233, (funTabType) &fun_234, (funTabType) &fun_235, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, (funTabType) &str_104, NULL, NULL, 
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
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &str_303, (funTabType) &str_304, (funTabType) &str_305, 
(funTabType) &str_306, (funTabType) &str_307, NULL, NULL, NULL, NULL, 
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
NULL, NULL, NULL, NULL, NULL, (funTabType) &str_449, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, (funTabType) &str_464, (funTabType) &str_465, 
(funTabType) &str_466, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_491, 
(funTabType) &str_492, NULL};

/* Initialisation des patterns AC */
TERM *EkerTerm[100];
static void EkerTermInit() {
  TERM_LIST *vlist[100];
  AC_LIST *acvlist[100];
  struct term *sv[100];
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
  int queryMode=2;  /* 0:noInput  1:REFInput  2:Elan form 3:command line */
  int ResultMode=2; /* 0:noOutput 1:REFOutput 2:Elan form 3:internalOutput */
  int evaluationMode=0; /* 0:lgi 1:sort 2:strat:sort*/
#ifdef CSETCHP
  choice_init(&bp);
#else
  bp_main=&bp;
#endif
  GC_free_space_divisor=2;
  for(i=1 ; i<argc && *argv[i]=='-' ; i++) {
    if(!strcmp(argv[i],"-noInput")) {
      queryMode=0;
    }
    if(!strcmp(argv[i],"-REFInput")) {
      queryMode=1;
    }
    if(!strcmp(argv[i],"-commandLine")) {
      queryMode=3;
    }
    if(!strcmp(argv[i],"-noOutput")) {
      ResultMode=0;
    }
    if(!strcmp(argv[i],"-REFOutput")) {
      ResultMode=1;
    }
    if(!strcmp(argv[i],"-internalOutput")) {
      ResultMode=3;
    }
    if(!strcmp(argv[i],"-debug")) {
      debugMode=1;
    }
    if(!strcmp(argv[i],"-quiet")) {
      quietMode=1;
    }
    if(!strcmp(argv[i],"-sort")) {
      evaluationMode=1;
      sortName=argv[++i];
    }
    if(!strcmp(argv[i],"-strategy")) {
      evaluationMode=2;
      strategyName=argv[++i];
    }
  }
  init_alloc();
  for(i=0 ; i<fsymtabSize ; i++) {
    fsym_init(i,0,"nullString",0,0,NULL);
  }
  fsym_init(code_212,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_211,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_210,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_15,2,"neq_ident(,)",15,0, NULL);
  fsym_init(code_14,2,"eq_ident(,)",14,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_68,1,"",0,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_158,1,"internstring()",158,0, NULL);
  fsym_init(code_157,2,"strcmp(,)",157,0, NULL);
  fsym_init(code_156,2,"strspn(,)",156,0, NULL);
  fsym_init(code_154,3,"internsubstr(,,)",154,0, NULL);
  fsym_init(code_153,3,"intern[<-]",153,0, NULL);
  fsym_init(code_152,2,"intern[]",152,0, NULL);
  fsym_init(code_151,2,"+",151,0, NULL);
  fsym_init(code_209,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_150,1,"strlen()",150,0, NULL);
  fsym_init(code_208,1,"umin()",0,0, NULL);
  fsym_init(code_207,2,"or(,)",0,0, NULL);
  fsym_init(code_206,2,"div(,)",0,0, NULL);
  fsym_init(code_205,2,"and(,)",0,0, NULL);
  fsym_init(code_204,2,"mod(,)",0,0, NULL);
  fsym_init(code_203,2,"time(,)",0,0, NULL);
  fsym_init(code_202,2,"minus(,)",0,0, NULL);
  fsym_init(code_201,2,"plus(,)",0,0, NULL);
  fsym_init(code_200,1,"[]",0,0, NULL);
  fsym_init(code_144,1,"call:Foo",144,0, NULL);
  fsym_init(code_143,0,"id",143,0, NULL);
  fsym_init(code_142,2,"",142,0, NULL);
  fsym_init(code_141,1,"",141,0, NULL);
  fsym_init(code_140,1,"iterateenditerate",140,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_247,0,"MMM",0,0, NULL);
  fsym_init(code_246,0,"DDD",0,0, NULL);
  fsym_init(code_245,0,"RRR",0,0, NULL);
  fsym_init(code_244,1,"dolist()",0,0, NULL);
  fsym_init(code_243,1,"do()",0,0, NULL);
  fsym_init(code_242,0,"f",0,0, NULL);
  fsym_init(code_241,0,"e",0,0, NULL);
  fsym_init(code_240,0,"d",0,0, NULL);
  fsym_init(code_139,1,"whileendwhile",139,0, NULL);
  fsym_init(code_138,1,"dk()",138,0, NULL);
  fsym_init(code_137,1,"dc()",137,0, NULL);
  fsym_init(code_136,1,"dk()",136,0, NULL);
  fsym_init(code_135,1,"dc()",135,0, NULL);
  fsym_init(code_134,2,"",134,0, NULL);
  fsym_init(code_133,1,"",133,0, NULL);
  fsym_init(code_132,2,",",132,0, NULL);
  fsym_init(code_131,1,"",131,0, NULL);
  fsym_init(code_130,3,"meta_apply(,,)",130,0, NULL);
  fsym_init(code_239,0,"c",0,0, NULL);
  fsym_init(code_238,0,"b",0,0, NULL);
  fsym_init(code_237,0,"a",0,0, NULL);
  fsym_init(code_236,4,"meta_apply(,,,)",-128,0, NULL);
  fsym_init(code_235,4,"meta_apply(,,,)",0,0, NULL);
  fsym_init(code_234,4,"set_of(,,,)",0,0, NULL);
  fsym_init(code_233,3,"set_of(,,)",0,0, NULL);
  fsym_init(code_232,2,"set_of(,)",0,0, NULL);
  fsym_init(code_231,3,"meta_apply(,,)",-130,0, NULL);
  fsym_init(code_230,2,"meta_apply(,)",-129,0, NULL);
  fsym_init(code_129,2,"meta_apply(,)",129,0, NULL);
  fsym_init(code_128,4,"meta_apply(,,,)",128,0, NULL);
  fsym_init(code_81,1,"",0,0, NULL);
  fsym_init(code_177,1,"ident2string()",177,0, NULL);
  fsym_init(code_229,1,"string()",0,0, NULL);
  fsym_init(code_228,3,"substr(,,)",0,0, NULL);
  fsym_init(code_227,3,"[<-]",0,0, NULL);
  fsym_init(code_226,2,"[]",0,0, NULL);
  fsym_init(code_225,0,"listExtract",0,0, NULL);
  fsym_init(code_224,2,"ccat(,)",0,0, NULL);
  fsym_init(code_223,1,"size_of_Foo_list()",0,0, NULL);
  fsym_init(code_222,2,"-thelem()",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_221,1,"elem()",0,0, NULL);
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
  TERM_CONST_ALLOC(con_247, code_247);
  TERM_CONST_ALLOC(con_246, code_246);
  TERM_CONST_ALLOC(con_245, code_245);
  TERM_CONST_ALLOC(con_242, code_242);
  TERM_CONST_ALLOC(con_241, code_241);
  TERM_CONST_ALLOC(con_240, code_240);
  TERM_CONST_ALLOC(con_239, code_239);
  TERM_CONST_ALLOC(con_238, code_238);
  TERM_CONST_ALLOC(con_237, code_237);
  TERM_CONST_ALLOC(con_225, code_225);
  TERM_CONST_ALLOC(con_218, code_218);
  /* Initialisation des pattern_list */
  if(queryMode==1) {
    yyparse();
  }
  EkerTermInit();
  initTabRef();

  getrusage(RUSAGE_SELF, &before_self);
  backTrackInit();
  if (!setChoicePoint()) {
    /* Input */
    res=termParser(queryMode,evaluationMode);
    /* Output */
    switch(ResultMode) {
      case 0:
        break;
      case 1:
        term_printREFln(stdout,res);
        break;
      case 2:
        printf("\nresult = ");
        termOut(stdout,term_unflatten(res));
        printf("\n");
        break;
      case 3:
        printf("\nresult = ");
        term_printnl(stdout,res);
        break;
    }
    if(ResultMode!=1) {
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
  FREE(fsymtab[code_15].name);
  FREE(fsymtab[code_14].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_68].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_67].name);
  FREE(fsymtab[code_158].name);
  FREE(fsymtab[code_157].name);
  FREE(fsymtab[code_156].name);
  FREE(fsymtab[code_154].name);
  FREE(fsymtab[code_153].name);
  FREE(fsymtab[code_152].name);
  FREE(fsymtab[code_151].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_150].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_144].name);
  FREE(fsymtab[code_143].name);
  FREE(fsymtab[code_142].name);
  FREE(fsymtab[code_141].name);
  FREE(fsymtab[code_140].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_247].name);
  FREE(fsymtab[code_246].name);
  FREE(fsymtab[code_245].name);
  FREE(fsymtab[code_244].name);
  FREE(fsymtab[code_243].name);
  FREE(fsymtab[code_242].name);
  FREE(fsymtab[code_241].name);
  FREE(fsymtab[code_240].name);
  FREE(fsymtab[code_139].name);
  FREE(fsymtab[code_138].name);
  FREE(fsymtab[code_137].name);
  FREE(fsymtab[code_136].name);
  FREE(fsymtab[code_135].name);
  FREE(fsymtab[code_134].name);
  FREE(fsymtab[code_133].name);
  FREE(fsymtab[code_132].name);
  FREE(fsymtab[code_131].name);
  FREE(fsymtab[code_130].name);
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
  FREE(fsymtab[code_129].name);
  FREE(fsymtab[code_128].name);
  FREE(fsymtab[code_81].name);
  FREE(fsymtab[code_177].name);
  FREE(fsymtab[code_229].name);
  FREE(fsymtab[code_228].name);
  FREE(fsymtab[code_227].name);
  FREE(fsymtab[code_226].name);
  FREE(fsymtab[code_225].name);
  FREE(fsymtab[code_224].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_221].name);
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
  TERM_FREE(con_247);
  TERM_FREE(con_246);
  TERM_FREE(con_245);
  TERM_FREE(con_242);
  TERM_FREE(con_241);
  TERM_FREE(con_240);
  TERM_FREE(con_239);
  TERM_FREE(con_238);
  TERM_FREE(con_237);
  TERM_FREE(con_225);
  TERM_FREE(con_218);
  /* Destruction des pattern_list */
#ifdef DEBUG
  backStatistics();
#endif
  if(ResultMode!=1) {
    printf("\nrewrite_step = %u\n",rewrite_step);
    diff_sec  = after_self.ru_utime.tv_sec  - before_self.ru_utime.tv_sec;
    diff_usec = after_self.ru_utime.tv_usec - before_self.ru_utime.tv_usec;
    total_time= (double)diff_sec + (((double)diff_usec)/1000000.0);
    if(!quietMode) {
      printf("total time    = %.3f sec\n",total_time);
      if(diff_sec > 0) {
        printf("average speed = %d rwr/sec\n",(long)(((double)rewrite_step)/total_time));
      } else if(diff_usec > 0) {
        printf("average speed = %d rwr/sec\n",(long)(((double)1000000*rewrite_step)/((double)diff_usec)));
      }
    }
  }
  exit(0);
}
int gram[] = {
1,32768,131,27,1,1,327,
1,32768,132,27,3,1,327,2,44,1,27,
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
1,32768,27,58,6,0,320,2,40,1,58,2,44,1,58,2,41,
1,32768,28,58,6,0,518,2,40,1,58,2,44,1,58,2,41,
1,32768,29,58,6,0,225,2,40,1,58,2,44,1,58,2,41,
1,0,20,58,4,0,441,2,40,1,58,2,41,
1,32768,20,58,4,0,1594,2,40,1,58,2,41,
1,900,25,58,4,0,430,2,40,1,428,2,41,
1,33668,25,58,4,0,1583,2,40,1,428,2,41,
1,33668,217,58,4,0,722,2,40,1,331,2,41,
1,34768,150,58,4,0,664,2,40,1,163,2,41,
1,2000,152,58,4,1,163,2,91,1,58,2,93,
1,34768,152,58,5,0,656,1,163,2,91,1,58,2,93,
1,34768,156,58,6,0,682,2,40,1,163,2,44,1,163,2,41,
1,34768,157,58,6,0,665,2,40,1,163,2,44,1,163,2,41,
1,49152,247,69,1,0,442,
1,32768,133,71,1,1,163,
1,32768,134,71,2,1,163,1,71,
1,10192,151,163,6,0,868,2,40,1,163,2,44,1,163,2,41,
1,42960,151,163,3,1,163,2,43,1,163,
1,2000,153,163,7,1,163,2,91,1,58,2,60,2,45,1,58,2,93,
1,34768,153,163,8,0,656,1,163,2,91,1,58,2,60,2,45,1,58,2,93,
1,2000,154,163,8,0,675,2,40,1,163,2,44,1,58,2,44,1,58,2,41,
1,34768,154,163,9,0,656,0,675,2,40,1,163,2,44,1,58,2,44,1,58,2,41,
1,2000,158,163,4,0,663,2,40,1,58,2,41,
1,34768,158,163,5,0,656,0,663,2,40,1,58,2,41,
1,32768,177,163,4,0,1245,2,40,1,32,2,41,
1,33268,227,163,7,1,163,2,91,1,331,2,60,2,45,1,331,2,93,
1,33268,228,163,8,0,675,2,40,1,163,2,44,1,331,2,44,1,331,2,41,
1,33268,229,163,4,0,663,2,40,1,331,2,41,
1,32768,135,206,4,0,199,2,40,1,71,2,41,
1,32768,136,206,4,0,207,2,40,1,71,2,41,
1,32768,137,206,4,0,199,2,40,1,27,2,41,
1,32768,138,206,4,0,207,2,40,1,27,2,41,
1,0,139,206,3,0,641,1,327,0,952,
1,32768,139,206,3,0,537,1,327,0,1059,
1,32768,140,206,3,0,750,1,327,0,1061,
1,32768,143,206,1,0,205,
1,32768,144,206,4,0,623,1,163,2,58,0,292,
1,32768,221,292,4,0,419,2,40,1,420,2,41,
1,32768,222,292,7,1,331,2,45,0,220,0,419,2,40,1,420,2,41,
1,32768,230,292,6,0,1068,2,40,1,327,2,44,1,292,2,41,
1,32768,231,292,8,0,1068,2,40,1,327,2,44,1,292,2,44,1,58,2,41,
1,32768,237,292,1,0,97,
1,32768,238,292,1,0,98,
1,32768,239,292,1,0,99,
1,32768,240,292,1,0,100,
1,32768,241,292,1,0,101,
1,32768,242,292,1,0,102,
1,32768,243,292,4,0,422,2,40,1,292,2,41,
1,49152,225,313,1,0,1175,
1,49152,245,313,1,0,246,
1,49152,246,313,1,0,204,
1,32768,141,327,1,1,206,
1,32768,142,327,2,1,206,1,327,
1,33768,200,331,1,1,58,
1,4596,201,331,3,1,331,2,43,1,331,
1,4696,202,331,3,1,331,2,45,1,331,
1,4796,203,331,3,1,331,2,42,1,331,
1,4896,204,331,3,1,331,2,37,1,331,
1,4896,205,331,3,1,331,2,38,1,331,
1,4896,206,331,3,1,331,2,47,1,331,
1,4896,207,331,3,1,331,2,124,1,331,
1,900,208,331,2,2,45,1,331,
1,500,201,331,5,2,40,1,331,2,43,1,331,2,41,
1,600,202,331,5,2,40,1,331,2,45,1,331,2,41,
1,700,203,331,5,2,40,1,331,2,42,1,331,2,41,
1,800,206,331,5,2,40,1,331,2,47,1,331,2,41,
1,800,204,331,5,2,40,1,331,2,37,1,331,2,41,
1,800,205,331,5,2,40,1,331,2,38,1,331,2,41,
1,800,207,331,5,2,40,1,331,2,124,1,331,2,41,
1,900,208,331,4,2,40,2,45,1,331,2,41,
1,33268,201,331,6,0,452,2,40,1,331,2,44,1,331,2,41,
1,33368,202,331,6,0,556,2,40,1,331,2,44,1,331,2,41,
1,33468,203,331,6,0,642,2,40,1,331,2,44,1,331,2,41,
1,33568,206,331,6,0,323,2,40,1,331,2,44,1,331,2,41,
1,33568,204,331,6,0,320,2,40,1,331,2,44,1,331,2,41,
1,33568,205,331,6,0,518,2,40,1,331,2,44,1,331,2,41,
1,33568,207,331,6,0,225,2,40,1,331,2,44,1,331,2,41,
1,33668,208,331,4,0,441,2,40,1,331,2,41,
1,900,215,331,4,0,430,2,40,1,428,2,41,
1,33668,215,331,4,0,856,2,40,1,428,2,41,
1,0,223,331,4,0,443,2,40,1,420,2,41,
1,32768,223,331,4,0,1677,2,40,1,420,2,41,
1,33268,226,331,4,1,163,2,91,1,331,2,93,
1,0,218,420,1,0,710,
1,32768,218,420,1,0,534,
1,0,219,420,6,0,435,2,40,1,292,2,44,1,420,2,41,
1,0,219,420,2,1,292,1,420,
1,0,219,420,3,1,292,2,44,1,420,
1,32768,219,420,3,1,292,2,46,1,420,
1,0,220,420,6,0,632,2,40,1,420,2,44,1,420,2,41,
1,32768,220,420,3,1,420,2,64,1,420,
1,0,224,420,6,0,833,2,40,1,331,2,42,1,420,2,41,
1,32768,224,420,6,0,833,2,40,1,331,2,44,1,420,2,41,
1,32768,232,420,6,0,1062,2,40,1,327,2,44,1,292,2,41,
1,32768,233,420,8,0,1062,2,40,1,327,2,44,1,292,2,44,1,58,2,41,
1,32768,234,420,10,0,1062,2,40,1,327,2,44,1,292,2,44,1,58,2,44,1,58,2,41,
1,32768,235,420,10,0,1068,2,40,1,327,2,44,1,292,2,44,1,58,2,44,1,58,2,41,
1,32768,244,420,4,0,1077,2,40,1,292,2,41,
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
1,300,209,428,4,1,331,2,61,2,61,1,331,
1,300,210,428,4,1,331,2,33,2,61,1,331,
1,300,211,428,3,1,331,2,62,1,331,
1,300,212,428,4,1,331,2,62,2,61,1,331,
1,300,213,428,4,1,331,2,60,2,61,1,331,
1,300,214,428,3,1,331,2,60,1,331,
1,33068,209,428,6,0,640,2,40,1,331,2,44,1,331,2,41,
1,33068,210,428,6,0,961,2,40,1,331,2,44,1,331,2,41,
1,33068,214,428,6,0,865,2,40,1,331,2,44,1,331,2,41,
1,33068,213,428,6,0,1079,2,40,1,331,2,44,1,331,2,41,
1,33068,211,428,6,0,1172,2,40,1,331,2,44,1,331,2,41,
1,33068,212,428,6,0,1386,2,40,1,331,2,44,1,331,2,41,
1,900,216,428,4,0,1063,2,40,1,331,2,41,
1,33668,216,428,4,0,1067,2,40,1,331,2,41,
1,0,14,428,4,1,32,2,61,2,61,1,32,
1,0,15,428,4,1,32,2,33,2,61,1,32,
1,32768,14,428,6,0,841,2,40,1,32,2,44,1,32,2,41,
1,32768,15,428,6,0,951,2,40,1,32,2,44,1,32,2,41,
1,1000,200,331,3,2,91,1,58,2,93,
1,32768,236,420,10,0,1068,2,40,1,327,2,44,1,420,2,44,1,58,2,44,1,58,2,41,
1,36768,68,32,1,1,220,
1,36768,67,58,1,1,19,
1,36768,79,58,2,2,45,1,19,
1,36768,81,163,1,1,351,
0};
int pos = 2434;
int earleyQuerySort = 420;
int earleyQueryStrategy = 104;
char *tabIdentStr[] = {
"int",
"greatereq_bool",
"res",
"builtin",
"th",
"part",
"dont",
"specification",
"endwhile",
"cons",
"builtinInt",
"t",
"size_of",
"s",
"iterate",
"then",
"btoi",
"n",
"Strategies",
"module",
"greater_bool",
"for",
"less_bool",
"neq_int",
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
"mod",
"where",
"btoi_int",
"while",
"neq_builtinInt",
"biy",
"itob_builtinInt",
"choose",
"nil",
"Strategy",
"END",
"inline",
"eq",
"ident",
"of",
"AND",
"greater",
"bool",
"bix",
"m",
"l",
"self",
"eq_builtinInt",
"bs",
"assocRight",
"meta",
"anyInteger",
"anyString",
"do",
"greater_builtinInt",
"Meta_strat",
"f",
"public",
"e",
"d",
"c",
"endrepeat",
"b",
"neq_ident",
"a",
"append",
"dcconcur",
"extractrule2",
"new_meta",
"end",
"extractrule1",
"neq_bool",
"dk",
"firstOne",
"eq_ident",
"id",
"local",
"DDD",
"alias",
"ee",
"eq_bool",
"elem",
"if",
"size_of_Foo_list",
"false",
"anyIdentifier",
"Meta_apply",
"Epsilon",
"fail",
"code",
"global",
"SUCH",
"X",
"Strateg",
"call",
"THAT",
"care",
"ident2string",
"dc",
"R",
"and",
"handline",
"case",
"ccat",
"size_of_Foo",
"check",
"valueOf",
"declare",
"dkcall",
"META",
"assocLeft",
"Foo",
"dccall",
"greatereq_builtinInt",
"defined",
"Meta",
"nil_Foo",
"Labels",
"EACH",
"dcOne",
"hardAlias",
"n1",
"l2",
"l1",
"strspn",
"definedAs",
"stratop",
"IF",
"strategies",
"substr",
"export",
"operators",
"ANDIF",
"result",
"try",
"query",
"strategy",
"description",
"RRR",
"sort",
"lesseq_int",
"dolist",
"plus",
"R3",
"AC",
"otherwise",
"M6",
"builtinString",
"import",
"M5",
"strcmp",
"strlen",
"string",
"M7",
"R1",
"queryend",
"start",
"minus",
"rules",
"spart",
"lesseq_bool",
"first",
"listExtract",
"strat",
"LPL",
"apply",
"FOR",
"greater_int",
"true",
"R2",
"M4",
"know",
"M3",
"private",
"M2",
"greatereq_int",
"meta_apply",
"M1",
"with",
"itob_int",
"size",
"MMM",
"umin",
"normalize",
"z",
"itob",
"y",
"switch",
"set_of",
"x",
"source",
"enditerate",
"intern",
"not",
"list",
"oneconcur",
"lesseq",
"umin_builtinInt",
"normalise",
"less_builtinInt",
"set",
"less",
"strcat",
"pri",
"new",
"explicit",
"less_int",
"lesseq_builtinInt",
"or",
""};
int tabIdentIndex[] = {
542,
1483,
541,
759,
220,
439,
437,
1377,
1059,
435,
1058,
116,
751,
115,
750,
431,
430,
110,
1051,
646,
1269,
327,
962,
961,
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
856,
537,
1477,
535,
1794,
852,
534,
851,
215,
850,
214,
532,
213,
211,
746,
428,
745,
109,
108,
426,
1367,
424,
1047,
423,
1046,
959,
422,
1899,
1044,
102,
639,
101,
100,
99,
952,
98,
951,
97,
632,
849,
1253,
848,
311,
1252,
847,
207,
842,
841,
205,
523,
204,
522,
202,
737,
419,
418,
1677,
734,
1355,
1036,
730,
412,
411,
625,
307,
88,
941,
623,
305,
622,
1245,
199,
82,
518,
835,
834,
833,
1138,
510,
722,
720,
619,
295,
932,
292,
611,
2113,
719,
391,
710,
595,
273,
489,
905,
159,
158,
157,
682,
899,
781,
143,
1083,
675,
674,
991,
354,
671,
351,
566,
883,
1188,
246,
456,
1079,
1077,
452,
133,
132,
986,
131,
1390,
667,
130,
665,
664,
663,
343,
342,
877,
558,
556,
555,
554,
1176,
552,
1175,
769,
232,
550,
231,
1172,
448,
765,
129,
447,
128,
763,
127,
1386,
1068,
126,
444,
1067,
443,
442,
441,
977,
122,
1063,
121,
658,
1062,
120,
657,
1061,
656,
337,
655,
972,
653,
1594,
970,
1592,
332,
650,
868,
331,
330,
866,
865,
1806,
225,
0};
int tabIdentSize = 218;
char *tabSortStr[] = {
"Strateg[Foo]",
"Labels[Foo]",
"ident",
"Foo",
"intern string",
"<Foo->Foo>",
"bool",
"builtinInt",
"Strategy[Foo]",
"string",
"intern int",
"list[Foo]",
"<list[Foo]->list[Foo]>",
"intern ident",
"Strategies[Foo]",
"int",
""};
int tabSortIndex[] = {
206,
71,
32,
292,
351,
313,
428,
58,
327,
163,
19,
420,
69,
220,
27,
331,
0};
int tabSortSize = 16;
char *tabStrategyStr[] = {
"WHERE7:list[Foo]/new_meta",
"WHERE6:list[Foo]/new_meta",
"WHERE5:list[Foo]/new_meta",
"WHERE4:list[Foo]/new_meta",
"WHERE3:list[Foo]/new_meta",
"listExtract:Foo/list[Foo]",
"MMM:list[Foo]/new_meta",
"RRR:Foo/new_meta",
"WHERE2:list[Foo]/Meta_apply[Foo]",
"WHERE1:list[Foo]/Meta_apply[Foo]",
"WHERE0:list[Foo]/Meta_apply[Foo]",
"DDD:Foo/new_meta",
""};
int tabStrategyIndex[] = {
307,
306,
305,
304,
303,
492,
104,
491,
466,
465,
464,
449,
0};
int tabStrategySize = 12;
