/* JJT: 0.2.2 */




public class ASTLiteral extends SimpleNode {
  private String val;

  ASTLiteral(String id) {
    super(id);
  }

  public static Node jjtCreate(String id) {
    return new ASTLiteral(id);
  }

  public void setVal(String n) {
    val = n;
  }

  public String toString() {
    return "Literal: " + val ;
  }

}
