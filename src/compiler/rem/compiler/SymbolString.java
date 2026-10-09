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

public class SymbolString extends SymbolValue {
  private static Hashtable table = new Hashtable(40);

  public SymbolString(String stringValue, Lexem sort) {
    super(new String(stringValue),sort);
  }

  public String setTag() {
    return "GsetStringTag";
  }

  public static Symbol get(String stringValue) {
    return (Symbol) table.get("sym" + stringValue);
  }

  public void put() {
    String stringValue = (String)getSymbolValue();
    Symbol oldSym = (Symbol)table.put("sym" + stringValue, this);
    if( oldSym != null ) {
      if (Flags.warnings) {
        System.out.println("Warning: '" + oldSym + "' and '" + this + "' have the same code");
      }
    }
  }

  public Object getSymbolValue() {
      //System.out.println("value1 = " + value);
    String result;
    char oldArray[] = value.toString().toCharArray();
    char newArray[] = new char[2*oldArray.length];
    int cpt=0;
    for(int i=0 ; i<oldArray.length ; i++) {
      if(oldArray[i] == '\\') { 
        newArray[cpt++] = '\\';
        newArray[cpt++] = '\\';
      } else if(oldArray[i] == '\n') {
        newArray[cpt++] = '\\';
        newArray[cpt++] = 'n';
      } else {
        newArray[cpt++] = oldArray[i];
      }
    }
    result = new String(newArray,0,cpt);
      //System.out.println("value2 = " + new String(newArray,0,cpt));
    
    return result;
  }



}

