/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
package rem.compiler;

import java.util.*;

public class Choice implements LocalEvaluation {
  /**
    * list of BranchEvaluation
    */
  private List branches;
  
  public Choice(List branches) {
    this.branches=branches;
  }

  private List getBranches() {
    return branches;
  }

  private int size() {
    return getBranches().size();
  }

  private BranchEvaluation get(int i) {
    return (BranchEvaluation)getBranches().get(i);
  }


  // LocEval/Term
  public void searchVariableShare(Term t) {
    for(int i=0 ; i<size() ; i++) {
      get(i).searchVariableShare(t);
    }
  }
  // Term/locEval (lhs of local evaluation)
  public void reverseSearchVariableShare(Term t) {
    for(int i=0 ; i<size() ; i++) {
      get(i).reverseSearchVariableShare(t);
    }
  }

  // rhs of locEval wrt lhs of this
  public void reverseSearchVariableShare(LocalEvaluation locEval) {
    for(int i=0 ; i<size() ; i++) {
      get(i).reverseSearchVariableShare(locEval);
    }
  }

  public void selfSearchVariableShare() {
    for(int i=0 ; i<size() ; i++) {
      get(i).selfSearchVariableShare();
      for(int j=i+1 ; j<size() ; j++) {
	get(j).searchVariableShare(get(i));
      }
    }
  }

  // rhs of this wrt. lhs of locEval
  public void searchVariableShare(LocalEvaluation locEval) {
    for(int i=0 ; i<size() ; i++) {
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

  public Term getLeftside() {
    return null;
  }

  public DetType getDetType() {
    // TODO
    /*
    DetType type=DetType.detType;
    for(int i=0 ; i<size() ; i++) {
      type = type.and(get(i).getDetType());
    }
    return type;
    */
    return DetType.nonDetType; // pour fonctionner avec l'optimisation
  }
  
  public void genCode(OutputCode s,
		      int deep,
		      RewriteRule rule,
		      int number,
		      StrategyChooseRule sterm,
		      boolean isFirst, boolean isLast) {
        // rule is applied under nonamed/one/dc/dk ?
    boolean isDcApply=false;
    if(!(sterm==null || sterm.isOne()) && sterm.isDc()) {
      isDcApply=true;
    }

    for(int i=0 ; i<size() ; i++) {
      s.write(deep+i,"if(!setChoicePoint()) {\n");
      if(isDcApply) {
	s.write(deep+i+1,"*((int*)getStablePointer(wasr_index))=0;\n");
      }
      s.write(deep+i+1,"/* try branch " + i + "\n");
      s.write(0,get(i).toString(deep+i+3));
      s.write(deep+i+1," */\n");
        /*
         * Il faut remettre a jour certains indicateurs de la branche
         * en particulier les flags
         *  - instantiated
         */
      boolean full=false;
      get(i).variableClear(full);
            
      boolean isLastBranch = (i+1==size()) ;
      boolean isFirstBranch = (i==0) ;
      //boolean isLastBranch = isLast;
      //boolean isFirstBranch = isFirst;
      get(i).genCode(s,deep+i+1,rule,number,sterm,isFirstBranch,isLastBranch);
      s.write(deep+i,"} else { \n");
      if(isDcApply) {
	s.write(deep+i+1,"/* branch failed */\n");
	s.write(deep+i+1,"if(*((int*)getStablePointer(wasr_index))!=0) {\n");
	s.write(deep+i+2,"/* If the rule succeed one then re-generate a fail */\n");
	Tools.genFail(s,deep+i+2);
	s.write(deep+i+1,"}\n");
      }
    }
     
    // See Condition.java
    // TODO : Il y a un probleme ici
    int tab=size();
    if(Flags.optimiseChoicePoint) {
      if(isFirst || !isLast) {
	if( rule.getTopLevelDetType(sterm).isSemiDet() &&
	    rule.allDetEvaluation() ) {
	  s.write(deep+tab,"goto myend" + number + ";\n");
	} else {
	  /* A choicePoint was generated */
	  Tools.genFail(s,deep+tab);
	} 
      } else {
	if(sterm!=null) {
	  Tools.genFail(s,deep+tab);
	} else {
          s.write(deep+1,"goto myend" + number + ";\n");
            /*
                // [pem: May 25 99]
          if( rule.getTopLevelDetType(sterm).isSemiDet() &&
              rule.allDetEvaluation() ) {
            s.write(deep+1,"goto myend" + number + ";\n");
          } else {
            // A choicePoint was generated 
            Tools.genFail(s,deep+1);
          }
        */
	}
      }
    } else {
      Tools.genFail(s,deep+tab);
    }
    
    for(int i=size()-1 ; i>=0 ; i--) {
      s.write(deep+i,"}\n");
    }
  }

  public boolean dependFrom(Term t) {
    boolean res=false;
    for(int i=0 ; i<size() && res==false ; i++) {
      res = res || get(i).dependFrom(t);
    }
    return res;
  }

  public String toString() {
    String s="choose ";
    for(int i=0 ; i<size() ; i++) {
      s+= get(i);
    }
    return s;
  }

  public String toString(int deep) {
    String s= Tools.indent(deep) + "choose\n";
    for(int i=0 ; i<size() ; i++) {
      s+= Tools.indent(deep) + "try\n";
      s+= get(i).toString(deep+1);
    }
    s+= Tools.indent(deep) + "end";
    return s;
  }

}

