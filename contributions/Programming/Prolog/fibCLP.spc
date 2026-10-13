specification fibCLP

     Vars         N X Y Z
     Ops          s:1 zero:0
     Predicates   plus:3  fib:2
     Clauses

/* X,Y,Z sont des entiers naturels tels que 
        Z soit la somme de X et de Y 
*/
                  plus(zero,X,X).
                  plus(s(X),Y,s(Z)):-plus(X,Y,Z),.

/* X est egal a fibonacci(N)
*/
                  fib(zero,X):- X=s(zero),.
                  fib(s(zero),X):- X=s(zero),.
                  fib(s(s(N)),X):-fib(s(N),Y),fib(N,Z),plus(Y,Z,X),.
                 
end of specification


// fib(s(s(s(s(zero)))),X).nil end