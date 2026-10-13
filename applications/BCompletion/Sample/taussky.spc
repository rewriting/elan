specification taussky
Vars 
	x   y   z  

Ops 
	E:0 m:2 I:1 F:2 G:2

Prec 
	E:0 m:2 I:4 F:6 G:8

System
	m(I(x),x) = E
	m(E,E) = E
	m(m(x,y),z) = m(x,m(y,z))
	F(m(y,x),x) = G(m(y,x), y)
	F(E, x) = x
	nil
end of specification

/*
// Huggy
[] statistics: time 158.444 sec
            517134 nonamed rules applied, 1467609 tried
            95318   named rules applied, 1182561 tried
// Huggy
[] statistics: time 877.848 sec
            797466 nonamed rules applied, 2796048 tried
            175625   named rules applied, 1955967 tried
[] result term:
    m(E,var(0))->var(0).m(var(0),E)->var(0).F(E,var(0))->var(0).I(I(var(0)))->var(0).m(I(var(0)),m(var(0),var(1)))->var(1).I(E)->E.m(I(var(0)),var(0))->E.m(var(0),I(var(0)))->E.m(var(0),m(I(var(0)),var(1)))->var(1).m(m(var(0),var(1)),var(2))->m(var(0),m(var(1),var(2))).G(var(0),var(1))->F(var(0),m(I(var(1)),var(0))).I(m(var(0),var(1)))->m(I(var(1)),I(var(0))).nil


Cans instancie
[] statistics: time 861.466 sec
            763412 nonamed rules applied, 2415623 tried
            158242   named rules applied, 1627801 tried

[] result term:
    F(E,var(0))->var(0)[true].m(E,var(0))->var(0)[true].m(var(0),E)->var(0)[true].I(I(var(0)))->var(0)[true].m(I(var(0)),var(0))->E[true].m(I(var(0)),m(var(0),var(1)))->var(1)[true].I(E)->E[true].m(var(0),m(I(var(0)),var(1)))->var(1)[true].m(var(0),I(var(0)))->E[true].m(m(var(0),var(1)),var(2))->m(var(0),m(var(1),var(2)))[true].G(var(0),var(1))->F(var(0),m(I(var(1)),var(0)))[true].I(m(var(0),var(1)))->m(I(var(1)),I(var(0)))[true].nil



// Robin
[] statistics: time 620.400 sec
            517134 nonamed rules applied, 1467609 tried
            95318   named rules applied, 1182561 tried
[] result term:
    F(E,var(0))->var(0).m(E,var(0))->var(0).m(var(0),E)->var(0).I(I(var(0)))->var(0).m(I(var(0)),var(0))->E.m(I(var(0)),m(var(0),var(1)))->var(1).I(E)->E.m(var(0),I(var(0)))->E.m(var(0),m(I(var(0)),var(1)))->var(1).m(m(var(0),var(1)),var(2))->m(var(0),m(var(1),var(2))).G(var(0),var(1))->F(var(0),m(I(var(1)),var(0))).I(m(var(0),var(1)))->m(I(var(1)),I(var(0))).nil


  [ main ] result :F(E,var(0))->var(0).m(E,var(0))->var(0).m(var(0),E)->var(0).I(I(var(0)))->var(0).m(I(var(0)),var(0))->E.m(I(var(0)),m(var(0),var(1)))->var(1).I(E)->E.m(var(0),I(var(0)))->E.m(var(0),m(I(var(0)),var(1)))->var(1).m(m(var(0),var(1)),var(2))->m(var(0),m(var(1),var(2))).G(var(0),var(1))->F(var(0),m(I(var(1)),var(0))).I(m(var(0),var(1)))->m(I(var(1)),I(var(0))).nil
  [ main ] stop :
   Statistics :
  517134( or 517134) nonamed reductions 95318 named reductions
  517134 nonamed tried 142814 named tried
  117599 fails
  real 5.2 user 3.5 sys 0.1
		4.9
*/

