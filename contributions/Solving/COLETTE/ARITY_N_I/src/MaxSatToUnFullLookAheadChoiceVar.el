module MaxSatToUnFullLookAheadChoiceVar[Variables,Type,Values]

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
	MaxSatToUnFLAChoiceVarFirstToLast				: < csop -> csop >	bs;
	MaxSatToUnFLAChoiceVarLastToFirst				: < csop -> csop >	bs;
	MaxSatToUnFLAChoiceVarSplitFirstToLast				: < csop -> csop >	bs;
	MaxSatToUnFLAChoiceVarSplitLastToFirst				: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointChoiceVarFirstToLast			: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointChoiceVarLastToFirst			: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIChoiceVarFirstToLast			: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIChoiceVarLastToFirst			: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MaxSatToUnFLAChoicePointIIChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoiceVarFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoiceVarFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoiceVarFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoiceVarLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoiceVarLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoiceVarLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoiceVarSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoiceVarSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoiceVarSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoiceVarSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


[] MaxSatToUnFLAChoicePointChoiceVarFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointChoiceVarFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointChoiceVarFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointChoiceVarLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointChoiceVarLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointChoiceVarLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointChoiceVarSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointChoiceVarSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointChoiceVarSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointChoiceVarSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIChoiceVarFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIChoiceVarFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIChoiceVarFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIChoiceVarLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIChoiceVarLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIChoiceVarLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIChoiceVarSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIChoiceVarSplitFirstToLastOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSatToUnFLAChoicePointIIChoiceVarSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFLAChoicePointIIChoiceVarSplitLastToFirstOne);
repeat* (
	first one( first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFLAChoicePointIIChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToLowerBound)
		 )
	);
first one (FLAChoicePointIIChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end



