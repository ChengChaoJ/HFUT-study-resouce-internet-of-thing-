stack	segment stack
stack	ends
data    segment
oldisr  dw ?,?
timer   db 100
counter dw 0
isdone  db 0
data    ends
    
code    segment
assume	cs:code,ds:data
start:  
        mov ax,data;把data赋给给ax							
	    mov ds,ax;把数据转到数据段寄存器
 
        mov ax,0
        mov es,ax
        ;protect
        cli;CLI
        mov ax,es:[1ch*4]
        mov oldisr[0],ax
        mov ax,es:[1ch*4+2]
        mov oldisr[2],ax
        sti
        ;set new isr
        cli
        mov word ptr es:[1ch*4],offset isr
        mov word ptr es:[1ch*4+2],seg isr
        sti
        ;wait here
 
waithere:
        cmp isdone,1
        jnz waithere
 
        ;restore
exit:
        cli
        mov ax,oldisr[0]
        mov es:[1ch*4],ax
        mov ax,oldisr[2]
        mov es:[1ch*4+2],ax;恢复原中断处理
        sti
 
        mov ax,4c00h;该中断就是安全退出程序
        int 21h
 
isr proc far
        push dx
        push ax
 
        mov ax,data
        mov ds,ax
 
        sti;便于中断嵌套             
       ;COUNT HERE
        inc timer
again:
        cmp timer,1000/55;18
        jb done
        mov timer,0
 
        ;carriage
        mov ah,2
        mov dl,13
        int 21h
 
        ;print time
        mov ax,counter
 
        mov dl,10
        div dl
        mov dh,ah
        mov dl,al
        mov ah,2
        add dl,30h
        int 21h
        mov dl,dh
        add dl,30h
        int 21h
        
        inc counter
        cmp counter,90 
        jnz done
        mov isdone,1;设置完成标志   
        
done:
        pushf
        call dword ptr oldisr;调用原中断处理
        cli
        pop ax
        pop dx
        iret;中断返回
isr     endp
 
code    ends
        end start