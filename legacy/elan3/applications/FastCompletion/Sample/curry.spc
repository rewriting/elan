specification curry
Vars 
	x y z

Ops 
	m:2 imp:2 I:0

Prec 
	I:2 m:4 imp:6

System
	imp(x,imp(y,z)) = imp(m(x,y),z)
	imp(x,m(y,z)) = m(imp(x,y),imp(x,z))
	imp(I,x)=x
	m(I,x)=x
	m(x,I)=x
	imp(x,I)=I
	m(m(x,y),z)=m(x,m(y,z))
	nil
end of specification
