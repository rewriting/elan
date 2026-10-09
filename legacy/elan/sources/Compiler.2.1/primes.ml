type l = nil
       | cons of int*l;;

let rec app = function x -> function z ->
	match x with
	  nil -> z
	| cons(x,y) -> cons(x,(app y z));;

let rec addifprime = function n -> function
	  nil -> cons(n,nil)
	| cons(i,y) -> if (i*i)>n then
			 cons(i,(app y (cons(n,nil))))
		       else
			 if (n mod i)=0 then
			   cons(i,y)
		         else
			   cons(i,(addifprime n y))
	;;


let rec primes = function 
	  0 -> nil
	| 1 -> nil
	| 2 -> cons(2,nil)
	| n -> addifprime n (primes (n-1))
	;;

let rec affiche = function
	  nil -> ()
	| cons(x,y) -> print_int x; print_string " "; affiche y;;

primes 10000;;

(* affiche (primes 10000);; print_newline();; *)

(*
affiche (primes 10);;
affiche (primes 100);;
affiche (primes 10000);;
*)
