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
  
  public abstract int getArity();
  public abstract Term normalize();
  public abstract String toString();
    
  public Symbol getSymbol() {
    return symbol;
  }

  public abstract Term getSubterm(int n);
  public abstract void setSubterm(Term t,int n);
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
}

abstract class UnaryTerm extends Term {
  Term subterm;
  
  public UnaryTerm(Term subterm) {
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
/*
  public Term normalize() {
      // plus(0,x) = x
    Term v0 = getSubterm(0);
    if(v0 instanceof Zero) {
      return getSubterm(1);
    }

      // plus(s(x),y) => plus(x,s(y))
    if(v0 instanceof Suc) {
      return new Plus(v0.getSubterm(0),
                      new Suc(getSubterm(1))).normalize();
    }
    return this;
   }
*/

  public Term normalize() {
    Term res = this;

    while(true) {
        // plus(0,x) = x
      Term v0 = res.getSubterm(0);
      if(v0 instanceof Zero) {
        res = res.getSubterm(1);
        break;
      }

        // plus(s(x),y) => plus(x,s(y))
      if(v0 instanceof Suc) {
        res = new Plus(v0.getSubterm(0),new Suc(res.getSubterm(1)));
        continue;
      }
      break;
    }
    return res;
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
    Term res = this;
    while(true) {
        // fib(0) = suc(0)
      Term v0 = res.getSubterm(0);
      if(v0 instanceof Zero) {
        res = new Suc(v0);
        break;
      }
        // fib(suc(0)) => suc(0)
      if(v0 instanceof Suc) {
        Term v1 = v0.getSubterm(0);
        if(v1 instanceof Zero) {
          res = v0;
          break;
        }
      }
        //  fib(s(s(x))) => plus(fib(x),fib(s(x)))
      if(v0 instanceof Suc) {
        Term v1 = v0.getSubterm(0);
        if(v1 instanceof Suc) {
          Term v2 = v1.getSubterm(0);
          Term fib1 = new Fib(v2).normalize();
          Term fib2 = new Fib(v1).normalize();
          res = new Plus(fib1,fib2).normalize();
          break;
        }
      }
      break;
    }
    return res;
  }
}

class Peano2 {

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
    Term twentyFive = new Plus(twenty,five).normalize();
    Term thirty = new Plus(twenty,ten).normalize();


    long start = System.currentTimeMillis();

    Term query = new Fib(thirty).normalize();

    long end   = System.currentTimeMillis();

    System.out.println("time = "+(end-start)+" ms");
    
//    System.out.println("result = " + query );
  }


}
