import java.util.*;

public class StrategyRepeat extends StrategyTerm {

  public StrategyRepeat(Vector sub) {
    super(sub);
  }

  protected void setDetType() {
    detTypeState=start;
    /*
     * subterms.size() should be equal to 1
     */
    detType = ((StrategyTerm) subterms.firstElement()).getDetType();

    if(detType.isSemiDet()) {
      detType=DetType.detType;
    } else if(detType.isNonDet()) {
      detType=DetType.multiDetType;
    } else if(detType.isDet() || detType.isMultiDet()) {
	 System.out.println("*** Warning: the strategy " + this +
					"cannot be terminating");
    }
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.multiDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    StrategyTerm subStrategy = (StrategyTerm) subterms.firstElement(); 

      //if(!Flags.optimiseChoicePoint || !subStrategy.isSemiDet()) {
      /*
       * On active tout le temp cette optimisation
       */
    if(!subStrategy.isSemiDet()) {
        // Methode de Marian
      Tools.indent(s,deep); s.write("{\n");
      super.compile(s,deep+1,matchSubtermCode);

      Tools.indent(s,deep+1); s.write("int *wasr;\n");

      //if(subStrategy.isSemiDet()) {
      //Tools.indent(s,deep+1); s.write("CUTOPEN(); /* Repeat */\n");
      //}

      Tools.indent(s,deep+1); s.write("while(1) {\n");
      Tools.indent(s,deep+2); s.write("wasr=(int*) allocStable(sizeof(int));\n");
      Tools.indent(s,deep+2); s.write("*wasr=0;\n");
      Tools.indent(s,deep+2); s.write("if(setChoicePoint()) {\n");
      Tools.indent(s,deep+3); s.write("if(*wasr!=0) {\n");
      Tools.genFail(s,deep+4);
      Tools.indent(s,deep+3); s.write("} else\n");
      Tools.indent(s,deep+4); s.write("break;\n");
      Tools.indent(s,deep+2); s.write("}\n");

      s.write(deep+2,"/* Apply the strategy */\n");
      for(int i=0 ; i<subterms.size() ; i++) { 
	StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i); 
	sterm.compile(s,deep+2,matchSubtermCode);
      }
      
      Tools.indent(s,deep+2); s.write("if(*wasr==0) {\n");
      Tools.indent(s,deep+3); s.write("*wasr=1;\n");
      Tools.indent(s,deep+2); s.write("}\n");
      Tools.indent(s,deep+1); s.write("}\n");
      Tools.indent(s,deep); s.write("}\n");

      //if(subStrategy.isSemiDet()) {
      //Tools.indent(s,deep+1); s.write("CUTCLOSE(); /* Repeat */\n");
      //}


    } else {
      // Methode PEM
      Tools.indent(s,deep); s.write("{\n");
      super.compile(s,deep+1,matchSubtermCode);
      
      s.write(deep+1,"struct term **lastTerm=(struct term **) allocStable(sizeof(struct term*));\n");
      s.write(deep+1,"*lastTerm=v0;\n");
      Tools.indent(s,deep+1); s.write("if(setChoicePoint()!=0) {\n");
      Tools.indent(s,deep+2); s.write("res = *lastTerm;\n");
      Tools.indent(s,deep+2); s.write("v0 = *lastTerm;\n");
      s.write(deep+2,"/* End of repeat */\n");

      s.write(deep+1,"} else {\n");
      s.write(deep+2,"while(1) {\n");
      s.write(deep+2,"/* Apply the strategy */\n");
      for(int i=0 ; i<subterms.size() ; i++) {
	StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i);
	sterm.compile(s,deep+3, matchSubtermCode);
      }
      Tools.indent(s,deep+3); s.write("*lastTerm = res;\n");
      Tools.indent(s,deep+2); s.write("}\n"); // end while
      Tools.indent(s,deep+1); s.write("}\n"); // end if
      Tools.indent(s,deep); s.write("}\n");
    }
  }
 
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "repeat" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_repeat";
  }
  
}
