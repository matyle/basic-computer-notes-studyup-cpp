data SEGMENT;定义一个数据段
    hello DB 'Hello world'$
data ENDS
code SEGMENT;定义一个代码段

start:
    MOV AX,data;data段首地址赋给ax
    MOV DS,AX ;ax赋给ds，ds指向data段
    LEA DX,hello;dx指向hello首地址
    MOV AH,09h;
    INT 21h;
    MOV AX 4C200h
    INT 21h;
code ENDS
END start

