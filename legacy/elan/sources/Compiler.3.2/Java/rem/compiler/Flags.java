package rem.compiler;

import java.util.*;

  public class Flags {

    public static boolean verbose = false;  // blabla pendant la compilation
    public static boolean quiet = false;  // Pour enlever tous les msg
    public static boolean debug = false;     // on for debugging
    public static boolean choicePointDebug = false;  // on for debugging
    public static int strat = 0;             // strategy interpreter
	// 0 - strategies are compiled by PEMC 
	// !!! BUT exported with elanlib.townsville
	// 1 - eval strategy in c++
	// 2 - application of streategies are compiled
	// 3 - strategies are compiled

    public static boolean optimiseChoicePoint = false;
    //public static boolean optimiseChoicePoint = true;

    /** pour conserver le code C genere */
    public static boolean code = true; 
    /** pour couper en morceaux le code genere */
    public static boolean split= false; 

    /** pour activer le coloriage des termes AC */
    public static boolean color = false; 

    /** pour utiliser le nouvel algo de numerotation */
    public static boolean newVariableAffectation = true;

    /** pour ajouter un branchement directe entre le filtrage et le rhs */
    public static boolean withGoto = false; 

      /** pour utiliser setChoicePoint ecrit en C */
    public static boolean onlyC = true; 

      /** pour utiliser localSetChoicePoint ecrit en C */
    public static boolean localSetChoicePoint = true; 

      /** pour compter le nombre de regles appliquees */
    public static boolean rwrCounter = true; 

      /** pour afficher des warnings pendant la compilation */
    public static boolean warnings = false; 

      /** pour generer le server COQ */
    public static boolean coq = false; 

      /** pour generer une library */
    public static boolean lib = false; 

  }
