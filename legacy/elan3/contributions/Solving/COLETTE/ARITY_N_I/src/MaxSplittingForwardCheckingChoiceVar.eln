module MaxSplittingForwardCheckingChoiceVar[Variables,Type,Values]

import
global
	OptimizationRules[Variables,Type,Values]
	ForwardCheckingChoiceVar[Variables,Type,Values]
	ForwardCheckingChoicePointChoiceVar[Variables,Type,Values]
	ForwardCheckingChoicePointIIChoiceVar[Variables,Type,Values]
	;
end

stratop
global
	MaxSplittingFCChoiceVarFirstToLast				: < csop -> csop >	bs;
	MaxSplittingFCChoiceVarLastToFirst				: < csop -> csop >	bs;
	MaxSplittingFCChoiceVarSplitFirstToLast				: < csop -> csop >	bs;
	MaxSplittingFCChoiceVarSplitLastToFirst				: < csop -> csop >	bs;
	MaxSplittingFCChoicePointChoiceVarFirstToLast			: < csop -> csop >	bs;
	MaxSplittingFCChoicePointChoiceVarLastToFirst			: < csop -> csop >	bs;
	MaxSplittingFCChoicePointChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MaxSplittingFCChoicePointChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
	MaxSplittingFCChoicePointIIChoiceVarFirstToLast			: < csop -> csop >	bs;
	MaxSplittingFCChoicePointIIChoiceVarLastToFirst			: < csop -> csop >	bs;
	MaxSplittingFCChoicePointIIChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MaxSplittingFCChoicePointIIChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoiceVarFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoiceVarFirstToLastOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoiceVarFirstToLastOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoiceVarLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoiceVarLastToFirstOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoiceVarLastToFirstOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoiceVarSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoiceVarSplitFirstToLastOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoiceVarSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoiceVarSplitLastToFirstOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointChoiceVarFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointChoiceVarFirstToLastOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointChoiceVarFirstToLastOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointChoiceVarLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointChoiceVarLastToFirstOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointChoiceVarLastToFirstOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointChoiceVarSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointChoiceVarSplitFirstToLastOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointChoiceVarSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointChoiceVarSplitLastToFirstOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointIIChoiceVarFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointIIChoiceVarFirstToLastOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointIIChoiceVarFirstToLastOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointIIChoiceVarLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointIIChoiceVarLastToFirstOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointIIChoiceVarLastToFirstOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointIIChoiceVarSplitFirstToLast =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointIIChoiceVarSplitFirstToLastOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointIIChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MaxSplittingFCChoicePointIIChoiceVarSplitLastToFirst =>
first one (PostGTELowerBound);
LocalConsistencyForEC;
first one (GetLBFCChoicePointIIChoiceVarSplitLastToFirstOne);
first one (OvervalueUpperBound);
repeat* (
	first one( first one (SetLowerBoundToMiddle);
		   first one (PostGTLowerBound);
		   LocalConsistencyForEC;
		   first one (GetLBFCChoicePointIIChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetUpperBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end



