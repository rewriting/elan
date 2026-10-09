//package Foo;

import java.util.*;

abstract class Term  {

  private static Map m = new HashMap();

  public Term subterm;

  public abstract Term alloc();
  
  public static void init(String symbol, Term term) {
    m.put(symbol,term);
    System.out.println("init Term with [" + symbol + "," + term + "]");
  }
  
  public static Term build(String symbol, Term arg1) {
    System.out.println("build Term[" + arg1 + "]");
    Term term = (Term) m.get(symbol);
    System.out.println("term = " + term);
    Term res = (Term) term.alloc();
    res.subterm = arg1;
    return res;
  }
  
}

class Suc extends Term  {
  public static String symbol = "suc";

  public Term alloc() {
    System.out.println("alloc Suc");
    return new Suc();
  }
  
  public static Suc build(Term arg1) {
    System.out.println("build Suc[" + arg1 + "]");
    Suc res = (Suc) build(symbol,arg1);
    return res;
  }

  public String toString() {
    return symbol + "(" + subterm + ")";
  }
  
}

class Test1 {

  static public void main(String[] args) {

    Term.init("suc", new Suc());

    Suc s1 = Suc.build(null);

    System.out.println("s1 = " + s1);
    
    Suc s2 = Suc.build(s1);

    System.out.println("s2 = " + s2);
    System.out.println("s2.getClass = " + s2.getClass());
    
  }

}
