ARCH = `uname -m`
ifeq '$(ARCH)' "alpha"
CC = cc
LIBELAN = -lelanmv -lexc
FAST = -fast
else
CC = gcc
LIBELAN = -lelan
FAST = -O2
endif
INC = -I$(ELANLIB)/Compiler/ -I$(ELANLIB)/Compiled/ -I$(ELANLIB)/Compiled/$(ARCH)
CC_OPT = -DBITSET32 -DBORO -DBGSHARE -DNONUNDERSCORED -DBITSETMASK -DGCMEM -DCOLOR $(INC)
LIB = -L$(ELANLIB)/Compiler/$(ARCH) -L$(ELANLIB)/Compiled/$(ARCH) $(LIBELAN) -lRuntimeSupport -ll -ly -lgc
OBJ = prod_rules.o prod_rules.split.fun_262.o prod_rules.split.fun_202.o prod_rules.split.fun_411.o prod_rules.split.fun_229.o prod_rules.split.fun_333.o prod_rules.split.fun_423.o prod_rules.split.fun_223.o prod_rules.split.fun_375.o prod_rules.split.fun_204.o prod_rules.split.fun_306.o prod_rules.split.fun_268.o prod_rules.split.fun_310.o prod_rules.split.fun_312.o prod_rules.split.fun_237.o prod_rules.split.fun_342.o prod_rules.split.fun_418.o prod_rules.split.fun_318.o prod_rules.split.fun_416.o prod_rules.split.fun_420.o prod_rules.split.fun_256.o prod_rules.split.fun_206.o prod_rules.split.fun_239.o prod_rules.split.fun_208.o prod_rules.split.fun_395.o prod_rules.split.fun_330.o prod_rules.split.fun_263.o prod_rules.split.fun_356.o prod_rules.split.fun_402.o prod_rules.split.fun_372.o prod_rules.split.fun_207.o prod_rules.split.fun_317.o prod_rules.split.fun_203.o prod_rules.split.fun_300.o prod_rules.split.fun_404.o prod_rules.split.fun_293.o prod_rules.split.fun_412.o prod_rules.split.fun_380.o prod_rules.split.fun_298.o prod_rules.split.fun_308.o prod_rules.split.fun_363.o prod_rules.split.fun_321.o prod_rules.split.fun_250.o prod_rules.split.fun_313.o prod_rules.split.fun_215.o prod_rules.split.fun_362.o prod_rules.split.fun_287.o prod_rules.split.fun_391.o prod_rules.split.fun_376.o prod_rules.split.fun_258.o prod_rules.split.fun_403.o prod_rules.split.fun_427.o prod_rules.split.fun_324.o prod_rules.split.fun_286.o prod_rules.split.fun_384.o prod_rules.split.fun_422.o prod_rules.split.fun_408.o prod_rules.split.fun_302.o prod_rules.split.fun_264.o prod_rules.split.fun_419.o prod_rules.split.fun_424.o prod_rules.split.fun_218.o prod_rules.split.fun_352.o prod_rules.split.fun_430.o prod_rules.split.fun_314.o prod_rules.split.fun_382.o prod_rules.split.fun_325.o prod_rules.split.fun_351.o prod_rules.split.fun_305.o prod_rules.split.fun_410.o prod_rules.split.fun_360.o prod_rules.split.fun_210.o prod_rules.split.fun_428.o prod_rules.split.fun_371.o prod_rules.split.fun_295.o prod_rules.split.fun_248.o prod_rules.split.fun_225.o prod_rules.split.fun_431.o prod_rules.split.fun_205.o prod_rules.split.fun_211.o prod_rules.split.fun_212.o prod_rules.split.fun_235.o prod_rules.split.fun_214.o prod_rules.split.fun_236.o prod_rules.split.fun_240.o prod_rules.split.fun_348.o prod_rules.split.fun_301.o prod_rules.split.fun_426.o prod_rules.split.fun_383.o prod_rules.split.fun_341.o prod_rules.split.fun_364.o prod_rules.split.fun_230.o prod_rules.split.fun_392.o prod_rules.split.fun_368.o prod_rules.split.fun_297.o prod_rules.split.fun_397.o prod_rules.split.fun_216.o prod_rules.split.fun_254.o prod_rules.split.fun_345.o prod_rules.split.fun_425.o prod_rules.split.fun_266.o prod_rules.split.fun_249.o prod_rules.split.fun_334.o prod_rules.split.fun_217.o prod_rules.split.fun_294.o prod_rules.split.fun_388.o prod_rules.split.fun_303.o prod_rules.split.fun_400.o prod_rules.split.fun_357.o prod_rules.split.fun_429.o prod_rules.split.fun_396.o prod_rules.split.fun_221.o prod_rules.split.fun_267.o prod_rules.split.fun_224.o prod_rules.split.fun_233.o prod_rules.split.fun_432.o prod_rules.split.fun_370.o prod_rules.split.fun_323.o prod_rules.split.fun_246.o prod_rules.split.fun_291.o prod_rules.split.fun_209.o prod_rules.split.fun_338.o prod_rules.split.fun_355.o prod_rules.split.fun_390.o prod_rules.split.fun_304.o prod_rules.split.fun_327.o prod_rules.split.fun_307.o prod_rules.split.fun_417.o prod_rules.split.fun_415.o prod_rules.split.fun_350.o prod_rules.split.fun_213.o prod_rules.split.fun_421.o prod_rules.split.fun_228.o prod_rules.split.fun_299.o prod_rules.split.fun_332.o prod_rules.split.fun_257.o prod_rules.split.fun_377.o prod_rules.split.str_242.o prod_rules.split.str_211.o prod_rules.split.str_58.o prod_rules.split.str_480.o prod_rules.split.str_214.o prod_rules.split.str_205.o prod_rules.split.str_152.o prod_rules.split.str_499.o prod_rules.split.str_236.o prod_rules.split.str_84.o prod_rules.split.str_26.o prod_rules.split.str_434.o prod_rules.split.str_288.o prod_rules.split.str_190.o prod_rules.split.str_70.o prod_rules.split.str_284.o prod_rules.split.str_0.o 
.c.o:
	$(CC) $(FAST) $(CC_OPT) -c -o $*.o $<
a.out: $(OBJ)
	$(CC) $(CC_OPT) -o a.out $(OBJ) $(LIB)
prod_rules.split.fun_262.o: prod_rules.h
prod_rules.split.fun_202.o: prod_rules.h
prod_rules.split.fun_411.o: prod_rules.h
prod_rules.split.fun_229.o: prod_rules.h
prod_rules.split.fun_333.o: prod_rules.h
prod_rules.split.fun_423.o: prod_rules.h
prod_rules.split.fun_223.o: prod_rules.h
prod_rules.split.fun_375.o: prod_rules.h
prod_rules.split.fun_204.o: prod_rules.h
prod_rules.split.fun_306.o: prod_rules.h
prod_rules.split.fun_268.o: prod_rules.h
prod_rules.split.fun_310.o: prod_rules.h
prod_rules.split.fun_312.o: prod_rules.h
prod_rules.split.fun_237.o: prod_rules.h
prod_rules.split.fun_342.o: prod_rules.h
prod_rules.split.fun_418.o: prod_rules.h
prod_rules.split.fun_318.o: prod_rules.h
prod_rules.split.fun_416.o: prod_rules.h
prod_rules.split.fun_420.o: prod_rules.h
prod_rules.split.fun_256.o: prod_rules.h
prod_rules.split.fun_206.o: prod_rules.h
prod_rules.split.fun_239.o: prod_rules.h
prod_rules.split.fun_208.o: prod_rules.h
prod_rules.split.fun_395.o: prod_rules.h
prod_rules.split.fun_330.o: prod_rules.h
prod_rules.split.fun_263.o: prod_rules.h
prod_rules.split.fun_356.o: prod_rules.h
prod_rules.split.fun_402.o: prod_rules.h
prod_rules.split.fun_372.o: prod_rules.h
prod_rules.split.fun_207.o: prod_rules.h
prod_rules.split.fun_317.o: prod_rules.h
prod_rules.split.fun_203.o: prod_rules.h
prod_rules.split.fun_300.o: prod_rules.h
prod_rules.split.fun_404.o: prod_rules.h
prod_rules.split.fun_293.o: prod_rules.h
prod_rules.split.fun_412.o: prod_rules.h
prod_rules.split.fun_380.o: prod_rules.h
prod_rules.split.fun_298.o: prod_rules.h
prod_rules.split.fun_308.o: prod_rules.h
prod_rules.split.fun_363.o: prod_rules.h
prod_rules.split.fun_321.o: prod_rules.h
prod_rules.split.fun_250.o: prod_rules.h
prod_rules.split.fun_313.o: prod_rules.h
prod_rules.split.fun_215.o: prod_rules.h
prod_rules.split.fun_362.o: prod_rules.h
prod_rules.split.fun_287.o: prod_rules.h
prod_rules.split.fun_391.o: prod_rules.h
prod_rules.split.fun_376.o: prod_rules.h
prod_rules.split.fun_258.o: prod_rules.h
prod_rules.split.fun_403.o: prod_rules.h
prod_rules.split.fun_427.o: prod_rules.h
prod_rules.split.fun_324.o: prod_rules.h
prod_rules.split.fun_286.o: prod_rules.h
prod_rules.split.fun_384.o: prod_rules.h
prod_rules.split.fun_422.o: prod_rules.h
prod_rules.split.fun_408.o: prod_rules.h
prod_rules.split.fun_302.o: prod_rules.h
prod_rules.split.fun_264.o: prod_rules.h
prod_rules.split.fun_419.o: prod_rules.h
prod_rules.split.fun_424.o: prod_rules.h
prod_rules.split.fun_218.o: prod_rules.h
prod_rules.split.fun_352.o: prod_rules.h
prod_rules.split.fun_430.o: prod_rules.h
prod_rules.split.fun_314.o: prod_rules.h
prod_rules.split.fun_382.o: prod_rules.h
prod_rules.split.fun_325.o: prod_rules.h
prod_rules.split.fun_351.o: prod_rules.h
prod_rules.split.fun_305.o: prod_rules.h
prod_rules.split.fun_410.o: prod_rules.h
prod_rules.split.fun_360.o: prod_rules.h
prod_rules.split.fun_210.o: prod_rules.h
prod_rules.split.fun_428.o: prod_rules.h
prod_rules.split.fun_371.o: prod_rules.h
prod_rules.split.fun_295.o: prod_rules.h
prod_rules.split.fun_248.o: prod_rules.h
prod_rules.split.fun_225.o: prod_rules.h
prod_rules.split.fun_431.o: prod_rules.h
prod_rules.split.fun_205.o: prod_rules.h
prod_rules.split.fun_211.o: prod_rules.h
prod_rules.split.fun_212.o: prod_rules.h
prod_rules.split.fun_235.o: prod_rules.h
prod_rules.split.fun_214.o: prod_rules.h
prod_rules.split.fun_236.o: prod_rules.h
prod_rules.split.fun_240.o: prod_rules.h
prod_rules.split.fun_348.o: prod_rules.h
prod_rules.split.fun_301.o: prod_rules.h
prod_rules.split.fun_426.o: prod_rules.h
prod_rules.split.fun_383.o: prod_rules.h
prod_rules.split.fun_341.o: prod_rules.h
prod_rules.split.fun_364.o: prod_rules.h
prod_rules.split.fun_230.o: prod_rules.h
prod_rules.split.fun_392.o: prod_rules.h
prod_rules.split.fun_368.o: prod_rules.h
prod_rules.split.fun_297.o: prod_rules.h
prod_rules.split.fun_397.o: prod_rules.h
prod_rules.split.fun_216.o: prod_rules.h
prod_rules.split.fun_254.o: prod_rules.h
prod_rules.split.fun_345.o: prod_rules.h
prod_rules.split.fun_425.o: prod_rules.h
prod_rules.split.fun_266.o: prod_rules.h
prod_rules.split.fun_249.o: prod_rules.h
prod_rules.split.fun_334.o: prod_rules.h
prod_rules.split.fun_217.o: prod_rules.h
prod_rules.split.fun_294.o: prod_rules.h
prod_rules.split.fun_388.o: prod_rules.h
prod_rules.split.fun_303.o: prod_rules.h
prod_rules.split.fun_400.o: prod_rules.h
prod_rules.split.fun_357.o: prod_rules.h
prod_rules.split.fun_429.o: prod_rules.h
prod_rules.split.fun_396.o: prod_rules.h
prod_rules.split.fun_221.o: prod_rules.h
prod_rules.split.fun_267.o: prod_rules.h
prod_rules.split.fun_224.o: prod_rules.h
prod_rules.split.fun_233.o: prod_rules.h
prod_rules.split.fun_432.o: prod_rules.h
prod_rules.split.fun_370.o: prod_rules.h
prod_rules.split.fun_323.o: prod_rules.h
prod_rules.split.fun_246.o: prod_rules.h
prod_rules.split.fun_291.o: prod_rules.h
prod_rules.split.fun_209.o: prod_rules.h
prod_rules.split.fun_338.o: prod_rules.h
prod_rules.split.fun_355.o: prod_rules.h
prod_rules.split.fun_390.o: prod_rules.h
prod_rules.split.fun_304.o: prod_rules.h
prod_rules.split.fun_327.o: prod_rules.h
prod_rules.split.fun_307.o: prod_rules.h
prod_rules.split.fun_417.o: prod_rules.h
prod_rules.split.fun_415.o: prod_rules.h
prod_rules.split.fun_350.o: prod_rules.h
prod_rules.split.fun_213.o: prod_rules.h
prod_rules.split.fun_421.o: prod_rules.h
prod_rules.split.fun_228.o: prod_rules.h
prod_rules.split.fun_299.o: prod_rules.h
prod_rules.split.fun_332.o: prod_rules.h
prod_rules.split.fun_257.o: prod_rules.h
prod_rules.split.fun_377.o: prod_rules.h
prod_rules.split.str_242.o: prod_rules.h
prod_rules.split.str_211.o: prod_rules.h
prod_rules.split.str_58.o: prod_rules.h
prod_rules.split.str_480.o: prod_rules.h
prod_rules.split.str_214.o: prod_rules.h
prod_rules.split.str_205.o: prod_rules.h
prod_rules.split.str_152.o: prod_rules.h
prod_rules.split.str_499.o: prod_rules.h
prod_rules.split.str_236.o: prod_rules.h
prod_rules.split.str_84.o: prod_rules.h
prod_rules.split.str_26.o: prod_rules.h
prod_rules.split.str_434.o: prod_rules.h
prod_rules.split.str_288.o: prod_rules.h
prod_rules.split.str_190.o: prod_rules.h
prod_rules.split.str_70.o: prod_rules.h
prod_rules.split.str_284.o: prod_rules.h
prod_rules.split.str_0.o: prod_rules.h
prod_rules.o: prod_rules.c prod_rules.h
	$(CC) $(CC_OPT) -c prod_rules.c
clean:
	/bin/rm -f prod_rules.o prod_rules.split.fun_262.o prod_rules.split.fun_202.o prod_rules.split.fun_411.o prod_rules.split.fun_229.o prod_rules.split.fun_333.o prod_rules.split.fun_423.o prod_rules.split.fun_223.o prod_rules.split.fun_375.o prod_rules.split.fun_204.o prod_rules.split.fun_306.o prod_rules.split.fun_268.o prod_rules.split.fun_310.o prod_rules.split.fun_312.o prod_rules.split.fun_237.o prod_rules.split.fun_342.o prod_rules.split.fun_418.o prod_rules.split.fun_318.o prod_rules.split.fun_416.o prod_rules.split.fun_420.o prod_rules.split.fun_256.o prod_rules.split.fun_206.o prod_rules.split.fun_239.o prod_rules.split.fun_208.o prod_rules.split.fun_395.o prod_rules.split.fun_330.o prod_rules.split.fun_263.o prod_rules.split.fun_356.o prod_rules.split.fun_402.o prod_rules.split.fun_372.o prod_rules.split.fun_207.o prod_rules.split.fun_317.o prod_rules.split.fun_203.o prod_rules.split.fun_300.o prod_rules.split.fun_404.o prod_rules.split.fun_293.o prod_rules.split.fun_412.o prod_rules.split.fun_380.o prod_rules.split.fun_298.o prod_rules.split.fun_308.o prod_rules.split.fun_363.o prod_rules.split.fun_321.o prod_rules.split.fun_250.o prod_rules.split.fun_313.o prod_rules.split.fun_215.o prod_rules.split.fun_362.o prod_rules.split.fun_287.o prod_rules.split.fun_391.o prod_rules.split.fun_376.o prod_rules.split.fun_258.o prod_rules.split.fun_403.o prod_rules.split.fun_427.o prod_rules.split.fun_324.o prod_rules.split.fun_286.o prod_rules.split.fun_384.o prod_rules.split.fun_422.o prod_rules.split.fun_408.o prod_rules.split.fun_302.o prod_rules.split.fun_264.o prod_rules.split.fun_419.o prod_rules.split.fun_424.o prod_rules.split.fun_218.o prod_rules.split.fun_352.o prod_rules.split.fun_430.o prod_rules.split.fun_314.o prod_rules.split.fun_382.o prod_rules.split.fun_325.o prod_rules.split.fun_351.o prod_rules.split.fun_305.o prod_rules.split.fun_410.o prod_rules.split.fun_360.o prod_rules.split.fun_210.o prod_rules.split.fun_428.o prod_rules.split.fun_371.o prod_rules.split.fun_295.o prod_rules.split.fun_248.o prod_rules.split.fun_225.o prod_rules.split.fun_431.o prod_rules.split.fun_205.o prod_rules.split.fun_211.o prod_rules.split.fun_212.o prod_rules.split.fun_235.o prod_rules.split.fun_214.o prod_rules.split.fun_236.o prod_rules.split.fun_240.o prod_rules.split.fun_348.o prod_rules.split.fun_301.o prod_rules.split.fun_426.o prod_rules.split.fun_383.o prod_rules.split.fun_341.o prod_rules.split.fun_364.o prod_rules.split.fun_230.o prod_rules.split.fun_392.o prod_rules.split.fun_368.o prod_rules.split.fun_297.o prod_rules.split.fun_397.o prod_rules.split.fun_216.o prod_rules.split.fun_254.o prod_rules.split.fun_345.o prod_rules.split.fun_425.o prod_rules.split.fun_266.o prod_rules.split.fun_249.o prod_rules.split.fun_334.o prod_rules.split.fun_217.o prod_rules.split.fun_294.o prod_rules.split.fun_388.o prod_rules.split.fun_303.o prod_rules.split.fun_400.o prod_rules.split.fun_357.o prod_rules.split.fun_429.o prod_rules.split.fun_396.o prod_rules.split.fun_221.o prod_rules.split.fun_267.o prod_rules.split.fun_224.o prod_rules.split.fun_233.o prod_rules.split.fun_432.o prod_rules.split.fun_370.o prod_rules.split.fun_323.o prod_rules.split.fun_246.o prod_rules.split.fun_291.o prod_rules.split.fun_209.o prod_rules.split.fun_338.o prod_rules.split.fun_355.o prod_rules.split.fun_390.o prod_rules.split.fun_304.o prod_rules.split.fun_327.o prod_rules.split.fun_307.o prod_rules.split.fun_417.o prod_rules.split.fun_415.o prod_rules.split.fun_350.o prod_rules.split.fun_213.o prod_rules.split.fun_421.o prod_rules.split.fun_228.o prod_rules.split.fun_299.o prod_rules.split.fun_332.o prod_rules.split.fun_257.o prod_rules.split.fun_377.o prod_rules.split.str_242.o prod_rules.split.str_211.o prod_rules.split.str_58.o prod_rules.split.str_480.o prod_rules.split.str_214.o prod_rules.split.str_205.o prod_rules.split.str_152.o prod_rules.split.str_499.o prod_rules.split.str_236.o prod_rules.split.str_84.o prod_rules.split.str_26.o prod_rules.split.str_434.o prod_rules.split.str_288.o prod_rules.split.str_190.o prod_rules.split.str_70.o prod_rules.split.str_284.o prod_rules.split.str_0.o 
veryclean: clean 
	/bin/rm -f prod_rules.c prod_rules.h prod_rules.split.fun_262.c prod_rules.split.fun_202.c prod_rules.split.fun_411.c prod_rules.split.fun_229.c prod_rules.split.fun_333.c prod_rules.split.fun_423.c prod_rules.split.fun_223.c prod_rules.split.fun_375.c prod_rules.split.fun_204.c prod_rules.split.fun_306.c prod_rules.split.fun_268.c prod_rules.split.fun_310.c prod_rules.split.fun_312.c prod_rules.split.fun_237.c prod_rules.split.fun_342.c prod_rules.split.fun_418.c prod_rules.split.fun_318.c prod_rules.split.fun_416.c prod_rules.split.fun_420.c prod_rules.split.fun_256.c prod_rules.split.fun_206.c prod_rules.split.fun_239.c prod_rules.split.fun_208.c prod_rules.split.fun_395.c prod_rules.split.fun_330.c prod_rules.split.fun_263.c prod_rules.split.fun_356.c prod_rules.split.fun_402.c prod_rules.split.fun_372.c prod_rules.split.fun_207.c prod_rules.split.fun_317.c prod_rules.split.fun_203.c prod_rules.split.fun_300.c prod_rules.split.fun_404.c prod_rules.split.fun_293.c prod_rules.split.fun_412.c prod_rules.split.fun_380.c prod_rules.split.fun_298.c prod_rules.split.fun_308.c prod_rules.split.fun_363.c prod_rules.split.fun_321.c prod_rules.split.fun_250.c prod_rules.split.fun_313.c prod_rules.split.fun_215.c prod_rules.split.fun_362.c prod_rules.split.fun_287.c prod_rules.split.fun_391.c prod_rules.split.fun_376.c prod_rules.split.fun_258.c prod_rules.split.fun_403.c prod_rules.split.fun_427.c prod_rules.split.fun_324.c prod_rules.split.fun_286.c prod_rules.split.fun_384.c prod_rules.split.fun_422.c prod_rules.split.fun_408.c prod_rules.split.fun_302.c prod_rules.split.fun_264.c prod_rules.split.fun_419.c prod_rules.split.fun_424.c prod_rules.split.fun_218.c prod_rules.split.fun_352.c prod_rules.split.fun_430.c prod_rules.split.fun_314.c prod_rules.split.fun_382.c prod_rules.split.fun_325.c prod_rules.split.fun_351.c prod_rules.split.fun_305.c prod_rules.split.fun_410.c prod_rules.split.fun_360.c prod_rules.split.fun_210.c prod_rules.split.fun_428.c prod_rules.split.fun_371.c prod_rules.split.fun_295.c prod_rules.split.fun_248.c prod_rules.split.fun_225.c prod_rules.split.fun_431.c prod_rules.split.fun_205.c prod_rules.split.fun_211.c prod_rules.split.fun_212.c prod_rules.split.fun_235.c prod_rules.split.fun_214.c prod_rules.split.fun_236.c prod_rules.split.fun_240.c prod_rules.split.fun_348.c prod_rules.split.fun_301.c prod_rules.split.fun_426.c prod_rules.split.fun_383.c prod_rules.split.fun_341.c prod_rules.split.fun_364.c prod_rules.split.fun_230.c prod_rules.split.fun_392.c prod_rules.split.fun_368.c prod_rules.split.fun_297.c prod_rules.split.fun_397.c prod_rules.split.fun_216.c prod_rules.split.fun_254.c prod_rules.split.fun_345.c prod_rules.split.fun_425.c prod_rules.split.fun_266.c prod_rules.split.fun_249.c prod_rules.split.fun_334.c prod_rules.split.fun_217.c prod_rules.split.fun_294.c prod_rules.split.fun_388.c prod_rules.split.fun_303.c prod_rules.split.fun_400.c prod_rules.split.fun_357.c prod_rules.split.fun_429.c prod_rules.split.fun_396.c prod_rules.split.fun_221.c prod_rules.split.fun_267.c prod_rules.split.fun_224.c prod_rules.split.fun_233.c prod_rules.split.fun_432.c prod_rules.split.fun_370.c prod_rules.split.fun_323.c prod_rules.split.fun_246.c prod_rules.split.fun_291.c prod_rules.split.fun_209.c prod_rules.split.fun_338.c prod_rules.split.fun_355.c prod_rules.split.fun_390.c prod_rules.split.fun_304.c prod_rules.split.fun_327.c prod_rules.split.fun_307.c prod_rules.split.fun_417.c prod_rules.split.fun_415.c prod_rules.split.fun_350.c prod_rules.split.fun_213.c prod_rules.split.fun_421.c prod_rules.split.fun_228.c prod_rules.split.fun_299.c prod_rules.split.fun_332.c prod_rules.split.fun_257.c prod_rules.split.fun_377.c prod_rules.split.str_242.c prod_rules.split.str_211.c prod_rules.split.str_58.c prod_rules.split.str_480.c prod_rules.split.str_214.c prod_rules.split.str_205.c prod_rules.split.str_152.c prod_rules.split.str_499.c prod_rules.split.str_236.c prod_rules.split.str_84.c prod_rules.split.str_26.c prod_rules.split.str_434.c prod_rules.split.str_288.c prod_rules.split.str_190.c prod_rules.split.str_70.c prod_rules.split.str_284.c prod_rules.split.str_0.c prod_rules.make prod_rules.ref 
