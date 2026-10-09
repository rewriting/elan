#!/bin/tcsh

if($1 =~ *C*) then
  echo "" >! tmpC.d
else
  echo "" >! tmpI.d
endif

@ i=0
while ($i<$2)
    @ i++
  if($1 =~ *C*) then
    cat startC | a.out -query | grep "average speed" >> tmpC.d
  else
    elan -b -q -s $3 $4 < startI | grep "average speed" >> tmpI.d
    endif
end

if($1 =~ *C*) then
  awk -f moyenne.awk tmpC.d
else
  awk -f moyenne.awk tmpI.d
endif
