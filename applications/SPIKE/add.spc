specification  add

    vars x y z

    ars equalT:0#2 n_0:0#0 s:0#1 pl:1#1 True:0#0 False:0#0

//----------------------------------------------------------------------------------
// OK

sorts nat bol

constructors 

	n_0 	: -> nat
	s 	: nat -> nat

	True   	: -> bol
	False  	: -> bol

functions 

	pl : nat nat -> nat

axioms

	. =>  n_0 pl x = x,.
	//. =>  s(x) pl y = x pl s(y),.
	. =>  s(x) pl y = s(x pl y),.

E

	//. => x pl y = y pl x,.
	// prove([EmptyT,x pl y = y pl x:EmptyT])       end

	. => x pl n_0 = x,.
	// prove([EmptyT,x pl n_0 = x:EmptyT])       end

	//. => (x pl y) pl z = x pl (y pl z),.
	// prove([EmptyT,(x pl y) pl z = x pl (y pl z):EmptyT])       end

L
	. => .

less
	pl s n_0 #
equiv
	#
status
	pl:MS
	

end of specification


