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

class AllocVariable {
  private static BitSet used = new BitSet();

  public static void init() {
    for(int i=0 ; i<used.size() ; i++) {
      used.clear(i);
    }
  }

  public static int get() {
    for(int i=0 ; i<1+used.size() ; i++) {
      if( used.get(i)==false ) {
	//System.out.println("get:  used = " + used + " : " + i);
	used.set(i);
	return i;
      }
    }
    throw new InternalError("No free variable found");   
  }

  public static void free(int n) {
    //System.out.println("free: " + n);
    if( used.get(n)==false ) {
      throw new InternalError("variable freed twice");   
    }
    used.clear(n);
  }
}
 
