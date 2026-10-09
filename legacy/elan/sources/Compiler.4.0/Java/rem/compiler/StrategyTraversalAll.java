package rem.compiler;

import java.util.*;

public class StrategyTraversalAll extends StrategyTerm {

  public StrategyTraversalAll(Vector sub) {
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
    Tools.genComment(s,deep+1,"compilation of tall(" + subStrategy +")");
    s.write(deep+1,"int i,j,mult,arity;\n");
    s.write(deep+1,"Gterm *newTerm;\n");
    s.write(deep+1,"arg0=res;\n");
    s.write(deep+1,"arity = genericGetArity(arg0);\n");

    s.write(deep+1,"if(arity!=0) {\n");
    s.write(deep+2,"genericTermAlloc(newTerm,arity,getSymb(arg0));\n");
    s.write(deep+1,"} else {\n");
    s.write(deep+2,"newTerm = arg0;\n");
    s.write(deep+1,"}\n");
    s.write(deep+1,"for(i=0 ; i<arity ; i++) {\n");
    s.write(deep+2,"v0 = genericGetSubterm(arg0,i);\n");
    s.write(deep+2,"mult = genericGetMult(arg0,i);\n");
    s.write(deep+2,"//printf(\"v0 = \"); internal_term_println(stdout,v0,resultMode);\n");
    
    Tools.genComment(s,deep+2,"begin " + subStrategy +"");
    subStrategy.compile(s,deep+2, matchSubtermCode);
    Tools.genComment(s,deep+2,"end   " + subStrategy +"");

    s.write(deep+2,"for(j=0 ; j<mult ; j++) {\n");
    s.write(deep+3,"genericSetSubterm(newTerm,i,res);\n");
    s.write(deep+2,"}\n");
    s.write(deep+1,"}\n");
    s.write(deep+1,"res = specialApply(newTerm);\n");

    s.write(deep,"}\n");
    
  }
 
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "tall" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_tall";
  }
  
}
