specification Zeh89
Vars 
	x y z

Ops 
	o:0 exp:1 p:2 m:2 u:0

Prec 
	exp:5 m:3 u:3

System
	p(p(x,y),z)=p(x,p(y,z))
	p(x,o)=x
	m(m(x,y),z)=m(x,m(y,z))
	m(x,u)=x
	exp(o)=u
	exp(p(x,y))=m(exp(x),exp(y))
	nil
end of specification

