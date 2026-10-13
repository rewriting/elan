

specification lambdaMember

MVars
        X:0     Y:0     Q:0     Z:1

Vars
        xR      lsR     mR      x       y       z

Ops
        a:0     b:0     c:0     d:0     e:0     f:0     // constants
        eq:2                                            // for constants
        nil:0   cons:2  car:1   cdr:1   null:1  // lists
        T:0     F:0     iff:3                    // conditionals
        lam:1   app:2                           // lambda

        C:1

        memR:0  member:0

Rules
        // Rules for constants
        eq(C(X),C(X))   -> C(T)
        eq(C(X),C(Y))   -> C(F)

        // Rules for lists
        car(C(cons(X,Y)))       -> X
        cdr(C(cons(X,Y)))       -> Y
        null(C(nil))            -> C(T)
        null(C(X))              -> C(F)

        // Rules for conditionals
        iff(C(T),X,Y)            -> X
        iff(C(F),X,Y)            -> Y

        // Rules for lambda reduction
        app(lam([x]Z(x)),Y)     -> Z(Y)

        // Member function:
        memR -> lam([mR]
                        lam([xR]lam([lsR]
                                iff(null(lsR),C(F),
                                iff(eq(xR,car(lsR)),C(T),
                                app(app(app(mR,mR),xR),cdr(lsR)))))))
        member -> app(memR,memR)
        nil
end of specification
