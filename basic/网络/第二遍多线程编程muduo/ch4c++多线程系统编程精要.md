# c++多线程系统编程精要
思维转变：
1. 当前线程可能随时会被切换出去，或者是说被抢占
2. 多线程程序中事件发生顺序不再有全局统一的先后顺序

当线程被切换回来继续执行下一条语句（指令）时，全局数据（包括进程在内核中的状态）可能已经被其他线程修改

- 多线程程序的正确性不能依赖于任何一个线程的执行速度，不能通过原地等待（sleep）来假定其他线程已经发生，必须用适当的同步看到其他线程发生的结果

## 基本线程原语的选用

- 11个最基本的线程函数
  - 2个：线程的创建和等待结束（join），封装
  - 4个：mutex的创建，销毁，加锁，解锁
  - 5个：条件变量的创建，销毁，等待，通知，广播
- ThreadPool 和 CountDownLatch
- 酌情使用
  - 1. pthread_once 封装为Singleton<T>
  - 2. pthread_key* 封装为ThreadLocal<T>
- 不建议使用
  - pthread_rwlock读写锁：实际上与mutex相比会降低性能
  - sem* 信号量 功能与条件变量重合，但容易用错

## C/C++系统库的线程安全性

- 尽量设计为immutable的类
- cout<<"time"<<time;不是线程安全的相当于调用了两次operator<<
  
## Linux上的线程标识

- POSIX提供了一个pthread_self函数用于返回当前进程的标识符，类型为pthread_t，pthread_t不一定是个数值类型，也有可能是个结构体
  - 无法打印pthread_t 因为不知道pthread_t确切类型。无法在日志中用它表示当前线程id
  - 无法比较pthread_t大小或者计算其hash值，因此无法用作关联容器的key
  - 无法定义一个非法的pthread_t值，用来表示绝对不可能存在的线程id，因此Mutexlock类没办法有效判断当前线程是否已经持有本锁
  - pthread_t只在进程内有意义，与操作系统任务无法建立有效关联，例如/proc中就无法找到pthread_t对应的task
  - 同时，glibc的线程实现实际把pthread_t用作一个结构体指针，指向一块动态分配的内存。且是反复使用的

```cpp
#include<pthread.h>
#include<stdio.h>
void* thread(void* arg){
    return NULL;
}
int main(){
    pthread_t t1,t2;
    pthread_create(&t1,NULL,thread,NULL);
    printf("%1x\n",t1);
    pthread_join(t1,NULL);

    pthread_create(&t2,NULL,thread,NULL);
    printf("%1x\n",t2);
    pthread_join(t2,NULL);

}
//t1 t2值相同
```

- 在Linux上，建议gittid(2) 系统调用作为线程id，glibc没有封装，可以自己封装
- 但是每次调用系统调用比较低效，因此可以用__thread变量缓存gittid(2)得到的线程id
- boost::this_thread::get_id()也实现了
- 如果fork(2)之后，子进程会不会看到stale的缓存结果呢？ 在pthread_atfork()函数中注册一个回调，清空缓存的线程id


## 线程的创建与销毁守则

- 线程创建准则
  - 程序库不应该在未提前告知的情况下创建自己的“背景线程”
  - 尽量用相同的方式创建线程
  - 在进入main函数之前不应该启动线程
  - 程序中线程的创建最好能在初始化阶段全部完成

- 销毁
  - 自然死亡
  - 非正常死亡
  - 自杀，pthread_exit()立刻退出线程
  - 它杀，其他线程调用pthread_cancel()来终止某个线程

### exit(3)不是线程安全的


### 善用__thread关键字
- __thread是gcc内置的线程局部存储设施(thread local storage)
- _thread只能用于修饰POD类型，不能修饰class类型
  ```cpp
  __thread string t_obj1("hello");//错误 不能调用对象的构造函数
  __thread string* t_obj2 = new string; //错误，初始化必须用编译器常量
  __thread string* t_obj3 = NULL;//正确，需要手动初始化 并销毁对象，修饰的是指针类型

  t_obj3 = new string;

  ```
- 各个线程之间的值互不干扰
- 还可以修饰“值可能汇编，带有全局性，但是又不值得用全局锁保护的变量”


## 多线程和IO 
- 只讨论同步IO

- 多个线程操作同一个文件描述符会导致逻辑混乱
- 因此应该是每个文件描述符只由一个线程操作。一个线程可以操作多个描述符，但是不能操作别的线程已经拥有了的文件描述符
- 例如**我们对同一个epoll_fd操作应该放到同一个线程**，否则当一个线程阻塞在epoll_wait()时，另一个线程往epoll_fd添加一个新的监视fd在epoll_wait调用中返回，导致错误
- 参考muduo::EventLoop::weakup()


## 用RAII包装文件描述符

- 对描述符的读写操作都通过Socket对象进行，只要对象还活着，就不会有其他跟他一样的描述符，对象结束，自动调用析构函数关闭文件描述符


## RAII与fork()

- 很可能出现对象构建了一次，析构两次（父进程，子进程各一次）
  ```cpp
  int main(){
    Foo foo; //调用构造函数
    fork(); 
    foo.doit();// 父子进程都调用了foo
    //析构函数会被调两次，父进程和子进程各一次
  }
  ```
- 如果Foo中封装了某种资源，且这个资源没有被子进程基础那么foo.doit()的功能在进程可能是错乱的。

- 子进程不会继承父进程的内存锁，文件锁，某些定时器（setitimer，alarm）。
## 多线程与fork()


## 多线程与signal