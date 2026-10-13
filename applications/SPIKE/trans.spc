specification trans

    vars x y z w

    ars equalT:0#2  n_0:0#0 s:0#1 True:0#0 False:0#0 null:0#0 Cons:0#2 dif:0#2 member:0#2 subsetp:0#2

//----------------------------------------------------------------------------------
sorts nat list bol 

constructors 

n_0 	: ->nat
s 	: nat -> nat
True 	: -> bol
False 	: -> bol
null 	: -> list
Cons 	: nat  list -> list

functions 

member	: nat  list -> bol
subsetp	: list  list -> bol
dif	: nat nat -> bol

axioms
   
True = False,. => .     

		. =>  member(x,null)=False,.
dif(x,y)=False,	. => member(x,Cons(y,z))=True,.
dif(x,y)=True,	. => member(x,Cons(y,z))=member(x,z),.

			. => subsetp(null,w)=True,.
member(x,w)=True,	. => subsetp(Cons(x,y),w)=subsetp(y,w),.
member(x,w)=False,	. => subsetp(Cons(x,y),w)=False,.


	. =>  dif(n_0,n_0)=False,.
	. =>  dif(n_0,s(x))=False,.
	. =>  dif(s(x),n_0)=True,.
	. =>  dif(s(x),s(y))=dif(x,y),.


//member(x,z)=True,subsetp(z,y)=True,. =>   member(x,y)=True,. //LEMME 1

E

subsetp(x,y)=True,subsetp(y,z)=True,. =>   subsetp(x,z)=True,.
// prove([subsetp(x,y)=True:subsetp(y,z)=True:EmptyT,subsetp(x,z)=True:EmptyT])       end
 
dif(x,y)=False,member(y,z)=True,. =>   member(x,z)=True,.
// prove([dif(x,y)=False:member(y,z)=True:EmptyT,member(x,z)=True:EmptyT])       end
 
member(x,z)=True,subsetp(z,y)=True,. =>   member(x,y)=True,. 
// prove([member(x,z)=True:subsetp(z,y)=True:EmptyT, member(x,y)=True:EmptyT])       end

L
//	. => .
//. =>   dif(x,y)=True,member(y,z)=False,member(x,z)=True,.
//member(x,z)=True,subsetp(z,y)=True,. =>   member(x,y)=True,. 


. =>  dif(x,y)=True,member(y,z)=False,member(x,z)=True,.
. =>  subsetp(z,y)=False,member(x,z)=False, member(x,y)=True,.

less

	subsetp Cons member dif False #

equiv

	False True  #

status
	dummy:LR
	

end of specification








