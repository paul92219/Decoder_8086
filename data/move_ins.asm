	bits 16

	mov ax, 0x1111
	mov cx, 0x2222
	mov bx, 0x3333
	mov dx, 0x4444
	
	mov al, bh		;ax: 0x1133
	mov bl, ah		;dx: 0x3311
	mov cl, dh		;bx: 0x2244
	mov dl, ch		;ax: 0x4422
