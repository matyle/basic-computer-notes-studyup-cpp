# 链接
- 链接是将不同部分的代码和数据收集和组合成为一个单一文件的过程,这个文件可以被加载到存储器并执行。
  - 链接可以执行与编译时
  - 可以执行于加载时（程序被加载器加载到存储器并执行时）
  - 可以执行于运行时
- 链接器（分离编译）
  - 理解链接器帮助你构造应用程序
  - 理解链接器将帮助你避免一些危险的编程错误
  - 理解链接帮助理解语言作用域规则
  - 理解加载和运行，虚拟内存，分页，存储器映射
  - 开发共享库
- 静态链接，加载动态链接，运行动态链接

## 编译器驱动程序

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-23-11-24-03.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

- `cpp main.c main.i `    c预处理器将main.c翻译成一个main.i文件
- `利用c编译器cci 将main.i翻译成main.s 汇编语言文件`
- 然后运行as 链接器
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-23-11-37-47.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
## 静态链接
- 链接器的两个任务
  - 符号解析
  - 重定位
## 目标文件
  - 可重定位目标文件
  - 可执行目标文件
  - 共享目标文件

## 可重定位目标文件
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-24-10-21-02.png
width=300px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">ELF可重定位目标文件</div>
</center>

## 符号和符号表
- 每个可重定位目标模块m都有一个符号表，包含m所定义和引用的符号信息，有三种不同符号
  - m定义并能被其他模块引用的全局符号。对应于非静态c函数以及不带C的static属性的全局变量
  - 由其他模块定义并被m模块引用的符号，称为外部符号。
  - 只被模块m定义和引用的本地符号。带static的c函数和全局变量。这些符号在m中任何地方都可见，但是不能被其他模块引用
- 任何声明带有static属性的全局变量或者函数都是模块私有的
- 符号表
  ```c
  typedef struct{
      int name;
      int value;
      int size;
      char type:4, //数据 函数 节，源文件名 4bits 4位
      binding:4// 局部还是全局 4bits 4位
      char reserved;
      char section;

  }Elf_Symobol;


    extern int buf[];
    int *bufp0 = &buf[0];
    int *bufp1;
    void swap(){
        int temp;
        bufp1 = &buf[1];
        temp = *bufp0;
        *bufp0 = *bufp1;
        *bufp1 = temp;
    }


    void swap();
    int buf[2] ={1,2};
    int main(){
        swap();
        return 0;
    }
  ```

- 链接器如何解析多处定义的局部符号
  - 编译器输出每个全局符号给汇编器，或者是强或者是弱
  - 强表示已经初始化的全局变量或者函数
  - 弱表示未初始化的全局符号
  - 规则
    - 1. 不允许有多个强符号
    - 2. 如果有一个强符号或者多个弱符号，任意选择其中一个
    - 3. 如果有多个弱符号，任意选择一个

- 与静态库链接
  - file.a的静态库
  - 使用：`gcc -c addvec.c multvec.c`,生成可重定位目标文件，然后使用`ar rcs libvec.a addvec.o mutlvec.c`生成静态库。
  - 同时，可以根据模块中使用 自行调用其中的库，比如main2.o只用到了加法，就不会加载mutvec.o
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-24-11-11-55.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">图片描述</div>
  </center>
  
- 链接器如何使用静态库解析引用
  - 按命令的从左往右来扫描可重定位目标文件和存档文件


##  重定位:合并输入模块，为每个符号分配运行时地址
- 重定位节和符号定义
- 重定位节中的符号引用

- 重定位表目
  ```c
  typedef struct{
      int offset;//引用重定位的偏移
      int symbol:24,//引用应该指向的符号
      type:8;

  }Elf32_Rel;
  ```
- 重定位节中的符号引用
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-25-10-26-06.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
- 重定位pc相关引用
  - pc相关地址就是距pc的偏移
  - 先计算运行时地址refaddress
- 重定位的绝对引用

## 可执行目标文件
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-25-10-34-00.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

## 加载可执行文件
- ./p p不是一个内置命令，会被认为是可执行程序，调用系统中的加载器来运行
- 代码总是从0x08048000开始
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-25-10-43-18.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
  ## 动态链接共享库
  - 共享库也称为共享目标
    - 任何给定的文件系统中，一个库只有一个so文件
    - 在存储器中，一个共享库的.text节只有一个副本可以被不同的正在运行的进程共享
    - `unix > gcc - shared - fPIC - o libvector.so addvec.c multvec.c`
    - 没有任何libvector.so的代码被实际拷贝到p2中，链接器拷贝了重定位和符号表信息，用于解析libvector.so中的代码和数据引用
    - p2中包含.interp节，这个节包含动态链接器的路径名。
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-25-10-54-13.png
  width=400px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
  ## 从应用程序中加载和链接共享库
  - 允许应用程序在运行时加载和链接共享库
  -  `-rdynamic`
  ```c
  #include<dlfcn.h>
  void *dlopen(const char *filename,int flag);//加载共享库filename

  void *dlsym(void* handle,char *symbol);

  int dlclose(void *handle);

  const char *dierror(void);
  ```