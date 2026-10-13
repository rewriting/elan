specification exa


//// see capitulation

Btypes A; B; C;					// basic types
Mvars   X : A -> A				// unification variables
	Y : B -> B				// context |- var:type
	Z : A 
	V : B->A
	W : A
	S : B;
Bvars	x : A					// lambda variables
	y : B					// binding variables
	z : A->B
	u : B->A
	f : A->A				// functional symbols
	g : A->B
	a : A
	b : A					// and constants
	c : B
	d : (A->A)->A;
end of specification



