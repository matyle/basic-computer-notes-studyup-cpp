TAR = test #目标文件
OBJ = circle.o cube.o main.o
CC := gcc

$(TAR):$(OBJ)
	$(CC) $(OBJ) -o $(TAR);
# 依赖怎么写？
(%.o):%.c
	$(CC) -c %.c -o -Wall %.o 

.PHONY:
clearall:
	rm -rf $(TAR) $(OBJ)
clear:
	rm -rf $(OBJ)
