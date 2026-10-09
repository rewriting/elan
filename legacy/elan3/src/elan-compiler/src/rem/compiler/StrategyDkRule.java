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

public class StrategyDkRule extends StrategyChooseRule {

  public StrategyDkRule(Vector sub) {
    super(sub);
  }

  protected void setDetType() {
    detTypeState=start;
    RewriteRule firstRule = (RewriteRule) listOfRules.firstElement();
    detType = DetType.semiDetType.and(firstRule.getDetTypeEvaluation());
    // Cas AC
    if(firstRule.hasACPattern()) {
      detType=detType.and(DetType.multiDetType);
    }

    for(int i=1 ; i<listOfRules.size() ; i++) {
      RewriteRule rule = (RewriteRule) listOfRules.elementAt(i);
      DetType ruleType = DetType.semiDetType.and(rule.getDetTypeEvaluation());
      // cas AC
      if(rule.hasACPattern()) {
	detType=detType.and(DetType.multiDetType);
      }
      detType = detType.or(ruleType);
    }
    detTypeState=done;
  }
  protected DetType getDefaultDetType() {
    return DetType.nonDetType;
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "dk" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_dk";
  }
  
}
