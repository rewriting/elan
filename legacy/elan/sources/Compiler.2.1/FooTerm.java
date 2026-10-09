import java.util.*;

class FooTerm {

  private int symbol;
  private FooTerm subterm[];

  public FooTerm(int symb,int arity) {
    symbol=symb;
    subterm = new FooTerm[arity];
  }

  public FooTerm fib() {
    BitSet mask = new BitSet(3);
    
    switch(symbol) {
    case 1:
      mask.set(1);
      mask.set(2);
      break;
    case 0:
      mask.set(0);
      mask.set(2);
      break;
    default:
      mask.set(2);
    }

    if(mask.get(0)) {
      return new FooTerm(1,0);
    }

    if(mask.get(1)) {
      return new FooTerm(1,0);
    }

    if(mask.get(2)) {
      FooTerm sv3,sv4,sv5,sv6;
      
      sv3=new FooTerm(symbol-2,0);
      sv4=sv3.fib();
      sv5=new FooTerm(symbol-1,0);
      sv6=sv5.fib();
      return new FooTerm(sv4.symbol+sv6.symbol,0);
    }
    
    return new FooTerm(-1,0);
  }

  public String toString() {
    String s = "" + symbol;
    return s;
  }

}
