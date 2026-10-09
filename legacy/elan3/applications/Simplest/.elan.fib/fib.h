#ifndef __main_header_h
#define __main_header_h
#include "main_skeleton.h"

/* Codes */
#define code_310 310
#define code_13 13
#define code_12 12
#define code_11 11
#define code_67 67
#define code_10 10
#define code_9 9
#define code_8 8
#define code_6 6
#define code_5 5
#define code_4 4
#define code_3 3
#define code_1 1
#define code_0 0
#define code_309 309
#define code_308 308
#define code_307 307
#define code_306 306
#define code_305 305
#define code_304 304
#define code_303 303
#define code_302 302
#define code_301 301
#define code_300 300
#define code_29 29
#define code_28 28
#define code_27 27
#define code_26 26
#define code_25 25
#define code_24 24
#define code_79 79
#define code_22 22
#define code_21 21
#define code_20 20
#define code_318 318
#define code_317 317
#define code_316 316
#define code_315 315
#define code_314 314
#define code_313 313
#define code_312 312
#define code_311 311

/* Structures */

/* Constantes d'execution */
#define FSYM_TAB_SIZE 319
extern int fsymtabSize;
/* 0: no trace, 1: result, 2: start with */
extern unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];

/* Entetes */
extern Gterm* fun_310(Gterm *v1,Gterm *v2);
extern Gterm *con_1;
extern Gterm *con_0;
extern Gterm* fun_309(Gterm *v1,Gterm *v2);
extern Gterm* fun_308(Gterm *v1);
extern Gterm* fun_307(Gterm *v1,Gterm *v2);
extern Gterm* fun_306(Gterm *v1,Gterm *v2);
extern Gterm* fun_305(Gterm *v1,Gterm *v2);
extern Gterm* fun_304(Gterm *v1,Gterm *v2);
extern Gterm* fun_303(Gterm *v1,Gterm *v2);
extern Gterm* fun_302(Gterm *v1,Gterm *v2);
extern Gterm* fun_301(Gterm *v1,Gterm *v2);
extern Gterm* fun_318(Gterm *v1);
extern Gterm* fun_317(Gterm *v1);
extern Gterm* fun_316(Gterm *v1);
extern Gterm* fun_315(Gterm *v1);
extern Gterm* fun_314(Gterm *v1,Gterm *v2);
extern Gterm* fun_313(Gterm *v1,Gterm *v2);
extern Gterm* fun_312(Gterm *v1,Gterm *v2);
extern Gterm* fun_311(Gterm *v1,Gterm *v2);

/* Entetes */
#endif
