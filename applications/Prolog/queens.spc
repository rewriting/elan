specification queens
  VarStop	300
  Vars	 	X D P L Dp Pd Diff Diff1 P1 PP1 P2 PP2 P3 PP3 P4 
  Ops 		cons:2  nil:0 
  Predicates 	set:1 ok:3 eq:2 queens:1
  Clauses
		set(1). set(2). set(3). set(4).
		ok(Diff,D,nil).
		ok(Diff,D,cons(P,L)) :- D\=P, Dp is D-P, Dp\=Diff,
			Pd is P-D, Pd\=Diff, Diff1 is Diff+1, ok(Diff1,D,L),.
		eq(X,X).

		queens(X) :- set(P1),
			set(P2),eq(PP1,cons(P1,nil)),
			ok(1,P2,PP1),
			set(P3),eq(PP2,cons(P2,PP1)),
			ok(1,P3,PP2),
			set(P4),eq(PP3,cons(P3,PP2)),
			ok(1,P4,PP3),
			eq(X,cons(P4,PP3)),.
//  VarStop	300
end of specification
