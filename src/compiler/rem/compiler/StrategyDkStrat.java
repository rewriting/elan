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

public class StrategyDkStrat extends StrategyTerm {

  public StrategyDkStrat(Vector sub) {
    super(sub);
  }
  
  protected void setDetType() {
    detTypeState=start;
    detType = ((StrategyTerm) subterms.firstElement()).getDetType();
    for(int i=1 ; i<subterms.size() ; i++) {
      StrategyTerm term = (StrategyTerm) subterms.elementAt(i);
      detType = detType.or(term.getDetType());
    }
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.nonDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    //int lab=StrategyTerm.strategyLabel++;

    Tools.indent(s,deep); s.write("{\n");
    super.compile(s,deep+1,matchSubtermCode);

    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm sterm = (StrategyTerm) subterms.elementAt(i);
      boolean isLast = (i+1 == subterms.size());

      if(!isLast) {
	Tools.indent(s,deep+1); s.write("if(!setChoicePoint()) {\n");
	Tools.indent(s,deep+1); s.write("/* Si la strategie suivante echoue, on passe a la suivante */\n");
	deep++;
      }
      
      sterm.compile(s,deep+1, matchSubtermCode);

      Tools.indent(s,deep+1); s.write("goto stratLab" + getExitLabel() + ";\n");
      
      if(!isLast) {
	deep--;
	Tools.indent(s,deep+1); s.write("}\n");
	Tools.indent(s,deep+1); s.write("/* On vient d'un fail */\n");
      }
    }

    Tools.indent(s,deep); s.write("stratLab" + getExitLabel() + ":;\n");
    Tools.indent(s,deep); s.write("}\n");
  }
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "DK" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "_DK";
  }
}
