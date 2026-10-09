package rem.compiler;

import java.util.*;

public class LocalAffectation implements LocalEvaluation {
  private Term leftside;   // variable ou pattern
  private Term rightside;  // term a evaluer
  private Lexem stratName; // nom de la strategie

  public LocalAffectation(Term lhs, Lexem stratName, Term rhs) {
    leftside = lhs;
    rightside = rhs;
    this.stratName = stratName;
  }

  public Term getLeftside() {
    return leftside;
  }

  public void setFullNumbering() {
    leftside.setFullNumbering();
    rightside.setFullNumbering();
  }

  public int variableAffectation(int index) {
    /*
     * Probleme de numerotation du membre gauche :
     * il ne faut pas liberer les variables utilisees
     */

    leftside.searchVariableShare(leftside);
      //System.out.println("leftside1 = " + leftside);
    index=leftside.leftsideACVariableAffectation(index);
      //System.out.println("leftside2 = " + leftside);
    leftside.rightsideVariableAffectationWithNoReuse();
      //System.out.println("leftside3 = " + leftside);
    rightside.rightsideVariableAffectation();
      //System.out.println("rightside1 = " + rightside);

    return index;
  }

  public void variableLiberation() {
    leftside.leftsideACVariableLiberation();
    leftside.rightsideVariableLiberationWithNoReuse();
    rightside.rightsideVariableLiberation();
  }

  public void variableClear(boolean full) {
    leftside.leftsideVariableClear(full);
    rightside.leftsideVariableClear(false);
  }

  // rhs of where wrt. lhs of rule
  public void searchVariableShare(Term t) {
    rightside.searchVariableShare(t);
  }

  // rhs of rule wrt. lhs of where
  public void reverseSearchVariableShare(Term t) {
    //System.out.println("\t" + t + " <--> " + leftside);
    t.searchVariableShare(leftside);
  }

  // rhs of locEval wrt lhs of this
  public void reverseSearchVariableShare(LocalEvaluation locEval) {
    locEval.searchVariableShare(leftside);
  }

  public void selfSearchVariableShare() {
    /* do nothing */
  }

  public void searchVariableShare(LocalEvaluation locEval) {
    // Test
    locEval.reverseSearchVariableShare(leftside);
  }

  public void searchVariableShareFullNumbering(Term lhs) {
    rightside.searchVariableShareFullNumbering(lhs);
    leftside.setFullNumbering();
  }

  public void markUsedVariable(BitSet b) {
    leftside.markUsedVariable(b);
    rightside.markUsedVariable(b);
  }

  public int getMaxVariableNumber(int max) {
    int maxLeftside  = leftside.getMaxVariableNumber(max);
    int maxRightside = rightside.getMaxVariableNumber(max);
    return (maxLeftside>maxRightside)?maxLeftside:maxRightside;
  }

  public int getMaxUsedVariable(int max) {
    int maxLeftside  = leftside.getMaxUsedVariable(max);
    int maxRightside = rightside.getMaxUsedVariable(max);
    return (maxLeftside>maxRightside)?maxLeftside:maxRightside;
  }

  public int getMaxSubstIndex(int max) {
    return leftside.getMaxSubstIndex(max);
  }

  public DetType getDetType() {
    DetType type=DetType.detType;
    if(stratName!=null) {
      // Strategie du `where'
      Strategy strat = Strategy.getStrategy(stratName);
      type=strat.getDetType();
    }
    if(!leftside.isVariable()) {
      // Where with pattern
      // l'echec devient possible
      if(leftside.containsAC()) {
	type=type.and(DetType.nonDetType);
      } else {
        type=type.and(DetType.semiDetType);
      }
    }

    if(!type.isExactlyDet() && rightside.dependFromVariableUnderAC()) {
      type=type.and(DetType.multiDetType);
    }

    return type;
  }

  public void genCode(OutputCode s,int deep, RewriteRule rule, 
		      int number,
		      StrategyChooseRule sterm,
		      boolean isFirst, boolean isLast) {
    s.write(deep,"/* " + this + " */\n");

    if(Flags.debug) { 
      s.write(deep,"if(traceLevel>=3) {\n");
      if(!leftside.isBuiltinSort()) {
        s.write(deep+1,"doindent(indentlevel);\n");
        s.write(deep+1,"printf(\"WHERE " + leftside + " = \\n\");\n");
      }
      s.write(deep,"}\n");
    }

    if(sterm!=null && sterm.isNorm()) {
      if(sterm.isNormOut()) {
        s.write(deep,"tmp_step = rewrite_real_step;\n");  
      }
      s.write(deep,"if(coqMode) {\n");
      s.write(deep+1,"MAXPOS = 0;\n");
      s.write(deep+1,"if((head_cond_tr[index] = MALLOC(sizeof(struct TR_COQ)))!= NULL) {\n");
      s.write(deep+2,"head_tr = tail_tr =  NULL;\n");
      s.write(deep+1,"}\n");
      s.write(deep,"}\n");
    }

    if(stratName==null) {
      rightside.genTermConstruction(s,deep);
      
      if(!leftside.isVariable()) {
	// Where with pattern
          /*
        if(Flags.debug) {
          if(!leftside.containsOnlyFreshVariable()) {
            System.out.println("Rule: " + rule);
          }
        }
          */
        s.write(deep,"tmp = "
                  // [pem: Sep  6 00]
                + "sv[" + leftside.getVarNumber() + "] = "
                  // + leftside.genCore() + " = "
                + rightside.genCore() + ";\n");
        leftside.genOneToOneMatching(s,deep);
      } else {
        s.write(deep,"tmp = "
                + leftside.genCore() + " = "
                + rightside.genCore() + ";\n");
      }
    } else {
      Strategy strategy = Strategy.getStrategy(stratName);

      if(strategy.isEval() && Flags.strat > 1 &&
	  StrategyEval.isCompilable(rightside)) {
	/*
	 * Truc de Peter !
	 * Est-ce que cela fonctionne avec des symboles AC ?
	 */
	Term str = rightside.getSubterm(0); 
	Term src = rightside.getSubterm(1);
	src.genTermConstruction(s,deep);  // generate term
	StrategyEval.genEval(s,deep, 
			     0, //StrategyTerm.strategyLabel, 
			     leftside.genCore(), str, 
			     src.genCore());
      } else {
	rightside.genTermConstruction(s,deep);

	s.write(deep,leftside.genCore() + " = str" + strategy.getName() +
		"( " + rightside.genCore() + " );\n");


        
	if(!leftside.isVariable()) {
	  // Where with pattern
            /*
          if(Flags.debug) {
            if(!leftside.containsOnlyFreshVariable()) {
              System.out.println("Rule: " + rule);
            }
            }
            */
          
          leftside.genOneToOneMatching(s,deep);
	}
      }
    }
    if(sterm!=null && sterm.isNorm()) {
      s.write(deep,"if (coqMode){\n");
      s.write(deep+1,"head_cond_tr[index] = head_tr;\n");
      s.write(deep+1,"index = index +1;\n");
      s.write(deep+1,"MAXPOS = pos;\n");
      s.write(deep+1,"head_tr = head_tmp;\n");
      s.write(deep+1,"tail_tr = tail_tmp;\n");
      s.write(deep+1,"for (i=0;i<MAXPOS; i++) position[i]=position_tmp[i];\n");
      s.write(deep,"}\n");
      if(sterm.isNormOut()) {
        s.write(deep,"rewrite_real_step = tmp_step;\n");
      }
    }
  }

  public boolean dependFrom(Term t) {
    return leftside.dependFrom(t) || rightside.dependFrom(t);
  }

  public List linearise(RewriteRule rule) {
      // [pem: Jun 26 00] linearisation des pattern ne contenant pas d'AC
    if(!leftside.containsAC()) {
      return leftside.linearise(rule);
    } else {
      return new ArrayList();
    }
  }
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "where " + leftside + " := " );
    if(stratName!=null) {
      s.append("(" + stratName + ") ");
    }
    s.append(rightside);
    return s.toString();
  }

  public String toString(int deep) {
    return Tools.indent(deep) + this;
  }
 
}
