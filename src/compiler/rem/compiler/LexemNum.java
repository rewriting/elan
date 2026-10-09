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

public class LexemNum extends Lexem {
  private static Hashtable table = new Hashtable(10);

  public LexemNum(int code) {
    super(code, (new Character((char)(code+(int)'0'))).toString() );
  }

  public void put() {
    Lexem old = (Lexem) table.put("lex" + code, this);
    if( old != null ) {
      if (Flags.warnings) {
	System.out.println("Warning: '" + old + "' and '" + this + "' have the same code");
      }
    }
  }

  public static Lexem get(int code) {
    return (Lexem)table.get("lex" + code);
  }

  public static void dump() {
    Enumeration e = table.keys();
    while(e.hasMoreElements()) {
      String key = (String) e.nextElement();
      Lexem lex = (Lexem)table.get(key);
      if (Flags.verbose) {
        System.out.println("<" + key + ":" + lex + ">");
      }
    }
  }

}

