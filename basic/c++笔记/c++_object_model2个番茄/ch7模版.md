## Template 模版

### 模版的具现行为
```cpp
template<typename T>
class Point{
    Point(T x):_x(x){}
};
```
-  编译器看到template类声明时，是没有任何反应的
-  取决于template的实现，编译器不知道T的任何实现
-  编译器可以决定要不要实现模版类

### 模版错误报告
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-07-15-31-29.png
width=490px>
</center>
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-07-15-31-55.png
width=490px>
</center>

- 要根据模版的具体实现

### 模版中的名称决议方式
- scope of the template definition 定义出template的程序
- scope of the template instantiation 具现出模版的程序
  ```cpp
  //scope of the template definition
  extern double foo(doubel);

  template<clas type>
  class ScoreRlues{
    public:
        void invariant(){
            _member = foo(_val);
        }
        type type_dependent(){
            return foo(_val);
        }
    private:
        int _val;
        type _member;

  };
  //scope of the template instantiation
  extern int foo(int)
  ScopeRules<int> sr0

  //
  sr0.invariant();  //会调用哪个？foo 调用definition也就是double foo 与参数无关
  sr0.type_dependent();//调用instantiation ，也就是 int foo
  ```
### 成员函数的具现行为
- 编译器两个策略
  - 编译期时的策略：程序代码必须在程序text file段中备妥可用
  - 链接时期策略：


## 异常处理
- 要支持异常处理，编译器主要工作是找出catch子句。以处理被丢出来的异常

### 异常处理方法
- throw 抛出一个异常，可以是内建类型， 也可以是自定义类型
- catch子句，每一个catch子句都是一个异常处理handler
- try语句，可能会引发catch子句起作用 ,如果没有try语句就不会和catch吻合
- 当一个exception丢出时，控制权会从函数中被释放，然后去找一个相应的catch子句。如果没有吻合的就调用默认的terminate()。。
  ```cpp
  Point* 
  mumble()
  {
      Point *pt1,*pt2;
      pt1 = foo(); //如果foo在这里抛出异常，mumble()函数被推出程序堆栈
      if(!pt1) return 0;
      Point p;

      pt2 = foo(); //如果在这里抛出异常,在推出函数之前，需要先析构p
      if(!pt2) return pt1; 
      ....
  }

  //管理共享内存时，如果在加锁和解锁直接发生了exception就会，未解锁以及未析构对象就结束了。
  //因此我们需要try语句对这一区间进行测试
  ```

- 因此资源管理最好是将资源封装成一个资源类对象之中
    - 智能指针


### 对异常的支持

- 检验发生的throw的函数
- 决定throw操作是否发生在try区段中
- 如果是，则编译系统把异常类型与catch子句比较
- 如果吻合， 就把流程控制应该交到catch子句中
- 如果throw发生并不在try中，或者没有吻合的catch子句
  - a. 摧毁所有的activa 局部变量。
  - b. 从堆栈中推出函数
  - c. 进行到程序堆栈的下一个函数

- 决定throw是否在一个try区段
  - try语句段外的区域，没有激活的局部对象需要析构
  - try语句段外的区域，有激活的局部对象需要析构
  - try段以内的区域
  - 编译器使用program counter-range表格表示区域
  - 其中pc（program counter)记录下一个即将执行的程序指令

### 执行期识别
- 向下转型
- 安全向下转型
- 虚表引入
  - 虚表的第一个表项存放着type_info对象的地址（指针），取得描述符交给runtime_library

### 引用并不是指针
- 引用不能像指针那样把自己设为0.

### typeid运算符

- 使用typeid可以以一个引用达到相同的运行期替代路线
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-09-11-12-50.png
width=490px>
</center>

