package rem.compiler;

import java.util.*;

public class StrategyCons extends StrategyTerm {

  public StrategyCons(Vector sub) {
    super(sub);
  }

  protected void setDetType() {
    detTypeState=start;
    detType = DetType.detType;
    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm term = (StrategyTerm) subterms.elementAt(i);
      detType = detType.and(term.getDetType());
    }
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.nonDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    super.compile(s,deep,matchSubtermCode);
    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i);
      sterm.compile(s,deep,matchSubtermCode);
    }
  }
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "cons" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_cons";
  }
  
}
