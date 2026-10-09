#include "set.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;

/* Table des symboles */
int fsymtabSize = 244;
fsym fsymtab[244];
/* Declaration des pattern_list */
void init_pattern_list_239_231();
void delete_pattern_list_239_231();
void init_pattern_list_234_231();
void delete_pattern_list_234_231();
void init_pattern_list_243_231();
void delete_pattern_list_243_231();
void init_pattern_list_221_219();
void delete_pattern_list_221_219();
void init_pattern_list_242_219();
void delete_pattern_list_242_219();
void init_pattern_list_226_219();
void delete_pattern_list_226_219();
void init_pattern_list_222_219();
void delete_pattern_list_222_219();
void init_pattern_list_238_231();
void delete_pattern_list_238_231();
void init_pattern_list_233_231();
void delete_pattern_list_233_231();
void init_pattern_list_227_219();
void delete_pattern_list_227_219();

/* Constantes */
struct term *con_223;
struct term *con_235;
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
NULL, (funTabType) &fun_219, NULL, (funTabType) &fun_221, 
(funTabType) &fun_222, NULL, NULL, (funTabType) &fun_225, 
(funTabType) &fun_226, (funTabType) &fun_227, (funTabType) &fun_228, 
(funTabType) &fun_229, NULL, (funTabType) &fun_231, NULL, 
(funTabType) &fun_233, (funTabType) &fun_234, NULL, NULL, 
(funTabType) &fun_237, (funTabType) &fun_238, (funTabType) &fun_239, 
(funTabType) &fun_240, (funTabType) &fun_241, (funTabType) &fun_242, 
(funTabType) &fun_243, NULL};

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
    /* card()(P()(mkSet()(,(([](1)),([](2)),([](3)),([](4)),([](5)),([](6)),([](7)),([](8)),([](9)))))) */
    struct term *tmp, *sv[10];
    TERM_ALLOC(sv[1],term1,code_200);
    sv[1]->sub[0] = (setIntegerTag(1));
    TERM_ALLOC(sv[0],term1,code_218);
    sv[0]->sub[0] = sv[1];
    TERM_ALLOC(sv[2],term1,code_200);
    sv[2]->sub[0] = (setIntegerTag(2));
    TERM_ALLOC(sv[1],term1,code_218);
    sv[1]->sub[0] = sv[2];
    TERM_ALLOC(sv[3],term1,code_200);
    sv[3]->sub[0] = (setIntegerTag(3));
    TERM_ALLOC(sv[2],term1,code_218);
    sv[2]->sub[0] = sv[3];
    TERM_ALLOC(sv[4],term1,code_200);
    sv[4]->sub[0] = (setIntegerTag(4));
    TERM_ALLOC(sv[3],term1,code_218);
    sv[3]->sub[0] = sv[4];
    TERM_ALLOC(sv[5],term1,code_200);
    sv[5]->sub[0] = (setIntegerTag(5));
    TERM_ALLOC(sv[4],term1,code_218);
    sv[4]->sub[0] = sv[5];
    TERM_ALLOC(sv[6],term1,code_200);
    sv[6]->sub[0] = (setIntegerTag(6));
    TERM_ALLOC(sv[5],term1,code_218);
    sv[5]->sub[0] = sv[6];
    TERM_ALLOC(sv[7],term1,code_200);
    sv[7]->sub[0] = (setIntegerTag(7));
    TERM_ALLOC(sv[6],term1,code_218);
    sv[6]->sub[0] = sv[7];
    TERM_ALLOC(sv[8],term1,code_200);
    sv[8]->sub[0] = (setIntegerTag(8));
    TERM_ALLOC(sv[7],term1,code_218);
    sv[7]->sub[0] = sv[8];
    TERM_ALLOC(sv[9],term1,code_200);
    sv[9]->sub[0] = (setIntegerTag(9));
    TERM_ALLOC(sv[8],term1,code_218);
    sv[8]->sub[0] = sv[9];
    TERM_ALLOC(sv[9],term2,code_219);
    term_add_onf_term_color(sv[9],sv[0],1);
    term_add_onf_term_color(sv[9],sv[1],2);
    term_add_onf_term_color(sv[9],sv[2],3);
    term_add_onf_term_color(sv[9],sv[3],4);
    term_add_onf_term_color(sv[9],sv[4],5);
    term_add_onf_term_color(sv[9],sv[5],6);
    term_add_onf_term_color(sv[9],sv[6],7);
    term_add_onf_term_color(sv[9],sv[7],8);
    term_add_onf_term_color(sv[9],sv[8],9);
    sv[9] = fun_219( sv[9] );
    TERM_ALLOC(sv[0],term1,code_224);
    sv[0]->sub[0] = sv[9];
    sv[1] = fun_242( sv[0] );
    sv[0] = fun_241( sv[1] );
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
  fsym_init(code_229,1,"card()",0,0, NULL);
  fsym_init(code_228,2,"in",0,0, NULL);
  fsym_init(code_227,2,"(\\)",0,0, NULL);
  fsym_init(code_226,2,"(I)",0,0, NULL);
  fsym_init(code_225,2,"(U)",0,0, NULL);
  fsym_init(code_224,1,"mkSet()",0,0, NULL);
  fsym_init(code_223,0,"emptyset",0,0, NULL);
  fsym_init(code_222,1,"size()",0,0, NULL);
  fsym_init(code_221,2,"in",0,0, NULL);
  fsym_init(code_220,1,"elem()",0,0, NULL);
  fsym_init(code_219,-1,",",0,0, NULL);
  fsym_init(code_218,1,"",0,0, NULL);
  fsym_init(code_217,1,"valueOf()",0,0, NULL);
  fsym_init(code_216,1,"itob_int()",0,0, NULL);
  fsym_init(code_215,1,"btoi_int()",0,0, NULL);
  fsym_init(code_214,2,"less_int(,)",0,0, NULL);
  fsym_init(code_213,2,"lesseq_int(,)",0,0, NULL);
  fsym_init(code_212,2,"greatereq_int(,)",0,0, NULL);
  fsym_init(code_211,2,"greater_int(,)",0,0, NULL);
  fsym_init(code_210,2,"neq_int(,)",0,0, NULL);
  fsym_init(code_209,2,"eq_int(,)",0,0, NULL);
  fsym_init(code_208,1,"umin()",0,0, NULL);
  fsym_init(code_207,2,"or(,)",0,0, NULL);
  fsym_init(code_33,2,"greatereq_set[int](,)",33,0, NULL);
  fsym_init(code_206,2,"div(,)",0,0, NULL);
  fsym_init(code_32,2,"greater_set[int](,)",32,0, NULL);
  fsym_init(code_205,2,"and(,)",0,0, NULL);
  fsym_init(code_31,2,"lesseq_set[int](,)",31,0, NULL);
  fsym_init(code_204,2,"mod(,)",0,0, NULL);
  fsym_init(code_30,2,"less_set[int](,)",30,0, NULL);
  fsym_init(code_203,2,"time(,)",0,0, NULL);
  fsym_init(code_202,2,"minus(,)",0,0, NULL);
  fsym_init(code_201,2,"plus(,)",0,0, NULL);
  fsym_init(code_200,1,"[]",0,0, NULL);
  fsym_init(code_79,1,"-",0,0, NULL);
  fsym_init(code_243,2,"augment(,)",0,0, NULL);
  fsym_init(code_242,1,"P()",0,0, NULL);
  fsym_init(code_241,1,"card()",0,0, NULL);
  fsym_init(code_240,2,"in",0,0, NULL);
  fsym_init(code_29,2,"or(,)",29,0, NULL);
  fsym_init(code_28,2,"and(,)",28,0, NULL);
  fsym_init(code_27,2,"mod(,)",27,0, NULL);
  fsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  fsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  fsym_init(code_67,1,"",0,0, NULL);
  fsym_init(code_239,2,"(\\)",0,0, NULL);
  fsym_init(code_238,2,"(I)",0,0, NULL);
  fsym_init(code_237,2,"(U)",0,0, NULL);
  fsym_init(code_236,1,"mkSet()",0,0, NULL);
  fsym_init(code_235,0,"emptyset",0,0, NULL);
  fsym_init(code_234,1,"size()",0,0, NULL);
  fsym_init(code_233,2,"in",0,0, NULL);
  fsym_init(code_232,1,"elem()",0,0, NULL);
  fsym_init(code_231,-1,",",0,0, NULL);
  fsym_init(code_230,1,"",0,0, NULL);
  fsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  fsym_init(code_6,2,"div(,)",6,0, NULL);
  fsym_init(code_5,2,"time(,)",5,0, NULL);
  fsym_init(code_4,2,"minus(,)",4,0, NULL);
  fsym_init(code_3,2,"plus(,)",3,0, NULL);
  fsym_init(code_19,2,"neq_set[int](,)",19,0, NULL);
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_18,2,"eq_set[int](,)",18,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  fsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_223, code_223);
  TERM_CONST_ALLOC(con_235, code_235);
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  /* Initialisation des pattern_list */
init_pattern_list_239_231();
init_pattern_list_234_231();
init_pattern_list_243_231();
init_pattern_list_221_219();
init_pattern_list_242_219();
init_pattern_list_226_219();
init_pattern_list_222_219();
init_pattern_list_238_231();
init_pattern_list_233_231();
init_pattern_list_227_219();
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
  FREE(fsymtab[code_243].name);
  FREE(fsymtab[code_242].name);
  FREE(fsymtab[code_241].name);
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
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  TERM_FREE(con_223);
  TERM_FREE(con_235);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  /* Destruction des pattern_list */
delete_pattern_list_239_231();
delete_pattern_list_234_231();
delete_pattern_list_243_231();
delete_pattern_list_221_219();
delete_pattern_list_242_219();
delete_pattern_list_226_219();
delete_pattern_list_222_219();
delete_pattern_list_238_231();
delete_pattern_list_233_231();
delete_pattern_list_227_219();
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
