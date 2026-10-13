specification  rpo

    vars x y z 

    ars n_0:0#0 n_1:0#0 n_2:0#0 Ff:0#2 Gg:0#2 Hh:0#1 Ii:0#1 Jj:1#1 

//----------------------------------------------------------------------------------

less
	
	Hh Ff Ii
	# 
	Gg Ff
	#  
	Jj Ii
	#  

equiv
	n_0 n_1 
	# 
status
	Ff:MS Jj:MS

end of specification
