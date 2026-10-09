#ifndef __main_header_h
#define __main_header_h
#include "main_skeleton.h"

/* Codes */
#define code_19 19
#define code_18 18
#define code_15 15
#define code_14 14
#define code_13 13
#define code_12 12
#define code_11 11
#define code_68 68
#define code_319 319
#define code_10 10
#define code_67 67
#define code_318 318
#define code_317 317
#define code_316 316
#define code_315 315
#define code_314 314
#define code_313 313
#define code_312 312
#define code_311 311
#define code_310 310
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
#define code_9 9
#define code_8 8
#define code_6 6
#define code_5 5
#define code_4 4
#define code_3 3
#define code_1 1
#define code_0 0
#define code_341 341
#define code_340 340
#define code_33 33
#define code_32 32
#define code_31 31
#define code_30 30
#define code_339 339
#define code_338 338
#define code_337 337
#define code_336 336
#define code_335 335
#define code_334 334
#define code_333 333
#define code_332 332
#define code_331 331
#define code_330 330
#define code_29 29
#define code_28 28
#define code_27 27
#define code_26 26
#define code_25 25
#define code_24 24
#define code_22 22
#define code_79 79
#define code_21 21
#define code_20 20
#define code_329 329
#define code_328 328
#define code_327 327
#define code_326 326
#define code_325 325
#define code_324 324
#define code_323 323
#define code_322 322
#define code_321 321
#define code_320 320

/* Structures */

/* Constantes d'execution */
#define FSYM_TAB_SIZE 342
extern int fsymtabSize;
/* 0: no trace, 1: result, 2: start with */
extern unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];

/* Entetes */
extern struct term *con_319;
extern struct term* fun_318(struct term *v1);
extern struct term* fun_317(struct term *v1);
extern struct term* fun_316(struct term *v1);
extern struct term* fun_315(struct term *v1,struct term *v2);
extern struct term* fun_314(struct term *v1,struct term *v2);
extern struct term* fun_313(struct term *v1,struct term *v2);
extern struct term* fun_312(struct term *v1,struct term *v2);
extern struct term* fun_311(struct term *v1,struct term *v2);
extern struct term* fun_310(struct term *v1,struct term *v2);
extern struct term* fun_309(struct term *v1);
extern struct term* fun_308(struct term *v1,struct term *v2);
extern struct term* fun_307(struct term *v1,struct term *v2);
extern struct term* fun_306(struct term *v1,struct term *v2);
extern struct term* fun_305(struct term *v1,struct term *v2);
extern struct term* fun_304(struct term *v1,struct term *v2);
extern struct term* fun_303(struct term *v1,struct term *v2);
extern struct term* fun_302(struct term *v1,struct term *v2);
extern struct term *con_1;
extern struct term *con_0;
extern struct term *con_341;
extern struct term *con_340;
extern struct term *con_339;
extern struct term *con_338;
extern struct term *con_337;
extern struct term* fun_335(struct term *v1,struct term *v2);
extern struct term *fun_334(struct term *t);
extern struct term *fun_333(struct term *t);
extern struct term *con_330;
extern struct term *con_329;
extern struct term *con_328;
extern struct term* fun_327();
extern struct term *con_326;
extern struct term* fun_325(struct term *v1,struct term *v2);
extern struct term* fun_324(struct term *v1);
extern struct term* fun_323(struct term *v1,struct term *v2);
extern struct term* fun_321(struct term *v1,struct term *v2);

/* Entetes */
extern struct term *str_26(struct term *t);
extern struct term *str_413(struct term *t);
extern struct term *str_240(struct term *t);
extern struct term *str_35(struct term *t);
extern struct term *str_14(struct term *t);
extern struct term *str_341(struct term *t);
#endif
