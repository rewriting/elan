specification revfalse
Vars 
	x   y   z  

Ops 
	rev:1 app:2 cons:2 null:0

Prec 
	rev:4 app:3 cons:2 null:1

System
	rev(null)=null
	rev(cons(x,y))=app(rev(y),cons(x,null))
	app(null,x)=null
	app(cons(x,y),z)=cons(x,app(y,z))
	nil
end

/*
Result:
-------
rev(null)->null.
app(null,x)->null.
app(cons(x,y),z)->cons(x,app(y,z)).
rev(cons(x,y))->app(rev(y),cons(x,null)).
nil
*/