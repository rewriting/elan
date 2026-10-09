import java.util.*;

public class StrategyOneStrat extends StrategyTerm {

  public StrategyOneStrat(Vector sub) {
    super(sub);
  }
 
  protected void setDetType() {
    detTypeState=start;
    detType = DetType.detType;
    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm term = (StrategyTerm) subterms.elementAt(i);
      detType = detType.and(term.getDetType());
    }
    if(detType.isMultiDet()) {
      detType=DetType.detType;
    } else if(detType.isNonDet()) {
      detType=DetType.semiDetType;
    }
    detTypeState=done;
  }
  
  protected DetType getDefaultDetType() {
    return DetType.semiDetType;
  }

  /*
   * Nouveau schema de compilation qui ajoute des CUTOPEN() et CUTCLOSE()
   * dans une strategie DC(S1,...,Sn), apres l'application d'une Si
   * on ne revient jamais en arriere
   * 2 versions possibles :
   *   - v1 ressemble a la compilation de dc(...)
   *   - v2 optimise le nombre de CUTOPEN() et CUTCLOSE()
   */

  public void compile(OutputCode s,int deep, 
		      OutputCode matchSubtermCode) {
    s.write(deep,"{\n");
    super.compile(s,deep+1,matchSubtermCode);
    s.write(deep+1,"CUTOPEN(); /* DC v2 */\n"); 
    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i);
      boolean isLast = (i+1 == subterms.size());

      if(!isLast) {
	s.write(deep+1,"if(!setChoicePoint()) {\n");
	s.write(deep+1,"/* Si la strategie suivante echoue, on passe a la suivante */\n");
	deep++;
      }
      
      sterm.compile(s,deep+1, matchSubtermCode);
      s.write(deep+1,"/* La strategie a donne un resultat */\n");
      s.write(deep+1,"goto stratLab" + getExitLabel() + ";\n");
      
      if(!isLast) {
	deep--;
	s.write(deep+1,"}\n");
	s.write(deep+1,"/* On vient d'un fail, on essai la strategie suivante */\n");
      } 
    }

    s.write(deep,"stratLab" + getExitLabel() + ":;\n");
    s.write(deep+1,"CUTCLOSE(); /* DC v2 */\n"); 
    s.write(deep,"}\n");
  }
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "ONE" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_ONE";
  }
 }
