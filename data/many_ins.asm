	bits 16
	;; 46
	mov ax, 0x2222 		; ax: 0x2222
	mov bx, 0x4444		; bx: 0x4444
	mov cx, 0x6666		; cx: 0x6666
	mov dx, 0x8888		; dx: 0x8888

	mov ss, ax		; ss: 0x2222
	mov ds, bx		; ds: 0x4444
	mov es, cx		; es: 0x6666
	mov al, 0x11		; ax: 0x2211

	mov bh, 0x33		; bx: 0x3344
	mov cl, 0x55		; cx: 0x6655
	mov dh, 0x77		; dx: 0x7788
	mov ah, bl		; ax: 0x4411

	mov cl, dh		; cx: 0x6677
	mov ss, ax		; ss: 0x4411
	mov ds, bx		; ds: 0x3344
	mov es, cx		; es: 0x6677

	mov sp, ss		; sp: 0x4411
	mov bp, ds		; bp: 0x3344
	mov si, es		; si: 0x6677
	mov di, dx		; di: 0x7788

	mov dx, 0x0f6c		; dx: 0x0f6c
	mov dx, 0xf094		; dx: 0xf094
	mov ah, [bx + si + 4]
	mov al, [bx + si + 4999]

	mov si, bx		; si: 0x3344
	mov dh, al		; dx: 0x1194
	mov cx, 0x000c		; cx: 0x000c
	mov cx, 0xfff4		; cx: 0xfff4

	mov al, [bx + si]
	mov bx, [bp + di]
	mov dx, [bp]
	mov [bx + di], cx

	mov [bp + si], cl
	mov [bp], ch
	mov ax, [2555]
	mov ax, [16]

	mov [2554], ax
	mov [15], ax
	mov bp, [5]
	mov bx, [3458]

	mov [bp + di], byte 7   
	mov [di + 901], word 347
	mov [bp], byte 8
	mov ax, [bx + di - 37]

	mov [si - 300], cx
	mov dx, [bx - 32]

	;; 23
	add al, -30 		;
	add cx, 8		; cx: 8
	add ax, cx		; ax: 8
	add si, 2900

	add word [4834], 29
	add byte [bx], 34
	add word [bp + si + 1000], 29   
	add si, 2

	add bp, 2
	add ax, 1000
	add bx, [bp]
	add bx, [bp + 0]

	add cx, [bx + 2]
	add bh, [bp + si + 4]
	add di, [bp + di + 6]
	add [bp + si + 4], bh

	add [bp + di + 6], di
	add ax, [bp]
	add bx, [bx + si]
	add al, [bx + si]

	add al, ah
	add [bx + si], bx
	add [bp], bx
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

 	;; 23
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
