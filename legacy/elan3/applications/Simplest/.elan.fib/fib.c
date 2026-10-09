#include "fib.h"
#include "main_skeleton.c"

/* Constantes d'execution */
unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];
int fsymtabSize = FSYM_TAB_SIZE;

/* Table des symboles */
Gfsym fsymtab[FSYM_TAB_SIZE];
/* Declaration des pattern_list */

/* Constantes */
Gterm *con_1;
Gterm *con_0;

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
NULL, NULL, NULL, NULL, (funTabType) &fun_301, (funTabType) &fun_302, 
(funTabType) &fun_303, (funTabType) &fun_304, (funTabType) &fun_305, 
(funTabType) &fun_306, (funTabType) &fun_307, (funTabType) &fun_308, 
(funTabType) &fun_309, (funTabType) &fun_310, (funTabType) &fun_311, 
(funTabType) &fun_312, (funTabType) &fun_313, (funTabType) &fun_314, 
(funTabType) &fun_315, (funTabType) &fun_316, (funTabType) &fun_317, 
(funTabType) &fun_318, NULL};

/* tableau de pointeurs de fonctions qui retournent */
/* un pointeur sur un term */
funTabType strTab[] = {
NULL, NULL, NULL};
int strTabSize = 3;

/* Initialisation des patterns AC */
TERM *EkerTerm[100];
void EkerTermInit() {
  TERM_LIST *vlist[100];
  AC_LIST *acvlist[100];
  Gterm *sv[100];
}

/* Query */
Gterm *main_query() {
  Gterm *res;
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
    Gfsym_init(i,0,"nullString","nullSort",0,0,NULL);
  }
  Gfsym_init(code_310,2,"neq_int(,)","bool",0,0, NULL);
  Gfsym_init(code_13,2,"greatereq_builtinInt(,)","bool",13,0, NULL);
  Gfsym_init(code_12,2,"greater_builtinInt(,)","bool",12,0, NULL);
  Gfsym_init(code_11,2,"lesseq_builtinInt(,)","bool",11,0, NULL);
  Gfsym_init(code_67,1,"","builtinInt",0,0, NULL);
  Gfsym_init(code_10,2,"less_builtinInt(,)","bool",10,0, NULL);
  Gfsym_init(code_9,2,"neq_builtinInt(,)","bool",9,0, NULL);
  Gfsym_init(code_8,2,"eq_builtinInt(,)","bool",8,0, NULL);
  Gfsym_init(code_6,2,"div(,)","builtinInt",6,0, NULL);
  Gfsym_init(code_5,2,"time(,)","builtinInt",5,0, NULL);
  Gfsym_init(code_4,2,"minus(,)","builtinInt",4,0, NULL);
  Gfsym_init(code_3,2,"plus(,)","builtinInt",3,0, NULL);
  Gfsym_init(code_1,0,"true","bool",0,0, NULL);
  Gfsym_init(code_0,0,"false","bool",0,0, NULL);
  Gfsym_init(code_309,2,"eq_int(,)","bool",0,0, NULL);
  Gfsym_init(code_308,1,"umin()","int",0,0, NULL);
  Gfsym_init(code_307,2,"or(,)","int",0,0, NULL);
  Gfsym_init(code_306,2,"div(,)","int",0,0, NULL);
  Gfsym_init(code_305,2,"and(,)","int",0,0, NULL);
  Gfsym_init(code_304,2,"mod(,)","int",0,0, NULL);
  Gfsym_init(code_303,2,"time(,)","int",0,0, NULL);
  Gfsym_init(code_302,2,"minus(,)","int",0,0, NULL);
  Gfsym_init(code_301,2,"plus(,)","int",0,0, NULL);
  Gfsym_init(code_300,1,"","int",0,0, NULL);
  Gfsym_init(code_29,2,"or(,)","builtinInt",29,0, NULL);
  Gfsym_init(code_28,2,"and(,)","builtinInt",28,0, NULL);
  Gfsym_init(code_27,2,"mod(,)","builtinInt",27,0, NULL);
  Gfsym_init(code_26,1,"itob_builtinInt()","bool",26,0, NULL);
  Gfsym_init(code_25,1,"btoi_builtinInt()","builtinInt",25,0, NULL);
  Gfsym_init(code_24,1,"(not())","bool",24,0, NULL);
  Gfsym_init(code_79,1,"-","builtinInt",0,0, NULL);
  Gfsym_init(code_22,2,"(or)","bool",22,0, NULL);
  Gfsym_init(code_21,2,"(and)","bool",21,0, NULL);
  Gfsym_init(code_20,1,"umin_builtinInt()","builtinInt",20,0, NULL);
  Gfsym_init(code_318,1,"fib()","int",0,0, NULL);
  Gfsym_init(code_317,1,"valueOf()","builtinInt",0,0, NULL);
  Gfsym_init(code_316,1,"itob_int()","bool",0,0, NULL);
  Gfsym_init(code_315,1,"btoi_int()","int",0,0, NULL);
  Gfsym_init(code_314,2,"less_int(,)","bool",0,0, NULL);
  Gfsym_init(code_313,2,"lesseq_int(,)","bool",0,0, NULL);
  Gfsym_init(code_312,2,"greatereq_int(,)","bool",0,0, NULL);
  Gfsym_init(code_311,2,"greater_int(,)","bool",0,0, NULL);
  
/* Initialisation des constantes */
  Gmake_const(con_1, code_1);
  Gmake_const(con_0, code_0);
  /* Initialisation des pattern_list */
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
1,32768,27,58,6,0,320,2,40,1,58,2,44,1,58,2,41,
1,32768,28,58,6,0,518,2,40,1,58,2,44,1,58,2,41,
1,32768,29,58,6,0,225,2,40,1,58,2,44,1,58,2,41,
1,0,20,58,4,0,441,2,40,1,58,2,41,
1,32768,20,58,4,0,1594,2,40,1,58,2,41,
1,900,25,58,4,0,430,2,40,1,428,2,41,
1,33668,25,58,4,0,1583,2,40,1,428,2,41,
1,33668,317,58,4,0,722,2,40,1,331,2,41,
1,1000,300,331,1,1,58,
1,1000,300,331,3,2,91,1,58,2,93,
1,4596,301,331,3,1,331,2,43,1,331,
1,4696,302,331,3,1,331,2,45,1,331,
1,4796,303,331,3,1,331,2,42,1,331,
1,4896,304,331,3,1,331,2,37,1,331,
1,4896,305,331,3,1,331,2,38,1,331,
1,4896,306,331,3,1,331,2,47,1,331,
1,4896,307,331,3,1,331,2,124,1,331,
1,900,308,331,2,2,45,1,331,
1,500,301,331,5,2,40,1,331,2,43,1,331,2,41,
1,600,302,331,5,2,40,1,331,2,45,1,331,2,41,
1,700,303,331,5,2,40,1,331,2,42,1,331,2,41,
1,800,306,331,5,2,40,1,331,2,47,1,331,2,41,
1,800,304,331,5,2,40,1,331,2,37,1,331,2,41,
1,800,305,331,5,2,40,1,331,2,38,1,331,2,41,
1,800,307,331,5,2,40,1,331,2,124,1,331,2,41,
1,900,308,331,4,2,40,2,45,1,331,2,41,
1,33268,301,331,6,0,452,2,40,1,331,2,44,1,331,2,41,
1,33368,302,331,6,0,556,2,40,1,331,2,44,1,331,2,41,
1,33468,303,331,6,0,642,2,40,1,331,2,44,1,331,2,41,
1,33568,306,331,6,0,323,2,40,1,331,2,44,1,331,2,41,
1,33568,304,331,6,0,320,2,40,1,331,2,44,1,331,2,41,
1,33568,305,331,6,0,518,2,40,1,331,2,44,1,331,2,41,
1,33568,307,331,6,0,225,2,40,1,331,2,44,1,331,2,41,
1,33668,308,331,4,0,441,2,40,1,331,2,41,
1,900,315,331,4,0,430,2,40,1,428,2,41,
1,33668,315,331,4,0,856,2,40,1,428,2,41,
1,33768,300,331,1,1,58,
1,32768,318,331,4,0,305,2,40,1,331,2,41,
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
1,32968,8,428,6,0,737,2,40,1,428,2,44,1,428,2,41,
1,32968,9,428,6,0,847,2,40,1,428,2,44,1,428,2,41,
1,32968,10,428,6,0,962,2,40,1,428,2,44,1,428,2,41,
1,32968,11,428,6,0,1176,2,40,1,428,2,44,1,428,2,41,
1,32968,12,428,6,0,1269,2,40,1,428,2,44,1,428,2,41,
1,32968,13,428,6,0,1483,2,40,1,428,2,44,1,428,2,41,
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
1,300,309,428,4,1,331,2,61,2,61,1,331,
1,300,310,428,4,1,331,2,33,2,61,1,331,
1,300,311,428,3,1,331,2,62,1,331,
1,300,312,428,4,1,331,2,62,2,61,1,331,
1,300,313,428,4,1,331,2,60,2,61,1,331,
1,300,314,428,3,1,331,2,60,1,331,
1,33068,309,428,6,0,640,2,40,1,331,2,44,1,331,2,41,
1,33068,310,428,6,0,961,2,40,1,331,2,44,1,331,2,41,
1,33068,314,428,6,0,865,2,40,1,331,2,44,1,331,2,41,
1,33068,313,428,6,0,1079,2,40,1,331,2,44,1,331,2,41,
1,33068,311,428,6,0,1172,2,40,1,331,2,44,1,331,2,41,
1,33068,312,428,6,0,1386,2,40,1,331,2,44,1,331,2,41,
1,900,316,428,4,0,1063,2,40,1,331,2,41,
1,33668,316,428,4,0,1067,2,40,1,331,2,41,
1,36768,67,58,1,1,19,
1,36768,79,58,2,2,45,1,19,
0};
int globalGramSize = 1502;
int earleyQuerySort = 331;
int earleyQueryStrategy = 0;
char *tabIdentStr[] = {
"int",
"greatereq_bool",
"builtin",
"part",
"tone",
"dont",
"specification",
"builtinInt",
"iterate",
"then",
"btoi",
"n",
"module",
"greater_bool",
"for",
"less_bool",
"neq_int",
"neq",
"time",
"greatereq",
"btoi_builtinInt",
"div",
"repeat",
"one",
"eq_int",
"implicit",
"mod",
"where",
"dkconcur",
"btoi_int",
"neq_builtinInt",
"itob_builtinInt",
"choose",
"END",
"eq",
"inline",
"of",
"AND",
"tall",
"bool",
"greater",
"eq_builtinInt",
"bs",
"assocRight",
"anyInteger",
"greater_builtinInt",
"public",
"d",
"c",
"b",
"a",
"dcconcur",
"end",
"neq_bool",
"dk",
"firstOne",
"id",
"local",
"alias",
"eq_bool",
"if",
"false",
"fail",
"Epsilon",
"code",
"global",
"SUCH",
"call",
"fib",
"care",
"dc",
"and",
"handline",
"THAT",
"case",
"check",
"valueOf",
"declare",
"dkcall",
"META",
"assocLeft",
"dccall",
"greatereq_builtinInt",
"defined",
"EACH",
"dcOne",
"hardAlias",
"definedAs",
"normout",
"stratop",
"IF",
"strategies",
"export",
"operators",
"ANDIF",
"result",
"try",
"query",
"strategy",
"description",
"sort",
"lesseq_int",
"plus",
"rewrite",
"AC",
"otherwise",
"import",
"queryend",
"start",
"minus",
"rules",
"spart",
"lesseq_bool",
"tsome",
"LPL",
"FOR",
"greater_int",
"true",
"know",
"first",
"greatereq_int",
"with",
"itob_int",
"umin",
"normin",
"normalize",
"itob",
"switch",
"source",
"private",
"not",
"oneconcur",
"lesseq",
"umin_builtinInt",
"normalise",
"less_builtinInt",
"less",
"pri",
"explicit",
"less_int",
"lesseq_builtinInt",
"or",
""};
int tabIdentIndex[] = {
542,
1483,
759,
439,
438,
437,
1377,
1058,
750,
431,
430,
110,
646,
1269,
327,
962,
961,
324,
642,
960,
1583,
323,
641,
322,
640,
859,
320,
539,
857,
856,
1477,
1794,
852,
215,
214,
850,
213,
211,
429,
428,
746,
1367,
424,
1047,
1046,
1899,
639,
100,
99,
98,
97,
849,
311,
847,
207,
842,
205,
523,
522,
737,
418,
734,
412,
730,
411,
625,
307,
623,
305,
622,
199,
518,
835,
516,
834,
510,
722,
720,
619,
295,
932,
611,
2113,
719,
273,
489,
905,
899,
788,
781,
143,
1083,
674,
991,
354,
671,
351,
566,
883,
1188,
456,
1079,
452,
770,
132,
986,
667,
877,
558,
556,
555,
554,
1176,
552,
232,
231,
1172,
448,
447,
763,
1386,
444,
1067,
441,
659,
977,
1063,
658,
657,
974,
337,
972,
653,
1594,
970,
1592,
650,
331,
866,
865,
1806,
225,
0};
int tabIdentSize = 142;
char *tabSortStr[] = {
"ident",
"builtinString",
"intern string",
"bool",
"builtinInt",
"intern int",
"intern ident",
"int",
""};
int tabSortIndex[] = {
32,
390,
351,
428,
58,
19,
220,
331,
0};
int tabSortSize = 8;
char *tabStrategyStr[] = {
""};
int tabStrategyIndex[] = {
0};
int tabStrategySize = 0;
