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

public class SymbolAC extends SymbolCode {

  public SymbolAC(int code, Lexem[] lexArray, Lexem sort) {
    super(code,lexArray,sort);
  }

  public String genFsymInit(int deep) {
    StringBuffer s = new StringBuffer();
    int arity = -1;
    int semantic = 0 ;
    int defstrat = 0;
    Tools.indent(s,deep);
    s.append("Gfsym_init(code_" + getSymbolCode() + "," + arity + "," +
	     "\"" + this + "\"," + 
             "\"" + getSort().getName() + "\"," +  // [hassen: Jun 28 01]
	     semantic + "," + defstrat + ", NULL" + ");\n");
    return s.toString();
  }

  public static Symbol getCreate(int id) {
    throw new InternalError("Not defined");
  }

  public String toString( Vector subterms ) {
    StringBuffer s = new StringBuffer();
    int i;
    s.append(this);
    s.append("[");
    for(i=0 ; i<subterms.size()-1 ; i++) {
      s.append( subterms.elementAt(i) );
      s.append( "." );
    }
    s.append( subterms.elementAt(i) );
    s.append("]");
    return s.toString();
  }

}



