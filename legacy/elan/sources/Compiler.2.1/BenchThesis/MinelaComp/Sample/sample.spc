specification sample

Types  ttt

Vars 
	x:ttt   y:ttt
	
Ops 
//	true	: --> boolean (builtin)
	a	: --> ttt
	f 	: ttt --> ttt
	g 	: ttt --> ttt
	h 	: ttt --> ttt
	i 	: ttt --> ttt

Labels	r1 r2 r3
	
Rules
	[r1] f(a) => a

	[r2] g(x) => f(f(y)) where y:=h(f(x))

	[r3] i(x) => y where y:=x
        nil
end of specification

/*
h(g(f(a)))

r1 	: h( g(a) )
r2[r1]	: h(f(f( h(f(a)) ))) --> h(f(f( h(a))))

[] result term:
   [h(f(f(h(a)))),proofnil;proofnil;h(g(("r1")));proofnil;h(("r2"(x->a)))[[proofnil;proofnil;h(("r1")).nil]]]


g(g(g(f(a))))

*/
