specification gdiv

Vars 
	x   y   z  u  v 

Ops 
	the:0 m:2 thi:1 div:2
Rules
	thi(thi(x))->x	
	div(x,x)->the
	div(the,x)->x
	thi(div(x,y))->div(y,x)
	m(x,y)->div(thi(x),y)
	div(x,the)->thi(x)
	div(x,div(thi(x),y))->y
	thi(the)->the
	div(thi(x),div(x,y))->y
	div(div(x,y),z)->div(y,div(thi(x),z))
	nil
end of specification

/* 
thi(m(u,v))

m(thi(v),thi(u))

*/
