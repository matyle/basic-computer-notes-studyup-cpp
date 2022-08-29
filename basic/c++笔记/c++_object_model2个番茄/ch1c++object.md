#关于对象

## point3d的不同构造



### c语言

c语言中，数据抽象和一些实施它的一些操作是完全分开的（对于函数和数据之间的关系无语言层面支持），称为一个面向过程的语言（它由一系列算法把分成基于任务的函数操作共享外部数据所驱动）

```c
typedef struct point3d
{
  float x;
  float y;
  float z;
}Point3d;

void Point3d_print(const Point3d *pd)
{
  printf("%f,%f,%f",pd->x,pd->y,pd->z);
}

//使用宏
#define Point3d_print（pd)\
printf("%f,%f,%f",pd->x,pd->y,pd->z);

void my_foo()
{
  Point3d *pd = get_a_point();
  printf("%f,%f,%f",pd->x,pd->y,pd->z);
}

```

函数：

- 使用宏替代函数更加高效
- 或者写成一个独立代码段

访问数据：

- 直接访问

  ```c
  Point3d pd;
  pd->x = 0.0;
  ```

- 或者使用宏



c++中使用类似的独立抽象数据类型ADT

- c++中写入一个类中，或继承类，或参数化带模版的类

  ```cpp
  class point3d{
  public:
    ...
  private:
    ...
  };
  
  //写为继承
  class point{
  public:
    	...
  private:
    	...
      int x;
    	int y;
  };
  
  class point2d:public point{
  public:
    ...
  private:
    ...
  };
  
  class point3d:public point2d{
  public:
    ...
  private:
    ...
  };
  
  
  //模板类
  template<typename T>
  class point
  {
  public:
    
    
  private:
    T x;
    T y;
  };
  
  //带维度的模板类
  template<typename T,int dim>
  class point
  {
  public:
    //按维度初始化
    point(T coords[dim])
    {
      for(int i=0;i<dim;++i)
      	coords_[i] = coords[i];
    }
    
  private:
    T _coords[dim];//若dim=3 即数据3个元素代表x,y,z
  };
  ```

C++的数据封装和继承体系比全局变量面向过程的C为什么更好？



讨论C++不好的人认为它更为复杂，但不一定更有威力或更好。就算两者都有，也不一定适用

首先是对象加上封装之后的布局成本

- 其实并未增加成本！！！
- 三个非静态数据成员与c结构体一样
- 成员函数虽然在类中声明，但并未存在于对象之中。
  - 每一个non-inline非内联函数只会产生一个函数实例（copy）
    - 非静态成员函数：它的默认参数第一个会加上`*this`，对象利用this指针取调用自己的函数
    - 静态成员函数：所有类共享这个函数，没有`this`指针,没有this指针当然无法直接调用对象数据。
  - 而inline函数会在每一个模块（编译单元）产生一个实例（联系程序员的自我修养）

**增加的成本 **

​	1. 因为c++布局和存取成本主要负担由virtual函数引起

- virtual function 虚函数：用来支持一个有效率的“动态绑定”
- virtual base class  ：实现多次出现在继承体系中的基类 有一个单一而被共享的实体

因此c++并不比C大或者更慢

	2. 多重继承：子类和第二基类或后继基类的转换



结论：一般情况下C++不会比C差

### c++对象模型

类数据成员：static和non-static

三种成员函数：static， non-static， virtual

```cpp
class Point
{
public:
  Point(float xval);
  virtual ~Point();
  
  float x() const;
  static int PointCount();
  
protected:
  virtual ostream & print(ostream &os) const;
  
  floax x_;
  static int point_count_;
};
```

静态函数和非静态函数区别？

静态成员和非静态成员区别？

如何塑造着各种数据成员和成员函数?

- 简单对象模型
- 表格对象模型
- C++对象模型

#### 简单对象模型

- 一个对象对于多个slot槽，每一个slot指向一个成员
- 通过index访问对象

每个slot中只存指向对象的指针，因此一个对象大小未指针大小乘成员数目

- 这种模型并未实际使用，但它的index或solt号，发展成了**指向成员的指针**的概念被广泛用

优缺点？

- 简单，最小化编译器的复杂度。而缺点是空间的花费以及执行时的效率

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-09-16-23-14.png
width=300px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">简单对象模型</div>
</center>

#### 表格驱动

- 两个slot 每个slot的指针分别指向数据成员表和成员函数表
  - 指向data的指针（实际数据）
  - 指向成员函数地址表的指针
- 指向成员函数地址的思想被虚函数采用
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-09-16-36-52.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">表格驱动</div>
</center>

#### c++对象模型

对象内

- 非静态数据成员被放在对象内，
- 虚函数

对象外：

- 静态数据成员放在类对象外
- 成员函数全部放在类对象外

虚函数的实现

- 虚表（每个类一个），虚表保存了每个虚函数的函数指针，因此是对象共享整个虚表
  - 虚表内有函数指针
  - 有type_info对象
- 每个==类对象== 有一个指针指向相关的虚表（虚表指针vptr)
- 虚表指针是由构造函数，拷贝构造，xx构造，赋值运算符自动生成代码

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-09-16-40-41.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">c++对象模型</div>
</center>
加上继承 
- 单一继承
- 多重继承
- 虚继承

关键词的差异
- class 和 struct
- 声明还是调用？

对象的差异
- 程序模型（过程模型）：c语言
- 抽象数据模型ADT（值语义）
- 面向对象模型：可以直接处理基类对象，但是只能通过指针或者引用来间接处理（多态）
  - 占用内存
    - 非静态数据成员大小总和（包括对象或者指针变量，只要是非静态数据）
    - 对齐：如何对齐？
    - virtual产生的额外的负担
  - 利用指针或者引用实现 函数调用会动态完成
  - 1. 将派生类的指针转为指向其公共继承的基类指针
    ```cpp
        shape *ps = new circle();
    ```
  - 2. 虚函数机制
    ```cpp
        ps->rotate();
    ```
  - 3. 利用dynamic_cast和typeid运算符
    ```cpp
        if(circle *pc = dynamic_cast<circle*>(ps));
    ```
- 指针的类型 ：指针类型会引导编译器如何解释某个特定地址中内存内容及其大小。
```cpp
class ZooAnimal{
    public:
        ZooAnimal();
        virtual ~ZooAnimal();
        //..
        virtual rotate();
    protected:
        int loc;
        string name;
}
ZooAnimal za("zoey");
ZooAnimal *pza = &za;
```

<center>
  <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-10-20-23-58.png
  width=400px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>

  - 加上多态之后
```cpp
  class Bear:public ZooAnimal{
      public:
          Bear();
          virtual ~Bear();
          //..
          void rotate();
          virtual void dance();
      protected:
          enum dances{};
          Dences dences_known;
          int cell_block;
  }
```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-10-20-35-49.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>
- 再多一个panda 继承于Bear
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-10-20-43-03.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- 派生类中有虚表吗？
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-10-20-56-15.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">子类虚表构造过程</div>
</center>
   

