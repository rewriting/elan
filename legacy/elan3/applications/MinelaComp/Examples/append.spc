specification append

Types	elem list

Vars	x : elem   y : list   z : list  l : list

Ops 
	a 	: --> elem   
	b	: --> elem   
	nil	: --> list
	cons	: elem list --> list   
	append	: list list --> list 

Labels  r1 r2

Rules

	[r1] append( cons(x,y) , z ) => cons(x , l) where l:=append(y,z)

	[r2] append(nil,z)       => z
		if true
	nil

end of specification

