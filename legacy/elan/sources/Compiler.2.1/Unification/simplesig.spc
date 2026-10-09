specification simplesig
    Vars  x  y  z  u v w
    Ops   a:0   b:0   c:0    f:2    g:1
end of specification

/*

  


  f(x,f(y,x))=f(a,f(f(z,f(u,v)),u)) end
  (y=f(z,f(a,v))&(x=a&u=a))

  (&)*+(
	(=((u),a)){1},
	(=((y),f(,)((z),f(,)(a,(v))))){2},
	(=((x),a)){2})


   f(x,h(y,z)) = 
   

*/