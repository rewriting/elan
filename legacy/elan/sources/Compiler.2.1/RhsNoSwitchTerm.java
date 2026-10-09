import java.util.*;

public class RhsNoSwitchTerm implements RightHandSide {
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

  public RhsNoSwitchTerm(Term rhs,BranchEvaluation localEvaluations) {
    rightside=rhs;
    branch=localEvaluations;
  }

  public BranchEvaluation getBranch() {
    return branch;
  }
  public Term getRightside() {
    throw new InternalError("getRightside: not implemented");
    //return rightside;
  }

  public DetType getDetTypeEvaluation() {
    /*
     * si on ne met pas la 2eme condition, il y a un comportement
     * incoherent entre java et java -nojit
     */
    if(detTypeState==clear || detTypeState==start) {
      setDetTypeEvaluation();
    }
    if(detTypeState==done) {
      return detTypeEvaluation;
    } else {
      System.out.println("detTypeState = " + detTypeState);
      System.out.println("this = " + this);
      throw new InternalError("unknown detTypeState!=done");
    }
  }

  public void setDetTypeEvaluation() {
    detTypeState=start;
    detTypeEvaluation = DetType.detType;
    
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      detTypeEvaluation = detTypeEvaluation.and(locEval.getDetType());
    }
    detTypeState=done;
  } 

  public void genCode(OutputCode s, int deep,
		      RewriteRule rule, int number,
		      StrategyChooseRule sterm,
		      boolean isFirst, boolean isLast) {
    // local evaluations
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      locEval.genCode(s,deep,rule,number,sterm,isFirst,isLast);
    }
    s.write(deep,"/* rhs: " + rightside +" */\n");
    rightside.genRightside(s,deep);
  }


  public int getMaxVariableNumber(int max) {
    max = rightside.getMaxVariableNumber(max);
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      int r = locEval.getMaxVariableNumber(max);
      max=(r>max)?r:max;
    }
    return max;
  }

  public int getMaxUsedVariable(int max) {
    max = rightside.getMaxUsedVariable(max);
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      int r = locEval.getMaxUsedVariable(max);
      max=(r>max)?r:max;
    }
    return max;
  }

  public void rightsideVariableAffectation() {
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      // Hack: ne marche pas avec les matching condition AC
      int index=locEval.variableAffectation(0);
    }
    rightside.rightsideVariableAffectation();
  }

  public void rightsideVariableLiberation() {
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      locEval.variableLiberation();
    }
    rightside.rightsideVariableLiberation();
  }

  public void rightsideVariableClear(boolean full) {
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      locEval.variableClear(full);
    }
  }

  public void searchVariableShareFullNumbering(Term leftside) {
    rightside.searchVariableShareFullNumbering(leftside);
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval;
      locEval = getBranch().get(i);
      // if/where wrt. left
      locEval.searchVariableShareFullNumbering(leftside);

      // right wrt. where's leftside
      rightside.searchVariableShare(locEval.getLeftside());
      for(int j=0 ; j<i ; j++) {
	Term lhs = getBranch().get(j).getLeftside();
	// if/where wrt. previous where's leftside
	locEval.searchVariableShare(lhs);
      }
    }
  }

  public void flatten() {
    rightside.flatten();
  }

  public void orderedNormalForm() {
    rightside.orderedNormalForm();
  }

  public String toString() {
    String s;
    s=rightside.toString();
    for(int i=0 ; i<getBranch().size() ; i++) {
      s+= "\n\t";
      s+= getBranch().get(i);
    }
    return s;
  }


}
