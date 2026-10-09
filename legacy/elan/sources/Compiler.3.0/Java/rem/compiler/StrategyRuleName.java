package rem.compiler;

import java.util.*;

public class StrategyRuleName extends StrategyTerm {
  /**
   * name of a rule
   */
  private Lexem name;

  public StrategyRuleName(Lexem name) {
    super();
    this.name = name;
  }
  
  protected void setDetType() {
    throw new InternalError("not implemented");
  }

  protected DetType getDefaultDetType() {
    throw new InternalError("not implemented");
  }

  public void compile(OutputCode s) {
    s.write("str_" + name.getCode());
  }

  public String toString() {
    return name.toString();
  }

  public boolean isStrategyRuleName() {
    return true;
  }

  public Vector getNamedRules() {
    return RewriteRule.getTable(name);
  }
  
}
