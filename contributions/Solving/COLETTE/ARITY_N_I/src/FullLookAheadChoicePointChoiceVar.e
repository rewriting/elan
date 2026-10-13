module FullLookAheadChoicePointChoiceVar[Variables,Type,Values]

import
global
	LocalConsistencyStrategies[Variables,Type,Values]
	EnumerationRules[Variables,Type,Values]
	;
end

stratop
global
	FLAChoicePointChoiceVarFirstToLastAll      		: < csp -> csp >	bs;
	FLAChoicePointChoiceVarFirstToLastOne      		: < csp -> csp >	bs;
	FLAChoicePointChoiceVarLastToFirstAll      		: < csp -> csp >	bs;
	FLAChoicePointChoiceVarLastToFirstOne      		: < csp -> csp >	bs;
	FLAChoicePointChoiceVarSplitFirstToLastAll      	: < csp -> csp >	bs;
	FLAChoicePointChoiceVarSplitFirstToLastOne      	: < csp -> csp >	bs;
	FLAChoicePointChoiceVarSplitLastToFirstAll      	: < csp -> csp >	bs;
	FLAChoicePointChoiceVarSplitLastToFirstOne      	: < csp -> csp >	bs;
end


strategies for csp
implicit

/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: All					*/

[] FLAChoicePointChoiceVarFirstToLastAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
	dk (iterate* (EliminateFirstValueOfDomain));
	first one (InstantiateFirstValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	LocalConsistencyForEC
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Value enumeration first to last	*/
/* Number of solutions	: First					*/

[] FLAChoicePointChoiceVarFirstToLastOne =>
first one (FLAChoicePointChoiceVarFirstToLastAll)
end


/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Value enumeration last to first	*/
/* Number of solutions	: All					*/

[] FLAChoicePointChoiceVarLastToFirstAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
	dk (iterate* (EliminateLastValueOfDomain));
	first one (InstantiateLastValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	LocalConsistencyForEC
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Value enumeration last to first	*/
/* Number of solutions	: First					*/

[] FLAChoicePointChoiceVarLastToFirstOne =>
first one (FLAChoicePointChoiceVarLastToFirstAll)
end


/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: All 					*/

[] FLAChoicePointChoiceVarSplitFirstToLastAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
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


/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain first to last		*/
/* Number of solutions	: First					*/

[] FLAChoicePointChoiceVarSplitFirstToLastOne =>
first one (FLAChoicePointChoiceVarSplitFirstToLastAll)
end


/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: All 					*/

[] FLAChoicePointChoiceVarSplitLastToFirstAll =>
dk (LocalConsistencyForECandDC);
repeat* (
	first one (GetVarSpecifiedByUser);
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


/* Strategy		: Full Lookahead with Choice Point	*/
/* Variable selection	: Specified by the user			*/
/* Value selection	: Split domain last to first		*/
/* Number of solutions	: First					*/

[] FLAChoicePointChoiceVarSplitLastToFirstOne =>
first one (FLAChoicePointChoiceVarSplitLastToFirstAll)
end

end


rules for csop

	x, y				: var;
	ub, lb, newub, newlb, middle	: Type;
	P, Q				: csp;
	lmc, lec, EC, DC, store		: list[constraint];
	l				: list[constraint];

global

[FLAChoicePointChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAChoicePointChoiceVarFirstToLastOne)P
end

[FLAChoicePointChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAChoicePointChoiceVarLastToFirstOne)P
end

[FLAChoicePointChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAChoicePointChoiceVarSplitFirstToLastOne)P
end

[FLAChoicePointChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAChoicePointChoiceVarSplitLastToFirstOne)P
end

[GetUBFLAChoicePointChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointChoiceVarFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAChoicePointChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointChoiceVarLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAChoicePointChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointChoiceVarSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAChoicePointChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointChoiceVarSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointChoiceVarFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointChoiceVarLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointChoiceVarSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointChoiceVarSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

end


end
