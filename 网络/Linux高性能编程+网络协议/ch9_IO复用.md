### IO复用

- 使程序能同时监听多个描述符
#### 9.1 select系统调用

  在一段指定时间内，监听用户感兴趣的文件描述符上的可读，可写，异常等事件。
  - select API
  ```c
  #include<sys/select.h>
  int select(int nfds，fd_set* readfds,fd_set* writefds,fd_set* exceptfds,
  struct timeval* timeout);
  //ndfs指定最多能监听fd的数量

  typedef struct{
      #ifdef _USE_XOPEN
      _fd_mask fds_bits[_FD_SETSIZE/_NFDBITS];
      #define _FDS_BITS(set) ((set)->fds_bits);

      #else
      _fd_mask fds_bits[_FD_SETSIZE/_NFDBITS];
      #define _FDS_BITS(set) ((set)->fds_bits);

      #endif


  } fd_set;

  //位操作过于频繁，因此用宏来访问结构体中的位

  #include<sys/select.h>
  FD_ZERO(fd_set *fdset); //清除fdset所有位
  FD_SET(int fd,fd_set *fd_set);//设置所有的位fd
  FD_CLR(int fd,fd_set *fd_set);//清除fdset的位
  int FD_SET(int fd,fd_set *fd_set);//fdset的位fd是否被设置

  ```
    - timeout为超时时间 timeval的结构指针；timeout为null则select阻塞
        ```c
            struct timeval{
                long tv_sec;//秒数
                long tv_usec;//微秒数
            }
        ```
  - 文件描述符就绪条件:哪些情况的文件描述符被认为是可读可写，异常，对于select的使用非常关键
    网络编程中
    - 下列情况表示socket可读
      - socket内核接收缓冲区字节数大于等于其低水位标记SO_RCVLOWAT。此时可以无阻塞读取socket，返回大于0的字节数
      - socket通信对方关闭连接，此时socket读操作返回0
      - 监听socket有新请求
      - 有未处理的错误，getsockopt读取和清除该错误
    - 下列情况可写
      - socket内核发送缓冲区字节数大于等于其低水位标记SO_RCVLOWAT。此时可以无阻塞写该socket，返回大于0的字节数
      - 写操作被关闭
      - socket使用非阻塞的connect成功或者失败超时之后
      - 有未处理的错误，getsockopt读取和清除该错误
  - 处理带外数据
    - select能处理的异常只有一种：socket上接收到外带数据
  ```c
  //代码9-1

  const char *ip =argv[1];
  int port = argv[2];
  //地址结构sockaddr_in
  struct sockeaddr_in address;
  bzreo(&address,sizeof(address));
  address.sin_family = AF_INET; //
  inet_pton(AF_INET,ip,&address.sin_addr) //ip
  address.sin_port = hton(port);
  //建立socket
  sockfd  = sockket(AF_INET,SOCK_STREAM,0);
  //bind
  //liseten
  //accpect

  ```

#### 9.2 poll系统调用
- poll系统调用与select类似，在指定时间内轮询一定数量的文件描述符，测试其中是否有就绪者。
```c
#include<poll.h>
int poll(strcut pollfd* fds,nfds_t nfds,int timeout);

struct pollfd
  {
    int fd;			/* File descriptor to poll.  */
    short int events;		/* Types of events poller cares about.  */
    short int revents;		/* Types of events that actually occurred.内核通知  */
  };
```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-17-15-05-02.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>


#### 9.3 epoll系列系统调用

- 9.3.1 内核事件表
  - epoll是Linux特有的io复用函数。使用一组函数来完成任务
  - 用户关心的事件描述符放在内核列表中
  - epoll的事件宏在上面poll事件上 加一个E，如epoll的可读事件是EPOLLIN
  - epoll通过epoll_create建立一棵红黑树，通过epoll_ctl将要监听的文件描述符注册到红黑树上
  - epoll_create时，内核除了帮我们在epoll文件系统里建了个红黑树用于存储以后epoll_ctl传来的fd外，还会再建立一个list链表，用于存储准备就绪的事件，当epoll_wait调用时，仅仅观察这个list链表里有没有数据即可。
  - epoll是根据每个fd上面的回调函数(中断函数)判断，只有发生了事件的socket才会主动的去调用 callback函数，其他空闲状态socket则不会，若是就绪事件，插入list
  ```c
  #include<sys/epoll.h>
  int epoll_create(int size);
  //操作epoll内核事件表
  int epoll_ctl(int epfd，int op,int fd,struct epoll_event *event);
  //op参数表示操作类型，有三种
  //EPOLL_CLT_ADD 注册fd上的事件
  //EPOLL_CLT_MOD 修改fd上的注册事件
  //EPOLL_CLT_DEL//删除fd上的注册事件
  
    struct epoll_event
    {
    uint32_t events;	/* Epoll events */
    epoll_data_t data;	/* User data variable */
    } __EPOLL_PACKED;
    //epoll_data_t 是一个联合体无法同时使用ptr和fd
    typedef union epoll_data
    {
    void *ptr; //指定与fd相关的用户数据
    int fd;
    uint32_t u32;
    uint64_t u64;
    } epoll_data_t;
  
  ```

  - epoll_wait函数
    - epoll系列系统调用主要接口是epoll_wait函数
    ```c
    #include<sys/epoll.h>
    int epoll_wait(int epfd,truct epoll_event *event,int maxevents,int timeout);
    //epfd 指定内核事件表
    //timeout=-1表示永久阻塞
    //maxevent 最多监听多少事件
    9-2
        struct pollfd* fds;
    int ret = poll(fds,MAX_EVENT_NUMBER,-1);
    
    for(int i=0;i<MAX_EVENT_NUMBER;++i){
        if(fds[i].revents & POLLIN){ //如果就绪
            int sockfd = fds[i].fd;
        }
    }
    //索引epoll
    struct epoll_event *events;
    int epollfd = 0;
    int ret = epoll_wait(epollfd,events,MAX_EVENT_NUMBER,-1);
    
    for(int i=0;i<ret;++i){ //返回的都是就绪的
        int sockfd = events[i].data.fd;
        }
    }
    ```
- LT和ET模式
  - EPOLLET 
  - LT(Level Trigger,电平触发) 默认
  - ET(Edge Trigger,边沿触发) epoll高效工作模式
  - LT模式下epoll_wait通知应用程序，应用程序可以不立即处理，等到下一次调用时 epoll_wait会再一次通知
  - ET模式必须马上处理，因为后续的epoll_wait不会再通知这一事件
- EPOLLONESHOT事件
  - 我们期望的是一个socket连接在任意时刻都只被一个线程处理，这时可以使用epolloneshot事件
  - 除非使用epoll_ctl重置文件描述符的EPOLLONESHOT，否则操作系统最多触发其上一个可读，可写，异常事件。
  - code9.4

- 三组IO复用函数的比较select poll epoll

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-18-16-43-23.png
width=600px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>


#### 9.5 IO复用的高级应用之一：非阻塞connect
- EINPROGRESS错误，对非阻塞socket调用connect，而连接又没有立即建立时。
- 在这种情况下可以调用select或者poll来监听连接失败的socket上面的可写事件。select，poll等函数返回后，利用getsockopt读取错误码并清除该socket的错误，如果错误码为0则建立成功
- 非阻塞connect方式可以同时发起多个连接并一起等待。


#### 聊天室应用
- 客户端
- 服务端
  - 使用poll同时管理和监听socket和连接socket，使用牺牲空间获取时间
#### 同时处理TCP和UDP服务 回射服务器

  - 从bind系统调用来看，一个socket只能与一个socket地址绑定，即一个socket只能用来监听一个端口。
  - 因此需要创建多个socket，利用epoll


#### 超级服务xinetd

