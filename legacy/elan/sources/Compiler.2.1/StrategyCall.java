import java.util.*;

public class StrategyCall extends StrategyTerm {
  private Lexem name;

  public StrategyCall(Lexem name) {
    super();
    this.name = name;
  }

  protected void setDetType() {
    detTypeState=start;
    detType = Strategy.getStrategy(name).getDetType();
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.nonDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    Tools.indent(s,deep); s.write("res = str_" + name.getCode() + "(v0);\n");
    Tools.indent(s,deep); s.write("v0 = res;\n");
  }
  
 
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "call(" + name + ")" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "call";
  }
  
}
