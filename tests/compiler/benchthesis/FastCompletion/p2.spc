specification p2

Vars 
	x   y   z

Ops 
	f:2 e1:0 e2:0 i1:1 i2:1

Prec 
	e1:1 e2:2 f:6 i1:7  i2:8

System
	f(f(x,y),z) = f(x,f(y,z)) 
	f(e1,x)=x 
	f(e2,x)=x 
	f(x,i1(x))=e1 
	f(x,i2(x))=e2 
	nil

end of specification

