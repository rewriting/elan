import java.util.*;

public class StrategyIterate extends StrategyTerm {

  public StrategyIterate(Vector sub) {
    super(sub);
  }

  protected void setDetType() {
    detTypeState=start;
    detType = ((StrategyTerm) subterms.firstElement()).getDetType();
    if(detType.isDet() || detType.isMultiDet()) {
	 System.out.println("*** Warning: the strategy " + this +
					"cannot be terminating");
    }
    detType = DetType.multiDetType;
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.multiDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {

    super.compile(s,deep,matchSubtermCode);

    Tools.indent(s,deep); s.write("while(1) {\n");
    Tools.indent(s,deep+1); s.write("if(!setChoicePoint()) {\n");
    Tools.indent(s,deep+2); s.write("break;\n");
    Tools.indent(s,deep+1); s.write("}\n");

    s.write(deep+1,"/* Apply the strategy */\n");
    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i);
      sterm.compile(s,deep+1,matchSubtermCode);
    }
    Tools.indent(s,deep); s.write("}\n");
  }
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "iterate" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_iterate";
  }
  
}
