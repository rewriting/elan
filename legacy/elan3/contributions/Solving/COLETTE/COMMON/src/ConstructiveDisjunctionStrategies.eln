module ConstructiveDisjunctionStrategies[Variables,Type,Values]

import
global
	ConstructiveDisjunctionRules[Variables,Type,Values]
	;
end

stratop
global
	ConstructiveDisjunction				: < cspCD -> cspCD>		bs;
	LocalConsistencyForECWithCD1			: < csp -> csp >		bs;
	LocalConsistencyForECWithCD2			: < csp -> csp >		bs;
	LocalConsistencyForECandDCWithCD1		: < csp -> csp >		bs;
	LocalConsistencyForECandDCWithCD2		: < csp -> csp >		bs;
	LocalConsistencyForECandDCWithCD3		: < csp -> csp >		bs;
	LocalConsistencyForECandDCWithCD4		: < csp -> csp >		bs;
end


strategies for cspCD
implicit

[] ConstructiveDisjunction =>

first one (DecomposeCSPCD);
repeat* (first one (SolveSubCSPCD));
repeat* (first one (ComposeCSPCD))
end

end


rules for csp

	P, Q			: csp;
	D1, D2			: cspCD;
	lmc1, lmc2, lmc3, lmc4, lec1, lec2	: list[constraint];

global 

[ConstructiveDisjunction]
	P
	=>
	Q
	where	  D1 := ()CreateCSPCD(P)
	where	  D2 := (ConstructiveDisjunction)D1
	where	   Q := ()GetCSP(D2)
	where	lmc1 := ()GetLMC(P)
	where	lmc2 := ()TransformLECintoLMC(GetLEC(P))
	where	lmc3 := ()SortList(append(lmc1,lmc2),AllMembershipConstraints)
	where	lmc4 := ()SortList(GetLMC(Q),AllMembershipConstraints)
	if	not lmc3 == lmc4
end

end


strategies for csp
implicit

[] LocalConsistencyForECWithCD1 =>
LocalConsistencyForEC;
first one (ConstructiveDisjunction, id)
end

[] LocalConsistencyForECWithCD2 =>
LocalConsistencyForEC;
repeat* (first one (ConstructiveDisjunction);
	 LocalConsistencyForEC
	)
end

[] LocalConsistencyForECandDCWithCD1 =>
LocalConsistencyForECWithCD1;
repeat* (dk (PostElementaryDisjunct);
	 LocalConsistencyForEC
	)
end

[] LocalConsistencyForECandDCWithCD2 =>
LocalConsistencyForECWithCD2;
repeat* (dk (PostElementaryDisjunct);
	 LocalConsistencyForEC
	)
end

[] LocalConsistencyForECandDCWithCD3 =>
LocalConsistencyForECWithCD1;
repeat* (dk (PostElementaryDisjunct);
	 LocalConsistencyForECWithCD1
	)
end

[] LocalConsistencyForECandDCWithCD4 =>
LocalConsistencyForECWithCD2;
repeat* (dk (PostElementaryDisjunct);
	 LocalConsistencyForECWithCD2
	)
end

end


rules for csop

	x			: var;
	lb, ub			: Type;
	P, Q			: csp;

global

[LocalConsistencyForECWithCD1]

	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q := (LocalConsistencyForECWithCD1)P
end

end


end

