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

public class SymbolLab extends SymbolBuiltin {
  private int defstrat;

  public SymbolLab(int code, Lexem[] lexArray, Lexem sort, int sem, int dstrat) {
    super(code,lexArray,sort,sem);
    defstrat = dstrat;
  }

  public int getSymbolDefstrat() {
    return defstrat;
  }	

  public int getRuleIndex() {
    return ElanConstants.indexLab(defstrat);
  }

  public int getApplyCode() {
    return ElanConstants.applySymbolLab(defstrat);
  }

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep);
    s.append("Gfsym_init(code_" + getSymbolCode() + "," + getArity() + "," +
	     "\"" + this + "\"," +
             "\"" + getSort().getName() + "\"," +  // [hassen: Jun 28 01]
	     getSymbolSemantic() + "," + defstrat + 
	     ", (void*)(&(str_rule" + getSymbolCode() +
	     ")) );\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}

