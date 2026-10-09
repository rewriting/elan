module MaxSatToUnFullLookAheadMinimumDomain[Variables,Type,Values]

import
global
	OptimizationRules[Variables,Type,Values]
	FullLookAheadMinimumDomain[Variables,Type,Values]
	FullLookAheadChoicePointMinimumDomain[Variables,Type,Values]
	FullLookAheadChoicePointIIMinimumDomain[Variables,Type,Values]
	;
end

stratop
global
	MaxSatToUnFLAMinimumDomainFirstToLast				: < csop -> csop >	bs;
	MaxSatToUnFLAMinimumDomainLastToFirst				: < csop -> csop >	bs;
	MaxSatToUnFLAMinimumDomainSplitFirstToLast			: < csop -> csop >	bs;
	MaxSatToUnFLAMinimumDomainSplitLastToFirst			: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMinimumDomainFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMinimumDomainLastToFirst		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMinimumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMinimumDomainSplitLastToFirst		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMinimumDomainFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMinimumDomainLastToFirst		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMinimumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMinimumDomainSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMinimumDomainFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMinimumDomainFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMinimumDomainFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMinimumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMinimumDomainLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMinimumDomainLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMinimumDomainLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMinimumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMinimumDomainSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMinimumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMinimumDomainSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMinimumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMinimumDomainSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMinimumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMinimumDomainSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMinimumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMinimumDomainFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMinimumDomainFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMinimumDomainFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMinimumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMinimumDomainLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMinimumDomainLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMinimumDomainLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMinimumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMinimumDomainSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMinimumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMinimumDomainSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMinimumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMinimumDomainSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMinimumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMinimumDomainSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMinimumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMinimumDomainFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMinimumDomainFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMinimumDomainFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMinimumDomainLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMinimumDomainLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMinimumDomainLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMinimumDomainSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMinimumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMinimumDomainSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMinimumDomainSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMinimumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMinimumDomainSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end



