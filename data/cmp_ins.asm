	bits 16
	;; 23
	cmp ax, 1000

	cmp al, -30 
	cmp bx, [bx + si]
	cmp al, [bx + si]
	cmp ax, bx

	cmp al, ah
	cmp [bx + si], bx
	cmp [bp], bx

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
