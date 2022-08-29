## 通过一个例子看虚拟内存管理



- 其中包括

```cpp

/*
matytan@ubuntu:~/study/C++$ g++ -c simplesection.cc
matytan@ubuntu:~/study/C++$ objdump -h simplesection.o

matytan@ubuntu:~/study/C++$ readelf -h simplesection.o

g++ simplesection.cc -o test //链接之后的可执行文件段
objdump -h test
*/
//#include<iostream>

//查看段

//堆栈怎么看？
int global_init = 2; //.data
char gc = 'a'; //.data
int global_uninit; //.bss
const int ci=1; //rodata
int &ri = global_init;  //.data.rel.ro.local 00000008 一个指针大小
//.eh_frame  
class Simple
{
public:
    // //Simple(){} //代码段 不占对象内存
    // int foo();
    // static int sfoo();
    //virtual int vfoo(){return 0;}  //虚表指针占8字节

private:
    int i; //4
    char c; //1
    char *cp; //8 
    //13对齐到16
    static int si; //未初始化的时候不占内存 不在.bss 类似于全局未初始化的数据
    
};
int Simple::si = 1; // static成员变量只能在外面初始化 初始化之后在.data中 

int main()
{
    Simple s; //5 对象在栈中，其对象中的内容也在栈中 段里面看不到
  
  	
    //std::cout<<sizeof(s);
    /*
    8 .eh_frame     00000058  0000000000000000  0000000000000000  00000100  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
    */
   
}
```





