#include "manySortedSet.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 224;
fsym fsymtab[224];
/* Declaration des pattern_list */
void init_pattern_list_206();
void delete_pattern_list_206();
void init_pattern_list_209_203();
void delete_pattern_list_209_203();
void init_pattern_list_207_203();
void delete_pattern_list_207_203();
void init_pattern_list_210_203();
void delete_pattern_list_210_203();
void init_pattern_list_208_203();
void delete_pattern_list_208_203();
void init_pattern_list_205();
void delete_pattern_list_205();

/* Constantes */
struct term *con_202;
struct term *con_1;
struct term *con_0;
struct term *con_215;
struct term *con_214;
struct term *con_213;
struct term *con_212;
struct term *con_211;

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
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_204, 
(funTabType) &fun_205, (funTabType) &fun_206, (funTabType) &fun_207, 
(funTabType) &fun_208, (funTabType) &fun_209, (funTabType) &fun_210, 
NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_216, 
(funTabType) &fun_217, (funTabType) &fun_218, (funTabType) &fun_219, 
(funTabType) &fun_220, (funTabType) &fun_221, (funTabType) &fun_222, 
(funTabType) &fun_223, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL};

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
    /* q2 */
    struct term *tmp, *sv[1];
    sv[0] = fun_222(  );
    res=sv[0];
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
  fsym_init(code_13,2,"greatereq_bool(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_bool(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_bool(,)",11,0, NULL);
  fsym_init(code_10,2,"less_bool(,)",10,0, NULL);
  fsym_init(code_9,2,"neq_bool(,)",9,0, NULL);
  fsym_init(code_209,1,"P()",0,0, NULL);
  fsym_init(code_8,2,"eq_bool(,)",8,0, NULL);
  fsym_init(code_208,2,"contains(,)",0,0, NULL);
  fsym_init(code_207,2,"-(,)",0,0, NULL);
  fsym_init(code_206,-1,"^(,)",0,0, NULL);
  fsym_init(code_205,-1,"U(,)",0,0, NULL);
  fsym_init(code_204,1,"s()",0,0, NULL);
  fsym_init(code_203,-1,"l(,)",0,0, NULL);
  fsym_init(code_202,0,"empty",0,0, NULL);
  fsym_init(code_201,1,"j()",0,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_33,2,"greatereq_Set(,)",33,0, NULL);
  fsym_init(code_200,1,"i()",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_32,2,"greater_Set(,)",32,0, NULL);
  fsym_init(code_31,2,"lesseq_Set(,)",31,0, NULL);
  fsym_init(code_30,2,"less_Set(,)",30,0, NULL);
  fsym_init(code_223,0,"q3",0,0, NULL);
  fsym_init(code_222,0,"q2",0,0, NULL);
  fsym_init(code_221,0,"q1",0,0, NULL);
  fsym_init(code_220,0,"z",0,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_219,0,"x",0,0, NULL);
  fsym_init(code_218,0,"w",0,0, NULL);
  fsym_init(code_217,0,"v",0,0, NULL);
  fsym_init(code_216,0,"u",0,0, NULL);
  fsym_init(code_215,0,"e",0,0, NULL);
  fsym_init(code_214,0,"d",0,0, NULL);
  fsym_init(code_213,0,"c",0,0, NULL);
  fsym_init(code_212,0,"b",0,0, NULL);
  fsym_init(code_211,0,"a",0,0, NULL);
  fsym_init(code_210,2,"augment(,)",0,0, NULL);
  fsym_init(code_19,2,"neq_Set(,)",19,0, NULL);
  fsym_init(code_18,2,"eq_Set(,)",18,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_202, code_202);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_215, code_215);
  TERM_CONST_ALLOC(con_214, code_214);
  TERM_CONST_ALLOC(con_213, code_213);
  TERM_CONST_ALLOC(con_212, code_212);
  TERM_CONST_ALLOC(con_211, code_211);
  /* Initialisation des pattern_list */
init_pattern_list_206();
init_pattern_list_209_203();
init_pattern_list_207_203();
init_pattern_list_210_203();
init_pattern_list_208_203();
init_pattern_list_205();
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
  diff_time_self = ((after_self.ru_utime.tv_sec*1000) + (after_self.ru_utime.tv_usec/1000)) - ((before_self.ru_utime.tv_sec*1000) + (before_self.ru_utime.tv_usec/1000));
destruction:
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_209].name);
  FREE(fsymtab[code_8].name);
  FREE(fsymtab[code_208].name);
  FREE(fsymtab[code_207].name);
  FREE(fsymtab[code_206].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_223].name);
  FREE(fsymtab[code_222].name);
  FREE(fsymtab[code_221].name);
  FREE(fsymtab[code_220].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
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
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_18].name);
  TERM_FREE(con_202);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_215);
  TERM_FREE(con_214);
  TERM_FREE(con_213);
  TERM_FREE(con_212);
  TERM_FREE(con_211);
  /* Destruction des pattern_list */
delete_pattern_list_206();
delete_pattern_list_209_203();
delete_pattern_list_207_203();
delete_pattern_list_210_203();
delete_pattern_list_208_203();
delete_pattern_list_205();
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
