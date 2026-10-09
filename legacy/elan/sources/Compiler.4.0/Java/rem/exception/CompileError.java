package rem.exception;

public class CompileError extends Exception {
  protected String msg="no message";
  
  public CompileError() {
  }

  public CompileError(String msg) {
    this.msg = msg;
  }

  public String toString() {
    return msg;
  }
 }


