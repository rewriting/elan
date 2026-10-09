#include "cs3.h"
extern struct term *query;

/* Constantes d'execution */
unsigned long rewrite_step=0;
int global_indentlevel=0;
int debugMode=0;
int quietMode=0;
char *sortName=NULL;
char *strategyName=NULL;

/* Table des symboles */
int fsymtabSize = 206;
fsym fsymtab[206];
/* Declaration des pattern_list */

/* Constantes */
struct term *con_1;
struct term *con_0;
struct term *con_205;
struct term *con_204;
struct term *con_202;

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
NULL, NULL, NULL, NULL, NULL, (funTabType) &fun_203, NULL, NULL, NULL};

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
  fsym_init(code_1,0,"true",0,0, NULL);
  fsym_init(code_0,0,"false",0,0, NULL);
  fsym_init(code_19,2,"neq_identifier(,)",19,0, NULL);
  fsym_init(code_18,2,"eq_identifier(,)",18,0, NULL);
  fsym_init(code_15,2,"neq_ident(,)",15,0, NULL);
  fsym_init(code_14,2,"eq_ident(,)",14,0, NULL);
  fsym_init(code_68,1,"",0,0, NULL);
  fsym_init(code_13,2,"greatereq_bool(,)",13,0, NULL);
  fsym_init(code_12,2,"greater_bool(,)",12,0, NULL);
  fsym_init(code_11,2,"lesseq_bool(,)",11,0, NULL);
  fsym_init(code_10,2,"less_bool(,)",10,0, NULL);
  fsym_init(code_24,1,"(not())",24,0, NULL);
  fsym_init(code_22,2,"(or)",22,0, NULL);
  fsym_init(code_21,2,"(and)",21,0, NULL);
  fsym_init(code_205,0,"specialLabel",0,0, NULL);
  fsym_init(code_33,2,"greatereq_identifier(,)",33,0, NULL);
  fsym_init(code_204,0,"normalId",0,0, NULL);
  fsym_init(code_32,2,"greater_identifier(,)",32,0, NULL);
  fsym_init(code_203,1,"whatAmI",0,0, NULL);
  fsym_init(code_31,2,"lesseq_identifier(,)",31,0, NULL);
  fsym_init(code_202,0,"theSpecialLabel",0,0, NULL);
  fsym_init(code_30,2,"less_identifier(,)",30,0, NULL);
  fsym_init(code_201,1,"",0,0, NULL);
  fsym_init(code_200,1,"",0,0, NULL);
  fsym_init(code_9,2,"neq_bool(,)",9,0, NULL);
  fsym_init(code_8,2,"eq_bool(,)",8,0, NULL);
  
/* Initialisation des constantes */
  TERM_CONST_ALLOC(con_1, code_1);
  TERM_CONST_ALLOC(con_0, code_0);
  TERM_CONST_ALLOC(con_205, code_205);
  TERM_CONST_ALLOC(con_204, code_204);
  TERM_CONST_ALLOC(con_202, code_202);
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
  FREE(fsymtab[code_1].name);
  FREE(fsymtab[code_0].name);
  FREE(fsymtab[code_19].name);
  FREE(fsymtab[code_18].name);
  FREE(fsymtab[code_15].name);
  FREE(fsymtab[code_14].name);
  FREE(fsymtab[code_68].name);
  FREE(fsymtab[code_13].name);
  FREE(fsymtab[code_12].name);
  FREE(fsymtab[code_11].name);
  FREE(fsymtab[code_10].name);
  FREE(fsymtab[code_24].name);
  FREE(fsymtab[code_22].name);
  FREE(fsymtab[code_21].name);
  FREE(fsymtab[code_205].name);
  FREE(fsymtab[code_33].name);
  FREE(fsymtab[code_204].name);
  FREE(fsymtab[code_32].name);
  FREE(fsymtab[code_203].name);
  FREE(fsymtab[code_31].name);
  FREE(fsymtab[code_202].name);
  FREE(fsymtab[code_30].name);
  FREE(fsymtab[code_201].name);
  FREE(fsymtab[code_200].name);
  FREE(fsymtab[code_9].name);
  FREE(fsymtab[code_8].name);
  TERM_FREE(con_1);
  TERM_FREE(con_0);
  TERM_FREE(con_205);
  TERM_FREE(con_204);
  TERM_FREE(con_202);
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
#include "ac_tools.c"
int gram[] = {
1,32788,201,12,1,1,59,
1,32798,202,12,1,0,1506,
1,32768,200,59,1,1,32,
1,32768,203,81,2,0,683,1,12,
1,32768,204,81,1,0,822,
1,32768,205,81,1,0,1217,
1,0,14,428,4,1,32,2,61,2,61,1,32,
1,0,15,428,4,1,32,2,33,2,61,1,32,
1,32768,14,428,6,0,841,2,40,1,32,2,44,1,32,2,41,
1,32768,15,428,6,0,951,2,40,1,32,2,44,1,32,2,41,
1,0,18,428,4,1,59,2,61,2,61,1,59,
1,0,19,428,4,1,59,2,33,2,61,1,59,
1,32768,18,428,6,0,1368,2,40,1,59,2,44,1,59,2,41,
1,32768,19,428,6,0,1478,2,40,1,59,2,44,1,59,2,41,
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
1,0,18,428,4,1,59,2,61,2,61,1,59,
1,0,19,428,4,1,59,2,33,2,61,1,59,
1,0,30,428,3,1,59,2,60,1,59,
1,0,31,428,4,1,59,2,60,2,61,1,59,
1,0,32,428,3,1,59,2,62,1,59,
1,0,33,428,4,1,59,2,62,2,61,1,59,
1,0,18,428,6,0,1368,2,40,1,59,2,44,1,59,2,41,
1,0,19,428,6,0,1478,2,40,1,59,2,44,1,59,2,41,
1,32768,30,428,6,0,1593,2,40,1,59,2,44,1,59,2,41,
1,32768,31,428,6,0,1807,2,40,1,59,2,44,1,59,2,41,
1,32768,32,428,6,0,1900,2,40,1,59,2,44,1,59,2,41,
1,32768,33,428,6,0,2114,2,40,1,59,2,44,1,59,2,41,
1,36768,68,32,1,1,220,
0};
int pos = 641;
int earleyQuerySort = 81;
int earleyQueryStrategy = 0;
char *tabIdentStr[] = {
"case",
"export",
"SUCH",
"lesseq_bool",
"THAT",
"and",
"result",
"strategy",
"label",
"IF",
"check",
"lesseq_identifier",
"less_identifier",
"dc",
"ANDIF",
"query",
"declare",
"assocLeft",
"try",
"otherwise",
"dkcall",
"greatereq_bool",
"sort",
"dccall",
"normalId",
"import",
"queryend",
"META",
"greater_identifier",
"specification",
"defined",
"specialLabel",
"start",
"AC",
"identifier",
"rules",
"spart",
"greater_bool",
"first",
"normalize",
"private",
"neq_identifier",
"oneconcur",
"normalise",
"true",
"know",
"switch",
"LPL",
"source",
"FOR",
"with",
"explicit",
"lesseq",
"greatereq_identifier",
"eq_identifier",
"less",
"not",
"builtin",
"x",
"pri",
"assocRight",
"iterate",
"less_bool",
"part",
"greatereq",
"or",
"dont",
"X",
"implicit",
"module",
"dkconcur",
"then",
"dcOne",
"repeat",
"EACH",
"choose",
"anyIdentifier",
"inline",
"for",
"where",
"hardAlias",
"neq",
"one",
"greater",
"cmp",
"ident",
"neq_ident",
"END",
"bool",
"typus",
"eq",
"public",
"of",
"strategies",
"AND",
"bs",
"dcconcur",
"theSpecialLabel",
"neq_bool",
"cs3",
"firstOne",
"definedAs",
"eq_ident",
"description",
"whatAmI",
"eq_bool",
"end",
"local",
"alias",
"false",
"Epsilon",
"dk",
"id",
"if",
"stratop",
"fail",
"global",
"operators",
"code",
"call",
"handline",
"care",
""};
int tabIdentIndex[] = {
834,
674,
307,
1176,
305,
518,
671,
883,
512,
143,
510,
1807,
1593,
199,
354,
566,
720,
932,
351,
986,
619,
1483,
456,
611,
822,
667,
877,
295,
1900,
1377,
719,
1217,
558,
132,
1059,
555,
554,
1269,
552,
977,
763,
1478,
972,
970,
448,
447,
658,
232,
657,
231,
444,
866,
653,
2114,
1368,
650,
337,
759,
120,
331,
1047,
750,
962,
439,
960,
225,
437,
88,
859,
646,
857,
431,
489,
641,
273,
852,
1355,
850,
327,
539,
905,
324,
322,
746,
320,
532,
951,
215,
428,
581,
214,
639,
213,
1083,
211,
424,
849,
1506,
847,
265,
842,
899,
841,
1188,
683,
737,
311,
523,
522,
734,
730,
207,
205,
418,
781,
412,
625,
991,
411,
623,
835,
622,
0};
int tabIdentSize = 122;
char *tabSortStr[] = {
"ident",
"intern string",
"typus",
"bool",
"identifier",
"builtinInt",
"string",
"intern int",
"intern ident",
"label",
""};
int tabSortIndex[] = {
32,
351,
81,
428,
59,
58,
163,
19,
220,
12,
0};
int tabSortSize = 10;
char *tabStrategyStr[] = {
""};
int tabStrategyIndex[] = {
0};
int tabStrategySize = 0;
