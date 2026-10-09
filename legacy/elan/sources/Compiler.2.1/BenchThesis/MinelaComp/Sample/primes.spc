specification primes

Types  	entier boolean list

Vars 
	x 	: entier
	y 	: list
	z	: list
	n	: entier
	i	: entier
	j	: entier
	b	: boolean
	q	: entier

Ops 
//	true	: --> boolean //(builtin)
	false	: --> boolean
	not	: boolean --> boolean
	or	: boolean boolean --> boolean
	o	: --> entier
	s	: entier --> entier
	plus	: entier entier --> entier
	moins	: entier entier --> entier
	mult	: entier entier --> entier
	div	: entier entier --> entier
	mod	: entier entier entier --> entier

	eq	: entier entier --> boolean
	gt	: entier entier --> boolean
	ge	: entier entier --> boolean

	nil     : --> list
        cons    : entier list --> list   
        append  : list list --> list 

	primes	: entier --> list
   	addifprime	:  entier list --> list

Labels	plus1 plus2 moins1 moins2 moins3 mult1 mult2
	mod1 mod2 eq1 eq2 eq3 eq4 gt1 gt2 gt3 ge1 not1 not2
	or1 or2 or3 append1 append2
	addifprime1 addifprime2 addifprime3 addifprime4
	primes1 primes2 primes3 primes4
	
Rules

        [plus1] plus(x,s(y)) => s(plus(x,y))
        
        [plus2] plus(x,o) => x 
        

	[moins1] moins(x,o) => x
  	
	[moins2] moins(o,x) => o
        
	[moins3] moins(s(x),s(y)) => moins(x,y)
 	

	[mult1] mult(x,o)	=> o
	
	[mult2] mult(x,s(y)) => plus(mult(x,y),x)
		

	[mod1] mod(x,y,q)	=> mod(moins(x,y),y,s(q))
		if ge(x,y)
	
	[mod2] mod(x,y,q)	=> x
		if not(ge(x,y))
	


	[eq1] eq(o,o) => true
        
	[eq2] eq(s(x),s(y)) => eq(x,y)
        
	[eq3] eq(o,s(x)) => false
        
	[eq4] eq(s(x),o) => false
        

	[gt1] gt(s(x),o) => true
	
	[gt2] gt(o,s(x)) => false
	
	[gt3] gt(s(x),s(y)) => gt(x,y)
	

	[ge1] ge(x,y) => or(gt(x,y) , eq(x,y))
	

	[not1] not(true) => false
 	
	[not2] not(false) => true
 	
	
	[or1] or(b,true) => true
	
	[or2] or(true,b) => true
	
	[or3] or(false,false) => false
	

	[append1] append( cons(x,y) , z ) => cons(x , append(y,z))
	
        [append2] append(nil,z)       => z
        

  	[addifprime1] addifprime(n,nil)		=> cons(n,nil)
	
	[addifprime2] addifprime(n,cons(i,y))	=> cons(i,append(y,cons(n,nil)))
		if gt(mult(i,i),n)
	
  	[addifprime3] addifprime(n,cons(i,y))	=> cons(i,y)
		if eq(mod(n,i,o),o)
	
	[addifprime4] addifprime(n,cons(i,y))	=> cons(i,addifprime(n,y))
	

  	[primes1] primes(o)		=> nil
	
  	[primes2] primes(s(o))		=> nil
	
  	[primes3] primes(s(s(o)))	=> cons(s(s(o)),nil)
	
  	[primes4] primes(n)		=> addifprime(n,primes(moins(n,s(o))))
	

  nil
end of specification

/*

primes( s(s(o)) )
4 : primes( s(s(s(s(o)))) )
5 : primes( s(s(s(s(s(o))))) )

7 : primes( s(s(s(s(s(s(s(o))))))) )
8 : primes( s(s(s(s(s(s(s(s(o)))))))) )
9 : primes( s(s(s(s(s(s(s(s(s(o))))))))) )

primes( s(s(s(s(s(s(s(s(s(s(o)))))))))) )
[] result term:
    cons(s(s(o)),cons(s(s(s(o))),cons(s(s(s(s(s(o))))),cons(s(s(s(s(s(s(s(o))))))),nil))))
[] statistics: time 2882.400 sec
            1970298 nonamed rules applied, 11800199 tried
            958480   named rules applied, 11340855 tried

                rule: applied   tried                 rule: applied   tried
           eliminate1       0       0            eliminate2       0       0
         MergingClash       0  124439            topOccRule  124439  124439
               delete     266  251853              coalesce       0       0
             conflict  123657  248096          extractrule1    9234   92912
         extractrule2    8466   85232            where_elim       0     782
             trueelim     762  124439              identity       0       0
              delete2       0  251587               delete3       0  251587
           truepropag      20  123677             inOccRule  561351 4162658
                match     782    9234   SymbolVariableClash       0  124439
              replace     768     782               if_elim      37     819
              trueadd  124439  124439             decompose    3491 5238621
           occ_check1       0       0            occ_check2       0       0
              rewrite     768     820           falsepropag       0       0
          orient_unif       0       0 


primes( s(s(s(o))) )
[] result term:
   [cons(s(s(o)),cons(s(s(s(o))),nil)),
	proofnil;("primes4"(n->s(s(s(o)))));addifprime(s(s(s(o))),primes(("moins3"(y->o o x->s(s(o))))));addifprime(s(s(s(o))),primes(("moins1"(x->s(s(o))))));addifprime(s(s(s(o))),("primes3"(identity)));("addifprime2"(y->nil o i->s(s(o))o n->s(s(s(o)))))[[gt(("mult2"(y->s(o)o x->s(s(o)))),s(s(s(o))));gt(("plus1"(x->mult(s(s(o)),s(o))o y->s(o))),s(s(s(o))));gt(s(("plus1"(y->o o x->mult(s(s(o)),s(o))))),s(s(s(o))));gt(s(s(("plus2"(x->mult(s(s(o)),s(o)))))),s(s(s(o))));gt(s(s(("mult2"(y->o o x->s(s(o)))))),s(s(s(o))));gt(s(s(("plus1"(x->mult(s(s(o)),o)o y->s(o))))),s(s(s(o))));gt(s(s(s(("plus1"(y->o o x->mult(s(s(o)),o)))))),s(s(s(o))));gt(s(s(s(s(("plus2"(x->mult(s(s(o)),o))))))),s(s(s(o))));gt(s(s(s(s(("mult1"(x->s(s(o)))))))),s(s(s(o))));("gt3"(y->s(s(o))o x->s(s(s(o)))));("gt3"(y->s(o)o x->s(s(o))));("gt3"(y->o o x->s(o)));("gt1"(x->o)).nil]];cons(s(s(o)),("append2"(z->cons(s(s(s(o))),nil))))]


*/


