specification ex_automaton
Ops 
	a:0 b:0 cons:2 null:0

Automata


// Non-empty flat lists of a and b            
// i.e. lists of the form (b,a,a,a) 
//			  (b)
//			  ...			      

	Description of A(0)
		states  q|0 q|1 q|2 nil 
		final states q|0 nil
		transitions 
			cons(q|2, q|1) -> q|0     
			null -> q|1
			cons(q|2, q|1) -> q|1
			a -> q|2
			b -> q|2
			nil
	End of Description 



// Any list of a and b whose length is smaller 
// or equal to 3 			
// i.e. lists of the form  (a, a, a)
//			   ()
//			   ((), (), ())
//			   ((a), (a, (a)))
//			   ...		

		     	
	Description of A(1)
		states  q|0 q|1 q|2 q|3 q|4 nil 
		final states q|0 nil
		transitions 
			null -> q|0
			cons(q|1, q|2) -> q|0
			null -> q|1
			cons(q|1, q|2) -> q|1
			a -> q|1
			b -> q|1
			null -> q|2
			cons(q|1, q|3) -> q|2
			null -> q|3
			cons(q|1, q|4) -> q|3
			null -> q|4
			nil
	End of Description 


// Any list of a and b, where any a is before any b 
// and any b is before any embedded list	    
// i.e. lists of the form 	()
//				(a,b)
//				((a,b),((),()))
//				(a,a,a,a,b,b,b,(b),())
//				...

	Description of A(2)
		states q|0 q|1 q|2 q|3 q|4 nil 
		final states q|0 nil
		transitions 
			null -> q|0
			cons(q|3, q|0) -> q|0
			cons(q|4, q|1) -> q|0
			cons(q|0, q|2) -> q|0

			null -> q|1
			cons(q|4, q|1) -> q|1
			cons(q|0, q|2) -> q|1

			null -> q|2
			cons(q|0, q|2) -> q|2

			a -> q|3
			b -> q|4
			nil
	End of Description 
	nil
end of specification



/* examples of queries: 

	get the description of automaton called A(0)

	!A(0)


	compute the intersection between A(0) and A(1)

	!A(0) inter !A(1)

	
	compute the intersection between A(0) and A(1) (cleant and renamed)

	simplify(!A(0) inter !A(1))


	compute the union of A(1) and A(2) (cleant and renamed)

	simplify(!A(1) union !A(2))


	etc...

	simplify(!A(1) union simplify(!A(0) inter !A(2)))


*/
	