specification derive
Vars
	alpha beta
Ops
	dx:1 x:0 two:0 one:0 a:0 zero:0 plus:2 times:2 minus:2 neg:1 div:2 exp:2 ln:1
Rules

dx(x) -> one
dx(a) -> zero
dx(plus(alpha, beta)) -> plus(dx(alpha), dx(beta))
dx(times(alpha, beta)) -> plus(times(beta, dx(alpha)), times(alpha, dx(beta)))
dx(minus(alpha, beta)) -> minus(dx(alpha), dx(beta))
dx(neg(alpha)) -> neg(dx(alpha))
dx(div(alpha, beta)) -> minus(div(dx(alpha), beta), times(alpha, div(dx(beta), exp(beta,two))))
dx(ln(alpha)) -> div(dx(alpha), alpha)
dx(exp(alpha, beta)) -> plus(times(beta, times(exp(alpha, minus(beta, one)), dx(alpha))), times(exp(alpha, beta), times(ln(alpha), dx(beta))))
nil
end of specification



/*

[ executionAbort Continue Dump Exit Statistics
  changeTrace changeQuiet Input Output stRategy (ACDESTQIOR|acdestqior)] ?s  
[] statistics:
 total time     (84124.652+0.000)=84124.652 sec (main+subprocesses)

            44122389 nonamed rules applied, 132172376 tried
            2718354   named rules applied, 419316277 tried

                rule: applied   tried                 rule: applied   tried
        anti_equality  270494 2102168            topOccRule       0       0
                 init       1       1     lpo_instantiation  817296101825078
         extractrule1       0       0          extractrule2       0       0
         decompose_eq      60148894205              equality       0 1186331
       decompose_ineq   34799154106260          anti_subterm  790592 2428440
            inOccRule       0       0                 final       0  127364
   prec_simplify_init   31841   31841             prec_next  176795  240475
              subterm  451517 6497336         prec_simplify  144959 1876778








*/