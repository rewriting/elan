#!/bin/csh

foreach i (`ls *.java`)   
  echo $i
  cat header >! $i.TMP
  cat $i >> $i.TMP
  /bin/mv $i.TMP $i
end
