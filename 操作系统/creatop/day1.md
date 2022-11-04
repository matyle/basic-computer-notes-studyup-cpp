<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-10-27-43.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;">操作系统课程图解</div>
</center>


<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-10-28-40.png
width=600px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>

#### 第一课 hello world

- 程序是如何运行的？
  - 预处理 `gcc helloworld.c -E -o hellowrold.i`
  - 编译器`gcc -S -c helloworld.c   `
  - 汇编`gcc helloworld.c -c helloworld.s`
  - 连接`gcc helloworld.o -o helloworld`
- 二进制数据命令`objdump -d helloworld`

- 图灵机与冯诺依曼机构
  - 图灵机：有一条无限长的纸带，纸带上有无限个小格子，小格子中写有相关的信息，纸带上有一个读头，读头能根据纸带小格子里的信息做相关的操作并能来回移动。
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-12-11-05-02.png
  width=400px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">图灵机</div>
  </center>
  
  - 冯诺依曼结构：
    - 把程序和数据装入到计算机中；输入设备；
    - 必须具有长期记住程序、数据的中间结果及最终运算结果； 内存
    - 完成各种算术、逻辑运算和数据传送等数据加工处理； cpu
    - 根据需要控制程序走向，并能根据指令控制机器的各部件协调操作；控制程序执行的控制器；
    - 能够按照要求将处理的数据结果显示给用户。输出
    - 根据冯诺依曼的理论，我们只要把图灵机的几个部件换成电子设备，就可以变成一个最小核心的电子计算机，如下图：
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-12-11-06-59.png
  width=400px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">图灵机转换</div>
  </center>
  
- helloworld原型装入计算机
```c
0000000100003f50 <_main>:
100003f50: 55                           pushq   %rbp
100003f51: 48 89 e5                     movq    %rsp, %rbp
100003f54: 48 83 ec 20                  subq    $32, %rsp
100003f58: c7 45 fc 00 00 00 00         movl    $0, -4(%rbp)
100003f5f: 89 7d f8                     movl    %edi, -8(%rbp)
100003f62: 48 89 75 f0                  movq    %rsi, -16(%rbp)
100003f66: 48 8d 3d 35 00 00 00         leaq    53(%rip), %rdi  # 100003fa2 <dyld_stub_binder+0x100003fa2>
100003f6d: b0 00                        movb    $0, %al
100003f6f: e8 0e 00 00 00               callq   0x100003f82 <dyld_stub_binder+0x100003f82>
100003f74: 31 c9                        xorl    %ecx, %ecx
100003f76: 89 45 ec                     movl    %eax, -20(%rbp)
100003f79: 89 c8                        movl    %ecx, %eax
100003f7b: 48 83 c4 20                  addq    $32, %rsp
100003f7f: 5d                           popq    %rbp
100003f80: c3                           retq
```

  - 分成四列：第一列为地址；
  - 第二列为十六进制，表示真正装入机器中的代码数据；
  - 第三列是对应的汇编代码；
  - 第四列是相关代码的注释。这是 x86_64 体系的代码，由此可以看出 x86 CPU 是变长指令集

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-11-10-12.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>
