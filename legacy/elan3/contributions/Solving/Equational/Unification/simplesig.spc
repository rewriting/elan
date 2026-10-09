specification simplesig
    Vars  x  y  z  u v w
    Ops   a:0   b:0   c:0    f:2    g:1 
end of specification

// f(x,f(y,x))=f(a,f(f(z,f(u,v)),u)) end
// f(x,y)=f(a,b) & f(y,z) = f(g(c),x) & z = y end
// f(x,y)=f(a,w) & f(w,z) = f(g(c),x) & v = g(w) end
// g(x) = g(c) & z=g(g(a)) & y=g(z) end
// x = a & y = z & z = b & x=y end
