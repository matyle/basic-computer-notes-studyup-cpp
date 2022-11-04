## 朴实的C++设计

## 程序的二进制兼容性

### 什么是二进制兼容性ABI

- 举例
  ```c
  open(const char* pathname,int flags);
  ```
  - 对于flag的取值O_RDONLY,O_WRONLY,O_RDWR 不满足按位或的关系，因为他们分别是0，1，2所以不满足，为啥不修改一下？简单的修改头文件就行了
  - 因为修改了会破坏二进制兼容性，对于编译好的二进制文件调用open参数是写死的。
- 本章的二进制兼容性是指在升级了库文件的时候，不必重新编译使用了这个库的可执行文件和其他库文件。

### 哪些情况会破坏库的ABI
- 函数参数传递方式 codec从右到左
- 虚函数调用方式（通常是虚表指针虚表机制）然后用vtbl[offset]调用
- struct和class的内存布局
- name mangling 名称修饰
- RTTI和异常处理的实现

### 解决办法

- 静态链接
- pimpl技法：使用指针管理，只给调用者指针

### 避免使用虚函数作为库的接口

- 使用PImpl手法作为编译防火墙

  ```cpp
  ```

  

- 使用std::function和std::bind取代虚函数

  - 做法：function类似于c#的delegate，可以指向任何函数，bind将某个成员函数绑定到某个对象上面
  - 成员函数必须要引用& 
  ```cpp
    using namespace std;
    void Foo::methodA(){
        cout<<"method A"<<endl;
    }
    void Foo::methodInt(int a){
        cout<<"methodInt:"<< a <<endl;
    }
  
    void Foo::methodString(const string &str){
  
        cout<<"methodStr:"<< str <<endl;
    }
  
    void Bar::methodB(){
        cout<<"method B"<<endl;
    }
  
    void methodC(int c){
        cout<<"method C"<<endl;
    }
    int main()
    {
        typedef function<void()> Functor; //定义函数对象名
        Functor f1;                       // 无参数无返回值
        Functor f2;//
        Functor f3;
        Foo foo;
        f1 = bind(&Foo::methodA, &foo);
        f1();//调用f1
  
        f2 = bind(&Foo::methodInt,&foo,42); //绑定this指针类似
        f2();
  
        f3 = bind(methodC,40);
  
        f3();
  
  }



### 对程序库的影响

- 继承是一种第二强的耦合（最强的是友元）
- 线程库的实现的例子 function和bind的使用
- 以及server和echo的回调函数



## iostream的局限与用途

- iostream用途：初学者方便的命令行输入输出，真实项目中很少用到iostream

### stdio格式化输出输出的缺点

  ```c
    int i ;
    short s;
    float f;
    double d;
    char name[80];

    scanf("%d, %hd, %f,%lf,%s",&i,&s,&f,&d,name);
    printf("%d,%d %f %f %s",i,s,f,d,name);
  ```
- 输入和输出的格式化字符不一样
- 输入参数不统一，scanf需要&，而对于字符数组则不取地址
- 缓冲区溢出的危险，例如name


- 正确的做法
  - 安全性
  - 类型安全


### iostream的设计初衷
- 高效可扩展的类型安全的IO机制

  
### “值”语意和“对象语意“

- 
- iostream是对象语意的，表示non-copyable，这是正确的。
- 禁止拷贝，利用对象的生命周期来明确管理资源，这就是RAII


- std::stringg
  - iostream与std::string配合的很好，有一个问题：谁依赖谁？
  - operator<<是一个二元操作符，参数是std::ostream和std::string。那么string的头文件中要不要#include<iostream>?
  - 定义<iosfwd>头文件，包含他们的前向声明。
  - 注意：istream::getline()成员函数的参数类型是char* 而不是string。而std::getline()是一个非成员函数，定义在string中



### iostream在使用方面的缺点
- 格式化输出繁琐
- stream的状态：例如输出16进制，后面的cout都会输出16进制
- 线程安全和原子性

### iostream在设计方面的缺陷
- 面向对象设计：多重继承和虚拟继承
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-07-05-15-39-34.png
  width=450px>
  </center>
  
  
  
  


- read(),write(),seek(),fcntl()
  


- 整数转换

## 值语义和数据抽象

- 什么是值语义：对象的拷贝与原对象无关。就像拷贝int一样。拷贝之后就与原对象脱离关系。
- 值语义对应的是”对象语义“或者叫引用语义。对象语义是值面向对象意义下的对象，对象拷贝是禁止的。例如muduo中Thread是对象语义。拷贝一个线程对象是无意义的。
- 值语义可以是mutable的，例如vector<>,string
- 值语义不一定是POD，例如string就是不是POD
- 值语义对象不一定小，例如vector<>对象可大可小

### 值语义与生命期
- 值语义最大的好处是生命期管理很简单，跟int一样。

- 相反对象语意不能拷贝，因此只能用指针或者引用来使用！

- son and parent问题
  - a parent has a child
  - child knows its parent;
  - c++中利用原始指针作为成员，可能会出现空悬指针(谁来释放各种指针，保证指针有效性），因此使用智能指针
  - 而直接使用shared_ptr又会出现循环引用，谁该用weak_ptr?
  ```cpp
  //错误写法
  class Child;
  class Parent:noncopyable{
      Child* mychild;
  };
  
   class Child:noncopyable{
      Parent* myParent;
  };
  ```

  那么谁该是weak_ptr呢？

    - 如果child的生命期由parent控制，child的生命期小于parent，则Parent类中的child指针成员使用scope_ptr
    - 如果相互独立可以将Child类中Parent指针改成weak_ptr （改parent类中的child指针也是可以的）

  


  - 第二个模型：一个孩子有爸妈，爸妈可能有多个孩子（vector),爸妈知道自己配偶
  - 使用智能指针 将裸指针转换为值语义（**实际上是利用shared_ptr对象来管理对象语意的对象，shared_ptr是一个值语义的对象**）


  ```cpp
#include<memory>
#include<vector>
#include<iostream>
#include<string>
//具有对象语意 不可拷贝 应该是noncopyable的,应该继承noncopyable
//不需要都用weak_ptr 其中之一用就行了

class Parent;
typedef std::shared_ptr<Parent> ParentPtr;

class Child{
public:
    //std::weak_ptr<Parent> Parent_;
    std::weak_ptr<Parent> mom_; //赋值之后引用计数为2
    std::weak_ptr<Parent> dad_; //赋值之后引用计数为2
    Child(const ParentPtr& dad, const ParentPtr& mam){
        std::cout<<"Child constructor function"<<std::endl;
        
    }
    ~Child(){
        std::cout<<"Child deconstructor function"<<std::endl;
    }
    // void print(){
    //     std::cout<<"Parent_ count:"<<Parent_.use_count()<<std::endl;
    // }

};

typedef std::shared_ptr<Child> ChildPtr; //child

class Parent:std::enable_shared_from_this<Parent>{
public: 
    std::weak_ptr<Parent> mySpouse;

    std::vector<ChildPtr> myChildren;// 孩子们
    Parent(){
        
        std::cout<<"Parent constructor function"<<std::endl;

    }
    ~Parent(){
        std::cout<<"Parent deconstructor function"<<std::endl;
    }

    void addChild(const ChildPtr& child){
        myChildren.push_back(child);
        Child_.reset(new Child(shared_from_this())); //直接给this可能会出现线程安全问题
    }

    void setSpouse(const ParentPtr &spouse){
        
        mySpouse = spouse;
        
    }

    // void print(){
    //     std::cout<<"Child_ count:"<<Child_.use_count()<<std::endl;

    // }


};
}

  ```

### 值语义与标准库

- c++要求凡是能放进容器的 都必须具有值语义的。 因此会默认实现拷贝构造函数和赋值符
- 除非明确禁止。禁止可以总是继承noncopyable类
  

### 值语义与C++语言
- 本质上是值语义的，所以我们采用传值和传引用的取舍
- 值语义是c++语言的三大约束之一，设计初衷就是让用户定义的类和内置类型int一样，具有同等地位
  - 类的layout和struct相同，没有额外开销。包含一个只有int的class对象开销和定义一个int一样
  - 类的数据成员默认都是未初始化的。因为局部变量也一样
  - 类的数组就是一个个类对象挨着，因为int数组也这样
  - 编译器默认生成拷贝构造函数和赋值运算符
  - 当把类类型传入函数时，默认是拷贝，因为int也是
- 挨着存性能更好，由于局部性原理
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-05-17-13-33.png
width=450px>
</center>


### 什么是数据抽象
- c++的强大之处在于，**抽象**不以性能损失为代价
- **数据抽象和面向对象并列的一种编程范式**（programming paradigm）
- ADT 数据抽象类型：数据抽象是用来描述数据结构的。
- ADT表现为支持某些操作，比如stack::push,stack::pop()，同时隐藏了实现细节
- ADT和对象很像呀？是很像，但是ADT通常是值语义，对象通常是对象语义
- ADT class是可以拷贝的，拷贝之后的instance和原instance脱离关系
  
- C++标准库中的数据抽象
  - complex<>,pair<>,vector<>,list<>,map<>,set<>,string,stack<>,queue<>都是数据抽象的例子

- **数据抽象和面向对象的区别**
  - data abstraction 数据抽象
  - object-based 基于对象
  - object-oriented 面向对象
  - 面向对象的三大特征：封装，继承，多态。而基于对象只有封装，即只有具体类，没有抽象接口。两个都是对象语义（不可拷贝）
  - **面向对象真正核心思想是：消息传递（messaging）**，封装继承多态只是表象。看看孟岩的文章
  - 数据抽象是值语义（可拷贝，赋值），例如muduo中的buffer类
  - ADT是针对数据的，ADT class可以作为面向/基于对象类的成员，反之不成立


### 数据抽象所需语言设施
- 支持数据聚合（data aggregation）即定义c-style struct。有关数据都放到一个struct。

- 全局函数和重载。
- 成员函数和private。声明为private能够防止外界意外修改。成员函数表示ADT中能够定义操作，读取修改私有数据，而不是只能使用全局函数操作
- 拷贝控制。拷贝控制是实现深拷贝的必要手段，c语言中没有拷贝控制。默认拷贝构造函数是浅拷贝（如果拷贝的成员是一个指针）。浅拷贝就是指针赋值！指向的是同一块空间