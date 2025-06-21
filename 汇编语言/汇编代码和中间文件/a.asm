; 程序名称: CalculateValue.asm
; 计算表达式 (V - (X*Y + Z - 720)) / X

.MODEL SMALL
.STACK 100h
.DATA
    X DW 10       ; 变量 X
    Y DW 20       ; 变量 Y
    Z DW 100      ; 变量 Z
    V DW 500      ; 变量 V
    Result DW ?   ; 存放结果

.CODE
MAIN PROC
    MOV AX, @DATA
    MOV DS, AX

    MOV AX, V
    SUB AX, 720
    MOV BX, X
    MOV CX, Y
    IMUL CX        ; AX = X * Y (将 Y 存入 CX，乘法结果在 DX:AX)
    ADD AX, DX     ; 如果需要，可以累加 DX (高位结果)
    ADD AX, BX     ; 加上 X * Y 的低位
    ADD AX, Z

    MOV BX, X
    CWD
    IDIV BX
    MOV Result, AX

    ; 退出程序
    MOV AX, 4C00h
    INT 21h
MAIN ENDP
END MAIN