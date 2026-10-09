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

public class StrategyMeta extends StrategyTerm {

  public StrategyMeta() {
    super();
  }

  protected void setDetType() {
    // TODO
    detType = DetType.nonDetType;
    detTypeState=done;
  }

  protected DetType getDefaultDetType() {
    // TODO
    return DetType.nonDetType;
  }

  public void compile(OutputCode s,int deep, OutputCode matchSubtermCode) {
    super.compile(s,deep,matchSubtermCode);
    // TODO
    //Tools.indent(s,deep); s.write("res=v0;\n");
    Tools.indent(s,deep); s.write("res=term_metaApply(v0);\n");
  }
  

  public String toString() {
    return "Meta";
  }

  public String getName() {
    return super.getName() + "_Meta";
  }

}
