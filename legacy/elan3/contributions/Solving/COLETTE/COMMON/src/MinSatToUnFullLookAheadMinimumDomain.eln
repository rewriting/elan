module MinSatToUnFullLookAheadMinimumDomain[Variables,Type,Values]

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
	MinSatToUnFLAMinimumDomainFirstToLast				: < csop -> csop >	bs;
	MinSatToUnFLAMinimumDomainLastToFirst				: < csop -> csop >	bs;
	MinSatToUnFLAMinimumDomainSplitFirstToLast			: < csop -> csop >	bs;
	MinSatToUnFLAMinimumDomainSplitLastToFirst			: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMinimumDomainFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMinimumDomainLastToFirst		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMinimumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointMinimumDomainSplitLastToFirst		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMinimumDomainFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMinimumDomainLastToFirst		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMinimumDomainSplitFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIMinimumDomainSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMinimumDomainFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMinimumDomainFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMinimumDomainFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMinimumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMinimumDomainLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMinimumDomainLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMinimumDomainLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMinimumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMinimumDomainSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMinimumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMinimumDomainSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMinimumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAMinimumDomainSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAMinimumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAMinimumDomainSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAMinimumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMinimumDomainFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMinimumDomainFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMinimumDomainFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMinimumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMinimumDomainLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMinimumDomainLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMinimumDomainLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMinimumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMinimumDomainSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMinimumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMinimumDomainSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMinimumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointMinimumDomainSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointMinimumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointMinimumDomainSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointMinimumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMinimumDomainFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMinimumDomainFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMinimumDomainFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMinimumDomainLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMinimumDomainLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMinimumDomainLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMinimumDomainSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMinimumDomainSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMinimumDomainSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIMinimumDomainSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIMinimumDomainSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIMinimumDomainSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIMinimumDomainSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end



