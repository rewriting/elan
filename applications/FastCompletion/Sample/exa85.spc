specification exa85
Vars 
	x   y   z  

Ops 
	e:0 p:2 i:1 h:1 

Prec 
	h:5 i:4 p:3 e:1 

System
	p(x,e)=x 
	p(x,p(y,z))=p(p(x,y),z) 
	p(x,i(x))=e 
	h(p(x,y))=p(h(x),h(y)) 
	nil

end of specification
