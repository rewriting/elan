specification append
Vars 
	x y z

Ops 
	append:2 cons:2 rev:1 nil:0

Rules
	append(nil,x)->x
	append(cons(x,y),z)->cons(x,append(y,z))
	nil
end of specification
