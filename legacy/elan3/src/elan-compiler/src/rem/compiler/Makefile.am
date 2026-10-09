javadir         = ${prefix}/classes/rem/compiler
CLASSPATH_ENV   = CLASSPATH=$(JAVAROOT):$(srcdir)/$(JAVAROOT):$$CLASSPATH

BOOTSOURCES	= \
ACDDTree.java		Pair.java		  StrategyTerm.java\
AllocVariable.java	PatternCode.java	  StrategyTraversalAll.java\
BranchEvaluation.java	PatternInfo.java	  StrategyTraversalOne.java\
Choice.java		Query.java		  StrategyTraversalSome.java\
Condition.java		RewriteRule.java	  Symbol.java\
DDNode.java		Strategy.java		  SymbolAC.java\
DDTree.java		StrategyCall.java	  SymbolApply.java\
DetType.java		StrategyChooseRule.java   SymbolBuiltin.java\
ElanConstants.java	StrategyCons.java	  SymbolCode.java\
Flags.java		StrategyDcRule.java	  SymbolDstr.java\
Flatterm.java		StrategyDcStrat.java	  SymbolExtensionVariable.java\
Information.java	StrategyDkRule.java	  SymbolFree.java\
Lexem.java		StrategyDkStrat.java	  SymbolFsym.java\
LexemChar.java		StrategyEval.java	  SymbolIdentifier.java\
LexemIdentifier.java	StrategyFail.java	  SymbolInteger.java\
LexemModule.java	StrategyId.java		  SymbolLab.java\
LexemNum.java		StrategyIterate.java	  SymbolString.java\
LexemRuleName.java	StrategyMeta.java	  SymbolValue.java\
LexemSort.java		StrategyNormInRule.java   SymbolVariable.java\
LexemStrategyName.java	StrategyNormOutRule.java  Term.java\
LexemVariableName.java	StrategyOneRule.java	  TermInformation.java\
LocalAffectation.java	StrategyOneStrat.java	  Tools.java\
LocalEvaluation.java	StrategyRepeat.java\
OutputCode.java		StrategyRuleName.java StrategyRewriteStep.java

java_JAVA       = $(BOOTSOURCES)

JAVAROOT        = ${top_builddir}/src
EXTRA_DIST      = $(BOOTSOURCES)

