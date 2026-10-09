import java.util.*;

public class StrategyOneRule extends StrategyChooseRule {

  public StrategyOneRule(Vector sub) {
    super(sub);
  }

  protected void setDetType() {
    detTypeState=start;
    /* matching phase can fail */
    detType = DetType.semiDetType;
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.semiDetType;
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "one" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_one";
  }
  
}
