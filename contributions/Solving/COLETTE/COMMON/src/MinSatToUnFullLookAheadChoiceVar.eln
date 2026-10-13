module MinSatToUnFullLookAheadChoiceVar[Variables,Type,Values]

import
global
	OptimizationRules[Variables,Type,Values]
	FullLookAheadChoiceVar[Variables,Type,Values]
	FullLookAheadChoicePointChoiceVar[Variables,Type,Values]
	FullLookAheadChoicePointIIChoiceVar[Variables,Type,Values]
	;
end

stratop
global
	MinSatToUnFLAChoiceVarFirstToLast				: < csop -> csop >	bs;
	MinSatToUnFLAChoiceVarLastToFirst				: < csop -> csop >	bs;
	MinSatToUnFLAChoiceVarSplitFirstToLast				: < csop -> csop >	bs;
	MinSatToUnFLAChoiceVarSplitLastToFirst				: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointChoiceVarFirstToLast			: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointChoiceVarLastToFirst			: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIChoiceVarFirstToLast			: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIChoiceVarLastToFirst			: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MinSatToUnFLAChoicePointIIChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoiceVarFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoiceVarFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoiceVarFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoiceVarLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoiceVarLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoiceVarLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoiceVarSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoiceVarSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoiceVarSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoiceVarSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointChoiceVarFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointChoiceVarFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointChoiceVarFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointChoiceVarLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointChoiceVarLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointChoiceVarLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointChoiceVarSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointChoiceVarSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointChoiceVarSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointChoiceVarSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIChoiceVarFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIChoiceVarFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIChoiceVarFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIChoiceVarLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIChoiceVarLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIChoiceVarLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIChoiceVarSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIChoiceVarSplitFirstToLastOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSatToUnFLAChoicePointIIChoiceVarSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFLAChoicePointIIChoiceVarSplitLastToFirstOne);
repeat* (
	first one( first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFLAChoicePointIIChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToUpperBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end
