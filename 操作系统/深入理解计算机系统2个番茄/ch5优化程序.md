# 优化程序性能
1. 选择一组最好的算法和数据结构
2. 必须编写出编译器能够有效优化以转换成高效的可执行代码的源代码
研究汇编代码是理解编译器以及产生的代码会如何运行的最有效的手段之一。


## 优化编译器的能力和局限性
- 优化过后不改变程序的正确性
  - 存储器别名（memory aliasing）
  - 函数调用阻碍优化
```c
void twiddlel(int *xp,int *yp){
    *xp += *yp; //3次存储器引用 取x，取y，写入xp对应的存储器
    *xp += *yp;//3次
}
void twiddle2(int *xp,int *yp){ 
    *xp += 2* *yp;//左移动1位 效率更高 三次存储器引用
}

//功能应该是一样的 且第二个更加高效
//分析：如果xp=yp？ 指向的是同一个地址 
第一个函数：增加了4倍。*xp += *yp; == *xp += *xp; //正确
第二个出错：因为右边计算了已经改变了*xp的值！！///增加了3倍
    *xp += 2* *xp;

void f(int);
int func1(x){
    return f(x)+f(x)+f(x)+f(x);
}

int func2(x){
    return 4*f(x); //只调用一次
}

//fun2有个副作用就是改变了全局程序状态的一部分

如果
int cnt = 0;
int f(int x){
    return cnt++;
}
//此时fun2只会调用一次 也就是4*0；
//fun1则为0+1+2+3 = 6；
```

## 表示程序性能
- 每元素的周期数(cycles per elment cpe)：线性函数的系数
  - 赫兹：兆赫兹（百万周每秒）
  - 纳秒 每个周期的时间是时钟频率的倒数
- 代码移动优化
  - 比如求字符串长度这样的函数不应该写在for循环中，应该移动到外面
  ```c
  for(int i=0;i<s.size();++i)
  改为
  int n = s.size();
  for(int i=0;i<n;++i)
  ```
- 减少过程调用
- 消除不必要的存储器引用
  - 用一个变量先作为计算的值 最后写入指针引用中
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-20-09-58-20.png
  width=600px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">图片描述</div>
  </center>
  
## 理解现代处理器
- ICU：指令控制单元
- EU：指令执行单元
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-22-09-55-25.png
width=400px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;"></div>
</center>

- 降低循环开销
  - 循环展开法，例如三次循环展开，i一次加3，一次循环里面处理三个元素 上限为n-k+1
- 转换为指针代码：数组转指针
- 提高并行性 
