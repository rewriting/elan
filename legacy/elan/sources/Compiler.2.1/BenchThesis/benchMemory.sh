#!/bin/csh

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis

if ($1 =~ "fib_builtin" || $1 =~ "all") then
echo "------------------------------------------------------------"

echo "fib_builtin"
elanc -quiet --batch -nosplit -optimiseChoicePoint -debug fib_builtin
gmake --quiet -f fib_builtin.make
gmake --quiet -f fib_builtin.make veryclean
echo "compiler"
echo "FSYM(INT(5).nil,200)" | a.out -query
echo "FSYM(INT(10).nil,200)" | a.out -query
echo "FSYM(INT(15).nil,200)" | a.out -query
echo "FSYM(INT(20).nil,200)" | a.out -query
echo "FSYM(INT(25).nil,200)" | a.out -query
echo "interpreter"
echo "fib(5) end" | elan -b -s -q fib_builtin
echo "fib(10) end" | elan -b -s -q fib_builtin
echo "fib(15) end" | elan -b -s -q fib_builtin
echo "fib(17) end" | elan -b -s -q fib_builtin
echo "fib(20) end" | elan -b -s -q fib_builtin
echo "fib(21) end" | elan -b -s -q fib_builtin
echo "fib(22) end" | elan -b -s -q fib_builtin
echo "fib(23) end" | elan -b -s -q fib_builtin
echo "fib(24) end" | elan -b -s -q fib_builtin
echo "fib(25) end" | elan -b -s -q fib_builtin
endif

if ($1 =~ "nqueensAC" || $1 =~ "all") then
echo "------------------------------------------------------------"

echo "nqueensAC"
elanc -quiet --batch -nosplit -optimiseChoicePoint -debug nqueensAC
gmake --quiet -f nqueensAC.make
gmake --quiet -f nqueensAC.make veryclean
echo "compiler"
echo "FSYM(FSYM(INT(4).nil,200).nil,226)" | a.out -query
echo "FSYM(FSYM(INT(5).nil,200).nil,226)" | a.out -query
echo "FSYM(FSYM(INT(6).nil,200).nil,226)" | a.out -query
echo "FSYM(FSYM(INT(7).nil,200).nil,226)" | a.out -query
echo "FSYM(FSYM(INT(8).nil,200).nil,226)" | a.out -query
echo "interpreter"
echo "nqueens(4) end" | elan -b -s -q nqueensAC
echo "nqueens(5) end" | elan -b -s -q nqueensAC
echo "nqueens(6) end" | elan -b -s -q nqueensAC
echo "nqueens(7) end" | elan -b -s -q nqueensAC
echo "nqueens(8) end" | elan -b -s -q nqueensAC
endif

if ($1 =~ "ans" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis/FastCompletion
echo "compiler"
echo "FastCompletion"
elanc -quiet --batch -nosplit -optimiseChoicePoint -debug ans p1
gmake --quiet -f ans.make
gmake --quiet -f ans.make veryclean
a.out
elanc -quiet --batch -nosplit -optimiseChoicePoint -debug ans p2
gmake --quiet -f ans.make
gmake --quiet -f ans.make veryclean
a.out
elanc -quiet --batch -nosplit -optimiseChoicePoint -debug ans p3
gmake --quiet -f ans.make
gmake --quiet -f ans.make veryclean
a.out
elanc -quiet --batch -nosplit -optimiseChoicePoint -debug ans p4
gmake --quiet -f ans.make
gmake --quiet -f ans.make veryclean
a.out
elanc -quiet --batch -nosplit -optimiseChoicePoint -debug ans p5
gmake --quiet -f ans.make
gmake --quiet -f ans.make veryclean
a.out
echo "interpreter"
echo "sat end" | elan -b -s -q ans p1
echo "sat end" | elan -b -s -q ans p2
echo "sat end" | elan -b -s -q ans p3
echo "sat end" | elan -b -s -q ans p4
#echo "sat end" | elan -b -s -q ans p5
cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
endif

if ($1 =~ "bool3" || $1 =~ "all") then
echo "------------------------------------------------------------"

cd /users/protheo/moreau/ELAN/elan/sources/Compiler/BenchThesis
echo "Bool3"
echo "compiler"
elanc -quiet --batch -nosplit -debug bool3
gmake --quiet -f bool3.make
gmake --quiet -f bool3.make veryclean
echo "FSYM(nil,217)" | a.out -query
echo "FSYM(nil,218)" | a.out -query
echo "FSYM(nil,219)" | a.out -query
echo "FSYM(nil,220)" | a.out -query
echo "FSYM(nil,221)" | a.out -query
echo "FSYM(nil,222)" | a.out -query
echo "interpreter"
echo "q2 end" | elan -b -s -q bool3
echo "q3 end" | elan -b -s -q bool3
echo "q4 end" | elan -b -s -q bool3
echo "q5 end" | elan -b -s -q bool3
endif

echo "------------------------------------------------------------"
