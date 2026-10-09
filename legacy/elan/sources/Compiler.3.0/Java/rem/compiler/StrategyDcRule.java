package rem.compiler;

import java.util.*;

public class StrategyDcRule extends StrategyChooseRule {

  public StrategyDcRule(Vector sub) {
    super(sub);
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
    s.append( "dc" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_dc";
  }
  
}
