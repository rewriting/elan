module Terms[K,Q,S]

import
 global
  K Q S
  int
  identifier
  list[identifier]
  list[invariant]
  list[transition]
  DBM[K] ;
end

sort
  invariant     //  spécification d'un invariant
  transition    //  spécification d'une transition
  label         //  les étiquettes des états
  state         //  les états discrets
  statezone ;   //  les zones d'etats temporels
end

operators
 global
  
    FOR EACH a : identifier SUCH THAT a := (listExtract) elem(S) :
  { a  :  label; }

    FOR EACH a : identifier SUCH THAT a := (listExtract) elem(Q) :
  { a  :  state; }

  @ ':' @       : (state clockzone) invariant ;
  @ , @ ':' @ , @ , @ : (state label clockzone list[clock] state) transition ;


end

end
