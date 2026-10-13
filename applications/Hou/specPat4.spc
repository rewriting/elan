specification specPat4

// (la x.(la y.(la z.(la w.((((F y) z) x) x))))) = (la x.(la y.(la z.(la w.((((F z) y) w) x))))),slt end
//
//[] result term:
//    true-AND-(la V_Bvars(1).(la V_Bvars(2).(la V_Bvars(3).(la V_Bvars(4).((((V_Mvars(7)V_Bvars(3))V_Bvars(2))V_Bvars(4))V_Bvars(1))))))=(la V_Bvars(1).(la V_Bvars(2).(la V_Bvars(3).(la V_Bvars(4).((((V_Mvars(7)V_Bvars(2))V_Bvars(3))V_Bvars(1))V_Bvars(1))))))-AND-V_Mvars(7)=F.nil

Btypes A;
Mvars  	F : A -> (A -> (A -> (A -> A)));
Bvars	x : A	
	y : A	
	z : A
	w : A;
end of specification
