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

public abstract class StrategyTerm
{
    /*
     * list of subterms (may be null)
     */
  protected Vector subterms;

  /*
   * back-chaining to the father StrategyTerm node
   */
  private StrategyTerm father;

  /*
   * chaining to the strategy in wich the current StrategyTerm is used
   */
  private Strategy topStrategy;

  /*
   * label to go to skip the current strategy
   */
  private int exitLabel;

  /**
   * to know if a strategy is deterministic
   */
  protected DetType detType;

    /*
     * [NGUYEN: Apr  5 01] at least one rule is AC=>true
     */
  private boolean isAC=false;

  static protected int clear=0;
  static protected int start=1;
  static protected int done=2;
  protected int detTypeState=clear;

  private static int strategyLabel=1;


  public StrategyTerm() {
    subterms = null;
    exitLabel=strategyLabel++;
  }

  public StrategyTerm(Vector sub) {
    this();
    subterms = sub;
    for(int i=0 ; i<subterms.size() ; i++) {
      StrategyTerm term = (StrategyTerm) subterms.elementAt(i);
      term.father=this;
      term.topStrategy=topStrategy;
    }
  }

  protected int getExitLabel() {
    return exitLabel;
  }

  public void setTopStrategy(Strategy root) {
    topStrategy=root;
  }

  public StrategyTerm getFather() {
    return father;
  }

  public Strategy getTopStrategy() {
    return topStrategy;
  }

  abstract protected void setDetType();
  abstract protected DetType getDefaultDetType();

  public DetType getDetType() {
    if(detTypeState==done) {
      return detType;
    } else if(detTypeState==start) {
      detType = getDefaultDetType();
      detTypeState=done;
      return detType;    }
    else if(detTypeState==clear) {
      setDetType();
      return detType;
    } else {
      throw new InternalError("unknown detTypeState");
    }
  }

  public boolean isDet() {
    return getDetType().isDet();
  }
  public boolean isExactlyDet() {
    return getDetType().isExactlyDet();
  }
  public boolean isSemiDet() {
    return getDetType().isSemiDet();
  }
  public boolean isMultiDet() {
    return getDetType().isMultiDet();
  }
  public boolean isNonDet() {
    return getDetType().isNonDet();
  }

  public boolean isNorm() {
    return false;
  }
  public boolean isNormIn() {
    return false;
  }
  public boolean isNormOut() {
    return false;
  }

  public boolean getAC() {// [NGUYEN: Apr  5 01] 
    return isAC;
  }

  public void setAC() {// [NGUYEN: Apr  5 01] 
    isAC =true;
  }

  public void compile(OutputCode s, int deep, OutputCode matchSubtermCode) { 
    Tools.indent(s,deep); s.write("/* " + this + " */\n");
  }

  public boolean isStrategyRuleName() {
    return false;
  }

  public Vector getNamedRules() {
    return null;
  }

/*
  public String getName() {  			// Peter
    if (getFather()!=null) {           
      return getFather().getName(); }
    else if (getTopStrategy() != null) {
      return getTopStrategy().getName(); }
    else {
      return "<unknown name of strategy>";
    }
  }
*/

  public String getName() {
    if(getFather()==null) {
        /*
         * symbole de tete : on donne le nom de la strategie (un code)
         */
      return getTopStrategy().getName();
    } else {
        /*
         * un sous-terme : nom du pere et sa position
         */
      return getFather().getName() + getPosition();
    }
  }

      /*--------- [NGUYEN: Apr 16 01]  -----------------*/
  public String getCallName() {
    String tmpName = getName();
    int i=1;
    
    while (tmpName.charAt(i)!='_') {
      i = i+1;
    }
    tmpName=tmpName.substring(0,i);
    return tmpName;
  }
    /*----------------- [NGUYEN: Apr 16 01] -------------*/
  private int getPosition() {
    if(getFather()==null) {
      return 0;
    } else {
      int pos;
      boolean found=false;
      Vector fatherSubterms = getFather().subterms;
      for(pos=0 ; !found && pos<fatherSubterms.size() ; pos++) {
        StrategyTerm term = (StrategyTerm)fatherSubterms.elementAt(pos);
        found = this.equals(term);
      }
      if(found) {
        return pos;
      } else {
        throw new InternalError("cannot find my position !");
      }
    }
  }
  
  

  
  public String toString() {
    StringBuffer s = new StringBuffer();
    
    s.append("[" + getDetType() + "]");

    if( subterms != null ) {
      int i;
      s.append("(");
      for(i=0 ; i<subterms.size()-1 ; i++) {
	s.append( subterms.elementAt(i) );
	s.append( "," );
      }
      s.append( subterms.elementAt(i) );
      s.append(")");
    }
    
    return s.toString();
  }
}


