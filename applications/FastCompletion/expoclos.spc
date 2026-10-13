specification expoclos
Vars 
	x

Ops 
	f:2 aa:0 ab:0 ac:0 ad:0 ae:0 af:0 b:0

Prec 
	f:1 af:3 ae:4 ad:5 ac:6 ab:7 aa:8 b:9

System
	aa=f(ab,ab)
	ab=f(ac,ac)
	ac=f(ad,ad)
	ad=f(ae,ae)
	ae=f(af,af)
	f(aa,aa)=b
	nil
end of specification
