specification test3
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
nil
end of specification



/*



dx(div(alpha, beta)) -> minus(div(dx(alpha), beta), times(alpha, div(dx(beta), exp(beta,two))))
dx(ln(alpha)) -> div(dx(alpha), alpha)
dx(exp(alpha, beta)) -> plus(times(beta, times(exp(alpha, minus(beta, one)), dx(alpha))), times(exp(alpha, beta), times(ln(alpha), dx(beta))))



[] result term:
    precedence=(prec(dx>neg)^(prec(x>one)^(prec(a>zero)^(prec(dx>plus)^(prec(dx>times)^(prec(dx>minus)^(prec(dx=dx)^T)))))))


[] end


enter query term finished by the key word 'end':


[ executionAbort Continue Dump Exit Statistics
  changeTrace changeQuiet Input Output stRategy (ACDESTQIOR|acdestqior)] ?s
[] statistics:
 total time     (40314.843+0.000)=40314.843 sec (main+subprocesses)
 average speed  42 inf/sec
            44609618 nonamed rules applied, 133109236 tried
            4365383   named rules applied, 858826584 tried

                rule: applied   tried                 rule: applied   tried
        anti_equality  276932 4045962            topOccRule       0       0
                 init       1       1     lpo_instantiation 1177764244797083
         extractrule1       0       0          extractrule2       0       0
         decompose_eq    1798283003549              equality       0 2161447
       decompose_ineq  209392301175125          anti_subterm  798713 3792696
            inOccRule       0       0                 final       1  991123
   prec_simplify_init  247780  247781             prec_next  562792 1082382
              subterm  83253611345709         prec_simplify  257674 6183726


*/