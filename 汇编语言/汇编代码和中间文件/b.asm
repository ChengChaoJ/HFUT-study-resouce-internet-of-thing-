.MODEL SMALL
.STACK 100h
.DATA
    message DB 'The X Bit is 1$', 0

.CODE
MAIN PROC
    MOV AX, @DATA
    MOV DS, AX

    ; 假设我们设置 BL 中只有某一位为 1
    MOV BL, 00000001b  ; 只设置最低位为1

    ; 检查 BL 中的位
    ; 使用地址表实现跳转
    TEST BL, 00000001b ; 测试最低位是否为1
    JZ NotSet          ; 如果最低位为0，跳转到 NotSet

    ; 输出消息
    MOV DX, OFFSET message
    MOV AH, 09h        ; DOS服务：输出字符串
    INT 21h            ; 调用 DOS 中断

    JMP ProgramEnd     ; 跳转到程序结束

NotSet:
    ; 可以在这里添加其他处理逻辑
    ; 这里没有输出任何信息，当然可以添加更多。

ProgramEnd:
    ; 程序结束
    MOV AX, 4C00h      ; 退出程序
    INT 21h

MAIN ENDP
END MAIN
