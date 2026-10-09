specification ex_termination

Vars none

Ops
	f:1 g:1 a:0 b:0
R1
	f(a) -> b
	nil

R2
	a -> b
	g(b) -> g(f(a))
	nil

Automata 
	Description of A(0)
	states q|0.q|1.q|2 nil
	final states q|0.nil
	transitions f(q|0) -> q|0.
		a -> q|0.
		g(q|1) -> q|0.
		g(q|1) -> q|1.
		a -> q|1.
		nil
	End of Description	
	nil	
end of specification


/*

	examples of queries:

	prove the termination of sequential reduction relation on T(F)
	
	start



	prove the termination of sequential reduction relation on L(A(0))

	start(!A(0))


*/
