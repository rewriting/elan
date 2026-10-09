package rem.compiler;

import java.util.*;

public class StrategyDcStrat extends StrategyTerm {

  public StrategyDcStrat(Vector sub) {
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
    Tools.indent(s,deep); s.write("{\n");
    super.compile(s,deep+1,matchSubtermCode);

    Tools.indent(s,deep+1); s.write("int *wasr=(int*) allocStable(sizeof(int));\n");
    Tools.indent(s,deep+1); s.write("*wasr=0;\n");

    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i);
      boolean isLast = (i+1 == subterms.size());
      boolean isFirst = (i==0);

      if(!isFirst) {
	/*
	 * Le test est place en debut de boucle pour propager le fail 
	 * de la derniere sous-strategie en cas d'echec
	 */
	Tools.indent(s,deep+1); s.write("/* On vient d'un fail */\n");
	Tools.indent(s,deep+1); s.write("if(*wasr!=0) {\n");
	Tools.indent(s,deep+2); s.write("/* Si on a un resultat on propage le fail */\n");
	Tools.genFail(s,deep+2);
	Tools.indent(s,deep+1); s.write("}\n");
	Tools.indent(s,deep+1); s.write("/* Sinon on essai la strategie suivante */\n");
      }
      if(!isLast) {
	Tools.indent(s,deep+1); s.write("if(!setChoicePoint()) {\n");
	Tools.indent(s,deep+2); s.write("/* Si la strategie suivante echoue, on passe a la suivante */\n");
	deep++;
      }

      sterm.compile(s,deep+1, matchSubtermCode);

      if(!isLast) {
	Tools.indent(s,deep+1); s.write("/* La strategie a donne un resultat */\n");
	Tools.indent(s,deep+1); s.write("if(*wasr==0) {\n");
	Tools.indent(s,deep+2); s.write("*wasr=1;\n");
	Tools.indent(s,deep+1); s.write("}\n");
      }

      Tools.indent(s,deep+1); s.write("goto stratLab" + getExitLabel() + ";\n");
      
      if(!isLast) {
	deep--;
	Tools.indent(s,deep+1); s.write("}\n");
      }
    }

    Tools.indent(s,deep); s.write("stratLab" + getExitLabel() + ":;\n");
    Tools.indent(s,deep); s.write("}\n");
  }
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "DC" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_DC";
  }
 

}
