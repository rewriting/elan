#!/bin/tcsh

@ nbRepeat = 10

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis

if ($1 =~ "fib_builtin" || $1 =~ "all") then
echo "------------------------------------------------------------"

echo "fib_builtin"
echo "interpreter"
echo "fib(18) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(19) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(20) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(21) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(22) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(23) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(24) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(25) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
echo "fib(26) end" |tee startI
oneRun.sh I $nbRepeat fib_builtin no
endif

if ($1 =~ "nqueensAC" || $1 =~ "all") then
echo "------------------------------------------------------------"
echo "nqueensAC"
echo "interpreter"
echo "nqueens(4) end" |tee startI
oneRun.sh I $nbRepeat nqueensAC no
echo "nqueens(5) end" |tee startI
oneRun.sh I $nbRepeat nqueensAC no
echo "nqueens(6) end" |tee startI
oneRun.sh I $nbRepeat nqueensAC no
echo "nqueens(7) end" |tee startI
oneRun.sh I $nbRepeat nqueensAC no
echo "nqueens(8) end" |tee startI
oneRun.sh I $nbRepeat nqueensAC no
endif

if ($1 =~ "bool3" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
echo "Bool3"
echo "interpreter"
echo "q2 end" |tee startI
oneRun.sh I $nbRepeat bool3 no
echo "q3 end" |tee startI
oneRun.sh I $nbRepeat bool3 no
echo "q4 end" |tee startI
oneRun.sh I $nbRepeat bool3 no
echo "q5 end" |tee startI
oneRun.sh I $nbRepeat bool3 no
endif

if ($1 =~ "ans" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis/FastCompletion
echo "interpreter"
echo "p1"
echo "sat end" |tee startI
oneRun.sh I 2 ans p1
echo "p2"
echo "sat end" |tee startI
oneRun.sh I 2 ans p2
echo "p3"
echo "sat end" |tee startI
oneRun.sh I 2 ans p3
echo "p4"
echo "sat end" |tee startI
oneRun.sh I 2 ans p4
#echo "sat end" | elan -b -q -s ans p5
cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
endif

if ($1 =~ "minela" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis/MinelaComp
echo "compiler"
echo "MinelaComp"
echo "interpreter"
echo "primes(s(s(o))) end" |tee startI
oneRun.sh I 2 rewriting primes
echo "primes(s(s(s(o)))) end" |tee startI
oneRun.sh I 2 rewriting primes
echo "primes(s(s(s(s(o))))) end" |tee startI
oneRun.sh I 2 rewriting primes
echo "primes(s(s(s(s(s(o)))))) end" |tee startI
oneRun.sh I 2 rewriting primes
cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
endif

if ($1 =~ "set" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
echo "Set"
echo "interpreter"
echo "card(P(mkSet(1,2,3,4))) end" |tee startI
oneRun.sh I $nbRepeat set no
echo "card(P(mkSet(1,2,3,4,5))) end" |tee startI
oneRun.sh I $nbRepeat set no
echo "card(P(mkSet(1,2,3,4,5,6))) end" |tee startI
oneRun.sh I $nbRepeat set no
echo "card(P(mkSet(1,2,3,4,5,6,7))) end" |tee startI
oneRun.sh I $nbRepeat set no
echo "card(P(mkSet(1,2,3,4,5,6,7,8))) end" |tee startI
oneRun.sh I $nbRepeat set no
endif

if ($1 =~ "nat10" || $1 =~ "all") then
echo "------------------------------------------------------------"
cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
echo "Nat10"
echo "interpreter"
echo "fib(((d)1)0) end" |tee startI
oneRun.sh I $nbRepeat nat10 no
echo "fib(((d)1)1) end" |tee startI
oneRun.sh I $nbRepeat nat10 no
echo "fib(((d)1)2) end" |tee startI
oneRun.sh I $nbRepeat nat10 no
echo "fib(((d)1)3) end" |tee startI
oneRun.sh I $nbRepeat nat10 no
echo "fib(((d)1)4) end" |tee startI
oneRun.sh I $nbRepeat nat10 no
echo "fib(((d)1)5) end" |tee startI
oneRun.sh I $nbRepeat nat10 no
endif


