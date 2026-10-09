/*
  
    REM - Reduce ELAN Machine

    Copyright (C) 2000-2001  LORIA (CNRS, INPL, INRIA, UHP, U-Nancy 2)
			     Nancy, France.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    Pierre-Etienne Moreau	e-mail: Pierre-Etienne.Moreau@loria.fr

*/
package rem.compiler;

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




