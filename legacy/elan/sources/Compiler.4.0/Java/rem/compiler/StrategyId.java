package rem.compiler;

import java.util.*;

public class StrategyId extends StrategyTerm {

  public StrategyId() {
    super();
  }

  protected void setDetType() {
    detType = DetType.detType;
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.detType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    super.compile(s,deep,matchSubtermCode);
    Tools.indent(s,deep); s.write("res=v0;\n");
  }
  

  public String toString() {
    return "id";
  }

  public String getName() {
    return super.getName() + "_id";
  }

}
