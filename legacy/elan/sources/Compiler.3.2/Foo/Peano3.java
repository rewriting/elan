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
  protected Symbol symbol;
  protected static HashMap h = new HashMap();

  public abstract int getArity();
  public abstract Term normalize();
  public abstract String toString();
    
  public Symbol getSymbol() {
    return symbol;
  }

  public abstract Term getSubterm(int n);
  public abstract void setSubterm(Term t,int n);

//  public abstract boolean equals(Term t);

}

abstract class ConstantTerm extends Term {

  public int getArity() {
    return 0;
  }

  public Term getSubterm(int n) {
    return null;
  }

  public void setSubterm(Term t,int n) {
  }
   
  public String toString() {
    return getSymbol().toString();
  }

  public boolean equals(Object o) {
    boolean res = (o instanceof ConstantTerm)
      && getSymbol().equals(((ConstantTerm)o).getSymbol());
    System.out.println(this + " == " + o + " => " + res);
    return res;
  }

  public int hashCode() {
    return getSymbol().hashCode();
  }
}

abstract class UnaryTerm extends Term {
  Term subterm;
  
  public UnaryTerm(Symbol symbol,Term subterm) {
    this.symbol = symbol;
    this.subterm = subterm;
  }

  public int getArity() {
    return 1;
  }

  public Term getSubterm(int n) {
    return subterm;
  }

  public void setSubterm(Term t,int n) {
    subterm=t;
  }
  
  public String toString() {
    return getSymbol().toString() + "(" + getSubterm(0).toString()  + ")";
  }
/*
  public boolean equals(Object o) {
    boolean res = (o instanceof UnaryTerm)
      && getSymbol().equals(((UnaryTerm)o).getSymbol())
      && getSubterm(0).equals(((UnaryTerm)o).getSubterm(0));
    System.out.println(this + " == " + o + " => " + res);
    return res;
  }
*/
  public boolean equals(Object o) {
    boolean res = (o instanceof UnaryTerm)
      && getSymbol() == ((UnaryTerm)o).getSymbol()
      && getSubterm(0) == ((UnaryTerm)o).getSubterm(0);
    System.out.println(this + " == " + o + " => " + res);
    return res;
  }


  public int hashCode() {
    int hnr =  getSymbol().hashCode();
    return getSubterm(0).hashCode() ^ (hnr << 1) ^ (hnr >> 1);
  }

}

abstract class BinaryTerm extends Term {
  Term subterm1;
  Term subterm2;

  public BinaryTerm(Term subterm1, Term subterm2) {
    this.subterm1 = subterm1;
    this.subterm2 = subterm2;
  }

  public int getArity() {
    return 2;
  }

  public Term getSubterm(int n) {
    switch(n) {
        case 0: return subterm1;
        case 1: return subterm2;
        default: return null;
    }
  }

  public void setSubterm(Term t,int n) {
    switch(n) {
        case 0: subterm1=t;
        case 1: subterm2=t;
    }
  }

  public String toString() {
    return getSymbol().toString() + "(" +
      getSubterm(0).toString()  + "," + getSubterm(1).toString()  + ")";
  }

  public boolean equals(Object o) {
    boolean res = (o instanceof BinaryTerm)
      && getSymbol().equals(((BinaryTerm)o).getSymbol())
      && getSubterm(0).equals(((BinaryTerm)o).getSubterm(0))
      && getSubterm(1).equals(((BinaryTerm)o).getSubterm(1));
    System.out.println(this + " == " + o + " => " + res);
    return res;
  }

  public int hashCode() {
    int hnr =  getSymbol().hashCode();
    hnr = getSubterm(0).hashCode() ^ (hnr << 1) ^ (hnr >> 1);
    return getSubterm(1).hashCode() ^ (hnr << 1) ^ (hnr >> 1);
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

class ImplUnaryTerm extends UnaryTerm {
  public ImplUnaryTerm(Symbol symbol, Term subterm) { 
    super(symbol, subterm);
  }

  public static Term build(Symbol symbol, Term subterm) {
    Term term = new ImplUnaryTerm(symbol,subterm);
    if(h.containsKey(term)) {
      System.out.println(term + " already in hash");
      return (Term) h.get(term);
    } else {
      System.out.println(term + " hashed");
      h.put(term,term);
      return term;
    }
  }
    
  public Symbol getSymbol() {
    return symbol;
  }
  
  public Term normalize() {
    return this;
  }
}

class Suc extends ImplUnaryTerm {
  private static Symbol staticSymbol = new Symbol("Suc");

  public Suc(Term subterm) { 
    super(staticSymbol,subterm);
  }

  public static Suc build(Term subterm) {
    return (Suc)ImplUnaryTerm.build(staticSymbol,subterm);
  }
    
}

/*
class Suc extends UnaryTerm {
  private static Symbol symbol = new Symbol("Suc");

  public Suc(Term subterm) { 
    super(subterm);
  }

  public static Term build(Term subterm) {
    Term term = new Suc(subterm);
    if(h.containsKey(term)) {
      System.out.println(term + " already in hash");
      return (Term) h.get(term);
    } else {
      System.out.println(term + " hashed");
      h.put(term,term);
      return term;
    }
  }
    
  public Symbol getSymbol() {
    return symbol;
  }
  
  public Term normalize() {
    return this;
  }
}
*/
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
      // plus(0,x) = x
    Term v0 = getSubterm(0);
    if(v0.getClass() == Zero.class) {
      return getSubterm(1);
    }
      // plus(s(x),y) => plus(x,s(y))
    if(v0.getClass() == Suc.class) {
      return new Plus(v0.getSubterm(0),
                      Suc.build(getSubterm(1))).normalize();
    }
    return this;
  }
}

class Fib extends UnaryTerm {
  private static Symbol staticSymbol = new Symbol("Fib");

  public Fib(Term subterm) { 
    super(staticSymbol,subterm);
  }

  public Symbol getSymbol() {
    return symbol;
  }

  public Term normalize() {
      // fib(0) = suc(0)
    Term v0 = getSubterm(0);
    if(v0.getClass() == Zero.class) {
      return Suc.build(v0);
    }
      // fib(suc(0)) => suc(0)
      //Term v0 = getSubterm(0);
    if(v0.getClass() == Suc.class) {
      Term v1 = v0.getSubterm(0);
      if(v1.getClass() == Zero.class) {
        return v0;
      }
    }
      //  fib(s(s(x))) => plus(fib(x),fib(s(x)))
      //Term v0 = getSubterm(0);
    if(v0.getClass() == Suc.class) {
      Term v1 = v0.getSubterm(0);
      if(v1.getClass() == Suc.class) {
        Term v2 = v1.getSubterm(0);
        Term fib1 = new Fib(v2).normalize();
        Term fib2 = new Fib(v1).normalize();
        return new Plus(fib1,fib2).normalize();
      }
    }
    return this;
  }
}

class Peano3 {

  static public void main(String[] args) {
    Term zero = Zero.getInstance();
    Term one = Suc.build(zero);
    Term two = Suc.build(one);
    Term three = Suc.build(two);
    Term four = Suc.build(three);
    Term five = Suc.build(four);
    Term ten = new Plus(five,five).normalize();
    Term fifteen = new Plus(ten,five).normalize();
    Term twenty = new Plus(fifteen,five).normalize();
    Term twentyTwo = new Plus(twenty,two).normalize();

    long start = System.currentTimeMillis();

//    Term query = new Fib(twentyTwo).normalize();
    Term query = new Fib(ten).normalize();

    long end   = System.currentTimeMillis();

    System.out.println("time = "+(end-start)+" ms");
    
    System.out.println("result = " + query );

    System.out.println("four.hashCode = " + four.hashCode());
    Term tmp = Suc.build(three);
    System.out.println("Suc(three).hashCode = " + tmp.hashCode());
    System.out.println(four + " .equals( " + tmp + " ) = " + four.equals(tmp));

    System.out.println("--------------------------------------------------");
    
    HashMap h = new HashMap();
    h.put(four,four);
    System.out.println("four == tmp => " + (four == tmp));
    System.out.println("key   four ? " + h.containsKey(four));
    System.out.println("value four ? " + h.containsValue(four));
    System.out.println("key   tmp  ? " + h.containsKey(tmp));
    System.out.println("value tmp  ? " + h.containsValue(tmp));

    System.out.println("--------------------------------------------------");
    Term hOne = Suc.build(zero);
    Term hTwo = Suc.build(hOne);
    Term hThree = Suc.build(hTwo);
    Term hThreeBis = Suc.build(hTwo);
    
  }


}
