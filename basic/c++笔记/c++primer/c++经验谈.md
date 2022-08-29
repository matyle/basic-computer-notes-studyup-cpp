
## 不要重载全局::operator new()


## 整数转字符串
- 利用一个digits[19]数组，解决负数问题
  ```c
  const char* convert(char buf[],int value){

  }
  ```

  - C语言中整数除法/和%号对于负数是implementation-defined.
  ```c
  int q = m / d;
  int r = m % d;
  //只能保证
  //m = q * d + r
  ```
  - 如果m和d中有负号，q和r的正负号不定
  - c++ 11默认商向0取整



## 在单元测试中mock系统调用

## 慎用匿名namespace
```cpp
namespace //匿名namespace
{
    void f();
}
```
- 对于一个大型的C语言软件项目，给函数和全局变量起名不是一个容易的事情，因为必须考虑有没有可能与其它程序员写的代码冲突，多数的做法是对每个模块的一组函数名加个特定前缀，如HTRequest_setInternal、HTRequest_internal等。


### c语言static关键字的两种用法
- 第一种：用于函数内部修饰变量，函数内的静态变量。生存期长于函数，使得函数具有一定状态，不可重入
- 第二种：用在文件级别，函数体之外。修饰变量或者函数，表示该函数或者变量只在本文件内可见 具有internal linkage

### c++ static四种用法
- 与c兼容2种
- 第3种：用于修饰类的数据成员，即静态成员。生命期大于类对象（实体）。静态数据成员是每个类只有一份（类变量class variable），而普通成员是每个对象都有一份(instance variable)。
- 第4种：修饰类的成员函数， 即静态成员函数。这种成员函数没有this指针，只能访问静态成员变量和其他静态函数。整个类只有一个实例，即可以直接类::调用，也可以对象调用。


### 代码风格
- 使用//注释
- 局部变量，一行代码定义一个变量
- 如果函数参数数量大于三个，那么应该在逗号后面换行
- 类的初始化列表的写法，也应该遵循一行一个的准则(列表初始化顺序应该和声明顺序相同)
  ```cpp
  class Buffer
  {
      Buffer()
        :buffer_();
         readIndex_();
  };
  ```

  - namespace不增加缩进

- 利于版本管理的代码，版本管理Git/Hg
  


## 再探std::string

- 直接拷贝（eager copy）
- COW（copy on write)，对多线程不友好 g++
  - string对象里只放一个指针。
  ```cpp
  class cow_string
  {
      struct Rep
      {
          size_t size;
          size_t capacity;
          size_t refcount;
          char* data[1];
      };
      char* start;
  };
  ```
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src= img/2021-07-06-20-08-54.png
  width=450px>
  </center>
  

- SSO短字符串优化
  - 用了一个本地缓冲区
  ```cpp
  class sso_string
  {
      char* start;
      size_t size;
      static const int kLocalSize = 15;

      union
      {
          char buffer[kLocalSize+1];
          size_t capacity;
      }data;
  };
  
  ```