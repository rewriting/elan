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

        System.out.println(vector);
        
        
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
      System.out.println("rules to compile (size=" + rules.size() + "):\n" + rules);
    }
    if (rules.size() > 0) {
      for(int i=0 ; i<rules.size() ; i++) {
	RewriteRule rule = (RewriteRule)rules.elementAt(i);
	ACPattern = ACPattern || rule.hasACPattern();
	allRuleDetEvaluation = allRuleDetEvaluation && rule.allDetEvaluation();
      }
	////////// [NGUYEN: Apr  5 01] one rule has ACpattern /////////////////
      if(ACPattern) {
	setAC();
      }

      RewriteRule.compileNamed(s,deep+1,this,rules,
			       getExitLabel(),matchSubtermCode);
      
    }
    
    Tools.indent(s,deep); s.write("}\n");

    // Si on arrive a cet endroit c'est que le filtrage a echoue'
    /*  [Huy: Oct 26 00] if no rule can be applied, return the original term
	as the result --> no fail to generate */
    if (this.isNorm()) 
	{ 
	  s.write("match_fail:\n"); 
	  Tools.indent(s,deep);
	  s.write("res = v0;\n");
	  s.write("stratLab" + getExitLabel() + ":;\n");
	} else {
	    Tools.genFail(s,deep);
	    Tools.indent(s,deep); s.write("stratLab" + getExitLabel() + ":;\n");
	    Tools.indent(s,deep); s.write("v0=res;\n");
	}
    if(Flags.debug) {
      s.write(deep+1,"if(traceLevel>=3) {\n");
      s.write(deep+2,"doindent(indentlevel);\n");
      s.write(deep+2,"printf(\"SUCCES\\n\");\n");
      s.write(deep+1,"}\n");
    }

    
    // if(ACPattern && !allRuleDetEvaluation) {
    //if(isSemiDet() && ACPattern && !allRuleDetEvaluation) {
    //Tools.indent(s,deep); s.write("CUTCLOSE(); /* AC matching */\n");
    //}
  }
  
}
