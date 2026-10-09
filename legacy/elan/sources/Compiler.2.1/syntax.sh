#! /bin/csh

foreach i (ls *.eln)   
  echo $i
  sed -f syntax $i >$i.TMP
  /bin/cp -f $i.TMP $i
  /bin/rm -f $i.TMP
end
