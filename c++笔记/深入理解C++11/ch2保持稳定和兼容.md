## 常见的宏





### 宽窄字符串连接

- char（窄字符）转宽字符(w_char_t)





### long long类型







## 扩展的整型







## 宏__cplusplus

```cpp
#ifdef __cplusplus
extern "C"
{
#endif

#ifdef __cplusplus
  
}
#endif
```



## 静态断言



```cpp
#include<cassert>
```



- NDBUG宏



### 静态断言和static_assert





- 模板实例化在编译时期（也叫编译时多态），这时候应该产生断言，而不是在运行时。

```cpp
template<typename T,typename U> int bit_copy(T& a, U& b){
    //编译时期报错
    static_assert(sizeof(a)==sizeof(b),"should have same width"); //判断相同
    memcpy(&b,&a,sizeof(b));
    return b;
}
```



## noexcept修饰符和noexcept操作符

- 库作者



noexcept修饰符可以有效的阻止异常的传播与扩散

```cpp
#include<iostream>
using namespace std;
void Throw(){throw 1;}
void NoBlockThrow(){Throw();}
void BlockThrow() noexcept{Throw();}
int main()
{
    try
    {
        Throw();
    }
    catch(...)
    {
        cout<<"Found throw."<<endl;
    }
    try
    {
        NoBlockThrow();
    }
    catch(...)
    {
        cout<<"Found throw is not blocked."<<endl;  //terminate called after throwing an instance of 'int'
    }

    try
    {
        BlockThrow();
    }
    catch(...)
    {
                cout<<"Found throw 1."<<endl;
    }
}

Found throw.
Found throw is not blocked.
terminate called after throwing an instance of 'int'
Aborted (core dumped)
```





## final和override控制



- final关键字使派生类不可覆盖它所修饰的虚函数

- 虚函数描述符override关键字作用

  - 检查是否在重载一个接口或重载的接口名称是否有错误

  - 派生类虚函数声明时使用了override描述符，那么该函数必须是重载其基类中的同名函数，否则无法编译

    ```cpp
    class Base
    {
        virtual void foo() = 0;
        virtual void fun() = 0;
        virtual void funcon() const;
    
        void print();
    };
    class MidDerived:public Base
    {
        virtual void midfun() final;
    
    };
    class TopDerived:public MidDerived
    {
        void foo() override;
        void funn() override; //拼写错误 并非重载
        void print() override; //print不是虚函数 
        void funcon() override;//报错 常量性不一致 并非重载
    };
    
    ```



## 模板函数的默认模板参数

- c++ 11 支持模板函数的默认模板参数
- 多个模板参数时如何指定？





## 外部模板



- 什么是外部模板？
  - 类似于`extern int i` ，只不是不是数据，而是函数代码

```cpp
//test.h
template<typename T> void fun(T){}


//test1.cc 生成test1.o
#include "test.h"
void test1()
{
    fun(3);
}

//test2.cc 生成test2.o
#include "test.h"
void test2()
{
    fun(4);//类型相同
}
```

- 这段代码模板实例化时test1.o,test2.o都会产生相同的实例代码，虽然在链接时能够只保留其一但是还是有浪费



因此出现了外部模板，加上extern关键字，基本和共享变量道理一样，只实例化没加extern的模板

```cpp
//test.h
template<typename T> void fun(T){}


//test1.cc 生成test1.o
#include "test.h"
template void fun<int>(int); //显式实例化
void test1()
{
    fun(3);
}

//test2.cc 生成test2.o
#include "test.h"
extern template void fun<int>(int); //不会实例化
void test2()
{
    fun(4);//类型相同
}
```







