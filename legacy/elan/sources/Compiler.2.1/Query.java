import java.util.*;

public class Query {
  private Lexem stratName;
  private Term queryTerm;
  private int sourceSort;
  
  public Query(int sourceSort, Lexem name, Term term) {
    this.sourceSort = sourceSort;
    stratName=name;
    if(term.isGround()) {
      queryTerm=term;
    } else {
      System.out.println("***\n*** The compiled query is not ground\n***");
      queryTerm=null;
        //throw new InternalError("");   
    }
  }

  public int getSourceSort() {
    return sourceSort;
  }
  public int getSourceStrategy() {
    if(stratName!=null) {
      return stratName.getCode();
    } else {
      return 0;
    }
  }
  
  public void genCode(OutputCode s,int deep) {
    BitSet b = new BitSet();

    if(queryTerm!=null) {
      
      queryTerm.setFullNumbering();
      queryTerm.rightsideVariableAffectation();
      queryTerm.rightsideVariableLiberation();
    
      s.write(deep,"{\n");
      s.write(deep+1,"/* " + queryTerm + " */\n");
    
        //queryTerm.markUsedVariable(b);
        //Tools.genDeclaration(s,deep+1,b);
      Tools.genDeclaration(s,deep+1, queryTerm.getMaxUsedVariable(0) );
    
      queryTerm.rgenRightside(s,deep+1);
      if(stratName==null) {
        s.write(deep+1,"res=" + queryTerm.genCore() + ";\n");
      } else {
        Strategy strategy = Strategy.getStrategy(stratName);
        s.write(deep+1,"res=str" + strategy.getName() +
                "(" + queryTerm.genCore() + ");\n");
      }
      s.write(deep,"}\n");
    } else {
      s.write(deep,"printf(\"No ground term is defined\\n\");\n");
      s.write(deep,"printf(\"Do not use -noInput in this case\\n\");\n");
      s.write(deep,"exit(0);\n");
    }
    
  }

  public void genStrategyCall(OutputCode s,int deep) {
    if(stratName==null) {
        /* generate nothing */
    } else {
      Strategy strategy = Strategy.getStrategy(stratName);
      s.write(deep,"res=str" + strategy.getName() + "(res);\n");
    }
  }


}

