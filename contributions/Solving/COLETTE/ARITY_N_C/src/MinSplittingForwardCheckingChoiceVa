module MinSplittingForwardCheckingChoiceVar[Variables,Type,Values]

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
	MinSplittingFCChoiceVarFirstToLast				: < csop -> csop >	bs;
	MinSplittingFCChoiceVarLastToFirst				: < csop -> csop >	bs;
	MinSplittingFCChoiceVarSplitFirstToLast				: < csop -> csop >	bs;
	MinSplittingFCChoiceVarSplitLastToFirst				: < csop -> csop >	bs;
	MinSplittingFCChoicePointChoiceVarFirstToLast			: < csop -> csop >	bs;
	MinSplittingFCChoicePointChoiceVarLastToFirst			: < csop -> csop >	bs;
	MinSplittingFCChoicePointChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MinSplittingFCChoicePointChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
	MinSplittingFCChoicePointIIChoiceVarFirstToLast			: < csop -> csop >	bs;
	MinSplittingFCChoicePointIIChoiceVarLastToFirst			: < csop -> csop >	bs;
	MinSplittingFCChoicePointIIChoiceVarSplitFirstToLast		: < csop -> csop >	bs;
	MinSplittingFCChoicePointIIChoiceVarSplitLastToFirst		: < csop -> csop >	bs;
end


strategies for csop
implicit

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoiceVarFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoiceVarFirstToLastOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoiceVarFirstToLastOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoiceVarLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoiceVarLastToFirstOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoiceVarLastToFirstOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoiceVarSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoiceVarSplitFirstToLastOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoiceVarSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoiceVarSplitLastToFirstOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointChoiceVarFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointChoiceVarFirstToLastOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointChoiceVarFirstToLastOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointChoiceVarLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointChoiceVarLastToFirstOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointChoiceVarLastToFirstOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointChoiceVarSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointChoiceVarSplitFirstToLastOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointChoiceVarSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointChoiceVarSplitLastToFirstOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointIIChoiceVarFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointIIChoiceVarFirstToLastOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointIIChoiceVarFirstToLastOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointIIChoiceVarLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointIIChoiceVarLastToFirstOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointIIChoiceVarLastToFirstOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarLastToFirstOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointIIChoiceVarSplitFirstToLast =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointIIChoiceVarSplitFirstToLastOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointIIChoiceVarSplitFirstToLastOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarSplitFirstToLastOne);
first one (GetSolutionCSOP)
end


/* Optimization Strategy: */
/* Search Strategy	: */
/* Variable selection	: */
/* Value selection	: */

[] MinSplittingFCChoicePointIIChoiceVarSplitLastToFirst =>
first one (PostLTEUpperBound);
LocalConsistencyForEC;
first one (GetUBFCChoicePointIIChoiceVarSplitLastToFirstOne);
first one (UndervalueLowerBound);
repeat* (
	first one( first one (SetUpperBoundToMiddle);
		   first one (PostLTUpperBound);
		   LocalConsistencyForEC;
		   first one (GetUBFCChoicePointIIChoiceVarSplitLastToFirstOne)
		   ,
		   first one (SetLowerBoundToMiddle)
		 )
	);
first one (FCChoicePointIIChoiceVarSplitLastToFirstOne);
first one (GetSolutionCSOP)
end

end


end



