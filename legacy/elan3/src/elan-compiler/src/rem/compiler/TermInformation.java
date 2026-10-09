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

class TermInformation {
  public int varNumber=-1;
  public int ekerVarNumber=-1;

  /**
   * if it is used at least once in the rhs
   */  
  private boolean shared;
  private boolean perfectShare;
  public Term sharedTerm;
  public int numberOfShare=0;

  // where the variable instantiation is stored in AC matching
  public int substIndex=-1; 
  public boolean dioVariable;
  public boolean instantiated;
  public boolean reduced;
  public boolean saved;
  public boolean matchingVariable;
  public boolean numbered;
  public boolean fullNumbering;
  public boolean linearVariable=false;// [NGUYEN: Mar 26 01] 
  public boolean abstractACVariable=false;// [NGUYEN: Mar 26 01] 

  public boolean isNotRhsLinear() {
    return (numberOfShare>=2);
  }

  public boolean isShared() {
      return shared;
  }
  public void setShared() {
      shared=true;
  }
  public boolean isPerfectShare() {
      return perfectShare;
  }
  public void setPerfectShare() {
      perfectShare=true;
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    if(matchingVariable) {
      s.append("[mv]");
    }
        s.append("[var=" + varNumber +
	     ", subst=" + substIndex +
	     //	     ", eker=" + ekerVarNumber +
	     //", linear=" + linearVariable + // [NGUYEN: Mar 26 01]
	     ", instanciated=" + instantiated +
	     ", " + shareTypeToString());
    if(numberOfShare>0) {
      s.append(", " + numberOfShare);
    }
    s.append("]");
    return s.toString();
  }

  private String shareTypeToString() {
    if(isPerfectShare()) {
      return "PERFECTSHARE";
    } else if(!shared) {
      return "NOSHARE";
    } else {
      return "UNDEFINED";
    }
  }
}
