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
    s.write(deep+1,"Gterm *newTerm;\n");
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
