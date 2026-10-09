leftasoc 10 + 10 - 20 * 20 / 20 %;

extension ppexgram;
typeuse numex,numex2,andexps,andexp,all,all2,idexp;
op

 __ )		::(all2)all;
 (__		::(andexps)all2			code 1;
 __		::(andexp)andexps;
 __ & __	::(andexps,andexp)andexps	code 9;

 __ == __	::(numex2,numex2)andexp		code 10;
 __ != __	::(numex2,numex2)andexp		code 11;
 __ <= __	::(numex2,numex2)andexp		code 12;
 __ >= __	::(numex2,numex2)andexp		code 13;
 __ > __	::(numex2,numex2)andexp		code 14;
 __ < __	::(numex2,numex2)andexp		code 15;

 __		::(identifier)idexp		code 16;
 __ == __	::(idexp,idexp)andexp		code 10;
 __ != __	::(idexp,idexp)andexp		code 11;


 (__		::(numex2)all2			code 1;
 __		::(number)numex2		code 2;
 __+__          ::(numex2,numex2)numex2		code 3 pri 10;
 __-__		::(numex2,numex2)numex2		code 4 pri 10;
 __*__          ::(numex2,numex2)numex2		code 5 pri 20;
 __/__		::(numex2,numex2)numex2		code 6 pri 20;
 __%__		::(numex2,numex2)numex2		code 7 pri 20;
 - __		::(numex2)numex2		code 8 pri 30;
 (__)		::(numex2)numex2;


::
axiom
for all:

end



