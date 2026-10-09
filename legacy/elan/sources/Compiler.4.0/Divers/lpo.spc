specification  lpo

    vars x y z 

    ars n_0:0#0 n_1:0#0 n_2:0#0 Ff:0#2 Gg:0#2 Hh:0#1 Ii:0#1 Jj:1#1 

//----------------------------------------------------------------------------------

less
	Jj Ii
	# 
	Ff Ii
	# 
	Gg Ff
	#  

equiv
	n_0 n_1 
	# 

end of specification

// False
// Ii(x Jj Ii(y)) >mpo Ii(Ii(y) Jj x)                           end

// False
// Ii(Ii(y) Jj x) >mpo Ii(x Jj Ii(y))                           end

// equiv_mpo(Ii(Ii(y) Jj x), Ii(x Jj Ii(y)))                    end

// Hh(Ff(n_0, n_1)) >mpo Ff(n_0, n_1) 				end
 
// Hh(Ff(n_0, n_1)) >lpo Ff(n_0, n_1) 				end 

// Hh(Ff(n_0, n_1)) >mpo Ff(n_1, n_0)				end
 
// Hh(Ff(n_0, n_1)) >lpo Ff(n_1, n_0)				end 

// Hh(Ff(n_0, n_2)) >mpo Ff(n_2, n_0) 				end 

// False (True if Hh >sig Ff)
// Hh(Ff(n_0, n_2)) >lpo Ff(n_2, n_0) 				end 

// False (True if Hh >sig Ff)
// Hh(Gg(n_2,Ff(n_0, n_2))) >mpo Ff(Hh(n_2), Hh(Ff(n_0,n_2))) 	end 

// False (True if Hh >sig Ff)
// Hh(Gg(n_2,Ff(n_0, n_2))) >lpo Ff(Hh(n_2), Hh(Ff(n_0,n_2))) 	end 

// Ff(n_0,y) >lpo Ii(y) 					end 

// Ff(n_0,y) >mpo Ii(y) 					end
 
// Ff(Ii(x),n_0) >lpo Ff(x,Ii(n_0)) 				end 

// Ff(Ii(x),n_0) >mpo Ff(x,Ii(n_0)) 				end 

// Ff(Ii(x),Ii(y)) >lpo Ff(x,Ff(Ii(x),y)) 			end 

// Ff(Ii(x),Ii(y)) >mpo Ff(x,Ff(Ii(x),y)) 			end 