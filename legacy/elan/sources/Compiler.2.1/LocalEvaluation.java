import java.util.*;

interface LocalEvaluation {

  void searchVariableShare(Term t);
  void reverseSearchVariableShare(Term t);
  void reverseSearchVariableShare(LocalEvaluation locEval);
  void selfSearchVariableShare();
  void searchVariableShare(LocalEvaluation locEval); // Test

  void searchVariableShareFullNumbering(Term lhs);
  Term getLeftside();
  int variableAffectation(int index);
  void variableLiberation();
  void variableClear(boolean full);
  void markUsedVariable(BitSet b);
  int getMaxVariableNumber(int max);
  int getMaxUsedVariable(int max);
  int getMaxSubstIndex(int max);
  DetType getDetType();
  void genCode(OutputCode s,int deep,
	       RewriteRule rule, int number,
	       StrategyChooseRule sterm,
	       boolean isFirst, boolean isLast);
  void setFullNumbering();

  boolean dependFrom(Term t);
  String toString(int deep);
}

