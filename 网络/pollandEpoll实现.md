[你管这破玩意叫 IO 多路复用？-技术圈 (proginn.com)](https://jishuin.proginn.com/p/763bfbd4f65a)

> select实现：
>
> 1. select 调用需要传入 fd 数组，需要拷贝一份到内核，高并发场景下这样的拷贝消耗的资源是惊人的。（可优化为不复制）
>
> 2. select 在内核层仍然是通过遍历的方式检查文件描述符的就绪状态，是个同步过程，只不过无系统调用切换上下文的开销。（内核层可优化为异步事件通知）
>
> 3. select 仅仅返回可读文件描述符的个数，具体哪个可读还是要用户自己遍历。（可优化为只返回给用户就绪的文件描述符，无需用户做无效的遍历）



>epoll 主要就是针对这三点进行了改进。
>
>1. 内核中保存一份文件描述符集合，无需用户每次都重新传入，只需告诉内核修改的部分即可。
>
>2. 内核不再通过轮询的方式找到就绪的文件描述符，而是通过异步 IO 事件唤醒。(设置回调函数)
>
>3. 内核仅会将有 IO 事件的文件描述符返回给用户，用户也无需遍历整个文件描述符集合。

## 等待队列

-  等待队列由循环链表实现，由等待队列头（wait_queue_head_t）和等待队列项（wait_queue）组成，其元素（等待队列项）包含指向进程描述符的指针。

  ```c
  等待队列头结构体的定义：
  
  struct wait_queue_head {
  
  　　spinlock_t  lock;          //自旋锁变量，用于在对等待队列头          
  
  　　struct list_head task_list;  // 指向等待队列的list_head
  
  }; 
  typedef struct __wait_queue_head  wait_queue_head_t;
  
  
  定义等待对列：
  
  struct wait_queue {
  　　unsigned int flags;  //prepare_to_wait()里有对flags的操作，查看以得出其含义
     #define WQ_FLAG_EXCLUSIVE        0x01 //一个常数,在prepare_to_wait()用于修改flags的值
  
    void * private          //通常指向当前任务控制块
  
    wait_queue_func_t func;    //唤醒阻塞任务的函数 ，决定了唤醒的方式
    struct list_head task_list;    // 阻塞任务链表
  
  };
  ```

  

- Linux中等待队列的实现思想如下图所示，当一个任务需要在某个wait_queue_head上睡眠时，将自己的进程控制块信息封装到wait_queue中，然后挂载到wait_queue的链表中，执行调度睡眠。当某些事件发生后，另一个任务（进程）会唤醒wait_queue_head上的某个或者所有任务，唤醒工作也就是将等待队列中的任务设置为可调度的状态，并且从队列中删除。

- 等待队列挂起的是无法获得资源的进程

![img](http://blog.chinaunix.net/attachment/201405/4/28541347_1399218743C00k.jpg)



- 

## select ，poll和epoll的区别

- select有大小限制，内核中限制（poll使用链表）。且效率被fd的数量影响，因为select、poll需要每次取出所有监听集合，然后线性查找其中的活动fd
-  但是select/poll每次调用都会线性扫描全部的集合，导致效率呈现线性下降。但是epoll不存在这个问题，它只会对"活跃"的socket进行 操作---这是因为在内核实现中epoll是根据每个fd上面的callback函数实现的。
- 1.select/poll缺点
 select/poll的缺点在于：
     1.每次调用时要重复地从用户态读入参数。
     2.每次调用时要重复地扫描文件描述符。
     3.每次在调用开始时，要把当前进程放入各个文件描述符的等待队列。在调用结束后，又把进程从各个等待队列中删除。(epoll只需要在ctl的时候挂一次)
## poll的实现
- 在Linux内核中等待队列有很多用途，可用于中断处理、进程同步及定时
  
  
  
  
  ````cpp
    (1) struct poll_table_entry {
  
            struct file  filp;
  
            wait_queue_t wait;//内部有一个指针指向一个进程
  
            wait_queue_head_t   wait_address;//等待队列头部（等待队列有多个wait_queue_t组成，通过双链表连接）
  
    };
  
    (2) struct poll_table_page {
  
            struct poll_table_page   next;
  
            struct poll_table_entry   entry;
  
            struct poll_table_entry entries[0];
  
    };
  
    (3) struct poll_wqueues {
  
        poll_table pt;//一个函数指针，通常指向__pollwait或null
  
        struct poll_table_page * table;
  
        int error;
  
    };
  
    (4) struct poll_list {
  
            struct poll_list *next;//按内存页连接，因为kmalloc有申请数据限制
  
            int len;//用户空间传入fd的数量
  
            struct pollfd entries[0];//存放用户空间存入的数据
  
    };
  
  typedef void (*poll_queue_proc)(struct file *, wait_queue_head_t *, struct poll_table_struct *);
    typedef struct poll_table  struct {
        poll_queue_proc qproc;
    } poll_table;
  
  ```
  ````
  
  - 先注册回调函数__poll_wait，再初始化table变量（类型为struct poll_wqueues)，接着拷贝用户传入的struct pollfd（其实主要是fd）(瓶颈1)，然后轮流调用所有fd对应的poll（把current挂到各个fd对应的设备等待队列上）(瓶颈2)。在设备收到一条消息（网络设备）或填写完文件数据（磁盘设备）后，会唤醒设备等待队列上的进程，这时current便被唤醒了。current醒来后离开sys_poll的操作相对简单，这里就不逐行分析了。
  
- ## Epoll
  - 通过上一章分析，poll运行效率的两个瓶颈已经找出，现在的问题是怎么改进。
    - 首先，如果要监听1000个fd，每次poll都要把1000个fd 拷入内核，太不科学了，内核干嘛不自己保存已经拷入的fd呢？答对了，epoll就是自己保存拷入的fd，它的API就已经说明了这一点——不是 epoll_wait的时候才传入fd，而是通过epoll_ctl把所有fd传入内核再一起"wait"，这就省掉了不必要的重复拷贝。(实现了自己的文件系统)
    - 其次，在 epoll_wait时，也不是把current轮流的加入fd对应的设备等待队列，而是在设备等待队列醒来时调用一个回调函数（当然，这就需要“唤醒回调”机制），把产生事件的fd归入一个链表，然后返回这个链表上的fd。
  - rdllist:事件就绪链表，epoll把fd都挂上一个回调函数，当fd对应的设备有消息时，回调函数就把fd放入rdllist链表，这样epoll_wait只要检查这个rdllist链表就可以知道哪些fd有事件了。

  ```cpp
      struct eventpoll {  
  
          spinlock_t lock;  
  
          struct mutex mtx;  
  
          wait_queue_head_t wq;  /* Wait queue used by sys_epoll_wait() ,调用epoll_wait()时, 我们就是"睡"在了这个等待队列上*/
  
      wait_queue_head_t poll_wait;  /* Wait queue used by file->poll() , 这个用于epollfd本事被poll的时候*/
  
      struct list_head rdllist; /* List of ready file descriptors, 所有已经ready的epitem都在这个链表里面*/ 
  
      struct rb_root rbr; /* RB tree root used to store monitored fd structs, 所有要监听的epitem都在这里*/ 
  
      epitem *ovflist;  /*存放的epitem都是我们在传递数据给用户空间时监听到了事件*/.
  
  
      struct user_struct *user; /*这里保存了一些用户变量,比如fd监听数量的最大值等*/  
  
      };  


3. 函数实现分析
3.1 eventpoll_init
epoll是个module，所以先看看module的入口eventpoll_init
[fs/eventpoll.c-->evetpoll_init()]（简化后）
```cpp
 static int __init eventpoll_init(void)
 {
 epi_cache = kmem_cache_create("eventpoll_epi", sizeof(struct epitem),
 0, SLAB_HWCACHE_ALIGN|EPI_SLAB_DEBUG|SLAB_PANIC, NULL, NULL);

 pwq_cache = kmem_cache_create("eventpoll_pwq",
 sizeof(struct eppoll_entry), 0, EPI_SLAB_DEBUG|SLAB_PANIC, NULL, NULL);
 //注册了一个新的文件系统，叫"eventpollfs"

 error = register_filesystem(&eventpoll_fs_type);
eventpoll_mnt = kern_mount(&eventpoll_fs_type);;
 }
```
- 很有趣，这个module在初始化时注册了一个新的文件系统，叫"eventpollfs"（在eventpoll_fs_type结构里），然后挂载此文件系统。另外创建两个内核cache（在内核编程中，如果需要频繁分配小块内存，应该创建kmem_cahe来做“内存池”）,分别用于存放struct epitem和eppoll_entry。

现在想想epoll_create为什么会返回一个新的fd？因为它就是在这个叫做"eventpollfs"的文件系统里创建了一个新文件！






###  epoll独有的EPOLLET
- EPOLLET是epoll系统调用独有的flag，ET就是Edge Trigger（边缘触发）的意思，具体含义和应用大家可google之。有了EPOLLET，重复的事件就不会总是出来打扰程序的判断，故而常被使用。那EPOLLET的原理是什么呢？
- 有了EPOLLET，重复的事件就不会总是出来打扰程序的判断，故而常被使用。那EPOLLET的原理是什么呢？



```cpp
4.1 ep_poll()    (接3.5)
[fs/eventpoll.c->ep_poll()]

 /* Try to transfer events to user space. */
 ep_events_transfer(ep, events, maxevents)
 ......
把rdllist里的fd拷到用户空间，这个任务是ep_events_transfer做的.

4.2 ep_events_transfer
[fs/eventpoll.c->ep_events_transfer()]
 static int ep_events_transfer(struct eventpoll *ep, struct epoll_event __user *events, 

int maxevents)
{
int eventcnt = 0;
struct list_head txlist;
INIT_LIST_HEAD(&txlist);
/* Collect/extract ready items */
if (ep_collect_ready_items(ep, &txlist, maxevents) > 0) {
/* Build result set in userspace */
eventcnt = ep_send_events(ep, &txlist, events);
/* Reinject ready items into the ready list */
ep_reinject_items(ep, &txlist);
}
up_read(&ep->sem);
return eventcnt;
}
```

- 代码很少，其中ep_collect_ready_items把rdllist里的fd挪到txlist里（挪完后rdllist就空了），接着ep_send_events把txlist里的fd拷给用户空间，然后ep_reinject_items把一部分fd从txlist里“返还”给rdllist以便下次还能从rdllist里发现它。



