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

public abstract class Lexem implements Comparable {
  protected int code;
  protected String name="<unnamed>";

  public abstract void put();

  public Lexem(int code, String name) {
    this.code = code;
    this.name = name;
  }

  public int getCode() {
    return code;
  }

  public String getName() {
    return name;
  }

  public int compareTo(Object o) {
    if(o instanceof Lexem) {
      String s1 = toStringCompare();
      String s2 = ((Lexem)o).toStringCompare();
        //System.out.println(s1 + ".compareTo(" + s2 + ") = " + s1.compareTo(s2));
      return s1.compareTo(s2);
    } else if(o instanceof Symbol) {
      String s1 = toStringCompare();
      String s2 = ((Symbol)o).toStringCompare();
        //System.out.println(s1 + ".compareTo(" + s2 + ") = " + s1.compareTo(s2));
      return s1.compareTo(s2);
    } else {
      throw new ClassCastException();
    }
  }

  public String toStringCompare() {
    return this.name + this.code;
  }
  
  public String toString() {
    // "<lex" + code + ":" + name + ">";
    if(name.equals("\\")) {
      /*
       * protection du backslash
       */
      return "\\" + name;
    } else {
      return name;
    }
  }
  
}

