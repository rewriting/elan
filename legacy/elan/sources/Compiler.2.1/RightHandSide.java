import java.util.*;

interface RightHandSide {

  public BranchEvaluation getBranch();
  public Term getRightside(); // Hack: Only for RhsTerm

  public DetType getDetTypeEvaluation();
  public void setDetTypeEvaluation();
  public void genCode(OutputCode s, int deep,
		      RewriteRule rule, int number,
		      StrategyChooseRule sterm,
		      boolean isFirst, boolean isLast);

  public int getMaxVariableNumber(int max);
  public int getMaxUsedVariable(int max);
  public void rightsideVariableAffectation();
  public void rightsideVariableLiberation();
  public void searchVariableShareFullNumbering(Term leftside);
  public void flatten();
  public void orderedNormalForm();
  public String toString();

}
