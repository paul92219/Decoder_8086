	bits 16

	mov bx, 1000
	mov bp, 2000
	mov si, 3000
	mov di, 4000

	mov cx, bx
	mov dx, 12

	mov dx, [1000]

	mov cx, [bx]
	mov cx, [bp]
	mov [si], cx
	mov [di], cx 

	mov cx, [bx + 1000]
	mov cx, [bp + 1000]
	mov [si + 1000], cx 
	mov [di + 1000], cx

	add cx, dx
	add [di + 1000], cx
	add dx, 50
	
