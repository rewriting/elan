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

public class Pair {
  private Object x;
  private Object y;

  public Pair(Object x, Object y) {
    this.x = x;
    this.y = y;
  }

  public Object getX() {
    return x;
  }
  public Object getY() {
    return y;
  }
  public void setX(Object o) {
    x=o;
  }
  public void setY(Object o) {
    y=o;
  }

  public int hashCode() {
    return getX().hashCode() + getY().hashCode();
  }
  
  public boolean equals(Object o) {
    if(o instanceof Pair) {
      Pair p = (Pair)o;
/*
      System.out.println(getX()+"=="+p.getX() + " --> " +
                         getX().equals(p.getX()));
      System.out.println(getY()+"=="+p.getY() + " --> " +
                         getY().equals(p.getY()));
*/
      return getX().equals(p.getX()) && getY().equals(p.getY());
    } else {
      return false;
    }
  }
  public String toString() {
    return "["+x+","+y+"]";
  }
}
