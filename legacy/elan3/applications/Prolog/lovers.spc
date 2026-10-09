specification lovers
  VarStop	20
  Vars 		X   Y   Z
  Ops 		jane:0   john:0   beer:0 toto:1
  Predicates 	love:2
  Clauses
		love(jane,john).
		love(john,beer).
		love(X,Y) :- love(X,Z),love(Z,Y),.
end of specification
