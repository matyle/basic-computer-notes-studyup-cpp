# 第一章 线程安全的对象生命期管理
- 对象的生与死不能由对象自身拥有的mutex来保护
- 如何避免对象析构时的竞态条件（race condition）可以借助智能指针


## 当析构函数遇到多线程

- C++要求程序员管理对象的生命期，在多线程环境下尤为困难，当一个对象同时被多个线程同时看到时，对象的销毁时机就会变得模糊不清，多种竞态条件
  - 在即将析构一个对象时，从何而知此时是否有别的线程正在执行该对象的成员函数？
  - 正在执行对象的成员函数时，对象会不会被另外一个线程析构？
  - 在调用对象的成员函数之前，如何得知这个对象还活着？它的析构函数会不会刚好执行到一半？

- 使用**shared_ptr**解决这些问题


### 线程安全的定义三个条件
- 多线程同时访问时，其表现出正确的行为
- 无论操作系统如何调度这些线程，无论这些线程的执行顺序如何交织
- 调用端代码无需额外的同步或其他协调动作

- c++标准库中大多数类都不是线程安全的，都需要外部加锁 例如std::vector，std::map


### MutexLock与MutexLockGuard工具类

- `MutexLock`封装临界区(critical section)，简单的资源管理类，RAII手法封装类互斥器的创建和销毁
  - RAII？    
  - windows中临界区是struct CRITICAL_SECTION是可重入的：同一个线程下，外层方法上锁之后，内层调用的方法也能正常获取锁。即可以通过相同的过程/线程多次锁定，而不会造成死锁。
  - linux下是pthread_mutex_t 默认是不可重入：当函数加锁之后，其他函数执行不能获取锁
  - 什么是可重入，不可重入函数
    - 重入：同一个函数被不同的执行流调用，当前一个流程还没有执行完，就有其他的进程已经再次调用（执行流之间的相互嵌套执行）；
    - 可重入：多个执行流反复执行一个代码，其结果不会发生改变，通常访问的都是各自的私有栈资源；
    - 不可重入：多个执行流反复执行一段代码时，其结果会发生改变；
    - 可重入函数：当一个执行流因为异常或者被内核切换而中断正在执行的函数而转为另外一个执行流时，当后者的执行流对同一个函数的操作并不影响前一个执行流恢复后执行函数产生的结果；


- `MutexLockGuard`封装临界区的进入与退出，即加锁和解锁，MutexLockGuard一般是个栈上对象，作用域刚好等于临界区
- 这两个类都不允许拷贝构造和赋值

- counter类示例
  - mutex_成员示mutable（可变的）const成员函数可以使用mutex_成员
  - 


## 对象创建很简单
- 对象创建的线程安全，唯一要求是构造期间不能泄漏this指针
  - 不要在构造函数中注册任何回调
  - 不要在构造函数中把this传给跨线程的对象
  - 在构造函数的最后一行也不行
- this指针暴露给了其他对象，别的线程可能访问半成品对象


## 销毁太难

- 单线程中只需要注意避免空悬指针和野指针
- 成员函数用来保护临界区的互斥器必须有效，而析构函数会破坏这一假设，因为mutex变量会被销毁

### mutex不是办法
```cpp
Foo::~Foo(){
    MutexLockGruad lock(mutex_)
    //free （1）
}
void Foo::update(){
    MutexLockGruad lock(mutex_) //（2）
    //make use of  internal state

}

//线程A
delete x;
x = NULL;

//线程B
if(x){ //很有可能x检查不为NUL的情况是 因为析构还没完成
    x->update(); //
}
```
- 此时如果AB两个线程都能看到Foo的对象x 线程A即将销毁x，而线程B调用x->update 记住是同时运行的线程 会有很多情况 ，
- 析构一旦执行，mutex_对象会被析构，因此导致死锁


### 1.3.2 作为数据成员的mutex不能保护析构函数

- 同时读写一个类的两个对象可能会死锁
  ```cpp
  void swap(Counter &a,Counter &b){
      MutexLockGuard aLock(a.mutex_);
      MutexLockGuard bLock(b.mutex_);
      int64_t value = a.value_;
      a.value_ = b.value_;
      b.value_ = value;
  }
  ```
  - 如果A线程执行swap(a,b),B线程同时执行swap(b,a) 有可能死锁 



## 线程安全的Observer有多难 观察者设计模式

- 动态创建的对象是活着，只看指针是看不出来的（引用也是）
- 指针指向一块内存，对象一旦销毁，那么这块内存就无法访问，不能访问当然不能知道对象的状态
- 万一原址又创建了一个新的对象呢？等于引用了一个错误的对象

- 对象关系
  -` composition（组合/复合）`：多线程不会遇到麻烦，对象x的生命期，由其唯一拥有者owner控制，owner析构x也会析构。例如owner的直接成员，scope_ptr成员，持有的容器的元素
  - `association（关联/联系）`：对象a用到了对象b！，调用了后者的成员函数，代码形式上是a持有了b的指针，但是b的生命期不由a单独控制
  - `aggregation（聚合）:`关系从形式上看和association一样，但是a和b有逻辑上的整体和部分的关系
  - 解决办法：只创建不销毁，使用对象池来暂存用过的对象，能避免访问失效的问题
    - 但是有很多问题：如何安全的，完整的放回池子里面。防止部分放回的竞态
    - 由于全局数据的lock contention，会不会使多线程串行化？
    - 如果共享对象的类型不止一种，那么是重复实现对象池还是类模版？
    - 会不会内存泄漏与分片，因为对象池的大小只增不减，多个对象池不能共享内存


- 我猜可以用引用计数解决，即shared_ptr

- 如果对象x注册类任何非静态成员函数的回调，那么在某处持有了指向x的指针，产生了竞态态哦就

- Observer模式

```cpp
class Observer //:boost::noncopyable
{
    //noncopyable
public:
    virtual ~Observer();
    virtual void update();

};

class Observable: boost::noncopyable{
public:
    void register_(Observer* x);
    void unregister_(Observer* x);

    void notifyObservers(){
        for(Observer* x :observers_){
            x->update(); //如何得知Observer的对象x还活着？？(1)
        }
    }
private:
    std::vector<Observer*> observers_
};

//在Observer中解注册？
class Observer //:boost::noncopyable
{
    //noncopyable
public:

    virtual void update();
    void observe(Observable* s){
        s->register_s(this);//
        subject_ =s;
    }
    virtual ~Observer(){
        subject_ -> unregister(this); //如何得知subject_还活着？ (2)
    }
    Observable* subject_; //


};


```

- 这是多线程环境---可能不止一个线程在调用Observable的对象
- 线程A还没执行到2，线程B执行到(1)，x正好指向（2）析构的对象
  - x指向的Obsver对象正在解析，这时x去调用它的任何非静态成员函数都是不安全的，何况是虚函数！


## 原始指针有何不妥

- 指向对象的原始指针(raw pointer）是坏的，尤其是当暴露给别的线程时！
- Observable应当保存的不是原始的Observer*
- 类似的，如果Observer要在析构函数中解注册，那么subject_类型也不能是raw pointer
- 直接使用智能指针？窝火，直接使用shared_ptr会造成循环引用，直接造成资源泄漏


- 空悬指针：两个指针p1，p2指向同一对象Object，位于不同线程中
  - 假设线程A通过p1销毁了对象，虽然置为了NULL，p2成了空悬指针
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-10-14-58-32.png
  width=490px>
  </center>
  - 解决方式：引入一层间接性，让p1和p2指向的对象永久有效
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-10-15-00-29.png
  width=490px>
  </center>
- 销毁Object之后，proxy仍然存在，其值变为0，p2可以通过proxy来查看对象是否还活着
- 静态条件：如果说p2看第一眼proxy不是0，正准备调用的时候，期间对象被p1释放了怎么办？
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-10-15-03-24.png
  width=490px>
  </center>

- 一个更好的办法：`引用计数`
  - 将p1，p2两个指针变成对象sp1和sp2 proxy有两个成员`指针和计数器`

  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-10-15-11-35.png
  width=490px>
  </center>

  - sp1析构了，引用计数-1
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-10-15-12-31.png
  width=490px>

  </center>

  - sp2也析构了
  - <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-10-15-12-49.png
    width=490px>
  </center>
  
- 一个万能解决方案
  - 引入另外一层间接性，用对象管理共享资源（把Object看作资源）
  - handler/body技术


## 神器 shared_ptr/weak_ptr

- shared_ptr控制对象的生命周期。强引用（用铁丝绑住了堆上的对象，只要有一个x对象shared_ptr存在，该x对象就不会析构，当指向对象x的最后一个share_ptr析构活着reset之后x保证会被析构
- weak_ptr不控制对象的生命周期，但它知道对象是否还活着（用棉线轻轻拴住堆上的对象）。如果对象还活着他可以提升为有效的share_d_ptr，如果对象已经死了，提升会失败
- shared_ptr和weak_ptr的计数，在主流平台为原子操作，没有用锁 性能好
- shared_ptr和weak_ptr的线程安全级别与std::string和STL容器一样



## 系统地避免各种指针错误

1. `缓冲区溢出` buffer overrun
   使用STL的vector,string或者自己编写Buffer类来管理缓冲区，自动记住缓冲区长度，并通过成员函数而不是裸指针来修改缓冲区
2. `空悬指针/野指针`：用shared_ptr/weak_ptr
3. `重复释放`（double delete）：用scoped_ptr，只在对象析构时释放一次
4.` 内存泄漏`：用scoped_ptr，只在对象析构时自动释放内存
5.` 不配对``new[]/delete`：`new[]`统一替换为vector或者scoped_array
6. 内存碎片

- 智能指针能解决前5个问题，第6个问题会在后面讲

- 注意shared_ptr/weak_ptr/scoped_ptr都是值语意，要么是栈上对象，要么是其他对象的直接数据成员，要么是标准容器中的元素
  - 几乎不会有下面这种用法
  ```cpp
  shared_ptr<Foo*> pFoo = new shared_ptr<Foo>(new Foo);//错误语意
  ```

- 注意：如果智能指针是对象x的数据成员，而它的模版参数T是个incomplete类型，那么x的析构函数不能是默认和内联的。。不懂（10.3.2会讲


## 应用到Observe上
- 既然能用weak_ptr能探查对象生死，Observable的竞态条件，让Observable保存weak_ptr<Observer>即可
```cpp
class Observer //:boost::noncopyable
{
    //noncopyable
public:
    virtual ~Observer();
    virtual void update();

};

class Observable: boost::noncopyable{
public:
    void register_(weak_ptr<Observer> x);//构造observers_数组
    //void unregister_(weak_ptr<Observer> x); //不再需要了 如果weak_ptr<Observer>没有提升成功直接清除

    void notifyObservers(){
        //
        MutexLockGuard lock(mutex_);
        Iterator it = observers_.begin();
        while(it != observers_.end()){
            shared_ptr<Observer>obj(it->lock());//尝试提升
            if(obj){
                //提升成功
                obj->update();//(1)
                ++it;
            }
        }
    }
private:
    mutable MutexLock mutex_;
    std::vector<weak_ptr<Observer>> observers_
    typedef std::vector<weak_ptr<Observer>>::iterator Iterator; //（2）
};

```

- 这个代码 侵入性：强制要求Observe必须以shared_ptr来管理
- 不是完全线程安全
- 锁争用
- 死锁：如果update函数又调用了(un)register,如果是不可重入锁，则会死锁。如果是可重入锁 迭代器会失效。

## 再论shared_ptr的线程安全

- shared_ptr对象本身的线程安全级别和内建类型，标准库容器，string一样。。。并不是它管理的对象线程安全级别
- 在多个线程中同时访问同一个shared_ptr正确做法是用mutex_保护:
  ```cpp
  MutexLock mutex;
  shard_ptr<Foo> globalPtr;
  //任务是把globalptr安全传递给doit
  //尽量减小临界区长度 线程的局部变量
  void doit(const shared_ptr<Foo>& pFoo);

  void read(){
      shared_ptr<Foo> localPtr;
      //临界区
      {
          MutexLockGuard lock(mutex);
          localPtr = globalPtr;//读全局指针
      }
      doit(localPtr);
  }

  //写入加锁
  void write(){
      shared_ptr<Foo> newPtr(new Foo); //对象的创建在临界区之外
      shared_ptr<Foo> desPtr;
      {
          MutexLockGuard lock(mutex);
          swap(desPtr,globalPtr)
          globalPtr = newPtr;
      }
      desPtr.reset();

      doit(newPtr);

  }


  ```


## shared_ptr技术与陷阱

- 意外延长对象的生命期：只要有一个指向x的对象的shard_ptr存在，该x对象就不会析构
- 然而shared_ptr是允许拷贝构造和赋值的（否则引用计数就是无意义了），若不小心遗留了一个拷贝，那么对象会永世长存
- 若把前面 `std::vector<weak_ptr<Observer>> observers_`改为`std::vector<shard_ptr<Observer>> observers_`---此时必须手动调用unregister()函数，否则observer对象永远不会析构。

- boost::bind会拷贝一份实参，如果参数是一个shared_ptr 那么对象的生命期不会短于 boost::function对象，pFoo是一个share_ptr对象
```cpp
class Foo{
    void doit();
};

share_ptr<Foo> pFoo(new Foo);
boost::function<void()> func = boost::bind(&Foo::doit,pFoo);//long life fooa
```

- share_ptr拷贝开销高于原始指针，通常一个线程只需要在外层函数有一个实体对象，之后都可以使用const reference方式传递使用shared_ptr
  ```cpp
  void save(const shared_ptr<Foo>& pFoo);
  void validateAccount(const Foo& foo);

  bool validate(const shared_ptr<Foo>& pFoo){
      validateAccount(*pFoo);
  }

  //pass by const reference
  void onMessage(const string& msg){ //只有最外层有一个实体 安全不成问题
      shared_ptr<Foo> pFoo(new Foo(msg));
      if(validate(pFoo)){
          save(pFoo);
      }
  }
  ```

- 不会反复拷贝shared_ptr导致性能问题
- pFoo是一个栈上对象，不可能被其他线程看到，读取始终是线程安全的
- 析构动作在创建时被捕获：
  - 虚析构不再必须
  - share_ptr<void> 可以持有任何对象，而且能够安全释放
  - share_ptr对象可以安全跨越模块边界；比如从DLL中返回，而不会造成模块A分配的内存在模块B中释放
  - 二进制兼容性
  - 析构动作可以定制:利用模版参数传入析构函数指针或者仿函数
- 对象析构是同步的。当最后一个指向x的shared_ptr离开作用域时候，x会同时在同一个线程析构。这个线程不一定是对象诞生的线程
  - 析构可能会拖慢关键线程
  - 用一个单独的线程专门做析构，通过`BlockingQueue<shared_ptr<void>>`把对象析构都转移到那个专门的线程
- RAII handle ,RAII(资源获取即初始)，每一个明确的资源配置动作（例如new）都应该在单一语句中指向，并在该语句中立刻将配置获得的资源交给handle对象，（如shared_ptr)
- 避免循环引用：owner持有指向child的shared_ptr，child持有指向owner的weak_ptr



## 对象池

- Stock类，代表一只股票的价格，不同股票用一个字符串唯一标识。同一个程序能够共享Stock对象，如果没有任何地方使用了，对应的Stock对象应该析构。
- 版本1：容器内使用shared_ptr导致对象不会被释放,除非手动erase容器内元素，或者要容器销毁,因此可以存入weak_ptr对象，
  ```cpp
  class StockFctory:boost::noncopyable{
    public:
        shared_ptr<Stock> get(const string &key);
    
    private:
        mutable MutexLock mutex_;
        std::map<string,shared_ptr<Stock>> stocks_;//对象始终有铁丝绑着 不会被销毁
    
  };
  ```

- 版本2:map中存入weak_ptr ,即`std::map<string,weak_ptr<Stock>> stocks_`
  ```cpp
  shared_ptr<Stock> StockFactory::get(const string& key){
      shared_ptr<Stock> pStock;
      MutexLockGuard lock(mutex_);

      weak_ptr<stock>& wkStock = stocks_[key];//如果key不存在，会默认构造一个
      pStock = wkStock.lock();//尝试提升

      if(!pStock){
          //提升失败 对象已经被销毁
          pStock.reset(new Stock(key));
          wkStock = pStock;//更新了stock_s[key],wkStock只是一个引用
      }
      return pStock;

  }
  ```

- 其实还是会有轻微内存泄漏，为啥？
  - 由于stocks_的大小只增加而没有减少。股票数量有限，泄漏内存很小
  - 如果是其他类型的对象池，对象的key的集合也不是封闭的，内存就会一直泄漏
  - 解决方法是shared_ptr的定制解析功能，shared_ptr的构造函数可以有一个额外的模版类型参数，传入一个函数指针或者仿函数d，在析构对象执行d(ptr),其中ptr是shared_ptr保存的对象指针
    ```cpp
    template<class Y,class D> shared_ptr::shared_ptr(Y* p, D d);
    template<class Y,class D> void shared_ptr::reset(Y* p, D d);
    //Y和T类型可能与T不同，是合法的，只要Y*能隐式转化为T*
    ```
- version 3
  ```cpp
    class StockFctory:boost::noncopyable{
    public:
        shared_ptr<Stock> get(const string &key);
    
    private:
        mutable MutexLock mutex_;
        std::map<string,shared_ptr<Stock,deleteStock>> stocks_;//对象始终有铁丝绑着 不会被销毁
    
    private:
        void deleteStock(Stock* stock){
            if(stock){
                MutexLockGuard lock(mutex_);
                stocks_.earse(stock->key());
            }
            delete stock;
        }
    
    };
    shared_ptr<Stock> StockFactory::get(const string& key){
    shared_ptr<Stock> pStock;
    MutexLockGuard lock(mutex_);

    weak_ptr<stock>& wkStock = stocks_[key];//如果key不存在，会默认构造一个
    pStock = wkStock.lock();//尝试提升

    if(!pStock){
        //提升失败 对象已经被销毁
        //pStock.reset(new Stock(key)); 修改如下
        pStock.reset(new Stock(key),boost::bind(&StockFactory::deleteStock,this,_1));
        // this指针暴露 会导致线程安全问题
        wkStock = pStock;//更新了stock_s[key],wkStock只是一个引用
    }
    return pStock;

    }
  ```
  - `boost::bind(&StockFactory::deleteStock,this,_1)`StockFactory的this指针暴露 会导致线程安全问题,StockFactory对象如果先于Stock对象析构就会core dump
  - 利用弱回调技术可以解决。

### enable_shared_from_this
```cpp
class StockFctory:public boost::enable_shared_from_this<StockFactory>,boost::noncopyable{...};


version4

shared_ptr<Stock> StockFactory::get(const string& key){
    shared_ptr<Stock> pStock;
    MutexLockGuard lock(mutex_);

    weak_ptr<stock>& wkStock = stocks_[key];//如果key不存在，会默认构造一个
    pStock = wkStock.lock();//尝试提升

    if(!pStock){
        //提升失败 对象已经被销毁
        //pStock.reset(new Stock(key)); 修改如下
        pStock.reset(new Stock(key),boost::bind(&StockFactory::deleteStock,shared_from_this(),_1));
        // this指针暴露 会导致线程安全问题
        wkStock = pStock;//更新了stock_s[key],wkStock只是一个引用
    }
    return pStock;

}
```
- 将shared_ptr绑到boost::function中，回调时StockFactory对象始终存在，安全。但是延长了对象的生命期，使之不短于boost::function
### 弱回调

- 将shared_ptr绑到boost::function中，回调时StockFactory对象始终存在，安全。但是延长了对象的生命期，使之不短于boost::function
- 有时我们需要：如果对象还活着，就调用它的成员函数，否则忽略之的语义，就行Observable::notifyObservers()那样。这时可以使用weak_ptr,在回调的时候尝试提升为shared_ptr
- **//必须是静态成员函数？为什么**
    - //静态成员函数本身是没有this指针的 因此才能传递shared_ptr的this指针
  ```cpp
  shared_ptr<Stock> StockFactory::get(const string& key){
    shared_ptr<Stock> pStock;
    MutexLockGuard lock(mutex_);

    weak_ptr<stock>& wkStock = stocks_[key];//如果key不存在，会默认构造一个
    pStock = wkStock.lock();//尝试提升

    if(!pStock){
        //提升失败 对象已经被销毁
        //pStock.reset(new Stock(key)); 修改如下
        pStock.reset(new Stock(key),boost::bind(&StockFactory::deleteStock,weak_ptr<StockFactory>(shared_from_this()),_1));

        // this指针暴露 会导致线程安全问题 改为shared_from_this()
        //为了不延长生命期 将shared_from_this()强制转换为weak_ptr
        wkStock = pStock;//更新了stock_s[key],wkStock只是一个引用
    }
    return pStock;

  ```