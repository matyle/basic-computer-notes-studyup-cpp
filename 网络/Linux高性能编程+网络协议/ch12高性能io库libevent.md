
# 高性能io库libevent

## I/O框架库
- IO事件，信号和定时事件统一称为事件源。
- Reactor模式
  - 句柄handle ：一个事件源通常绑定一个句柄
    - 作用是，当内核检测到就绪事件时，通过句柄来通知应用程序这一事件。
    - IO事件的句柄为文件描述符，信号事件的句柄就是信号值
- 事件多路分发器
  - 事件的到来是随机的，异步的，因此只能循环等待事件的到来，即事件循环。
  - 等待事件一般使用io复用技术。io框架库将系统支持的各种io复用系统调用封装为统一接口，称为事件多路分发器
  - 事件多路分发器demultiple是核心函数,内部调用的是select poll，epoll_wait
  - 还需要register_event remove_event 向事件多路分发器注册或移除事件
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-27-15-25-20.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>


- 事件处理器和具体事件处理器 event类型作为事件
  - 事件处理器执行事件对应的业务逻辑。它通常有一个或多个handl_event回调函数,这些回调函数在事件循环中被执行。 回调函数是虚函数，用户扩展
  - 事件处理器必须和句柄绑定
- Reactor是io框架库的核心，有以下主要方法
  - handle_event.执行事件循环：等待事件，依次处理所有就绪事件对应的事件处理器
  - register_handle，调用还需要register_event 向事件多路分发器注册事件
  - remove_handle,调用remove_event向事件多路分发器移除事件、
- event_base类相当于一个Reactor实例
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-27-15-33-31.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  

## Libevent源码分析

- 提高代码能力，c语言


- 一个实例
  - `event_base *base = event_init();` event_base相当于一个Reactor实例
  ```c
      event_base *base = event_init(); //创建一个Reactor
    event* signal_event = evsignal_new(base,SIGINT,signal_cb,base); //创建信号事件处理器
    event_add(signal_event,NULL);//添加事件

    struct timeval tv = {1,0};

    event* timeout_event = evtimer_new(base,timeout_cb,NULL);

    event_add(timeout_event,&tv);

    event_base_dispatch(base); //执行事件循环
    event_free(timeout_event);
    event_free(signal_event);

    event_base_free(base);
  ```
- 事件：句柄上绑定的事件
- 事件处理器：event结构体的对象 event对象不是描述事件，而是**事件处理器**
- 事件由事件多路分发器管理，事件处理器则由事件队列管理。
- 事件循环对一个被激活的事件（就绪事件）的处理，即执行事件对应的事件处理器的回调函数

### event 结构体

```cpp
    struct event_callback {
        TAILQ_ENTRY(event_callback) evcb_active_next;
        short evcb_flags;
        ev_uint8_t evcb_pri;	/* smaller numbers are higher priority */
        ev_uint8_t evcb_closure;
        /* allows us to adopt for different types of events */
            union {
            void (*evcb_callback)(evutil_socket_t, short, void *);
            void (*evcb_selfcb)(struct event_callback *, void *);
            void (*evcb_evfinalize)(struct event *, void *);
            void (*evcb_cbfinalize)(struct event_callback *, void *);
        } evcb_cb_union;
        void *evcb_arg;
    };

    struct event_base;

    struct event {
        struct event_callback ev_evcallback;

        /* for managing timeouts */
        union {
            TAILQ_ENTRY(event) ev_next_with_common_timeout;
            int min_heap_idx;
        } ev_timeout_pos;
        evutil_socket_t ev_fd;

        struct event_base *ev_base;

        union {
            /* used for io events */
            struct {
                LIST_ENTRY (event) ev_io_next;
                struct timeval ev_timeout;
            } ev_io;

            /* used by signal events */
            struct {
                LIST_ENTRY (event) ev_signal_next;
                short ev_ncalls;
                /* Allows deletes in callback */
                short *ev_pncalls;
            } ev_signal;
        } ev_;

        short ev_events;
        short ev_res;		/* result passed to event callback */
        struct timeval ev_timeout;
};

```

- ev_events。代表事件类型
- ev_next:所有已经注册事件通过该成员串联成注册事件队列，成员可找到下一成员的位置
- ev_activte_next:所有被激活的事件处理器通过该成员串联成一个尾队列。称之为活动事件队列
- ev_timeout_pos是一个联合体，仅用于定时事件处理器，简称为定时器。`min_heap_idx;`指出定时器在堆中的位置
  - 后面引入了通过定时器（使用尾队列存储定时器）`ev_next_with_common_timeout`指出通用定时器的位置
  ```cpp
  	union {
		TAILQ_ENTRY(event) ev_next_with_common_timeout;
		int min_heap_idx;
	} ev_timeout_pos;
  ```

- _ev联合体，通过_ev.ev_io.ev_io_next将io事件处理器串联成一个尾队列，称为io事件队列
  - 同理 ，通过_ev.ev_signal.ev_signal_next;将信号事件处理器串联成一个尾队列，称之为信号事件队列。
  - ev.ev_signal.ev_ncalls用于记录成员指定信号发生时，Reactor需要指向多少次该事件对应的事件处理器中的回调函数。
  - ev_pncalls要么指向NULL，要么指向ev.ev_signal.ev_ncalls
  - 可能对同一个socket文件描述符的可读/可写事件创建多个事件处理器（有不同的回调函数）。

- ev_fd：对于io事件处理器，是文件描述符。对于信号事件处理器，是信号值
- ev_base:该事件处理器从属的event_base实例
- ev_res:激活事件类型
- ev_flags:事件标志
  ```cpp
    #define EVLIST_TIMEOUT	    0x01
    #define EVLIST_INSERTED	    0x02
    #define EVLIST_SIGNAL	    0x04 //没有使用
    #define EVLIST_ACTIVE	    0x08 //属于活动队列
    #define EVLIST_INTERNAL	    0x10 //内部使用
    #define EVLIST_ACTIVE_LATER 0x20
    #define EVLIST_FINALIZING   0x40
    #define EVLIST_INIT	    0x80 //事件处理器已经被初始化
    #define EVLIST_ALL          0xff
  ```
- ev_pri:事件处理器优先级，值越小优先级越高
- ev_closure: 指定event_base执行事件处理器的回调函数时的行为
- ev_timeout:指定定时器超时值
- evcb_callback:事件处理器的回调函数，传入三个参数：ev_fd,ev_res,ev_arg
- ev_arg回调函数参数


### 往注册事件队列中添加事件处理器

```cpp
    event_add(struct event *ev, const struct timeval *tv)
    {
        int res;

        if (EVUTIL_FAILURE_CHECK(!ev->ev_base)) {
            event_warnx("%s: event has no event_base set.", __func__);
            return -1;
        }

        EVBASE_ACQUIRE_LOCK(ev->ev_base, th_base_lock);

        res = event_add_nolock_(ev, tv, 0);

        EVBASE_RELEASE_LOCK(ev->ev_base, th_base_lock);

        return (res);
    }

```

- 位运算
  - 位运算:
```
消除位标志EVLIST_ACTIVE: evcb->evcb_flags &= ~EVLIST_ACTIVE;
判断是否包含某些标志位: (ev->ev_events & (EV_READ|EV_WRITE|EV_CLOSED|EV_SIGNAL) ev_events 是否被置这些位 EV_READ|EV_WRITE|EV_CLOSED|EV_SIGNAL
#define OA_FF 0xff
判断是否只有8位有值, 其它高八位有值则非法 : !(flgs& ~OA_FF)
置EVLIST_TIMEOUT位:ev->ev_flags |= EVLIST_TIMEOUT; 一般先获取原先的置位, 然后再执行该操作
```

- evamp_io_add:将io事件添加到事件多路分发器中，并将对应的事件处理器添加到io事件队列中，同时建立io事件和io事件处理器的映射
- evmap_signal_add
- event_queue_insert函数
  - 三类
  ```cpp
  static void event_queue_insert_inserted(struct event_base *base, struct event *ev);
  static void event_queue_insert_active(struct event_base *base, struct event_callback *evcb);
  static void event_queue_insert_active_later(struct event_base *base, struct event_callback *evcb)
  static void event_queue_insert_timeout(struct event_base *base, struct event *ev)

  ```
### 往事件多路分发器中注册事件

- 基本数据结构
```cpp
    #define	_TAILQ_HEAD(name, type, qual)					\
    struct name {								\
        qual type *tqh_first;		/* first element */		\
        qual type *qual *tqh_last;	/* addr of last next element */	\
    }
    #ifdef EVMAP_USE_HT
    #define HT_NO_CACHE_HASH_VALUES
    #include "ht-internal.h"
    struct event_map_entry;
    HT_HEAD(event_io_map, event_map_entry);
    #else
    #define event_io_map event_signal_map
    #endif

    

    /* Used to map signal numbers to a list of events.  If EVMAP_USE_HT is not
    defined, this structure is also used as event_io_map, which maps fds to a
    list of events.
    */
    struct evmap_io {
        struct event_dlist events;
        ev_uint16_t nread;
        ev_uint16_t nwrite;
        ev_uint16_t nclose;
    };

    struct event_signal_map {
	/* An array of evmap_io * or of evmap_signal *; empty entries are
	 * set to NULL. */
	void **entries;
	/* The number of entries available in entries */
	int nentries;
    };

    #ifdef EVMAP_USE_HT
    struct event_map_entry {
        HT_ENTRY(event_map_entry) map_node;
        evutil_socket_t fd;
        union { /* This is a union in case we need to make more things that can
                be in the hashtable. */
            struct evmap_io evmap_io;
        } ent;
    };


```

### eventop 结构体

- eventop封装了io复用机制必要的操作：注册事件，等待事件等。
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-29-16-13-57.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
### event_base结构体

```c
struct event_base {
        /** Function pointers and other data to describe this event_base's
         * backend. */
        const struct eventop *evsel;
        /** Pointer to backend-specific data. */
        void *evbase;

        /** List of changes to tell backend about at next dispatch.  Only used
         * by the O(1) backends. */
        struct event_changelist changelist;

        /** Function pointers used to describe the backend that this event_base
         * uses for signals */
        const struct eventop *evsigsel;
        /** Data to implement the common signal handler code. */
        struct evsig_info sig;

        /** Number of virtual events */
        int virtual_event_count;
        /** Maximum number of virtual events active */
        int virtual_event_count_max;
        /** Number of total events added to this event_base */
        int event_count;
        /** Maximum number of total events added to this event_base */
        int event_count_max;
        /** Number of total events active in this event_base */
        int event_count_active;
        /** Maximum number of total events active in this event_base */
        int event_count_active_max;

        /** Set if we should terminate the loop once we're done processing
         * events. */
        int event_gotterm;
        /** Set if we should terminate the loop immediately */
        int event_break;
        /** Set if we should start a new instance of the loop immediately. */
        int event_continue;

        /** The currently running priority of events */
        int event_running_priority;

        /** Set if we're running the event_base_loop function, to prevent
         * reentrant invocation. */
        int running_loop;

        /** Set to the number of deferred_cbs we've made 'active' in the
         * loop.  This is a hack to prevent starvation; it would be smarter
         * to just use event_config_set_max_dispatch_interval's max_callbacks
         * feature */
        int n_deferreds_queued;

        /* Active event management. */
        /** An array of nactivequeues queues for active event_callbacks (ones
         * that have triggered, and whose callbacks need to be called).  Low
         * priority numbers are more important, and stall higher ones.
         */
        struct evcallback_list *activequeues;
        /** The length of the activequeues array */
        int nactivequeues;
        /** A list of event_callbacks that should become active the next time
         * we process events, but not this time. */
        struct evcallback_list active_later_queue;

        /* common timeout logic */

        /** An array of common_timeout_list* for all of the common timeout
         * values we know. */
        struct common_timeout_list **common_timeout_queues;
        /** The number of entries used in common_timeout_queues */
        int n_common_timeouts;
        /** The total size of common_timeout_queues. */
        int n_common_timeouts_allocated;

        /** Mapping from file descriptors to enabled (added) events */
        struct event_io_map io;

        /** Mapping from signal numbers to enabled (added) events. */
        struct event_signal_map sigmap;

        /** Priority queue of events with timeouts. */
        struct min_heap timeheap;

        /** Stored timeval: used to avoid calling gettimeofday/clock_gettime
         * too often. */
        struct timeval tv_cache;

        struct evutil_monotonic_timer monotonic_timer;

        /** Difference between internal time (maybe from clock_gettime) and
         * gettimeofday. */
        struct timeval tv_clock_diff;
        /** Second in which we last updated tv_clock_diff, in monotonic time. */
        time_t last_updated_clock_diff;

    #ifndef EVENT__DISABLE_THREAD_SUPPORT
        /* threading support */
        /** The thread currently running the event_loop for this base */
        unsigned long th_owner_id;
        /** A lock to prevent conflicting accesses to this event_base */
        void *th_base_lock;
        /** A condition that gets signalled when we're done processing an
         * event with waiters on it. */
        void *current_event_cond;
        /** Number of threads blocking on current_event_cond. */
        int current_event_waiters;
    #endif
        /** The event whose callback is executing right now */
        struct event_callback *current_event;

    #ifdef _WIN32
        /** IOCP support structure, if IOCP is enabled. */
        struct event_iocp_port *iocp;
    #endif

        /** Flags that this base was configured with */
        enum event_base_config_flag flags;

        struct timeval max_dispatch_time;
        int max_dispatch_callbacks;
        int limit_callbacks_after_prio;

        /* Notify main thread to wake up break, etc. */
        /** True if the base already has a pending notify, and we don't need
         * to add any more. */
        int is_notify_pending;
        /** A socketpair used by some th_notify functions to wake up the main
         * thread. */
        evutil_socket_t th_notify_fd[2];
        /** An event used by some th_notify functions to wake up the main
         * thread. */
        struct event th_notify;
        /** A function used to wake up the main thread from another thread. */
        int (*th_notify_fn)(struct event_base *base);

        /** Saved seed for weak random number generator. Some backends use
         * this to produce fairness among sockets. Protected by th_base_lock. */
        struct evutil_weakrand_state weakrand_seed;

        /** List of event_onces that have not yet fired. */
        LIST_HEAD(once_event_list, event_once) once_events;

    };
```

### 事件循环
event_base_loop
