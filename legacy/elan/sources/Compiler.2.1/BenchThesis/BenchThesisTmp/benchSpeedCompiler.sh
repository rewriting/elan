#!/bin/tcsh

@ nbRepeat = 50

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis

if ($1 =~ "fib_builtin" || $1 =~ "all") then
echo "------------------------------------------------------------"

echo "fib_builtin"
elanc -quiet --batch -nosplit -optimiseChoicePoint  fib_builtin >/dev/null
gmake --quiet -f fib_builtin.make
echo "compiler"
echo "fib(21)"
echo "builtinInt fib(21) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(22)"
echo "builtinInt fib(22) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(23)"
echo "builtinInt fib(23) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(24)"
echo "builtinInt fib(24) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(25)"
echo "builtinInt fib(25) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(26)"
echo "builtinInt fib(26) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(27)"
echo "builtinInt fib(27) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(28)"
echo "builtinInt fib(28) end end" | query2ref fib_builtin.ref -b >! startC
oneRun.sh C $nbRepeat

gmake --quiet -f fib_builtin.make veryclean
endif

if ($1 =~ "nqueensAC" || $1 =~ "all") then
echo "------------------------------------------------------------"
echo "nqueensAC"
elanc -quiet --batch -nosplit -optimiseChoicePoint nqueensAC >/dev/null
gmake --quiet -f nqueensAC.make
echo "compiler"
echo "nqueens(7)"
echo "list[int] nqueens(7) end end" | query2ref nqueensAC.ref -b >! startC
oneRun.sh C $nbRepeat
echo "nqueens(8)"
echo "list[int] nqueens(8) end end" | query2ref nqueensAC.ref -b >! startC
oneRun.sh C $nbRepeat
echo "nqueens(9)"
echo "list[int] nqueens(9) end end" | query2ref nqueensAC.ref -b >! startC
oneRun.sh C $nbRepeat
echo "nqueens(10)"
echo "list[int] nqueens(10) end end" | query2ref nqueensAC.ref -b >! startC
oneRun.sh C 20
echo "nqueens(11)"
echo "list[int] nqueens(11) end end" | query2ref nqueensAC.ref -b >! startC
oneRun.sh C 20

gmake --quiet -f nqueensAC.make veryclean
endif

if ($1 =~ "bool3" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
echo "Bool3"
echo "compiler"
elanc -quiet --batch -nosplit -optimiseChoicePoint bool3  >/dev/null
gmake --quiet -f bool3.make
echo "q3"
echo "bool q3 end end" | query2ref bool3.ref -b >! startC
oneRun.sh C $nbRepeat
echo "q4"
echo "bool q4 end end" | query2ref bool3.ref -b >! startC
oneRun.sh C $nbRepeat
echo "q5"
echo "bool q5 end end" | query2ref bool3.ref -b >! startC
oneRun.sh C $nbRepeat
echo "q6"
echo "bool q6 end end" | query2ref bool3.ref -b >! startC
oneRun.sh C $nbRepeat
echo "q8"
echo "bool q8 end end" | query2ref bool3.ref -b >! startC
oneRun.sh C $nbRepeat

gmake --quiet -f bool3.make veryclean
endif

if ($1 =~ "ans" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis/FastCompletion
echo "compiler"
echo "FastCompletion"

elanc -quiet --batch -nosplit -optimiseChoicePoint ans p1  >/dev/null
gmake --quiet -f ans.make
echo "p1"
echo "list[equation] sat end end" | query2ref ans.ref -b >! startC
oneRun.sh C $nbRepeat
gmake --quiet -f ans.make veryclean

elanc -quiet --batch -nosplit -optimiseChoicePoint ans p2  >/dev/null
gmake --quiet -f ans.make
echo "p2"
echo "list[equation] sat end end" | query2ref ans.ref -b >! startC
oneRun.sh C $nbRepeat
gmake --quiet -f ans.make veryclean

elanc -quiet --batch -nosplit -optimiseChoicePoint ans p3  >/dev/null
gmake --quiet -f ans.make
echo "p3"
echo "list[equation] sat end end" | query2ref ans.ref -b >! startC
oneRun.sh C $nbRepeat
gmake --quiet -f ans.make veryclean

elanc -quiet --batch -nosplit -optimiseChoicePoint ans p4  >/dev/null
gmake --quiet -f ans.make
echo "p4"
echo "list[equation] sat end end" | query2ref ans.ref -b >! startC
oneRun.sh C $nbRepeat
gmake --quiet -f ans.make veryclean

elanc -quiet --batch -nosplit -optimiseChoicePoint ans p5  >/dev/null
gmake --quiet -f ans.make
echo "p5"
echo "list[equation] sat end end" | query2ref ans.ref -b >! startC
oneRun.sh C $nbRepeat
gmake --quiet -f ans.make veryclean

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
endif

if ($1 =~ "minela" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis/MinelaComp
echo "compiler"
echo "MinelaComp"
elanc -quiet --batch -nosplit -optimiseChoicePoint  rewriting primes  >/dev/null
gmake --quiet -f rewriting.make

echo "primes(2)"
echo "pair[term,proofterm] [primes(s(s(o))),proofnil] end end" | query2ref rewriting.ref -b >! startC
oneRun.sh C $nbRepeat
echo "primes(3)"
echo "pair[term,proofterm] [primes(s(s(s(o)))),proofnil] end end" | query2ref rewriting.ref -b >! startC
oneRun.sh C $nbRepeat
echo "primes(4)"
echo "pair[term,proofterm] [primes(s(s(s(s(o))))),proofnil] end end" | query2ref rewriting.ref -b >! startC
oneRun.sh C $nbRepeat
echo "primes(5)"
echo "pair[term,proofterm] [primes(s(s(s(s(s(o)))))),proofnil] end end" | query2ref rewriting.ref -b >! startC
oneRun.sh C $nbRepeat
echo "primes(6)"
echo "pair[term,proofterm] [primes(s(s(s(s(s(s(o))))))),proofnil] end end" | query2ref rewriting.ref -b >! startC
oneRun.sh C $nbRepeat
echo "primes(7)"
echo "pair[term,proofterm] [primes(s(s(s(s(s(s(s(o)))))))),proofnil] end end" | query2ref rewriting.ref -b >! startC
oneRun.sh C $nbRepeat
echo "primes(8)"
echo "pair[term,proofterm] [primes(s(s(s(s(s(s(s(s(o))))))))),proofnil] end end" | query2ref rewriting.ref -b >! startC

oneRun.sh C $nbRepeat

gmake --quiet -f rewriting.make veryclean
cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
endif

if ($1 =~ "set" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
echo "Set"
echo "compiler"
elanc -quiet --batch -nosplit -optimiseChoicePoint set  >/dev/null
gmake --quiet -f set.make
echo "powerSet(4)"
echo "int card(P(mkSet(1,2,3,4))) end end" | query2ref set.ref -b >! startC
oneRun.sh C $nbRepeat
echo "powerSet(5)"
echo "int card(P(mkSet(1,2,3,4,5))) end end" | query2ref set.ref -b >! startC
oneRun.sh C $nbRepeat
echo "powerSet(6)"
echo "int card(P(mkSet(1,2,3,4,5,6))) end end" | query2ref set.ref -b >! startC
oneRun.sh C $nbRepeat
echo "powerSet(7)"
echo "int card(P(mkSet(1,2,3,4,5,6,7))) end end" | query2ref set.ref -b >! startC
oneRun.sh C $nbRepeat
echo "powerSet(8)"
echo "int card(P(mkSet(1,2,3,4,5,6,7,8))) end end" | query2ref set.ref -b >! startC
oneRun.sh C $nbRepeat

gmake --quiet -f set.make veryclean

endif

if ($1 =~ "nat10" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
echo "Nat10"
echo "compiler"
elanc -quiet --batch -nosplit -optimiseChoicePoint nat10  >/dev/null
gmake --quiet -f nat10.make
echo "fib(10)"
echo "Nat fib(((d)1)0) end end" | query2ref nat10.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(11)"
echo "Nat fib(((d)1)1) end end" | query2ref nat10.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(12)"
echo "Nat fib(((d)1)2) end end" | query2ref nat10.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(13)"
echo "Nat fib(((d)1)3) end end" | query2ref nat10.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(14)"
echo "Nat fib(((d)1)4) end end" | query2ref nat10.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(15)"
echo "Nat fib(((d)1)5) end end" | query2ref nat10.ref -b >! startC
oneRun.sh C $nbRepeat
echo "fib(16)"
echo "Nat fib(((d)1)6) end end" | query2ref nat10.ref -b >! startC
oneRun.sh C $nbRepeat

gmake --quiet -f nat10.make veryclean

endif
