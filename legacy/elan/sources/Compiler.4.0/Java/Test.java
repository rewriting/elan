//package rem;

import java.io.*;
import rem.exception.*;

public class Test {

  public static void test(int n)
    throws RuleCompiled, RuleTransformed, CompileError {
    switch(n) {
        case 1:
          throw new CompileError();
        case 2:
          throw new RuleCompiled();
        case 3:
          throw new RuleTransformed();
    }
  }
  
  public static void main(String args[]) {
    try {
      System.out.println("hello");
      Test.test(1);
      
    } catch (Exception e) {
    }
    
    
  }
}
