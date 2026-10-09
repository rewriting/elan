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

public class StrategyCall extends StrategyTerm {
  protected Lexem name;

  public StrategyCall(Lexem name) {
    super();
    this.name = name;
  }

  protected void setDetType() {
    detTypeState=start;
    detType = Strategy.getStrategy(name).getDetType();
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    return DetType.nonDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    Tools.indent(s,deep); s.write("res = str_" + name.getCode() + "(v0);\n");
    Tools.indent(s,deep); s.write("v0 = res;\n");
  }
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "call(" + name + ")" );
    s.append( super.toString() );
    return s.toString();
  }

  public String getName() {
    return super.getName() + "call";
  }
  
}
