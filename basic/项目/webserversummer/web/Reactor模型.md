

##  

IO复用



### Channel实现
- 使用Poll实现
- 功能：
  - 负责管理分发描述符，但是不拥有描述符fd
  - 析构也不会关闭这个fd
  - 管理事件，更新事件
  - 设置事件
  - 设置事件回调函数
  - 设置事件处理成员函数 handleEvent
    - 根据事件类型调用不同的事件回调函数

### EventLooper实现
  - 判断是否looper运行
  - 判断当前线程是否是创建looper的线程
  - 判断当前线程是否只有一个looper（利用线程的局部存储looper的this指针）
  - 用一个静态成员获取looper对象指针（this指针），只允许最多有一个looper对象

添加**runInLoop函数和queueInLoop**

IO线程平时阻塞在事件循环EventLoop::loop()的poll(2)调用中，为了让IO线程能立刻执行用户回调，我们需要唤醒它。

1. 传统的办法是用pipe(2)，IO线程始终监视此管道的readable事件，在需要唤醒的时候，其他线程往管道里写一个字节，这样IO线程就从IO

   multiplexing阻塞调用中返回。（原理类似HTTP long polling。）

2. 利用eventfd实现，更加高效。

**实现**：

在loop循环中，实际上IO线程阻塞在poll中，这时如何唤醒呢？得给他一个可读事件让他醒过来。醒过来之后调用`doWatiingFunctor()` 

这个可读事件就是eventfd。wirte之后，有readable事件，然后调用readhandle读出事件。重要的是并没有在这里调用`doWatiingFunctor()`

实际上是在loop中调用的

 -  监听可读事件的`weakupChannel()`

    

- runInLoop在它的IO线程内执行某个用户的任务回调函数。

- 如果用户在其他线程调用runInloop，cb会被加入队列，IO线程会被唤醒来调用这个Functor

- wakeup() ，功能写eventfd计数（递增）,在quit中调用。什么时候调用`weakup()`

  - 线程退出时，调用`queueInloop`的不是IO线程，这时需要调用weakup
  - 在IO线程调用，同时正在调用`waitingFunctors`时需要唤醒

- `handleRead()` //读eventfd

  ```cpp
   weakupChannel_->setEECallback(std::bind(&EventLooper::handleRead,this)); *//handleRead需要this指针，但是不能直接传函数名
  ```

  

- `waitingFunctors`成员：容器中装有Functor 唤醒IO线程已供调用

- `doWatiingFunctor()`此函数负责调用等待的函数对象 。为什么放到loop函数中？而不是处理函数中？  

  

  

  

  evenfd示例：

  ```c
  #include<stdio.h>
  #include<sys/eventfd.h>
  #include<unistd.h>
  #include<stdint.h>
  #include<stdlib.h>
  int createEventfd()
  {
      //int evfd = eventfd(0,EFD_NONBLOCK|EFD_CLOEXEC);
      int evfd = eventfd(0,0);
      if(evfd<0)
      {
          printf("Failed in eventfd\n");
          exit(0);
      }
      return evfd;
  
  }
  int main()
  {
      int weakfd = createEventfd();
      uint64_t one=0;
      printf("about to fork\n");
      int pid;
      if((pid=fork())<0)
      {
          printf("fork fail\n");
          exit(1);
      }
  
      else if(pid==0)
      {
              one = 1;
              write(weakfd,&one,sizeof one);
  						//重新设置为3
              one = 3;
              write(weakfd,&one,sizeof one);
      }
      
      else
      {
          sleep(2);
          one = 1;
          read(weakfd,&one,sizeof one); //读取的实际上还是weakfd的，也就是weakfd是跨进程的，最后结果为4
          printf("%llu\n",(unsigned long long) one);
      }
      exit(0);
  }
  /*
  输出：
  about to fork
  4
  */
  ```
  
  

  

#### EventLooperThread

IO线程不一定是主线程，可以在任一一个线程创建并运行EventLooper。同时一个程序也不只有一个IO线程。

按优先级将不同的socket非给不同的IO线程，避免优先级反转。



EventLooperThread会启动自己的线程，符号one loop per thread

### select函数：

- 有描述符数量限制
- 每次扫描描述符的全部集合（若只有小部分活跃则十分浪费资源），效率随着描述符数量线性下降（poll也是）
- 每次返回后，需要重新加入内核FD_ADDSET。然后Select（系统调用每次都需要重新拷贝）
- 


### Poller
- 描述：使用poll和epoll实现（纯虚函数，写为接口），目前使用poll

1. select/poll缺点select/poll的缺点在于：
     1. 每次调用时要重复地从用户态读入参数。（每次系统调用都会创建poll_list链表并拷贝用户数组的pollfd）
     2. 每次调用时要重复地扫描文件描述符。
     3. 每次在调用开始时，要把当前进程放入各个文件描述符的等待队列。在调用结束后，又把进程从各个等待队列中删除。
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-07-20-19-37-08.png
width=450px>
</center>



  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-20-22-06-24.png
  width=450px>
  </center>

- 先注册回调函数__poll_wait，再初始化table变量（类型为struct poll_wqueues)，
- 接着拷贝用户传入的struct pollfd（其实主要是fd）(瓶颈1)，

- 然后轮流调用所有fd对应的poll（把current挂到各个fd对应的设备等待队列上）(瓶颈2)。
- 在设备收到一条消息（网络设备）或填写完文件数据（磁盘设备）后，会唤醒设备等待队列上的进程，这时current便被唤醒了。current醒来后离开sys_poll的操作相对简单，这里就不逐行分析了。

### PollPoller

typedef std::vector<Channel*> ChannelList;
- 主要成员函数：
  - poll(int timeout,)函数 调用poll或epoll。进而调用成员函数fillActiveChannel将活动事件写入到Channel中，使用这个函数
  - fillActiveChannel参数为activeChannels以及事件发生数量，将有事件发生的结果写入activeChannels，const成员函数（不用修改）
  - updateChannel(Channel *ch)；用于更新维护pollfd数组。
    - 第一种情况是新的channel 即channel中还不存在pollfds数组下标。
    - 已经存在的channel

- 主要成员
  - pollfds pollfd数组
  - channels_，map<int,Channel>类型的一个map
  - 

```c
//关于事件分类，带外数据 还是要看unix网络编程呀
POLLIN: 有普通数据或者优先数据可读
POLLRDNORM: 有普通数据可读
POLLRDBAND: 有优先数据可读
POLLPRI: 有紧急数据可读
POLLOUT: 有普通数据可写
POLLWRNORM: 有普通数据可写
POLLWRBAND: 有紧急数据可写
POLLERR: 有错误发生
POLLHUP: 有描述符挂起事件发生
POLLNVAL: 描述符非法
```

### 关闭连接





### EpollPoller



### TimeQueue定时器

