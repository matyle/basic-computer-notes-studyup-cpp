### 1.[push_back vs emplace_back](https://stackoverflow.com/questions/4303513/push-back-vs-emplace-back)

push_back

emplace_back:直接在容器中构造对象



###  typeinfo 虚函数和纯虚函数

typeinfo是一个对象，用来标记对象类型是否相同

### 编译相关

- extern "C" :被 extern "C" 修饰的变量和函数是按照 `C` 语言方式编译和连接的。

- 被 extern "C" 限定的函数或变量是 `extern` 类型的。

首先看看 `C++` 中，在未加 extern "C" 声明时，对类似 `C` 的函数是怎样编译的。

```c
void foo( int x, int y );

```

`C` 编译器编译后在符号库中的名字为 `_foo` ，而 `C++` 编译器则会产生像 `_foo_int_int` 之类的名字（不同的编译器可能生成的名字不同，但是都采用了相同的机制，生成的新名字称为 `mangled name` ）。

extern "C" 这个声明的真实目的，就是实现 `C++` 与 `C` 及其它语言的混合编程。

```c
#ifdef __cplusplus             //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
extern "C"{
#endif

/*…*/

#ifdef __cplusplus

}
#endif
```



## c++

### 什么时候用堆，什么用栈

首先应该思考两者的区别呀

- 分配方式不同
  - 碎片不同
- 空间大小不同
- 生长方向不同
- 效率不同？
  - 栈的效率更高，毕竟和寄存器打交道
  - 堆涉及到库函数的内存管理（分配合并等） 可能还会有系统调用

栈就没缺点吗？

- 它快但是空间有限（编译器做主）
- 同时是由编译器管理，分配空间不灵活
- 栈对象生命期随着函数作用域结束而结束

堆为啥就不能有完全垃圾回收？

- 可以有，真正实现的话就要把指针给你屏蔽了才行
- 因为你有地址你能干事，如果给你回收了内存，你还有地址再次使用咋办？
- 指针隐藏问题

什么时候用堆

- 动态分配数组
- 返回局部对象的指针
- 需要分配大的空间

什么时候用栈

- 静态分配：局部变量，编译器时期分配

- 动态分配：malloca在栈上分配，编译器进行释放，无需我们手工实现。可以理解为栈帧被占用。

### 请你说说左值右值

从 ++i 和 i++说起

i++ 是会产生一个中间变量的 返回一个左值

++i 返回一个右值 

左值（在存储单元内的值）通常来说有这几种

An lvalue (so called, historically, because lvalues could appear on the left-hand side of an assignment expression) designates指定 a function or an object.

Example: If E is an expression of pointer type, then *E is an lvalue expression referring to the object or function to which E points. As another example, the result of calling a function whose return type is an lvalue reference is an lvalue. — end example ]

- 变量表达式
- 可以取地址的变量
- 注意左值可以当右值使用

右值（不在存储单元，可能在寄存器内也可能是立即数）：让一个将亡临时对象活过来

—  An rvalue (so called, historically, because rvalues could appear on the right-hand side of an assignment expressions) is an xvalue, a temporary object (12.2) or subobject thereof, or a value that is not associated with an object.

- 字面值常量，立即数

- 不跟对象关联的值

- 临时对象或其子对象

- 返回对象的引用（xvalue）

- 返回值（寄存器内的值，pvalue）返回的不是引用

  ![Taonomy](牛客1.assets/GNhBF.png)

- 

扩展：

x值（表示要将要消亡的值）可能会被移动（移动语义）。某种包含右值引用表达式产生的值

—  An xvalue (an “eXpiring” value) also refers to an object, usually near the end of its lifetime (so that its resources may be moved, for example). An xvalue is the result of certain kinds of expressions involving rvalue references (8.3.2).

[Example: The result of calling a function whose return type is an rvalue reference is an xvalue. — end example ]

A **glvalue** (“generalized” lvalue) is an **lvalue** or an **xvalue**.g value ：可以是lvalue或者是xvalue。



### 移动语义

[移动语义](/Users/matytan/Desktop/code/基础/c++/深入理解C++11/移动语义.md)

右值引用：

左值引用：

什么是移动语义？是浅拷贝吗？

和浅拷贝的区别？

- 浅拷贝的指针赋值：一个对象死了对应的指针指向的空间也没了？
- 为什么？
- 右值而不会？因此需要move，move实际上只是把左值变长了右值

移动语义的优点？



移动语义注意点？



移动构造函数？



为什么移动构造需要右值？



move之后？

### POD



### 多态，虚函数底层实现

多态分为几种

虚表存在哪里的？经过实验放在只读数据区(重定位)

虚表对象共享吗？同一对象共享虚表，什么时候生成？

- 编译时期生成，且确定其中内容

c++内存结构懂了之后，有希望访问private数据吗？

```sh
class A
{
public:
    virtual void foo(){} //虚表
    // virtual ~A(){};
private:
};
// A b; //虚函数指针 实验1-1
// A c;
// A d;
// A f;
// int i = 5;
int main()
{
    //A * pa = new A();
    A b;
    // A c;
    // A d;
    //d.foo();
    //delete pa;
}
动态链接：
无虚函数时
 17 .rodata       00000004  
 无.data.rel.ro 段
有虚函数函数时
 17 .rodata       00000007   经过实验发现，其实.rodata 3字节很可能存的是.data.rel.ro的偏移，真实数据在.data.rel.ro 段中
 
 22 .data.rel.ro  00000028  1个虚函数 多的16字节很可能就是type_info
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
22 .data.rel.ro  00000038     rodata重定位  2个虚函数        
 .data.rel.ro  00000048  4个虚函数
```

经实验之后发现：

```sh
matytan@ubuntu:~/study/C++$ g++ vtabletest.cc -o vtable
matytan@ubuntu:~/study/C++$ objdump -h vtable
```

==实验1== ,对于可执行文件 动态链接

- 1.带虚函数（不管几个），对象声明为全局，也就是main函数之前构造。

  - 对象存储在.data区(注意如果是pod[类型](/Users/matytan/Desktop/code/基础/c++/深入理解C++11/POD.md)，对象没有初始化会被存在.bss)，大小为一个指针大小（虚表指针）

  - .rodata大小为0x00000007(有多个虚函数，和有多个对象固定)

    - 有多个对象虚表大小不变，说明相同对象共享虚表

  - 多的16字节很可能就是`type_info`

  - 进一步发现其实真实的数据存在了`.data.rel.ro`

  - 经过实验发现，其实.rodata 3字节很可能存的是.data.rel.ro的偏移，真实数据在.data.rel.ro 段中

    

- 2.不带虚函数，对象声明为全局，也就是main函数之前构造。

  - `.rodata`大小为0x00000004 （说明确实是有东西存在了.rodata,那为什么只少了3字节？一个虚表是3字节？)

    - 进一步对比`.data.rel.ro`

    

  - 同时我发现一个问题。对于可执行文件.data段和.bss就算没有数据 。也会有默认大小。是动态链接的。

结论：

1. 确实是存在了.rodata不过由于链接装载(编译静态）时重定位 多了一个.data.rel.ro 段（找了好久）
2. 多个对象虚表只有一个



- 进一步静态链接发现

  ```sh
  //无虚函数 静态链接仍然有.data.rel.ro
  .rodata       0001bfcc 
  .data.rel.ro  00002df4
  
  //1个虚函数
  .rodata       0001c06c
  .data.rel.ro  00002f54
  
  //3个虚函数
  9 .rodata       0001c06c
  17 .data.rel.ro  00002f74
  ```

<img src="https://jacktang816.github.io/img/cpp/virtualFunction/vptrLocation.png" alt="mg" style="zoom:50%;" />

### 访问私有成员

```cpp
#include<iostream>
class A
{
public:
    //virtual void foo(){} //虚表
    // virtual void foo1(){} //虚表
    // virtual void foo2(){} //虚表
    // virtual ~A(){};
private:
    int i =3;
    char a = 'a';
    void printf(){
        std::cout<<"I am A"<<std::endl;
    }
};

class B
{
public:
    void printf(){
        std::cout<<"I am B"<<std::endl;
    }
    int i = 2;
    int 
};

int main()
{
    //想要访问A的变量，此时需要一个指针去获取A的内容，A自己的指针this肯定不能直接用了
    //那试试int？不行 需要成员函数绕过编译器


    // void *p = static_cast<void*>(new A());
    // p->print();
    A a;
    B *b = reinterpret_cast<B*>(&a);
    b->printf();
    std::cout<<b->i<<std::endl;


}
```





### 今天做三数之和 返回了局部对象的引用

今天做三数之和（两数之和双指针） 返回了局部对象的引用。导致出问题。

但是又不想返回对象

想用智能指针

unique_ptr 需要初始化

- 初始化注意不能用等号

```cpp
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        // -4 -1 -1 0 1 2
        //确定一个 剩下的就是2数之和问题

        //nsum问题
        int n = nums.size();
        sort(nums.begin(),nums.end());
        vector<vector<int>>  res;
        
        for(int i=0;i<n;++i)
        {
            unique_ptr<vector<vector<int>>>
                tuples = move(twoSum(nums,i+1,0-nums[i])); //这里注意 
            
            for(auto tuple : *tuples)
            {
                tuple.push_back(nums[i]);
                res.push_back(tuple);
            }

            //跳过第一个数重复的情况 例如[1,1,1,2,3] 6
            while(i<n-1&&nums[i]==nums[i+1]) i++;
        
        }
        return res;

    }

    unique_ptr<vector<vector<int>>> twoSum(vector<int>& nums,int start,int target) //返回局部对象的引用！！！
    {
        int lo = start; //start最多取到
        int hi = nums.size()-1;
        unique_ptr<vector<vector<int>>> res(new vector<vector<int>>());
        //注意要去重
        while(lo<hi)  //[lo,hi)  两个指针不需要相等 相等就是一个数相加了
        {
            int sum = nums[lo] + nums[hi];
            int left = nums[lo],right = nums[hi];//记录当初两个值 方便去重
            if(sum<target)
            {
                while(lo<hi&&nums[lo]==left) lo++;//相同值跳过
            }
            else if(sum>target)
            {
                while(lo<hi&&nums[hi]==right) hi--;//相同值跳过
            }
            else if(sum == target)
            {
                res->push_back({left,right});
                while(lo<hi&&nums[lo]==left) lo++;
                while(lo<hi&&nums[hi]==right) hi--;
            }

        }
        return res;
    }
};
```



## 指针相关

野指针、空悬指针是什么？指的是指针指向的对象已经释放，但是指针变量仍然指向这块地址





shared_ptr

weak_ptr：解决循环引用，强引用问题





## C++11

匿名函数：即lambda函数

 [C++11中的匿名函数(lambda函数,lambda表达式)](https://www.cnblogs.com/pzhfei/archive/2013/01/14/lambda_expression.html)
