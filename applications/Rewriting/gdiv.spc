specification gdiv

Vars 
	x   y   z  u  v 

Ops 
	E:0 m:2 I:1 div:2
Rules
	I(I(x))->x	
	div(x,x)->E
	div(E,x)->x
	I(div(x,y))->div(y,x)
	m(x,y)->div(I(x),y)
	div(x,E)->I(x)
	div(x,div(I(x),y))->y
	I(E)->E
	div(I(x),div(x,y))->y
	div(div(x,y),z)->div(y,div(I(x),z))
	nil
end of specification

/* 
I(m(u,v))

m(I(v),I(u))

*/
