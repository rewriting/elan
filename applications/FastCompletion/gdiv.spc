specification gdiv
Vars 
	x   y   z  

Ops 
	E:0 m:2 I:1 div:2

Prec 
	E:0 m:4 I:2 div:2

System
	m(I(x),x) = E
	m(E,x) = x
	m(m(x,y),z) = m(x,m(y,z))
	div(x,y)=m(I(x),y)
	nil
end of specification

/*
Ans
[] statistics: time 112.995 sec
            128115 nonamed rules applied, 455333 tried
            26618   named rules applied, 270858 tried
[] result term:
I(I(var(0)))->var(0).
div(var(0),var(0))->E.
div(E,var(0))->var(0).
I(div(var(0),var(1)))->div(var(1),var(0)).
m(var(0),var(1))->div(I(var(0)),var(1)).
div(var(0),E)->I(var(0)).
div(var(0),div(I(var(0)),var(1)))->var(1).
I(E)->E.
div(I(var(0)),div(var(0),var(1)))->var(1).
div(div(var(0),var(1)),var(2))->div(var(1),div(I(var(0)),var(2))).
nil



Cans instancie
[] statistics: time 218.608 sec
            245734 nonamed rules applied, 777368 tried
            43904   named rules applied, 402202 tried
[] result term:
    div(var(0),var(0))->E[true].div(E,var(0))->var(0)[true].I(I(var(0)))->var(0)[true].div(var(0),div(I(var(0)),var(1)))->var(1)[true].m(var(0),var(1))->div(I(var(0)),var(1))[true].I(E)->E[true].div(var(0),E)->I(var(0))[true].div(I(var(0)),div(var(0),var(1)))->var(1)[true].I(div(var(0),var(1)))->div(var(1),var(0))[true].div(div(var(0),var(1)),var(2))->div(var(1),div(I(var(0)),var(2)))[true].nil

Cans + Propagation
[] statistics: time 257.773 sec
            245804 nonamed rules applied, 810653 tried
            55079   named rules applied, 528155 tried

*/




