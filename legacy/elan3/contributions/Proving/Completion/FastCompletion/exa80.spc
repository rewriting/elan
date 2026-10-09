specification exa80
Vars 
	x   y   z  

Ops 
	o:0 m:2 i:1 

Prec 
	i:4 m:3 o:1 

System
	m(o,x)=x 
	m(x,i(x))=o 
	m(m(x,y),z)=m(x,m(y,z)) 
	nil

end of specification

