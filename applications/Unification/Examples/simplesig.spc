specification simplesig
    Vars  x  y  z  u  v
    Ops   f:2  g:1 a:0
	  div:2 I:1
end of specification

/*
 f(g(x),x)=f(g(a),a) end
 f(g(a),x)=f(g(x),x) end
 f(g(x),x)=f(g(a),y) end
 f(x,f(y,x))=f(a,f(f(z,f(u,v)),u)) end
*/
