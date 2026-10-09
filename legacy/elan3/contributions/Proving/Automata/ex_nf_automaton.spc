specification ex_nf_automaton
Vars	x y z 

Ops 
	a:0 b:0 cons:2 append:2 null:0
R1	
	append(null, x) -> x
	append(cons(x, y), z) -> cons(y, append(y, z))
	nil

Automata

// Terms of the form append(l1, l2) where l1 et l2 are 
// any flat lists of a and b.
//

	Description of A(0)
	states q|0.q|1.q|2nil
	final states q|0.nil
	transitions append(q|1, q|1) -> q|0.
		cons(q|2, q|1) -> q|1.
		null -> q|1.
		a -> q|2.
		b -> q|2.
		nil
	End of Description	
	nil	
end of specification


/*  examples of queries:

	get the description of automaton called A(0)

	!A(0)


	
	compute the approximation automaton of R1 on A(0)
	(superset of R1*(L(A(0)), i.e. R1-descendants of L(A(0)))

	T_up(R1) on(!A(0))


	
	build the automaton recognising the set of R1-irreducible terms
	(irr(R1))

	build_nf(R1)


	
	compute the approximation of R1-normal forms of L(A(0)), i.e. 
	superset of R1!(L(A(0)).

	simplify(T_up(R1) on(!A(0)) inter build_nf(R1))


*/
