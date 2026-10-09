specification p8

Vars 
	x   y   z

Ops 
	f:2
	e1:0 e2:0 e3:0 e4:0 e5:0 e6:0 e7:0 e8:0
	i1:1 i2:1 i3:1 i4:1 i5:1 i6:1 i7:1 i8:1

Prec 
	e1:1 e2:2 e3:3 e4:4 e5:5 e6:6 e7:7 e8:8
	f:10 
	i1:11  i2:12 i3:13 i4:14 i5:15 i6:16 i7:17 i8:18

System
	f(f(x,y),z) = f(x,f(y,z)) 
	f(e1,x)=x 
	f(e2,x)=x 
	f(e3,x)=x 
	f(e4,x)=x 
	f(e5,x)=x 
	f(e6,x)=x 
	f(e7,x)=x 
	f(e8,x)=x 
	f(x,i1(x))=e1 
	f(x,i2(x))=e2 
	f(x,i3(x))=e3 
	f(x,i4(x))=e4 
	f(x,i5(x))=e5
	f(x,i6(x))=e6
	f(x,i7(x))=e7
	f(x,i8(x))=e8
	nil

end of specification

