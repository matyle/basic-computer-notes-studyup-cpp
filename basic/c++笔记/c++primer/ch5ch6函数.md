# 语句
第二遍回顾重点

### throw表达式
- 使用throw表达式引发一个异常。

```cpp
if(a!=b)
    throw runtime_error("a != b");
cout<<a+b<<endl;
```
- 抛出异常会终止当前函数，把控制权转移给能力该异常的代码

### try-catch语句块 

```cpp
try{
    ...
}catch(异常声明){
    ...
}catch(exception-declaration){
    ...
}
finally{
}
```
- 编写处理代码


# ch6 函数

### 局部静态对象
- static 只初始化一次，然后在函数调用期间一直存在存在，程序结束才被销毁


### 传引用参数
- 引用实际上是一个对象别名，引用在定义的时候必须绑定一个对象初始化，且不能修改
- 使用引用避免拷贝
- 使用引用形参返回额外信息（RVE优化）

### const实参和形参
- 为了避免修改引用参数的值 我们可以加上const 

- 顶层const作用于对象本身 int * const p;表示p是一个const 指向一个int变量，而且p的值不能改变只能指向那个int，但是int的值可以变
```c
const int ci = 42;//ci不能改变，const是顶层的
int i = ci;//拷贝ci时，忽略了顶层const
int * const p  = &i;//p是顶层const 只能指向i
*p = 0;//正确，p指向的对象可以改变
```
-  和其他初始化一样，实参的初始化会忽略顶层const。当形参有顶层const时，传给他常量对象或者非常量对象都是可以的
   - `void fcn(const int i)`//fcn可以读取i，但是不能向i写值
   - 若调用`fcn(const int i)`时，可以传const int，也可以传int ，如果传int相当于调用fcn(int i);
   - 因此不能同时定义`fcn(const int i)和fcn(int i)`

- 指针或引用形参和const
  - 底层const `const int *p`:可以使用一个非常量初始化一个底层对象，反过来不行
  - 一个普通的引用必须用同类型的对象初始化
  ```c
  int i = 42;
  const int  *cp = &i;// cp指向的对象是const的，也就是底层const，这时i不能被改变
  const int &r = i;//正确但是r不能改变i r引用了一个const对象，因此改变r不能改变i
  const int &r2 = 42;//用字面值初始化一个引用 正确

  //错误
  int *p = cp;//类型不匹配 (1)
  int &r3 = r; //类型不匹配，const int变量赋值给int (2)
  int &r4 = 42 ;//不能用一个字面值初始化一个非常量引用

  void reset(int *ip){
      *ip = 0;
      ip = 0;
  }

  void reset(int &i){
      i = 0;
  }
  int i = 0;
  const int ci = i;
  string::size_type ctr= 0; //无符号类型
  reset(&i);//可以，调用参数为int*的reset
  reset(&ci)//错误，类型不匹配，只用指向const int的对象初始化int*

  reset(ctr);//错误，crt是无符号类型，不能初始化int*
  ```
- 尽量使用常量引用
  - 我们不能把const对象，字面值或者需要转换的类型的对象传递给普通的引用形参
  - 如果不修改实参的值，我们使用常量引用
  ```cpp
  //不良设计
  string::size_type find_char(string &s,char c,string:size_type &occurs);
  //如果我们这么调用
  find_char("hello word",'h',ctr);//报错，字面值常量
  
  //还有一个问题，如果其他函数调用这个函数，使用了常量引用，那么也会报错
  bool is_sentence(const string& s){
    string::size_type ctr= 0; //无符号类型
    //s的末尾有且只有一个句号
    return find_char(s,'.',ctr)==s.size()-1&&ctr=1;//报错，不能用一个const对象初始化非const对象
      
  }
  //因此最好这么设计
  string::size_type find_char(cosnt string &s,char c,string:size_type &occurs);
  ```


### 数组形参
- 数组不允许拷贝，因此不能直接传参
```c
//形参
void print(const int*);//传数组首地址
void print(const int []);
void print(const int [10])//不一定是10 期望10


int i=0,j[2]={0,1};
print(&i); //正确，&i为 int* 类型
print(j);// j转换成int* 并指向j[0];
```
- 由于传递数组是以指针形式传递给函数，因此函数一开始不知道数组大小
  - 使用标记指定数组长度：c风格的字符串，数组本身包含一个结束标记
  - 使用标准库规范：传递首位指针，类似于迭代器 指针也是迭代器p1067
  ```cpp
  void print(int *beg,int *end){
    while(beg!=end){
      cout<<beg++<<endl;
    }
  }

  //begin()和end()获取首尾指针
  int j[2] = {0,1};
  print(begin(j),end(j));
  ```

- 显示传递一个数组大小的形式参数


- 数组形式参数和const
  - 当函数不需要对数组元素执行写操作时，数组形成应该是指向const的指针 const int*

- 数组引用形参
  ```cpp
  void print(int(&arr)[10]){
    for(auto elem :arr){
      cout<<elem<<endl;
    }
  }
  ```
- 传递多维数组
  ```cpp
  void print(int (*arr)[10],int rowsize);
  void print(int arr[][10]);
  ```


### main：处理命令行选项

### 含有可变形参的函数

- initializer_list形参
  ```cpp
  initializer_list<T> lst
  lst.size()
  lst.begin()
  lst.end()
  ```

- 省略符形参
  ```cpp
  void foo(parm_list,...);
  void foo(...);
  ```

### 返回语句
- 返回一个值的方式和初始化一个变量或形参方法完全一样：返回值用于初始化调用点的一个临时量。
- 不要返回局部对象的指针或者引用。函数完成之后局部对象的内存空间也会随之被释放。因此返回一个它的地址只会指向一个不再有效的内存区域
-  调用运算符优先级与点，箭头运算符优先级相同，且符合左结合律

- 引用返回左值，其他返回类型返回右值。
- 函数返回值在左边，可以赋值给返回值
  ```cpp
  char &get_val(string &str,string::szie_type ix){
    return str[ix];//左值
  }
  int main(){
    string s("a value");
    cout<<s<<endl;
    get_val(s,0) = 'A';//函数调用写在了左边

  }
  ```

- 列表初始化返回值 返回{1,2,3} vector<int>类型

### 返回数组指针
- 不能直接返回数组。可以返回数组的指针或者引用
  ```cpp
  typedef int arrT[10];
  //或者
  using arrT = int[10];
  arrT* func(int i);
  ```
- 不使用别名： Type (*function(parameter_lsit))[dimension]
  ```cpp
  int (*func(int i))[10];
  ```

- 使用尾置返回类型
  ```cpp
  auto fun(int i) -> int(*)[10];
  ```

- 使用decltype 自动获取返回类型
  ```cpp
  int odd[] = {1,3,5,7,9}
  //返回一个指针，该指针指向含有5个整数的数组
  decltype(odd) *arrptr(int i){
    //
  }
  ```

## 函数重载
- overload重载函数:同一个作用域，几个函数名字相同，但是形式参数列表不同
- 重载和const形式参数
  - 顶层const和没有一个顶层const的形式参数区分 声明等价
  - 形参i是否为const与传入的实参是否为const是完全无关的。这里的const仅表示i在函数体中不允许修改。
  - **因为值传递的const形参在调用上与非const形参没有区别，所以仅仅使用const无法区分参数类别，所以无法实现函数重载，如下的重载是错误的：**
  ```cpp
  Record lookup(Phone);
  Record lookup(const Phone); //顶层const 重复定义

  Record lookup(Phone);
  Record lookup(Phone* const); //顶层指针 重复定义 指针本身是const

  ```
  - 底层const可以实现函数重载
  ```cpp
  Record lookup(Account&);
  Record lookup(const Account&); //新函数

  Record lookup(Accout*);
  Record lookup(const Account*); //新函数
  ```

- const_cast与重载
  - const_cast在重载函数中最有用
  - 如果对两个非常量的string实参调用这个函数，但返回的的结果仍然是const string的引用，改变了本身非const的性质
  ```cpp
  const string  &shorterString(const string &s1,const string &s2){
    return s1.size()<=s2.size()?s1:s2;//如果s1，s2非常量，但返回的的结果会是是const string的引用
  }
  //利用const_cast
  string &shorterString(string &s1,string &s2){
    auto &r = shorterString(const_cast<const string&>(s1),
                            const_cast<const string&>(s2));//调用const版本
    return const_cast<string &>(r);//返回转换为非const
  }
  ```

### 重载与作用域
- 作用域内声明新的函数，会隐藏掉外层作用域声明的函数。在不同作用域无法重载函数名
  ```cpp
  void print(const string &);

  void foo(){
    void print(int);//在局部作用域中声明函数不是好习惯
    print("hell0");//错误，外部print(const string &)被隐藏了
  }
  ```

- C++查找是从当前作用域开始查找，如果在当前作用域找到了所需要的名字，就会忽略外层函数声明。同时名字查找发生在类型匹配之前


## 特殊用途语言特性
默认实参，内联函数，constexpr函数

### 默认实参

- 多次调用中赋予相同的值，反复出现的值称为默认实参
  ```cpp
  string screen(int ht=24,int wid=80);
  ```
  - 为每一个形参数提供了一个默认实参。可以为一个或者多个形式参数赋默认值，**但是注意一旦给某个形参，他后面的形式参数都必须有默认实参数**
  - 函数调用时 按其位置解析。默认实参负责填补函数调用缺少的尾部实参。例如想覆盖wid的默认值，必须要为ht提供实参
  ```cpp
  screen(,90);//错误，只能省略尾部实参
  screen(90);//调用screen(90,80) //因为wid已经有默认值了
  ```
  - 因此 我们设计顺序时，尽量让经常使用默认实参的形参放在后面

- 默认实参声明
  - 多次声明同一个函数合法，但是给定作用域中默认实参只能赋值一次默认实参。后续声明只能给之前没有默认值的形参添加默认实参
  ```cpp
  string screen(int ht,int wid=80);

  string screen(int ht,int wid=8); //错误 重复声明

  string screen(int ht=24,int wid); //正确 添加默认实参
  ```

### 内联函数和constexpr
- 内联函数（适用于规模小，流程直接，频繁调用的函数）
```cpp
const string  &shorterString(const string &s1,const string &s2){
  return s1.size()<=s2.size()?s1:s2;//如果s1，s2非常量，但返回的的结果会是是const string的引用
}
//利用const_cast
string &shorterString(string &s1,string &s2){
  auto &r = shorterString(const_cast<const string&>(s1),
                          const_cast<const string&>(s2));//调用const版本
  return const_cast<string &>(r);//返回转换为非const
}
```

- **shorterstring小函数的优点**：函数名阅读性更好，调用行为统一，如果修改过程只需要修改函数不需要语句中所有表达式修改；重复利用
- **缺点是**：调用要慢，因为函数调用要保存寄存器，返回时恢复（栈帧），可能还要拷贝实参。转向一个新位置继续执行

- 这时可以使用**inline函数**（内联函数）避免开销。在它每一个调用点上“内联地”展开
  ```cpp
  inline const string  &shorterString(const string &s1,const string &s2){
  return s1.size()<=s2.size()?s1:s2;//如果s1，s2非常量，但返回的的结果会是是const string的引用
  }

  //调用
  cout<<shorterstring(s1,s2);
  //编译器会将内联为表达式
  cout<<(s1.size()<=s2.size()?s1:s2);
  ```

- constexpr函数是能用于常量表达式的函数，定义方法如下
  - 与其他函数类似，但是返回值和形参类型都必须是字面值类型
  - 有且只能有一个return语句
  - const不一定返回常量表达式
  ```cpp
  constexpr int new_sz(){return 42;}
  constexpr int foo = new_sz();// 正确foo是一个常量表达式
  
  constexpr size_t scale(size_t cnt){return new_sz()*cnt;}

  int arr[scale(2)];//正确，scale(2)也是常量表达式
  int i = 2;
  int a2[scale(i)];//错误，scale(i)不是常量表达式
  ```
- 把内联函数和constexpr函数放到头文件内


## 函数匹配
- 确定候选函数和可行函数
```cpp
void f();
void f(int);
void f(int, int);
void f(double,double=3.14);

f(5.6)//调用f(double,double)//相当于f(5.6 ,3,14)
```
- 寻找最佳匹配
  - 实参类型和形参类型越接近越好

- 含有多个形参的函数匹配
  ```cpp
  f(42,2.56)
  //那么调用哪个更好呢？
  //如果看第一个参数int，调用f(int,int)更好
  //第二个参数2.56 f(double,double)更好
  //因此报错 二义性
  ```


### 实参类型转换

- 精确匹配
  - 实参类型与形参类型相同
  - 实参从数组类型或函数类型转换为相应指针类型
  - 向实参中添加顶层const（指针本身是const），或者从实参中删除底层const
- 通过const转换实现的匹配
- 通过类型提升实现的匹配
- 通过算术类型转换或指针转换实现的匹配
- 通过类类型转换实现的匹配(p514 14.9节)

通过类型提升实现的匹配和通过算术类型转换的匹配
- 小类型通常会提升到int或者更大的整数类型
```cpp
void ff(int);
void ff(short);
ff('a');//char提升到int 调用f(int)

void manip(long);
void manip(float);
manip(3.14);//错误，二义性调用
```

函数匹配和const实参
- 如果重载函数区别是在于引用类型是否**引用了const或者指针类型形参是否指向const**。当调用时编译器通过实参是否是常量来决定调用哪个函数

```cpp
Record lookup(Account &);
Record lookup(const Account&);

```

## 函数指针

```cpp
bool lengthCompare(const string&,const string &);
//函数指针写法
bool (*pf)(const string&,const string &); //括号是必须的，不然就就是返回一个指针啦


//将lengthCompare地址赋值给pf
pf = lengthCompare;
pf = &lengthCompare; //&取地址符是可选的，函数名本身代表地址

//直接使用指向函数的指针调用该函数
bool b1 = pf("hello","goodbye");
bool b1 = (*pf)("hello","goodbye");  //与上面等价

//可以给pf赋值为0 指向nullptr
pf = 0;

string foo();

pf = foo;//错误，返回值类型不匹配，形参类型不匹配
```


- 重载函数的指针
  - 重载函数时上下文必须清晰界定调用哪个函数
  - 函数指针类型必须和**其中某一个精确匹配**
```cpp
void ff(int *);
void ff(unsigned int);

void (*pf1)(unsigned int) = ff;//pf1指向ff(unsigned int)


void (*pf2)(int) = ff;//错误形参列表不匹配
double (*pf3)(int *) = ff;//返回值类型不匹配

```

- 函数指针形参
  - 虽然不能定义函数类型的形参，但是形参可以是指向函数的指针。
  - 形参看起来是函数类型，但实际上被当做指针使用
```cpp
void useBigger(const string &s1,const string &s2,bool pf(const string&,const string&)); //制动转换为指针

void useBigger(const string &s1,const string &s2,bool (*pf)(const string&,const string&));


void useBigger(s1,s2,lengthCompare);
```

- 利用类型别名typedef和decltype可以简化代码
  ```cpp
  //Func和Fun2是函数类型
  typedef bool Func(const string&, const string&);
  typedef decltype(lengthCompare) Fun2;//等价与上面

  typedef bool(*Funcp)(const string&,const string&);
  typedef decltype(lengthCompare) *Funp2;

  void useBigger(const string&, const string&,Func);//自动转为指针
  void useBigger(const string&, const string&,FuncP2);//自动转为指针

  ```
- 返回指向函数的指针
```cpp
using F = int(int*,int);//F是函数类型， 不是指针
using PF = int(*)(int*,int);//PF是指针类型

PF f1(int);//正确，PF是指向函数的指针，f1返回指向函数的指针
F f1(int);//F是一个函数类型，不能返回一个函数 错误！
F *f1(int);//正确

int (*f1(int))(int*,int);
//由内向外读f1(int) 说明f1是个函数，返回一个指针类型，，这个指针类型也有形参列表(int*,int)，返回值为int，因此是返回一个指向函数的指针，

//等价声明
auto f1(int) -> int(*)(int*,int);//尾置返回值类型
```

- 将auto和decltype用于函数指针
  - 使用decltype作用于函数时，返回的是一个函数类型而非指针类型，显示加上*表明我们需要返回指针
```cpp
string::size_type sumLength(const string&,const string&);
string::size_type largerLength(const string&,const string&);

decltype(sumLength) *
(const string&);
```

```cpp
#include <iostream>
#include <vector>
int add(int a,int b){
    return a - b;
}
int sub(int a,int b){
    return a + b;
}
int muti(int a,int b){
    return a * b;
}
int divide(int a,int b){
    return a / b;
}
int main(){
    typedef decltype(add) *Fp;
    std::vector<Fp> vec;
    vec.push_back(add);
    vec.push_back(sub);
    vec.push_back(muti);
    vec.push_back(divide);
    for(auto c:vec){
        std::cout<<c(5,3)<<std::endl;
    }
    return 0;
}

```