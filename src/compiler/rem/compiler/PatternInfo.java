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

class PatternInfo {
    // nombre de variables de multiplicite 1
  private int nbSingleACVariable=0;
    // nombre de variables de multiplicite n>1
  private int nbMultiACVariable=0;
    // nombre de sous-termes de multiplicite 1
  private int nbSingleACSubterm=0;
      // nombre de sous-termes de multiplicite n>1
  private int nbMultiACSubterm=0;
    // contient une variable apparaissant dans une localEvaluation
  private boolean constrainedVariable=false;  


  public void setNbSingleACVariable(int n) {
    nbSingleACVariable=n;
  }

  public void setNbMultiACVariable(int n) {
    nbMultiACVariable=n;
  }

  public void setConstrainedVariable() {
    constrainedVariable=true;
  }

  public int getNbSingleACVariable() {
    return nbSingleACVariable;
  }

  public int getNbMultiACVariable() {
    return nbMultiACVariable;
  }

  public boolean getConstrainedVariable() {
    return constrainedVariable;
  }

  public void setNbSingleACSubterm(int n) {
    nbSingleACSubterm=n;
  }

  public void setNbMultiACSubterm(int n) {
    nbMultiACSubterm=n;
  }

  public int getNbSingleACSubterm() {
    return nbSingleACSubterm;
  }

  public int getNbMultiACSubterm() {
    return nbMultiACSubterm;
  }
  

  public String toString() {
    String s = new String();
    s += "nbSingleACVariable  = " + nbSingleACVariable  + "\n";
    s += "nbMultiACVariable   = " + nbMultiACVariable   + "\n";
    s += "nbSingleACSubterm   = " + nbSingleACSubterm   + "\n";
    s += "nbMultiACSubterm    = " + nbMultiACSubterm    + "\n";
    s += "constrainedVariable = " + constrainedVariable + "\n";
    return s;
  }


}
