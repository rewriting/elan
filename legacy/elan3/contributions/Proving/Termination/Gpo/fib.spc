specification fib

Vars
 varr  var_0 var_1

Ops
 fsym_201: 1 fsym_203: 1 fsym_202: 2 fsym_200: 0

Rules
 fsym_202( fsym_200(), var_0) ->  var_0
 fsym_202( fsym_201( var_0), var_1) ->  fsym_201( fsym_202( var_0, var_1))
 fsym_203( fsym_200()) ->  fsym_201( fsym_200())
 fsym_203( fsym_201( fsym_200())) ->  fsym_201( fsym_200())
 fsym_203( fsym_201( fsym_201( var_0))) ->  fsym_202( fsym_203( fsym_201( var_0)), fsym_203( var_0))

 nil

 end
