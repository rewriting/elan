specification echec
Vars 
	x y

Ops 
	f:4 g:2 h:1 k:1 l:1 zero:0 bot:0

Rules
	f(h(x),k(x),l(x),g(x,y))->f(y,y,y,g(x,y))
	f(x,x,x,g(zero,y))->bot
	nil
end of specification
