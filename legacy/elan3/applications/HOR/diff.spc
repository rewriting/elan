specification diff

MVars
        Z:0     Y:0     U:0     X:0     X:1     U:1     V:1     

Vars
        x       y       z       pv      pv1     pv2     rv1
Ops
        d:1     D:2
        ln:0    exp:0   lam:1
        plus:2  minus:2 times:2 div:2   app:2   
        one:0   zero:0
Rules
        // Application simplification
        app(lam([rv1]X(rv1)),Y)         -> X(Y)                         // beta

        // Algebraic simplifications
        plus(zero,X)                    -> X
        plus(X,zero)                    -> X
        minus(X,zero)                   -> X
        times(one,X)                    -> X
        times(X,one)                    -> X
        times(zero,X)                   -> zero
        times(X,zero)                   -> zero
        div(X,one)                      -> X
        div(zero,X)                     -> zero
        times(div(one,X),div(one,Y))    -> div(one,times(X,Y))
        
        // Differentiation rules
        d(lam([rv1]X(rv1)))             -> lam([rv1]D([pv]X(pv),rv1))   // function
        d(ln)                           -> lam([pv]div(one,pv))         // ln
        d(exp)                          -> exp                          // exp
        D([rv1]Y,Z)                     -> zero                         // constant
        D([rv1]rv1,Z)                   -> one                          // identity
        D([rv1]plus(U(rv1),V(rv1)),Z)   -> plus(D([pv1]U(pv1),Z),D([pv2]V(pv2),Z))      // plus 
        D([rv1]minus(U(rv1),V(rv1)),Z)  -> minus(D([pv1]U(pv1),Z),D([pv2]V(pv2),Z))     // minus
        D([rv1]times(U(rv1),V(rv1)),Z)  -> 
                        plus(times(D([pv1]U(pv1),Z),V(Z)),times(U(Z),D([pv2]V(pv2),Z))) // times
        D([rv1]div(U(rv1),V(rv1)),Z)    -> 
                        div(minus(times(D([pv1]U(pv1),Z),V(Z)),times(U(Z),D([pv2]V(pv2),Z))),
                            times(V(Z),V(Z)))                           // div          
        D([rv1]app(U,V(rv1)),Z)         -> times(app(d(U),V(Z)),D([rv1]V(rv1),Z))       // chain

        nil
end of specification
