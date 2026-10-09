module MaxSatToUnFullLookAheadMaximumDomain[Variables,Type,Values]

import
global
	OptimizationRules[Variables,Type,Values]
	FullLookAheadMaximumDomain[Variables,Type,Values]
	FullLookAheadChoicePointMaximumDomain[Variables,Type,Values]
	FullLookAheadChoicePointIIMaximumDomain[Variables,Type,Values]
	;
end

stratop
global
	MaxSatToUnFLAMaximumDomainFirstToLast				: < csop -> csop >	bs;
	MaxSatToUnFLAMaximumDomainLastToFirst				: < csop -> csop >	bs;
	MaxSatToUnFLAMaximumDomainSplitFirstToLast			: < csop -> csop >	bs;
	MaxSatToUnFLAMaximumDomainSplitLastToFirst			: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMaximumDomainFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMaximumDomainLastToFirst		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMaximumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointMaximumDomainSplitLastToFirst		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMaximumDomainFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMaximumDomainLastToFirst		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMaximumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIMaximumDomainSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMaximumDomainFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMaximumDomainFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMaximumDomainFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMaximumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMaximumDomainLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMaximumDomainLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMaximumDomainLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMaximumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMaximumDomainSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMaximumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMaximumDomainSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMaximumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAMaximumDomainSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAMaximumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAMaximumDomainSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAMaximumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMaximumDomainFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMaximumDomainFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMaximumDomainFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMaximumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMaximumDomainLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMaximumDomainLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMaximumDomainLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMaximumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMaximumDomainSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMaximumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMaximumDomainSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMaximumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointMaximumDomainSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointMaximumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointMaximumDomainSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointMaximumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMaximumDomainFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMaximumDomainFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMaximumDomainFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMaximumDomainLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMaximumDomainLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMaximumDomainLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMaximumDomainSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMaximumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMaximumDomainSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIMaximumDomainSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIMaximumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIMaximumDomainSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end



