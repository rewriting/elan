specification  sorted

    vars x x1 x2 x3 x4 y z l

    ars equalT:0#2 n_0:0#0 s:0#1 True:0#0 False:0#0 Nil:0#0 Cons:0#2 lte:1#1 dif:0#2 length:0#1 count:0#2 insert:0#2 isort:0#1 sorted:0#1

//----------------------------------------------------------------------------------
//OK
sorts nat list bol 

constructors 

	n_0    :          -> nat
	s      : nat      -> nat
	True   :          -> bol
	False  :          -> bol
	Nil    :          -> list
	Cons   : nat list -> list

 functions

	lte      : nat nat  -> bol 
	dif      : nat nat  -> bol 
	length   : list     -> nat
	count    : nat list -> nat
	insert   : nat list -> list
	isort    : list     -> list
	sorted   : list     -> bol

axioms

	True=False,. => .

	. =>n_0  lte  x = True ,. 
	. =>s(x)  lte  n_0 = False,. 
	. =>s(x)  lte  s(y) = x  lte  y  ,.
      
	. =>dif(n_0,n_0)=False ,.
	. =>dif(n_0,s(x))=True ,.
	. =>dif(s(x),n_0)=True ,.
	. =>dif(s(x),s(y))=dif(x,y) ,.

	. =>length(Nil)=n_0,.
	. =>length(Cons(x,y))=s(length(y)),.

	. =>count(x,Nil)=n_0,.
	dif(x,y)=False,. => count(x,Cons(y,z)) = s(count(x,z)),.
	dif(x,y)=True,. => count(x,Cons(y,z)) = count(x,z),.

	. =>insert(x,Nil) = Cons(x,Nil) ,.
	x lte y=False,. => insert(x,Cons(y,z)) = Cons(y,insert(x,z)) ,.
	x lte y=True,.  => insert(x,Cons(y,z)) = Cons(x,Cons(y,z)) ,.

	. =>isort(Nil) = Nil ,.
	. =>isort(Cons(x,l)) = insert(x,isort(l)) ,.


	x lte y = True,.  => sorted(Cons(x,Cons(y,z))) = sorted(Cons(y,z)) ,.
	x lte y = False,. => sorted(Cons(x,Cons(y,z))) = False ,.

	. =>sorted(Cons(x,Nil)) = True ,.
	. =>sorted(Nil) = True ,.

// Lemme
//. => length(insert(x,y))=s(length(y)),.
//. => sorted(insert(x,y))=sorted(y),. .  
E

//	. => .

. => sorted(isort(x))=True,.
// prove([EmptyT,sorted(isort(x))=True:EmptyT])       end

//. => length(insert(x,y))=s(length(y)),. 
// prove([EmptyT,length(insert(x,y))=s(length(y)):EmptyT])       end

//. => sorted(insert(x,y))=sorted(y),. 
// prove([EmptyT,sorted(insert(x,y))=sorted(y):EmptyT])       end

L
	. => .

//. => sorted(insert(x,y))=sorted(y),. 

less
//	dummy #
isort insert sorted lte length count dif Cons Nil s n_0 True False  #

equiv
	dummy #
status
	dummy:LR
	

end of specification
