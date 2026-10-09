#include "sdart_bi.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 210;
fsym fsymtab[210];
/* Declaration des pattern_list */
void init_pattern_list_202();
void delete_pattern_list_202();
void init_pattern_list_203();
void delete_pattern_list_203();
void init_pattern_list_204();
void delete_pattern_list_204();

/* Constantes */
struct term *con_1;
struct term *con_200;
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
(funTabType) &fun_207, (funTabType) &fun_208, (funTabType) &fun_209, NULL};

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
    /* finish */
    struct term *tmp, *sv[1];
    sv[0] = fun_209(  );
    res=sv[0];
  }
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
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_209,0,"finish",0,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_208,0,"all",0,0, NULL);
  fsym_init(code_207,0,"triples",0,0, NULL);
  fsym_init(code_206,0,"doubles",0,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_205,0,"singles",0,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_204,-1,"m2(,)",0,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_203,-1,"p2(,)",0,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_202,-1,"+",0,0, NULL);
  fsym_init(code_201,1,"set()",0,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_200,0,"empty",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_200, code_200);
  TERM_CONST_ALLOC(con_0, code_0);
  /* Initialisation des pattern_list */
init_pattern_list_202();
init_pattern_list_203();
init_pattern_list_204();
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
  FREE(fsymtab[code_29].name);
  FREE(fsymtab[code_28].name);
  FREE(fsymtab[code_27].name);
  FREE(fsymtab[code_26].name);
  FREE(fsymtab[code_25].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_20].name);
  TERM_FREE(con_1);
  TERM_FREE(con_200);
  TERM_FREE(con_0);
  /* Destruction des pattern_list */
delete_pattern_list_202();
delete_pattern_list_203();
delete_pattern_list_204();
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
