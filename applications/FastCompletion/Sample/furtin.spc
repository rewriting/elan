specification furtin
Vars 
	x y z

Ops 
	m:2 e:0 i:1 
	c1:0 c2:0 c3:0 c4:0 c5:0 c6:0 c7:0 c8:0 c9:0
	c10:0 c11:0 c12:0 c13:0 c14:0 c15:0 c16:0
	g1:0 g2:0
Prec 
	e:1 m:3 i:5 g1:7 g2:9
	c1:10 c2:11 c3:12
	c4:11
	c5:11 c9:12
	c6:12 c7:12
	c8:12
	c10:13 c11:14
	c12:13
	c13:10 c14:11 c15:12
	c16:11

System
 m(x , e) = x 
 m(x , m(y , z)) = m(m(x , y) , z) 
 m(x , i(x)) = e 
 m(e , x) = x 
 m(i(x) , x) = e 
 i(e) = e 
 i(m(x , y)) = m(i(y) , i(x)) 
 m(x , m(i(x) , y)) = y 
 m(i(x) , m(x , y)) = y 
 i(i(x)) = x 
 c1 = e 
 c2 = m(c1 , g2) 
 c3 = m(c2 , g2)  
 c4 = m(c1 , i(g2)) 
 c5 = m(c1 , g1) 
 c6 = m(c5 , g2) 
 c7 = m(c6 , g2) 
 c8 = m(c5 , i(g2)) 
 c9 = m(c5 , g1) 
 c10 = m(c9 , g2) 
 c11 = m(c10 , g2) 
 c12 = m(c9 , i(g2)) 
 c13 = m(c1 , i(g1)) 
 c14 = m(c13 , g2) 
 c15 = m(c14 , g2) 
 c16 = m(c13 , i(g2)) 
 m(g1 , m(g1 , m(g1 , g1))) = e 
 m(g2 , m(g2 , m(g2 , g2))) = e 
	nil
end of specification
