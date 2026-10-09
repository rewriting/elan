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
