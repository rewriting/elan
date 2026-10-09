specification p1

Vars 
	x   y   z

Ops 
	f:2 e1:0 i1:1

Prec 
	e1:1 f:6 i1:7 

System
	f(f(x,y),z) = f(x,f(y,z)) 
	f(e1,x)=x 
	f(x,i1(x))=e1 
	nil

end of specification

