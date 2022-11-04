## 什么都不做的EventLoop

- **one loop per thread** 每个线程只能有一个EventLoop对象，因此EventLoop构造函数会检查当前线程是否创建了其他eventloop，且不允许其他线程使用其他线程的loop
- 
- Channel类：Channel是Reactor结构中的“事件”，它自始至终都属于一个EventLoop，负责一个文件描述符的IO事件，在Channel类中保存这IO事件的类型以及对应的回调函数，当IO事件发生时，最终会调用到Channel类中的回调函数。因此，程序中所有带有读写时间的对象都会和一个Channel关联，包括loop中的eventfd，listenfd，HttpData等。
EventLoop：One loop per thread意味着每个线程只能有一个EventLoop对象，EventLoop即是时间循环，每次从poller里拿活跃事件，并给到Channel里分发处理。EventLoop中的loop函数会在最底层(Thread)中被真正调用，开始无限的循环，直到某一轮的检查到退出状态后从底层一层一层的退出。


- **callback** 回调函数是一种特殊的函数，这个函数被作为参数传给另一个函数去调用。这样的函数就是回调函数。
- 「我调用一个函数 f」(I call a function），那么「调用函数」的人是「我」。代码是 f(c）
- 「我」让这个函数 f 在执行完后，调用我传给它的另一个函数 c。
- f 执行完的时候，就会「调用 c」，也叫做「回调 c」（call c back），调用 c 的人是 f。
## Reactor关键结构
### channel 类

- 每个channel对象只属于一个EventLoop，因此一个channel对象只属于一个IO线程，EventLoop里有多个channel形成Channels
- 每个Channel对象始终只负责一个文件描述符fd，但是它并不拥有fd。也不会析构时关闭fd。map映射fd
- Channel把不同的IO事件分发为不同的回调，如ReadCallback，WriterCallback。且由用户调用，从而传入该使用什么处理函数
- EventLoop中回调使用std::function表示，无需继承Channel。
- Channel中有fd和事件，活动事件revent_
- 有回调处理函数（由用户设置）
- function对象
  ```cpp

    Channel(EventLoop* loop, int fd); //为什么要传loop？
    void Channel::update()
    {
    loop_->updateChannel(this);
    }
    //更新channel时用到了loop ，如果说要更新某个channel对象 ，方便上层封装或者用户调用更新
    void Channel::update()
    {
    loop_->updateChannel(this);
    }



    using std::placeholders::_1;
    server_.setConnectionCallback(
      std::bind(&EchoServer::onConnection, this, _1));
    //setConnectionCallback
    void setConnectionCallback(const EventCallback& cb)
    { writeCallback_ = cb; }
  ```


### Poller类

- Poller类是IO复用的封装，
- 将pollfd缓存vector<struct pollfd> pollfds_
- Channel* 和 fd的映射map<int,Channel*> ChannelMap;
- `g++ test2.cc EventLoop.cc  -o test2 -lmuduo_net -lmuduo_base -lpthread`

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-26-17-09-03.png
width=490px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">时序图</div>
</center>



## 定时器 TimerQueue
### 定时器类
- 三个类实现
  - TimerId
  - Timer
  - TimerQueue
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-29-15-58-58.png
width=450px>
</center>

- timerfd：可以实现定时器的功能，将定时器抽象为文件描述符，当定时器到期时可以对其read，这样也可以放到监听队列的主循环中。
```c
- #include <sys/timerfd.h>
//第一个系统调用, 该系统调用创建一个新的定时器对象 :
int timerfd_create(int clockid, int flags);
//成功返回一个指代该对象的文件描述符, 失败返回-1及errno

#include <sys/timerfd.h>
//第二个系统调用， 可以启动或停止由文件描述符 fd 所指代的定时器
int timerfd_settime(int fd, int flags, const struct 
		itimerspec* new_value, struct itimerspec* old_value);
//成功返回0, 失败返回-1和 errno
//flags 的值可以是 0 (相对时间), 可以是 TFD_TIMER_ABSTIME (绝对时间)

struct itimerspec 
{
    struct timespec it_interval;   //间隔时间
    struct timespec it_value;      //第一次到期时间
};

struct timespec 
{
    time_t tv_sec;    //秒
    long tv_nsec;    //纳秒
}; 

```
- new_value 为定时器指定新设置, old_value 用来返回定时器的前一设置, 如果不关心, 可将其设置为 NULL
- new_value： 指定新的超时时间，若 newValue.it_value非 0 则启动定时器，否则关闭定时器。若 newValue.it_interval 为 0 则定时器只定时一次，否则之后每隔设定时间超时一次。
- old_value：不为 NULL 时则返回定时器这次设置之前的超时时间。

```c
//第三个系统调用, 返回文件描述符 fd 所标识定时器的间隔及剩余时间
#include <sys/timerfd.h>
int timerfd_gettime(int fd, struct itimerspec *curr_value);
//成功返回0, 失败返回-1和errno

 ctime()、gmtime()和localtime()函数都接受数据类型time_t的参数，该参数表示日历时间(时间戳).

当解释为绝对的时间值时，它表示从纪元(1970-01-01 00:00:00 +0000 (UTC)开始经过的秒数。

  asctime()和mktime()函数都接受一个表示分解时间的参数，该参数是一个以年、月、日等格式分隔的表示的tm结构。
```
### EPOLL 触发模式
- 对于读进程来说，初始时，buffer为空，可以看做电平为0状态，当写进程写入数据后，buffer里就会有数据，这样buffer状态发生变化，此时可以看做是电平1的状态，这样就发生了电平由0到1的变化.
- 如果采用的是**边沿触发（edge trigger）**，就会触发，rfd就会作为ready的描述符返回。但是一次并没有读取完，buffer里还有剩余数据，那么此时还是处于电平1状态，这样的话再次epoll_wait就不会触发，因为电平状态没有发生变化。
- 但是如果使用**电平触发（level trigger）**，buffer里有数据就触发，即电平为1就触发，那么当buffer由空变成有数据状态，就会触发，而且一次没读完，再次进入epoll_wait时还会触发，直到把数据读完，buffer变成空，就不再触发了。epoll默认使用电平触发。
处。

- ET还是LT?
  - LT的处理过程：.、
    -  accept一个连接，添加到epoll中监听EPOLLIN事件. 
    -  当EPOLLIN事件到达时，read fd中的数据并处理. 当需要写出数据时，把数据write到fd中；如果数据较大，无法一次性写出，那么在epoll中监听EPOLLOUT事件. 
    -  当EPOLLOUT事件到达时，继续把数据write到fd中；
    -  如果数据写出完毕，那么在epoll中关闭EPOLLOUT事件
  - ET的处理过程：
    - accept一个一个连接，添加到epoll中监听EPOLLIN|EPOLLOUT事件. 
    - 当EPOLLIN事件到达时，read fd中的数据并处理，read需要一直读，直到返回EAGAIN为止. 
    - 当需要写出数据时，把数据write到fd中，直到数据全部写完，或者write返回EAGAIN. 
    - 当EPOLLOUT事件到达时，继续把数据write到fd中，直到数据全部写完，或者write返回EAGAIN
  - 从ET的处理过程中可以看到，ET的要求是需要一直读写，直到返回EAGAIN，否则就会遗漏事件。而LT的处理过程中，直到返回EAGAIN不是硬性要求，但通常的处理过程都会读写直到返回EAGAIN，但LT比ET多了一个开关EPOLLOUT事件的步骤LT的编程与poll/select接近，符合一直以来的习惯，不易出错ET的编程可以做到更加简洁，某些场景下更加高效，但另一方面容易遗漏事件，容易产生bug


## EventLoop::runInLoop()函数
- 在它的IO线程执行某个用户的回调
- EventLoop::runInloop(const Functor& cb);
- Functor是std::function<void()>
- 如果用户在当前IO线程调用这个函数，回调会同步进行。如果其他用户调用runInLoop() ，cb会被加入到队列，IO线程会被唤醒来调用这个Functor
- 作用：把TimeQueue的成员函数移动到其IO线程，可以在不用锁的情况下实现线程安全

- IO线程平时阻塞在EventLoop::loop()的poll调用中，为了让IO线程能够立即执行它，我们需要唤醒它
  - 使用管道，让IO线程监视此管道的可读事件
  - 使用eventfd ---怎么使用？
    - **eventfd**是一个用来通知事件的文件描述符，**timerfd**是的定时器事件的文件描述符。二者都是内核向用户空间的应用发送通知的机制，可以有效地被用来实现用户空间的事件/通知驱动的应用程序。
    - 就是eventfd用来触发事件通知，timerfd用来触发将来的事件通知。
    ```c
    int eventfd(unsigned int initval, int flags);

    ```
      -  flags
         -  FD_CLOEXEC：FD_CLOEXEC，简单说就是fork子进程时不继承，对于多线程的程序设上这个值不会有错的。
         -  EFD_NONBLOCK：文件会被设置成O_NONBLOCK，一般要设置。
         -  EFD_SEMAPHORE：（2.6.30以后支持）支持semophore语义的read，简单说就值递减1。

      - read(): 读操作就是将counter值置0，如果是semophore就减1
      - write(): 设置counter的值。
      - 注意，还支持epoll/poll/select操作，当然，以及每种fd都都必须实现的close。

### EventLoopThread class 类
- EventLoopThread启动自己的线程，并运行其中的loop()。
- 每个线程一个loop思想
```cpp
  muduo::EventLoopThread loopThread;
  muduo::EventLoop* loop = loopThread.startLoop();
  loop->runInLoop(runInThread);
  sleep(1);
  loop->runAfter(2, runInThread);
  sleep(3);
  loop->quit();
```

## 实现TCP网络库
- 从poll返回到再次调用poll阻塞称为一次事件循环
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-28-15-29-54.png
width=490px>
</center>


### Acceptor class

- handleRead()函数，接受连接，此函数作为channel的回调函数。
```cpp

```



### TcpConnection class
- 唯一用shared_ptr管理的类
- 用户不使用这个的对象，给TcpServer类使用
- 且没有发起连接功能，构造函数参数是已经建立好的socket fd
- 回调函数

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-29-15-59-42.png
width=490px>
</center>

## TcpConnection断开连接

- 删除元素o(1)复杂度，先将要删除的元素交换到末尾，然后pop_back()，如果里面的元素和顺序无关的话



## Buffer
- tcp链接中必须有output buffer。
  - 如果100kb一次只读取了80kb，剩下的应该存在网络库的buffer中。并注册EPOLL_OUT事件，等socket可用后再发送。读取完之后应该取消关注这个事件。（使用电平触发）
  - 假设这时又需要写入50kb，而buffer中还有20kb，不应该直接调用write，也是应该append在20kb后面
  - 如果buffer中还有数据，不能立即关闭连接，必须等数据发送完毕，因此TcpConnction::shutdown没有直接关闭连接


- 必须有input_buffer

- 为什么使用电平触发？
  - 一是为了与传统的poll(2)兼容，因为在文件描述符数目较少，活动文件描述符比例较高时，epoll(4)不见得比poll(2)更高效13，必要时可以在进程启动时切换Poller。
  - 二是level trigger编程更容易，以往select(2)/poll(2)的经验都可以继续用，不可能发生漏掉事件的bug。
  - 三是读写的时候不必等候出现EAGAIN，可以节省系统调用次数，降低延
  迟。
### Buffer作为TcpConnection的输入
```cpp
void TcpConnection::handleRead(Timestamp receiveTime)
{
  int savedErrno = 0;
  ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
  if (n > 0) {
    messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
  } else if (n == 0) {
    handleClose();
  } else {
    errno = savedErrno;
    LOG_SYSERR << "TcpConnection::handleRead";
    handleError();
  }
}
```


### Buffer::readFd()

- 如何设计并使用缓冲区？
  - 一方面我们希望减少系统调用，一次读的数据越多越划算，那么似乎应该准备一个大的缓冲区。
  - 另一方面希望减少内存占用。如果有10000个并发连接，每个连接一建立就分配各50kB的读写缓冲区的话，将占用1GB内存，而大多数时候这些缓冲区的使用率很低。

- 具体做法：在栈上准备一个65536字节的extrabuf，然后利用readv()来读取数据，**iovec**有两块，第一块指向muduo Buffer中的writable字节，另一块指向栈上的extrabuf。
  - 这样如果读入的数据不多，那么全部都读到Buffer中去了；
  - 如果长度超过Buffer的writable字节数，就会读到栈上的extrabuf里，然后程序再把extrabuf里的数据append()到Buffer中
  ```cpp
  char extrabuf[65536];
  ```

  - writev和readv：多缓存的收据发送和接收
  - iovec：是一个缓冲区，包含开始地址和缓冲区长度
    - 我们把分散在各处的数据位置和长度依次赋值给iovec结构数组中的每个iov_base和iov_len，然后交给writev/readv系统调用，就能完成集中写/读的任务了。
  ```c
    struct iovec {
      ptr_t iov_base; /* Starting address */
      size_t iov_len; /* Length in bytes */
    };

    int readv(int fd, const struct iovec *vector, int count);
    int writev(int fd, const struct iovec *vector, int count);
  ```

- 状态图
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-30-17-26-56.png
  width=450px>`
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">tcpconnection 状态图</div>
  </center>
  
- 程序向标准输出的buffer中写入“welcome to epoll's world！”，但是因为没有输出换行，所以buffer中的内容一直存在(退出时才会清空缓冲)

- 经过前面的案例分析，我们已经了解到，当epoll工作在ET模式下时，对于读操作，如果read一次没有读尽buffer中的数据，那么下次将得不到读就绪的通知，造成buffer中已有的数据无机会读出，除非有新的数据再次到达。对于写操作，主要是因为ET模式下fd通常为非阻塞造成的一个问题——如何保证将用户要求写的数据写完。

要解决上述两个ET模式下的读写问题，我们必须实现：

对于读，只要buffer中还有数据就一直读；
对于写，只要buffer还有空间且用户请求写的数据还未写完，就一直写。



**EPOLL事件有两种模型：**

- Level Triggered (LT) 水平触发
  - .socket接收缓冲区不为空 有数据可读 读事件一直触发
  - .socket发送缓冲区不满 可以继续写入数据 写事件一直触发（因此发送完应该要关闭out事件）
符合思维习惯，epoll_wait返回的事件就是socket的状态

- Edge Triggered (ET) 边沿触发
  - .socket的接收缓冲区状态变化时触发读事件，即空的接收缓冲区刚接收到数据时触发读事件
  - .socket的发送缓冲区状态变化时触发写事件，即满的缓冲区刚空出空间时触发读事件仅在状态变化时触发事件


## 多线程TCP Server
- one loop per thread：多线程中，在新建的TCPConnection中挑选一个loop给TCP Connection用
- 单线程：TCP server的EventLoop是和TCP Connection共享的


### EventLoop ThreadPool

