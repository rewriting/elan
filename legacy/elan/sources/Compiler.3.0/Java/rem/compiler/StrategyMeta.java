package rem.compiler;

import java.util.*;

public class StrategyMeta extends StrategyTerm {

  public StrategyMeta() {
    super();
  }

  protected void setDetType() {
    // TODO
    detType = DetType.nonDetType;
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    // TODO
    return DetType.nonDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    super.compile(s,deep,matchSubtermCode);
    // TODO
    //Tools.indent(s,deep); s.write("res=v0;\n");
    Tools.indent(s,deep); s.write("res=term_metaApply(v0);\n");
  }
  

  public String toString() {
    return "Meta";
  }

  public String getName() {
    return super.getName() + "_Meta";
  }

}
