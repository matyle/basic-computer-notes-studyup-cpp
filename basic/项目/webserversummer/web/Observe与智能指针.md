
## 原始指针有何不可？
- 单线程中
  - 我们容易忘了delete！
  - raw pointer无法得知对象是否存活
  - a对象中拥有b对象的指针，同时b对象中拥有a对象的指针。总有一个对象要先销毁，销毁其中一个等于也要销毁另一个，然后另外一个调用的时候已经是一个没有指向对象的指针了，造成空悬指针


## 好吧，那我用shared_ptr
不好意思用shared_ptr仍然有问题呀，会造成循环引用
- shared_ptr属于强引用，如果它只要存在，它指向的对象就不会消失
- 循环引用：
  ```cpp
  class B;
  class A{
      public:
        A(){}
        ~A(){}
        shared_ptr<B> b_;

  };

    class B{
      public:
        B(){}
        ~B(){}
        shared_ptr<a> a_;

  };

  main:
    auto a=share_ptr(new A());
    auto b=share_ptr(new B());
    a.b_ = b;
    b.a_ = a;

  ```
  - 假设`auto a=share_ptr(new A());auto b=share_ptr(new B());`
  - shared_ptr对象a中shared_ptr拥有对象b，同时shared_ptr对象b中拥有shared_ptr对象a。
  - shared_ptr对象只要指向的有对象就不会析构,因此a想析构的时候发现自己的引用计数不为0，因为b中还有它的指向，同样的，b也会发现自己的引用计数不为0，不能析构。就导致最后指向两个对象都没有析构。


## weak_ptr神器
- 我只需要知道 其中一个有没有活着就行，我不干预它的生死，它死了我知道就行，我不增加引用计数就行了。


## 貌似没问题了，继续看对象池 那个例子，写了5个版本才一步一步解决了问题。
- 原始指针
- shared_ptr指针
- weak_ptr
- 使用删除器的shared_ptr+weak_ptr
- 避免了this指针暴露导致线程安全问题  使用shared_from_this。但增加了对象的生命期，因为将shared_ptr的this指针作为函数对象的参数
- 弱回调，