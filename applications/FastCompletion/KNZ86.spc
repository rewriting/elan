specification KNZ86
Vars 
	x y z

Ops 
	s:1 o:0 f:2 h:1 g:2

Prec 
	g:5 f:4 s:2	// g>f>s
	o:4		// g>o
	h:3		// h>s

System
	s(s(x))=x
	f(o,y)=y
	f(s(x),y)=s(f(x,y))
	f(f(g(x,y),o),o)=g(x,y)
	g(o,y)=y
	g(s(x),y)=f(g(x,y),o)
	h(o)=s(o)
	nil
end of specification

/*
[] result term:
s(s(var(0)))->var(0).
f(o,var(0))->var(0).
g(o,var(0))->var(0).
f(s(var(0)),var(1))->s(f(var(0),var(1))).
h(o)->s(o).
g(s(var(0)),var(1))->f(g(var(0),var(1)),o).
f(f(var(0),o),o)->var(0).
nil
*/
