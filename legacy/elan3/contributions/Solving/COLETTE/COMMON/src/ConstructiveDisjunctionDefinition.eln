module ConstructiveDisjunctionDefinition[Variables,Type,Values]

import
global
	CSPDefinition[Variables,Type,Values]
	tuple[3,csp,list[csp],list[csp]]
	list[csp]
	;
end

sort

	cspCD;

end

operators
global
	CSPCD@		: (set:tuple[csp,list[csp],list[csp]])		cspCD;
	CreateCSPCD(@)	: (csp)						cspCD;
	GetCSP(@)	: (cspCD)					csp;

end


rules for cspCD

	P	: csp;

global

[]	CreateCSPCD(P)
	=>
	CSPCD[P,nil,nil]
end

end


rules for csp

	P		: csp;
	luCsp, lsCsp	: list[csp];

global

[]	GetCSP(CSPCD[P,luCsp,lsCsp])
	=>
	P
end

end


end
