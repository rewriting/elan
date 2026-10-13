specification rwAppend

Vars 
	x   y   z

Ops 
	a:0   b:0   c:0
	nil:0   cons:2   append:2 

Rules
	append(nil,x)       -> x 
	append(cons(x,y),z) -> cons(x,append(y,z))
	nil
end of specification
