mov ax, 8738
mov bx, 17476
mov cx, 26214
mov dx, 34952
mov ss, ax
mov ds, bx
mov es, cx
mov al, 17
mov bh, 51
mov cl, 85
mov dh, 119
mov ah, bl
mov cl, dh
mov ss, ax
mov ds, bx
mov es, cx
mov sp, ss
mov bp, ds
mov si, es
mov di, dx
mov dx, 3948
mov dx, 61588
mov ah, [bx + si +4]
mov al, [bx + si +4999]
mov si, bx
mov dh, al
mov cx, 12
mov cx, 65524
mov al, [bx + si]
mov bx, [bp + di]
mov dx, [bp +0]
mov [bx + di], cx
mov [bp + si], cl
mov [bp +0], ch
mov ax, [2555]
mov ax, [16]
mov [2554], ax
mov [15], ax
mov bp, [5]
mov bx, [3458]
mov [bp + di], byte 7
mov [di +901], word 347
mov [bp +0], byte 8
mov ax, [bx + di -37]
mov [si -300], cx
mov dx, [bx -32]
add al, 226
add cx, 8
add ax, cx
add si, 2900
add word [4834], 29
add byte [bx], 34
add word [bp + si +1000], 29
add si, 2
add bp, 2
add ax, 1000
add bx, [bp +0]
add bx, [bp +0]
add cx, [bx +2]
add bh, [bp + si +4]
add di, [bp + di +6]
add [bp + si +4], bh
add [bp + di +6], di
add ax, [bp +0]
add bx, [bx + si]
add al, [bx + si]
add al, ah
add [bx + si], bx
add [bp +0], bx
sub si, 2900
sub word [4834], 29
sub bx, [bp +0]
sub bx, [bp +0]
sub cx, [bx +2]
sub bh, [bp + si +4]
sub di, [bp + di +6]
sub [bp + si +4], bh
sub [bp + di +6], di
sub ax, [bp +0]
sub byte [bx], 34
sub word [bp + si +1000], 29
sub si, 2
sub bp, 2
sub cx, 8
sub ax, 1000
sub al, 226
sub bx, [bx + si]
sub al, [bx + si]
sub ax, bx
sub al, ah
sub [bx + si], bx
sub [bp +0], bx
cmp si, 2900
cmp word [4834], 29
cmp bx, [bp +0]
cmp bx, [bp +0]
cmp cx, [bx +2]
cmp bh, [bp + si +4]
cmp di, [bp + di +6]
cmp [bp + si +4], bh
cmp [bp + di +6], di
cmp ax, [bp +0]
cmp byte [bx], 34
cmp word [bp + si +1000], 29
cmp si, 2
cmp bp, 2
cmp cx, 8
cmp ax, 1000
cmp al, 226
cmp bx, [bx + si]
cmp al, [bx + si]
cmp ax, bx
cmp al, ah
cmp [bx + si], bx
cmp [bp +0], bx
jnz $+2
jnz $+4
jnz $+6
jnz $+4
jl $+2
jng $+4
jb $+6
jna $+8
jp $+10
jo $+12
js $+14
jnz $+16
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
