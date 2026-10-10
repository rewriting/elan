specification p4

Vars 
	x   y   z

Ops 
	f:2 e1:0 e2:0 e3:0 e4:0 i1:1 i2:1 i3:1 i4:1

Prec 
	e1:1 e2:2 e3:3 e4:4 f:6 i1:7  i2:8 i3:9 i4:10

System
	f(f(x,y),z) = f(x,f(y,z)) 
	f(e1,x)=x 
	f(e2,x)=x 
	f(e3,x)=x 
	f(e4,x)=x 
	f(x,i1(x))=e1 
	f(x,i2(x))=e2 
	f(x,i3(x))=e3 
	f(x,i4(x))=e4 
	nil

end of specification

