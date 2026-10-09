package rem.compiler;

import java.util.*;

public class Condition implements LocalEvaluation {
  private Term condition;
  
  public Condition(Term cond) {
    condition = cond;
  }

  public Term getTerm() {
    return condition;
  }
  
  public static Condition buildEquality(Term t1, Term t2) {
    int idFsym;
    if(t1.getSymbol().getSort().isSortInteger()) {
      idFsym = 8; // Code of equality for builtinInt
    } else {
      idFsym = 18; // Code of equality for terms
    }
    Symbol eqSymbol=Symbol.get(idFsym);
    if(eqSymbol == null) {
      System.out.println("You need to import the equality module on sort: "
                         + t1.getSymbol().getSort().getName());
      throw new InternalError("symbol sym" + idFsym + " not found");
    }
    Vector subterms = new Vector(2);
    subterms.addElement(t1.deepCopy());
    subterms.addElement(t2.deepCopy());
    Term eqTerm = new Term(eqSymbol , subterms);
    return new Condition( eqTerm );
  }

  public void searchVariableShare(Term t) {
    condition.searchVariableShare(t);
  }

  public void reverseSearchVariableShare(Term t) {
    /* do nothing */
  }

  public void reverseSearchVariableShare(LocalEvaluation locEval) {
    /* do nothing */
  }

  public void selfSearchVariableShare() {
    /* do nothing */
  }

  public void searchVariableShare(LocalEvaluation locEval) {
    // Test
  }

  public void searchVariableShareFullNumbering(Term lhs) {
    condition.searchVariableShareFullNumbering(lhs);
  }

  public void setFullNumbering() {
    condition.setFullNumbering();
  }

  public int variableAffectation(int index) {
    condition.rightsideVariableAffectation();
    return index;
  }

  public void variableLiberation() {
    condition.rightsideVariableLiberation();
  }

  public void variableClear(boolean full) {
    /*
     * do nothing
     */
    condition.leftsideVariableClear(false);
  }

  public void markUsedVariable(BitSet b) {
    condition.markUsedVariable(b);
  }

  public int getMaxVariableNumber(int max) {
    return condition.getMaxVariableNumber(max);
  }

  public int getMaxUsedVariable(int max) {
    return condition.getMaxUsedVariable(max);
  }

  public int getMaxSubstIndex(int max) {
    return max;
  }

  public Term getLeftside() {
    return null;
  }

  
  public DetType getDetType() {
    //DetType type=DetType.semiDetType;
    DetType type=DetType.detType;
    if(condition.dependFromVariableUnderAC()) {
      type=type.and(DetType.multiDetType);
    }
    return type;
  }


  public void genCode(OutputCode s,
		      int deep,
		      RewriteRule rule,
		      int number,
		      StrategyChooseRule sterm,
		      boolean isFirst, boolean isLast) {
    s.write(deep,"/* " + this + " */\n");

    if(Flags.debug) { 
      s.write(deep,"if(traceLevel>=3) {\n");
      if(!condition.isBuiltinSort()) {
        s.write(deep+1,"doindent(indentlevel);\n");
        s.write(deep+1,"printf(\"IF\\n\");\n");
      }
      s.write(deep,"}\n");
    }
    condition.genTermConstruction(s,deep);
    
    s.write(deep,"if( " + condition.genCore() + " != con_1 ) {\n");
    if(Flags.optimiseChoicePoint == false) {
      Tools.genFail(s,deep+1);
    } else {
      if(isFirst || !isLast) {
          // si la regle est la premiere ou n'est pas la derniere
	if( rule.getTopLevelDetType(sterm).isSemiDet() &&
	    rule.allDetEvaluation() ) {
	  s.write(deep+1,"goto myend" + number + ";\n");
	} else {
	  /* A choicePoint was generated */
	  Tools.genFail(s,deep+1);
	}
      } else {
	if(sterm!=null) {
	  Tools.genFail(s,deep+1);
	} else {
            // [pem: May 25 99]
          if( rule.getTopLevelDetType(sterm).isSemiDet() &&
              rule.allDetEvaluation() ) {
            s.write(deep+1,"goto myend" + number + ";\n");
          } else {
              // A choicePoint was generated 
            Tools.genFail(s,deep+1);
          }
        }
      }
    }

    s.write(deep,"}\n");
  }

  public boolean dependFrom(Term t) {
    return condition.dependFrom(t);
  }

  public String toString() {
    String s;
    s = "if " + condition;
    return s;
  }

  public String toString(int deep) {
    return Tools.indent(deep) + this;
  }

}

