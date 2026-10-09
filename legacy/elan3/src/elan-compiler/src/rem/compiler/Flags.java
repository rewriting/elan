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

      /**  pour transformer les regles nommees */
    public static boolean proofterm = false; 

      /**  pour compiler sans optimisation */
    public static boolean fast = false; 

      /** pour generer une library */
    public static boolean lib = false; 

      /** pour generer avec les ATerms */
    public static boolean aterm = false; 
    public static boolean atermns = false; 
  }
