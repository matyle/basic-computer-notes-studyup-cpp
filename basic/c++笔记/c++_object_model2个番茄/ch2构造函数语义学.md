### 构造函数语义学
- implicit：暗中的、隐式的（通常意指并非在程序源代码中出现的）
- explicit：显式的（通常意指程序源代码中所出现的）
- trivial：没有用的
- nontrivial：有用的
- memberwise：对每一个member施以……
- bitwise：对每一个bit施以……
- semantics：语意

#### 2.1 默认构造函数的构建操作

- 在需要的时候才会生成默认构造函数（编译器需要或者人需要）
  - 如果是程序员需要，必须显式声明
- 带有默认构造函数的成员类对象
  - 如果说本身没有构造函数，而类中存在一个成员对象的含有默认构造函数
```cpp
class Foo{public: Foo(),Foo(int)...}
class Bar{public: Foo foo,char* str...}
void foo_bar(){
  Bar bar;
  if(str){}...
}
```

- 带有默认构造函数的基类
  - 没有任何构造器的类派生于一个带有默认构造函数的基类

- 总结：
  - 有四种情况编译器会自动合成默认构造函数：调用成员对象的或基类的默认构造函数；为每一个对象初始化其虚函数或虚基类。
  - 并不是所有类都会合成默认构造函数
  - 合成的默认构造函数并不会初始化data
  
#### 2.1 拷贝构造函数的构建操作

1. 对一个对象做明确的初始化操作 =

2. 当对象被当作参数交给某个函数，以及传回一个类对象时
3. 一个类对象以另外一个同类实体作为参数（明确定义）
- 以上三种情况都会构建拷贝构造函数
   ```cpp
   class X{...}
   X x;
   X xx = x;

   extern void foo(X x);
   void bar(){
     X xx;
     foo(xx);
   }
   //
   X bar(){
     X xx;
     return xx
   }

   //
   X::X(const X& x);
   ```

#### 缺省成员化的初始化 （default memberwise initailzation）
- 没有显式的拷贝构造函数？把每一个内建或者派生的数据成员（如指针或数组）的值拷贝到另一个对象。不会直接拷贝**成员类对象**，而是会递归施行memberwise initailzation,如下:如果拷贝Word中的String类对象word，则会逐次拷贝word中的数据成员成员

```cpp
class Word{
  public:
    //...无显式的拷贝构造函数
  private:
    int _occurs;
    String word;
  
}

```
- 总结：拷贝构造函数和构造函数在必要的时候才由编译器产生出来。

#### Bitwise copy semantics(位逐次拷贝)

```cpp
#include "Word.h"
word noun("book");

void foo(){
  Word verb = noun;
  //...
}
//这时verb根据noun初始化，但是我们不知道这个初始化的行为是否正确
//第一种声明情况情况 位逐次拷贝 verb的初始化操作不需要以一个函数调用收场
//后果严重的是 局部类对象verb退出foo()时会被销毁，同时他指向的字符指针也会被删除，这时noun就会指向无意义之物。
class Word{
  public:
    Word(const char*);
    ~Word(){delete []str};
  private:
    int cnt;
    char *str;
}

//如果这么声明 未展现位逐次拷贝
//这种情况下 编译器必须合成一个拷贝构造函数
class Word{
  public:
    Word(const char*); //构造函数
    ~Word(){};
  private:
    int cnt;
    String str;
}
class String{
  public:
    String(const char*);
    String(const String&); //拷贝构造函数
    ~String();
    //...
}

//合成的拷贝构造函数
inline Word::Word(const Word& wd){
  str.String::String(wd.str);
  cnt = wd.cnt;
}
```
#### 不要 Bitwise copy semantics位逐次拷贝

- 不展现Bitwise copy semantics的情况
  - 当类中含有一个类成员，而后者的类声明有一个拷贝构造函数时。
  - 基类中存在拷贝构造函数
  - 声明了虚函数
  - 派生自一个继承串链 其中有一个或者多个虚基类


#### 重新设定虚表指针
- 编译期间的两个程序的扩张操作（只要有一个类声明了一个或者多个虚函数就会如此）
  - 增加一个虚表,内含每一个有作用的虚函数地址
  - 指向虚表的指针，安插在每一个类对象中
```cpp
class ZooAnimal{
  public:
    ZooAnimal();
    virtual ~zooAnimal();
    virtual void animate();
    virtual void draw();
  //...
  private:
    //...
};
class Bear:public ZooAnimal{
  public:
    Bear();
    void animate();
    void draw();
    virtual void dance();
    //..
  private:
    //...成员函数需要的数据
}
Bear yogi;
Bear winnie = yogi;//yogi的虚指针被设定指向Bear的虚表，直接拷贝给winnie是安全的

//如果这样
ZooAnimal franny = yogi;//发生切割行为 frany的虚表不能指向yogi的虚表
```
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-18-17-34-28.png
width=500px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

- 程序转为语意学
  ```c
  #inclue "x.h"
  X foo(){
    X xx;
    return xx;
  }
  ```
  - 明确的的初始化
  - 参数的初始化
  - 返回值的初始化
    - 实际上编译器会加一个 返回类型的引用，然后把要返回的对象使用拷贝构造函数给引用，实际并没有返回任何东西
    - c++中有一个叫NRV优化（Named Return value)

#### 成员初始化队伍（member initialization list）

```cpp
Word::Word():_cnt(0),_name(0);
//c++伪代码 编译器操作
Word::Word(){
  _name.String::String(0);
  _cnt = 0;
}
```
- 编译器会一一操作初始化列表，以适当次序在构造函数内安插初始化代码，并且在任何显式的用户代码之前
- 注意是声明次序之前！如代码
  ```cpp
  Word::Word():_cnt(0),_name(0);
  //c++伪代码 编译器操作
  Word::Word(){
    _name.String::String(0);
    _cnt = 0;
  }
  ```
- 初始化次序和排列次序的混乱会导致未初始化赋值的行为；
  ```cpp
  class X{
    int i;
    int j;
  public:
    X(int val):j(val),i(j){}
    ...
  };
  //实际上是会先执行i=j 而j未初始化
  //可改为
  class X{
    int i;
    int j;
  public:
    X(int val):j(val){
      i = j;//j(val)会插入在任何显式的用户代码之前
    }
    ...
  };
  ```