import java.util.*;

abstract class StrategyChooseRule extends StrategyTerm {
  protected Vector listOfRules = new Vector();

  public StrategyChooseRule(Vector sub) {
    super(sub);

    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm sTerm = (StrategyTerm) subterms.elementAt(i);
      if(sTerm.isStrategyRuleName()) {
	Vector vector=sTerm.getNamedRules();
	for(int j=0 ; j<vector.size() ; j++) {
	  RewriteRule rule = (RewriteRule)vector.elementAt(j);
	  listOfRules.addElement(rule);
	}
      }
    }

  }

  public boolean isOne() {
    return this instanceof StrategyOneRule;
  }
  public boolean isDc() {
    return this instanceof StrategyDcRule;
  }
  public boolean isDk() {
    return this instanceof StrategyDkRule;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {

// CHENNOUFI
//    s.write(deep,"myDoindent(indentlevel);\n");
//    s.write(deep,"printf(\"the strategy is : " + this + "\\n\");\n");
    
    s.write(deep,"{\n");
    super.compile(s,deep+1,matchSubtermCode);
    compileEpilog(s,deep, matchSubtermCode, listOfRules);
  }

  /**
   * construction of the vector of rules
   * for compatibility only
   */
  public void compileProlog(OutputCode s,int deep, 
			    OutputCode matchSubtermCode, Vector rules) {
    Tools.indent(s,deep); s.write("{\n");
    super.compile(s,deep+1,matchSubtermCode);

    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm sTerm = (StrategyTerm) subterms.elementAt(i);
      if(sTerm.isStrategyRuleName()) {
	Vector vector=sTerm.getNamedRules();
	if(vector.size()>1 && this instanceof StrategyDcRule) {
	  System.out.println("Warning: several rules with the same label occur under a dc constructor\n");
	}
	for(int j=0 ; j<vector.size() ; j++) {
	  RewriteRule rule = (RewriteRule)vector.elementAt(j);
	  rules.addElement(rule);
	}
      }
    }
  }

  /**
   * generation from the vector of rules
   */
  public void compileEpilog(OutputCode s, int deep,
			    OutputCode matchSubtermCode, Vector rules) {
    /*
     * is there an AC pattern ?
     */
    boolean ACPattern = false;
    boolean allRuleDetEvaluation = true;

    if (Flags.verbose) {
      System.out.println("rules to compile :\n" + rules);
    }
    if (rules.size() > 0) {
      RewriteRule.compileNamed(s,deep+1,this,rules,
			       getExitLabel(),matchSubtermCode);
      
      for(int i=0 ; i<rules.size() ; i++) {
	RewriteRule rule = (RewriteRule)rules.elementAt(i);
	ACPattern = ACPattern || rule.hasACPattern();
	allRuleDetEvaluation = allRuleDetEvaluation && rule.allDetEvaluation();
      }
    }
    
    Tools.indent(s,deep); s.write("}\n");

    // Si on arrive a cet endroit c'est que le filtrage a echoue'
    Tools.genFail(s,deep);

    Tools.indent(s,deep); s.write("stratLab" + getExitLabel() + ":;\n");
    Tools.indent(s,deep); s.write("v0=res;\n");

    // if(ACPattern && !allRuleDetEvaluation) {
    //if(isSemiDet() && ACPattern && !allRuleDetEvaluation) {
    //Tools.indent(s,deep); s.write("CUTCLOSE(); /* AC matching */\n");
    //}
  }
  
}
