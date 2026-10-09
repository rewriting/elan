#!/bin/tcsh

@ nbRepeat = 50

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

echo "nqueensAC without color"
elanc -quiet --batch -nosplit -optimiseChoicePoint -noColor nqueensAC >/dev/null
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

echo "Bool3 without color"
echo "compiler"
elanc -quiet --batch -nosplit -optimiseChoicePoint -noColor bool3  >/dev/null
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

if ($1 =~ "set" || $1 =~ "all") then
echo "------------------------------------------------------------"

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
echo "Set without color"
echo "compiler"
elanc -quiet --batch -nosplit -optimiseChoicePoint -noColor set  >/dev/null
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

echo "Nat10 without color"
echo "compiler"
elanc -quiet --batch -nosplit -optimiseChoicePoint -noColor nat10  >/dev/null
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
