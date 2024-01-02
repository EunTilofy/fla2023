; Input: a string of a^nb^m
; Output: c^nm

; the finite set of states
#Q = {0,halt_acc,halt_rej,wait,i1,l2,l3,e4,g5,a6,l7,_8,i9,n10,p11,u12,t13,scan_a,scan_b}

; the finite set of input symbols
#S = {a,b}

; the complete set of tape symbols
#G = {a,b,c}

; the start state
#q0 = 0

; the blank symbol
#B = _

; the set of final states
#F = {halt_accept}

; the number of tapes
#N = 3

; the transition functions

0 a__ *__ *** scan_a
0 b__ ___ r** wait
0 ___ ___ *** wait
scan_a a** _a_ rr* scan_a
scan_a _** ___ *** wait
scan_a b** b__ *** scan_b
scan_b b** __b r*r scan_b
scan_b a** ___ r** wait
scan_b _** ___ *** halt_acc



; Reject Input
wait a** ___ r** wait
wait b** ___ r** wait
wait _** ___ *** i1
i1 *** i__ r** l2
l2 *** l__ r** l3
l3 *** l__ r** e4
e4 *** e__ r** g5
g5 *** g__ r** a6
a6 *** a__ r** l7
l7 *** l__ r** _8
_8 *** ___ r** i9
i9 *** i__ r** n10
n10 *** n__ r** p11
p11 *** p__ r** u12
u12 *** u__ r** t13
t13 *** t__ r** halt_rej
