import java.util.*;

public class RhsSwitchTerm implements RightHandSide {
  private Vector caseTerm; // liste de cles (Term)
  private Vector caseRhs;  // liste de membres droits (RightHandSide)
  private BranchEvaluation branch;  // liste de if/where
  /**
   * to know if local evaluations are deterministic 
   */
  private DetType detTypeEvaluation;
  static private int clear=0;
  static private int start=1;
  static private int done=2;
  private int detTypeState=clear;

  private int getNumberOfCase() {
    return caseTerm.size();
  }

  private Term getCaseTerm(int i) {
    return (Term) caseTerm.elementAt(i);
  }

  private RightHandSide getCaseRhs(int i) {
    return (RightHandSide) caseRhs.elementAt(i);
  }

  public RhsSwitchTerm(Vector caseTerm, Vector caseRhs,
		       BranchEvaluation localEvaluations) {
    this.caseTerm=caseTerm;
    this.caseRhs=caseRhs;
    branch=localEvaluations;
  }

  public BranchEvaluation getBranch() {
    return branch;
  }
  public Term getRightside() {
    throw new InternalError("getRightside: not implemented");
    //return null;
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
	 throw new InternalError("unknown detTypeState!=done");
    }
  }

  public void setDetTypeEvaluation() {
    detTypeState=start;
    detTypeEvaluation = DetType.detType;

    // Typage des wheres eventuels
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      detTypeEvaluation = detTypeEvaluation.and(locEval.getDetType());
    }
    // Typage des cases
    for(int i=0 ; i<getNumberOfCase() ; i++) {
      detTypeEvaluation = detTypeEvaluation.and(getCaseRhs(i).getDetTypeEvaluation());
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
    
    //for(int i=0 ; i<getNumberOfCase() ; i++) {
    // s.write(deep,"/* case: " + getCaseTerm(i) + " */\n");
    // getCaseTerm(i).rgenRightside(s,deep);
    // s.write(deep,"if(" + getCaseTerm(i).genCore() + " == con_1) {\n");
    // getCaseRhs(i).genCode(s,deep+1,rule,number,sterm,isFirst,isLast);
    // s.write(deep+1,"goto endCase" + number + ";\n");
    // s.write(deep,"}\n");
    //}
    
    for(int i=0 ; i<getNumberOfCase() ; i++) {
	 s.write(deep+i,"/* case: " + getCaseTerm(i) + " */\n");
	 getCaseTerm(i).rgenRightside(s,deep+i);
	 s.write(deep+i,"if(" + getCaseTerm(i).genCore() + " == con_1) {\n");
	 getCaseRhs(i).genCode(s,deep+i+1,rule,number,sterm,isFirst,isLast);
	 s.write(deep+i,"} else {\n");
    }

    // See Condition.java
    int tab=getNumberOfCase();
    if(Flags.optimiseChoicePoint) {
      if(isFirst || !isLast) {
	if( !rule.getTopLevelDetType(sterm).isSemiDet() ||
	    !rule.allDetEvaluation() ) {
	  /* A choicePoint was generated */
	  Tools.genFail(s,deep+tab);
	} else {
	  s.write(deep+tab,"goto myend" + number + ";\n");
	}
      } else {
	if(sterm!=null) {
	  Tools.genFail(s,deep+tab);
	} else {
	  s.write(deep+tab,"goto myend" + number + ";\n");
	}
      }
    } else {
      Tools.genFail(s,deep+tab);
    }
    
    for(int i=getNumberOfCase()-1 ; i>=0 ; i--) {
      s.write(deep+i,"}\n");
    }
    //s.write(deep,"endCase" + number + ":\n");
  }


  public int getMaxVariableNumber(int max) {
    int r;
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      r = locEval.getMaxVariableNumber(max);
      max=(r>max)?r:max;
    }

    for(int i=0 ; i<getNumberOfCase() ; i++) {
      r=getCaseTerm(i).getMaxVariableNumber(max);
      max=(r>max)?r:max;
      r=getCaseRhs(i).getMaxVariableNumber(max);
      max=(r>max)?r:max;
    }

    return max;
  }

  public int getMaxUsedVariable(int max) {
    int r;
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      r = locEval.getMaxUsedVariable(max);
      max=(r>max)?r:max;
    }

    for(int i=0 ; i<getNumberOfCase() ; i++) {
      r=getCaseTerm(i).getMaxUsedVariable(max);
      max=(r>max)?r:max;
      r=getCaseRhs(i).getMaxUsedVariable(max);
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

    for(int i=0 ; i<getNumberOfCase() ; i++) {
      getCaseTerm(i).rightsideVariableAffectation();
      getCaseRhs(i).rightsideVariableAffectation();
    }
  }

  public void rightsideVariableLiberation() {
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      locEval.variableLiberation();
    }
    for(int i=0 ; i<getNumberOfCase() ; i++) {
      getCaseTerm(i).rightsideVariableLiberation();
      getCaseRhs(i).rightsideVariableLiberation();
    }
  }

  public void rightsideVariableClear(boolean full) {
    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval = getBranch().get(i);
      locEval.variableClear(full);
    }
    for(int i=0 ; i<getNumberOfCase() ; i++) {
      getCaseRhs(i).rightsideVariableLiberation();
    }
  }

  public void searchVariableShareFullNumbering(Term leftside) {

    for(int i=0 ; i<getNumberOfCase() ; i++) { 
      getCaseTerm(i).searchVariableShareFullNumbering(leftside);
      getCaseRhs(i).searchVariableShareFullNumbering(leftside);
    }


    for(int i=0 ; i<getBranch().size() ; i++) {
      LocalEvaluation locEval;
      locEval = getBranch().get(i);
      // if/where wrt. left
      locEval.searchVariableShareFullNumbering(leftside);

      // right wrt. where's leftside
      for(int j=0 ; j<getNumberOfCase() ; j++) { 
	getCaseTerm(j).searchVariableShareFullNumbering(locEval.getLeftside());
	getCaseRhs(j).searchVariableShareFullNumbering(locEval.getLeftside());
      }

      for(int j=0 ; j<i ; j++) {
	Term lhs = getBranch().get(j).getLeftside();
	// if/where wrt. previous where's leftside
	locEval.searchVariableShare(lhs);
      }
    }




  }

  public void flatten() {
    for(int i=0 ; i<getNumberOfCase() ; i++) {
      getCaseTerm(i).flatten();
      getCaseRhs(i).flatten();
    }
  }

  public void orderedNormalForm() {
    for(int i=0 ; i<getNumberOfCase() ; i++) {
      getCaseTerm(i).orderedNormalForm();
      getCaseRhs(i).orderedNormalForm();
    }
  }

  public String toString() {
    String s=new String(); 
    for(int i=0 ; i<getBranch().size() ; i++) {
      s+= "\n\t" + getBranch().get(i);
    }

    for(int i=0 ; i<getNumberOfCase() ; i++) {
      s+= "\ncase " + getCaseTerm(i) + ":";
      s+= "\n\t" + getCaseRhs(i);
    }
    return s;
  }

}
