# 进程环境

main函数如何被调用，命令行参数如何传递给新程序；存储空间布局，如何分配另外的内存，进程如何使用环境变量....
## main函数

- 内核执行c的时候（使用exce函数），调用main之前先调用一个特殊的启动例程。_start?
- 可执行程序将此启动例程指定为程序启动地址-由链接器设置，链接器由c编译器调用。启动例程从内核取得命令行参数和环境变量值，为调用main函数做好安排

## 进程终止
- 正常终止
  1. 从main返回
  2. 调用exit，_exit,_exit
  3. 最后一个线程从启动例程返回
  4. 从最后一个线程调用pthread_exit()

- 异常终止
  - 调用abort
  - 接到一个信号
  - 最后一个线程对取消请求做出相应

- 从main返回后立即调用exit
  ```c
  exit(main(argc,argv));
  ```

- 退出函数
  ```c
  #include<stdlib.h>
  void exit(int status);
  void _Exit(int status);
  #include<unistd.h>
  void _exit(int status);
  ```

- echo $? 打印终止状态，

- 函数atexit,终止处理程序
  ```c
  #include<stdlib.h>
  int atexit(void (*func)(void));
  ```
  - 参数为一个函数地址，无需传递任何参数。退出时调用func
  - 系统至少支持32个终止处理程序
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-26-11-26-22.png
  width=490px>
  </center>
  
- 内核使程序执行的唯一方法是exec
- 进程自愿终止exit

- atexit每登记一次就会被调用一次
  ```cpp
  #include <stdlib.h>
  static void my_exit1(void);
  static void my_exit2(void);

  int main(void){
    if(atexit(my_exit2)!=0){
          err_sys("cannot register my_exit2");
      }
    if(atexit(my_exit2)!=0){
          err_sys("cannot register my_exit2");
      }
    if(atexit(my_exit1)!=0){
          err_sys("cannot register my_exit1");
      }
      printf("main is done \n");
      return 0;
  }
  static void my_exit1(void){
      printf("1st myexit is done \n");

  }
  static void my_exit2(void){
      printf("2nd myexit is done \n");
  }

  ```


## 命令行参数
```cpp
for(int i=0;i<argc;i++){
    //遍历argv
}
for(int i=0;argv[i]!=NULL;i++)
```


## 环境表
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-26-11-39-19.png
width=490px>
</center>

```c
extern char** environ;
int main(int argc,char** argv,char* envp[]);

//通常使用 7.9节访问特定环境变量
getenv
putenv
```


## C程序的存储空间布局
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-26-11-44-25.png
width=290px>
</center>

- 正文段，只读，由cpu执行的机器指令部分。正文段是共享的，因此频繁的执行的程序在内存中只需要一个副本。从0x08048000开始增长
- 初始化数据段.data
- 未初始化数据段
- 栈 ，栈底从0xc0000000向下开始，局部变量和函数调用时所需保存的地址都保存在这里，以及返回地址，以及调用者的环境信息。然后，最近被调用的函数在栈上为其自动和临时变量分配存储空间
- 堆


## 共享库
- 动态链接与共享库链接
- `gcc -static hello1.c` 阻止使用共享库


## 存储空间分配

- malloc
- calloc：指定长度，并将每一位初始化为0
- realloc

```c
#include<stdlib.h>
void *malloc(size_t size);
void *calloc(size_t nobj,size_t size);
void *realloc(size_t nobj,size_t new_size);
void free(void *ptr);
```

## 环境变量

```c
#include<stdlib.h>
char* getenv(const char *name);//用其取环境变量的值

int putenv(char *str);
int setenv(const char *name,const char* value,int rewrite);
int unsetenv(const char *name);
```
- putenv中str为 name=value的字符串，若name存在则删除原来的定义
- setenv若rewrite非0，则首先删除现有定义。若rewrite=0,则不删除现有定义，不设置新的
- unsetenv删除name的定义 

## 函数setjump和longjmp

- 处理发生很深层嵌套函数调用中的出错情况，跨函数调用
- 比如深层调用，发现错误想返回到main函数

```c
#include<setjump.h>
int setjump(jum_buf env);
void longjump(jmp_buf env,int val); //绕过中间函数的栈帧，直接跳到set那里
```
- 自动变量，寄存器变量和易失变量
  - 在进行longjump之后是否还能回滚到setjump之前的变量?
  - 某些系统是存储在存储器的值还是执行longjump时的值，如果是寄存器上面的值则会是setjump时的值
  - `gcc -O jumptest.c -o jumptest`  执行优化后 局部变量会存储在寄存器，因此值会被设置setjump时的值
  ```c
    in f1:
    i:4
    f3() exec
    -1
    jump after:
    i:3
  ```

- 自动变量（局部变量）的潜在问题
  - 函数返回后，局部变量不应该再被引用！
  - 例如在f函数中定义了一个变量，且定义一个引用或者指针指向它。然后将指针或者引用返回。这是非常危险的
    - 因为我们返回的地址指向的是刚刚f的栈帧区域，这个空间很有可能会被下一被调用的函数的栈帧使用。


## 函数getrlimit和setrlimit
- 查询和设置资源限制
  ```c
  #include<resource.h>
  int getrlimit(int resource,struct rlimit* rlptr);
  int getrlimit(int resource,const struct rlimit* rlptr);

  struct rlimit{
      rlim_t rlim_cur;//当前限制 软限制
      rlim_t rlim_max;//当前限制的最大值 硬   限制

  };

  ```


  #