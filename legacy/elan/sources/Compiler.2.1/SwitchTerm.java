import java.util.*;

public class SwitchTerm extends Term {
  private SwitchTerm switchStatement;

  public SwitchTerm(Symbol termSymbol, Vector termSubterms,
		    SwitchTerm switchStatement) {
    super(termSymbol,termSubterms);
    this.switchStatement=switchStatement;
  }

}
