specification p2

Vars 
	x   y   z

Ops 
	f:2 e1:0 e2:0 i1:1 i2:1

Prec 
	e1:1 e2:2  f:3 i1:4  i2:5

System
	f(f(x,y),z) = f(x,f(y,z)) 
	f(e1,x)=x 
	f(e2,x)=x 
	f(x,i1(x))=e1 
	f(x,i2(x))=e2 
	nil

end of specification

