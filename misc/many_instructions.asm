	bits 16

	;; Register to register
	mov si, bx
	mov dh, al

	;; 8-bit immediate-to-register
	mov cx, 12
	mov cx, -12

	;; 16-bit immediate-to-register
	mov dx, 3948
	mov dx, -3948
	    
	;; Source address calculation
	mov al, [bx + si]
	mov bx, [bp + di]
	mov dx, [bp]

	;; Source address calculation plus 8-bit displacment
	mov ah, [bx + si + 4]

	;; Source address calculation plus 16-bit displacment
	mov al, [bx + si + 4999]

	;; Dest address calculation
	mov [bx + di], cx
	mov [bp + si], cl
	mov [bp], ch

	;; Memory-to-accumulator test
	mov ax, [2555]
	mov ax, [16]

	;; Accumulator-to-memory test
	mov [2554], ax
	mov [15], ax

	;; Direct address
	mov bp, [5]
	mov bx, [3458]

	;; Explicit sizes
	mov [bp + di], byte 7   
	mov [di + 901], word 347
	mov [bp], byte 8

	;; Signed displacements
	mov ax, [bx + di - 37]
	mov [si - 300], cx
	mov dx, [bx - 32]

	add si, 2900
	add word [4834], 29
	add bx, [bp]
	add bx, [bp + 0]
	add cx, [bx + 2]
	add bh, [bp + si + 4]
	add di, [bp + di + 6]
	add [bp + si + 4], bh
	add [bp + di + 6], di
	add ax, [bp]
	add byte [bx], 34
	add word [bp + si + 1000], 29   
	add si, 2
	add bp, 2
	add cx, 8
	add ax, 1000
	add al, -30 
	add bx, [bx + si]
	add al, [bx + si]
	add ax, bx
	add al, ah
	add [bx + si], bx
	add [bp], bx

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
	  
	cmp si, 2900
	cmp word [4834], 29
	cmp bx, [bp]
	cmp bx, [bp + 0]
	cmp cx, [bx + 2]
	cmp bh, [bp + si + 4]
	cmp di, [bp + di + 6]
	cmp [bp + si + 4], bh
	cmp [bp + di + 6], di
	cmp ax, [bp]
	cmp byte [bx], 34
	cmp word [bp + si + 1000], 29   
	cmp si, 2
	cmp bp, 2
	cmp cx, 8
	cmp ax, 1000
	cmp al, -30 
	cmp bx, [bx + si]
	cmp al, [bx + si]
	cmp ax, bx
	cmp al, ah
	cmp [bx + si], bx
	cmp [bp], bx
	
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