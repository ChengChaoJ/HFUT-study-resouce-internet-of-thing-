stack	segment stack
		dw 512 dup(?)
stack	ends
data	segment
		buffer 	db 20
				db 0
				db 20 dup(0)
		minx db ?
		maxx db ?
		avgx db ?
		num dw ?
		string db 'Enter the grade of student:',13,10,'$'
		minni db 'The min of grade is:',13,10,'$'
		maxxa db 'The max of grade is:',13,10,'$'
		avggv db 'The avg of grade is:',13,10,'$'
data	ends
code	segment
		assume cs:code,ds:data,ss:stack
start:	mov ax,data
		mov ds,ax
		mov bx,0
		mov cx,50
		mov [minx],64h
		mov [maxx],00h
again:	lea dx,string
		mov ah,9
		int 21h
		mov ah,0ah
		mov dx,seg buffer
		mov ds,dx
		mov dx,offset buffer
		int 21h
		push bx
		push ax
		call stoi
		pop ax
		pop bx
		add bx,[num]
		mov ax,[num]
		cmp al,[minx]
		jb minnx
fuck:	cmp al,[maxx]
		ja maxxx
		jmp done
minnx:	mov [minx],al
		jmp fuck
maxxx:	mov [maxx],al
done:	mov dl,10
		mov ah,2
		int 21h
		mov dl,13
		mov ah,2
		int 21h
		loop again
		mov ax,bx
		mov bx,32h
		mov dx,0
		div bx
		mov bx,ax
		lea dx,avggv
		mov ah,9
		int 21h
		mov ax,bx
		call dis
		lea dx,minni
		mov ah,9
		int 21h
		mov al,[minx]
		call dis
		lea dx,maxxa
		mov ah,9
		int 21h
		mov al,[maxx]
		call dis
		mov ah,4ch
		int 21h
stoi	proc
		mov DX,0
		mov bx,10
		mov SI,2
		mov [num],0
		mov AX,0
LOP:	MOV AL,buffer[SI]
		CMP AL,0DH
		JE  FINAL
		SUB AL,30H
		CMP [num],0
		JE  DO_DEAL 
		PUSH AX
		MOV AX,[num]
		MUL bx 
		MOV [num],AX  
		POP AX
DO_DEAL:
		ADD [NUM],AX
		MOV AX,0
		INC SI
		JMP LOP
FINAL:    
		RET
		stoi ENDP
dis		proc
		push dx
		mov cl,4
		;mov dl,ah
		;call dispa
		;mov dl,ah
		;call dispap
		mov dl,al
		call dispa
		mov dl,al
		call dispap
		pop dx
		push ax
		mov dl,10
		mov ah,2
		int 21h
		mov dl,13
		mov ah,2
		int 21h
		pop ax
		ret
dis		endp
dispa	proc
		push ax
		shr dl,cl
		or dl,30h
		cmp dl,39h
		jbe lab
		add dl,7
lab:	mov ah,2
		int 21h
		pop ax
		ret
dispa	endp

dispap	proc
		push ax
		and dl,0fh
		or dl,30h
		cmp dl,39h
		jbe laba
		add dl,7
laba:	mov ah,2
		int 21h
		pop ax
		ret
dispap	endp
code	ends
		end start
