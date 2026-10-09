#ifndef __main_header_h
#define __main_header_h
#include "main_skeleton.h"

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
#define code_459 459
#define code_458 458
#define code_457 457
#define code_456 456
#define code_455 455
#define code_454 454
#define code_453 453
#define code_452 452
#define code_451 451
#define code_450 450
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
#define code_449 449
#define code_448 448
#define code_447 447
#define code_446 446
#define code_445 445
#define code_444 444
#define code_443 443
#define code_442 442
#define code_441 441
#define code_440 440
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
#define code_439 439
#define code_438 438
#define code_437 437
#define code_436 436
#define code_435 435
#define code_434 434
#define code_433 433
#define code_432 432
#define code_431 431
#define code_430 430
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
#define code_429 429
#define code_428 428
#define code_427 427
#define code_426 426
#define code_425 425
#define code_424 424
#define code_423 423
#define code_422 422
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
#define code_30 30
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
#define code_509 509
#define code_18 18
#define code_17 17
#define code_508 508
#define code_16 16
#define code_507 507
#define code_506 506
#define code_15 15
#define code_505 505
#define code_14 14
#define code_504 504
#define code_13 13
#define code_503 503
#define code_12 12
#define code_502 502
#define code_11 11
#define code_399 399
#define code_501 501
#define code_10 10
#define code_398 398
#define code_500 500
#define code_397 397
#define code_396 396
#define code_395 395
#define code_394 394
#define code_393 393
#define code_392 392
#define code_391 391
#define code_390 390
#define code_499 499
#define code_498 498
#define code_497 497
#define code_496 496
#define code_495 495
#define code_494 494
#define code_493 493
#define code_492 492
#define code_491 491
#define code_490 490
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
#define code_489 489
#define code_488 488
#define code_487 487
#define code_486 486
#define code_485 485
#define code_484 484
#define code_483 483
#define code_482 482
#define code_481 481
#define code_480 480
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
#define code_479 479
#define code_478 478
#define code_477 477
#define code_476 476
#define code_475 475
#define code_474 474
#define code_473 473
#define code_472 472
#define code_471 471
#define code_470 470
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
#define code_469 469
#define code_468 468
#define code_467 467
#define code_466 466
#define code_465 465
#define code_464 464
#define code_463 463
#define code_462 462
#define code_461 461
#define code_460 460
#define code_359 359

/* Structures */

/* Constantes d'execution */
#define FSYM_TAB_SIZE 510
extern int fsymtabSize;
/* 0: no trace, 1: result, 2: start with */
extern unsigned long tab_rewrite_step[2][FSYM_TAB_SIZE];

/* Entetes */
extern Gterm* fun_358(Gterm *v1);
extern Gterm* fun_357(Gterm *v1,Gterm *v2);
extern Gterm* fun_355(Gterm *v1,Gterm *v2);
extern Gterm *con_353;
extern Gterm *con_352;
extern Gterm* fun_351(Gterm *v1,Gterm *v2);
extern Gterm* fun_350(Gterm *v1);
extern Gterm *con_458;
extern Gterm *con_457;
extern Gterm *con_456;
extern Gterm *con_455;
extern Gterm* fun_454(Gterm *v1,Gterm *v2,Gterm *v3);
extern Gterm* fun_453(Gterm *v1,Gterm *v2);
extern Gterm* fun_452();
extern Gterm *con_451;
extern Gterm* fun_450(Gterm *v1,Gterm *v2);
extern Gterm* fun_349(Gterm *v1,Gterm *v2);
extern Gterm* fun_347(Gterm *v1,Gterm *v2);
extern Gterm *con_345;
extern Gterm *con_342;
extern Gterm* fun_341();
extern Gterm* fun_340();
extern Gterm* fun_449(Gterm *v1);
extern Gterm* fun_448(Gterm *v1,Gterm *v2);
extern Gterm* fun_446(Gterm *v1,Gterm *v2);
extern Gterm *con_444;
extern Gterm *con_443;
extern Gterm* fun_441(Gterm *v1);
extern Gterm* fun_440(Gterm *v1,Gterm *v2);
extern Gterm* fun_339();
extern Gterm *con_338;
extern Gterm* fun_337(Gterm *v1,Gterm *v2);
extern Gterm* fun_336(Gterm *v1);
extern Gterm* fun_335(Gterm *v1,Gterm *v2);
extern Gterm* fun_333(Gterm *v1,Gterm *v2);
extern Gterm *con_331;
extern Gterm* fun_330(Gterm *v1);
extern Gterm* fun_439(Gterm *v1);
extern Gterm* fun_438(Gterm *v1);
extern Gterm* fun_437(Gterm *v1,Gterm *v2);
extern Gterm* fun_436(Gterm *v1,Gterm *v2);
extern Gterm* fun_435(Gterm *v1,Gterm *v2);
extern Gterm* fun_434(Gterm *v1,Gterm *v2);
extern Gterm* fun_433(Gterm *v1);
extern Gterm* fun_431(Gterm *v1);
extern Gterm* fun_430(Gterm *v1);
extern Gterm* fun_329(Gterm *v1);
extern Gterm* fun_328(Gterm *v1);
extern Gterm *con_326;
extern Gterm* fun_325(Gterm *v1,Gterm *v2);
extern Gterm* fun_324(Gterm *v1);
extern Gterm* fun_323(Gterm *v1,Gterm *v2);
extern Gterm* fun_321(Gterm *v1,Gterm *v2);
extern Gterm *con_428;
extern Gterm* fun_427(Gterm *v1);
extern Gterm* fun_422(Gterm *v1,Gterm *v2);
extern Gterm* fun_421(Gterm *v1);
extern Gterm* fun_420(Gterm *v1);
extern Gterm *con_319;
extern Gterm* fun_318(Gterm *v1);
extern Gterm* fun_317(Gterm *v1);
extern Gterm* fun_316(Gterm *v1);
extern Gterm* fun_315(Gterm *v1,Gterm *v2);
extern Gterm* fun_314(Gterm *v1,Gterm *v2);
extern Gterm* fun_313(Gterm *v1,Gterm *v2);
extern Gterm* fun_312(Gterm *v1,Gterm *v2);
extern Gterm* fun_311(Gterm *v1,Gterm *v2);
extern Gterm* fun_310(Gterm *v1,Gterm *v2);
extern Gterm* fun_419(Gterm *v1);
extern Gterm* fun_417(Gterm *v1,Gterm *v2);
extern Gterm* fun_416(Gterm *v1,Gterm *v2);
extern Gterm* fun_415(Gterm *v1,Gterm *v2);
extern Gterm* fun_414(Gterm *v1,Gterm *v2);
extern Gterm* fun_413(Gterm *v1,Gterm *v2);
extern Gterm* fun_412(Gterm *v1,Gterm *v2);
extern Gterm* fun_411(Gterm *v1,Gterm *v2);
extern Gterm* fun_410(Gterm *v1);
extern Gterm* fun_309(Gterm *v1);
extern Gterm* fun_308(Gterm *v1,Gterm *v2);
extern Gterm* fun_307(Gterm *v1,Gterm *v2);
extern Gterm* fun_306(Gterm *v1,Gterm *v2);
extern Gterm* fun_305(Gterm *v1,Gterm *v2);
extern Gterm* fun_304(Gterm *v1,Gterm *v2);
extern Gterm* fun_303(Gterm *v1,Gterm *v2);
extern Gterm* fun_302(Gterm *v1,Gterm *v2);
extern Gterm* fun_409(Gterm *v1,Gterm *v2);
extern Gterm* fun_408(Gterm *v1,Gterm *v2);
extern Gterm* fun_407(Gterm *v1,Gterm *v2);
extern Gterm* fun_406(Gterm *v1);
extern Gterm* fun_405(Gterm *v1);
extern Gterm* fun_404(Gterm *v1);
extern Gterm* fun_403(Gterm *v1);
extern Gterm* fun_402(Gterm *v1);
extern Gterm* fun_401(Gterm *v1);
extern Gterm* fun_400(Gterm *v1,Gterm *v2);
extern Gterm* fun_509(Gterm *v1);
extern Gterm *con_508;
extern Gterm *con_507;
extern Gterm *con_506;
extern Gterm *con_505;
extern Gterm *con_504;
extern Gterm *con_503;
extern Gterm *con_502;
extern Gterm* fun_399(Gterm *v1,Gterm *v2);
extern Gterm *con_501;
extern Gterm* fun_398(Gterm *v1);
extern Gterm *con_500;
extern Gterm *con_397;
extern Gterm* fun_396(Gterm *v1,Gterm *v2);
extern Gterm* fun_395(Gterm *v1);
extern Gterm* fun_394(Gterm *v1,Gterm *v2);
extern Gterm* fun_392(Gterm *v1,Gterm *v2);
extern Gterm *con_390;
extern Gterm *con_499;
extern Gterm *con_498;
extern Gterm *con_497;
extern Gterm *con_496;
extern Gterm* fun_495(Gterm *v1,Gterm *v2,Gterm *v3,Gterm *v4);
extern Gterm* fun_389(Gterm *v1);
extern Gterm* fun_388(Gterm *v1,Gterm *v2);
extern Gterm* fun_387(Gterm *v1,Gterm *v2);
extern Gterm *con_386;
extern Gterm* fun_385();
extern Gterm* fun_384();
extern Gterm *con_383;
extern Gterm *con_382;
extern Gterm *con_381;
extern Gterm* fun_487();
extern Gterm* fun_486(Gterm *v1);
extern Gterm* fun_485(Gterm *v1);
extern Gterm* fun_484(Gterm *v1);
extern Gterm* fun_481(Gterm *v1,Gterm *v2);
extern Gterm* fun_480(Gterm *v1,Gterm *v2);
extern Gterm *con_374;
extern Gterm *con_373;
extern Gterm *con_372;
extern Gterm *con_371;
extern Gterm *con_370;
extern Gterm *con_479;
extern Gterm *con_478;
extern Gterm* fun_477(Gterm *v1);
extern Gterm* fun_476(Gterm *v1);
extern Gterm* fun_475(Gterm *v1,Gterm *v2);
extern Gterm* fun_474(Gterm *v1,Gterm *v2);
extern Gterm* fun_473(Gterm *v1,Gterm *v2);
extern Gterm* fun_472(Gterm *v1);
extern Gterm* fun_471(Gterm *v1);
extern Gterm* fun_470(Gterm *v1);
extern Gterm* fun_369(Gterm *v1,Gterm *v2);
extern Gterm* fun_368(Gterm *v1,Gterm *v2,Gterm *v3);
extern Gterm* fun_367(Gterm *v1,Gterm *v2);
extern Gterm* fun_365(Gterm *v1);
extern Gterm* fun_364(Gterm *v1,Gterm *v2);
extern Gterm* fun_363(Gterm *v1);
extern Gterm *con_360;
extern Gterm *con_1;
extern Gterm *con_0;
extern Gterm* fun_469(Gterm *v1);
extern Gterm* fun_468(Gterm *v1);
extern Gterm* fun_467(Gterm *v1);
extern Gterm* fun_466(Gterm *v1);
extern Gterm* fun_465(Gterm *v1);
extern Gterm* fun_464(Gterm *v1);
extern Gterm* fun_463(Gterm *v1);
extern Gterm* fun_460(Gterm *v1);
extern Gterm* fun_359(Gterm *v1,Gterm *v2);

/* Entetes */
extern Gterm *str_9(Gterm *t);
extern Gterm *str_151(Gterm *t);
extern Gterm *str_24(Gterm *t);
extern Gterm *str_25(Gterm *t);
extern Gterm *str_237(Gterm *t);
extern Gterm *str_27(Gterm *t);
extern Gterm *str_28(Gterm *t);
extern Gterm *str_29(Gterm *t);
extern Gterm *str_100(Gterm *t);
extern Gterm *str_101(Gterm *t);
extern Gterm *str_102(Gterm *t);
extern Gterm *str_10(Gterm *t);
extern Gterm *str_152(Gterm *t);
extern Gterm *str_153(Gterm *t);
extern Gterm *str_11(Gterm *t);
extern Gterm *str_12(Gterm *t);
extern Gterm *str_13(Gterm *t);
extern Gterm *str_14(Gterm *t);
extern Gterm *str_15(Gterm *t);
extern Gterm *str_16(Gterm *t);
extern Gterm *str_17(Gterm *t);
extern Gterm *str_18(Gterm *t);
extern Gterm *str_242(Gterm *t);
extern Gterm *str_470(Gterm *t);
extern Gterm *str_464(Gterm *t);
extern Gterm *str_369(Gterm *t);
extern Gterm *str_148(Gterm *t);
extern Gterm *str_26(Gterm *t);
extern Gterm *str_70(Gterm *t);
extern Gterm *str_0(Gterm *t);
extern Gterm *str_288(Gterm *t);
extern Gterm *str_84(Gterm *t);
extern Gterm *str_58(Gterm *t);
extern Gterm *str_173(Gterm *t);
extern Gterm *str_176(Gterm *t);
extern Gterm *str_224(Gterm *t);
extern Gterm *str_221(Gterm *t);
extern Gterm *str_467(Gterm *t);
extern Gterm *str_420(Gterm *t);
extern Gterm *str_99(Gterm *t);
extern Gterm *str_96(Gterm *t);
extern Gterm *str_116(Gterm *t);
extern Gterm *str_405(Gterm *t);
extern Gterm *str_269(Gterm *t);
extern Gterm *str_39(Gterm *t);
extern Gterm *str_164(Gterm *t);
extern Gterm *str_88(Gterm *t);
extern Gterm *str_203(Gterm *t);
#endif
