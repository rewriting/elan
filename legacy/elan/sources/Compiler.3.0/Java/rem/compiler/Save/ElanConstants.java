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
