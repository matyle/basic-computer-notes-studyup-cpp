# Runtime Semantics

```cpp
if(yy == xx.getvalue())....
```

## 对象的构造和析构
- 对象尽可能放置在使用他的区域中，而不是把所有对象全部放置在某个区段的起始处

### 全局对象

- 全局对象需要静态初始化操作和内存释放操作
  - 所有的全局对象都放置在数据段中。
  ```cpp
  int v1 = 1024;
  int v2;
  ```
  - v1,v2都放置在数据段中，v2初值为0.
  - 全局类对象编译期间被放置到data segment中并且内容为0。但是构造函数需要到程序激活时才会执行
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-05-16-39-17.png
  width=490px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>

### 局部静态对象
```cpp
const Matrix& identity(){
    static Matrix mat_identity;
    return mat_identity;
}
```

- 静态对象存储在全局变量存储的数据段
- 如何保证 只有在identity被调用时才构建对象，同时只构建一次？

### 对象数组


### 默认构造函数与数组

- 如何使带参数的构造函数的类对象 创建数组？因为内部的实现的时候需要传递构造函数的指针

## new和delete运算符
- new运算符
    ```cpp
    int *pi = new int(5);
    ```
    1. 通过适当的new运算符函数实体，分配所需的内存
    `int *pi = _new(sizeof(int))`

    2. 给配置得到的对象设立初值
    *pi = 5;
    (如果是类对象，需要调用构造函数)

- delete运算符
  - if pi是0 则delete不能有操作
  ```cpp
  if(pi!=0){
      _delete(pi);
  }
  ```
  - pi并不会因此被自动清除为0 ！！
  - pi所指向的对象会因delete而结束，然而pi指针并不会结束，仍然可以使用
  - pi指向的对象不再合法，但地址本身却仍然代表一个合法空间


- 使用构造函数来分配一个类对象 和delete一个对象
  ```cpp
  Point3d *orgin = new Point3d;
  
  //被转化为
  Point3d *orgin;
  if(orgin=_new(sizeof(Point3d))){
      orgin = Point3d::Point3d(orgin);
  }
  //异常版本
  Point3d *orgin;
  if(orgin=_new(sizeof(Point3d))){
      try{
      orgin = Point3d::Point3d(orgin);
      }
      catch(...){
          _delete(origin);//发生异常 释放new产生但空间
          throw；//抛出一个异常
      }
  }
  
  //delete操作，应用析构函数
  if(orgin!=0){
      Point3d::~Point3d(orgin);
      _delete(origin);
  }


  ```

- new和delete的实现
  ```cpp
  extern void*
  operator new(size_t size){
      if(size==0)
        size = 1; //new调用必须传回一个独一无二的指针
      void *last_alloc
      while(!(last_alloc=malloc(size))){
            if(_new_handler)
                (*_new_handler)();

            else
                return 0;

      }
      return last_alloc;
  }

  //delete实现
  extern void operator delete(void *ptr){
      if(ptr)
        free((char*)ptr);
  }
  ```

- 针对数组的new语义
  ```CPP
  int *p_array = new int[5];
  //默认构造函数施行于类对象所组成数组的每一个元素上，所以 vec_new不会调用
  //new运算符会调用申请5个int大小的空间
  //因此 这时p_array变量仅仅是个地址 根本就还谈不上对象，因此不能用它
  
  simple_aggr *p_aggr = new simple_aggr[5];
  //与上面一个一样 仅仅是开辟了空间而已，
  
  delete [] p_array //删除整个数组的指针
  delete p_array; //仅仅删除了第一个元素会被析构，其他元素仍然存在，虽然相关内存已经被要求收回了


  //如果元素数目被明显指定，是否使用这个值？

  // 1.优先使用程序员指定的值，现在由于大小明确所以不再使用
  ```
  - 如何明确数组大小？如何记录元素数目？
    - 在vec_new函数传回的每一个内存块配置一个额外的字，叫cookie存入元素数目。
    - 维护一个联合数组，放置指针和大小
    - 如果将一个野指针的交给delete_vec()，这时cookie中存储的元素数目也不合法
```cpp
class Point{
public:
    Point();
    virtual ~Point();
};

class Point3d:public Point{
public:
    Point3d();
    virtual ~Point3d();
}

//若配置这样一个数组
Point *ptr = new Point3d[10];

//delete ptr指向的10个Point3d元素
delete [] ptr; //会出错 只有Point::~Point被调用

// 根据交给vec_delete()函数的是被删除指针类型的析构函数来完成。。这里只有Point的析构函数
// 同时这里传递到vec_delete()的大小是Point 类对象的大小
//不止是运行了错误的析构函数，自第一个元素之后，析构函数就施行于错误的内存区域（因为元素大小不对）

  ```
- 避免一个基类指针指向一个派生类的数组
- 用循环类删除，每次强转

## Placement 操作符new的语义

- 有一个预先定义好的一个重载new运算符，称之为placement operator new
  ```cpp
  Point2w *ptw = new (arena) Point2w;
  
  //其中
  void* operator new(size_t,void* p){
      return p;
  }
  //如果仅仅只是返回p为什么要大费周折？
    Point2w *ptw = (Point2w*) arena;//效果一样吗 这只是一半操作
    // new (arena) 还会调用构造函数施于arena所指的地址 
    //因此上面的
    Point2w *ptw = new (arena) Point2w;
    //等价于
    Point2w *ptw = (Point2w*) arena;
    if(ptw!=0){
        ptw->Point2w::Point2w();
    }



  ```


## 临时对象
- 临时对象不能无条件被摧毁，只有在临时对象被产生出来的情况下才去摧毁它

- 未初始化的堆地址不能直接引用赋值！！未定义的堆内存
- 临时对象绑定于临时性对象被一个引用绑定