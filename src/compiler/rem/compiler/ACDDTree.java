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

public class ACDDTree extends DDTree {

  /**
   * list of AC pattern (with the AC top symbol)
   * this is a copy of patterns
   */
  private Vector ACpattern = new Vector();
  private int numberOfPattern=0;
  private DDTree father;

  public ACDDTree(Object symbol, DDTree father) {
    super(symbol);
    this.father=father;
  }

  // return the main DDTree
  protected DDTree getRootFather() {
    return father.getRootFather();
  }

  public int getACPatternSize() {
    return ACpattern.size();
  }
   
  public int getNumberOfPattern() {
    return numberOfPattern;
  }

  public int ACinit(Term term) {
      /*
       * [pem: May 19 99]
       * un meme pattern peut etre ajoute plusieurs fois
       * si on incremente numberOfPattern ici, cela fausse
       * le compte du nombre de patterns
       * Faire attention aux regles ayant des patterns identiques
       */
    for(int i=0 ; i<ACpattern.size() ; i++) {
      if( term.deepEquals((Term)ACpattern.elementAt(i)) ) {
//        System.out.println("\n *** Echec de : " + term);
//        System.out.println("numberOfPattern = " + numberOfPattern);
	return -1;
      }
    }
    numberOfPattern++;

//    System.out.println("\n *** Ajout de : " + term);
//    System.out.println("numberOfPattern = " + numberOfPattern);

    ACpattern.addElement(term);
    for(int i=0 ; i<term.arity() ; i++) {
      init( term.getSubterm(i) );
    }
    return ACpattern.size();
  }

  public int init(Term term) {
    for(int i=0 ; i<pattern.size() ; i++) {
        /*
         * System.out.println("cmp " + term + " <--> " + ((Term)pattern.elementAt(i)));
         */
      
      if( term.cmpRename((Term)pattern.elementAt(i)) == 0 ) {
          // egalite a un renommage pres

          //System.out.println("\nTerm : " + term + " is already in the ACDDTree");
          //System.out.println("Re-insertion");
          //super.init(term);
	return i;
      }
    }

      // System.out.println("insertion de : " + term);
    
    if( term.isVariable() ) {
      // pas de variable seule dans le ACnet
      return -1;
    } else {
      return super.init(term);
    }
  }

  
  public void leftsideVariableClear() {
    //System.out.println("---------- leftsideVariableClear");
    for(int i=0 ; i<ACpattern.size() ; i++) {
      boolean full=true;
      ((Term)ACpattern.elementAt(i)).leftsideVariableClear(full);
    }
    if(ACnet!=null) {
      ACnet.leftsideVariableClear();
    }
  }

  public void leftsideVariableInit(boolean b) {
    //System.out.println("---------- leftsideVariableInit");
    for(int i=0 ; i<ACpattern.size() ; i++) {
      ((Term)ACpattern.elementAt(i)).leftsideVariableInit(b);
    }
    if(ACnet!=null) {
      ACnet.leftsideVariableInit(b);
    }
  }
  
    /*
     * numero du motif AC dans le CBG
     * lorsqu'on compte les dioPatterns
     */
  public int getACNumber(Term term) {
    for(int i=0 ; i<ACpattern.size() ; i++) {
      if( term.cmpRename((Term)ACpattern.elementAt(i)) == 0 ) {
	return i;
      }
    } 
    throw new InternalError("pattern not found:" + term);
  }

    /*
     * numero du motif AC dans le CBG
     * lorsqu'on elimine les dioPatterns
     */
  public int getCBGNumber(Term term) {
    int compteur=0;
    int res=-1;
    boolean found=false;
      //System.out.println("getCBGNumber(" + term + ") ");

    for(int i=0 ; !found && i<ACpattern.size() ; i++) {
      Term acterm = (Term)ACpattern.elementAt(i);

        //System.out.println("\tacterm[" + i + "] = " + acterm);

      if( term.cmpRename(acterm) == 0 ) {
	found=true;
	if(acterm.isDio()) {
	  res = -1;
	} else {
	  res = compteur;
	}
      } else {
	if(!acterm.isDio()) {
	  compteur++;
	}
      }
    } 
    if(!found) {
      throw new InternalError("pattern not found:" + term);
    }
    //System.out.println("res = " + res);
    return res;
  }

  public void genPatternListConstruction(OutputCode s,int deep,String prefix) {
    Information[] list = father.collectInformation();
//    System.out.println("collectInformation: ");
    for(int i=0; i<list.length; i++) {
      if(list[i] != null) {
//        System.out.println("list[" + i + "] = " + list[i]);
        list[i].genPatternListConstruction(s,deep,prefix,i);
      } else {
        Information.genPatternListConstruction(s,deep,prefix);
      }
    }
  }
  
  
  public String toString() {
    StringBuffer s = new StringBuffer();
    for(int i = 0 ; i < ACpattern.size() ; i++) {
      s.append( "AC  [" + i + "] " + ACpattern.elementAt(i) + "\n");
    }
    s.append( super.toString() );
    return s.toString();
  }

}

