specification specPat1


// (la x. (la y. (la z . ((F z) y)))) = (la x. (la y. (la z . (z ((G y) x))))) , slt end

//[] result term:
//    true-AND-(la A.(la A.(V_Mvars(6)[^o^]#1[^])))=G-AND-(la(A->A).(la A.(#2(V_Mvars(6)[^o^]#1))))=F.nil

// 3.2.1

Btypes  A;
Mvars  	F : (A -> A) -> (A -> A) 
	G : A -> (A -> A);
Bvars	x : A				
	y : A	
	z : A->A;

end of specification

