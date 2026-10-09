specification specPat


//// see capitulation

Btypes  A; 					// basic types
Mvars   F : (A -> A) -> ((A -> A) -> A)
	G : (A -> A) -> ((A -> A) -> A)
        H : A
        I : A
        H1 : A -> A
        I1 : A -> A;
Bvars	x : A -> A				// lambda variables
	y : A -> A				// binding variables
	z : A -> A ;
end of specification
