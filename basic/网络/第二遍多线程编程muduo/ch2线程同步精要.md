# 第二章 线程同步精要
- 并发编程的两种模型：
  - 消息传递（message passing)
  - 共享内存 (shared memory)
- 在分布式系统中，只有一种实用模型：消息传递

- 线程同步的四项原则，按重要性排序
  - 1. 首先原则是最低限度的共享对象。减少需要同步的场合。一个对象尽量不要暴露给别的线程。如果要暴露，优先考虑**immutable对象（不可变对象）看知乎理解**，实在不行才能暴露其他对象，并且用同步措施充分保护
  - 2. 使用高级并发构件，TaskQueue，CountDownLatch，Consumer Queue等
  - 3. 不得已必须使用底层同步原语时，只用非递归的互斥器和条件变量。慎用读写锁，不要用信号量！
  - 4. 除了使用atomic整数之外，不自己编写lock free代码。不要用内核级别同步原语

## 第三条：互斥器 mutex（底层同步原理）
- 用的最多的同步原语，保护临界区
- 任何一个时刻最多只能有一个线程在此mutex划出的临界区活动
- 单独使用mutex保护共享数据

使用原则：
- 用**RAII手法**封装mutex**创建，销毁，加锁，解锁**四个操作
- 保证锁的生效期间等于一个作用域，不会因异常而忘记解锁
- 只用非递归的mutex（即**不可重用的mutex**）
- 不手动调用lock和unlock函数。一切交给栈上的Guard对象的构造函数和析构函数负责。Guard对象的生命期正好等于临界区（需要分析对象什么时候析构）始终保证了在同一个函数同一个scope里对某个mutex加锁和解锁 Scoped-Locking（不会在foo函数加锁，bar函数内解锁）
- 在每次构造了Guard对象之后 思考（调用栈上）已经持有的锁，防止因加锁顺序导致死锁

次要原则：
- 不使用跨进程的mutex，进程间通信只用TCP socket
- 加锁解锁在同一个线程，线程a不能去unlock线程b已经锁住的mutex（RAII自动保证）
- 别忘了解锁（RAII自动保证）
- 不重复解锁（RAII自动保证）
- 必要时考虑用PTHREAD_MUTEX_ERRORCHECK排错

- mutex是最简单的同步原语 按上面原则

### 只使用非递归的mutex（不可重入）
- 递归（recursive）mutex==可重入（reentrant），非递归(non-recursive)==不可重入
- 区别在于：同一个线程可以重复对recursive mutex加锁，不能重复对non-recursive mutex加锁
- 首选非递归mutex不是为了性能，只是少了一个计数器，效果差别不大。
  - 而是为了设计意图，在同一个线程多次对非递归（不可重入）会立即导致死锁，这能够帮助我们思考代码对锁的期求，及早（编码阶段）发现问题
  - Java和windows默认提供可重入mutex更加方便，但是问题也更多
  - 例如你以为拿到一个锁就可以修改对象了，殊不知外层代码已经拿到了锁，正在修改或者读取同一个对象呢。这时导致出错
  ```cpp
  MutexLock mutex
  std::vector<Foo> foos;
  void post(const Foo& f){
      MutexLockGuard lock(mutex);
      foos.push_back(f);
  }

  void traverse(){
      MutexLockGuard lock(mutex);
      for(std::vector<Foo>::const_iterator it = foos.begin();it!=foos.end();++it){
          it->doit();
      }
  }
  ```
  - 如果Foo::doit间接调用了post：
    - 如果mutex是非递归的，那么会死锁
    - 如果mutex是递归的，push_back()可能会导致迭代器失效
  - 如果确实需要在遍历的时候修改vector，一是将修改推后记住添加或删除的元素。等循环结束再依次修改，二是用copy on write
  - 如果一个函数可能在已加锁的情况下调用，又可能在未加锁的情况下调用，拆成两个函数
    - 1. 跟原来同名，函数加锁，转而调用第二个函数
    - 2. 给函数名加上后缀WithLockHold不加锁，把原来函数体搬过来
    ```cpp
    void post(const Foo& f){
        MutexLockGuard lock(mutex);
        postWithLockHold(f);
    }
    void postWithLockHold(const Foo& f){
        //对于b的问题
        assert(mutex.isLockedThisThread());
        foos.push_back(f);
    }
    ```
    - 问题：误用了加锁版本，死锁了；b.误用了不加锁版本，数据损坏

## 死锁

- 对同一个mutex加锁多次，一个加锁函数里面调用另一个加锁函数

```cpp
class Request{
public:
    void process()[
        muduo::MutexLockGuard(mutex_);;
        print()//调试
    ]
    void print() const{
        muduo::MutexLockGuard lock(mutex_);//
        //...
    }
private:
    mutable muduo::MutexLock mutex_;
};
```
- process中调用了print导致死锁，可以从print中抽取出printWithLockHold()，并让Request::print和Request::process 都调用它


- 两个线程死锁的例子
  ```cpp
  class Inventory{
    public:
        void add(Request* req){
            muduo::MutexLockGuard(mutex_);
            requests_.insert(req);
        }
        void remove(Request* req){
            muduo::MutexLockGuard(mutex_);
            requests_.erase();
        }
        void printAll() const{
            muduo::MutexLockGuard(mutex_);
            sleep(1);
            for(std::set<Request*>::const_iterator it = requests_.begin();it!=requests_.end();++it){
                (*it)->print();
            }
            printf("unlock");
        }

    private:
        mutable moduo::MutexLock mutex_;
        std::set<Request*> requests_; //

  };
  Inventory g_inventory;
  class Request{
    public:
        void process()[
            muduo::MutexLockGuard(mutex_);
            g_inventory.add(this);//暴露了this指针
            print()//调试
        ]
        void print() const{
            muduo::MutexLockGuard lock(mutex_);//
            //...
        }
        ～Request()__attribute__((noinline)){
            muduo::MutexLockGuard lock(mutex_);//
            sleep(1);
            g_inventory.remove(this):


        }
    private:
        mutable muduo::MutexLock mutex_;
    };
  ```




## 条件变量（管程）

- 互斥器是加锁原语，排他性访问共享数据，不是等待原语。
- 在使用mutex时希望加锁不要阻塞，能够立刻拿到锁。用完之后想尽快解锁
- 如果需要等待某个条件变量成立，应该使用条件变量
- 对于**wait端**的条件变量：
  - 必须和mutex一起使用，该bool表达式的值受此mutex保护
  - 在mutex已上锁的情况下才能调用wait()
  - 把判断布尔条件和wait()放在while循环中
  ```cpp
  muduo::MutexLock mutex
  muduo::Condition cond(mutex);
  std::deque<it> queue;

  int dequeue(){
      MutexLockGuard lock(mutex);
      while(queue.empty()){ //必须用循环 判断之后进入wait
        cond.wait();
        //wait执行完时自动加锁
      }
      assert(!queue.empty());
      int top = queue.front();
      queue.pop_front();
      return top;
  }
  ```
  - 必须使用while循环，而不能使用if 原因是spurious wakeup (虚假唤醒)
  - 虚假唤醒：Spurious wakeup describes a complication in the use of condition variables as provided by certain multithreading APIs such as POSIX Threads and the Windows API. Even after a condition variable appears to have been signaled from a waiting thread's point of view, the condition that was awaited may still be false.
  - 

- 对于Signal/broadcast端
  - 不一定要在mutex已上锁的情况下调用signal
  - 在signal之前要改布尔表达式
  - 修改布尔表达式要mutex保护
  - signal一般表示资源可用，broadcast表明状变化
  ```cpp
  void enqueue(int x){
    MutexLockGuard lock(mutex);
    queue.push_back(x);
    cond.notify();//可以移出到临界区之外 无限容量

  }
  ```
- 条件变量属于底层同步原语，用的较少。一般使用CountDownLatch 倒计时 是一个常用且易用的同步手段。
  - 主线程发起多个子线程，等这些子线程都完成之后，主线程才能继续执行。通常用于等待多个对等线程初始化
  - 主线程发起多个子线程，子线程都等待主线程。主线程完成其他一些任务之后通知子线程开始执行。通常用于多个子线程等待主线程通知“起跑”。


- 一般不使用信号量
  

## 线程安全的Singleton实现
- double checked locking（DCL）


## sleep不是同步原语
- sleep/usleep/nanosleep只能用在测试代码中

## 归纳与总结
- 线程同步四个原则：尽量使用高层同步设施（线程池，队列，倒计时
- 使用普通互斥器和条件变量完成剩余同步任务，采用RAII惯用手法，idiom和Scoped locking

- 先把程序写正确（并尽量保持清晰和简单），然后再优化
- 让一个正确的程序变快，比让一个快的程序变正确简单
- 真正影响性能的不是锁，而是锁的争用 lock contention


## 2.8 使用share_ptr实现copy on write(在写端copy)
- 利用引用计数，利用g_foos.unique() 判断是否有其他函数引用
- 在读端foos = g_foos; //引用计数+1 unique 为false
- 在写端拷贝时unique 为false即拷贝 reset对象重置引用计数即`g_foos.reset(new FooList(*g_foos));`,
- 拷贝和插入元素时需要加锁
```cpp
void post(const Foo &f)
{
    printf("post\n"); 
    //临界区
    {
    //printf("get_count:%ld\n",g_foos.use_count());
    MutexLockGuard lock(mutex);
    if (!g_foos.unique()) //如果有人引用
    {
        //复制一份 引用计数清为11
        g_foos.reset(new FooList(*g_foos));//为什么初始化为FooList(g_foos*)出错 没有初始化
        
    }
    if(!g_foos.unique()){ //被引用了
        exit(1);
    }
    g_foos->push_back(f);

    }
    printf("copy the whole list\n");
}
```

- MutualLock request那个例子
  - shared_ptr使用
  - enable_shared_from_this类模板的使用。

- 用普通锁换读写锁的一个例子
- copy on other reading