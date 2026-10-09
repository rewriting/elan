specification group
Vars 
	x   y   z  

Ops 
	o:0 e:0 m:2 i:1 

Prec 
	i:4 m:3 o:1 e:1

System
	m(m(x,y),z)=m(x,m(y,z)) 
	x=m(o,x) 
	x=m(x,o) 
	o=m(i(x),x) 
	o=m(x,i(x)) 
	nil

end of specification

