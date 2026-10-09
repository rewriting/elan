/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
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
