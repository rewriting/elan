specification peano

Types  entier boolean

Vars 
	x:entier   y:entier z:entier	b:boolean
	
Ops 
//	true	: --> boolean (builtin)
	o 	: --> entier
	s	: entier --> entier
	plus	: entier entier --> entier

Labels	r1 r2
	
Rules
	[r1] plus(x,s(y)) => s(z)
	  if true where z:=plus(x,y)

	[r2] plus(x,o) => x 
	  if b where b:=true
        nil
end of specification
