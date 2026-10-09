\begin{code}

module MuGameCheck (

  gamecheck,
  plotgamegraph

)         




where

import GlaExts
import Array
import List (nub, (\\), partition )
	
import MuFormula
import MutLTS
import LTSState
import LTSMonad
import LTSClosure
import LTSLabeling
import PAToLTS
import ProcAlg
import FiniteMap
import Set
import SST
import Pretty
import Outputable
import Hash
import Id
import Unique
import UniqSupply
       



type GameColoring = FiniteMap Int GameColor
data GameColor = White | Red | Green deriving Eq
{- Ein Label für Games besteht aus einem Bool, je nachdem, ob
   der Knoten bereits expandiert wurde oder nicht (wie bei allen
   Transitionssystemen), der Farbe des Knoten und einem weiteren
   Bool, welches für das Durchlaufen des Transsys. verwendet wird -}

data GameLabel = GameLabel Bool GameColoring Bool 

instance LTSLabeling GameLabel where
  isExpanded (GameLabel b _ _) = b
  markExpanded (GameLabel _ gc visited) = GameLabel True gc visited
  defLabeling = GameLabel False emptyFM False

data GFormula
  = GTrue
  | GFalse
  | GAnd Int Int
  | GOr Int Int
  | GBox Bool Bool [Action] Int
  | GDiam Bool Bool [Action] Int
  | GLFP Int Int
  | GGFP Int Int
  | GVar Int

type FTable = Array Int GFormula

freeVariables::GFormula->FTable->Set Int
freeVariables f ftab
  = fv f
  where fv GTrue = emptySet	      				     
        fv GFalse = emptySet
	fv (GAnd i1 i2) = fv (ftab!i1) `union` fv (ftab!i2)
	fv (GOr i1 i2) = fv (ftab!i1) `union` fv (ftab!i2)
	fv (GBox _ _ _ i) = fv (ftab!i)
	fv (GDiam _ _ _ i) = fv (ftab!i)
	fv (GLFP i my_i) = fv (ftab!i) `minusSet` unitSet my_i
	fv (GGFP i my_i) = fv (ftab!i) `minusSet` unitSet my_i
	fv (GVar i) = unitSet i

-- hmm, sieht so aus, als ob hasFiniteGame True zurückliefert, genau dann,
-- wenn keine Fixpunkte/Variablen in der Formel vorkommen. Warum wird bei
-- LFP und GFP bereits False zurückgeliefert??

hasFiniteGame::MuFormula->Bool
hasFiniteGame f
  = hfg f
  where hfg MuTrue = True
	hfg MuFalse = True
	hfg (MuAtom _) = False
	hfg (MuBox _ _ _ f1) = hfg f1
	hfg (MuDiam _ _ _ f1) = hfg f1
	hfg (MuAnd f1 f2) = hfg f1 && hfg f2
	hfg (MuOr f1 f2) = hfg f1 && hfg f2
	hfg (MuLFP _ _) = False
	hfg (MuGFP _ _) = False
	hfg (MuNeg _) = error "hasFiniteGame called with negation in formula"
	hfg (MuApp _ _) = error "hasFiniteGame called with non-expanded argument"

flattenMuAnd::MuFormula->[MuFormula]
flattenMuAnd (MuAnd f1 f2) = (flattenMuAnd f1)++(flattenMuAnd f2)
flattenMuAnd f = [f]

flattenMuOr::MuFormula->[MuFormula]
flattenMuOr (MuOr f1 f2) = (flattenMuOr f1)++(flattenMuOr f2)
flattenMuOr f = [f]	    		     	     	


-- Die Ordnung ist komisch definiert da eigentlich partiell und nicht als Liste
-- darstellbar
	
extractVarOrder::MuFormula->[Id]
extractVarOrder f
  = evo f
  where evo MuTrue = []
	evo MuFalse = []
	evo (MuAtom _) = []
	evo (MuDiam _ _ _ f1) = evo f1
	evo (MuBox _ _ _ f1) = evo f1
	evo (MuAnd f1 f2) = (evo f1)++(evo f2)
	evo (MuOr f1 f2) = (evo f1)++(evo f2)
	evo (MuLFP id f1) = id:(evo f1)
	evo (MuGFP id f1) = id:(evo f1)

-- buildGameOrder ordnet die übergebenen Formeln
-- derart um, dass zunächst die vorne stehen, bei denen
-- ein endliches Spiel bevorsteht, dann die mit noFrreVars
-- aber warum??
buildGameOrder::[Id]->[MuFormula]->[MuFormula]
buildGameOrder varorder fs
  = let
      (f1s, f2s) = partition hasFiniteGame fs
      (f21s, f22s) = partition noFreeVars f2s
      f22ss = orderByFreeVars varorder f22s
    in f1s++f21s++f22ss
  where orderByFreeVars varorder [] = []
	orderByFreeVars varorder [f] = [f]
	orderByFreeVars (var:vars) fs
	  = let (f1s,f2s) = partition (hasFreeVar var) fs
	    in
	    f1s++(orderByFreeVars vars f2s)
	hasFreeVar var f
	  = var `elementOf` (freeVars f)
	noFreeVars f
	  = case setToList (freeVars f) of
		 [] -> True
		 _ -> False
		
colorByFreeVar::GFormula->FTable->GameColor
colorByFreeVar f ftab
  = case setToList (freeVariables f ftab) of
      [] -> error "no free variables"
      [i] -> case (ftab!i) of
	       GLFP _ _ -> Red
	       GGFP _ _ -> Green
	       _ -> error "Fixpoint exspected"
      _ -> error "more then one free variable"	       
      
buildFTable::MuFormula->(Int,FTable)
buildFTable f
  = let

      (nf,fenv,entries,max_n) = bft f [(MuTrue,0),(MuFalse,1)] [] 2
    in
    (nf,array (0,max_n-1) ((0,GTrue):(1,GFalse):entries))
  where varorder = extractVarOrder f
	bft f env venv n
          = case lookup f env of
	         Just i -> (i,env,[],n)
		 Nothing -> 
		   case f of
		     MuNeg _ -> error "buildFTable called with negation not removed"
		     MuAtom id ->
		       case lookup id venv of
		         Nothing -> error "free variable in formula"
			 Just n1 -> (n,(f,n):env,[(n,GVar n1)], n+1)
		     MuAnd _ _ ->
		       let fs = flattenMuAnd f
		           orderedfs = buildGameOrder varorder fs
			   new_f = foldl1 MuAnd orderedfs
		       in case new_f of
			    MuAnd f1 f2 ->
			      let 
			        (nf1,env1,entries1,n1) = bft f1 ((new_f,n):env) venv (n+1)
				(nf2,env2,entries2,n2) = bft f2 env1 venv n1
			      in
			      (n,env2,(n,GAnd nf1 nf2):(entries1++entries2), n2)
		     MuOr _ _ ->
		       let fs = flattenMuOr f
		           orderedfs = buildGameOrder varorder fs
			   new_f = foldl1 MuOr orderedfs
		       in case new_f of
			    MuOr f1 f2 ->
			      let 
			        (nf1,env1,entries1,n1) = bft f1 ((new_f,n):env) venv (n+1)
				(nf2,env2,entries2,n2) = bft f2 env1 venv n1
			      in
			      (n,env2,(n,GOr nf1 nf2):(entries1++entries2), n2)
		     MuBox cl pom acts f1 -> 
                       let 
			 (nf1,env1,entries1,n1) = bft f1 ((f,n):env) venv (n+1) 
		       in
		       (n,env1,(n,GBox cl pom acts nf1):entries1, n1)
		     MuDiam cl pom acts f1 ->
                       let 
			 (nf1,env1,entries1,n1) = bft f1 ((f,n):env) venv (n+1) 
		       in
		       (n,env1,(n,GDiam cl pom acts nf1):entries1, n1)
		     MuGFP id f1 ->
 		       let
			 (nf1, env1, entries1, n1) = bft f1 ((f,n):env) ((id,n):venv) (n+1)
		       in
		       (n,env1,(n,GGFP nf1 n):entries1, n1)
		     MuLFP id f1 ->
 		       let
			 (nf1, env1, entries1, n1) = bft f1 ((f,n):env) ((id,n):venv) (n+1)
		       in
		       (n,env1,(n,GLFP nf1 n):entries1, n1)
		     MuApp _ _ -> error "buildFTable called with non-expanded formula"


gamecheck::(Int,Int,Int)->ProcEnv->Process->MuFormula->Bool
gamecheck tablesizes penv proc f
  = runLTS tablesizes (defLabeling::GameLabel)
           (
             getFreshStateLTS `thenLTS` \ st ->
	     setLabelLTS st proc `seqLTS`
	     addToStateHTblLTS proc st `seqLTS`
	     color penv st ftable fi  `thenLTS` \ c ->
	     case c of
		  White -> error "white"
		  Green -> returnLTS True
		  Red -> returnLTS False
	   )
  where (fi,ftable) = buildFTable f -- turning the formula into an int and the tables
	   

plotgamegraph::(Int,Int,Int)->ProcEnv->Process->MuFormula->Pretty
plotgamegraph tablesizes penv proc f
  = runLTS tablesizes (defLabeling::GameLabel)
	  (
	     getFreshStateLTS `thenLTS` \ st ->
	     setLabelLTS st proc `seqLTS`
	     addToStateHTblLTS proc st `seqLTS`
	     color penv st ftable fi `seqLTS`
	     plot st fi ftable
          )
  where 
    (fi, ftable) = buildFTable f            
    -- plot:: LTSState -> Int -> FTable -> LTS_M  s l  Pretty 
    plot st fi ftable = 
              getAddLabelLTS st `thenLTS` \ (GameLabel b coloring visited) -> 
              if visited
	      then returnLTS ppNil
	      else 
                let  pp_id = ppBesides [ ppStr "\"", 
                                         ppr PprUser st, 
                                         ppChar '#', 
	  				 ppr PprUser fi, ppStr "\"" 
		                       ] 
                     pp_name = ppStr "x" -- not used
                     pp_color Red   = ppStr "red" 
                     pp_color Green = ppStr "green" 
                     pp_color White = ppStr "white" 
                     pp_edges = ppStr "a" 

                      -- expandLTSState penv st `seqLTS` -- Zustand mueste expandiert sein
                 in
                   getLabelListLTS st `thenLTS` \ outgoing_acts -> 
                   let relevant_acts = {-act_select-} outgoing_acts in
                     mapConcatLTS (getSuccsLTS st) relevant_acts `thenLTS` \ relevant_succs ->
		     setAddLabelLTS st (GameLabel b coloring True) `seqLTS` -- mark node as visited
                     let erg = case lookupFM coloring fi of
           			Nothing -> ppBesides [ pp_id, 
                                                       ppStr " [label=\"", 
                        			       pp_id, ppStr " (no color)\"" 
				                     ]
			        Just c -> ppBesides [ pp_id, 
                                                      ppStr " [label=\"", 
           					      pp_id, ppStr "\" ", 
                                                      (pp_color c) ] 
                     accumulate 
                     

                     
 
{-	                            ppStr "n(\"\",[",
				    ppStr "a(\"OBJECT\",\"", pp_name, 
                                    ppStr "\"),a(\"COLOR\",\"",
				    pp_color c, ppStr "\")]",
				    pp_edges,
				    ppStr "))"
				    ])
-}
{- for daVinci	   let pp_id = ppBesides [ ppStr "\"", 
                                           ppr PprUser st, 
                                           ppChar '#',
					   ppr PprUser fi, ppStr "\""
					 ] 
                       pp_name = ppStr "x"
                       pp_color = ppStr "red"
                       pp_edges = ppStr "a"

                   in 
	           case lookupFM coloring fi of
			Nothing -> returnLTS (ppBesides [
				     ppStr "l(", pp_id,
				     ppStr "n(\"\",[a(\"OBJECT\",\"not visited\")],[]))"
				     ])
			Just c -> returnLTS (ppBesides [ ppStr "l(\"", ppr PprUser st, 
                                    ppChar '#', ppInt fi, ppStr "\"",
	                            ppStr "n(\"\",[",
				    ppStr "a(\"OBJECT\",\"",pp_name, 
                                    ppStr "\"),a(\"COLOR\",\"",
				    pp_color, ppStr "\")]",
				    pp_edges,
				    ppStr "))"
				    ])
-}

	   
	   
color::ProcEnv->LTSState->FTable->Int->LTS_M s GameLabel GameColor
color penv st ftable fi
  = col st fi
  where	col::LTSState->Int->LTS_M s GameLabel GameColor
        col st i -- b sag
	  = getAddLabelLTS st `thenLTS` \ (GameLabel b coloring visited) ->
	    case lookupFM coloring i of
	         Just White -> let mycolor = colorByFreeVar (ftable!i) ftable in
			       setAddLabelLTS st (GameLabel b (addToFM coloring i mycolor) visited)
			       `seqLTS` returnLTS mycolor
	         Just c -> returnLTS c
		 Nothing ->
		   setAddLabelLTS st (GameLabel b (addToFM coloring i White) visited) `seqLTS`
		   case (ftable!i) of
			GTrue -> returnLTS Green
			GFalse -> returnLTS Red
			GAnd i1 i2 -> col st i1 `thenLTS` \ c1 ->
				      case c1 of
				        White -> error "WM and"
					Red -> returnLTS Red
					Green -> col st i2
			GOr i1 i2 -> col st i1 `thenLTS` \ c1 ->
				     case c1 of
				       White -> error "WM or"
				       Green -> returnLTS Green
				       Red -> col st i2
			GBox False False acts i -> colorMod doBox st (\_ -> acts) i
			GBox False True acts i -> colorMod doBox st
						    (\outacts -> (nub outacts) \\ (nub acts))
						    i
			GDiam False False acts i -> colorMod doDiam st (\_ -> acts) i
			GDiam False True acts i -> colorMod doDiam st
						     (\outacts -> (nub outacts) \\ (nub acts))
						     i
			GLFP i1 _ -> col st i1
			GGFP i1 _ -> col st i1
			GVar i1 -> case lookupFM coloring i1 of
				   Nothing -> col st i1
				   Just White -> case (ftable!i1) of
					           GLFP _ _ -> returnLTS Red
						   GGFP _ _ -> returnLTS Green
						   _ -> error "another unexspected case"
				   Just c -> returnLTS c
			_ -> error "unexspected case"
		   `thenLTS` \ mycolor ->
		   getAddLabelLTS st `thenLTS` \ (GameLabel b coloring visited) ->
		   setAddLabelLTS st (GameLabel b (addToFM coloring i mycolor) visited) `seqLTS`
		   returnLTS mycolor
	colorMod doColoring st act_select i
	  = expandLTSState penv st `seqLTS`
	    getLabelListLTS st `thenLTS` \ outgoing_acts ->
	    let relevant_acts = act_select outgoing_acts in
	    mapConcatLTS (getSuccsLTS st) relevant_acts `thenLTS` \ relevant_succs ->
	    let
	      next_colorings = [ col s' i | s' <- relevant_succs ]
	    in
	    doColoring next_colorings
	doDiam [] = returnLTS Red
	doDiam (coloring:colorings) = coloring `thenLTS` \ c ->
				      case c of
					White -> error "WM diam"
					Green -> returnLTS Green
					Red -> doDiam colorings
	doBox [] = returnLTS Green
	doBox (coloring:colorings) = coloring `thenLTS` \ c ->
				     case c of
				       White -> error "WM box"
				       Green -> doBox colorings
				       Red -> returnLTS Red
\end{code}q

				 


