stack	segment stack
		dw 512	dup(?)
stack	ends
data	segment;在数据段定义数据
		x dw 0 
		y dw 0
		z dw 0
		mes1 db 'Please input x: $';,0dh,0ah,'$'
		mes2 db 'Please input y: $';,0dh,0ah,'$'
		mes3 db 'The result is: $';,0dh,0ah,'$'
data	ends
code	segment ;z=f(x,y)=x*y+x-y
assume	cs:code,ds:data,ss:stack
start:	mov ax,data;把data赋给给ax							
		mov ds,ax;把数据转到数据段寄存器
 
        mov ah,09h
       	mov dx,offset mes1;提示输入
        int 21h
				
		call input
		mov x,ax;输入x
 
		mov ah,09h
       	mov dx,offset mes2;提示输入
        int 21h
 
		call input
		mov y,ax;输入y
 
		;计算
		push x
		push y
		call fun
 
		mov z,ax
		mov ah,09h
        mov dx,offset mes3
        int 21h
		mov ax,z
		call output
		jmp done
 
fun	proc near
		push bp
		mov bp,sp
		push bx
		push cx
		
		mov cx,[bp+6];提取x
		mov bx,[bp+4];提取y
 
		mov ax,cx
		imul bx;x*y
		add ax,cx;x*y+x
		sub ax,bx;x*y+x-y
 
		pop cx
		pop bx
		pop bp
		
		ret 4
fun	endp
 
input proc near
		push bx
        push cx
        push dx
		xor bx,bx;bx保存结果
		xor cx,cx;cx为正负标志，0为正，1为负
		mov ah,1;输入一个字符
		int 21h
		cmp al,'+';是“+”，继续输入字符
		jz input1
		cmp al,'-';是“-”
		jnz input2
		mov cx,-1;设置-1标志
input1:
		mov ah,1
		int 21h
input2:
		cmp al,'0';不是0~9之间的字符，则输入数据结束
		jb input3
		cmp al,'9'
		ja input3
		sub al,30h;是0~9之间的字符，则转换为二进制数
		
		shl bx,1;利用移位指令，实现数值乘10：BX<-BX*10
		mov dx,bx
		shl bx,1
		shl bx,1
		add bx,dx
		mov ah,0
		add bx,ax;已输入数值乘10后，与新输入数值相加
		jmp input1;继续输入字符
input3:
		cmp cx,0;是负数，进行求补
		jz input4
		neg bx
input4:
		mov ax,bx;设置出口参数
		pop dx
		pop cx
		pop bx
		ret;子程序返回
 
output proc near
        push ax
        push bx
        push dx
        test ax,ax
        jnz output1
        mov dl,'0'
        mov ah,2
        int 21h
        jmp output5
output1:
		jns output2
        mov bx,ax
        mov dl,'-'
        mov ah,2
        int 21h
        mov ax,bx
        neg  ax
output2:
		mov bx,10
        push bx
output3:
		cmp ax,0
        jz output4
        sub dx,dx
        div bx
        add dl,30h
        push dx
        jmp output3
output4:
		pop dx
        cmp dl,10
        je output5
        mov ah,2
        int 21h
        jmp output4
output5: 
		pop dx
        pop bx
        pop ax
        ret
 
done:	
		mov ah,4ch
		int 21h
code	ends
		end start