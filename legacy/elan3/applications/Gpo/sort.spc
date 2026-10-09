specification sort
Vars  x y z l

Ops
	leq:2 dif:2 insert:2 cons:2 isort:1 s:1 null:0 o:0 true:0 false:0
Rules

leq(o,x) -> true
leq(s(x),o) -> false
leq(s(x), s(y)) -> leq(x,y)
dif(o,o) -> false
dif(o,s(x)) -> true
dif(s(x),o) -> true
dif(s(x),s(y)) -> dif(x,y)
insert(x,null) -> cons(x,null)
/*little bidouille */
insert(x,cons(y,z)) -> cons(x,cons(y,z))
insert(x,cons(y,z)) -> leq(x, y)
insert(x,cons(y,z)) -> true

insert(x,cons(y,z)) -> cons(y,insert(x,z))
insert(x,cons(y,z)) -> leq(x, y)
insert(x,cons(y,z)) -> false
isort(null) -> null
isort(cons(x,z)) -> insert(x,isort(z))
nil
end of specification

/*
[ executionAbort Continue Dump Exit Statistics
  changeTrace changeQuiet Input Output stRategy (ACDESTQIOR|acdestqior)] ?s
[] statistics:
 total time     (59692.446+0.000)=59692.446 sec (main+subprocesses)

            117843820 nonamed rules applied, 340245356 tried
            3376905   named rules applied, 340925590 tried

                rule: applied   tried                 rule: applied   tried
        anti_equality   24127 2116119            topOccRule       0       0
                 init       1       1     lpo_instantiation  741592109471746
         extractrule1       0       0          extractrule2       0       0
         decompose_eq    5078102952780              equality       0 1070123
       decompose_ineq   13278103725414          anti_subterm  145822 1559155
            inOccRule       0       0                 final       0  572098
   prec_simplify_init  143024  143024             prec_next 1195413 1765559
              subterm  343209 5410684         prec_simplify  76536112138887


*/