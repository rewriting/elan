specification simplesig
    Vars  z  u v w
    Ops   a:0   b:0   c:0    f:2    g:1 
end of specification

// f(z,u)=f(a,w) /\ f(w,z) = f(g(c),v) end
// g(v) = g(c) /\ z=g(g(a)) /\ w=g(z) end
// v = a /\ w = z /\ z = b /\ v=w end

