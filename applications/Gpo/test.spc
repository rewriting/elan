specification test
Vars
	alpha beta
Ops
	dx:1 x:0 two:0 one:0 a:0 zero:0 plus:2 times:2 minus:2 neg:1 div:2 exp:2 ln:1
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




*/