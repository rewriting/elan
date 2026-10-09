#! /bin/csh

sed -f ~/elan/bin/jolifier $1 >$1.TMP
/bin/cp -f $1.TMP $1
/bin/rm -f $1.TMP
