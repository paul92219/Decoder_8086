	bits 16

	mov cx, 3
	mov bx, 1000
	loop_start:
	add bx, 10
	sub cx, 1
	jnz loop_start
