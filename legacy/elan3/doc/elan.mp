input boxes

beginfig(1);
numeric hsep,vsep;
hsep=4cm;
vsep=3cm;

verbatimtex \def\troisligne#1#2#3{$\displaystyle{\matrix{#1\cr#2\cr#3\cr}}$} etex

boxit.l(btex \bf \troisligne{\bf Logic}{and}{\bf Strategies} etex);
boxit.rl(btex \bf Computational System etex);
circleit.s(btex \bf Specification etex);
circleit.q(btex \bf Query etex);
circleit.sl();
circleit.qrl();
circleit.r(btex \bf Result etex);
l.dx=l.dy=rl.dx=rl.dy=0.5cm;
s.dx=q.dx=r.dx=0.5cm;
s.dy=q.dy=r.dy=0.5cm;
sl.dx=qrl.dx=0.3cm;
sl.dy=qrl.dy=0.3cm;


s.c-sl.c=sl.c-rl.c=(0,vsep);
q.c-qrl.c=qrl.c-r.c=(0,vsep);
rl.c-qrl.c=(hsep,0);
l.c-sl.c=sl.c-q.c=(hsep,0);

drawboxed(l,rl,s,q,sl,qrl,r);

label.rt(btex \bf Super User etex,l.e);
label.rt(btex \bf User etex,s.e);
label.lft(btex \bf End User etex,q.w);
label.lft(btex \sl Preprocessing etex,sl.w);
label.lft(btex \sl Rewrite Engine etex,qrl.w);

drawarrow s.s .. sl.n;
drawarrow sl.s .. rl.n;
drawarrow l.w .. sl.e;
drawarrow rl.w .. qrl.e;
drawarrow q.s .. qrl.n;
drawarrow qrl.s .. r.n;

endfig;


beginfig(2);
numeric hsep,vsep;
hsep=4.3cm;
vsep=3cm;

verbatimtex \def\deuxligne#1#2{\hbox{\vbox{\hbox{#1}\hbox{#2}}}} etex
verbatimtex \def\troisligne#1#2#3{\hbox{\vbox{\hbox{#1}\hbox{#2}\hbox{#3}}}} etex
verbatimtex \def\quatreligne#1#2#3#4{\hbox{\vbox{\hbox{#1}\hbox{#2}\hbox{#3}\hbox{#4}}}} etex
verbatimtex
\def\begin#1#2#3\end#4{\edef\preamble{\if#2c\hss####\hss
                                      \else
                                         \if#2l####\hss
                                         \else
                                            \hss####%
                                         \fi
                                      \fi}%
                       \vbox{\halign\expandafter{\preamble\cr
                               #3}}%
                       }
etex

boxit.l(btex \bf \troisligne{\bf Unification Logic}{and}{\bf Strategies} etex);
boxit.rl(btex \bf Computational System etex);
%circleit.s(btex \bf \troisligne{\bf Specification}{\bf of the}{\bf signature} etex);

circleit.s(btex \begin{multiligne}{c}
\bf    Specification\cr
    of the\cr
    signature\cr
\end{multiligne}
etex);

circleit.q(btex $f(x,a)=f(b,y)$ etex);

circleit.sl();
circleit.qrl();
circleit.r(btex \bf Result etex);
l.dx=l.dy=rl.dx=rl.dy=0.5cm;
s.dx=q.dx=r.dx=0.5cm;
s.dy=q.dy=r.dy=0.5cm;
sl.dx=qrl.dx=0.3cm;
sl.dy=qrl.dy=0.3cm;


s.c-sl.c=sl.c-rl.c=(0,vsep);
q.c-qrl.c=qrl.c-r.c=(0,vsep);
rl.c-qrl.c=(hsep,0);
l.c-sl.c=sl.c-q.c=(hsep,0);

drawboxed(l,rl,s,q,sl,qrl,r);

%label.rt(btex \deuxligne{toto}{tata} etex,l.e);

label.rt(btex \quatreligne{Decomposition}{Merging}{Clash}{$\ldots$} etex,l.e);
label.rt(btex \deuxligne{Vars $x\ y\ z$}{Ops $a:0\ b:0\ c:0\ f:2$} etex,s.e);
label.lft(btex \sl Preprocessing etex,sl.w);
label.lft(btex \sl Rewrite Engine etex,qrl.w);

drawarrow s.s .. sl.n;
drawarrow sl.s .. rl.n;
drawarrow l.w .. sl.e;
drawarrow rl.w .. qrl.e;
drawarrow q.s .. qrl.n;
drawarrow qrl.s .. r.n;

endfig;






beginfig(3);
numeric hsep,vsep;
hsep=4.3cm;
vsep=3cm;

verbatimtex \def\deuxligne#1#2{\hbox{\vbox{\hbox{#1}\hbox{#2}}}} etex
verbatimtex \def\troisligne#1#2#3{\hbox{\vbox{\hbox{#1}\hbox{#2}\hbox{#3}}}} etex
verbatimtex \def\quatreligne#1#2#3#4{\hbox{\vbox{\hbox{#1}\hbox{#2}\hbox{#3}\hbox{#4}}}} etex
verbatimtex
\def\begin#1#2#3\end#4{\edef\preamble{\if#2c\hss####\hss
                                      \else
                                         \if#2l####\hss
                                         \else
                                            \hss####%
                                         \fi
                                      \fi}%
                       \vbox{\halign\expandafter{\preamble\cr
                               #3}}%
                       }
etex

boxit.l(btex \begin{multiligne}{c}
\bf Differentiation Logic\cr
and\cr
\bf Strategies\cr
\end{multiligne}
etex);
%\bf \troisligne{\bf Differentiation Logic}{and}{\bf Strategies} etex);
boxit.rl(btex \bf Computational System etex);
%circleit.s(btex \bf \troisligne{\bf Specification}{\bf of the}{\bf signature} etex);

circleit.s(btex \begin{multiligne}{c}
\bf    Specification\cr
    of the\cr
    signature\cr
\end{multiligne}
etex);

circleit.q(btex $deriv(3*X*X+2*X+7,X)$ etex);

circleit.sl();
circleit.qrl();
circleit.r(btex \bf Result etex);
l.dx=l.dy=rl.dx=rl.dy=0.5cm;
s.dx=q.dx=r.dx=0.5cm;
s.dy=q.dy=r.dy=0.5cm;
sl.dx=qrl.dx=0.3cm;
sl.dy=qrl.dy=0.3cm;


s.c-sl.c=sl.c-rl.c=(0,vsep);
q.c-qrl.c=qrl.c-r.c=(0,vsep);
rl.c-qrl.c=(hsep,0);
l.c-sl.c=sl.c-q.c=(hsep,0);

drawboxed(l,rl,s,q,sl,qrl,r);

label.rt(btex \troisligne{Expand}{Factorize}{$\ldots$} etex,l.e);
label.rt(btex Vars $X.Y.Z.nil$ etex,s.e);
label.top(btex \sl Query etex,q.n);
label.lft(btex \sl Rewrite Engine etex,qrl.w);

drawarrow s.s .. sl.n;
drawarrow sl.s .. rl.n;
drawarrow l.w .. sl.e;
drawarrow rl.w .. qrl.e;
drawarrow q.s .. qrl.n;
drawarrow qrl.s .. r.n;

endfig;



end
