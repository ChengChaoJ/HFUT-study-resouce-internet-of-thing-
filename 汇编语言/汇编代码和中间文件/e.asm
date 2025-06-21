assume cs:code, ds:data
data segment
    buff1 db 50, ?, 50 dup(?)
    buff2 db "asm"
    new db 0dh, 0ah, "$"
    a db "yes$"
    b db "no$"
data ends

code segment
start: 
    mov ax, data
    mov ds, ax
    mov es, ax
    lea dx, buff1
    mov ah, 0ah
    int 21h

    mov dx, offset new
    mov ah, 9
    int 21h

    lea dx, buff1+2
    mov ah, 9
    int 21h

    lea dx, new
    mov ah, 9
    int 21h

    lea si, buff1+2
    lea di, buff2
    mov dx, si
    mov bx, di
    mov al, 50

s: 
    mov cx, 3
    repz cmpsb
    jz find

    inc dx
    mov si, dx
    mov di, bx
    dec al
    jnz s

    lea dx, b
    mov ah, 9
    int 21h
    jmp done

find:
    lea dx, a
    mov ah, 9
    int 21h

done:
    mov ah, 4ch
    int 21h

code ends
end start