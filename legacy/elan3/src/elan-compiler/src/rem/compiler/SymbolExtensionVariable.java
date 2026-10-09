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

public class SymbolExtensionVariable extends SymbolVariable {
  private static Hashtable table = new Hashtable(10);

  public SymbolExtensionVariable(int code, Lexem[] lexArray, Lexem sort) {
    super(code,lexArray,sort);
  }

  public static Symbol get(int code, Lexem sort) {
    return (Symbol) table.get("sym" + code + "_" + sort.getName());
  }

  public void put() {
    Symbol old = (Symbol)table.put("sym" + getSymbolCode() + "_" +
				   getSort().getName() ,this);
    if( old != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + old + "' and '" + this + "' have the same code");
      }
    }
  }

  public static Symbol getCreate(int id, Lexem sort) {
    Lexem lex = LexemVariableName.get(id);
    if(lex==null) {
      lex = new LexemVariableName(id, "var"+id);
      lex.put();
    }

    Symbol sym = SymbolExtensionVariable.get(id,sort);
    if(sym==null) {
      sym = new SymbolExtensionVariable(id, new Lexem[] {lex}, sort);
      sym.put();
    }
    return sym;
  }


}

