module FullLookAheadChoicePointIIChoiceVar[Variables,Type,Values]

import
global
	LocalConsistencyStrategies[Variables,Type,Values]
	EnumerationRules[Variables,Type,Values]
	;
end

stratop
global
	FLAChoicePointIIChoiceVarFirstToLastAll      		: < csp -> csp >	bs;
	FLAChoicePointIIChoiceVarFirstToLastOne      		: < csp -> csp >	bs;
	FLAChoicePointIIChoiceVarLastToFirstAll      		: < csp -> csp >	bs;
	FLAChoicePointIIChoiceVarLastToFirstOne      		: < csp -> csp >	bs;
	FLAChoicePointIIChoiceVarSplitFirstToLastAll		: < csp -> csp >	bs;
	FLAChoicePointIIChoiceVarSplitFirstToLastOne		: < csp -> csp >	bs;
	FLAChoicePointIIChoiceVarSplitLastToFirstAll		: < csp -> csp >	bs;
	FLAChoicePointIIChoiceVarSplitLastToFirstOne		: < csp -> csp >	bs;
end


strategies for csp
implicit

/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Value enumeration first to last			*/
/* Number of solutions	: All							*/

[] FLAChoicePointIIChoiceVarFirstToLastAll =>
LocalConsistencyForEC;
repeat* (
	dk (PostElementaryDisjunct);
	LocalConsistencyForEC;
	first one (GetVarSpecifiedByUser);
	dk (iterate* (EliminateFirstValueOfDomain));
	first one (InstantiateFirstValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	LocalConsistencyForEC
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Value enumeration first to last			*/
/* Number of solutions	: First							*/

[] FLAChoicePointIIChoiceVarFirstToLastOne =>
first one (FLAChoicePointIIChoiceVarFirstToLastAll)
end


/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Value enumeration last to first			*/
/* Number of solutions	: All							*/

[] FLAChoicePointIIChoiceVarLastToFirstAll =>
LocalConsistencyForEC;
repeat* (
	dk (PostElementaryDisjunct);
	LocalConsistencyForEC;
	first one (GetVarSpecifiedByUser);
	dk (iterate* (EliminateLastValueOfDomain));
	first one (InstantiateLastValueOfDomain);
	first one (ExtractConstraintsOnEqualityVar, id);
	first one (Elimination, id);
	LocalConsistencyForEC
	);
first one (GetSolutionCSP)
end


/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Value enumeration last to first			*/
/* Number of solutions	: First							*/

[] FLAChoicePointIIChoiceVarLastToFirstOne =>
first one (FLAChoicePointIIChoiceVarLastToFirstAll)
end


/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Split domain first to last				*/
/* Number of solutions	: All 							*/


[] FLAChoicePointIIChoiceVarSplitFirstToLastAll =>
LocalConsistencyForEC;
repeat* (
	dk (PostElementaryDisjunct);
	LocalConsistencyForEC;
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


/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Split domain first to last				*/
/* Number of solutions	: First							*/

[] FLAChoicePointIIChoiceVarSplitFirstToLastOne =>
first one (FLAChoicePointIIChoiceVarSplitFirstToLastAll)
end


/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Split domain last to first				*/
/* Number of solutions	: All 							*/

[] FLAChoicePointIIChoiceVarSplitLastToFirstAll =>
LocalConsistencyForEC;
repeat* (
	dk (PostElementaryDisjunct);
	LocalConsistencyForEC;
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


/* Strategy		: Full Lookahead with Choice Point and Enumeration	*/
/* Variable selection	: Specified by the user					*/
/* Value selection	: Split domain last to first				*/
/* Number of solutions	: First							*/

[] FLAChoicePointIIChoiceVarSplitLastToFirstOne =>
first one (FLAChoicePointIIChoiceVarSplitLastToFirstAll)
end

end


rules for csop

	x, y				: var;
	ub, lb, newub, newlb, middle	: Type;
	P, Q				: csp;
	lmc, lec, EC, DC, store		: list[constraint];
	l				: list[constraint];

global

[FLAChoicePointIIChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAChoicePointIIChoiceVarFirstToLastOne)P
end

[FLAChoicePointIIChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=(FLAChoicePointIIChoiceVarLastToFirstOne)P
end

[FLAChoicePointIIChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=()P
end

[FLAChoicePointIIChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[Q,x,lb,ub]
	where	Q :=()P
end

[GetUBFLAChoicePointIIChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointIIChoiceVarFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAChoicePointIIChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointIIChoiceVarLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAChoicePointIIChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointIIChoiceVarSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetUBFLAChoicePointIIChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,lb,newub]
	where	Q :=(FLAChoicePointIIChoiceVarSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newub.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointIIChoiceVarFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointIIChoiceVarFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointIIChoiceVarLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointIIChoiceVarLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointIIChoiceVarSplitFirstToLastOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointIIChoiceVarSplitFirstToLastOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

[GetLBFLAChoicePointIIChoiceVarSplitLastToFirstOne]
	CSOP[P,x,lb,ub]
	=>
	CSOP[P,x,newlb,ub]
	where	Q :=(FLAChoicePointIIChoiceVarSplitLastToFirstOne)P
	if	Q != Unsatisfiable
	where	(list[constraint]) y = newlb.l := ()GetVarInHead(x,GetLEC(Q),nil)
end

end


end
