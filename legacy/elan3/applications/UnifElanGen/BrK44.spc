specification BrK44
    Vars  x y z 
    Ops   a:0  f:1  g:5
end of specification


/* Perfect matching K4,4 graph
   with combi0plusFA

(    z.1 = g(f(x.11),x.12,x.13,x.14,y.1) & 
     z.2 = g(x.21,f(x.22),x.23,x.24,y.2) &
     z.3 = g(x.31,x.32,f(x.33),x.34,y.3) &
     z.4 = g(x.41,x.42,x.43,f(x.44),y.4) &
     z.5 = g(f(a),f(a),f(a),f(a),f(a)) &
     z.6 = g(f(a),f(a),f(a),f(a),f(f(a))) &
     z.7 = g(f(a),f(a),f(a),f(a),f(f(f(a)))) &
     z.8 = g(f(a),f(a),f(a),f(a),f(f(f(f(a))))),
     z.1 * z.2 * z.3 * z.4 = z.5 * z.6 * z.7 * z.8,
     true
     |
     z.1 . z.2 . z.3 . z.4 . z.5 . z.6 . z.7 . z.8 . nil,
     [nil,nil],
     nil) end


*/

/* Perfect matching K4,4 graph
   with combiBrKnn

(    z.1 = g(f(x.11),x.12,x.13,x.14,y.1) & 
     z.2 = g(x.21,f(x.22),x.23,x.24,y.2) &
     z.3 = g(x.31,x.32,f(x.33),x.34,y.3) &
     z.4 = g(x.41,x.42,x.43,f(x.44),y.4) &
     z.5 = g(f(a),f(a),f(a),f(a),f(a)) &
     z.6 = g(f(a),f(a),f(a),f(a),f(f(a))) &
     z.7 = g(f(a),f(a),f(a),f(a),f(f(f(a)))) &
     z.8 = g(f(a),f(a),f(a),f(a),f(f(f(f(a))))),
     z.1 * z.2 * z.3 * z.4 = z.5 * z.6 * z.7 * z.8,
     true
     |
     z.1 . z.2 . z.3 . z.4 . nil,
     [z.5 . z.6 . z.7 . z.8 . nil,nil],
     nil) end


*/

