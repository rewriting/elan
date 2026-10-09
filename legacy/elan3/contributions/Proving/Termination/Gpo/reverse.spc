specification reverse
Vars 
	x y z

Ops 
	append:2 cons:2 rev:1 nil:0

Rules
	append(nil,x)->x
	append(cons(x,y),z)->cons(x,append(y,z))
	rev(nil)->nil
	rev(cons(x,y))->append(rev(y),cons(x,nil))
	nil
end of specification




/*

[] result term:
    precedence=(prec(append>cons)^(prec(rev>append)^(prec(rev>cons)^(prec(cons>nil)^Tinterp))))


[] end


enter query term finished by the key word 'end':


[ executionAbort Continue Dump Exit Statistics
  changeTrace changeQuiet Input Output stRategy (ACDESTQIOR|acdestqior)] ?s

 Statistics:
 total time     (13.860+0.000)=13.860 sec       (main+subprocesses)



*/
