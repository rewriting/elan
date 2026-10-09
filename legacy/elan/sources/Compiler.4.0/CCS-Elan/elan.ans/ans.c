#include "ans.h"
#include "main_skeleton_lib.c"

/* Constantes d'execution */
unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];
int fsymtabSize = FSYM_TAB_SIZE;

/* Table des symboles */
Gfsym fsymtab[FSYM_TAB_SIZE];
/* Declaration des pattern_list */

/* Constantes */
Gterm *con_348;
Gterm *con_344;
Gterm *con_342;
Gterm *con_335;
Gterm *con_328;
Gterm *con_327;
Gterm *con_420;
Gterm *con_419;
Gterm *con_413;
Gterm *con_412;
Gterm *con_411;
Gterm *con_410;
Gterm *con_409;
Gterm *con_408;
Gterm *con_407;
Gterm *con_406;
Gterm *con_405;
Gterm *con_404;
Gterm *con_403;
Gterm *con_402;
Gterm *con_401;
Gterm *con_400;
Gterm *con_399;
Gterm *con_398;
Gterm *con_397;
Gterm *con_396;
Gterm *con_395;
Gterm *con_394;
Gterm *con_393;
Gterm *con_392;
Gterm *con_391;
Gterm *con_390;
Gterm *con_389;
Gterm *con_388;
Gterm *con_387;
Gterm *con_386;
Gterm *con_385;
Gterm *con_384;
Gterm *con_383;
Gterm *con_382;
Gterm *con_381;
Gterm *con_380;
Gterm *con_379;
Gterm *con_378;
Gterm *con_377;
Gterm *con_376;
Gterm *con_375;
Gterm *con_374;
Gterm *con_373;
Gterm *con_372;
Gterm *con_371;
Gterm *con_370;
Gterm *con_369;
Gterm *con_368;
Gterm *con_367;
Gterm *con_366;
Gterm *con_365;
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
(funTabType) &fun_318, (funTabType) &fun_319, (funTabType) &fun_320, 
(funTabType) &fun_321, (funTabType) &fun_322, (funTabType) &fun_323, 
(funTabType) &fun_324, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_331, (funTabType) &fun_332, (funTabType) &fun_333, 
(funTabType) &fun_334, NULL, NULL, (funTabType) &fun_337, NULL, 
(funTabType) &fun_339, (funTabType) &fun_340, (funTabType) &fun_341, 
NULL, (funTabType) &fun_343, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, (funTabType) &fun_352, NULL, NULL, NULL, 
(funTabType) &fun_356, (funTabType) &fun_357, NULL, NULL, 
(funTabType) &fun_360, (funTabType) &fun_361, (funTabType) &fun_362, 
NULL, (funTabType) &fun_364, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
(funTabType) &fun_414, (funTabType) &fun_415, (funTabType) &fun_416, 
(funTabType) &fun_417, (funTabType) &fun_418, NULL, NULL, NULL, NULL};

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
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, (funTabType) &str_127, NULL, 
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
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, (funTabType) &str_472, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
NULL, NULL, NULL, NULL, NULL, (funTabType) &str_494, NULL, NULL, 
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
NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};
int strTabSize = 991;

/* Initialisation des patterns AC */
//TERM *EkerTerm[100];
void EkerTermInit() {
  //TERM_LIST *vlist[100];
  //AC_LIST *acvlist[100];
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
    Gfsym_init(i,0,"nullString",0,0,NULL);
  }
  Gfsym_init(code_358,1,"(;)",0,0, NULL);
  Gfsym_init(code_357,1,"getCcst()",0,0, NULL);
  Gfsym_init(code_356,1,"getLabelCcst()",0,0, NULL);
  Gfsym_init(code_355,2,"(;;)",0,0, NULL);
  Gfsym_init(code_354,2,"[]",0,0, NULL);
  Gfsym_init(code_353,2,"\\",0,0, NULL);
  Gfsym_init(code_352,1,"()",0,0, NULL);
  Gfsym_init(code_79,1,"-",0,0, NULL);
  Gfsym_init(code_351,2,".",0,0, NULL);
  Gfsym_init(code_350,2,"+",0,0, NULL);
  Gfsym_init(code_349,2,"#",0,0, NULL);
  Gfsym_init(code_348,0,"0",0,0, NULL);
  Gfsym_init(code_347,1,"",0,0, NULL);
  Gfsym_init(code_346,1,"'",0,0, NULL);
  Gfsym_init(code_345,1,"",0,0, NULL);
  Gfsym_init(code_344,0,"tau",0,0, NULL);
  Gfsym_init(code_343,3,".updateHashTable(,)",0,0, NULL);
  Gfsym_init(code_342,0,"listExtract",0,0, NULL);
  Gfsym_init(code_341,2,"ccat(,)",0,0, NULL);
  Gfsym_init(code_68,1,"",0,0, NULL);
  Gfsym_init(code_340,1,"size_of_rccst_list()",0,0, NULL);
  Gfsym_init(code_67,1,"",0,0, NULL);
  Gfsym_init(code_339,2,"-thelem()",0,0, NULL);
  Gfsym_init(code_338,1,"elem()",0,0, NULL);
  Gfsym_init(code_337,2,"@",0,0, NULL);
  Gfsym_init(code_336,2,",",0,0, NULL);
  Gfsym_init(code_335,0,"nil",0,0, NULL);
  Gfsym_init(code_334,2,".cleanIndex()",0,0, NULL);
  Gfsym_init(code_333,3,"update(,,)",0,0, NULL);
  Gfsym_init(code_332,2,"append(,)",0,0, NULL);
  Gfsym_init(code_331,2,"find(,)",0,0, NULL);
  Gfsym_init(code_330,2,"[,]",0,0, NULL);
  Gfsym_init(code_329,2,".",0,0, NULL);
  Gfsym_init(code_328,0,"nil_HT",0,0, NULL);
  Gfsym_init(code_327,0,"emptyKeyHashTable",0,0, NULL);
  Gfsym_init(code_326,2,"[,]",0,0, NULL);
  Gfsym_init(code_325,1,"[]",0,0, NULL);
  Gfsym_init(code_324,1,"hashTerm()",0,0, NULL);
  Gfsym_init(code_323,1,"size()",0,0, NULL);
  Gfsym_init(code_322,2,".containsKey()",0,0, NULL);
  Gfsym_init(code_321,2,".get()",0,0, NULL);
  Gfsym_init(code_320,3,".add(,)",0,0, NULL);
  Gfsym_init(code_210,1,"builtinHashTerm()",210,0, NULL);
  Gfsym_init(code_421,1,"",0,0, NULL);
  Gfsym_init(code_420,0,"calc_str",0,0, NULL);
  Gfsym_init(code_319,1,".cleanAll",0,0, NULL);
  Gfsym_init(code_318,2,"new(,)",0,0, NULL);
  Gfsym_init(code_317,1,"valueOf()",0,0, NULL);
  Gfsym_init(code_316,1,"itob_int()",0,0, NULL);
  Gfsym_init(code_315,1,"btoi_int()",0,0, NULL);
  Gfsym_init(code_314,2,"less_int(,)",0,0, NULL);
  Gfsym_init(code_313,2,"lesseq_int(,)",0,0, NULL);
  Gfsym_init(code_312,2,"greatereq_int(,)",0,0, NULL);
  Gfsym_init(code_311,2,"greater_int(,)",0,0, NULL);
  Gfsym_init(code_310,2,"neq_int(,)",0,0, NULL);
  Gfsym_init(code_33,2,"greatereq_identifier(,)",33,0, NULL);
  Gfsym_init(code_32,2,"greater_identifier(,)",32,0, NULL);
  Gfsym_init(code_31,2,"lesseq_identifier(,)",31,0, NULL);
  Gfsym_init(code_203,1,"length()",203,0, NULL);
  Gfsym_init(code_30,2,"less_identifier(,)",30,0, NULL);
  Gfsym_init(code_202,3,"[]<-",202,0, NULL);
  Gfsym_init(code_201,2,"[]",201,0, NULL);
  Gfsym_init(code_200,2,"new(,)",200,0, NULL);
  Gfsym_init(code_419,0,"start_str",0,0, NULL);
  Gfsym_init(code_418,2,"applon",0,0, NULL);
  Gfsym_init(code_417,2,"applyon",0,0, NULL);
  Gfsym_init(code_416,2,"isInverseLabel(,)",0,0, NULL);
  Gfsym_init(code_415,2,"doesNotRestrictName(,)",0,0, NULL);
  Gfsym_init(code_414,2,"doesNotRestrict(,)",0,0, NULL);
  Gfsym_init(code_413,0,"req2",0,0, NULL);
  Gfsym_init(code_412,0,"req1",0,0, NULL);
  Gfsym_init(code_411,0,"kw2",0,0, NULL);
  Gfsym_init(code_410,0,"kw1",0,0, NULL);
  Gfsym_init(code_309,2,"eq_int(,)",0,0, NULL);
  Gfsym_init(code_308,1,"umin()",0,0, NULL);
  Gfsym_init(code_307,2,"or(,)",0,0, NULL);
  Gfsym_init(code_306,2,"div(,)",0,0, NULL);
  Gfsym_init(code_305,2,"and(,)",0,0, NULL);
  Gfsym_init(code_304,2,"mod(,)",0,0, NULL);
  Gfsym_init(code_303,2,"time(,)",0,0, NULL);
  Gfsym_init(code_302,2,"minus(,)",0,0, NULL);
  Gfsym_init(code_29,2,"or(,)",29,0, NULL);
  Gfsym_init(code_301,2,"plus(,)",0,0, NULL);
  Gfsym_init(code_28,2,"and(,)",28,0, NULL);
  Gfsym_init(code_300,1,"[]",0,0, NULL);
  Gfsym_init(code_27,2,"mod(,)",27,0, NULL);
  Gfsym_init(code_26,1,"itob_builtinInt()",26,0, NULL);
  Gfsym_init(code_25,1,"btoi_builtinInt()",25,0, NULL);
  Gfsym_init(code_24,1,"(not())",24,0, NULL);
  Gfsym_init(code_22,2,"(or)",22,0, NULL);
  Gfsym_init(code_21,2,"(and)",21,0, NULL);
  Gfsym_init(code_20,1,"umin_builtinInt()",20,0, NULL);
  Gfsym_init(code_409,0,"kr2",0,0, NULL);
  Gfsym_init(code_408,0,"kr1",0,0, NULL);
  Gfsym_init(code_407,0,"exit2",0,0, NULL);
  Gfsym_init(code_406,0,"exit1",0,0, NULL);
  Gfsym_init(code_405,0,"enter2",0,0, NULL);
  Gfsym_init(code_404,0,"enter1",0,0, NULL);
  Gfsym_init(code_403,0,"c2w2",0,0, NULL);
  Gfsym_init(code_402,0,"c2w1",0,0, NULL);
  Gfsym_init(code_401,0,"c2w0",0,0, NULL);
  Gfsym_init(code_400,0,"c2r2",0,0, NULL);
  Gfsym_init(code_19,2,"neq_identifier(,)",19,0, NULL);
  Gfsym_init(code_18,2,"eq_identifier(,)",18,0, NULL);
  Gfsym_init(code_15,2,"neq_ident(,)",15,0, NULL);
  Gfsym_init(code_14,2,"eq_ident(,)",14,0, NULL);
  Gfsym_init(code_13,2,"greatereq_builtinInt(,)",13,0, NULL);
  Gfsym_init(code_12,2,"greater_builtinInt(,)",12,0, NULL);
  Gfsym_init(code_11,2,"lesseq_builtinInt(,)",11,0, NULL);
  Gfsym_init(code_399,0,"c2r1",0,0, NULL);
  Gfsym_init(code_10,2,"less_builtinInt(,)",10,0, NULL);
  Gfsym_init(code_398,0,"c2r0",0,0, NULL);
  Gfsym_init(code_397,0,"c1w2",0,0, NULL);
  Gfsym_init(code_396,0,"c1w1",0,0, NULL);
  Gfsym_init(code_395,0,"c1w0",0,0, NULL);
  Gfsym_init(code_394,0,"c1r2",0,0, NULL);
  Gfsym_init(code_393,0,"c1r1",0,0, NULL);
  Gfsym_init(code_392,0,"c1r0",0,0, NULL);
  Gfsym_init(code_391,0,"L",0,0, NULL);
  Gfsym_init(code_390,0,"KBig",0,0, NULL);
  Gfsym_init(code_389,0,"P27",0,0, NULL);
  Gfsym_init(code_388,0,"P26",0,0, NULL);
  Gfsym_init(code_387,0,"P25",0,0, NULL);
  Gfsym_init(code_386,0,"P24",0,0, NULL);
  Gfsym_init(code_385,0,"P23",0,0, NULL);
  Gfsym_init(code_384,0,"P22",0,0, NULL);
  Gfsym_init(code_383,0,"P21",0,0, NULL);
  Gfsym_init(code_382,0,"P2",0,0, NULL);
  Gfsym_init(code_381,0,"P17",0,0, NULL);
  Gfsym_init(code_380,0,"P16",0,0, NULL);
  Gfsym_init(code_379,0,"P15",0,0, NULL);
  Gfsym_init(code_378,0,"P14",0,0, NULL);
  Gfsym_init(code_377,0,"P13",0,0, NULL);
  Gfsym_init(code_376,0,"P12",0,0, NULL);
  Gfsym_init(code_375,0,"P11",0,0, NULL);
  Gfsym_init(code_374,0,"P1",0,0, NULL);
  Gfsym_init(code_373,0,"C2_2",0,0, NULL);
  Gfsym_init(code_372,0,"C2_1",0,0, NULL);
  Gfsym_init(code_371,0,"C2_0",0,0, NULL);
  Gfsym_init(code_370,0,"C1_2",0,0, NULL);
  Gfsym_init(code_369,0,"C1_1",0,0, NULL);
  Gfsym_init(code_368,0,"C1_0",0,0, NULL);
  Gfsym_init(code_367,0,"K2",0,0, NULL);
  Gfsym_init(code_366,0,"K1",0,0, NULL);
  Gfsym_init(code_365,0,"Knuth",0,0, NULL);
  Gfsym_init(code_364,0,"go",0,0, NULL);
  Gfsym_init(code_363,2,"-->",0,0, NULL);
  Gfsym_init(code_362,2,"buildRccst(,)",0,0, NULL);
  Gfsym_init(code_361,1,"getHashTable()",0,0, NULL);
  Gfsym_init(code_360,1,"getRccst()",0,0, NULL);
  Gfsym_init(code_9,2,"neq_builtinInt(,)",9,0, NULL);
  Gfsym_init(code_8,2,"eq_builtinInt(,)",8,0, NULL);
  Gfsym_init(code_6,2,"div(,)",6,0, NULL);
  Gfsym_init(code_5,2,"time(,)",5,0, NULL);
  Gfsym_init(code_4,2,"minus(,)",4,0, NULL);
  Gfsym_init(code_3,2,"plus(,)",3,0, NULL);
  Gfsym_init(code_1,0,"true",0,0, NULL);
  Gfsym_init(code_0,0,"false",0,0, NULL);
  Gfsym_init(code_359,2,"|",0,0, NULL);
  
/* Initialisation des constantes */
  Gmake_const(con_348, code_348);
  Gmake_const(con_344, code_344);
  Gmake_const(con_342, code_342);
  Gmake_const(con_335, code_335);
  Gmake_const(con_328, code_328);
  Gmake_const(con_327, code_327);
  Gmake_const(con_420, code_420);
  Gmake_const(con_419, code_419);
  Gmake_const(con_413, code_413);
  Gmake_const(con_412, code_412);
  Gmake_const(con_411, code_411);
  Gmake_const(con_410, code_410);
  Gmake_const(con_409, code_409);
  Gmake_const(con_408, code_408);
  Gmake_const(con_407, code_407);
  Gmake_const(con_406, code_406);
  Gmake_const(con_405, code_405);
  Gmake_const(con_404, code_404);
  Gmake_const(con_403, code_403);
  Gmake_const(con_402, code_402);
  Gmake_const(con_401, code_401);
  Gmake_const(con_400, code_400);
  Gmake_const(con_399, code_399);
  Gmake_const(con_398, code_398);
  Gmake_const(con_397, code_397);
  Gmake_const(con_396, code_396);
  Gmake_const(con_395, code_395);
  Gmake_const(con_394, code_394);
  Gmake_const(con_393, code_393);
  Gmake_const(con_392, code_392);
  Gmake_const(con_391, code_391);
  Gmake_const(con_390, code_390);
  Gmake_const(con_389, code_389);
  Gmake_const(con_388, code_388);
  Gmake_const(con_387, code_387);
  Gmake_const(con_386, code_386);
  Gmake_const(con_385, code_385);
  Gmake_const(con_384, code_384);
  Gmake_const(con_383, code_383);
  Gmake_const(con_382, code_382);
  Gmake_const(con_381, code_381);
  Gmake_const(con_380, code_380);
  Gmake_const(con_379, code_379);
  Gmake_const(con_378, code_378);
  Gmake_const(con_377, code_377);
  Gmake_const(con_376, code_376);
  Gmake_const(con_375, code_375);
  Gmake_const(con_374, code_374);
  Gmake_const(con_373, code_373);
  Gmake_const(con_372, code_372);
  Gmake_const(con_371, code_371);
  Gmake_const(con_370, code_370);
  Gmake_const(con_369, code_369);
  Gmake_const(con_368, code_368);
  Gmake_const(con_367, code_367);
  Gmake_const(con_366, code_366);
  Gmake_const(con_365, code_365);
  Gmake_const(con_1, code_1);
  Gmake_const(con_0, code_0);
  /* Initialisation des pattern_list */
}
int gram[] = {
1,32768,391,3,1,0,76,
1,32818,344,12,1,0,541,
1,0,345,12,4,0,112,2,40,1,429,2,41,
1,32769,345,12,1,1,429,
1,0,346,12,4,0,113,2,40,1,429,2,41,
1,32768,346,12,2,2,39,1,429,
1,32768,338,43,4,0,419,2,40,1,171,2,41,
1,32768,339,43,7,1,331,2,45,0,220,0,419,2,40,1,171,2,41,
1,32768,355,43,6,2,40,1,12,2,59,2,59,1,140,2,41,
1,32768,358,43,4,2,40,2,59,1,140,2,41,
1,32770,359,43,3,1,43,2,124,1,350,
1,32768,360,43,4,0,831,2,40,1,43,2,41,
1,32768,362,43,6,0,1039,2,40,1,140,2,44,1,350,2,41,
1,32818,363,43,5,1,43,2,45,2,45,2,62,1,43,
1,32768,364,43,1,0,425,
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
1,32768,27,58,6,0,531,2,40,1,58,2,44,1,58,2,41,
1,32768,28,58,6,0,518,2,40,1,58,2,44,1,58,2,41,
1,32768,29,58,6,0,225,2,40,1,58,2,44,1,58,2,41,
1,0,20,58,4,0,441,2,40,1,58,2,41,
1,32768,20,58,4,0,1594,2,40,1,58,2,41,
1,900,25,58,4,0,430,2,40,1,428,2,41,
1,33668,25,58,4,0,1583,2,40,1,428,2,41,
1,33668,317,58,4,0,722,2,40,1,331,2,41,
1,32768,421,59,1,1,32,
1,32768,365,109,1,0,733,
1,32768,366,109,1,0,124,
1,32768,367,109,1,0,125,
1,32768,368,109,1,0,259,
1,32768,369,109,1,0,260,
1,32768,370,109,1,0,261,
1,32768,371,109,1,0,471,
1,32768,372,109,1,0,472,
1,32768,373,109,1,0,262,
1,32768,374,109,1,0,129,
1,32768,375,109,1,0,178,
1,32768,376,109,1,0,179,
1,32768,377,109,1,0,180,
1,32768,378,109,1,0,181,
1,32768,379,109,1,0,182,
1,32768,380,109,1,0,183,
1,32768,381,109,1,0,184,
1,32768,382,109,1,0,130,
1,32768,383,109,1,0,390,
1,32768,384,109,1,0,391,
1,32768,385,109,1,0,392,
1,32768,386,109,1,0,393,
1,32768,387,109,1,0,394,
1,32768,388,109,1,0,395,
1,32768,389,109,1,0,185,
1,32768,390,109,1,0,349,
1,32808,347,140,1,1,109,
1,32818,348,140,1,3,0,
1,8202,349,140,6,2,124,2,40,1,140,2,44,1,140,2,41,
1,8212,350,140,6,2,43,2,40,1,140,2,44,1,140,2,41,
1,8202,349,140,3,1,140,2,124,1,140,
1,40970,349,140,3,1,140,2,35,1,140,
1,40980,350,140,3,1,140,2,43,1,140,
1,32798,351,140,3,1,12,2,46,1,140,
1,32768,352,140,3,2,40,1,140,2,41,
1,32773,353,140,3,1,140,2,92,1,3,
1,32775,354,140,4,1,140,2,91,1,496,2,93,
1,32768,356,140,4,0,1197,2,40,1,43,2,41,
1,32768,357,140,4,0,717,2,40,1,43,2,41,
1,32768,321,171,6,1,350,2,46,0,742,2,40,1,43,2,41,
1,0,335,171,1,0,1383,
1,32768,335,171,1,0,534,
1,0,336,171,3,1,43,2,46,1,171,
1,0,336,171,6,0,435,2,40,1,43,2,44,1,171,2,41,
1,0,336,171,2,1,43,1,171,
1,32768,336,171,3,1,43,2,44,1,171,
1,0,337,171,6,0,632,2,40,1,171,2,44,1,171,2,41,
1,32768,337,171,3,1,171,2,64,1,171,
1,0,341,171,6,0,833,2,40,1,331,2,42,1,171,2,41,
1,32768,341,171,6,0,833,2,40,1,331,2,44,1,171,2,41,
1,49152,342,315,1,0,1175,
1,49152,419,315,1,0,998,
1,49152,420,315,1,0,843,
1,33768,300,331,1,1,58,
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
1,33568,304,331,6,0,531,2,40,1,331,2,44,1,331,2,41,
1,33568,305,331,6,0,518,2,40,1,331,2,44,1,331,2,41,
1,33568,307,331,6,0,225,2,40,1,331,2,44,1,331,2,41,
1,33668,308,331,4,0,441,2,40,1,331,2,41,
1,900,315,331,4,0,430,2,40,1,428,2,41,
1,33668,315,331,4,0,856,2,40,1,428,2,41,
1,0,323,331,3,1,350,2,46,0,443,
1,32768,323,331,4,0,443,2,40,1,350,2,41,
1,32768,324,331,4,0,828,2,40,1,43,2,41,
1,0,340,331,4,0,443,2,40,1,171,2,41,
1,32768,340,331,4,0,1928,2,40,1,171,2,41,
1,32768,318,350,6,0,330,2,40,1,331,2,44,1,171,2,41,
1,32768,319,350,3,1,350,2,46,0,796,
1,32768,320,350,8,1,350,2,46,0,508,2,40,1,43,2,44,1,171,2,41,
1,32768,343,350,8,1,350,2,46,0,1519,2,40,1,43,2,44,1,43,2,41,
1,32768,361,350,4,0,1196,2,40,1,43,2,41,
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
1,4296,8,428,6,0,737,2,40,1,428,2,44,1,428,2,41,
1,4296,9,428,6,0,847,2,40,1,428,2,44,1,428,2,41,
1,4296,10,428,6,0,962,2,40,1,428,2,44,1,428,2,41,
1,4296,11,428,6,0,1176,2,40,1,428,2,44,1,428,2,41,
1,4296,12,428,6,0,1269,2,40,1,428,2,44,1,428,2,41,
1,4296,13,428,6,0,1483,2,40,1,428,2,44,1,428,2,41,
1,0,18,428,4,1,429,2,61,2,61,1,429,
1,0,19,428,4,1,429,2,33,2,61,1,429,
1,0,18,428,6,0,1238,2,40,1,429,2,44,1,429,2,41,
1,0,19,428,6,0,1348,2,40,1,429,2,44,1,429,2,41,
1,300,8,428,4,1,58,2,61,2,61,1,58,
1,300,9,428,4,1,58,2,33,2,61,1,58,
1,300,10,428,3,1,58,2,60,1,58,
1,300,11,428,4,1,58,2,60,2,61,1,58,
1,300,12,428,3,1,58,2,62,1,58,
1,300,13,428,4,1,58,2,62,2,61,1,58,
1,33068,8,428,6,0,1367,2,40,1,58,2,44,1,58,2,41,
1,33068,9,428,6,0,1477,2,40,1,58,2,44,1,58,2,41,
1,33068,10,428,6,0,1592,2,40,1,58,2,44,1,58,2,41,
1,33068,11,428,6,0,1806,2,40,1,58,2,44,1,58,2,41,
1,33068,12,428,6,0,1899,2,40,1,58,2,44,1,58,2,41,
1,33068,13,428,6,0,2113,2,40,1,58,2,44,1,58,2,41,
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
1,32768,322,428,6,1,350,2,46,0,1160,2,40,1,43,2,41,
1,0,18,428,4,1,171,2,61,2,61,1,171,
1,0,19,428,4,1,171,2,33,2,61,1,171,
1,0,30,428,3,1,171,2,60,1,171,
1,0,31,428,4,1,171,2,60,2,61,1,171,
1,0,32,428,3,1,171,2,62,1,171,
1,0,33,428,4,1,171,2,62,2,61,1,171,
1,0,18,428,9,0,753,2,91,0,543,2,93,2,40,1,171,2,44,1,171,2,41,
1,0,19,428,9,0,863,2,91,0,543,2,93,2,40,1,171,2,44,1,171,2,41,
1,32768,30,428,9,0,978,2,91,0,543,2,93,2,40,1,171,2,44,1,171,2,41,
1,32768,31,428,9,0,1192,2,91,0,543,2,93,2,40,1,171,2,44,1,171,2,41,
1,32768,32,428,9,0,1285,2,91,0,543,2,93,2,40,1,171,2,44,1,171,2,41,
1,32768,33,428,9,0,1499,2,91,0,543,2,93,2,40,1,171,2,44,1,171,2,41,
1,0,14,428,4,1,32,2,61,2,61,1,32,
1,0,15,428,4,1,32,2,33,2,61,1,32,
1,32768,14,428,6,0,841,2,40,1,32,2,44,1,32,2,41,
1,32768,15,428,6,0,951,2,40,1,32,2,44,1,32,2,41,
1,0,18,428,4,1,59,2,61,2,61,1,59,
1,0,19,428,4,1,59,2,33,2,61,1,59,
1,32768,18,428,6,0,1368,2,40,1,59,2,44,1,59,2,41,
1,32768,19,428,6,0,1478,2,40,1,59,2,44,1,59,2,41,
1,32768,392,429,1,0,310,
1,32768,393,429,1,0,944,
1,32768,394,429,1,0,312,
1,32768,395,429,1,0,315,
1,32768,396,429,1,0,316,
1,32768,397,429,1,0,317,
1,32768,398,429,1,0,1155,
1,32768,399,429,1,0,945,
1,32768,400,429,1,0,313,
1,32768,401,429,1,0,527,
1,32768,402,429,1,0,528,
1,32768,403,429,1,0,318,
1,32768,404,429,1,0,591,
1,32768,405,429,1,0,592,
1,32768,406,429,1,0,491,
1,32768,407,429,1,0,492,
1,32768,408,429,1,0,692,
1,32768,409,429,1,0,271,
1,32768,410,429,1,0,275,
1,32768,411,429,1,0,276,
1,32768,412,429,1,0,377,
1,32768,413,429,1,0,378,
1,32768,326,4,5,2,91,1,43,2,44,1,171,2,93,
1,32768,331,4,6,0,417,2,40,1,43,2,44,1,132,2,41,
1,32768,417,12,4,0,550,1,496,0,221,1,12,
1,32768,327,43,1,0,1732,
1,0,203,58,3,1,86,2,46,0,853,
1,32768,203,58,4,0,853,2,40,1,86,2,41,
1,32768,210,58,4,0,1555,2,40,1,43,2,41,
1,32768,200,86,6,0,330,2,40,1,58,2,44,1,132,2,41,
1,0,202,86,8,1,86,2,46,0,332,2,40,1,58,2,44,1,132,2,41,
1,32768,202,86,7,1,86,2,91,1,58,2,93,2,60,2,45,1,132,
1,0,201,132,6,1,86,2,46,0,742,2,40,1,58,2,41,
1,32768,201,132,4,1,86,2,91,1,58,2,93,
1,32768,328,132,1,0,574,
1,32768,329,132,3,1,4,2,46,1,132,
1,32768,330,132,5,2,91,1,4,2,44,1,132,2,93,
1,32768,332,132,6,0,632,2,40,1,132,2,44,1,132,2,41,
1,32768,333,132,8,0,643,2,40,1,4,2,44,1,132,2,44,1,132,2,41,
1,1000,300,331,3,2,91,1,58,2,93,
1,32768,325,350,3,2,91,1,86,2,93,
1,32768,334,350,6,1,350,2,46,0,1019,2,40,1,331,2,41,
1,0,18,428,4,1,429,2,61,2,61,1,429,
1,0,19,428,4,1,429,2,33,2,61,1,429,
1,0,30,428,3,1,429,2,60,1,429,
1,0,31,428,4,1,429,2,60,2,61,1,429,
1,0,32,428,3,1,429,2,62,1,429,
1,0,33,428,4,1,429,2,62,2,61,1,429,
1,0,18,428,6,0,1238,2,40,1,429,2,44,1,429,2,41,
1,0,19,428,6,0,1348,2,40,1,429,2,44,1,429,2,41,
1,0,30,428,6,0,1463,2,40,1,429,2,44,1,429,2,41,
1,0,31,428,6,0,1677,2,40,1,429,2,44,1,429,2,41,
1,0,32,428,6,0,1770,2,40,1,429,2,44,1,429,2,41,
1,0,33,428,6,0,1984,2,40,1,429,2,44,1,429,2,41,
1,0,18,428,4,1,4,2,61,2,61,1,4,
1,0,19,428,4,1,4,2,33,2,61,1,4,
1,0,30,428,3,1,4,2,60,1,4,
1,0,31,428,4,1,4,2,60,2,61,1,4,
1,0,32,428,3,1,4,2,62,1,4,
1,0,33,428,4,1,4,2,62,2,61,1,4,
1,0,18,428,14,0,871,2,91,0,543,2,44,0,655,2,91,0,543,2,93,2,93,2,40,1,4,2,44,1,4,2,41,
1,0,19,428,14,0,981,2,91,0,543,2,44,0,655,2,91,0,543,2,93,2,93,2,40,1,4,2,44,1,4,2,41,
1,0,30,428,14,0,1096,2,91,0,543,2,44,0,655,2,91,0,543,2,93,2,93,2,40,1,4,2,44,1,4,2,41,
1,0,31,428,14,0,1310,2,91,0,543,2,44,0,655,2,91,0,543,2,93,2,93,2,40,1,4,2,44,1,4,2,41,
1,0,32,428,14,0,1403,2,91,0,543,2,44,0,655,2,91,0,543,2,93,2,93,2,40,1,4,2,44,1,4,2,41,
1,0,33,428,14,0,1617,2,91,0,543,2,44,0,655,2,91,0,543,2,93,2,93,2,40,1,4,2,44,1,4,2,41,
1,0,18,428,4,1,43,2,61,2,61,1,43,
1,0,19,428,4,1,43,2,33,2,61,1,43,
1,0,30,428,3,1,43,2,60,1,43,
1,0,31,428,4,1,43,2,60,2,61,1,43,
1,0,32,428,3,1,43,2,62,1,43,
1,0,33,428,4,1,43,2,62,2,61,1,43,
1,0,18,428,6,0,1274,2,40,1,43,2,44,1,43,2,41,
1,0,19,428,6,0,1173,2,40,1,43,2,44,1,43,2,41,
1,0,30,428,6,0,1077,2,40,1,43,2,44,1,43,2,41,
1,0,31,428,6,0,1291,2,40,1,43,2,44,1,43,2,41,
1,0,32,428,6,0,1384,2,40,1,43,2,44,1,43,2,41,
1,0,33,428,6,0,1598,2,40,1,43,2,44,1,43,2,41,
1,32768,414,428,6,0,1580,2,40,1,3,2,44,1,12,2,41,
1,32768,415,428,6,0,1965,2,40,1,3,2,44,1,429,2,41,
1,32768,416,428,6,0,1432,2,40,1,12,2,44,1,12,2,41,
1,0,18,428,4,1,59,2,61,2,61,1,59,
1,0,19,428,4,1,59,2,33,2,61,1,59,
1,0,30,428,3,1,59,2,60,1,59,
1,0,31,428,4,1,59,2,60,2,61,1,59,
1,0,32,428,3,1,59,2,62,1,59,
1,0,33,428,4,1,59,2,62,2,61,1,59,
1,0,18,428,6,0,1368,2,40,1,59,2,44,1,59,2,41,
1,0,19,428,6,0,1478,2,40,1,59,2,44,1,59,2,41,
1,0,30,428,6,0,1593,2,40,1,59,2,44,1,59,2,41,
1,0,31,428,6,0,1807,2,40,1,59,2,44,1,59,2,41,
1,0,32,428,6,0,1900,2,40,1,59,2,44,1,59,2,41,
1,0,33,428,6,0,2114,2,40,1,59,2,44,1,59,2,41,
1,32768,418,429,4,0,1062,1,496,0,221,1,429,
1,36768,68,32,1,1,220,
1,36768,67,58,1,1,19,
1,36768,79,58,2,2,45,1,19,
0};
int pos = 4022;
int earleyQuerySort = 43;
int earleyQueryStrategy = 472;
char *tabIdentStr[] = {
"less_int",
"th",
"neq_list",
"part",
"tone",
"dont",
"proc",
"cons",
"orig",
"dest",
"neq_identifier",
"then",
"neq_builtinInt",
"btoi",
"greater_identifier",
"module",
"assocRight",
"anyInteger",
"update",
"time",
"ln",
"repeat",
"eq_int",
"updateMemo",
"END",
"eq",
"implicit",
"of",
"dkconcur",
"AND",
"btoi_int",
"length",
"greater_builtinInt",
"tall",
"choose",
"extractrule2",
"bool",
"ccst",
"extractrule1",
"inline",
"self",
"go",
"internBuiltinArray",
"bs",
"public",
"less_labelname",
"buildRccst",
"t21",
"t11",
"append",
"lesseq_labelname",
"dk",
"id",
"dcconcur",
"ee",
"neq_bool",
"calc_str",
"elem",
"firstOne",
"if",
"eq_ident",
"find",
"fail",
"code",
"dc",
"global",
"call",
"care",
"eq_labelname",
"handline",
"case",
"ccat",
"getRccst",
"calc",
"dkcall",
"cleanIndex",
"P27",
"P17",
"dccall",
"P16",
"P15",
"P14",
"P13",
"hashTerm",
"P26",
"P25",
"P24",
"P23",
"P22",
"P21",
"c",
"b",
"a",
"procId",
"isInverseLabel",
"P12",
"P11",
"greatereq_identifier",
"greatereq_builtinInt",
"resId_",
"X",
"enter2",
"v2",
"enter1",
"R",
"v1",
"t2",
"t1",
"req2",
"req1",
"par3",
"par2",
"par1",
"L",
"n1",
"l2",
"k2",
"HT",
"cleanAll",
"getLabelCcst",
"getHashTable",
"l1",
"lesseq_list",
"k1",
"greater_entry",
"nil_HT",
"greatereq_entry",
"IF",
"normout",
"description",
"stratop",
"ANDIF",
"start_str",
"try",
"operators",
"query",
"entry",
"AC",
"P2",
"lesseq_bool",
"listExtract",
"KBig",
"neq_rccst",
"greater_int",
"str",
"size_of_rccst",
"otherwise",
"greatereq_int",
"greater_rccst",
"nil_rccst",
"neq_entry",
"start",
"minus",
"rules",
"spart",
"P1",
"tsome",
"greatereq_rccst",
"apply",
"K2",
"umin_builtinInt",
"K1",
"less_identifier",
"less_builtinInt",
"first",
"not",
"containsKey",
"lesseq_identifier",
"less_list",
"lesseq_builtinInt",
"normalize",
"set",
"pri",
"specification",
"new",
"private",
"oneconcur",
"newln",
"normalise",
"rccst",
"int",
"v",
"tau",
"C2",
"t",
"builtin",
"btoi_builtinInt",
"q",
"p",
"doesNotRestrict",
"n",
"act",
"c2r0",
"eq_list",
"res",
"size_of",
"for",
"iterate",
"itob_builtinInt",
"neq",
"div",
"one",
"eq_identifier",
"eq_builtinInt",
"cmp",
"less_bool",
"where",
"neq_int",
"C1",
"greatereq",
"nil",
"ident",
"l",
"mod",
"k",
"i",
"e",
"greater",
"d",
"rel",
"get",
"c2w2",
"c1w2",
"c1w1",
"c1w0",
"c2r2",
"c1r2",
"end",
"c1r0",
"anyIdentifier",
"newList",
"neq_ident",
"c2w1",
"c2w0",
"local",
"alias",
"eq_bool",
"false",
"Knuth",
"SUCH",
"Epsilon",
"THAT",
"neq_labelname",
"greater_labelname",
"c2r1",
"c1r1",
"and",
"hcode",
"greatereq_labelname",
"label",
"check",
"builtinHashTerm",
"Key",
"META",
"valueOf",
"declare",
"assocLeft",
"Value",
"add",
"resId",
"plus2",
"plus1",
"defined",
"checkMemo",
"getCcst",
"labelname",
"relId",
"exit2",
"doesNotRestrictName",
"hashTableList",
"exit1",
"kw2",
"kw1",
"EACH",
"ht4",
"kr2",
"ht2",
"dcOne",
"ht3",
"lesseq_entry",
"memo3",
"less_entry",
"resId__",
"ht1",
"kr1",
"start_rule",
"ln2",
"ln1",
"C2_2",
"C1_2",
"C1_1",
"hashTable",
"emptyKeyHashTable",
"hardAlias",
"C2_1",
"C2_0",
"updateHashTable",
"C1_0",
"strategies",
"definedAs",
"lesseq_rccst",
"lesseq_int",
"less_rccst",
"export",
"result",
"greater_list",
"strategy",
"sort",
"size_of_rccst_list",
"greatereq_list",
"plus",
"import",
"itob_int",
"itob",
"appl",
"LPL",
"queryend",
"FOR",
"eq_rccst",
"true",
"eq_entry",
"know",
"builtinArray",
"with",
"size",
"umin",
"term",
"normin",
"greatereq_bool",
"switch",
"identifier",
"source",
"builtinInt",
"list",
"lesseq",
"rule",
"less",
"or",
"greater_bool",
"explicit",
"on",
""};
int tabIdentIndex[] = {
865,
220,
863,
439,
438,
437,
436,
435,
433,
432,
1478,
431,
1477,
430,
1900,
646,
1047,
1046,
643,
642,
218,
641,
640,
1041,
215,
214,
859,
213,
857,
211,
856,
853,
1899,
429,
852,
1253,
428,
851,
1252,
850,
426,
425,
1894,
424,
639,
1463,
1039,
637,
636,
632,
1677,
207,
205,
849,
202,
847,
843,
419,
842,
418,
841,
417,
412,
411,
199,
625,
623,
622,
1238,
835,
834,
833,
831,
403,
619,
1019,
185,
184,
611,
183,
182,
181,
180,
828,
395,
394,
393,
392,
391,
390,
99,
98,
97,
609,
1432,
179,
178,
2114,
2113,
598,
88,
592,
168,
591,
82,
167,
166,
165,
378,
377,
374,
373,
372,
76,
159,
158,
157,
156,
796,
1197,
1196,
368,
1192,
367,
1403,
574,
1617,
143,
788,
1188,
781,
354,
998,
351,
991,
566,
562,
132,
130,
1176,
1175,
349,
1173,
1172,
345,
1389,
986,
1386,
1384,
1383,
981,
558,
556,
555,
554,
129,
552,
1598,
550,
125,
1594,
124,
1593,
1592,
763,
337,
1160,
1807,
978,
1806,
977,
332,
331,
1377,
330,
974,
972,
548,
970,
543,
542,
118,
541,
117,
116,
759,
1583,
113,
112,
1580,
110,
1156,
1155,
753,
752,
751,
327,
750,
1794,
324,
323,
322,
1368,
1367,
320,
962,
539,
961,
538,
960,
534,
532,
108,
531,
107,
105,
101,
746,
100,
745,
742,
318,
317,
316,
315,
313,
312,
311,
310,
1355,
953,
951,
528,
527,
523,
522,
737,
734,
733,
307,
730,
305,
1348,
1770,
945,
944,
518,
515,
1984,
512,
510,
1555,
297,
295,
722,
720,
932,
509,
508,
503,
502,
501,
719,
1119,
717,
929,
496,
492,
1965,
1320,
491,
276,
275,
273,
272,
271,
270,
489,
482,
1310,
481,
1096,
693,
269,
692,
1093,
268,
267,
262,
261,
260,
908,
1732,
905,
472,
471,
1519,
259,
1083,
899,
1291,
1079,
1077,
674,
671,
1285,
883,
456,
1928,
1499,
452,
667,
1067,
1063,
1062,
232,
877,
231,
1274,
448,
871,
447,
1270,
444,
443,
441,
440,
659,
1483,
658,
1059,
657,
1058,
655,
653,
651,
650,
225,
1269,
866,
221,
0};
int tabIdentSize = 341;
char *tabSortStr[] = {
"procId",
"relId",
"ccst",
"ident",
"builtinArray[list[entry[rccst,list[rccst]]]]",
"intern string",
"<rccst->rccst>",
"hashTable[rccst,list[rccst]]",
"rccst",
"entry[rccst,list[rccst]]",
"labelname",
"resId",
"bool",
"identifier",
"internBuiltinArray[list[entry[rccst,list[rccst]]]]",
"builtinInt",
"string",
"intern int",
"intern ident",
"label",
"list[rccst]",
"int",
"list[entry[rccst,list[rccst]]]",
""};
int tabSortIndex[] = {
109,
496,
140,
32,
86,
351,
315,
350,
43,
4,
429,
3,
428,
59,
210,
58,
163,
19,
220,
12,
171,
331,
132,
0};
int tabSortSize = 23;
char *tabStrategyStr[] = {
"listExtract:rccst/list[rccst]",
"start_str:rccst/memo3",
"calc_str:rccst/memo3",
""};
int tabStrategyIndex[] = {
494,
127,
472,
0};
int tabStrategySize = 3;
