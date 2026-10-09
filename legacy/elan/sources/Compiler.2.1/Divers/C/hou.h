#ifndef __main_header_h
#define __main_header_h
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "term.h"
#include "bitset.h"
#include "bgraph.h"
#include "match_state.h"
#include "back.h"
#include "builtin.h"
#include "builtinMatching.h"
#include "streval.h"
#include "gc.h"
#include "acmatchdefs.h"

/* Codes */
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
#define code_79 79
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
#define code_68 68
#define code_67 67
#define code_359 359
#define code_358 358
#define code_357 357
#define code_356 356
#define code_355 355
#define code_354 354
#define code_353 353
#define code_352 352
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
#define code_340 340
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
#define code_33 33
#define code_32 32
#define code_31 31
#define code_30 30
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
#define code_29 29
#define code_28 28
#define code_27 27
#define code_26 26
#define code_25 25
#define code_24 24
#define code_22 22
#define code_21 21
#define code_20 20
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
#define code_19 19
#define code_18 18
#define code_17 17
#define code_16 16
#define code_15 15
#define code_14 14
#define code_13 13
#define code_12 12
#define code_11 11
#define code_10 10
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
#define code_299 299
#define code_298 298
#define code_297 297
#define code_296 296
#define code_295 295
#define code_294 294
#define code_293 293
#define code_292 292
#define code_291 291
#define code_290 290
#define code_289 289
#define code_288 288
#define code_287 287
#define code_286 286
#define code_285 285
#define code_284 284
#define code_283 283
#define code_282 282
#define code_281 281
#define code_280 280
#define code_279 279
#define code_278 278
#define code_277 277
#define code_276 276
#define code_275 275
#define code_274 274
#define code_273 273
#define code_272 272
#define code_271 271
#define code_270 270
#define code_480 480
#define code_269 269
#define code_268 268
#define code_267 267
#define code_266 266
#define code_265 265
#define code_264 264
#define code_263 263
#define code_262 262
#define code_261 261
#define code_260 260
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
#define code_259 259
#define code_258 258
#define code_257 257
#define code_256 256
#define code_255 255
#define code_254 254
#define code_253 253
#define code_252 252
#define code_251 251
#define code_250 250
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
#define code_249 249
#define code_248 248
#define code_247 247
#define code_246 246
#define code_245 245
#define code_244 244
#define code_243 243
#define code_242 242
#define code_241 241
#define code_240 240
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
#define code_239 239
#define code_238 238
#define code_237 237
#define code_236 236
#define code_9 9
#define code_235 235
#define code_8 8
#define code_234 234
#define code_233 233
#define code_6 6
#define code_232 232
#define code_5 5
#define code_231 231
#define code_4 4
#define code_230 230
#define code_3 3
#define code_1 1
#define code_0 0
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
#define code_229 229
#define code_228 228
#define code_227 227
#define code_226 226
#define code_225 225
#define code_224 224
#define code_223 223
#define code_222 222
#define code_221 221
#define code_220 220
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
#define code_219 219
#define code_218 218
#define code_217 217
#define code_216 216
#define code_215 215
#define code_214 214
#define code_213 213
#define code_212 212
#define code_211 211
#define code_210 210
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
#define code_209 209
#define code_208 208
#define code_207 207
#define code_206 206
#define code_205 205
#define code_204 204
#define code_203 203
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
#define code_399 399
#define code_398 398
#define code_397 397
#define code_396 396
#define code_395 395
#define code_394 394
#define code_393 393
#define code_392 392

/* Structures */
TERMSTR(term3,3);
TERMSTR(term5,5);
TERMSTR(term4,4);
TERMSTR(term6,6);

/* Constantes d'execution */
/* 0: no trace, 1: result, 2: start with */
extern int trace;
extern int debugMode;
extern unsigned long rewrite_step;
extern unsigned long tab_rewrite_step[2][480];
extern int global_indentlevel;
extern TERM *EkerTerm[];

/* Macros */
#define MAX_TERM_SIZE 1000 /* nb max du sous-termes d'un symbole AC */
#define MAX_CBG_SIZE  100  /* nb max de patterns dans un CBG */
#ifdef DEBUG
#define addindent() global_indentlevel++;
#define subindent() global_indentlevel--;
#define doindent(deep) indent(deep);
#define saveGlobalIndent() int indentlevel=global_indentlevel;
#define restoreGlobalIndent() global_indentlevel=indentlevel;
#else
#define addindent()
#define subindent()
#define doindent(deep)
#define saveGlobalIndent()
#define restoreGlobalIndent()
#endif

/* Entetes */
extern struct term* fun_391(struct term *v1);
extern struct term* fun_390(struct term *v1);
extern struct term* fun_389(struct term *v1);
extern struct term* fun_388(struct term *v1);
extern struct term* fun_387(struct term *v1);
extern struct term* fun_386(struct term *v1);
extern struct term* fun_385(struct term *v1);
extern struct term* fun_384(struct term *v1);
extern struct term* fun_383(struct term *v1);
extern struct term* fun_382(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_381(struct term *v1,struct term *v2);
extern struct term* fun_379(struct term *v1);
extern struct term* fun_378(struct term *v1);
extern struct term* fun_377(struct term *v1);
extern struct term* fun_376(struct term *v1,struct term *v2);
extern struct term* fun_375(struct term *v1,struct term *v2);
extern struct term* fun_374(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_373(struct term *v1);
extern struct term* fun_372(struct term *v1,struct term *v2);
extern struct term* fun_371(struct term *v1,struct term *v2);
extern struct term* fun_370(struct term *v1);
extern struct term* fun_369(struct term *v1);
extern struct term* fun_368(struct term *v1);
extern struct term* fun_367(struct term *v1);
extern struct term* fun_366(struct term *v1,struct term *v2);
extern struct term* fun_365(struct term *v1,struct term *v2);
extern struct term* fun_364(struct term *v1);
extern struct term* fun_363(struct term *v1);
extern struct term* fun_362(struct term *v1,struct term *v2);
extern struct term* fun_361(struct term *v1);
extern struct term* fun_360(struct term *v1,struct term *v2);
extern struct term* fun_359(struct term *v1);
extern struct term* fun_358(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_357(struct term *v1);
extern struct term* fun_356(struct term *v1,struct term *v2);
extern struct term* fun_355(struct term *v1);
extern struct term *con_350;
extern struct term *con_349;
extern struct term *con_347;
extern struct term* fun_339(struct term *v1);
extern struct term *con_336;
extern struct term *con_335;
extern struct term* fun_321(struct term *v1);
extern struct term* fun_320(struct term *v1);
extern struct term* fun_319(struct term *v1);
extern struct term *con_317;
extern struct term* fun_316(struct term *v1,struct term *v2);
extern struct term* fun_315(struct term *v1);
extern struct term* fun_314(struct term *v1,struct term *v2);
extern struct term* fun_312(struct term *v1,struct term *v2);
extern struct term *con_310;
extern struct term* fun_309();
extern struct term* fun_308();
extern struct term *con_307;
extern struct term *con_306;
extern struct term *con_305;
extern struct term* fun_302(struct term *v1);
extern struct term *con_301;
extern struct term* fun_300(struct term *v1,struct term *v2);
extern struct term* fun_299(struct term *v1);
extern struct term* fun_298(struct term *v1,struct term *v2);
extern struct term* fun_296(struct term *v1,struct term *v2);
extern struct term *con_294;
extern struct term* fun_293();
extern struct term* fun_292();
extern struct term *con_291;
extern struct term *con_290;
extern struct term* fun_287(struct term *v1);
extern struct term *con_286;
extern struct term* fun_285(struct term *v1,struct term *v2);
extern struct term* fun_284(struct term *v1);
extern struct term* fun_283(struct term *v1,struct term *v2);
extern struct term* fun_281(struct term *v1,struct term *v2);
extern struct term *con_279;
extern struct term *con_278;
extern struct term* fun_277(struct term *v1,struct term *v2);
extern struct term* fun_276(struct term *v1);
extern struct term* fun_275(struct term *v1,struct term *v2);
extern struct term* fun_273(struct term *v1,struct term *v2);
extern struct term *con_271;
extern struct term *con_480;
extern struct term *con_267;
extern struct term *con_266;
extern struct term* fun_265(struct term *v1);
extern struct term* fun_264(struct term *v1);
extern struct term* fun_263(struct term *v1);
extern struct term* fun_262(struct term *v1,struct term *v2);
extern struct term* fun_479();
extern struct term* fun_478();
extern struct term* fun_477();
extern struct term* fun_476();
extern struct term* fun_475();
extern struct term* fun_474();
extern struct term* fun_473(struct term *v1,struct term *v2);
extern struct term* fun_472(struct term *v1);
extern struct term* fun_471(struct term *v1);
extern struct term* fun_470(struct term *v1,struct term *v2);
extern struct term *con_259;
extern struct term *con_258;
extern struct term* fun_257();
extern struct term* fun_256();
extern struct term* fun_255(struct term *v1);
extern struct term* fun_254(struct term *v1);
extern struct term *con_252;
extern struct term* fun_251(struct term *v1,struct term *v2);
extern struct term* fun_250(struct term *v1);
extern struct term* fun_469(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_468(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_467(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_466(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_465(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_464(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_463(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_462(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_461(struct term *v1,struct term *v2);
extern struct term* fun_460(struct term *v1,struct term *v2);
extern struct term* fun_249(struct term *v1,struct term *v2);
extern struct term* fun_247(struct term *v1,struct term *v2);
extern struct term *con_245;
extern struct term *con_240;
extern struct term* fun_459(struct term *v1);
extern struct term* fun_458(struct term *v1,struct term *v2);
extern struct term* fun_457(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_456(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_455(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_454(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_453(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_452(struct term *v1,struct term *v2,struct term *v3,struct term *v4);
extern struct term* fun_451(struct term *v1);
extern struct term* fun_239(struct term *v1);
extern struct term* fun_238(struct term *v1);
extern struct term* fun_237(struct term *v1);
extern struct term* fun_236(struct term *v1);
extern struct term* fun_235(struct term *v1);
extern struct term* fun_234(struct term *v1);
extern struct term* fun_233(struct term *v1);
extern struct term *con_1;
extern struct term *con_0;
extern struct term* fun_448(struct term *v1,struct term *v2);
extern struct term* fun_447(struct term *v1);
extern struct term* fun_446(struct term *v1,struct term *v2);
extern struct term* fun_445(struct term *v1);
extern struct term* fun_444(struct term *v1,struct term *v2);
extern struct term* fun_443(struct term *v1);
extern struct term* fun_442(struct term *v1,struct term *v2);
extern struct term* fun_441(struct term *v1);
extern struct term *con_229;
extern struct term *con_228;
extern struct term* fun_227();
extern struct term *con_226;
extern struct term* fun_225(struct term *v1,struct term *v2);
extern struct term* fun_224(struct term *v1);
extern struct term* fun_223(struct term *v1,struct term *v2);
extern struct term* fun_221(struct term *v1,struct term *v2);
extern struct term *fun_439(struct term *t);
extern struct term *con_436;
extern struct term *fun_435(struct term *t);
extern struct term *con_219;
extern struct term* fun_218(struct term *v1);
extern struct term* fun_217(struct term *v1);
extern struct term* fun_216(struct term *v1);
extern struct term* fun_215(struct term *v1,struct term *v2);
extern struct term* fun_214(struct term *v1,struct term *v2);
extern struct term* fun_213(struct term *v1,struct term *v2);
extern struct term* fun_212(struct term *v1,struct term *v2);
extern struct term* fun_211(struct term *v1,struct term *v2);
extern struct term* fun_210(struct term *v1,struct term *v2);
extern struct term* fun_429(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_428(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_427(struct term *v1,struct term *v2,struct term *v3);
extern struct term *con_426;
extern struct term* fun_425(struct term *v1,struct term *v2);
extern struct term* fun_424(struct term *v1);
extern struct term* fun_423(struct term *v1,struct term *v2);
extern struct term* fun_421(struct term *v1,struct term *v2);
extern struct term* fun_209(struct term *v1);
extern struct term* fun_208(struct term *v1,struct term *v2);
extern struct term* fun_207(struct term *v1,struct term *v2);
extern struct term* fun_206(struct term *v1,struct term *v2);
extern struct term* fun_205(struct term *v1,struct term *v2);
extern struct term* fun_204(struct term *v1,struct term *v2);
extern struct term* fun_203(struct term *v1,struct term *v2);
extern struct term* fun_202(struct term *v1,struct term *v2);
extern struct term *con_419;
extern struct term *con_418;
extern struct term* fun_417(struct term *v1,struct term *v2);
extern struct term* fun_416(struct term *v1);
extern struct term* fun_415(struct term *v1,struct term *v2);
extern struct term* fun_413(struct term *v1,struct term *v2);
extern struct term *con_411;
extern struct term* fun_410(struct term *v1);
extern struct term* fun_409(struct term *v1,struct term *v2);
extern struct term* fun_408(struct term *v1,struct term *v2);
extern struct term* fun_407(struct term *v1,struct term *v2);
extern struct term* fun_406(struct term *v1);
extern struct term* fun_405(struct term *v1,struct term *v2);
extern struct term* fun_404(struct term *v1,struct term *v2);
extern struct term* fun_403(struct term *v1);
extern struct term *con_402;
extern struct term *con_401;
extern struct term *con_400;
extern struct term* fun_399(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_398(struct term *v1,struct term *v2);
extern struct term* fun_397(struct term *v1);
extern struct term* fun_396(struct term *v1);
extern struct term* fun_395(struct term *v1);
extern struct term* fun_394(struct term *v1);
extern struct term* fun_393(struct term *v1);
extern struct term* fun_392(struct term *v1);

/* Entetes */
extern struct term *str_258(struct term *t);
extern struct term *str_252(struct term *t);
extern struct term *str_172(struct term *t);
extern struct term *str_158(struct term *t);
extern struct term *str_390(struct term *t);
extern struct term *str_270(struct term *t);
extern struct term *str_248(struct term *t);
extern struct term *str_122(struct term *t);
extern struct term *str_89(struct term *t);
extern struct term *str_26(struct term *t);
extern struct term *str_468(struct term *t);
extern struct term *str_402(struct term *t);
extern struct term *str_494(struct term *t);
#endif
