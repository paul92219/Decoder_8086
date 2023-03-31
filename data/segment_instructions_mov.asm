	bits 16
	
	mov es, [10]
	mov cs, [43]
	mov ss, [1231]
	mov ds, [115]

	mov es, bx
	mov cs, dx
	mov ss, cx
	mov ds, ax

	mov bx, es
	mov dx, cs
	mov cx, ss
	mov ax, ds

	mov ss, [bx + si + 4]
	mov es, [bx + si + 4999]

	mov [bx + si + 4], cs

	mov [bx + si + 4999], ds

	
