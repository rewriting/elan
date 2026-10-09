specification specPat3

// 3.2.3

// (la x. (la y. (la z . ((F z) y)))) = (la x. (la y. (la z . (z ((F y) x))))) , lt end

//[] result term:
//    false.nil

Btypes  A;
Mvars   F : (A -> A) -> ((A -> A) -> A);
Bvars	x : A -> A	
	y : A -> A
	z : A -> A;
end of specification
