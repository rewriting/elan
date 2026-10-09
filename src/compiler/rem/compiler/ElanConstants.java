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

  public class ElanConstants {

    // --- Constants

    public static final int DS_FSYM = 0x1000000;
    public static final int DS_LAB  = 0x2000000;
    public static final int DS_DSTR = 0x4000000;
    public static final int DS_APPL = 0x8000000;

    public static final int DS_APPLY  = 180;
    public static final int DS_DC     = 181;
    public static final int DS_DK     = 182;
    public static final int DS_FAIL   = 183;
    public static final int DS_IFTE   = 184;
    public static final int DS_COMMA  = 185;
    public static final int DS_ID     = 186;
    public static final int DS_IFTOE  = 187;
    public static final int DS_CONC   = 188;
    public static final int DS_EPSILON= 189;
    public static final int DS_ONE    = 190;

    // --- Macros

  public static boolean isFsym(int x) {
    if ((x & DS_FSYM) != 0) return true;   
    else return false;
  }

  public static int fsymF1(int x) {
    return ((x & 0xfff000) >> 12);
  }

  public static int fsymF2(int x) {
    return (x & 0xfff);
  }

  public static boolean isLab(int x) {
    if ((x & DS_LAB) != 0) return true;   
    else return false;
  }

  public static int applySymbolLab(int x) {
    return ((x & 0xfff000) >> 12);  // 0106
  }

  public static int indexLab(int x) {
    return (x & 0xfff);
   //0106    return (x - DS_LAB);           
  }

  public static boolean isDstr(int x) {
    if ((x & DS_DSTR) != 0) return true;   
    else return false;
  }

  public static int applySymbolDstr(int x) {
    return ((x & 0xfff000) >> 12);
  }

  public static int indexDstr(int x) {
    return (x & 0xfff);
  }

  public static boolean isApply(int x) {
    if ((x & DS_APPL) != 0) return true;   
    else return false;
  }

  public static boolean isPrimalSymbol(int x) {
     System.out.println("isPrimal x = " + x);

     return     
	( x == DS_DC || x == DS_DK || x == DS_FAIL || x == DS_ID ||
	    x == DS_IFTE || x == DS_IFTOE || x == DS_CONC );
  }

}
