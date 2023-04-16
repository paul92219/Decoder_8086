	bits 16
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
