package rem.compiler;

import java.util.*;

public class Strategy {
    /*
     * ELAN Strategy: name + sort + strategyTerm
     */
  private Lexem leftside;  /** name of the strategy */
  private Lexem sort;
  private StrategyTerm rightside; // definition

  // table : nom de strategy (term) <--> strategy
  //  ATTENTION : 2 TERMES IDENTIQUES NE SONT PAS LA MEME CLE
  private static Map map = new TreeMap();

  public Strategy(Lexem lhs, Lexem sort,  StrategyTerm rhs) {
    leftside = lhs;
    this.sort = sort;
    rightside = rhs;
    rightside.setTopStrategy(this);
  }

  public Strategy(Lexem lhs, StrategyTerm rhs) {
    leftside = lhs;
    rightside = rhs;
    rightside.setTopStrategy(this);
  }

  public static Iterator valuesIterator() {
    return map.values().iterator();
  }

  /**
   * Ajoute une strategy au systeme
   */
  public static void addElement(Strategy strat) {
    map.put(strat.leftside,strat);
  }

  public static Strategy getStrategy(Lexem key) {
    return (Strategy)map.get(key);
  }

  public DetType getDetType() {
    return rightside.getDetType();
  }

  public String toString() {
    StringBuffer s = new StringBuffer();
    s.append( "[" + leftside + "] " + rightside );
    return s.toString();
  }


  public static void dump() {
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Strategy strat = (Strategy)it.next();
      System.out.println( strat );
    }
  }

  public int getCodeName() {
    return leftside.getCode(); 
  }

  public String getName() {
    return "_" + getCodeName();
  }

  public boolean isEval() {				// Peter
    if (Flags.strat > 0) 
      return (-1 != (leftside.toString()).indexOf("eval:"));
    else
      return false;
  }

  public void compile(OutputCode s) {
    OutputCode matchSubtermCode = new OutputCode();
    OutputCode strategyCode = new OutputCode();
    int deep=0;
    boolean is_eval = isEval();				// Peter

    // Entete de la fonction
    strategyCode.write("\nstruct term* str" + getName() + "( struct term *arg0 ) {\n");
    strategyCode.write("  struct term *v0=arg0;\n");

    if (is_eval) {                                    // Peter
      strategyCode.write("#ifdef BORO\n return str_eval(v0);\n#else\n");
    }

    Tools.genStrategyProlog(strategyCode,deep, getName());

    //StrategyTerm.strategyLabel=0;
    rightside.compile(strategyCode,deep+1,matchSubtermCode);

    Tools.genStrategyEpilog(strategyCode,deep, getName());

    if (is_eval) {                                    // Peter
      strategyCode.write("#endif\n"); }

    strategyCode.write("}\n");

    s.write(matchSubtermCode.stringDump());
    s.write(strategyCode.stringDump());
  }

  public static String genDeclar(int deep) {
    StringBuffer s = new StringBuffer();
    s.append("\n/* Entetes */\n");
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Strategy strat = (Strategy)it.next();
      Tools.indent(s,deep); 
      s.append("extern struct term *str" + strat.getName() + 
	       "(struct term *t);\n");
    }
    return s.toString();
  }

  private static int getMaxCode() {
      int max=0;
    Iterator it = map.values().iterator();
    while(it.hasNext()) {
      Strategy strat = (Strategy)it.next();
      if(strat.getCodeName() > max)
	max=strat.getCodeName();
    }
    return max;
  }

  public static void genRedirectStrategy(OutputCode s,int deep) {
      int maxCode =   getMaxCode();
      boolean tmpTab[] = new boolean[2*maxCode+2];
      int normTab[] = new int[maxCode+1];
    
      Iterator it = map.values().iterator();
      while(it.hasNext()) {
	  Strategy strat = (Strategy)it.next();
	  tmpTab[strat.getCodeName()]= true;
	  if(strat.rightside.isNormIn()) { // [Huy: Oct 17 00] 
	      normTab[strat.getCodeName()]= 1;
	  }else if(strat.rightside.isNormOut()) {
	      normTab[strat.getCodeName()]= 2;
	  } else {
	      normTab[strat.getCodeName()]= 0;
	  }
      }
      s.write(deep,"\n/* tableau de pointeurs de fonctions qui retournent */\n");
      s.write(deep,"/* un pointeur sur un term */\n");
      s.write("funTabType strTab[] = {\n");
      
      int lineLength=0;
      int maxLineLength=70;
      String redirection="";
      for(int i=0 ; i <= maxCode ; i++) { // [Huy: Oct 17 00] 
	  if(tmpTab[i]) {
	      if(normTab[i]==0) {
		  redirection="(funTabType) &str_" + i;
	      } else if(normTab[i]==1) {
		  redirection="(funTabType) &norm_lazy";
	      } else if(normTab[i]==2){
		  redirection="(funTabType) &norm_out";
	      }
	  } else {
	      redirection="NULL";
	  }
	  lineLength+=2+redirection.length();
	  if(lineLength>maxLineLength) {
	      s.write("\n");
	      lineLength=2+redirection.length();
	  }
	  s.write(redirection+", ");
      }
      for (int i = maxCode+1; i <= tmpTab.length-1; i++) { // [Huy: Oct 17 00] 
	  if(tmpTab[i-maxCode-1] && (normTab[i-maxCode-1] != 0)) {
	      redirection = "(funTabType) &str_" + (i-maxCode-1);
	  }else{
	      redirection = "NULL";
	  }
	  
	  lineLength+=2+redirection.length();
	  if(lineLength>maxLineLength) {
	      s.write("\n");
	      lineLength=2+redirection.length();
	  }
	  s.write(redirection+", ");
      }
      s.write("NULL};\n");
      s.write("int strTabSize = " + (tmpTab.length+1) + ";\n");
  }
    
}

