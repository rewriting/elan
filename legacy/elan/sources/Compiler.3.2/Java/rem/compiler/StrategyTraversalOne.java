package rem.compiler;

import java.util.*;

public class StrategyTraversalOne extends StrategyTerm {

  public StrategyTraversalOne(Vector sub) {
    super(sub);
  }

  protected void setDetType() {
    detTypeState=start;
    /*
     * subterms.size() should be equal to 1
     */
    detType = ((StrategyTerm) subterms.firstElement()).getDetType();
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.nonDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    StrategyTerm subStrategy = (StrategyTerm) subterms.firstElement(); 
    s.write(deep,"{\n");
    Tools.genComment(s,deep+1,"compilation of tone(" + subStrategy +")");
    s.write(deep+1,"int i,j,mult,arity;\n");
    s.write(deep+1,"struct term *newTerm;\n");
    s.write(deep+1,"arg0=res;\n");
    s.write(deep+1,"arity = genericGetArity(arg0);\n");
    
    s.write(deep+1,"for(i=0 ; i<arity ; i++) {\n");
    s.write(deep+2,"v0 = genericGetSubterm(arg0,i);\n");
    s.write(deep+2,"mult = genericGetMult(arg0,i);\n");
    s.write(deep+2,"//printf(\"v0 = \"); internal_term_println(stdout,v0,resultMode);\n");

    s.write(deep+2,"if(setChoicePoint()) {\n");
    Tools.genComment(s,deep+3,"la strategie a echoue: on essaie le fils suivant");
    s.write(deep+2,"} else {\n");
            
    Tools.genComment(s,deep+3,"begin " + subStrategy +"");
    subStrategy.compile(s,deep+3, matchSubtermCode);
    Tools.genComment(s,deep+3,"end   " + subStrategy +"");

    s.write(deep+3,"//printf(\"res = \"); internal_term_println(stdout,res,resultMode);\n");
    
    s.write(deep+3,"genericCopyTermAllocExcept(newTerm,i,arg0);\n");
    s.write(deep+3,"genericSetSubterm(newTerm,i,res);\n");
    s.write(deep+3,"res = specialApply(newTerm);\n");
    s.write(deep+3,"break;\n");
    s.write(deep+2,"}\n");
    s.write(deep+1,"}\n");

    s.write(deep+1,"if(i==arity) {\n");
    Tools.genFail(s,deep+2);
    s.write(deep+1,"}\n");
    
    s.write(deep,"}\n");
    
  }
 
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "tone" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_tone";
  }
  
}
