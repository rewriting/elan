type term = o | s of term;;

let rec plus a y = 
	match a with 
	  o -> y
	| s(x) -> plus x (s(y));;


let rec times a y = 
	match a with 
	  o -> o
	| s(x) -> plus y (times x y);;

let rec fac = function
	  o -> s(o)
	| s(x) -> times (s(x)) (fac x);;

let rec fib = function
	  o -> s(o)
	| s(o) -> s(o)
	| s(s(x)) -> plus (fib x) (fib (s(x)));;

fib( s(s(s(s(s(s(s(s(s(s(
     s(s(s(s(s(s(s(s(s(s(
     s(s(s(
	o
     ))))))))))))))))))))))));;
