# 第7章 类
类的基本思想是**数据抽象** 和 **封装**
数据抽象：依赖于**接口（interface）**和**实现（implementation）**分离
类的接口表示包括用户能够执行的操作。类的实现包括类的成员，负责接口实现的函数体，定义所需的各种私有函数，封装后的类隐藏了实现细节。用户只能使用接口，而无法访问实现
首先要定义**抽象数据类型**，
类的用户是程序员
## 定义抽象数据类型

### 设计Sales_data类
```cpp
class Sales_data{
private:
    std::string bookNo;
    unsigned int units_sold;
    double revenue;
    string isbn();//返回isbn号
    Sales_data& combine(const Sales_data& rhs); //将s1加到s2

};
//执行加法和IO的函数不作为成员函数 
Sales_data add(const Sales_data& s1, const Sales_data& s2); //将s1加到s2
std::istream read(std::istream,Sales_data& s);
std::istream print(std::ostream,const Sales_data& s);
```

- 定义在类内部的函数时隐式inline

- 定义成员函数：执行加法和IO的函数不作为成员函数（14章会说原因）
    - 引入this，this代表本对象，作为隐式参数访问调用它的对象
  ```cpp
  total.isbn();
  //等价于
  isbn(&total);
  ```
  - 引入const成员函数:const作用是修改this指针的类型。
  - 默认情况下this的类型是**指向类类型非常量版本的常量指针**（`Sales_data *const this`)。因此默认情况下不能将this绑定到一个常量对象（加对象前面加const）。因此我们不能在一个常量对象上调用普通成员函数。
  - const成员函数的this指针为`const Sales_data *const this`(显式定义this 错误)
  - this是指向常量的常量指针，因此常量成员函数不能改变调用它对象的内容。isbn可以读取对象的数据成员，但是不能修改
  ```cpp
  void isbn() const;
  ```

### 定义类相关的非成员函数

- 常常需要定义一些辅助函数，如add,read,print等，属于类的组成部分，但是不属于类本身
- read和print函数参数是输入流和输出流的引用 istream和ostream，返回他们的引用，IO类属于不能拷贝的类型！！（noncopyable），只能用引用传递。且读取和写入操作都会改变流的内容，因此不能用const
- print不负责换行，把格式操作尽量留给用户
```cpp
//item从is中读入
std::istream &read(std::istream &is,Sales_data &item){
    double price = 0;
    is >> item.bookNo >> item.units_sold >> price;
    item.revenue = item.units_sold * price;
    return is;

}

std::ostream &print(std::ostream &os,const Sales_data &item){
    os << item.isbn() << " " << item.units_sold << " " <<item.revenue << " ";
    return os;

}
```

- 定义add函数
```cpp
Sales_data add(const Sales_data& lhs, const Sales_data& rhs){
    Sales_data sum = lhs; //左值赋给sum
    sum.combine(rhs);
    return sum;
}
```


### 构造函数
- 构造函数没有返回类型
- 构造函数不能被声明为const


### 拷贝，赋值，析构

- 拷贝运算符 operator=

### class 和 struct唯一区别是默认访问权限不同
- struct默认为public

### 友元
- 类中声明友元函数可以访问类的私有成员 为非成员函数声明为友元
- 例如
  ```cpp
  class Sales_data{
    friend Sales_data add(const Sales_data& lhs, const Sales_data& rhs); //将s1加到s2
  };
    Sales_data add(const Sales_data& lhs, const Sales_data& rhs); //将s1加到s2
  ```

## 类的其他特性

### 类成员再探

- 定义一个类型成员
```cpp
class Screen{
public:
    typedef std::string::size_type pos;
    //等价声明
    using pos = std::string::size_type;
private:
    pos cursor = 0; //隐藏实现细节 给定行号和列号 找到相应内容
    pos height = 0, width = 0;//高和宽,
    std::string contents; //存储内容，根据行号和列号找到相应内容
};
```
- 类内定义的成员函数是隐式内联（自动内联）的，类外定义的默认不是内联（需要inline关键字显式的内联）

- 重载成员函数
- **可变数据成员**
  - mutable成员，永远不会是const，即使是const对象的成员。因此一个const成员函数可以改变一个mutable成员的值
  ```cpp
    class Screen{
    public:
        void some_member() const{
            cnt++;//可以改变mutable成员
            a++;//错误，不能改变普通数据成员
        }
    private:
        mutable int cnt;
        int a;
    };
  ```

### 返回*this的成员函数

- 普通成员返回*this
```cpp
class Screen{
public:
    Screen &set(char);
    Screen &set(pos,pos,char);
    Screen &move(pos,pos);

private:
    mutable int cnt;
    int a;
};
inlien Screen &Screen::move(pos r,pos c){
    pos row = r * width; //计算行的位置
    cursor = row + c;
    return *this;

}
inline Screen &Screen::set(char ch){
    contents[cursor] = ch;
    return *this;//将this对象作为左值返回
}
```
- const成员函数返回*this，返回类型是一个常量引用
  - 返回的对象不能调用非常量成员函数s


- **基于const的重载**
  - 跟之前指针参数是否指向const是一致而重载函数。
  - 通过区分成员函数是否是const的可以对其重载。因为非常量的版本对于常量对象是不可用的，所以我们只能在一个常**量对象上调用const成员函数**，虽然可以在非常量对象上调用const成员函数或者非const成员函数，但是const成员函数是一个更好的匹配


### 类类型
- 每个类定义了唯一的类型，哪怕成员全部一样，也是不同的类型
- 类的声明：可以像函数声明和定义分离开来，被称之为前向声明
- 在声明之后，定义之前是一个**不完全类型**。使用有限，只能定义这种**类型的指针或者引用**，可以声明（不能定义）返回类型或者作为参数的函数
- 因为一个类的名字出现过，就被认为是声明过了，因此类运行包含指向它自身类型的引用或者指针


### 友元再探

- 友元能定义在类的内部，这样的函数是隐式内联的。
- 类之间的友元关系
  - 可以在a类中指定某一个b类是友元，然后b类的成员函数就可以访问a类的私有成员，即b是a的友元类
  - 友元关系不存在传递性

- 令成员函数为友元
  - 可以只让b类的某一个成员函数作为a类的友元例如
  `friend void Window_mgr::clear(ScreenIndex)`

- 函数重载与友元
  - 重载虽然名字一样，但是任然是不同的函数，因此需要分别声明友元

- 友元声明和作用域
  - 类和非成员函数的声明不是必须在他们友元声明之前。
  - 就算在类的内部定义了友元函数，我们必须在内外提供相应的声明
  - 友元声明的作用
  ```cpp
  class X{
    friend void f(){//}
  };
  void X::g(){return f();} //错误，f()还没有被声明
  void f();
  void X::g(){return f();} //正确
  ```

## 类的作用域

- 在类的作用域之外，普通数据和函数成员只能由对象，引用或者指针使用成员访问运算符来访问。

- 作用域和定义在类外部的成员
  - 一个类就是一个作用域，因此在类的外部成员函数实际上对于类是不可见的，因此要用::
  - 同时，函数的返回类型通常出现在函数名之前。void B::foo();


### 名字查找与类的作用域

- 名字查找：首先在名字所在的块找声明语句，只考虑在名字出现之前的语句
  - 没找到，继续查找外层作用域
  - 最终没找到，报错

- 类的定义
  - 编译成员的声明
  -  直到类全部可见后才编译函数体（先处理类中的全部声明，然后处理成员函数的定义）
  - **因此成员函数能使用类中定义的任何名字**
- 类成员声明和名字查找
  ```cpp
  typedef double Money;
  string bal
  class Accout{
  public:
    Money balance(){ //1.Money内层没有找到定义，然后查找外层
      return bal; //类中声明全部处理完，然后处理函数体，因此bal是类中的，Money类型
    }
  private:
    Money bal;
  }
  ```
- 类名特殊处理：成员使用了外层作用域的某个名字，该名子代表一种的类型 不允许重新定义
  ```cpp
  typedef double Money;
  string bal
  class Accout{
  public:
    Money balance(){ //1.Money内层没有找到定义，然后查找外层 使用外层定义。这是声明，不是函数体
      return bal; //类中声明全部处理完，然后处理函数体，因此bal是类中的，Money类型
    }
  private:
    typedef double Money;//错误，已经使用了外层声明了，不能再重新定义
    Money bal;
  }
  ```

- 不要把成员名字作为参数或者其他局部变量使用！
  - 首先在成员函数内找名字声明
  - 没找到，然后在类内找
  - 没找到，然后在类外找，成员函数定义之前的语句
    ```cpp
  string bal //全局
  typedef double Money;
  class Accout{
  public:
    Money balance(string bal){ // 
      return bal; //哪个bal？ 使用的应该是参数声明
    }
  private:
    string bal;
  }
  ```
- 类作用域之后，在外围作用域中查找
  ```cpp
  string bal //全局
  typedef double Money;
  class Accout{
  public:
    Money balance(string bal){ // 
      return bal; //哪个bal？
    }
  private:
    string bal;
  }

  //
  Money balance(string ba_){ // 
      return bal; //成员bal
    }

  Money balance(string ba_){ // 
      return ::bal; //全局bar
    }
  ```

## 7.5 构造函数再探

### 初始值列表
- 如果成员是const或者引用 必须初始化。因为引用和const是定义即初始化，不能后面赋值

- 成员初始化顺序：严格按声明顺序。因此列表初始化也是按声明顺序



### 委托构造函数
```cpp
class Sales_data{
  Sales_data(std::string s,unsigned cnt,doule price):bookNo(s),unit_sold(cnt),revenue(cnt*price);
  Sales_daata()::Sales_data("",0,0){} //委托构造函数
};
```

### 默认构造函数的作用

### 隐式的类类型转换
- 如果构造函数只接收一个实参，则它实际定义了一个转换此类型的隐式转换机制，有时称这种构造函数称作**转换构造函数**

- 抑制构造函数定义的隐式转换
  - `explicit` 
  - 关键字只能出现在类内的构造函数声明处
  - `explicit` 构造函数只能用于直接初始化
  - 为转换显式地使用构造函数
  - 标准库中使用`explicit` 关键字的类
    - 接收一个const *char的string的构造函数
    - 接收一个容量参数的vector构造参数

### 聚合类
- 聚合类使用户可以直接访问其成员，并且有特殊初始化语法形式，如下形式满足聚合类
  - 所有成员都是public的
  - 没有定义任何构造函数
  - 没有类内初始值
  - 没有基类，也没有virtual函数
  ```cpp
  struct Data{
    int ival;
    string s;
  }
   Data vall = {0,"anna"};//初始值按声明顺序
  ```


### 字面值常量类

- constexpr函数的参数和返回值都必须是字面值类型。字面值类可能含有constexpr成员
  - 数据成员必须都是字面值类型
  - 类至少含有一个constexpr构造函数
  - 如果一个数据成员含有类内初始值，则内置类型成员的初始值必须是常量表达式。如果是类类型，则初始值必须使用成员自己的constexpr构造函数
  - 必须使用析构函数的默认定义


## 类的静态成员
- 有时候类需要它的一些成员直接和类本身相关，而不是与各个对象保持关联
- 没必要每个对象都存一个这个共同的成员
- 声明静态成员
  - 通过在成员声明之前加上关键字static使得其与类关联在一起
  - 静态成员函数不与任何类绑定在一起，没有this指针
  - **静态成员函数不能声明为const**，而且也不能在static函数体重使用this指针（因此static里面不能定义成员）？

- 定义静态成员
  - 在类外定义静态成员函数时，不能重复static关键字
  - 必须在类外定义并初始化一个静态成员。但是可以为字面值类型constexpr类型的静态成员初始化赋值
  ```cpp
  double Account::interestRate = initRate();
  ```

  ```cpp
  class StaticTest{
  public:
      static int var_st; //声明静态成员
      static int foo();
      int pvar;
  private:
      int var;

  };
  int StaticTest::var_st = 1;
  int StaticTest::foo(){
      int cnt = pvar; //错误 没有this指针不能使用类成员
      int cnt2 = var_st; //正确

  }

  int main(){

      StaticTest::foo();//直接调用 正确
      StaticTest st;
      st.foo();// 对象调用 正确
  }  ```