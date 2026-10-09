import java.util.*;
import java.io.*;

final public class OutputCode {
  
  //private BufferedWriter file;
  //private StringWriter string;
  private Writer file;

  public OutputCode(BufferedWriter file) {
    this.file = file;
  }

  public OutputCode() {
    this.file = new StringWriter();
  }

  public void write(String s) {
    try {
      file.write(s);
    } catch (IOException e) {
      System.out.println("write error");
      e.printStackTrace();
    }
  }

  public void write(int deep,String s) {
    indent(deep);
    write(s);
  }

  public void close() {
    try {
      file.flush();
      file.close();
    } catch (IOException e) {
      System.out.println("close error");
      e.printStackTrace();
    }
  }

  public String stringDump() {
    try {
      if(file instanceof StringWriter) {
	file.flush();
	return file.toString();
      } else {
	throw new InternalError("OutputCode does not contain any string");
      }
    } catch (IOException e) {
      System.out.println("stringDump error");
      e.printStackTrace();
      return null;
    }
  }

  private static char[] tab = new char[] {32,32};
  public void indent(int deep) {
    try {
      for(int i=0 ; i<deep ; i++) {
        file.write(tab);
      }
    } catch (IOException e) {
      System.out.println("write error");
      e.printStackTrace();
    }
  }

}




