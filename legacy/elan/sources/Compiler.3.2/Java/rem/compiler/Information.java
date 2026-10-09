package rem.compiler;

import java.util.*;

class Information {
    /**
     * numero de la regle dans l'arbre de filtrage
     */
  private int number;
  private DDTree myTree;

    /**
     * liste de motifs et d'arbres associes a une feuille
     * les arbres sont des ACDDTree
     * les motifs sont des sous-termes commencant par un symbole AC
     */
  private Vector tree = new Vector();
  private Vector pattern = new Vector();

  public Information(DDTree tree,int number) {
    this.myTree = tree;
    this.number = number;
  }
  
  public void addInfo(DDTree tree, Term term) {
    this.tree.addElement(tree);
    this.pattern.addElement(term);
//    System.out.println("addInfo : " + this);
  }

  public void merge(Information info) {
    if(pattern.size() < info.pattern.size()) {
      pattern=info.pattern;
      tree=info.tree;
    }
  }
  
  public int getNumber() {
    return number;
  }

  public int getCBGNumber() {
    if(pattern.size()>0) {
      ACDDTree tmpTree = (ACDDTree)tree.elementAt(0);
      Term ACterm = (Term)pattern.elementAt(0);
      return tmpTree.getCBGNumber(ACterm);
    }
    return -1;
  }
    
  public String toString() {
    StringBuffer s = new StringBuffer();
      // numero du motif dans l'arbre de filtrage
    s.append("[" + number + "] ");
      // Il n'y a qu'un seul sous-motif AC !
    for(int i=0 ; i<pattern.size() ; i++) {
      ACDDTree tmpTree = (ACDDTree)tree.elementAt(i);
      Term ACterm = (Term)pattern.elementAt(i);
        // System.out.println("ACterm = " + ACterm);
      
      s.append("{ ");
        // numero du motif AC dans le CBG (en comptant les dioPatterns)
      s.append( tmpTree.getCBGNumber(ACterm) );
      s.append(" |-> ");
      boolean condition = false;
        // pour chaque sous-terme du terme AC         
      for(int j=0 ; j<ACterm.arity() ; j++) {
          // si ce n'est pas une variable
	if( !ACterm.getSubterm(j).isVariable() ) {
            // numero des sous-motifs
	  s.append( tmpTree.getNumber(ACterm.getSubterm(j)) + " ");
	  condition = true;
	}
      }
      if(condition == false) {
	s.append("no condition ");
      }
      s.append("} ");
    }
    return s.toString();
  }

  public void compile(OutputCode s, int deep, String prefix,
		      boolean isSyntacticCase,
		      boolean incrementNbBit, int fatherVarNumber) {
    String name = prefix;

    if( pattern.size() == 0 ) {
      // Cas standard
      if(incrementNbBit) {
	s.write(deep,"mask[nb_bit++]=" + number + ";\n");
      } else {
	if(!myTree.isFunctionTree() || myTree.getBitSetSize()>32) {
	  s.write(deep,"bitSet_set(mask," + number + ");\n");
	} else {
	  s.write(deep,"bitSet32_set(mask32," + number + ");\n");
	}
      }
    } else {
      // Cas AC
      if(isSyntacticCase) {
	if(incrementNbBit) {
	  s.write(deep,"mask[nb_bit++]=" + number + ";\n");
	} else {
	if(!myTree.isFunctionTree() || myTree.getBitSetSize()>32) {
	    s.write(deep,"bitSet_set(mask," + number + ");\n");
	  } else {
	    s.write(deep,"bitSet32_set(mask32," + number + ");\n");
	  }
	}
      } else {
	s.write(deep,"/* AC case: Not tested */\n");
	int base_id_pattern=number; //2;
	if(myTree.getACnet() == null) {
	  if(incrementNbBit) {
	    s.write(deep,"mask[nb_bit++]=" + number + ";\n");
	  } else {
	    s.write(deep,"bitSet_set(mask," + number + ");\n");
	  }
	} else {
	  /*
	   * il faut trouver le no de la variable qui stocke le terme AC
	   */
	  String varName = "v" + fatherVarNumber;
	  //System.out.println("--> " + varName);
	  s.write(deep,"nb_bit += match_subterm_AC(" + base_id_pattern + ", no_arg_subject, mask, cbg, match_subterm" + name + ", no_pattern" + name + "_niv_0, pattern_list" + name + ", nb_pattern" + name + "_niv_1," + varName + ", max_nb_pattern_under" + name + ");\n");
	}
      }
    }
    //System.out.println( this );
    //System.out.println( s );
  }

  public void genPatternListConstruction(OutputCode s,int deep,
                                        String name,
                                        int cbgNumber) {
      //System.out.println("*** genPatternListConstruction");
      // Il n'y a qu'un seul sous-motif AC !
    for(int i=0 ; i<pattern.size() ; i++) {
      ACDDTree tmpTree = (ACDDTree)tree.elementAt(i);
      Term ACterm = (Term)pattern.elementAt(i);
      int compteur=0;
      for(int j=0 ; j<ACterm.arity() ; j++) {
        if( !ACterm.getSubterm(j).isVariable() ) {
          int patternNumber = tmpTree.getNumber(ACterm.getSubterm(j));
//          System.out.print("patternNumber = " + patternNumber + " ");
          for(int k=0 ; k<ACterm.getSubterm(j).getMultiplicity() ; k++) {
            s.write(deep,"pattern_tab[" + compteur + "]=" + patternNumber + ";\n");
            compteur++;
          }
        }
      }
//      System.out.println();
      
      s.write(deep,"MS_pattern_list_init(pattern_list" + name + ",no_pattern" + name + "_niv_0++," + compteur + ",pattern_tab);\n");
    }
  }
  
  static public void genPatternListConstruction(OutputCode s,int deep,String name) {
    s.write(deep,"no_pattern" + name + "_niv_0++;\n");
  }
  
  public int maxNbPatternUnderAC() {
    int max=0;
    for(int i=0 ; i<pattern.size() ; i++) {
      ACDDTree tmpTree = (ACDDTree)tree.elementAt(i);
      Term ACterm = (Term)pattern.elementAt(i);
      int compteur=0;
      for(int j=0 ; j<ACterm.arity() ; j++) {
	if( !ACterm.getSubterm(j).isVariable() ) {
	  compteur+=ACterm.getSubterm(j).getMultiplicity();
	}
      }
      if(compteur>max)
	max=compteur;
    }
    return max;
  }

}
