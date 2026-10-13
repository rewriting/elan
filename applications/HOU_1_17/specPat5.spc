specification specPat5

// 3.2.5

// (la x.(la y.(la z.((F z) y)))) = (la x.(la y.(la z.((F y) x)))),slt end
//
//[] result term:
//   (la V_Bvars(1).(la V_Bvars(2).V_Mvars(3)))=F.nil

Btypes A;
Mvars  	F : A -> (A -> A);
Bvars	x : A		
	y : A		
	z : A;     
end of specification
