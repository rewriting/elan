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

public class StrategyNormOutRule extends StrategyChooseRule {

  public StrategyNormOutRule(Vector sub) {
    super(sub);
  }

  public boolean isNorm() {
    return true;
  }
  
  public boolean isNormOut() {
    return true;
  }

  protected void setDetType() {
    detTypeState=start;
    /* matching phase can fail */
    detType = DetType.semiDetType;
    for(int i=0 ; i<listOfRules.size() ; i++) {
      RewriteRule rule = (RewriteRule) listOfRules.elementAt(i);
      DetType ruleType = rule.getDetTypeEvaluation();
      // cas AC
      if(rule.hasACPattern()) {
	ruleType=ruleType.and(DetType.multiDetType);
      }
      detType = detType.and(ruleType);
    }
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.nonDetType;
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "normout" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_normout";
  }
  
}
