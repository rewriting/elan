specification member
  VarStop	20
  Vars 		X   Y   Z 
  Ops 		cons:2  nil:0 a:0 b:0 c:0 
  Predicates 	member:2 
  Clauses	member(X,cons(X,Y)).
                member(X,cons(Y,Z)) :- member(X,Z),.
end
