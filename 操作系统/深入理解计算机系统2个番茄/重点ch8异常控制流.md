# 异常控制流-应用是如何与系统交互

- 从处理器加电开始，直到断电，程序计数器假设一个序列的值$a_0,a_1...a_{n-1}$
  - 其中$a_k$是某个指令$I_k$的地址。从$a_k$到$a_{k+1}$过渡称为控制转移，控制转移序列叫做处理器的控制流
  - $a_k$到$a_{k+1}$在存储器相邻则是平滑的，反之是平滑的突变
- 现代系统通过使控制流发生突变来对这些情况做出反应，我们称为ECF 异常控制流（exceptional control flow）。
  - ECF是操作系统实现IO，进程和虚拟内存的基本机制
  - 应用程序通过一个叫陷阱或者系统调用的ECF向系统请求服务。


## 异常
- 异常是一种形式的异常控制流，分别由硬件和操作系统实现
- 异常就是控制流中的突变，用来相应处理器状态中的某些变化
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-10-29-29.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>

- 状态变化被称为事件（event），事件可能与当前指令直接相关，如除以0，内存缺页等；也可能与指令无关，例如定时器产生一个信号
- 有事件发生时通过一张异常跳转表进行间接异常调用 到异常处理程序（操作系统子程序）
- 根据异常事件的类型，会发生三种情况的一种
  - 返回到当前指令
  - 返回到吓一跳指令
  - 处理程序终止中断的程序

### 异常处理
- 每种类型都有一个非负整数的异常号，根据异常号跳转到相应的异常处理程序
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-10-39-30.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>

  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-10-42-05.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
  ### 异常的类别
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-10-44-17.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
  - 中断 异步：不由任何一条指令造成
    - io设备等通过向处理器芯片一个管角发送信号，处理器检测到中断管脚电压变高，从系统总线读取异常号，然后调用适当的中断处理程序
  - 陷阱：有意的异常，是执行一条指令的结果。
    - 最重要的作用是系统调用 syscall n；例如读文件 创建新进程（fork），加载新程序（execve），终止当前进程exit
    - 系统调用发生在**内核模式**，而普通函数调用发生在**用户模式**
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-10-55-43.png
  width=500px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
- 故障：由错误情况引起，可能被故障处理程序修复
  - 如缺页异常：虚拟地址对应的物理页面不在内存中，需要从磁盘取出。（并没有把程序所有的数据和代码加载如内存）
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-10-59-47.png
  width=500px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>

  
- 终止：不可回复的致命错误造成的结果
  - 硬件错误：DRAM或者SRAM位被损坏发生的奇偶错误。

### intel处理器中的异常
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-26-11-06-05.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- 0 除法错误除以0:**浮点异常 Floating exception**
- 13：一般保护故障：引用了未定义的虚拟内存区域或者写一个只读的文本段 Unix中称为 **段故障（Segmentation fault）**
- 18:硬件错误
- 系统调用使用IA32系统上指令是 INT n；其中n可能是异常表中256个表目中任何一个索引

## 进程

- 异常提供基本的构造块，允许操作系统提供进程的概念，进程是计算机科学最成功最深刻的概念之一
- 假象：程序独占处理器和内存，好像处理器无间断一条接一条执行指令。好像我们程序的代码和数据是内存中唯一的对象
- 系统中每个程序都运行在某个进程的上下文中context
  - context由正确运行所需的状态组成包括：**内存中的代码和数据，栈，通用寄存器内容，程序计数器pc，环境变量，以及打开的文件描述符的集合**


### 逻辑控制流

- 一系列的PC的值（当前正在执行的指令的地址，取地址）唯一对应于我们程序的可执行目标文件的指令或是包含在运行时动态连接到我们程序共享对象中的指令。
- PC的值序列叫做逻辑控制流
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-26-11-25-53.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- 不同进程的逻辑控制流与其他控制流独立，除非使用进程通信会有例外：管道pipe，套接字，共享内存，信号量，信号等，显式与其他进程交互时不独立
- 任何逻辑流在时间上与另外的逻辑流重叠我们称为并发进程
  - 如A和B，A和C都是并发进程，B与C不是，因为C开始时，B已经结束
- 进程与其他进程轮换运行的概念称为多任务。一个进程执行的控制流的一部分称为时间片。

### 私有地址空间

- 假象：进程好像正在独占系统地址空间
- n位地址的机器，地址空间是$2^n$个可能地址的集合$0,1...2^n-1$;
- 一个进程为每个程序提供一个私有地址空间，一般而言这个空间对应的内存其他程序不能读写
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-11-35-15.png
  width=400px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">、</div>
  </center>

- 地址空间底部3/4是预留给应用程序的，顶部1/4是预留给内核的
  - 且执行程序都是从0x08048000

### 用户模式和内核模式

- 方式位（mode bit）
  - 设置方式位：进程运行在内核模式（也叫超级用户模式）
  - 未设置方式位：进程运行在用户模式，用户模式不允许执行特权指令，如停止处理器，改变方式位，或者io操作；无法直接访问内核区的代码和数据，可以通过系统调用访问


### 上下文切换
- 操作系统内核用一种称为**上下文切换**的较高级形式的异常控制流来实现多任务
- 内核为每个进程维护一个上下文，上下文就是一个**被抢占进程所需的状态：由一些对象值组成，这些对象包括通用目的寄存器，浮点寄存器，程序计数器，用户栈，状态寄存器，内核栈和各种内核数据结构**，如描述地址空间的页表，包含有关当前进程信息的进程表，包含进程已打开文件的文件表
- 调度：抢占当前进程，被重新开始一个先前被抢占的进程，由内核中的代码调度器处理
- 上下文切换
  - 1.保存当前上下文
  - 恢复先前被抢占进程所保存的上下文
  - 将控制传递给新恢复的进程
  - 系统调用，中断都会发生上下文切换
  - 上下文切换和中断会导致高速缓存是冷的：程序所需要的数据不在缓存中
    - 因为中断处理程序也是进程实际上也会往高速缓存中读取写入数据，因此切换回来的时候原来程序的数据也已经不在高速缓存中了


### 系统调用和错误处理
- man syscall Linux中提供了160个系统调用
- 系统级函数可以通过传递参数进行系统调用
- 错误处理，全局整数变量errno表示出了什么错
  ```c
  pid_t pid;
  if((pid = fork())<0){
      fprintf(stderr,"%s:%s",msg,strerror(errno));
  }
  ```

- 错误处理包装（error-handling wrapping)函数 将创建进程代码减为1行
  ```c
  pid_t Fork(void){
    pid_t pid;
    if((pid = fork())<0){
    fprintf(stderr,"%s:%s",msg,strerror(errno));
    return pid;
  }
  }
  ```

  ## 进程控制

- 获取进程ID函数 `getpid()`返回调用进程的pid，`getppid()`返回父进程的pid
  ```c
  #include<unistd.h>
  #include<sys/types.h> 
  pid_t getpid();//返回调用进程的pid
  pid_t getppid();//返回父进程pid

  ```

- 创建与终止进程
  - 运行：要么在cpu上面执行，要么在等待被执行（就绪）
  - 暂停：进程的执行被挂起（阻塞） 收到信号SIGSTOP,SIGTSTP,SIDTTIN,SIGTTOU信号进程会被暂停，直到收到一个SIGCONT
  - 终止
  ```c
  #include<stdlib.h>
  void exit(int status);
  pid_t fork();//
  ```
  - 父进程通过调用fork创建子进程，新创建的子进程几乎但不完全和父进程相同。
    - 子进程得到与父进程用户级虚拟地址空间相同但独立的拷贝包括：文本、数据和bss段，堆以及用户栈。还能获得父进程任何打开的文件描述符fd。
    - 最大的区别是有不同的pid
  - fork调用一次返回2次：一次返回在父进程中返回子进程pid，一次是在创建的子进程中返回0。因为子进程的pid总是非0的，返回值就提供一个明确的方法分辨程序是在父进程还是子进程中执行的。
  ```c
  #include "csapp.h"
  #include<stdlib.h>
  int main(){
      pid_t pid;
      int x = 1;
      pid = fork():
      if(pid==0){ //子进程中 如果不exit(0)退出 还会执行下面的代码因为是拷贝
        printf("child: x = %d",++x);
        exit(0);
      }

      printf("parent:x=%d\n",--x);
      exit(0);
  }
    //输出
    //parent:x=0
    //child: x = 2
  ```

  - 从结果可以看出
    - 调用一次返回两次
    - 并发执行:理解并发的执行！！！！例如main函数中调用doit doit中有2个fork 则会并行执行
    ```c
        void doit(){
        fork(); //第一个fork
        fork();
        printf("hello\n");
        return;
    }
    int main(){
        doit();
        printf("hello world\n");
        exit(0);
    }

    /*
    输出
    hello
    hello world
    hello
    hello world
    hello
    hello world
    hello
    hello world
    */
    ```
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-27-10-41-23.png
    width=600px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;"></div>
    </center>
    
    - 相同但独立的地址空间，x是拷贝所以结果没有影响,因此会执行父进程中所有代码
    - 共享文件


- 回收子进程
  - 当一个进程被某种原因终止后，内核并不会把它立即从系统中清除。而是保持在终止状态中，直到被它的父进程回收。
  - 父进程回收子进程之后，内核将子进程的退出状态传递给父进程，然后抛弃，这时该进程就不存在了。
  - 若终止而没有回收的进程 称之为僵尸进程
  - 若父进程在没有回收僵尸进程之前就终止了，会安排init进程来回收他们
  ```c
  #include<sys/types.h>
  #include<sys/wait.h>
  pid_t waitpid(pid_t pid,int *status,int options);
  ```
  - 默认option=0，waitpid挂起调用进程的执行，直到它等待集合中的一个子进程终止
  - 若它等待集合中一个进程在刚调用的时候就已经终止，那么waitpid立即返回。
    - 判断等待集合的成员
    - pid>0 等待集合就是一个单独的子进程，进程ID等于pid
    - pid=-1表示等待集合就是由父进程所有子进程组成的
  - 修改默认行为
    - WNOHANG：立即返回
    - WUNTRACED：挂起调用进程的执行
  - status
  - 
  - 错误条件：

- 让进程休眠
  ```c
  #include<unistd.h>
  unsigned int sleep(unsigned int secs); //参数是休眠时长
  //返回0 说明请求时间量到了，或者返回剩下要休眠的秒数。（如sleep函数被一个信号中断过早返回时）

  int pause(void);//使调用函数休眠 直到进程收到一个信号为止
  ```

- 加载并运行程序
```c
#include<unistd.h>
int execve(char* filename, char* argv[],char* envp)//参数列表argv，环境变量列表envp

//操作环境数组
#include<stdlib.h>
char *getenv(const char* name);//存在则返回执行名字的指针，否则返回NULL

int setenv(const char *name,const char *newvalue,int overwrite);

void unsetenv(const char *name); //删除环境名为name的字符串
```
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src= img/2021-05-27-11-19-27.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>

  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-27-11-20-20.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-28-09-17-33.png
  width=500px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>


- 程序是代码和数据的集合；而进程是动态的是程序执行的实例，就好像fork调用之后的子进程是父进程的一个拷贝，其实拷贝的指令是父进程执行到fork之后，比如子进程就无法重新运行fork之前的代码，所以并不是把整个程序拷贝给了子进程
- 进程是执行中程序的一个特殊实例；程序总是运行在某个进程的上下文中。
- 使用execve函数在当前进程的上下文中运行新的程序，会覆盖当前进程的地址空间，**但是这并没有创建新的进程**。**虽然是新的程序，但是任然是一个进程，有相同的pid，并继承了调用execve函数时打开的文件描述符**

### 利用fork和execve运行程序

- 实现shellex.c [实现shellex.c ](/Users/matytan/Desktop/code/基础/操作系统/深入理解计算机系统2个番茄/codeforop/ch8/shellex.c
)

## 信号
- 更高层形式的异常，unix信号，运行进程中断其他进程
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-28-11-06-15.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

### 信号术语

- 发送信号 ：内核通过更新目的进程的上下文中的某个状态，发送一个信号给目的进程。可发送给自己
  - 1.检测到系统事件
  - 2.调用了kill函数
- 接收信号：目的进程被内核强迫以某种方式对信号发送做出反应时，目的进程就接收了这个信号。
  - 可以忽略信号，也可以通过执行信号处理程序的用户层函数捕获这个信号
- 一个只发出没有被接收的信号叫**待处理信号** **一个类型最多只会有一个待处理信号**，如果一个进程已经有了一个类型k的待处理信号之后，那么接下来发送到这个进程的类型为k的信号都不会排队等待，只会简单被丢弃
- 一个进程可以选择**阻塞**接收某种信号：被阻塞之后啊，产生的待处理信号可以发送，但是不会被接收
- **一个带出来信号最多只能被接收一次**，内核中为每个进程在pending位向量中维护着待处理信号集合，在block位向量中维护被阻塞的信号集合。即只要一个类型为k的信号被发送，内核就在pending位向量中设置第k个位，如果类型为k的信号被接收，就清除第k个位

### 发送信号

- 进程组 process group
  ```cpp
  //返回进程组ID 正整数
  #include<unistd.h>
  pid_t getpgrp(void);

  //默认子进程和父进程属于同一个进程组 setpgid改变自己或其他进程的进程组
  pid_t setpgid(pid_t pid,pid_t pgid);//将进程的pid进程组改为pgid，如果pid是0，就使用当前进程的PID。
  //如果pgid是0 那么就用pid指定的进程的PID作为进程组ID
  //例如调用进程是15213，setpgid(0,0)会创建一个新的进程组，进程组ID是15213,并把进程15213加入进程组
  ```
- 用kill程序发送信号
  - `kill -9 15213`发送信号9（SIGKILL）给15213
  - 负的PID会导致信号被发送到PID进程组中每个进程`kill -9 -15213`

- 从键盘发送信号
  - shell使用作业抽象概念来表示求值一条命令而产生的进程 `ls | sort` 由两个进程组成的前台作业，通过管道连接
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-29-11-01-54.png
  width=400px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
- kill函数发送信号
  ```cpp
  #include<sys/types.h>
  #include<signal.h>

  int kill(pid_t pid,int sig);
  //pid大于0则发送sig给进程pid，若小于0，则发送信号sig给abs（pid）的进程组
  ```

- 用alarm函数发送信号SIGALRM 在sec秒内发送一个SIGALRM信号给调用进程。
  ```cpp
  #include<unistd.h>
  unsigned int alarm(unsigned int secs);
  ```

  ```cpp
  //信号处理函数
  #include "csapp.h"

    void handler(int sig){
        static int beeps = 0;
        printf("BEEP\n");
        if(++beeps<5)
            Alarm(1); //调用alarm
        else{
            printf("BOOM\n");
            exit(0);
        }
    }

    int main(){
        Signal(SIGALRM,handler);//信号处理
        Alarm(1);
        while (1)
        {
            ; //信号处理函数每次在循环中返回
        }
        exit(0);
        
    }
  ```


### 接收信号

- 当内核从异常处理程序返回，准备将控制传递给进程p时，它会检查未被阻塞的待处理信号的集合（pending&~blocked)。集合为空，内核将控制传递给p的逻辑控制流中的下一条指令$I_{next}$

```cpp
//修改默认行为
#include<signal.h>
typedef void handler_t(int)
handler_t *signal(int signum,handler_t *handler);

// handler是SIG_IGN 忽略信号
// handler是SIG_DFL 类型为signum的恢复默认行为
// handler是自定义函数的地址，称为信号处理程序

//发送ctrl-c shell发送的SIGINT信号
```


### 信号处理问题
- 捕捉多个信号 时存在细微问题
- 待处理信号被阻塞
- 待处理信号不会排队等待：信号不可以用来对其他进程中发生对事件计数。
- 系统调用可以被中断


### 可移植的信号处理
- sigaction显式定义指定他们想要的信号处理语义
- 包装函数Signal调用sigaction，Signal的定义
  - 只有这个处理程序当前正在处理那种类型的信号被阻塞
  - 和所有信号一样，，信号不会排队
  - 只要可能，被中断的系统调用会自动重启
  - 一旦设置来信号处理程序，就会一直保持，指定Signal带着handler参数SIG——IGN或SIG_DFL
```c
#include<signal.h>
int sigaction(int sigsum,struct sigaction* act,struct sigaction *oldact);

```

### 显式的阻塞信号
```c
#include<signal.h>
int sigprocmask(int how,const sigset_t *set sigset_t *oldset);
int sigemptyset(sigset_t *set);
int sigfillset(sigset_t *set);
int sigaddset(sigset_t *set,int signum);
int sigdelset(sigset_t *set,int signum);
int sigsimember(const sigset_t *set,int signum);
```
- `sigprocmask` 的具体行为依靠how值
  - `SIG_BLOCK`:添加set中的信号到block中
  - `SIG_UNBLOCK：从block中删除set中的信号`
  - `SIG_SETMASk`:block=set
  - 如果oldset非空，那么之前的blocked位向量的值会保存爱oldset中
- `sigemptyset`初始化set为空集
- `sigfillset` 将每个信号添加到set中
- `sigaddset` 函数添加signum到set


### 非本地跳转
- 用户级异常控制流，将控制直接从一个函数转移到另一个当前正在执行的函数，不需要正常的调用返回序列。
- 类似于catch throw
```c
#include<setjmp.h>

int setjmp(jmp_buf env);
int sigsetjmp(sigjmp_buf env,int savesigs);
//setjmp在env缓冲区保存当前栈的内容，供kongjmp使用，类似于catch
void longjmp(jmp_buf env,int retval);
void siglongjmp(sigjmp_buf env,int savesigs,int retval);
//longjmp从env缓冲区中恢复栈的内容,类似于throw
```
