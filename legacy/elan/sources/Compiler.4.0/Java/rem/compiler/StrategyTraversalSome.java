package rem.compiler;

import java.util.*;

public class StrategyTraversalSome extends StrategyTerm {

  public StrategyTraversalSome(Vector sub) {
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
    Tools.genComment(s,deep+1,"compilation of tsome(" + subStrategy +")");
    s.write(deep+1,"int i,j,mult,arity;\n");
    s.write(deep+1,"Gterm *newTerm;\n");
    s.write(deep+1,"int localWasr=0;\n");
    s.write(deep+1,"arg0=res;\n");
    s.write(deep+1,"arity = genericGetArity(arg0);\n");

    s.write(deep+1,"if(arity!=0) {\n");
    s.write(deep+2,"genericTermAlloc(newTerm,arity,getSymb(arg0));\n");
    s.write(deep+1,"}\n");
    s.write(deep+1,"for(i=0 ; i<arity ; i++) {\n");
    s.write(deep+2,"v0 = genericGetSubterm(arg0,i);\n");
    s.write(deep+2,"mult = genericGetMult(arg0,i);\n");
    s.write(deep+2,"//printf(\"v0 = \"); internal_term_println(stdout,v0,resultMode);\n");

    s.write(deep+2,"if(setChoicePoint()) {\n");
    s.write(deep+3,"for(j=0 ; j<mult ; j++) {\n");
    s.write(deep+4,"genericSetSubterm(newTerm,i,v0);\n");
    s.write(deep+3,"}\n");
    s.write(deep+2,"} else {\n");

    Tools.genComment(s,deep+3,"begin " + subStrategy +"");
    subStrategy.compile(s,deep+3, matchSubtermCode);
    Tools.genComment(s,deep+3,"end   " + subStrategy +"");

    s.write(deep+3,"localWasr=1;\n");
    s.write(deep+3,"for(j=0 ; j<mult ; j++) {\n");
    s.write(deep+4,"genericSetSubterm(newTerm,i,res);\n");
    s.write(deep+3,"}\n");
    s.write(deep+2,"} // choice\n");
    s.write(deep+1,"} // for\n");

    s.write(deep+1,"if(localWasr) {\n");
    s.write(deep+2,"res = specialApply(newTerm);\n");
    s.write(deep+1,"} else {\n");
    Tools.genFail(s,deep+2);
    s.write(deep+1,"}\n");

    
    s.write(deep,"}\n");
    
  }
 
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "tsome" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_tsome";
  }
  
}
