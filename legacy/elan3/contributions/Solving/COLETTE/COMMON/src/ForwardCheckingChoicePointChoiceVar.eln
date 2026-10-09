module ForwardCheckingChoicePointChoiceVar[Variables,Type,Values]

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
	FCChoicePointChoiceVarFirstToLastAll      		: < csp -> csp >	bs;
	FCChoicePointChoiceVarFirstToLastOne      		: < csp -> csp >	bs;
	FCChoicePointChoiceVarLastToFirstAll      		: < csp -> csp >	bs;
	FCChoicePointChoiceVarLastToFirstOne      		: < csp -> csp >	bs;
	FCChoicePointChoiceVarSplitFirstToLastAll      		: < csp -> csp >	bs;
	FCChoicePointChoiceVarSplitFirstToLastOne      		: < csp -> csp >	bs;
	FCChoicePointChoiceVarSplitLastToFirstAll      		: < csp -> csp >	bs;
	FCChoicePointChoiceVarSplitLastToFirstOne      		: < csp -> csp >	bs;
end


strategies for csp
implicit

/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: All 					*/

[] FCChoicePointChoiceVarFirstToLastAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
	dk (iterate* (EliminateFirstValueOfDomain));
	first one (InstantiateFirstValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	first one (LocalConsistencyInEC, id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: First 				*/

[] FCChoicePointChoiceVarFirstToLastOne =>
first one (FCChoicePointChoiceVarFirstToLastAll)
end


/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Value enumeration last to first	*/
/* Number of solutions	: All 					*/

[] FCChoicePointChoiceVarLastToFirstAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
	dk (iterate* (EliminateLastValueOfDomain));
	first one (InstantiateLastValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	first one (LocalConsistencyInEC, id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection 	: Value enumeration last to first	*/
/* Number of solutions	: First 				*/

[] FCChoicePointChoiceVarLastToFirstOne =>
first one (FCChoicePointChoiceVarLastToFirstAll)
end


/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: All 					*/

[] FCChoicePointChoiceVarSplitFirstToLastAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
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


/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: First					*/

[] FCChoicePointChoiceVarSplitFirstToLastOne =>
first one (FCChoicePointChoiceVarSplitFirstToLastAll)
end


/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: All 					*/

[] FCChoicePointChoiceVarSplitLastToFirstAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
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


/* Strategy		: Forward Checking with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: First					*/

[] FCChoicePointChoiceVarSplitLastToFirstOne =>
first one (FCChoicePointChoiceVarSplitLastToFirstAll)
end

end


rules for csop

	x, y				: var;
	ub, lb, newub, newlb, middle	: Type;
	P, Q				: csp;
	lmc, lec, EC, DC, store		: list[constraint];
	l				: list[constraint];

global

[FCChoicePointChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCChoicePointChoiceVarFirstToLastOne)P
end

[FCChoicePointChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCChoicePointChoiceVarLastToFirstOne)P
end

[FCChoicePointChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCChoicePointChoiceVarSplitFirstToLastOne)P
end

[FCChoicePointChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FCChoicePointChoiceVarSplitLastToFirstOne)P
end

[GetUBFCChoicePointChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCChoicePointChoiceVarFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFCChoicePointChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCChoicePointChoiceVarLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFCChoicePointChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCChoicePointChoiceVarSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFCChoicePointChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FCChoicePointChoiceVarSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCChoicePointChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCChoicePointChoiceVarFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCChoicePointChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCChoicePointChoiceVarLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCChoicePointChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCChoicePointChoiceVarSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFCChoicePointChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FCChoicePointChoiceVarSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

end


end
