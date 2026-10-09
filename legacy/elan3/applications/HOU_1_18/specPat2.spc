specification specPat2

// 3.2.2

// (la x. (la y. (la z . ((F z) y)))) = (la x. (la y. (la z . (x ((G y) x))))) , lt end

//[] result term:
//    false.nil

Btypes A;
Mvars   
	F : A -> (A -> A)
	G : A -> ((A -> A) -> A);
Bvars	x : A -> A
	y : A	
	z : A;
end of specification
