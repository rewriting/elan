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

public class Flatterm implements Cloneable {
  private Symbol symbol;
  private Flatterm next;
  private Flatterm prev;
  private Flatterm end;
  //private Flatterm top;
  /** multiplicity in Ordered Normal Form AC representation */
  private int multiplicity;

  /* To each Flatterm a classical Term is associated */
  private Term term;
  /*
   * To each Flatterm a DDNode is associated.
   * The DDNode is in the main branch of the DDTree
   */
  //private DDNode node;

  public Flatterm(Symbol s) {
    symbol = s;
    end=this;
  }


  public Flatterm(Term term) {
    this(term.getSymbol());

    Flatterm subterm;
    Flatterm lastSubterm;

    this.term = term;
    this.multiplicity=term.getMultiplicity();

    if( term.arity() != 0 ) {
      subterm=new Flatterm(term.getSubterm(0));
      lastSubterm=subterm;

      for(int i=1 ; i<term.arity() ; i++) {
	Flatterm ft;
	ft = new Flatterm(term.getSubterm(i));
	lastSubterm.concat(ft);
	lastSubterm=ft;
      }
      heading(subterm);
    }
  }    

  /*
  public boolean equals(Object obj) {
    //System.out.println(this + ".equals(" + obj + ")");
    if ((obj != null) && (obj instanceof Flatterm)) {
      if (this == obj) {
	//System.out.println("true1");
	return true;
      }
      Flatterm term=(Flatterm) obj;
      if(getSymbol()!=term.getSymbol()) {
	//System.out.println("false1");
	return false;
      } else if(this != this.end) {
	Flatterm current=this.next;
	Flatterm currentTerm=term.next;
	boolean res=true;
	while( res && current != this.end.next ) {
	  res &= current.equals(currentTerm);
	  current = current.end.next;
	  currentTerm = currentTerm.end.next;
	}
	if(!res) {
	  //System.out.println("false2: " + term.getNext());
	  return false;
	}
      }
      
      if(!getTerm().equals(term.getTerm())) {
	System.out.println("Warning: associated Terms are not equal");
      }
      //System.out.println("true2");
      return true;
    }
    //System.out.println("false4");
    return false;
  }
  */

  /*
  public boolean equals(Flatterm term) {
    if(this==term) return true;
    if(term==null) return false;
    boolean res;
    res = getSymbol()==term.getSymbol();
    if(getNext()==null) {
      res&= term.getNext()==null;
    } else {
      res&= getNext().equals(term.getNext());
    }

    if(res && !getTerm().equals(term.getTerm())) {
      System.out.println("Warning: associated Terms are not equal");
    }
    return res;
  }
  */

  public void concat(Flatterm subterm) {
    this.end.next = subterm;
    subterm.prev = this.end;
  }

  public void heading(Flatterm subterm) {
    if(subterm != null) {
      subterm.prev=this;
      this.next=subterm;
      //subterm.top=this;
    }
    while(subterm != null) {
      //subterm.top=this;
      this.end=subterm.end;
      subterm=subterm.end.next;
    }
  }
  
  public Symbol getSymbol() {
    return symbol;
  }
  public Flatterm getNext() {
    return next;
  }
  public Flatterm getPrev() {
    return prev;
  }
  public Flatterm getEnd() {
    return end;
  }
  /*
    public Flatterm getTop() {
    return top;
  }
  */
  public Term getTerm() {
    return term;
  }

  public int getMultiplicity() {
    return multiplicity;
  }
  /*
  public DDNode getNode() {
    return node;
  }
  */

  /* used in DDNode */
  /*
  public void setNode(DDNode node) {
    this.node=node;
  }
  */

  public String toString() {
    StringBuffer s = new StringBuffer();
    Flatterm current;

    s.append( symbol );
    if(this != this.end) {
      s.append("(");
      current=this.next;
      while( current != this.end.next ) {
	s.append(current);
	current = current.end.next;
	if (current != this.end.next) 
	  s.append(",");
      }
      s.append(")");
    }
    if(getMultiplicity()>1) {
      s.append("[" + this.getMultiplicity() + "]");
    }
    return s.toString();
  }

  public Object clone() {
    try { 
      Flatterm tail = (Flatterm)super.clone();
      tail.symbol=symbol;
      tail.term=term;
      tail.multiplicity=multiplicity;
      Flatterm func=tail;
      Flatterm arg=this.next;
      Flatterm end=this.end.next;
      while(arg != end) {
	Flatterm argCopy=(Flatterm)arg.clone();
	argCopy.prev=tail;
	tail.next=argCopy;
	//argCopy.top=func;
	tail=argCopy.end;
	func.end=tail;
	arg=arg.end.next;
      }
      return func;
    } catch (CloneNotSupportedException e) { 
      // this shouldn't happen, since we are Cloneable
      throw new InternalError();
    }
  }


}

