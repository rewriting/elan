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

/*
[] result term:
p(var(0),o)->var(0).
m(var(0),u)->var(0).
exp(o)->u.
p(p(var(0),var(1)),var(2))->p(var(0),p(var(1),var(2))).
m(m(var(0),var(1)),var(2))->m(var(0),m(var(1),var(2))).
exp(p(var(0),var(1)))->m(exp(var(0)),exp(var(1))).

p(var(0),p(o,var(1)))->p(var(0),var(1)).
m(var(0),m(u,var(1)))->m(var(0),var(1)).
nil
*/
