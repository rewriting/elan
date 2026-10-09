specification test2
Vars
	alpha beta
Ops
	dx:1 one:0 plus:2 times:2 minus:2 div:2 exp:2 ln:1
Rules
dx(ln(alpha)) -> div(dx(alpha), alpha)
dx(exp(alpha, beta)) -> plus(times(beta, times(exp(alpha, minus(beta, one)), dx(alpha))), times(exp(alpha, beta), times(ln(alpha), dx(beta))))
nil
end of specification



/*


dx(x) -> one
dx(a) -> zero
dx(plus(alpha, beta)) -> plus(dx(alpha), dx(beta))
dx(times(alpha, beta)) -> plus(times(beta, dx(alpha)), times(alpha, dx(beta)))
dx(minus(alpha, beta)) -> minus(dx(alpha), dx(beta))
dx(neg(alpha)) -> neg(dx(alpha))
dx(div(alpha, beta)) -> minus(div(dx(alpha), beta), times(alpha, div(dx(beta), exp(beta,two))))



[] result term:
    precedence=(prec(exp>div)^(prec(exp>ln)^(prec(ln>div)^(prec(ln>plus)^(prec(ln>dx)^(pre(dx>plus)^(prec(dx>minus)^(prec(dx>exp)^(prec(exp>minus)^(prec(exp>one)^(prec(dx=dx)^(precdx>times)^T))))))))))))


[] end


enter query term finished by the key word 'end':
^C

[ executionAbort Continue Dump Exit Statistics
  changeTrace changeQuiet Input Output stRategy (ACDESTQIOR|acdestqior)] ?s
[] statistics:
 total time     (2323.890+0.000)=2323.890 sec   (main+subprocesses)
 average speed  -723 inf/sec
            2443688 nonamed rules applied, 7046967 tried
            170378   named rules applied, 9102840 tried

                rule: applied   tried                 rule: applied   tried
        anti_equality   14011  112369            topOccRule       0       0
                 init       1       1     lpo_instantiation   29962 2617020
         extractrule1       0       0          extractrule2       0       0
         decompose_eq      44 2502933              equality       0   63190
       decompose_ineq   10062 3023194          anti_subterm   44884  137786
            inOccRule       0       0                 final       1   18223
   prec_simplify_init    4555    4556             prec_next   21416   31038
              subterm   29712  355580         prec_simplify   15730  236950




*/