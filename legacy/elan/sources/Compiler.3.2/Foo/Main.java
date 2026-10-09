//package Foo;

import java.util.*;

class Main {

  private int symbol;
    //private Main subterm[];

  static public void main(String[] args) {
    Main query = new Main(30);
    System.out.println("result = " + query.fib() );
  }

  
  public Main(int symb) {
    symbol=symb;
      //subterm = new Main[arity];
  }

  private static Main Main_1 = new Main(1);
  
  public Main fib() {
    switch(symbol) {
    case 1:
      return Main_1;
    case 0:
      return Main_1;
    default:
      Main sv1,sv2;
      sv1=new Main(symbol-2).fib();
      sv2=new Main(symbol-1).fib();
      return new Main(sv1.symbol+sv2.symbol);
    }
  }

  public String toString() {
    String s = "" + symbol;
    return s;
  }

}
