	bits 16
	;; 23
	jnz $+2
	jnz $+4
	jnz $+6
	jnz $+4

	jl $+2
	jle $+4
	jb $+6
	jbe $+8

	jp $+10
	jo $+12
	js $+14
	jne $+16

	jnl $+18
	jg $+20
	jnb $+22
	ja $+24

	jnp $+26
	jno $+28
	jns $+30
	loop $+32

	loopz $+34
	loopnz $+36
	jcxz $+38
