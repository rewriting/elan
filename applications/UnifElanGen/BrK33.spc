specification BrK33
    Vars  x y z 
    Ops   a:0  f:1  g:4
end of specification


/* Perfect matching K3,3 graph

(    z.1 = g(f(x.11),x.12,x.13,y.1) & 
     z.2 = g(x.21,f(x.22),x.23,y.2) &
     z.3 = g(x.31,x.32,f(x.33),y.3) &
     z.4 = g(f(a),f(a),f(a),f(a)) &
     z.5 = g(f(a),f(a),f(a),f(f(a))) &
     z.6 = g(f(a),f(a),f(a),f(f(f(a)))),
     z.1 * z.2 * z.3 = z.4 * z.5 * z.6,
     true
     |
     z.1 . z.2 . z.3 . z.4 . z.5 . z.6 . nil,
     [nil,nil],
     nil) end


*/