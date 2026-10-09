#include "enum1.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
unsigned long tab_rewrite_step[2][223];
int trace=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 224;
fsym fsymtab[224];
/* Declaration des pattern_list */

/* Constantes */
struct term *con_1;
struct term *con_0;
struct term *con_223;
struct term *con_222;
struct term *con_221;

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
NULL, NULL, NULL, NULL, NULL, NULL, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_50, NULL, NULL, 
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
NULL, NULL, (funTabType) &str_385, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, (funTabType) &str_395, NULL};

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
  {
    /* enum(,)([](1),[](3)) */
    struct term *tmp, *sv[3];
    TERM_ALLOC(sv[1],term1,code_200);
    sv[1]->sub[0] = (setIntegerTag(1));
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = (setIntegerTag(3));
    TERM_ALLOC(sv[0],term2,code_220);
    sv[0]->sub[0] = sv[1];
    sv[0]->sub[1] = sv[2];
    res=str_385(sv[0]);
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
    if(!strcmp(argv[i],"-trace")) {
      trace=2;
    }
  }
  if(queryMode) {
    yyparse();
  }
  init_alloc();
  for(i=0 ; i<fsymtabSize ; i++) {
    fsym_init(i,0,"nullString",0,0,NULL);
  }
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_209,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_208,1,"umin()",0,0, NULL);
  fsym_init(code_207,2,"or(,)",0,0, NULL);
  fsym_init(code_206,2,"div(,)",0,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_205,2,"and(,)",0,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_204,2,"mod(,)",0,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_203,2,"time(,)",0,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_202,2,"minus(,)",0,0, NULL);
  fsym_init(code_201,2,"plus(,)",0,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_200,1,"[]",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_223,0,"nextStrat",0,0, NULL);
  fsym_init(code_222,0,"fStrat",0,0, NULL);
  fsym_init(code_221,0,"enumStrat",0,0, NULL);
  fsym_init(code_220,2,"enum(,)",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_79,1,"-",0,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_219,1,"next()",0,0, NULL);
  fsym_init(code_218,1,"f()",0,0, NULL);
  fsym_init(code_217,1,"valueOf()",0,0, NULL);
  fsym_init(code_216,1,"itob_int()",0,0, NULL);
  fsym_init(code_215,1,"btoi_int()",0,0, NULL);
  fsym_init(code_214,2,"less_int(,)",0,0, NULL);
  fsym_init(code_213,2,"lesseq_int(,)",0,0, NULL);
  fsym_init(code_212,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_211,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_210,2,"neq_int(,)",0,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_223, code_223);
  TERM_CONST_ALLOC(con_222, code_222);
  TERM_CONST_ALLOC(con_221, code_221);
  EkerTermInit();

  backTrackInit();
  /* Initialisation des pattern_list */
  for(i=0 ; i<2 ; i++) {
    for(j=0 ; j<223; j++) {
      tab_rewrite_step[i][j] = 0;
    }
  }

  if (!setChoicePoint()) {
    if(queryMode) {
      res = normalise(query);
      res=str_385(res);
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
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_67].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_6].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_5].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_4].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_3].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_79].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_20].name);
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
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_223);
  TERM_FREE(con_222);
  TERM_FREE(con_221);
  /* Destruction des pattern_list */
#ifdef DEBUG
  backStatistics();
#endif
  if(!REFMode) {
    printf("\nrewrite_step = %u\n",rewrite_step);
    printf("                      	fails	success\n");
    for(j=0 ; j<223; j++) {
      if(tab_rewrite_step[0][j] > 0 || tab_rewrite_step[1][j] > 0)
        printf("tab_rewrite_step[%d] :	%u	%u\n",j,tab_rewrite_step[0][j],tab_rewrite_step[1][j]);
    }
  }
  exit(0);
}
#include "ac_tools.c"
