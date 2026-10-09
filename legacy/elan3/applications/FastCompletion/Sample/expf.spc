specification expf
Vars 
	x

Ops 
	f:2 b:0 aa:0 ab:0 ac:0 ad:0 ae:0 af:0 ag:0

Prec 
	b:9 aa:8 ab:7 ac:6 ad:5 ae:4 af:3 ag:2 f:1

System
	aa = f(ab,ab)
	ab = f(ac,ac)
	ac = f(ad,ad)
	ad = f(ae,ae)
	ae = f(af,af)
	af = f(ag,ag)
	f(aa,aa) = b
	nil
end of specification
