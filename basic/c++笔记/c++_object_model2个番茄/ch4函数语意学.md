## Function语义学
- 三种成员函数类型：静态，非静态，虚函数
```cpp
Point3d obj
Point3d *ptr = &obj
//如果这么调用
obj.normalize();
ptr->normalize();
//会发生什么？

Point3d Point3d::normalize() const {
    ..

}
float Point3d::magnitude() const {

}
//这两个函数不确定是不是虚函数，但一定不是static的
```

## 成员的各种调用方式

### 非静态成员函数

```cpp
float magnitude3d(const Point3d* _this){...} //非成员函数
 
float Point3d::magnitude3d() const {...} //成员函数
```

- 成员函数内部经过一系列转化
  - this指针
  - NRV（named return value）优化

- 名称的特殊处理

- 虚拟成员函数
  ```cpp
  //若normalize()是一个虚函数
  //则
  ptr->normalize();
  //内部转化为
  (* ptr->vptr[1])(ptr);
  
  //第二个ptr是this指针
  ```

- 静态成员函数
  - 静态成员函数没有this指针，不能够直接存取其类的非静态数据成员。
  - 全局类中只有一个实例！
  - 不能被声明const volatile， virtual
  - 不需要由类对象才被调用，但是可以由类对象调用
  - 如果将static数据成员设置为nonpubilic时 ，必须要一个成员函数调用，而这个成员函数还是绑定于类对象，因此可以使用static函数

## 虚拟成员函数

- 每一个类有一个虚表，每一个类对象有一个或者多个虚指针

### 多重继承

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-29-21-35-24.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

### 虚拟继承下的虚函数


## 指向成员函数的指针
- 取一个非静态成员的地址，得到的是该成员在该成员的类布局的bytes的位置+1
- 如果非成员函数不是虚函数，那么可以得到真实地址，但是不完全，仍然需要绑定于一个类对象
```cpp
double foo( //返回类型
    Point::*//属于哪个类
    pmf) //成员指针变量名
    ();//参数列表
//初始化指针
double (Point::*coord)() = &Point::x;

coord =&Point::y;

//调用
(orgin.*coord)();
(ptr->*coord)();

//虚拟c++码
(coord)(&orgin);
```
### 支持指向虚成员函数的指针

```cpp
float (Point::*pmf)() = &Point::z;
Point *ptr = new Point3d;
```
- 实际得到的虚表中的索引值
- 如何实现是虚函数的时候得到索引值，非虚函数的时候得到的是内存地址？


## 内联函数

1. 分析函数定义，决定函数本质的inline能力。
2. 真正inline函数扩展操作是在调用的那一点上，会带来参数求值操作以及临时性对象的管理

### 形式参数

- 导致实际参数多次求值操作
- 若实际参数是一个常量表达式，我可以在替换之前完成其求值操作

### 局部变量
- inline函数中加入一个局部变量
