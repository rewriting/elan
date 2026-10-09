class tstExec {
  public static void main(String args[]){
  Runtime r = Runtime.getRuntime();
  Process Exec = null;
  String exec_param[] = { "sh" , "-c" , "ls >> ls-output" } ;
  try {
    Exec = r.exec(exec_param);
  } catch (Exception e){
      System.out.println("Error in command execution!");
    }
  }
}
