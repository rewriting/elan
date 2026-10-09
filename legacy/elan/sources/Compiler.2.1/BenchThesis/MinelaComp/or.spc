specification or

Types  	boolean

Vars 
	b	: boolean
Ops 
//	true	: --> boolean //(builtin)
	false	: --> boolean
	or	: boolean boolean --> boolean

Labels	or1 or2 or3
	
Rules
	[or1] or(b,true) => true
	[or2] or(true,b) => true
	[or3] or(false,false) => false
  nil
end of specification
