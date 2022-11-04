## 概述


## IO模型
- 阻塞IO
- 非阻塞式IO
- I/O复用
- 信号驱动式IO
- 异步IO （AIO）


### 阻塞IO
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-16-08-46-11.png
width=450px>
</center>

- 等待这段时间是阻塞的（睡眠），系统剥夺了cpu的控制权，让它一直等待着。指定数据准备好或者发生错误才返回
  - 我理解的是wait 仍然是需要while的。只不过被阻塞不消耗cpu资源的while？


### 非阻塞式IO模型
- EPOLL中ET边沿触发模式必须用非阻塞fd
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-16-08-57-57.png
  width=450px>
  </center>

  - 等待这段时间，不把线程或者进程睡眠，而是一直系统调用（浪费资源），如果没准备好就返回EWOULDBLOCK错误
  - 这种称为轮询（polling），持续轮询内核，查看某个操作是否就绪，耗费大量CPU时间


### IO复用模型
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-16-09-00-57.png
width=450px>
</center>
  
- 效率上没有什么优势，多了一个select的系统调用。优势在于可以监听多个描述符
- 就好像select帮recvfrom阻塞了（前提select中加入了这个描述符），同时可能不止帮了它一个IO操作或者一个套接字。哪个操作好了，这时select就会返回可读事件，操作（recvfrom）就会系统调用去取数据。
  - 返回可读条件后 select还会按这个继续阻塞吗？还是需要重新添加事件，使用FD_SET在循环中添加事件

### 信号驱动式IO
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-16-09-15-22.png
width=450px>
</center>

- 优势在于等待数据报到达期间进程可以不被阻塞。主循环可以继续执行，只要等待来自信号处理的通知。
### 异步IO模型
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-16-09-18-30.png
width=450px>
</center>

### 各种IO模型的对比
- 我们发现前四种都是第一阶段不同，第二阶段相同：内核赋值到调用者缓冲区期间，进程阻塞于recvfrom调用
- 异步IO在这两个结点都要处理，而没有阻塞


### 同步IO与异步IO
- 可以推断前四种IO模型都是同步IO：导致进程阻塞直到IO操作完成
- 异步IO：不导致请求阻塞

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-16-09-23-05.png
width=450px>
</center>



## select函数
- 该函数指示内核等待多个事件任何一个发生，并且只有在一个或者多个事件发生或经历一段指定时间后才唤醒它

- 例如:调用select告知内核仅在下列情况发生时才返回，可以用于任何描述符
  - 集合{1,4,5}中任何**描述符**准备好**读**（事件event）
  - 集合{2，7}中任何**描述符**准备好**写**（事件event）
  - 集合{1,4}中任何**描述符**有任何异常条件待处理（事件event）
  - 已经历3秒

```c
#include<sys/select.h>
#include<sys/time.h>

int select(int maxfdp1,fd_set *readset,fd_set *writeset,fd_set *exceptset,const struct timeval *timeout);

//用于指定秒数和微秒数
struct timeval {
    long    tv_sec;         /* seconds */
    long    tv_usec;        /* microseconds */
};


void FD_CLR(int fd, fd_set *set); //关闭描述符
int  FD_ISSET(int fd, fd_set *set); //检查是否就绪
void FD_SET(int fd, fd_set *set); //将描述符加入fd_set 打开描述符
void FD_ZERO(fd_set *set); //初始化所有位

select()有描述符数量限制can monitor only file descriptors numbers that are less than  FD_SET‐
       SIZE; poll(2) does not have this limitation.  See BUGS.

typedef struct
  {
#ifdef __USE_XOPEN
    __fd_mask fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->fds_bits)
#else
    __fd_mask __fds_bits[__FD_SETSIZE / __NFDBITS];
# define __FDS_BITS(set) ((set)->__fds_bits)
#endif
  } fd_set;

```

- timeout为NULL：永远等待，直到事件发生。timeval中指定秒数和微秒数；**为0表示**检查描述符后立即返回，称为轮询；
  - 前面两种情形通常会被进程在等待期间捕获的信号**中断**，并从信号处理函数返回
  - 可以指定SA_RESTART标志 自动重启中断的select。做好select返回EINTR的准备

- fd_set结构体，其中包含一个整数数组（例如32位整数，这时该数组第一个元素就代表0-31个描述符，第二个元素代表32-63，依次类推。。。），整数数组的一位就代表一个描述符（poll是一个可变长度的结构数组，每一个结构代表一个描述符）
- maxfdp1：指定待测试的描述符个数，它的值是待测试最大描述符+1;最大描述符（比如打开1，4，5描述符）这时最大描述符个数为6，因为描述符从0开始。因此在循环中测试描述符0，1，2...maxfdspl-1均被测试。可能不需要使用FD_SETSIZE这么多个描述符
- void FD_ZERO(fd_set *set); //初始化所有位 初始化非常重要
- void FD_SET(int fd, fd_set *set); //将描述符加入fd_set 打开描述符 **每一次在循环中都要重新加入描述符！！！系统调用**
- 使用 FD_ISSET 测试描述符是否就绪

```c
int maxfdp1;
fd_set rset;
fd_set wset;
FD_ZERO(&rset);
// FD_SET(0,&rset);//描述符0加入
// FD_SET(1,&rset);//描述符1加入;
// FD_SET(listenfd,&rset);//listenfd加入
maxfdp1 = lisenfd + 1;
for(;;)
{
    //实际上每次都要加入
    FD_SET(0,&rset);//描述符0加入
    FD_SET(1,&wset);//描述符1加入;
    FD_SET(listenfd,&rset);//listenfd加入

    select(maxfdp1,&rset,&wset,NULL,NULL);

    if(FD_ISSET(0,&rset))
    {

    }
    if(FD_ISSET(1,&wset)) //写事件
    {
        
    }

}
```
### 描述符就绪条件

- **select对fd的监听其实就是监听fd的缓冲区**
- select就认为该fd可读就绪，当缓冲中有数据待写的时候，select就认为该fd可写就绪
- 满足下面四个条件之一，一个套接字准备好读 
  - 该套接字接收缓冲区中的数据字节数大于等于套接字接收缓冲区低水位标记的当前大小
  - 该链接的读半部关闭（接收了FIN的连接） 不阻塞返回0（EOF）
  - 监听套接字且已完成的连接数不为0
  - 其上有一个套接字错误待处理
- 一个套接字准备好写
  - 发送缓冲区可用字节数大于等于套接字发送缓冲区低水位标记的当前大小，并且已连接（或者不需要连接UDP）
  - 该连接的写半部关闭，对这样的套接字写会发送SIGPIPE信号
  - 使用非阻塞式Connection的套接字已建立连接，或者connect已经以失败告终
  - 其上有一个错误待处理
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-17-08-45-51.png
  width=450px>
  </center>

```c

缓冲区为2时，char* buf[2]，可以发现一次读2个，由于stdin有3个字符，所以就绪了2次
ddd
select ready
select ready
dd
select ready
select ready
d
select ready

（1） 输入一个字符’s’,s被放入缓冲区，同时放入缓冲区中的还有换行符’\n’,缓冲区中有待读数据，select返回读就绪，read将’s\n’读出，缓冲区清空，select再次阻塞。

（2） 输入两个”dd”，缓冲区中的数据变为”dd\n”，select返回读就绪，read读出两个字符——”ss”，输出hello world！此时缓冲区中还有’\n’，所以下一次select依然返回读就绪，之后read将’\n’读出，输出hello world！，缓冲区清空，select阻塞。
```
  
### select最大描述符
- __FD_SETSIZE,如果想改变只能先改变，然后重新编译内核



### str_cli函数之缓冲区，shutdown函数

- 改用read 和 write对缓冲区而不是对文本行操作，避免错误
- 把文件流指针转换成文件描述符用fileno函数，其原型为：
int fileno(FILE *stream);
- 碰到EOF时 新标志stdineof设为1，为SHUT_WR使用shutdown发送FIN




## 6.8 回射服务器程序 p139
- 理解服务器上的多个描述符，才能理解IO复用
- 
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-17-09-56-00.png
  width=450px>
  </center>
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-17-09-56-25.png
  width=450px>
  </center>

  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-17-09-56-50.png
  width=450px>
  </center>
  
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-17-09-57-25.png
  width=450px>
  </center>
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-17-09-57-44.png
  width=450px>
  </center>
  


## poll函数

```c
#include<poll.h>
int poll(struct pollfd *fdarray,unsigned long nfds,int timeout);

struct pollfd {
      int   fd;         /* file descriptor */
      short events;     /* requested events */
      short revents;    /* returned events */
  };
```
- 第一个参数是指向一个结构数组的第一个元素的指针，每个数组元素都是一个pollfd结构（类似于链表，大小没有限制）
- 每一个结构体代表一个描述符
- 每个描述符有两个变量，一个未调用值，一个为返回结果，避免了使用**值-结果参数（就不用每次都需要重新添加描述符）select中三个参数都是值-结果参数**

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-07-17-10-23-15.png
width=450px>
</center>
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-07-17-10-30-03.png
width=450px>
</center>

- tcp服务器实现 - poll