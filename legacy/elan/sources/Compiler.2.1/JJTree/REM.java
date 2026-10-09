
class REM {

  public static void main(String args[]) {
    ATermsTree parser;
    if (args.length == 1) {
      System.out.println("Reduce ELAN Code Machine. Reading from file " + args[0] + " . . ."); 
      try {
        parser = new ATermsTree(new java.io.FileInputStream(args[0]));
      } catch (java.io.FileNotFoundException e) {
        System.out.println("Reduce ELAN Code Machine. File " + args[0] + " not found.");
        return;
      }
    } else {
      System.out.println("Reduce ELAN Code Machine. Usage :");
      System.out.println("         java REM inputfile");
      return;
    }
    try {
      parser.Start();
      /************************* Added by PEM. *******************/
      parser.jjtree.rootNode().dump("");
    } catch (ParseError e) {
      System.out.println("Reduce ELAN Code Machine. Encountered errors during parse.");
      e.printStackTrace();
    } catch (Exception e1) {
      System.out.println("Reduce ELAN Code Machine. Encountered errors during interpretation/tree building.");
      e1.printStackTrace();
    }
  }
}
