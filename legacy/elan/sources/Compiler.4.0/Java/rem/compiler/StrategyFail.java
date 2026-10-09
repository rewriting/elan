package rem.compiler;

import java.util.*;

public class StrategyFail extends StrategyTerm {

  public StrategyFail() {
    super();
  }

  protected void setDetType() {
    detType = DetType.semiDetType; 
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.semiDetType;
  }
  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    super.compile(s,deep,matchSubtermCode);
    Tools.genFail(s,deep);
  }
  
  public String toString() {
    return "fail";
  }

  public String getName() {
    return super.getName() + "_fail";
  }

}
