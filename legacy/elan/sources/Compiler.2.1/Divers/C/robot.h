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
#include "streval.h"

/* Codes */
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
#define code_309 309
#define code_308 308
#define code_307 307
#define code_306 306
#define code_305 305
#define code_304 304
#define code_303 303
#define code_139 139
#define code_302 302
#define code_138 138
#define code_301 301
#define code_137 137
#define code_300 300
#define code_136 136
#define code_135 135
#define code_134 134
#define code_133 133
#define code_132 132
#define code_131 131
#define code_130 130
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
#define code_129 129
#define code_128 128
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
#define code_81 81
#define code_79 79
#define code_9 9
#define code_8 8
#define code_6 6
#define code_5 5
#define code_382 382
#define code_4 4
#define code_381 381
#define code_3 3
#define code_380 380
#define code_1 1
#define code_0 0
#define code_67 67
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
#define code_190 190
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
#define code_359 359
#define code_358 358
#define code_357 357
#define code_356 356
#define code_355 355
#define code_354 354
#define code_353 353
#define code_189 189
#define code_352 352
#define code_188 188
#define code_351 351
#define code_187 187
#define code_350 350
#define code_186 186
#define code_185 185
#define code_184 184
#define code_183 183
#define code_182 182
#define code_181 181
#define code_180 180
#define code_269 269
#define code_268 268
#define code_33 33
#define code_267 267
#define code_32 32
#define code_266 266
#define code_31 31
#define code_265 265
#define code_30 30
#define code_264 264
#define code_263 263
#define code_262 262
#define code_261 261
#define code_260 260
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
#define code_29 29
#define code_28 28
#define code_27 27
#define code_26 26
#define code_259 259
#define code_25 25
#define code_258 258
#define code_24 24
#define code_257 257
#define code_256 256
#define code_22 22
#define code_255 255
#define code_21 21
#define code_254 254
#define code_20 20
#define code_253 253
#define code_252 252
#define code_251 251
#define code_250 250
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
#define code_19 19
#define code_18 18
#define code_249 249
#define code_248 248
#define code_247 247
#define code_13 13
#define code_246 246
#define code_12 12
#define code_245 245
#define code_11 11
#define code_244 244
#define code_10 10
#define code_243 243
#define code_242 242
#define code_241 241
#define code_240 240
#define code_329 329
#define code_328 328
#define code_327 327
#define code_326 326
#define code_325 325
#define code_324 324
#define code_323 323
#define code_322 322
#define code_158 158
#define code_321 321
#define code_157 157
#define code_320 320
#define code_156 156
#define code_154 154
#define code_153 153
#define code_152 152
#define code_151 151
#define code_150 150
#define code_239 239
#define code_238 238
#define code_237 237
#define code_236 236
#define code_235 235
#define code_234 234
#define code_233 233
#define code_232 232
#define code_231 231
#define code_230 230
#define code_319 319
#define code_318 318
#define code_317 317
#define code_316 316
#define code_315 315
#define code_314 314
#define code_313 313
#define code_312 312
#define code_311 311
#define code_147 147
#define code_310 310
#define code_146 146
#define code_145 145
#define code_144 144
#define code_143 143
#define code_142 142
#define code_141 141
#define code_140 140

/* Structures */
TERMSTR(term3,3);
TERMSTR(term4,4);

/* Constantes d'execution */
/* 0: no trace, 1: result, 2: start with */
#define trace 2
extern unsigned long rewrite_step;
extern unsigned long tab_rewrite_step[2][382];
extern int global_indentlevel;

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
extern struct term *fun_226(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_225();
extern struct term *fun_224(struct term *v0, struct term *v1);
extern struct term *fun_223();
extern struct term *fun_222(struct term *v0);
extern struct term *fun_221(struct term *v0);
extern struct term *fun_220(struct term *v0);
extern struct term* fun_309(struct term *v1,struct term *v2);
extern struct term* fun_308(struct term *v1);
extern struct term* fun_307(struct term *v1,struct term *v2);
extern struct term* fun_305(struct term *v1,struct term *v2);
extern struct term *con_303;
extern struct term *con_302;
extern struct term *con_301;
extern struct term *con_300;
extern struct term *fun_218(struct term *v0, struct term *v1);
extern struct term* fun_217(struct term *v1);
extern struct term* fun_216(struct term *v1);
extern struct term* fun_215(struct term *v1);
extern struct term* fun_214(struct term *v1,struct term *v2);
extern struct term* fun_213(struct term *v1,struct term *v2);
extern struct term* fun_212(struct term *v1,struct term *v2);
extern struct term* fun_211(struct term *v1,struct term *v2);
extern struct term* fun_210(struct term *v1,struct term *v2);
extern struct term* fun_209(struct term *v1,struct term *v2);
extern struct term* fun_208(struct term *v1);
extern struct term* fun_207(struct term *v1,struct term *v2);
extern struct term* fun_206(struct term *v1,struct term *v2);
extern struct term* fun_205(struct term *v1,struct term *v2);
extern struct term* fun_204(struct term *v1,struct term *v2);
extern struct term* fun_203(struct term *v1,struct term *v2);
extern struct term* fun_202(struct term *v1,struct term *v2);
extern struct term* fun_201(struct term *v1,struct term *v2);
extern struct term *con_381;
extern struct term *con_380;
extern struct term *con_1;
extern struct term *con_0;
extern struct term* fun_299(struct term *v1,struct term *v2);
extern struct term* fun_298(struct term *v1);
extern struct term* fun_297(struct term *v1,struct term *v2);
extern struct term* fun_295(struct term *v1,struct term *v2);
extern struct term *con_293;
extern struct term *con_292;
extern struct term *con_290;
extern struct term *con_379;
extern struct term *con_378;
extern struct term *con_377;
extern struct term *con_376;
extern struct term *con_375;
extern struct term *con_374;
extern struct term *con_372;
extern struct term *con_371;
extern struct term *con_370;
extern struct term *con_287;
extern struct term *fun_286(struct term *v0, struct term *v1);
extern struct term *con_285;
extern struct term *con_284;
extern struct term *con_283;
extern struct term *con_282;
extern struct term *con_369;
extern struct term *con_368;
extern struct term* fun_367(struct term *v1,struct term *v2);
extern struct term* fun_366();
extern struct term* fun_365();
extern struct term* fun_364();
extern struct term* fun_363();
extern struct term* fun_362();
extern struct term *fun_279(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_278();
extern struct term *fun_274(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_273();
extern struct term *fun_272(struct term *v0, struct term *v1);
extern struct term *fun_271();
extern struct term *fun_270(struct term *v0);
extern struct term *con_357;
extern struct term *con_356;
extern struct term* fun_353(struct term *v1,struct term *v2);
extern struct term *con_352;
extern struct term *con_351;
extern struct term *fun_269(struct term *v0);
extern struct term *fun_268(struct term *v0);
extern struct term *fun_266(struct term *v0, struct term *v1);
extern struct term *con_265;
extern struct term *con_349;
extern struct term *con_348;
extern struct term *con_347;
extern struct term *con_346;
extern struct term *fun_345(struct term *v0, struct term *v1);
extern struct term *con_344;
extern struct term *con_343;
extern struct term *con_342;
extern struct term *con_341;
extern struct term *fun_338(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_337();
extern struct term *fun_333(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_332();
extern struct term *fun_331(struct term *v0, struct term *v1);
extern struct term *fun_330();
extern struct term *con_240;
extern struct term *fun_329(struct term *v0);
extern struct term *fun_328(struct term *v0);
extern struct term *fun_327(struct term *v0);
extern struct term *fun_325(struct term *v0, struct term *v1);
extern struct term* fun_324(struct term *v1,struct term *v2,struct term *v3,struct term *v4);
extern struct term* fun_323(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_322(struct term *v1,struct term *v2);
extern struct term *fun_321(struct term *v0, struct term *v1, struct term *v2, struct term *v3);
extern struct term* fun_320(struct term *v1,struct term *v2,struct term *v3,struct term *v4);
extern struct term *con_239;
extern struct term *fun_238(struct term *v0, struct term *v1);
extern struct term *con_237;
extern struct term *con_236;
extern struct term *con_235;
extern struct term *con_234;
extern struct term *fun_231(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_230();
extern struct term* fun_319(struct term *v1,struct term *v2,struct term *v3,struct term *v4);
extern struct term* fun_318(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_317(struct term *v1,struct term *v2);
extern struct term *fun_316(struct term *v0, struct term *v1, struct term *v2);
extern struct term *fun_315(struct term *v0, struct term *v1);
extern struct term* fun_314(struct term *v1);
extern struct term* fun_313(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_312(struct term *v1,struct term *v2,struct term *v3);
extern struct term* fun_311(struct term *v1,struct term *v2);
extern struct term *con_310;

/* Entetes */
extern struct term *str_103(struct term *t);
extern struct term *str_361(struct term *t);
extern struct term *str_111(struct term *t);
extern struct term *str_498(struct term *t);
extern struct term *str_373(struct term *t);
extern struct term *str_163(struct term *t);
extern struct term *str_433(struct term *t);
extern struct term *str_171(struct term *t);
extern struct term *str_465(struct term *t);
extern struct term *str_182(struct term *t);
extern struct term *str_470(struct term *t);
extern struct term *str_91(struct term *t);
extern struct term *str_194(struct term *t);
extern struct term *str_471(struct term *t);
extern struct term *str_99(struct term *t);
extern struct term *str_254(struct term *t);
extern struct term *str_472(struct term *t);
#endif
