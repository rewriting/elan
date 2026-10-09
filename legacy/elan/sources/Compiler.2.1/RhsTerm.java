import java.util.*;

public class RhsTerm implements RightHandSide {
  private Term rightside; // membre droit
  private BranchEvaluation branch;  // liste de if/where
  /**
   * to know if local evaluations are deterministic 
   */
  private DetType detTypeEvaluation;
  static private int clear=0;
  static private int start=1;
  static private int done=2;
  private int detTypeState=clear;

  public RhsTerm(Term rhs,BranchEvaluation localEvaluations) {
    rightside=rhs;
    branch=localEvaluations;
  }

  public BranchEvaluation getBranch() {
    return branch;
  }
  public Term getRightside() {
    return rightside;
  }

  public DetType getDetTypeEvaluation() {
    setDetTypeEvaluation();
    return detTypeEvaluation;
  }

  public void setDetTypeEvaluation() {
    detTypeState=start;
    detTypeEvaluation = DetType.detType;
    detTypeState=done;
  } 

  public void genCode(OutputCode s, int deep,
		      RewriteRule rule, int number,
		      StrategyChooseRule sterm,
		      boolean isFirst, boolean isLast) {
    s.write(deep,"/* rhs: " + rightside +" */\n");
    rightside.genRightside(s,deep);
  }


  public int getMaxVariableNumber(int max) {
    max = rightside.getMaxVariableNumber(max);
    return max;
  }

  public int getMaxUsedVariable(int max) {
    max = rightside.getMaxUsedVariable(max);
    return max;
  }

  public void rightsideVariableAffectation() {
    rightside.rightsideVariableAffectation();
  }

  public void rightsideVariableLiberation() {
    rightside.rightsideVariableLiberation();
  }

  public void searchVariableShareFullNumbering(Term leftside) {
    rightside.searchVariableShareFullNumbering(leftside);
  }

  public void flatten() {
    rightside.flatten();
  }

  public void orderedNormalForm() {
    rightside.orderedNormalForm();
  }

  public String toString() {
    return rightside.toString();
  }


}
