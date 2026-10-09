import java.util.*;

public class BranchEvaluation {
  /**
    * list of localEvaluation (if/where/try)
    */
  private Vector wheres;  

  /**
   * to know if local evaluations are deterministic 
   */
  private DetType detType;
  static private int clear=0;
  static private int start=1;
  static private int done=2;
  private int detTypeState=clear;
  
  public BranchEvaluation(Vector localEvaluations) {
    wheres=localEvaluations;
  }
  
  private Vector getLocalEvaluation() {
    return wheres;
  }

  public int size() {
    return getLocalEvaluation().size();
  }

  public LocalEvaluation get(int i) {
    return (LocalEvaluation)getLocalEvaluation().elementAt(i);
  }

  public void insertLocalEvaluation(LocalEvaluation locEval) {
    getLocalEvaluation().insertElementAt(locEval,0);
  }
  
  /*
  public void searchVariableShare(Term leftside, Term rightside) {
    for(int i=0 ; i<size() ; i++) {
      LocalEvaluation locEval = get(i);
      // if/where/choice wrt. left
      locEval.searchVariableShareFullNumbering(leftside);
      
      // right wrt. where's leftside
      rightside.searchVariableShare(locEval.getLeftside());
      // TODO
      // right wrt. choice

      System.out.println("BranchEvaluation.searchVariableShare(): not finished");

      for(int j=0 ; j<i ; j++) {
	Term lhs = get(j).getLeftside();
	// if/where/choice wrt. previous where's leftside
	locEval.searchVariableShare(lhs,rightside);
      }
    }
  }
  */

  // Branch/Term (lhs of rule)
  public void searchVariableShare(Term t) {
    for(int i=0 ; i<size() ; i++) {
      get(i).searchVariableShare(t);
    }
  }

  // Term/Branch (rhs of rule/lhs of local evaluation)
  public void reverseSearchVariableShare(Term t) {
    for(int i=0 ; i<size() ; i++) {
      get(i).reverseSearchVariableShare(t);
    }
  }

  // Old switch
  // RightHandSide/Branch (rhs of rule/lhs of local evaluation)
  public void reverseSearchVariableShare(RightHandSide t) {
    if(t.getRightside()!=null) {
      reverseSearchVariableShare(t.getRightside());
    } else {
      System.out.println("RightHandSide/Branch not implemented");
    }
  }

  // locEval(j)/locEval(i)
  public void selfSearchVariableShare() {
    for(int i=0 ; i<size() ; i++) {
      get(i).selfSearchVariableShare();
      for(int j=i+1 ; j<size() ; j++) {
	get(i).reverseSearchVariableShare(get(j));
      }
    }
  }

  // locEval/Branch
  public void reverseSearchVariableShare(LocalEvaluation locEval) {
    for(int i=0 ; i<size() ; i++) {
      get(i).reverseSearchVariableShare(locEval);
    }
  }
  
  //  // rhs of branch wrt. lhs of this
  //  C'est faux !!!

  // Branch/Branch
  // branch est la reference :
  // rhs of this wrt. lhs of branch
  public void searchVariableShare(BranchEvaluation branch) {
    //System.out.println("Branch/Branch");
    for(int i=0 ; i<branch.size() ; i++) {
      //System.out.println(this + "\nwtr.\n" + branch.get(i));
      searchVariableShare(branch.get(i));
    }
  }

  // rhs of this wrt. lhs of locEval
  public void searchVariableShare(LocalEvaluation locEval) {
    // Test
    for(int i=0 ; i<size() ; i++) {
      //System.out.println("\nTEST\n" + get(i) + " wtr. " + locEval);
      get(i).searchVariableShare(locEval);
    }
  }

  public void searchVariableShareFullNumbering(Term lhs) {
    for(int i=0 ; i<size() ; i++) {
      get(i).searchVariableShareFullNumbering(lhs);
    }
  }
  
  public void setFullNumbering() {
    for(int i=0 ; i<size() ; i++) {
      get(i).setFullNumbering();
    }
  }

  public int variableAffectation(int index) {
    for(int i=0 ; i<size() ; i++) {
      index=get(i).variableAffectation(index);
    }
    return index;
  }

  public void variableLiberation() {
    for(int i=0 ; i<size() ; i++) {
      get(i).variableLiberation();
    }
  }

  public void variableClear(boolean full) {
    for(int i=0 ; i<size() ; i++) {
      get(i).variableClear(full);
    }
  }

  public void markUsedVariable(BitSet b) {
    for(int i=0 ; i<size() ; i++) {
      get(i).markUsedVariable(b);
    }
  }

  public int getMaxVariableNumber(int max) {
    for(int i=0 ; i<size() ; i++) {
      int r = get(i).getMaxVariableNumber(max);
      max=(r>max)?r:max;
    }
    return max;
  }

  public int getMaxUsedVariable(int max) {
    for(int i=0 ; i<size() ; i++) {
      int r = get(i).getMaxUsedVariable(max);
      max=(r>max)?r:max;
    }
    return max;
  }

  public int getMaxSubstIndex(int max) {
    for(int i=0 ; i<size() ; i++) {
      int r = get(i).getMaxSubstIndex(max);
      max=(r>max)?r:max;
    }
    return max;
  }

  public DetType getDetType() {
    /*
     * si on ne met pas la 2eme condition, il y a un comportement
     * incoherent entre java et java -nojit
     */
    if(detTypeState==clear || detTypeState==start) {
      setDetType();
    }
    if(detTypeState==done) {
      return detType;
    } else {
      System.out.println("detTypeState = " + detTypeState);
      System.out.println("this = " + this);
      throw new InternalError("unknown detTypeState!=done");
    }
  }

  public void setDetType() {
    detTypeState=start;
    detType = DetType.detType;
    for(int i=0 ; i<size() ; i++) {
      detType = detType.and(get(i).getDetType());
    }
    detTypeState=done;
  } 

  public void genCode(OutputCode s,
		      int deep,
		      RewriteRule rule,
		      int number,
		      StrategyChooseRule sterm,
		      boolean isFirst, boolean isLast) {
    for(int i=0 ; i<size() ; i++) {
      get(i).genCode(s,deep,rule,number,sterm,isFirst,isLast);
    }
  }

  public String toString() {
    String s="";
    for(int i=0 ; i<size() ; i++) {
      s+= "\n\t" + get(i);
    }
    return s;
  }

  public boolean dependFrom(Term t) {
    boolean res=false;
    for(int i=0 ; i<size() && res==false ; i++) {
      res = res || get(i).dependFrom(t);
    }
    return res;
  }

  public String toString(int deep) {
    String s="";
    for(int i=0 ; i<size() ; i++) {
      s+= get(i).toString(deep) + "\n";
    }
    return s;
  }

}

