 BEGIN {
 number = 0
 total  = 0
}

$4 ~ /^[0-9]/ {
   number++
   printf("%d : %d\n",number, $4);
   total = total + $4
}

END {
#   printf("total   = %d\n",total);	
#   printf("number = %d\n",number);	
   printf("moyenne = %d rwr/sec\n",total/number);	
}