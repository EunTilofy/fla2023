; input ucu -> accept

; the finite set of states
#Q = {0,halt_acc,halt_rej,before_c,after_c,wait_rej,t1,r1,u1,e1,f2,a2,l2,s2,e2,chk}

; the finite set of input symbols
#S = {a,b,c}

; the complete set of tape symbols
#G = {a,b,c,_,t,r,u,e,f,a,l,s}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_acc}

; the number of tapes
#N = 3

; the transition functions

0 *** *** *** before_c
before_c a** _a* rr* before_c
before_c b** _b* rr* before_c
before_c c** _** r** after_c
before_c _** _** *** wait_rej
after_c a** _*a r*r after_c
after_c b** _*b r*r after_c
after_c c** _** r** wait_rej
after_c _** _** *ll chk

chk *aa *__ *ll chk
chk *ab *__ *** wait_rej
chk *bb *__ *ll chk
chk *ba *__ *** wait_rej
chk *__ *__ *** t1
chk *_a *__ *** wait_rej
chk *_b *__ *** wait_rej
chk *a_ *__ *** wait_rej
chk *b_ *__ *** wait_rej

t1 *** t__ r** r1
r1 *** r__ r** u1
u1 *** u__ r** e1
e1 *** e__ *** halt_acc

wait_rej a** _** r** wait_rej
wait_rej b** _** r** wait_rej
wait_rej c** _** r** wait_rej
wait_rej _** _** *** f2
f2 *** f** r** a2
a2 *** a** r** l2
l2 *** l** r** s2
s2 *** s** r** e2
e2 *** e** *** halt_rej