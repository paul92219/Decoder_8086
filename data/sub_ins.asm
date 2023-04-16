	bits 16
	;; 23
	sub si, 2900
	sub word [4834], 29
	sub bx, [bp]
	sub bx, [bp + 0]

	sub cx, [bx + 2]
	sub bh, [bp + si + 4]
	sub di, [bp + di + 6]
	sub [bp + si + 4], bh

	sub [bp + di + 6], di
	sub ax, [bp]
	sub byte [bx], 34
	sub word [bp + si + 1000], 29   

	sub si, 2
	sub bp, 2
	sub cx, 8
	sub ax, 1000

	sub al, -30 
	sub bx, [bx + si]
	sub al, [bx + si]
	sub ax, bx

	sub al, ah
	sub [bx + si], bx
	sub [bp], bx
