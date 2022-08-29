

## 可执行文件的装载与过程

## 进程虚拟地址空间

程序和进程的区别



虚拟地址空间：

- 程序被运行起来之后，将拥有自己独立的**虚拟地址空间**，虚拟地址空间由CPU的位数决定
  - 32位程序虚拟地址空间为0-$2^{32}-1$,即4GB空间大小
  - 64位寻址$2^{64}GB$
- 通过C语言中指针所占空间计算虚拟地址空间大小。
- 32位下指针大小为4字节（4\*8位），8字节（8\*8位）
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-28-09-48-54.png
width=290px>
</center>

- 32位：操作系统部分0xC0000000~0xFFFFFFFF 共1GB，剩下的0x00000000~0xBFFFFFFF 3GB用户空间留给进程，3GB还要预留一部分

进程地址空间不够用怎么办？PAE物理地址扩展

- 32位的CPU使用空间能不能超过4GB
  - 如果空间指的是虚拟地址：那么是不能的。因为 指针（也就是地址）最多8位 范围就是4GB
  - 如果是物理空间则可以，
  - 修改页的映射方式（PAE物理地址扩展），
    - 比如在一个应用程序中某一块地址0x10000000-0x20000000的一段256MB的虚拟地址空间用来做窗口，程序可以从高于4GB的物理空间中申请多个大小为256的物理空间，编号为ABC，用到A时将0x10000000-0x20000000映射到A，用到BC时再映射过去。，在Windows下称为AWE（address Windowing extension），Unix上面使用`mmap()`(将不同的物理内存块映射到进程地址空间)系统调用来实现

## 装载的方式

- 静态装载：一次性把所有指令和数据全部装入内存

- 覆盖装入和页映射是两种典型的动态装载方法。程序用到哪个模块就将哪个模块装入内存

### 覆盖装入

- 程序员手工将程序分为若干块。且要分清各个模块的调用关系
  
- 例如main中有A B两个模块，两个模块不会相互调用。但是
  
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-28-10-07-32.png
  width=290px>
  </center>
  
### 页映射
页映射是虚拟存储机制的一部分。将内存和磁盘中的数据和指令按照“页”为单位划分为若干个页，所有装载和操作的单位都是页。

- 32KB的指令和数据，被分为8页。运行到p0发现p0不在物理页面中，然后产生缺页中断，进而加载p0到内存F0。需要p5时就加载到内存F1。
- 如果F0-F3都满了，这时其他页想进入。物理页面只能淘汰一个页面，怎么淘汰？
  - 先进先出，最先来的最先淘汰（但是可能这个非常常用，马上换出然后又要换进，导致颠簸）
  - LRU 淘汰最近最不常用的页面。可以使用链表，每次最近才使用的放到队尾，防止被换出（跳表+哈希）。换的时候直接取队头就行
    - LRU的详细实现，需要认真想想(包括插入，移除，读取的各个操作)

- 程序划分虚拟页，然后动态的映射到物理内存
  
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-28-10-11-23.png
  width=390px>
  </center>
  
  - 如果此时需要用到p4，装载管理器必须做出抉择，替换掉哪一个页，可以使用FIFO先进先出，也可以使用LRU，最近未使用的页（查询或者使用，或者添加页都需要在尾部）LRU（哈希表+链表）


## 从操作系统看可执行文件的装载
- 如果像上面直接操作物理地址，很明显每次的虚拟页对应的物理地址都会不同，每次页被装入的时候都需要重定位
### 进程的建立


程序的执行：创建一个进程，装载相应的可执行文件并且执行

- 创建一个独立的虚拟地址空间：==分配页目录（一级页表）==（虚拟页和物理内存页映射的**数据结构**，二级页表在磁盘中），不是真正的映射。在缺页的时候才真正映射。
- 读取可执行文件的头，建立虚拟空间和可执行文件的映射关系（其实也就是和磁盘的映射关系）
- CPU指令寄存器设置成可执行文件的入口地址，启动运行（入口函数，不是main函数）

具体实现：

- 首先是**创建虚拟地址空间：** 通过**页映射关系函数**将虚拟空间的页映射到物理页 。这并不是创建空间，而是**创建映射函数所需的数据结构** 。在i386 Linux下只是分配**页目录**，不需设置映射关系。
- 读取可执行文件的头，**建立虚拟空间和可执行文件的映射关系**。虚拟空间与可执行文件的映射。（这一步是开始就映射好了的）
  - 比如发生缺页时，我们需要分配一个物理页，将该“缺页”从磁盘读取到内存中（这一步我们需要知道读哪里），再设置虚拟页和物理页的关系。但是我们也需要知道应许所需要的页应当在可执行文件的哪个位置（因此需要虚拟地址空间和可执行文件的映射关系）。
  - 可执行文件是一个映像文件，因为它被映射到了虚拟空间
  
  虚拟内存区域（VMA）：保存虚拟空间和可执行文件映射关系的数据结构。Windows中称为虚拟段
  
  - 例如本例它在虚拟空间地址为0x08048000-0x08049000 

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-28-10-25-55.png
width=390px>
</center>

- **将CPU指令寄存器设置为入口地址**：EFI文件头中保存的入口地址

### 页错误

- 执行完上面三步，其实可执行文件的代码和数据并没有真正装入物理内存
- 如果操作系统准备执行0x08048000-0x08049000 它会发现是空页面
  - 这时就产生页错误了，操作系统会调用缺页处理程序
  - 系统查找第二步建立的数据结构（可执行文件和虚拟地址空间的映射关系）
    - 找到空页面的VMA（虚拟内存区域）
    - 然后计算出空页面在可执行文件的偏移，
    - 然后在物理内存中分配一个页面
  - 然后**建立物理页面和虚拟地址的映射关系**（在这一步才建立）
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-29-08-36-32.png
width=390px>
</center>


![image-20210730090852658](ch6可执行文件的装载与过程.assets/image-20210730090852658.png)

## 进程虚拟空间分布

### ELF链接视图和执行视图
- 引入了segment（段），之前section（节）也被称为了段
- 引入原因是：我们在装载的时候，如果按section进行分配页，如果说section大小不是页的整数倍，那么剩下的大小也需要分配一个页，当section多的时候浪费就比较严重，如何处理呢？我们又不能把所有section都分配到一个页上吧？
  - 还真行，就是不同section权限不一样，.text可读可执行，.data .bss可读可写，.rodata只读等
  - 我们在装载的时候只需要按权限分页就行，因为并不实际关心数据的存放
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-29-08-45-21.png
  width=490px>
  </center>
  
  - 我们发现init和text权限相同，于是可以使text和init合并，因为有一个4097的section，页大小只有4096！如果不加入segment 剩下1字节需要花一个页面，造成浪费
  - section和segment的区别很明显了，在elf存储我们称为section，如果在加载时进行合并我们称为segment


- 查看segment
  - `readelf -S sectionmapping.elf`，查看Section header
  - `readelf -l sectionmapping.o` 查看程序头
  ```c
  Program Headers:
  Type           Offset   VirtAddr   PhysAddr   FileSiz MemSiz  Flg Align
  LOAD           0x000000 0x08048000 0x08048000 0xa11c1 0xa11c1 R E 0x1000 4096字节对齐
  LOAD           0x0a1f5c 0x080eaf5c 0x080eaf5c 0x01024 0x01e48 RW  0x1000
  NOTE           0x0000f4 0x080480f4 0x080480f4 0x00044 0x00044 R   0x4
  TLS            0x0a1f5c 0x080eaf5c 0x080eaf5c 0x00010 0x00028 R   0x4
  GNU_STACK      0x000000 0x00000000 0x00000000 0x00000 0x00000 RW  0x10
  GNU_RELRO      0x0a1f5c 0x080eaf5c 0x080eaf5c 0x000a4 0x000a4 R   0x1
  ```
  

  - 下图中VMA0是代码（可读可执行）segment，VMA1是可读可写的segment 都是LOAD类型

  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-29-08-55-40.png
  width=290px>
  </center>
  
  
- 从section角度看就是连接视图，从segment角度看是执行视图

程序头表（PHT）


- **ELF可执行文件**中存在程序头表数据结构，ELF连接文件中没有
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-29-09-03-20.png
  width=390px>
  </center>
  
- 对于LOAD类型（代码+数据）来说FileSize不能大于MemSiz，不然虚拟内存会放不下

- 而如果说MemSiz大于FileSiz是为什么？表示该segment分配的内存空间大于实际文件大小。多余部分填充0。想一想bss段。额外部分是不是可以作为bss段的空间。==因为bss段本身在实际文件中不占大小，而在内存中初始化后需要占空间的==

### 堆和栈 0729

VMA除了用来存储可执行文件中各个Segment之外

- 命令：`./sectionmapping.elf &` 得到 [1] 19314 进程号
- `cat /proc/19314/maps` 查看进程空间分布
  
  ```c 
    虚拟地址范围        权限   偏移   主:从设备号 映像文件节点      映像文件路径        
    08048000-080ea000 r-xp 00000000 08:01 1054322    /home/matytan/Desktop/sectionmapping.elf
    080ea000-080ec000 rw-p 000a1000 08:01 1054322    /home/matytan/Desktop/sectionmapping.elf
    080ec000-080ed000 rw-p 00000000 00:00 0 
    09db0000-09dd2000 rw-p 00000000 00:00 0          [heap] //堆
    b7f3b000-b7f3e000 r--p 00000000 00:00 0          [vvar]
    b7f3e000-b7f40000 r-xp 00000000 00:00 0          [vdso] 内核空间
    bfe68000-bfe89000 rw-p 00000000 00:00 0          [stack] 栈
  ```
  
  - 偏移是指segment在映像文件中的偏移
  - 7个VMA虚拟内存区域，只有前面两个映射到了可执行文件中两个segment。后面几个VMA没有映射到文件的，称为匿名虚拟内存区域。（mmap）
  - 其中 堆和栈，栈大小（0x00021000）
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-29-09-23-58.png
  width=390px>
  </center>


### 堆的最大申请数量

### 段地址对齐

- 假设页面大小为4096字节，即4kb，虚拟地址和物理地址要进行映射，物理地址这段空间必须是4096的整数倍，但是这样会有一个问题，比如我占127字节的segment也需要一页物理内存即4096，会造成空间的浪费，形成内部碎片。
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-30-08-41-39.png
      width=390px>
  </center>
  
  
- Linux的做法是segment0和segment1的接壤部分共享一个物理页面，然后该物理界面映射两次到虚拟页面

<center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-30-08-40-23.png
      width=390px>
</center>

- 因此对于一个物理页面，可能含有多个段，比如文件头，代码段，数据段。


### 进程栈的初始化

- 进程运行时需要知道 系统环境变量和运行的参数
- 利用虚拟空间段中的栈保存 stack VMA。
```c
HOME = /home/user
PATH = /usr/bin

prog 123 
```
- 上面程序 堆栈段底部地址为0xBF802000
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-30-08-54-12.png
  width=390px>
  </center>

## Linux内核装载ELF过程简介

- bash进程先调用fork()系统调用创建一个新的进程，然后新的进程调用execve()系统调用执行新的程序，之前的bash返回继续等待刚刚启动的进程结束

```c
int execve(const char* filename,char *const argv[],char *const envp[]);
```
- execve系统调用相应入口是sys_execve()，它进行参数检查后，然后调用do_execve()，它会首先查找被执行的文件，读取文件的前128字节，判断可执行文件的类型

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-30-09-10-01.png
width=490px>
</center>
