module MinSatToUnFullLookAheadMaximumDomain[Variables,Type,Values]

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
	MinSatToUnFLAMaximumDomainFirstToLast				: < csop -> csop >	bs;
	MinSatToUnFLAMaximumDomainLastToFirst				: < csop -> csop >	bs;
	MinSatToUnFLAMaximumDomainSplitFirstToLast			: < csop -> csop >	bs;
	MinSatToUnFLAMaximumDomainSplitLastToFirst			: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMaximumDomainFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMaximumDomainLastToFirst		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMaximumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMaximumDomainSplitLastToFirst		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMaximumDomainFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMaximumDomainLastToFirst		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMaximumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMaximumDomainSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMaximumDomainFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMaximumDomainFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMaximumDomainFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMaximumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMaximumDomainLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMaximumDomainLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMaximumDomainLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMaximumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMaximumDomainSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMaximumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMaximumDomainSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMaximumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMaximumDomainSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMaximumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMaximumDomainSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMaximumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMaximumDomainFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMaximumDomainFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMaximumDomainFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMaximumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMaximumDomainLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMaximumDomainLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMaximumDomainLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMaximumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMaximumDomainSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMaximumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMaximumDomainSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMaximumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMaximumDomainSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMaximumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMaximumDomainSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMaximumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMaximumDomainFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMaximumDomainFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMaximumDomainFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMaximumDomainLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMaximumDomainLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMaximumDomainLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMaximumDomainSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMaximumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMaximumDomainSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMaximumDomainSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMaximumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMaximumDomainSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMaximumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end



