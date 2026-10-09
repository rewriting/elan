module ForwardCheckingMaximumDegreeInEC[Variables,Type,Values]

import
global
	LocalConsistencyStrategies[Variables,Type,Values]
	EnumerationRules[Variables,Type,Values]
	PropagationRules[Variables,Type,Values]
	ChoicePointStrategies[Variables,Type,Values]
	;
end

stratop
global
	FCMaximumDegreeInECFirstToLastAll		: < csp -> csp >	bs;
	FCMaximumDegreeInECFirstToLastOne		: < csp -> csp >	bs;
	FCMaximumDegreeInECLastToFirstAll		: < csp -> csp >	bs;
	FCMaximumDegreeInECLastToFirstOne		: < csp -> csp >	bs;
	FCMaximumDegreeInECSplitFirstToLastAll		: < csp -> csp >	bs;
	FCMaximumDegreeInECSplitFirstToLastOne		: < csp -> csp >	bs;
	FCMaximumDegreeInECSplitLastToFirstAll		: < csp -> csp >	bs;
	FCMaximumDegreeInECSplitLastToFirstOne		: < csp -> csp >	bs;
end


strategies for csp
implicit

/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: All 					*/

[] FCMaximumDegreeInECFirstToLastAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInEC);
	dk (iterate* (EliminateFirstValueOfDomain));
	first one (InstantiateFirstValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	first one (LocalConsistencyInEC, id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: First					*/

[] FCMaximumDegreeInECFirstToLastOne =>
first one (FCMaximumDegreeInECFirstToLastAll)
end


/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration last to first	*/
/* Number of solutions	: All 					*/

[] FCMaximumDegreeInECLastToFirstAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInEC);
	dk (iterate* (EliminateLastValueOfDomain));
	first one (InstantiateLastValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	first one (LocalConsistencyInEC, id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration last to first	*/
/* Number of solutions	: First					*/

[] FCMaximumDegreeInECLastToFirstOne =>
first one (FCMaximumDegreeInECLastToFirstAll)
end


/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: All 					*/

[] FCMaximumDegreeInECSplitFirstToLastAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInEC);
	dk (first one (SplitDomainFirstMiddle), first one (SplitDomainSecondMiddle));
	first one (first one (ExtractConstraintsOnEqualityVar);
		   first one (Elimination, id);
		   first one (LocalConsistencyInEC, id)
		   ,
		   first one (ExtractConstraintsOnDomainVar);
		   first one (LocalConsistencyInEC, id)
		   ,
		   id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: First					*/

[] FCMaximumDegreeInECSplitFirstToLastOne =>
first one (FCMaximumDegreeInECSplitFirstToLastAll)
end


/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: All 					*/

[] FCMaximumDegreeInECSplitLastToFirstAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInEC);
	dk (first one (SplitDomainSecondMiddle), first one (SplitDomainFirstMiddle));
	first one (first one (ExtractConstraintsOnEqualityVar);
		   first one (Elimination, id);
		   first one (LocalConsistencyInEC, id)
		   ,
		   first one (ExtractConstraintsOnDomainVar);
		   first one (LocalConsistencyInEC, id)
		   ,
		   id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Forward Checking			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: First					*/

[] FCMaximumDegreeInECSplitLastToFirstOne =>
first one (FCMaximumDegreeInECSplitLastToFirstAll)
end

end


rules for csop

	x, y				: var;
	ub, lb, newub, newlb, middle	: Type;
	P, Q				: csp;
	lmc, lec, EC, DC, store		: list[constraint];
	l				: list[constraint];

global

[FCMaximumDegreeInECFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCMaximumDegreeInECFirstToLastOne)P
end

[FCMaximumDegreeInECLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCMaximumDegreeInECLastToFirstOne)P
end

[FCMaximumDegreeInECSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCMaximumDegreeInECSplitFirstToLastOne)P
end

[FCMaximumDegreeInECSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCMaximumDegreeInECSplitLastToFirstOne)P
end

[GetUBFCMaximumDegreeInECFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCMaximumDegreeInECFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFCMaximumDegreeInECLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCMaximumDegreeInECLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFCMaximumDegreeInECSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCMaximumDegreeInECSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFCMaximumDegreeInECSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCMaximumDegreeInECSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCMaximumDegreeInECFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCMaximumDegreeInECFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCMaximumDegreeInECLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCMaximumDegreeInECLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCMaximumDegreeInECSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCMaximumDegreeInECSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCMaximumDegreeInECSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCMaximumDegreeInECSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

end


end
