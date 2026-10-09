#ifndef __main_header_h
#define __main_header_h
#include "main_skeleton_lib.h"

/* Codes */
#define code_358 358
#define code_357 357
#define code_356 356
#define code_355 355
#define code_354 354
#define code_353 353
#define code_352 352
#define code_79 79
#define code_351 351
#define code_350 350
#define code_349 349
#define code_348 348
#define code_347 347
#define code_346 346
#define code_345 345
#define code_344 344
#define code_343 343
#define code_342 342
#define code_341 341
#define code_68 68
#define code_340 340
#define code_67 67
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
#define code_210 210
#define code_421 421
#define code_420 420
#define code_319 319
#define code_318 318
#define code_317 317
#define code_316 316
#define code_315 315
#define code_314 314
#define code_313 313
#define code_312 312
#define code_311 311
#define code_310 310
#define code_33 33
#define code_32 32
#define code_31 31
#define code_203 203
#define code_30 30
#define code_202 202
#define code_201 201
#define code_200 200
#define code_419 419
#define code_418 418
#define code_417 417
#define code_416 416
#define code_415 415
#define code_414 414
#define code_413 413
#define code_412 412
#define code_411 411
#define code_410 410
#define code_309 309
#define code_308 308
#define code_307 307
#define code_306 306
#define code_305 305
#define code_304 304
#define code_303 303
#define code_302 302
#define code_29 29
#define code_301 301
#define code_28 28
#define code_300 300
#define code_27 27
#define code_26 26
#define code_25 25
#define code_24 24
#define code_22 22
#define code_21 21
#define code_20 20
#define code_409 409
#define code_408 408
#define code_407 407
#define code_406 406
#define code_405 405
#define code_404 404
#define code_403 403
#define code_402 402
#define code_401 401
#define code_400 400
#define code_19 19
#define code_18 18
#define code_15 15
#define code_14 14
#define code_13 13
#define code_12 12
#define code_11 11
#define code_399 399
#define code_10 10
#define code_398 398
#define code_397 397
#define code_396 396
#define code_395 395
#define code_394 394
#define code_393 393
#define code_392 392
#define code_391 391
#define code_390 390
#define code_389 389
#define code_388 388
#define code_387 387
#define code_386 386
#define code_385 385
#define code_384 384
#define code_383 383
#define code_382 382
#define code_381 381
#define code_380 380
#define code_379 379
#define code_378 378
#define code_377 377
#define code_376 376
#define code_375 375
#define code_374 374
#define code_373 373
#define code_372 372
#define code_371 371
#define code_370 370
#define code_369 369
#define code_368 368
#define code_367 367
#define code_366 366
#define code_365 365
#define code_364 364
#define code_363 363
#define code_362 362
#define code_361 361
#define code_360 360
#define code_9 9
#define code_8 8
#define code_6 6
#define code_5 5
#define code_4 4
#define code_3 3
#define code_1 1
#define code_0 0
#define code_359 359

/* Structures */
TERMSTR(term3,3);

/* Constantes d'execution */
#define FSYM_TAB_SIZE 422
extern int fsymtabSize;
/* 0: no trace, 1: result, 2: start with */
extern unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];

/* Entetes */
extern struct term* fun_357(struct term *v1);
extern struct term* fun_356(struct term *v1);
extern struct term* fun_352(struct term *v1);
extern struct term *con_348;
extern struct term *con_344;
extern struct term* fun_343(struct term *v1,struct term *v2,struct term *v3);
extern struct term *con_342;
extern struct term* fun_341(struct term *v1,struct term *v2);
extern struct term* fun_340(struct term *v1);
extern struct term* fun_339(struct term *v1,struct term *v2);
extern struct term* fun_337(struct term *v1,struct term *v2);
extern struct term *con_335;
extern struct term* fun_334(struct term *v1,struct term *v2);
extern struct term* fun_333(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_332(struct term *v1,struct term *v2);
extern struct term* fun_331(struct term *v1,struct term *v2);
extern struct term *con_328;
extern struct term *con_327;
extern struct term* fun_324(struct term *v1);
extern struct term* fun_323(struct term *v1);
extern struct term* fun_322(struct term *v1,struct term *v2);
extern struct term* fun_321(struct term *v1,struct term *v2);
extern struct term* fun_320(struct term *v1,struct term *v2,struct term *v3);
extern struct term *con_420;
extern struct term* fun_319(struct term *v1);
extern struct term* fun_318(struct term *v1,struct term *v2);
extern struct term* fun_317(struct term *v1);
extern struct term* fun_316(struct term *v1);
extern struct term* fun_315(struct term *v1);
extern struct term* fun_314(struct term *v1,struct term *v2);
extern struct term* fun_313(struct term *v1,struct term *v2);
extern struct term* fun_312(struct term *v1,struct term *v2);
extern struct term* fun_311(struct term *v1,struct term *v2);
extern struct term* fun_310(struct term *v1,struct term *v2);
extern struct term *con_419;
extern struct term* fun_418(struct term *v1,struct term *v2);
extern struct term* fun_417(struct term *v1,struct term *v2);
extern struct term* fun_416(struct term *v1,struct term *v2);
extern struct term* fun_415(struct term *v1,struct term *v2);
extern struct term* fun_414(struct term *v1,struct term *v2);
extern struct term *con_413;
extern struct term *con_412;
extern struct term *con_411;
extern struct term *con_410;
extern struct term* fun_309(struct term *v1,struct term *v2);
extern struct term* fun_308(struct term *v1);
extern struct term* fun_307(struct term *v1,struct term *v2);
extern struct term* fun_306(struct term *v1,struct term *v2);
extern struct term* fun_305(struct term *v1,struct term *v2);
extern struct term* fun_304(struct term *v1,struct term *v2);
extern struct term* fun_303(struct term *v1,struct term *v2);
extern struct term* fun_302(struct term *v1,struct term *v2);
extern struct term* fun_301(struct term *v1,struct term *v2);
extern struct term *con_409;
extern struct term *con_408;
extern struct term *con_407;
extern struct term *con_406;
extern struct term *con_405;
extern struct term *con_404;
extern struct term *con_403;
extern struct term *con_402;
extern struct term *con_401;
extern struct term *con_400;
extern struct term *con_399;
extern struct term *con_398;
extern struct term *con_397;
extern struct term *con_396;
extern struct term *con_395;
extern struct term *con_394;
extern struct term *con_393;
extern struct term *con_392;
extern struct term *con_391;
extern struct term *con_390;
extern struct term *con_389;
extern struct term *con_388;
extern struct term *con_387;
extern struct term *con_386;
extern struct term *con_385;
extern struct term *con_384;
extern struct term *con_383;
extern struct term *con_382;
extern struct term *con_381;
extern struct term *con_380;
extern struct term *con_379;
extern struct term *con_378;
extern struct term *con_377;
extern struct term *con_376;
extern struct term *con_375;
extern struct term *con_374;
extern struct term *con_373;
extern struct term *con_372;
extern struct term *con_371;
extern struct term *con_370;
extern struct term *con_369;
extern struct term *con_368;
extern struct term *con_367;
extern struct term *con_366;
extern struct term *con_365;
extern struct term* fun_364();
extern struct term* fun_362(struct term *v1,struct term *v2);
extern struct term* fun_361(struct term *v1);
extern struct term* fun_360(struct term *v1);
extern struct term *con_1;
extern struct term *con_0;

/* Entetes */
extern struct term *str_472(struct term *t);
extern struct term *str_494(struct term *t);
extern struct term *str_127(struct term *t);
#endif
