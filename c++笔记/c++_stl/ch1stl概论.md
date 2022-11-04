
## STL六大组件 功能与应用

1. 容器：各种数据结构例如 vector,list,deque,set,map用来存放数据
2. 算法：各种常见算法如sort，search，copy，erase。。。
3. 迭代器：容器与算法之间的胶合剂，即范型指针。共有5种类型，迭代器是一种将operator*，operator->,operator++,operator--等指针相关操作予以重载的类模版
4. 仿函数（functors）：行为类似函数，可作为算法的某种策略。实现的角度来看，仿函数重载了operator()的类或类模版，一般函数指针可以视为狭义的仿函数
5. 接配器：修饰容器或者仿函数或迭代器接口的东西。例如STL中的queue和stack看似容器，其实都是**容器配接器**，因为他们底层完全借助deque。改变fuctor接口的，叫做fuctor adapter，改变容器接口者，称为container adapter；。。。
6. 配置器：负责空间配置与管理。

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-02-22-08-55.png
width=490px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>
