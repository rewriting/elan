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

public class LexemStrategyName extends Lexem {
  private static Hashtable table = new Hashtable(40);

  private static int maxCode = 0;

  public LexemStrategyName(int code, String name) {
    super(code,name);
    if (code >= maxCode) maxCode = code+1;
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

  public static Lexem newStrategyName() {
    Lexem lex = new LexemStrategyName(maxCode,"new_lexem"+maxCode);
    lex.put();
    return lex;
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

  public static void genCode(OutputCode s) {
    String sIdentifier = "char *tabStrategyStr[] = {\n";
    String sIndex = "int tabStrategyIndex[] = {\n";
    int cpt = 0;
    
    Enumeration e = table.keys();
    while(e.hasMoreElements()) {
      String key = (String) e.nextElement();
      Lexem lex = (Lexem)table.get(key);
      sIdentifier += "\"" + lex.name + "\",\n";
      sIndex += lex.code + ",\n";
      cpt++;
    }
    sIdentifier += "\"\"};\n";
    sIndex += "0};\n";
    s.write(sIdentifier);
    s.write(sIndex);
    s.write("int tabStrategySize = " + cpt + ";\n");
  }
  
}

