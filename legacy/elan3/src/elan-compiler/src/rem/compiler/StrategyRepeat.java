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

      Tools.indent(s,deep+1); s.write("long wasr_index;\n");

      //if(subStrategy.isSemiDet()) {
      //Tools.indent(s,deep+1); s.write("CUTOPEN(); /* Repeat */\n");
      //}

      Tools.indent(s,deep+1); s.write("while(1) {\n");
      Tools.indent(s,deep+2); s.write("wasr_index = allocStable(sizeof(int));\n");
      Tools.indent(s,deep+2); s.write("*((int*)getStablePointer(wasr_index))=0;\n");
      if(Flags.optimiseChoicePoint && Flags.localSetChoicePoint) {
        s.write(deep+3,"if(localSetChoicePoint()) {\n");
      } else {
        s.write(deep+3,"if(setChoicePoint()) {\n");
      }
      Tools.indent(s,deep+3); s.write("if(*((int*)getStablePointer(wasr_index))!=0) {\n");
      Tools.genFail(s,deep+4);
      Tools.indent(s,deep+3); s.write("} else\n");
      Tools.indent(s,deep+4); s.write("break;\n");
      Tools.indent(s,deep+2); s.write("}\n");

      s.write(deep+2,"/* Apply the strategy */\n");
      if(Flags.debug) { 
        s.write(deep,"if(traceLevel>=3) {\n");
        s.write(deep+1,"doindent(indentlevel);\n");
        s.write(deep+1,"printf(\"REPEAT\\n\");\n");
        s.write(deep,"}\n");
      }

      for(int i=0 ; i<subterms.size() ; i++) { 
	StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i); 
	sterm.compile(s,deep+2,matchSubtermCode);
      }
      
      Tools.indent(s,deep+2); s.write("if(*((int*)getStablePointer(wasr_index))==0) {\n");
      Tools.indent(s,deep+3); s.write("*((int*)getStablePointer(wasr_index))=1;\n");
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
      
      s.write(deep+1,"long lastTerm_index;\n");
      s.write(deep+1,"CUTOPEN(); /* Repeat */\n");
      s.write(deep+1,"lastTerm_index = allocStable(sizeof(Gterm*));\n");
      s.write(deep+1,"*((Gterm**)getStablePointer(lastTerm_index))=v0;\n");
      if(Flags.optimiseChoicePoint && Flags.localSetChoicePoint) {
        s.write(deep+1,"if(localSetChoicePoint()!=0) {\n");
      } else {
        s.write(deep+1,"if(setChoicePoint()!=0) {\n");
      }
      Tools.indent(s,deep+2); s.write("res = v0 = *((Gterm**)getStablePointer(lastTerm_index));\n");
        //Tools.indent(s,deep+2); s.write("FREE(lastTerm);\n");
      s.write(deep+2,"/* End of repeat */\n");

      s.write(deep+1,"} else {\n");
      s.write(deep+2,"while(1) {\n");
      s.write(deep+2,"/* Apply the strategy */\n");
      if(Flags.debug) { 
        s.write(deep,"if(traceLevel>=3) {\n");
        s.write(deep+1,"doindent(indentlevel);\n");
        s.write(deep+1,"printf(\"REPEAT\\n\");\n");
        s.write(deep,"}\n");
      }
      
      for(int i=0 ; i<subterms.size() ; i++) {
	StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i);
	sterm.compile(s,deep+3, matchSubtermCode);
      }
      Tools.indent(s,deep+3); s.write("*((Gterm**)getStablePointer(lastTerm_index)) = res;\n");
      Tools.indent(s,deep+2); s.write("}\n"); // end while
      Tools.indent(s,deep+1); s.write("}\n"); // end if
      s.write(deep+1,"CUTCLOSE(); /* Repeat */\n");
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
