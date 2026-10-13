specification spc

Btypes A ;B ;C;					// basic types
Mvars   Mx : A -> A				// unification variables
	My : B -> B				// context |- var:type
	Mz : A 
	Mv : B->A
	Mw : A
	Ms : B;
Bvars	Vx : A					// lambda variables
	Vy : B					// binding variables
	Vz : A->B
	Vu : B->A
	Vf : A->A				// functional symbols
	Vg : A->B
	Va : A
	Vb : A					// and constants
	Vc : B
	Vd : (A->A)->A;
end of specification



