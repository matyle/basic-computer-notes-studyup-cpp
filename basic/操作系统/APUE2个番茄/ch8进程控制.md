## 进程标识
- 进程ID为0的为交换进程（调度进程）

## 函数fork
```c
#include<unistd.h>
pid_t fork(void);
```
- 调用一次返回两次（实际上是内核像子进程和父进程分别返回）
- 两次返回：父进程中返回的是子进程的pid。子进程返回值为0
  - 返回子进程ID的原因：一个父进程有多个子进程，没有一个函数可以获得其所有子进程的ID
  - 使子进程返回0的原因：一个进程只有一个父进程，所以子进程可以调用getppid获取父进程的ID，而0是内核交换进程独有，因此一个子进程的ID不可能为0（所以只会是本身）

- fork之后 子进程拥有和父进程完全相同的堆，栈，数据段副本（注意，他们不真正共享！其实共享，但是是写时拷贝），他们可以共享一些代码段。
- clone函数可以控制哪些部分父进程和子进程共享

- 注意：fork之后父进程和子进程谁先执行，是不确定的，取决于调度算法。因此我们需要同步算法控制他们的顺序（如信号）

    ```c
    //子进程不影响父进程的值
    #include<apue.h>
    char buf[] = "write to stdout";
    int globvar = 6;
    int main(){
        pid_t pid;
        int var;

        var = 88;

        if(write(STDOUT_FILENO,buf,sizeof(buf)-1)!=sizeof(buf)-1){
            err_sys("write error");
        }
        printf("before fork;\n");

        if((pid = fork())<0){
            err_sys("fork error");
        }else if(pid==0){
            globvar++;
            var++;
            printf("child pid = %ld,glob=%d,var=%d\n",(long)getpid(),globvar,var);
        }
        sleep(2);//父进程休眠两秒
        printf("pid = %ld,glob=%d,var=%d\n",(long)getpid(),globvar,var);
        exit(0);

    }

    FILE* fp = fopen("test","w");
        // if(write(STDOUT_FILENO,buf,sizeof(buf)-1)!=sizeof(buf)-1){
        //     err_sys("write error");
        // }
    fputs(buf,fp);
    ```
    - write中将buf的大小-1是为了防止结束符NULL写出到终端输出
    - `strlen`不包含终止null字节的字符串长度，而`sizeof`包含null结束符长度。区别之二是strlen需要进行一次函数调用，因为缓冲区用已知字符初始化长度固定，因此`sizeof`是在编译期间计算的缓冲区大小
    - fork与IO函数的交互关系:fork之后缓冲区也会拷贝
      - 带缓冲:标准输出是行缓冲，重定向到文件是全缓冲的
      - dup2(int oldfd,int newfd); //若参数newfd已经被程序使用，则系统就会将newfd所指的文件关闭
      - 比如`dup2(fd,STDOUT_FILENO); `关闭`STDOUT_FILENO`之后，拷贝fd给`STDOUT_FILENO`,表示`STDOUT_FILENO`不再指向标准输出文件了，而是指向了fd，因此后面所有的像printf等函数向stdout写的都会写到fd指向文件中。


- 文件共享
  - 子进程拷贝了父进程的文件描述符表（好像执行了dup一样），他们描述符指向的是同一个文件表
  - 父进程和子进程共享同一文件偏移量：例如刚刚的输出到文件，子进程写了之后，父进程会追加到后面，而不是覆盖文件
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-30-15-29-02.png
  width=450px>
  </center>

  - 一般来说 fork之后父进程子进程会各自关闭不再使用的文件描述符，以避免干扰其他进程（网络编程中经常使用）

- 子父进程的区别：
  - fork返回值不同，进程id不同，两个进程的父进程id不同
  - 子进程的tms_utime,tms_stime,tms_cutime,tms_ustime设置为0
  - 子进程不继承父进程的文件锁
  - 子进程未处理的闹钟被清除
  - 子进程未处理的信号集被设置为空集


- fork两种做法:
  - 复制父进程，执行不同代码段
  - 一个进程要执行一个新的程序，fork返回后立即调用exec,称之为spawn


## vfork函数

- 目的是exec一个新的程序，因此不将父进程的地址空间完全复制，因此会改变父进程的变量值，因为不再完全拷贝！子进程在父进程的地址空间中运行
- vfork保证子进程先运行，它调用了exec或者exit之后父进程才能被调度运行
- _exit不执行标准io的冲洗缓冲区工作，
```c
if((pid = vfork())<0){
    err_sys("fork error");
}else if(pid==0){
    globvar++;
    var++;
}
...

pid = 82080,glob=7,var=89
pid = 82079,glob=7,var=89 //父进程的值也被改变，因为子进程借用了父进程的地址空间，没有exec
```


## exit函数
- main函数中执行return 等价于执行exit
- 调用exit
  - 调用终止处理程序atexit
  - 关闭标准IO流等

- 调用_exit或者_EXIT
  - 为进程提供一种无需运行终止处理程序和信号处理程序而终止的方法
  - 在unix中，不进行IO流冲洗

- 最后一个线程从启动例程返回时，进程以终止状态0返回
- 进程的最后一个线程调用pthread_exit函数

- 3 种异常终止
  - 调用abort，产生SIGABRT
  - 当进程接收到某些信号
  - 最后一个线程对“取消cancel”请求做出响应时
- 父进程在子进程结束之前终止了怎么解决？内核会检查所有活动进程，判断是否是正要终止进程的子进程，如果是就将其父进程id设置为1
- 父进程在子进程结束之后终止，父进程如何获取子进程的终止状态，如果子进程完全消失了，父进程是检查不到的。这时可以调用wait或者waitpid得到结束子进程的信息
- 如果不使用wait或者waitpid回收子进程资源，子进程会成为僵尸进程
- 在linux系统中，父进程通常会接收子进程SIGCHLD信号监测子进程是否退出，通过waitpid函数对子进程进行回收。但是实际应用过程中waitpid使用不当，会出现子进程成为僵尸进程。
  - 原因在于，SIGCHLD信号是不可靠信号，不可靠信号在Linux中不进行排队，只是放到一个缓冲区，一旦该信号解除阻塞则会立即被发送一次(不可靠信号会丢失)。
- 任何一个子进程（init除外）在exit()之后，并非马上就消失，而是留下一个称为僵尸进程(Zombie)的数据结构，等待父进程回收。由于SIGCHLD信号的丢失，如果父进程不进行回收处理，子进程就会一直处于僵尸状态。
- 解决的办法有两个，一是在信号处理函数中，以非阻塞的方式调用waitpid，使用while循环，直到waitpid回收完所有等待回收的子进程。如下所示：


## wait和waitpid函数

- 当一个进程正常或者异常终止时，内核向其父进程会发送信号**SIGCHLD信号** 子进程的终止是异步事件，因此这也是内核发送的异步通知。
- 父进程可以选择忽略信号，也可以编写信号处理函数处理
  ```c
  #include<sys/wait.h>
  pid_t wait(int *statloc);
  pid_t waitpid(pid_t,int *statloc,int options);
  ```
  -  `*statloc`参数，如果statloc不是空指针，那么进程终止状态会存放在它指向的单元内
  -  pid的值
     -  pid==-1 等待任一子进程
     -  pid>0 等待进程id等于pid的子进程
     -  pid==0 等待组ID等于进程组ID任一子进程
     -  pid<-1 等待组ID等于pid绝对值任一子进程
  - option参数
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-01-17-05-03.png
  width=450px>
  </center>
  
- 如果我们不要父进程等待子进程终止，也不希望子进程处于僵尸状态直到父进程终止~一个诀窍是调用fork两次（子进程中调用子进程）
- 子进程和父进程中的变量互不干扰！！！ 是父进程的完整拷贝


## waitid
```c
#include<sys/wait.h>
int waitid(idtype_t idtype,id_t id,siginfo_t *infop,int option);
```
- idtype
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-01-17-22-11.png
  width=450px>
  </center>
- siginfo_t 指针
- options：
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-01-17-22-40.png
  width=450px>
  </center>
  
## wait3 wait4

## 竞争条件

- 定义：多个进程或者线程对共享数据进行某种处理，而最后的结果又取决于进程运行的顺序，这称为竞争条件（race condition）
- while循环形式称之为**轮询**
- 使用条件变量或者信号量wait，或者使用信号和管道



## 函数exec
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-02-09-46-45.png
width=490px>
</center>

- 第一个区别是前四个函数第一个参数是*pathname,后三个是filename
  - filename中包含/，视为路径名
  - 否则按PATH环境变量，在指定的目录中搜索可执行文件
- 第二个区别参数表的传递（l表示list，v表示vector）。
- 函数名中字母p表示该函数取filename作为参数。并用PATH环境变量寻址可执行文件
- l表示取一个参数表，v表示该函数取一个argv矢量。e表该函数取envp[]数组
- fexecve 表示第一个参数是fd，使用文件描述符执行文件

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-02-09-55-28.png
width=450px>
</center>




## 更改用户id和更改组ID
- 有效用户：文件拥有者
- 实际用户：我们的用户ID
- 保存的设置哦用户ID
- 特权和访问控制是基于用户ID和组ID的。
  - 增加或减少特权或者访问不允许访问或者设置不允许访问的文件都需要更改ID或组ID
  ```c
  include<unistd.h>
  int setuid(uid_t uid);
  int setgid(uid_t gid);
  ```
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-02-10-04-59.png
  width=450px>
  </center>
  
- setreuid 和 setregid
  - 交换实际用户ID和有效用户ID的值

- seteuid和setegid
  ```c
  #include<unistd.h>
  int seteuid(uid_t uid);
  int setegid(uid_t gid);
  ```
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-02-10-12-27.png
  width=450px>

  </center>

## 解释器文件
- 是一种文本文件。起始行形式
  - #！pathname[optional-argument]

## 函数system

## 进程会计
- 内核写一个会计记录，包括命令名，使用的cpu时间总量，用户ID和组ID

```c
#include<sys/acct.h>

```

## 用户标识

```c
#include<unistd.h>
char *getlogin(void);
```
- 返回指向登录名的指针

## 进程调度
- 优先级调度（nice值）
  - nice值越小，优先级越高。你越友好，你的优先级越低（IO频繁的优先级越高）
  ```c
  #include<unistd.h>
  int nice(int incr);

  #include<sys/resource.h>
  int getpriority(int which,id_t who);

  ```
  - incr参数会增加到nice值上。
  - getpriority函数中which参数
    - PRIO_PROCESS进程
    - PRIO_PGRP表示进程组
    - PRIO_USER表示用户ID


## 进程时间
- 墙上时钟时间
- 用户cpu时间
- 系统cpu时间

```c
#include<sys/times.h>
clock_t times(struct tms *buf);

struct tms{
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;

}
```