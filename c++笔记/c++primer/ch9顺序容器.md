# 顺序容器

## 顺序容器概述

- 像容器中添加或从容器中删除元素的代价
- 非顺序访问容器中元素的代价
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"src=img/2021-06-28-10-46-02.png
  width=490px>
  </center>
  
 - 大部分情况使用**vector**，除非你有很好的理由用其他容器
   - 小元素，有额外空间限制，不要使用链表
   - 如果在中间插入或者删除 使用list（双向链表），forward_list（单向）


## 容器库概览
- 左值是存储单元内的值，即是有实际存储地址的；
- 右值则不是存储单元内的值，比如它可能是寄存器内的值也可能是立即数。

- 容器操作：
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-29-09-54-36.png
  width=490px>
  </center>
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-29-09-54-52.png
  width=490px>

  </center>
  

### 迭代器
- 元素范围为左闭合区间，即[begin,end)


### 容器类型成员
- size_type,iterator，const_iterator,reference,value_type;
  ```cpp
  list<string>::iterator iter;//定义一个迭代器类型

  vector<int>::
  ```

### begin和end
- begin,end,cbegin,cend
- rbegin,rend,crbegin,crend;//反迭代器

### 赋值和交换

- assign(仅顺序容器)
- swap,交换两个容器很快，并未交换两个元素，而是交换了内部的数据结构



## 顺序容器操作

### 添加元素

- emplace操作：emplace_front,emplace,emplace_back，执行的是构造而不是拷贝
  ```cpp
  c.emplace_back("00000-x1232",10,100);//使用这三个参数初始化构造函数
  ```
- emplace 在容器中直接构造元素，传递给emplace的函数必须与构造函数参数匹配

### 访问元素
- 在vector，string，deque，array中
  - `c.back;`//访问尾元素
  - `c.front;`//访问头元素
  - c[n];
  - c.at(n);

- 访问成员函数返回的是引用，可以改变元素的值

### 删除元素

- 除了deque删除首尾位置之外的任何元素都会使迭代器时效
- vector，string删除点之后的迭代器全部时效
- 删除元素时，需要我们自己检查元素是否存在


### 容器操作导致迭代器时效

- 添加元素

- 编写改变容器的循环程序
- 不要保存end返回的迭代器
  ```cpp
  auto it = v.begin();
  auto end = v.end();//很容易出错
  while(it != end()){
    it++;
    it = v.insert(it,42);
    ++it;//移动it，跳过我们刚刚加的
  }
  //由于end在被v插入时已经改变，所以end会失效
  ```
## vector对象如何增长的？
- 预先分配空间
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-29-10-30-54.png
width=490px>
</center>

- 只有当size大于当前空间容量，reserve调用才会生效

- capacity和size
  - capacity指的是已分配的空间大小
  - size指的是在已分配的空间中已用空间大小

- 只有在size大于capacity之后才会重新分配空间

## 额外的string操作
```cpp
//拷贝构造
string s(cp,n);//s是cp指向的数组的前n个字符的拷贝
string s(s1,pos2);//从s1的pos位开始拷贝
string s(s1,pos2,len);//从s1的pos位开始拷贝len个字符

//substr操作
substr(pos,n);//返回一个从pos开始的n个字符的拷贝

//插入操作
s.insert(pos,args);
s.earse(pos,len);
s.assign(args);//将s的字符替换为args指定的字符


//替换操作
s.replace(pos,len,args);//args可以是str, str,pos,len
s.replace(s.begin(),s.end(),args);//args可以是str 

```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-29-10-45-03.png
width=490px>
</center>
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-29-10-45-21.png
width=490px>
</center>

### string 搜索操作
```cpp
s.find(args);//args 可以是c,pos从pos开始查找c字符 ，返回位置下标
s.rfind(args);//最后一次出现的位置
s.find_first_of(args);//任何一个字符第一次出现的位置
```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-30-09-53-17.png
width=450px>
</center>



### compare 函数


### 数值转换
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-30-10-07-26.png
width=450px>
</center>

- b为转换所用的基数


## 容器适配器
- 适配器是一个通用概念：容器，迭代器，函数都有适配器
- stack：接受一种顺序容器（除了链表）
- queue
- priority_queue

- 定义一个适配器
  ```cpp
  stack<int> stk(deq);//deq是一个deque<int>
 
  ```
### 栈适配器


### 队列适配器

- queue
- priority_queue 优先队列，默认是最小堆。最小的值优先出队