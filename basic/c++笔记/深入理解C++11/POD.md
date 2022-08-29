# C++11新特性比较重要的

找工作暂时没有时间全部看了，这本书确实不错，找重点看了~



## POD类型



`plain old data`非常重要的一个概念。

什么是POD类型,满足下面两个条件

- 平凡的（trivial）
- 标准布局

#### 平凡的（trivial）

- 拥有平凡的默认构造函数（编译器构造的默认构造函数，且什么都不干）自己写的空的也不行
- 拥有平凡的默认拷贝构造函数（编译器构造的默认拷贝构造函数，且什么都不干），显示设置=default
- 拥有平凡的默认移动构造函数（编译器构造的默认移动构造函数，且什么都不干），显示设置=default
- 拥有平凡的默认拷贝赋值运算符和移动赋值运算符（编译器构造的默认构造函数，且什么都不干）
- 不能包含虚函数和虚基类



```cpp
类模板可以判断是否是一个平凡的类型
template<typename T> struct std::is_trivial
  
  
#include<iostream>
#include<type_traits>
//平凡的
using namespace std;
class trivial
{
public:
    void fun(){}
public:
    int i;
};

int main()
{
    cout<<is_trivial<trivial>::value<<endl; //1
    cout<<is_pod<trivial>::value<<endl; //1
    cout<<is_trivial<nontrivial>::value<<endl; //0
    cout<<is_pod<nontrivial>::value<<endl; //0 

}
```

#### 标准布局

- 所有非静态成员有相同的访问权限
- 类或者结构体继承时，满足两种情况之一
  - 派生类有非静态成员，
- 没有虚函数和虚基类





### POD优点

1. 字节赋值 安全使用memset
2. c内存布局兼容
3. 保证静态初始化安全有效，pod对象初始化（放入目标文件的.bss段，初始化赋0）

