#! /bin/csh

foreach i (`ls *.[ch]`)   
  echo $i
  sed -f ./barbar $i >$i.TMP
  /bin/cp -f $i.TMP $i
  /bin/rm -f $i.TMP 
end


