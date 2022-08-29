# Data语意学

- 空类的大小
  - 未做处理的编译器：X为1 ，Y为8，Z为8，A为12(其中包含1，class Y 8，对齐3)
  - 做了特殊处理的编译器：X为1 ，Y为4，Z为4，A为8.
  - 事实上X并不是空的，编译器会插入一个char，使得这个类的对象能得到不同的地址；
  - Y Z的大小
    - 语言本身所造成的额外负担：指向虚基类子对象表的指针
    - 特殊情况的优化处理
    - 对齐的限制：起始地址能够整除以大小
```cpp
class X{};
class Y:public virtual X{};
class Z:public virtual X{};
class A:public Y,public Z{};

X a,b;//分别获得独一无二的地址
``` 
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-23-17-31-26.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

-

- 一个虚基类只会在派生类中存在一份实体

- 非静态数据成员放置的个别的类对象感兴趣的数据，静态数据成员放置的是整个类感兴趣的数据
  - 非静态数据被放在每一个类对象中，包括继承而来的非静态数据成员
  - 静态成员则被放在全局数据段中，不管有多少个对象 都只有一份实体，哪怕不存在对象，静态数据也已经存在。但是一个模版类的静态数据成员稍有不同



## 数据成员的绑定
- 外部全局变量和类中的某成员变量同名，在类中怎么绑定


## 数据成员的布局
- 较晚出现的非静态成员会存在在较高的地址，不一定连续排列。
- 虚表指针

## 数据成员的存取

- 静态数据成员
- 非静态数据成员

## 继承与数据成员
- 一个派生类所表现出来的东西是自己的成员加上基类成员的总和

```cpp
class Point2d{
    public:
        //...
    private:
        float x,y;
}

class Point3d{
    public:
        //...
    private:
        float x,y,z;
}
```

### 只要继承不要多态
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-26-16-54-38.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
  ```cpp
  class Point2d{
    public:
        Point2d(float x=0.0,float y=0.0):_x(x),_y(y){}
        float x(){return x;}
        float y(){return y;}

        void x(float newX){_x = newX;}
        void y(float newY){_y = newY;}

        void operator+=(const Point2d& rhs){
            _x += rhs.x;
            _y += rhs.y;
        }

    protected:
        float _x,_y;

  };
  //继承于point2d的point3d
  class Point3d:public Point2d{
      public:
        Point3d(float x=0.0,float y=0.0,float z=0.0):Point2d(x,y),_z(z){}

        float z(){return z;}
        void z(float newZ){_z = newZ;}

        void operator+=(const Point2d& rhs){
            Point2d::operator+=(rhs);
            _z += rhs.z;
        }

      protected:
        float _z;

  };
  ```
  - 两个容易犯的错误
  - inline函数的设计
  - 把类分两层或者更多层，为了表现体系抽象化，而膨胀了所需的空间
    - 每一个Concrate对象大小为8bit
    - 若分裂为3个类 通过继承，则con3为16b
    ```cpp
    class Concreat{
        public:

        private:
            int val;
            char c1;
            char c2;
            char c3;
    };
    ```

    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-26-17-08-56.png
    width=600px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;"></div>
    </center>
    
    - 为什么子类不利用父类的填充空间？
      - 若把子类对象拷贝给父类
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-26-17-20-36.png
    width=600px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;"></div>
    </center>
    
### 加上多态

```cpp
class Point2d{
public:
    Point2d(float x=0.0,float y=0.0):_x(x),_y(y){}
    float x(){return x;}
    float y(){return y;}

    void x(float newX){_x = newX;}
    void y(float newY){_y = newY;}

    virtual float z(){return 0.0;}
    virtual void  z(float newZ){}


    virtual void operator+=(const Point2d& rhs){
        _x += rhs.x;
        _y += rhs.y;
    }

protected:
    float _x,_y;

};
//继承于point2d的point3d
class Point3d:public Point2d{
    public:
    Point3d(float x=0.0,float y=0.0,float z=0.0):Point2d(x,y),_z(z){}

        float z(){return z;}
        void  z(float newZ){_z = newZ;}

        void operator+=(const Point2d& rhs){
        Point2d::operator+=(rhs);
        _z += rhs.z;
    }

    protected:
    float _z;
};
```   
    - 此时可以使用多态 可以传入point2d或者point3d对象
    ```cpp
    void foo(Point2d &p1,Point2d &p2){
        p1+=p2;
    }
    ```

- 虚表指针放在类的尾端
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-27-16-49-51.png
    width=400px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;"></div>
    </center>
    
- 虚表指针放在类的前端
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-27-16-50-23.png
    width=400px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;"></div>
    </center>
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-05-27-16-51-20.png
    width=400px>
        <br>
        <div style="color:orange; border-bottom: 1px solid #d9d9d9;
        display: inline-block;
        color: #999;
        padding: 2px;">图片描述</div>
    </center>
    
### 多重继承
- 多重继承的内存分布，有多个父类，所以在给指针赋值的时候，并不能够直接赋值，因为它的上面可能还有其他基类
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-27-17-07-15.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-27-17-07-56.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>



### 虚拟继承


<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-27-17-12-29.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- 如何实现iostream共享同一个subobject（也就是ios）
  - 虚基类表
  - 虚基类指针：指针指向基类
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-27-17-20-18.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
  - 虚函数表中直接放置虚基类的offset（因为他们在一个对象里面不需要放地址）
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-27-17-26-53.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  

## 对象成员的效率

- 分别利用数组，结构体，类+inline CC编译器和NCC编译器
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-28-11-15-19.png
width=500px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-28-11-21-15.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- 然后比较了直接存取，虚拟继承，虚拟继承三层

## 指向数据成员的指针
- `& Point3d::z;`:得到z在类对象中的偏移量 即x+y的大小和
```cpp
class Point3d{
    public:
        virtual ~Point3d();
    private:
        static Point3d orgin;
        float x,y,z; //内存中
}
```
- vptr是放在类的起始处或者尾端
- 那么数据成员xyz的指针偏移应该要么是0 4 8 要么是4 8 12，为什么实际是1 5 9 或者5 9 13
- 问题在于，如何区分没有指向任何data number的指针与一个指向第一个数据成员的指针。
- //内存中如果虚表指针在尾端，那么x指向的应该是0，则
- //取一个非静态数据成员的地址，会得到他在类中的offset
- //而取一个 绑定在类对象的数据成员的地址，会得到内存中真正的地址！
```cpp
float Point3d::*p1 = 0;//指针指向Point3d中地址0
float Point3d::*p2 = &Point3d::x;
//如何区分？因此每一个真正的成员数据的offset都被加1，真正使用的时候需要减1
if(p1==p2){
    cout<<;
}

//下一个
& Point3d::z
& origin.z
//取一个非静态数据成员的地址，会得到他在类中的offset
//而取一个 绑定在类对象的数据成员的地址，会得到内存中真正的地址！
```


- 指向成员的指针效率问题:效率更低，多了一层间接性
```cpp
//1.已绑定成员的地址
float *ax = &pA.x;

*bx = *ax - *bz

//2.指向数据成员的指针
float Point3d::*ax = &Point3d::x;

pB.*bx = pA.*ax - pB.bz;
```
