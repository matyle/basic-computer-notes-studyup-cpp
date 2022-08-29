

ET模式和LT模式



### ET模式

读操作的注意事项

如果一次没有read完缓冲区，下次也不会收到新的通知（除非有新的事件），即**buffer**中已有的数据无机会读出，除非有新的数据再次到达**

对于写操作，主要是因为ET模式下fd通常为非阻塞造成的一个问题——**如何保证将用户要求写的数据写完。**---添加EPOOLOUT事件通知

- **a.** **对于读，只要buffer中还有数据就一直读；**
- **b. ** **对于写，只要buffer还有空间且用户请求写的数据还未写完，就一直写。**

方法一：

(1) 每次读入操作后（read，recv），用户主动epoll_mod IN事件，此时只要该fd的缓冲还有数据可以读，则epoll_wait会返回读就绪。

(2) 每次输出操作后（write，send），用户主动epoll_mod OUT事件，此时只要该该fd的缓冲可以发送数据（发送buffer不满），则epoll_wait就会返回写就绪（有时候采用该机制通知epoll_wai醒过来）。

即，当buffer中有数据可读（即buffer不空）且用户对相应fd进行epoll_mod IN事件时ET模式返回读就绪，当buffer中有可写空间（即buffer不满）且用户对相应fd进行epoll_mod OUT事件时返回写就绪。

```c
while(1)
{
  int num = epoll_wait(epfd,events,maxEvents,-1);//无限阻塞
for(int i=0;i<num;++i)
{
  
  if (events[i].events & EPOLLIN) //如果收到数据，那么进行读入
  {
      cout << "EPOLLIN" << endl;
      sockfd = events[i].data.fd;
      if ((n = read(sockfd, line, MAXLINE)) > 0)

      {
          line[n] = '/0';

          cout << "read " << line << endl;
          if (n == MAXLINE) /问题1

          {
              ev.data.fd = sockfd;
              ev.events = EPOLLIN | EPOLLET;
              epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev); //数据还没读完，重新MOD IN事件
          }

          else
          {

              ev.data.fd = sockfd;
              ev.events = EPOLLIN | EPOLLET;
              epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev); //buffer中的数据已经读取完毕MOD OUT事件
          }
      }

      else if (n == 0)

      {
          close(sockfd);
      }
  }

  else if (events[i].events & EPOLLOUT) // 如果有数据发送
  {

      sockfd = events[i].data.fd;
      write(sockfd, line, n);
      ev.data.fd = sockfd; //设置用于读操作的文件描述符
      ev.events = EPOLLIN | EPOLLET; //设置用于注测的读操作事件
      epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev); //修改sockfd上要处理的事件为EPOLIN
  }

}
  
}
```



方法一的问题是：

1. if (n == MAXLINE) 一定是数据没读完吗？数据只有这么多呢？
2. 每次都要mod重新添加事件，违背了epoll的高效原则

首先思考一下为什么要用ET模式，因为ET模式能够减少epoll_wait等系统调用，而我们在这里每次read后都要MOD IN,之后又要epoll_wait，势必造成效率降低，这不是适得其反吗？

方法二：改用循环

对于读操作，我们循环读，直到返回0或者返回errno=EAGAIN

对于写操作，一直写，直到**直到数据发送完****,** **或者** **errno = EAGAIN** 

```cpp
if (events[i].events & EPOLLIN)
{

    n = 0;
    while ((nread = read(fd, buf + n, BUFSIZ - 1)) > 0)
    {
        n += nread;
    }
    if (nread == -1 && errno != EAGAIN)
    {
        perror("read error");
    }
  	//添加epollout事件
    ev.data.fd = fd;
    ev.events = events[i].events | EPOLLOUT;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev);
}
//写事件
if (events[i].events & EPOLLOUT)

{
    int nwrite, data_size = strlen(buf);
    n = data_size;
    while (n > 0)
    {
        nwrite = write(fd, buf + data_size - n, n);
        if (nwrite < n)
        {
            if (nwrite == -1 && errno != EAGAIN)
            {
                perror("write error");
            }
            break;
        }
        n -= nwrite;
    }
    ev.data.fd = fd;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev); //修改sockfd上要处理的事件为EPOLIN
}
```







为什么ET需要工作在非阻塞：因为它要一直读或者一直写，否则可能阻塞在read上面回不到epoll_wait

 因为ET模式下的读写需要一直读或写直到出错（对于读，当读到的实际字节数小于请求字节数时就可以停止），而如果你的文件描述符如果不是非阻塞的，那这个一直读或一直写势必会在最后一次阻塞。这样就不能在阻塞在epoll_wait上了，造成其他文件描述符的任务饿死。



为什么LT可以工作在阻塞下？

epoll的帮助中提到可以支持阻塞和非阻塞两种，其中特别提到ET只能配合非阻塞使用。
epoll的常见使用方式都是非阻塞的，相信大家没有在实际应用中碰见epoll+阻塞模式socket，下面从理论上分析epoll与阻塞模式socket的使用
epoll的ET模式：ET模式要求对read和write的调用必须不断调用，直到返回EAGAIN，只有这样，下一次epoll_wait才有可能返回EPOLLIN或EPOLLOUT。当socket是阻塞方式的，永远也不可能返回EAGAIN，只会阻塞在read/wite调用上。
epoll的LT模式：LT模式能够支持阻塞模式的socket，在阻塞模式下，当有数据到达，epoll_wait返回EPOLLIN事件，此时的处理中调用read读取数据，请注意，第一次调用read，可以保证socket里面有数据(EPOLLIN事件说明有数据)，read不会阻塞。第二次调用，socket里面有没有数据是不确定的，要是贸然调用，read可能就阻塞了，因此不能进行第二次调用，必须等待epoll_wait再次返回EPOLLIN才能再次read。因此LT模式下的阻塞socket使用就必须read/write一次就转到epoll_wait，这对于网络流量较大的应用效率是相当低的，而且一不小心就会阻塞在某个socket上，因此epoll的LT+阻塞式socket几乎不出现在实际应用中。

- 读操作，如果缓冲区有数据，内核会不断通知
- 写操作,如果缓冲区可写，内核会不断通知（有个问题那么如果sockt一直为空那不是一直通知？）
  - 每次监听完OUT事件，需要移出OUT事件
  - 



**从本质上讲：与**LT相比，ET模型是通过减少系统调用来达到提高并行效率的



LT：水平触发，效率会低于ET触发，尤其在大并发，大流量的情况下。但是LT对代码编写要求比较低，不容易出现问题。LT模式服务编写上的表现是：只要有数据没有被获取，内核就不断通知你，因此不用担心事件丢失的情况。

ET：边缘触发，效率非常高，在并发，大流量的情况下，**会比LT少很多epoll的系统调用**，因此效率高。但是对编程要求高，需要细致的处理每个请求，否则容易发生丢失事件的情况。

关于accept：

采用LT模式下，如果accept调用有返回就可以马上建立当前这个连接了，再epoll_wait等待下次通知，和select一样。

但是对于ET而言，如果accpet调用有返回，除了建立当前这个连接外，不能马上就epoll_wait还需要继续循环accpet，直到返回-1，且errno==EAGAIN，



 使用Linux epoll模型，水平（LT）触发模式，当socket可写时，会不停的触发socket可写的事件，如何处理？

第一种最普遍的方式：

需要向socket写数据的时候才把socket加入epoll，等待可写事件。接受到可写事件后，调用write或者send发送数据。当所有数据都写完后，把socket移出epoll。

这种方式的缺点是，即使发送很少的数据，也要把socket加入epoll，写完后在移出epoll，有一定操作代价。



一种改进的方式：
开始不把socket加入epoll，需要向socket写数据的时候，直接调用write或者send发送数据。如果返回EAGAIN，把socket加入epoll，在epoll的驱动下写数据，全部数据发送完毕后，再移出epoll。

这种方式的优点是：数据不多的时候可以避免epoll的事件处理，提高效率。



实例

一开始是没有添加写事件的

listenfd不是ET模式

在收到listenfd事件时，建立连接后将新连接处理为ET

```cpp
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/epoll.h>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <fcntl.h>
#include <errno.h>

#define MAX_EVENTS 10
#define PORT 8080

//设置socket连接为非阻塞模式
void setnonblocking(int sockfd)
{
  int opts;
  opts = fcntl(sockfd, F_GETFL);
  if (opts < 0)
  {
    perror("fcntl(F_GETFL)\n");
    exit(1);
  }
  opts = (opts | O_NONBLOCK);
  if (fcntl(sockfd, F_SETFL, opts) < 0)
  {
    perror("fcntl(F_SETFL)\n");
    exit(1);
  }
}

int main()
{
  struct epoll_event ev, events[MAX_EVENTS]; //ev负责添加事件，events接收返回事件
  int addrlen, listenfd, conn_sock, nfds, epfd, fd, i, nread, n;
  struct sockaddr_in local, remote;
  char buf[BUFSIZ];

  //创建listen socket
  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("sockfd\n");
    exit(1);
  }
  setnonblocking(listenfd); //listenfd设置为非阻塞[1]
  bzero(&local, sizeof(local));
  local.sin_family = AF_INET;
  local.sin_addr.s_addr = htonl(INADDR_ANY);
  ;
  local.sin_port = htons(PORT);
  if (bind(listenfd, (struct sockaddr *)&local, sizeof(local)) < 0)
  {
    perror("bind\n");
    exit(1);
  }
  listen(listenfd, 20);

  epfd = epoll_create(MAX_EVENTS);
  if (epfd == -1)
  {
    perror("epoll_create");
    exit(EXIT_FAILURE);
  }

  ev.events = EPOLLIN;
  ev.data.fd = listenfd;
  if (epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev) == -1)
  { //监听listenfd
    perror("epoll_ctl: listen_sock");
    exit(EXIT_FAILURE);
  }

  for (;;)
  {
    nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
    if (nfds == -1)
    {
      perror("epoll_pwait");
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < nfds; ++i)
    {
      fd = events[i].data.fd;
      if (fd == listenfd)
      {
        while ((conn_sock = accept(listenfd, (struct sockaddr *)&remote,
                                   (size_t *)&addrlen)) > 0)
        {
          setnonblocking(conn_sock); //下面设置ET模式，所以要设置非阻塞
          ev.events = EPOLLIN | EPOLLET;
          ev.data.fd = conn_sock;
          if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn_sock, &ev) == -1)
          {                             //读监听
            perror("epoll_ctl: add"); //连接套接字
            exit(EXIT_FAILURE);
          }
        }
        if (conn_sock == -1)
        {
          if (errno != EAGAIN && errno != ECONNABORTED && errno != EPROTO && errno != EINTR)
            perror("accept");
        }
        continue;
      }
      if (events[i].events & EPOLLIN)
      {
        n = 0;
        while ((nread = read(fd, buf + n, BUFSIZ - 1)) > 0)
        { //ET下可以读就一直读
          n += nread;
        }
        if (nread == -1 && errno != EAGAIN)
        {
          perror("read error");
        }
        ev.data.fd = fd;
        ev.events = events[i].events | EPOLLOUT; //MOD OUT
        if (epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1)
        {
          perror("epoll_ctl: mod");
        }
      }
      if (events[i].events & EPOLLOUT)
      {
        sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World", 11);
        int nwrite, data_size = strlen(buf);
        n = data_size;
        while (n > 0)
        {
          nwrite = write(fd, buf + data_size - n, n); //ET下一直将要写数据写完
          if (nwrite < n)
          {
            if (nwrite == -1 && errno != EAGAIN)
            {
              perror("write error");
            }
            break;
          }
          n -= nwrite;
        }
        close(fd);
      }
    }
  }
  return 0;
}
```

