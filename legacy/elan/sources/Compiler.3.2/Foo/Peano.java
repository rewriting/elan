//package Foo;

import java.util.*;

class Symbol {
  public String symbol;

  public Symbol(String symbol) {
    this.symbol=symbol;
  }
  
  public String toString() {
    return symbol;
  }
}

abstract class Term {
  protected static Symbol symbol;
  protected Term[] subterm;
  
  public abstract int getArity();
  public abstract Term normalize();
  public abstract String toString();
    
  public Symbol getSymbol() {
    return symbol;
  }

  public Term getSubterm(int n) {
    return subterm[n];
  }

  public Term setSubterm(Term t,int n) {
    return subterm[n]=t;
  }

}

abstract class ConstantTerm extends Term {
  public int getArity() {
    return 0;
  }

  public String toString() {
    return getSymbol().toString();
  }
}

abstract class UnaryTerm extends Term {
  public UnaryTerm() {
    this.subterm = new Term[1];
  }

  public UnaryTerm(Term subterm) {
    this.subterm = new Term[1];
    this.subterm[0] = subterm;
  }

  public int getArity() {
    return 1;
  }

  public String toString() {
    return getSymbol().toString() + "(" + getSubterm(0).toString()  + ")";
  }

}

abstract class BinaryTerm extends Term {
  public BinaryTerm() {
    subterm = new Term[2];
  }
  
  public BinaryTerm(Term subterm1, Term subterm2) {
    this.subterm = new Term[2];
    this.subterm[0] = subterm1;
    this.subterm[1] = subterm2;
  }

  public int getArity() {
    return 2;
  }

  public String toString() {
    return getSymbol().toString() + "(" +
      getSubterm(0).toString()  + "," + getSubterm(1).toString()  + ")";
  }
}

/**************************************************
 * 
 **************************************************/

class Zero extends ConstantTerm {
  private static Symbol symbol = new Symbol("Zero");
  private static Zero uniqueInstance = new Zero();
  
  public Symbol getSymbol() {
    return symbol;
  }

  public Term normalize() {
    return this;
  }

  public static Zero getInstance() {
    return uniqueInstance;
  }
  
}

class Suc extends UnaryTerm {
  private static Symbol symbol = new Symbol("Suc");

  public Suc(Term subterm) { 
    super(subterm);
  }

  public Symbol getSymbol() {
    return symbol;
  }
  
  public Term normalize() {
    return this;
  }
}

class MatchingFailureException extends Exception {
  public static MatchingFailureException uniqueInstance = new MatchingFailureException();
}

class Plus extends BinaryTerm {
  private static Symbol symbol = new Symbol("Plus");

  public Plus(Term subterm1, Term subterm2) { 
    super(subterm1,subterm2);
  }


  public Symbol getSymbol() {
    return symbol;
  }

  public Term normalize() {

    try {
      return rule_1();
    } catch (MatchingFailureException e1) {
    }
    
    try {
      return rule_2();
    } catch (MatchingFailureException e2) {
    }

    return this;
  }
  
  public Term rule_1()
    throws MatchingFailureException {
    // plus(0,x) = x
    if(getSubterm(0) instanceof Zero) {
      return getSubterm(1);
    }
    throw MatchingFailureException.uniqueInstance;
  }
      
  public Term rule_2()
    throws MatchingFailureException {
    // plus(s(x),y) => plus(x,s(y))
    Term v0 = getSubterm(0);
    if(v0 instanceof Suc) {
      return new Plus(v0.getSubterm(0),
                      new Suc(getSubterm(1))).normalize();
    }
    throw MatchingFailureException.uniqueInstance;
  }
  
}

class Fib extends UnaryTerm {
  private static Symbol symbol = new Symbol("Fib");

  public Fib(Term subterm) { 
    super(subterm);
  }

  public Symbol getSymbol() {
    return symbol;
  }

  public Term normalize() {
    try {
      return rule_1();
    } catch (MatchingFailureException e1) {
    }
    
    try {
      return rule_2();
    } catch (MatchingFailureException e2) {
    }

    try {
      return rule_3();
    } catch (MatchingFailureException e2) {
    }

/*

    try {
      Term res = rule_1();
      if(res!=this) return res;
      res = rule_2();
      if(res!=this) return res;
      res = rule_3();
      if(res!=this) return res;

    } catch (MatchingFailureException e2) {
    }
*/

    
    return this;
  }
  
  public Term rule_1()
    throws MatchingFailureException {
    // fib(0) = suc(0)
    Term v0 = getSubterm(0);
    if(v0 instanceof Zero) {
      return new Suc(v0);
    }
      //return this;
      throw MatchingFailureException.uniqueInstance;
  }

  public Term rule_2()
    throws MatchingFailureException {
    // fib(suc(0)) => suc(0)
    Term v0 = getSubterm(0);
    if(v0 instanceof Suc) {
      Term v1 = v0.getSubterm(0);
      if(v1 instanceof Zero) {
        return v0;
      }
    }
      //return this;
      throw MatchingFailureException.uniqueInstance;
  }

    public Term rule_3()
    throws MatchingFailureException {
    //  fib(s(s(x))) => plus(fib(x),fib(s(x)))
      Term v0 = getSubterm(0);
      if(v0 instanceof Suc) {
        Term v1 = v0.getSubterm(0);
        if(v1 instanceof Suc) {
          Term v2 = v1.getSubterm(0);
          Term fib1 = new Fib(v2).normalize();
          Term fib2 = new Fib(v1).normalize();
          return new Plus(fib1,fib2).normalize();
        }
      }
        //return this;
        throw MatchingFailureException.uniqueInstance;
    }
}


class Peano {

  static public void main(String[] args) {
    Term zero = Zero.getInstance();
    Term one = new Suc(zero);
    Term two = new Suc(one);
    Term three = new Suc(two);
    Term four = new Plus(two,two).normalize();
    Term five = new Suc(four);
    Term ten = new Plus(five,five).normalize();
    Term fifteen = new Plus(ten,five).normalize();
    Term twenty = new Plus(fifteen,five).normalize();
    Term twentyTwo = new Plus(twenty,two).normalize();


    long start = System.currentTimeMillis();

    Term query = new Fib(twentyTwo).normalize();

    long end   = System.currentTimeMillis();

    System.out.println("time = "+(end-start)+" ms");
    
      //System.out.println("result = " + query.normalize() );
  }


}
