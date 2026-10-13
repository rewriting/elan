module FullLookAheadMaximumDegreeInECandDC[Variables,Type,Values]

import
global
	LocalConsistencyStrategies[Variables,Type,Values]
	EnumerationRules[Variables,Type,Values]
	;
end

stratop
global
	FLAMaximumDegreeInECandDCFirstToLastAll			: < csp -> csp >	bs;
	FLAMaximumDegreeInECandDCFirstToLastOne			: < csp -> csp >	bs;
	FLAMaximumDegreeInECandDCLastToFirstAll			: < csp -> csp >	bs;
	FLAMaximumDegreeInECandDCLastToFirstOne			: < csp -> csp >	bs;
	FLAMaximumDegreeInECandDCSplitFirstToLastAll		: < csp -> csp >	bs;
	FLAMaximumDegreeInECandDCSplitFirstToLastOne		: < csp -> csp >	bs;
	FLAMaximumDegreeInECandDCSplitLastToFirstAll		: < csp -> csp >	bs;
	FLAMaximumDegreeInECandDCSplitLastToFirstOne		: < csp -> csp >	bs;
end


strategies for csp
implicit

/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: All					*/

[] FLAMaximumDegreeInECandDCFirstToLastAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInECandDC);
	dk (iterate* (EliminateFirstValueOfDomain));
	first one (InstantiateFirstValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	LocalConsistencyForEC
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: First					*/

[] FLAMaximumDegreeInECandDCFirstToLastOne =>
first one (FLAMaximumDegreeInECandDCFirstToLastAll)
end


/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration last to first	*/
/* Number of solutions	: All					*/

[] FLAMaximumDegreeInECandDCLastToFirstAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInECandDC);
	dk (iterate* (EliminateLastValueOfDomain));
	first one (InstantiateLastValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	LocalConsistencyForEC
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Value enumeration last to first	*/
/* Number of solutions	: First					*/

[] FLAMaximumDegreeInECandDCLastToFirstOne =>
first one (FLAMaximumDegreeInECandDCLastToFirstAll)
end

/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: All 					*/

[] FLAMaximumDegreeInECandDCSplitFirstToLastAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInECandDC);
	dk (first one (SplitDomainFirstMiddle), first one (SplitDomainSecondMiddle));
	first one (first one (ExtractConstraintsOnEqualityVar);
		   first one (Elimination, id);
		   LocalConsistencyForEC
		   ,
		   first one (ExtractConstraintsOnDomainVar);
		   LocalConsistencyForEC
		   ,
		   id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: First					*/

[] FLAMaximumDegreeInECandDCSplitFirstToLastOne =>
first one (FLAMaximumDegreeInECandDCSplitFirstToLastAll)
end


/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: All 					*/

[] FLAMaximumDegreeInECandDCSplitLastToFirstAll =>
LocalConsistencyForEC;
repeat* (
	first one (GetVarWithMaximumDegreeInECandDC);
	dk (first one (SplitDomainSecondMiddle), first one (SplitDomainFirstMiddle));
	first one (first one (ExtractConstraintsOnEqualityVar);
		   first one (Elimination, id);
		   LocalConsistencyForEC
		   ,
		   first one (ExtractConstraintsOnDomainVar);
		   LocalConsistencyForEC
		   ,
		   id)
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead			*/
/* Variable selection	: Maximum Degree			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: First					*/

[] FLAMaximumDegreeInECandDCSplitLastToFirstOne =>
first one (FLAMaximumDegreeInECandDCSplitLastToFirstAll)
end

end


rules for csop

	x, y				: var;
	ub, lb, newub, newlb, middle	: Type;
	P, Q				: csp;
	lmc, lec, EC, DC, store		: list[constraint];
	l				: list[constraint];

global

[FLAMaximumDegreeInECandDCFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCFirstToLastOne)P
end

[FLAMaximumDegreeInECandDCLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCLastToFirstOne)P
end

[FLAMaximumDegreeInECandDCSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCSplitFirstToLastOne)P
end

[FLAMaximumDegreeInECandDCSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCSplitLastToFirstOne)P
end

[GetUBFLAMaximumDegreeInECandDCFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAMaximumDegreeInECandDCFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAMaximumDegreeInECandDCLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAMaximumDegreeInECandDCLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAMaximumDegreeInECandDCSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAMaximumDegreeInECandDCSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAMaximumDegreeInECandDCSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAMaximumDegreeInECandDCSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAMaximumDegreeInECandDCFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAMaximumDegreeInECandDCLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAMaximumDegreeInECandDCSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAMaximumDegreeInECandDCSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAMaximumDegreeInECandDCSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

end


end
