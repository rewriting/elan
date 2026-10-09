specification ctesig
    Vars  x  y  z  u v w
    Ops   a:0   b:0   c:0 d:0 e:0
end of specification

// x + y <=AC a + b + c end
// x + y <=AC a + b + c + d end
// y + x + y <=AC a + a + c + d end
// x <=AC a+b+b+b+c+a+c & x <=AC b+a+c+b+b+a+c end
// x <=AC a+b+b+c+a+c & x <=AC b+a+c+b+b+a+c end
// x +y+y <=AC a+a+b+b end


