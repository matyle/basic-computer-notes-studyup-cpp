## 第一层 显式规则
目标:依赖
[tab] 指令
伪目标：.PHONY:
递归规则
```makefile
hello : hello.o
    gcc hello.o -o hello
hello.o : hello.s
    gcc -c hello.s -o hello.o
hello.s : hello.i
    gcc -S hello.i -o hello.s
hello.i : hello.i
    gcc -E hello.c -o hello.i

.PHONY:
clearall:
    rm -rf hello.o hello.s hello.i hello
clear:
    rm -rf hello.o hello.s hello hello.i
```

多个文件的makefile
目标：依赖
[tab] gcc
```makefile
test:circle.o cube.o main.o
    gcc circle.o cube.o main.o -o test

circle.o:circle.c
    gcc -c circle.c -o circle.o
cube.o : cube.c
    gcc -c cube.c -o circle.o
main.o : main.c
    gcc -c main.c -o main.o

.PHONY:
clearall:
    rm -rf circle.o cube.o main.o test
clear:
    rm -rf circle.o cube.o main.o

```

## 变量 + 隐藏规则

- `=` 替换（赋值）
- `+=` 追加
- `:=` 恒等

`%.c,%.o` 任意的.c文件
`*.c,.c` 所有的.c 文件
`$` 符号表示取变量

```makefile
TAR = test #目标文件
OBJ = circle.o cube.o main.o
CC := gcc
CFLAG = -o -Wall

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

```

## 通配符 
`$^` 所有依赖文件
`$@` 所有目标文件
`$<` 第一个目标文件
## 实践

```c
//注意是#ifndef!!! cube.h文件 
#ifndef CUBE_H
#define CUBE_H
#include<stdio.h>
void cube();
#endif

//cube.c
#include "cube.h"
void cube()
{
    printf("cube");
}
//circle.h
#ifndef Circle_H
#define Circle_H
#include<stdio.h>
void circle();
#endif

//circle.c

#include<stdio.h>
#include "circle.h"
void circle()
{
    printf("s= PI * r^2\n");
    printf("c= 2 * PI * r\n");
}

//main.c
include "circle.h"
#include "cube.h"
int main()
{
    cube();
    circle();
    return 0;
}
```

执行make
```bash
(base) matytan@matytandeMacBook-Pro maketest % make      
gcc    -c -o circle.o circle.c
gcc    -c -o cube.o cube.c
gcc    -c -o main.o main.c
gcc circle.o cube.o main.o -o test ;
(base) matytan@matytandeMacBook-Pro maketest % ./test
```
