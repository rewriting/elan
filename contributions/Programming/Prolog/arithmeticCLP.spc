specification arithmeticCLP

     Vars         N X Y Z W
     Ops          s:1 zero:0
     Predicates   plus:3 mult:3  exp:3  fact:2
     Clauses

/* X,Y,Z are natural numbers such that 
        Z is the sum (product) of X and Y 
*/
                  plus(zero,X,X).
                  plus(s(X),Y,s(Z)):-plus(X,Y,Z),.

                  mult(zero,X,zero).
                  mult(s(X),Y,Z):-mult(X,Y,W),plus(W,Y,Z),.

/* X,Y,N are natural numbers such that 
        Y is  X exponent n
*/
                  exp(s(zero),zero,zero).
                  exp(zero,s(X),s(zero)).
                  exp(s(N),X,Y):-exp(N,X,Z),mult(Z,X,Y),.

                  fact(zero,X):-X=s(zero),.
                  fact(s(N),X):-fact(N,Z),mult(s(N),Z,X),.  
               

end of specification
// fact(s(s(s(zero))),X).nil end
// fact(s(s(zero)),X).X=s(zero).nil end
