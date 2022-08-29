## c语言编译模型及其成因

- 分离编译：节约内存当时
- C语言运行使用未定义的函数，编译可以通过，但是链接会失败
- C++则会直接报错
- C语言编译模型
  - 单遍编译：从头到尾扫描源码，一边解析代码。编译器只能看到目前语句/符号已经解析过的代码，看不到之后的代码，而且过眼即忘
  - C语言要求结构体必须先定义，才能访问成员
  - 局部变量必须先定义在使用

## C++编译模型
- 单遍编译 影响了名字查找和函数重载决议
- g++ -Wshadow编译比较好
- 函数重载
- 内部是多重编译的，multiple pass
### 前向声明
- 使用前向声明减少编译期依赖
- c++编译器解析某处函数调用时，需要生成函数调用的目标代码。为了完成语法检查编译器需要知道函数参数个数和类型，函数返回类型，不需要知道函数体内的实现（除非inline展开）
- 因此我们可以把函数原型放到头文件中。包含头文件的源文件都可以使用这个函数
- 光有原型不定义，链接时是会报错的（符号未定义）
- 如果说声明和定义不一致（参数写错了）---c++会认为是重载


- 类的前向声明：如果不是访问类的成员，或者要知道大小分配空间。
- 其他时候有类的前向声明就够了，编译器只需要知道有这么个名字
  - 例如：定义声明*Foo或者&Foo，包括用于函数参数，返回类型，局部变量，类成员变量等。
  - 声明一个以Foo参数为参数或者返回值的函数（调用时就需要知道Foo的定义）

- 使用**类前向声明避免减少include**，并且避免把内部class定义暴露给用户代码

-  不能重载&& ，||, 和,(逗号)这三个操作符，同时Google的C++编程规范不能重载&，因为一旦重载了&，这个class就不能前向声明了
  ```cpp
  class Foo;
  void bar(Foo& foo){
      Foo *p = &foo;//是取foo的地址，如果重载了& 意思就变了
  }
  ```


## C++链接

- 作者用整理书为例
- one-pass链接器的工作方式：参数顺序是越基础的库越放到后面，**例如A和B独立，同时依赖于C。这时参数顺序应该是ABC或者BAC**
- 为什么是这样?因为这样可以节约大量内存。如果我们先处理基础库，我们不知道AB会用到C中的哪一些符号，因此只能全部记住，链接器就会造成大量内存浪费。如果我们知道AB中尚未定义的符号，我们在C中只需要记住那些尚未定义的符号就行了

- C++相对于C的连接**增加了两项内容**：
  - 函数重载，需要类型安全的链接，即name mangling
  - name mangling：使用extern "C" 符号修饰和C语言兼容（自我修养第三章目标文件里有什么）
  ```cpp
    extern "C"{
    int func(int);
    int var;
  }
  //或者
  extern "C" int func(int);
    //为了能够灵活使用
    #ifdef __cplusplus
    extern "C"{
    #endif
        void *memset(void*, int, size_t);
    #ifdef __cplusplus
    }
    #endif
  ```

  - vague linkage，同一个符号有多份互不冲突的定义。C++处理单个源文件时，不确定某个符号是否应该在本单元定义，为了保险 只能为每份目标文件生成一个弱定义，而依赖链接器去选择最终的定义


### 函数重载

- **name mangling**：为每一个重载函数生成一个独一无二的名字，例如_Z3foob
  ```cpp
  int foo(bool x){} //_Z3foob
  
  int foo(int x){}// _Z3fooi
  
  int foo(bool x){
    return 42;
  }
  int foo(int x){
    return 102;
  }
  int foo3(int x){
      return 142;
  }
  (base) matytan@matytandeMacBook-Pro code % nm foo.o
  0000000000000000 T __Z3foob
  0000000000000020 T __Z3fooi
  0000000000000030 T __Z4foo3i

  - 非模板函数的mangled name不包含返回类型，返回类型不参与函数重载
  - 一个源文件用到了重载函数，但它看到的函数院校的函数声明类型是错的，链接器无法捕捉这个错误。

### inline函数
- 编译器自动inline
- 头文件里inline还是必须要的，为了防止链接抱怨重复定义。
- C++使用重复代码消除来避免重复定义。编译器无法按inline展开，每个编译单元都会生成inline函数的目标代码，然后链接器从多份实现中选一份保留。其余则丢弃（vague linkage）。

- 如何判断可执行文件是debug build还是relese build 即判断是O0编译还是-O2编译。看类模板的短成员函数有没有被inline展开
- inline中的static变量如何处理？定义多份吗？

  ```cpp
inline void f()
{
  static int i = 0;
}

两个cpp中调用f()两次：
  f();
f();
static int i = 0;
static int i = 0;
  ```
  - 如果是 extern inline 里的 static 变量，那么就一定是同一个对象。


### 模板

- 函数模板和类模板
- 函数定义：具现化后的函数模板，类模板的成员函数，类模板的静态成员函数
- 变量定义：包括函数模板的静态数据变量，类模板的静态数据成员，类模板的全局对象等
  - 模板编译链接不同之处，以上具有extern linkage对象会在多个编译单元内被定义，必须进行重复代码消除（好像是映射到同一个段中）

- 模板的定义需要放在头文件中，否则会链接报错。只声明未定义
- 模板具现化private中可以使用函数模板，因为外部成员无法调用
- externtemplate可以阻止隐式模板具现化

### 虚函数
- 虚函数没有定义，即没有定义虚表，会提示找不到虚表的定义

## 工程项目中头文件的使用规则
- 头文件的害处
  - 传递性：头文件会包含其他头文件
    - include<complex>展开有2w行代码
    - 编译缓慢
    - 头文件如果改变一点点，代码需要重新编译所有直接或间接包含的源文件。
  - 顺序性。一个源文件可以包含多个头文件。
  - 差异性

### 头文件的使用规则
- 将文件间编译依赖降至最低
- 将定义式之间的依赖关系降至最低，避免循环依赖。
- 让class名字，头文件名字，源文件名字直接相关。
- 总是在头文件内写内部#include guard（护套）宏
- #include guard用的名字应该包含文件的路径全名
- 编写程序库，那么公开的头文件应该表达的模块的接口。必要的时候可以把实现细节放到头文件内部。



- STL（Standard Template Library）：就是 Stepanov 用 C++ 模板写的库，后来成为了标准库的一部分。
- RAII（Resource Acquisition is Initialization）：在构造（初始化）对象时获取资源，令资源在对象生命周期结束后自动释放，特别用来避免因异常而导致资源泄漏。是 C++ 重要特性。
- RTTI（RunTime Type Information）：运行时可提供类型的信息，其实就是打开了才可用 dynamic_cast ，以及取得多态对象的 typeid。
- POD（Plain Old Data）：兼容 C 的数据类型。
- ODR（One Definition Rule）：每个变量、函数、类类型等只能定义一次。
- UB（Undefined behavior）：标准中未定义的行为。
- RVO（Return Value Optimization）：编译器避免把返回值做复制。
- NRVO（Named Return Value Optimization）：在函数内直接用调用方的具名变量作为返回值。
- EBO（Empty Base Optimization）：本来每个对象至少要占一字节，但空的基类就免了。
- COW（Copy On Write）：复制对象时只引用原来的数据，直至真的要改动才做一次复制。一些 C++ 的标准库字符串类会用这个优化，但现在来说并不一定好。
- **PIMPL（Pointer to Implementation）：只给调用者指针，不暴露类的定义及细节。真正实现在类指针中**
- LSP (Liskov Substitution Principle)：符合此原则的继承关系中，子类对象可替代父类对象被使用。
- TMP（Template MetaProgramming）：用模版实现元编程。
- ADL（Argument Dependent name Lookup）：用调用参数去查找函数。
- CRTP（Curiously recurring template pattern）：一种常用的模版实现手法，令一个模版类的基模板类使用自己的定义。
- SFINAE（Substitution Failture Is Not An Error）：尝试匹配模板时，如果替代模版参数后出现编译问题，并不当作是错误。C++11 前一些 TMP 会依赖这种特性。
- 加送一个：RTFM（Read The Fxxxing Manual）：不要问了，去读那可爱的手册吧。