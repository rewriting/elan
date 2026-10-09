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

public class SymbolBuiltin extends SymbolCode {

  private int semantic;
  private int theory=0;

  public SymbolBuiltin(int code, Lexem[] lexArray, Lexem sort, int sem) {
    super(code,lexArray,sort);
    semantic=sem;
  }

  public SymbolBuiltin(int code, Lexem[] lexArray, Lexem sort, int sem,
		       int intTheory) {
    this(code,lexArray,sort,sem);
    theory=intTheory;
  }

  public int getSymbolSemantic() {
    return semantic;
  }	

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    Tools.indent(s,deep);
    int defstrat = 0;
    int arity=0;
    if(theory==2) {
      arity=-1;
    } else {
      arity=getArity();
    }
    s.append("Gfsym_init(code_" + getSymbolCode() + "," + arity + "," +
	     "\"" + this + "\"," +
             "\"" + getSort().getName() + "\"," +  // [hassen: Jun 28 01]
	     semantic + "," + defstrat + ", NULL" + ");\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

}

