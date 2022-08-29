# 第三章 目标文件里有什么

## 3.1 目标文件的格式
- 目标文件从格式上来讲是编译后的可执行文件，只是还没有经过连接的过程。有些符号有些地址还没有被调整

- **Windows中的PE和linux中的ELF可执行文件格式（文件类型有四种）** 都是COFF的变种
- 目标文件是编译后未链接的中间文件(.o)，与可执行文件结构很相似，他们统称为ELF

可重定位文件
- 动态链接库（DLL），Windows下是.dll，Linux下是.so
- 静态链接库,windows下是.lib，linux下是.a

**四种ELF格式文件类型：**
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-20-09-56-03.png
  width=490px>
  </center>

- COFF在目标文件里面引入了”段“机制

### 目标文件是什么样的？
- 目标文件按不同属性”分节“或者”分段“储存。**节和段（segment多个同类型的节组成）唯一区别是在链接视图和装载视图时有区别**

几个重要段：
- **程序源代码和机器代码** 放到了 **代码段.code或者.text**
- **全局变量和静态局部变量** 放到了 **.data数据段**
- **未初始化的全局变量和静态局部变量** 放到了 **.bss数据段只是为未初始化的全局变量和静态局部变量预留位置，记录他们需要的位置**，因此BSS段实际上不占据空间，并没有实际内容
- 只读段 .rodate (read only)
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-20-10-07-06.png
  width=450px>
  </center>
  
- 文件开头的头文件描述文件属性 **ELF格式**
  - 文件是否可执行，如果是可执行文件（入口地址）
  - 是静态链接还是动态链接
  - 目标硬件
  - 目标操作系统等
  - **段表**：是一个描述文件中各**个段的数组，各个段在文件中的偏移位置以及段的属性**
  - 注意.bss段是不占文件空间的，为未初始化的全局变量和局部静态变量预留位置

-  为什么指令和数据要分开放？
   -  1. 程序装载后，指令和数据分别映射到虚存的两个区域，数据对于进程可读写，而指令对进程是**只读**的，防止指令被改写
   - 2. 指令和数据分开可以提高程序的局部性。**设计数据缓存和指令缓存**
   - 3. **最重要的原因**：当系统中运行多个该程序的副本时，他们指令是相同的，所以内存中只需要保存一份该程序的副本。因为指令区域是只读的，因此可以直接共享，能节省大量内存

### 挖掘Simplesection.o

- 真正了不起的程序员，对自己的程序每一个字节都了如指掌~
- 比如了解tcpip却从没有看到过包的结构，包的头部
- `objdump -h SimpleSection.o`
  - .rodata(只读数据段) 
  - .comment 注释数据段
  - .note.GNU-stack 堆栈提示段
  - .bss 我们发现只有4字节。（全局未初始化的变量在最终链接成可执行文件的时候再在.bss段分配空间）
- 属性：
  - 长度Size，段所在位置File off
  - CONTENTS表示该段在文件中存在
```c

SimpleSection.o:     file format elf64-x86-64

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .text         0000005f  0000000000000000  0000000000000000  00000040  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
  1 .data         00000008  0000000000000000  0000000000000000  000000a0  2**2
                  CONTENTS, ALLOC, LOAD, DATA
  2 .bss          00000004  0000000000000000  0000000000000000  000000a8  2**2
                  ALLOC
  3 .rodata(只读数据段)      00000004  0000000000000000  0000000000000000  000000a8  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
  4 .comment      0000002b  0000000000000000  0000000000000000  000000ac  2**0
                  CONTENTS, READONLY
  5 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000000d7  2**0
                  CONTENTS, READONLY
```

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-20-10-52-16.png
width=290px>
</center>


- size SimpleSection.o

### 代码段 .text

- objdump -s -d SimpleSection.o
- 55 8位为一个字节。 size 0x62
```c
Contents of section .text:
 0000 5589e583 ec0883ec 08ff7508 68000000  U.........u.h... 16字节 
 0010 00e8fcff ffff83c4 1090c9c3 8d4c2404  .............L$.
 0020 83e4f0ff 71fc5589 e55183ec 14c745f0  ....q.U..Q....E.
 0030 01000000 8b150400 0000a100 00000001  ................
 0040 c28b45f0 01c28b45 f401d083 ec0c50e8  ..E....E......P.
 0050 fcffffff 83c4108b 45f08b4d fcc98d61  ........E..M...a
 0060 fcc3                                 ..        
 Disassembly of section .text:

00000000 <func1>:
   0:   55                      push   %ebp
   1:   89 e5                   mov    %esp,%ebp
   3:   83 ec 08                sub    $0x8,%esp
   6:   83 ec 08                sub    $0x8,%esp
   9:   ff 75 08                pushl  0x8(%ebp)
   c:   68 00 00 00 00          push   $0x0
  11:   e8 fc ff ff ff          call   12 <func1+0x12>
  16:   83 c4 10                add    $0x10,%esp
  19:   90                      nop
  1a:   c9                      leave  
  1b:   c3                      ret    

0000001c <main>:
  1c:   8d 4c 24 04             lea    0x4(%esp),%ecx
  20:   83 e4 f0                and    $0xfffffff0,%esp
  23:   ff 71 fc                pushl  -0x4(%ecx)
  26:   55                      push   %ebp
  27:   89 e5                   mov    %esp,%ebp
  29:   51                      push   %ecx
  2a:   83 ec 14                sub    $0x14,%esp
  2d:   c7 45 f0 01 00 00 00    movl   $0x1,-0x10(%ebp)
  34:   8b 15 04 00 00 00       mov    0x4,%edx
  3a:   a1 00 00 00 00          mov    0x0,%eax
  3f:   01 c2                   add    %eax,%edx
  41:   8b 45 f0                mov    -0x10(%ebp),%eax
  44:   01 c2                   add    %eax,%edx
  46:   8b 45 f4                mov    -0xc(%ebp),%eax
  49:   01 d0                   add    %edx,%eax
  4b:   83 ec 0c                sub    $0xc,%esp
  4e:   50                      push   %eax
  4f:   e8 fc ff ff ff          call   50 <main+0x34>
  54:   83 c4 10                add    $0x10,%esp
  57:   8b 45 f0                mov    -0x10(%ebp),%eax
  5a:   8b 4d fc                mov    -0x4(%ebp),%ecx
  5d:   c9                      leave  
  5e:   8d 61 fc                lea    -0x4(%ecx),%esp
  61:   c3                      ret       
```

### 3.3.2 数据段和只读数据段（只针对全局变量或者静态局部变量）

- 两个已经初始化的int 8字节 
- "%d\n" 是一种只读字段  4字节（\0)结尾 %和d两个字节 \n \0 两个字节
- .rodata段只存放只读数据，（如const修饰的已初始化的全局变量或者静态局部变量）
- 54000000 小端存储 -- 00000054
- 小端存储：底地址位存16进制数的低位

```c
Contents of section .data:
 0000 54000000 55000000                    T...U...        
Contents of section .rodata:
 0000 03000000 25640a00 03000000           ....%d......    
```

### BSS段
- `00000000 l     O .bss   00000004 static_var2.1490` 可以看到static_var2.1490在bss段中
- 但是globle_uninit_var却没有在bss段中？ 
- 只是一个未定义的*COM*符号，跟编译器有关。有的编译器会将全局未初始化的变量放在bss段，有一些只是预留一个**未定义全局变量的符号**
- globle_uninit_var之前加上static修饰会在bss段中找到

- `static int i = 0;`**是存放在bss段中的，**因为为**0都认为是未初始化的变量**，因此被优化到了bss中节省内存。
```c
static int x1 = 0; //放在.bss
static int x2 =  1; //放在.data
```


- 符号表
```c
SYMBOL TABLE:
00000000 l    df *ABS*  00000000 SimpleSection.c
00000000 l    d  .text  00000000 .text
00000000 l    d  .data  00000000 .data
00000000 l    d  .bss   00000000 .bss
00000000 l    d  .rodata        00000000 .rodata
00000004 l     O .data  00000004 static_var.1489
00000000 l     O .bss   00000004 static_var2.1490
00000008 l     O .rodata        00000004 const_var2.1491
00000000 l    d  .note.GNU-stack        00000000 .note.GNU-stack
00000000 l    d  .eh_frame      00000000 .eh_frame
00000000 l    d  .comment       00000000 .comment
00000000 g     O .data  00000004 globle_init_var
00000004       O *COM*  00000004 globle_uninit_var
00000000 g     O .rodata        00000004 const_var
00000000 g     F .text  0000001c func1
00000000         *UND*  00000000 printf
0000001c g     F .text  00000046 main
```

### 其他段
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-20-12-09-43.png
width=490px>
</center>

- 向段中插入数据利用objcopy `objcopy -I binary -O elf32-386 -B i386 image.jpg image.o`

**自定义段**
- 以前早期段管理估计就这么玩
```c
__attribute__((section("FOO"))) int global = 42; //加入到段FOO中
__attribute__((section("BAR"))) void foo; //加入到段BAR中

```

## ELF文件结构描述
- 基本结构

|  ELF Header  文件头|  
|----|
|  .text  |
|  .data  |
|  .bss  |
|  ...<br>other section |  
|  string table<br>Symbol table  | 

- ELF文件头：描述整个文件的基本信息


### 文件头
- Magic(魔数)
- `readelf -h SimpleSection.o` 读取ELF文件头
- 魔数
```c
ELF Header:
  Magic:   7f(DEL控制符) 45(E) 4c(L) 46(F) 01（32位） 01（字节序) 01(版本号) 00 00 00 00 00 00 00 00 00 （16字节）
  Class:                             ELF32
  Data:                              2's complement, little endian（小端存储）
  Version:                           1 (current)
  OS/ABI:                            UNIX - System V
  ABI Version:                       0
  Type:                              REL (Relocatable file) (重定位文件)
  Machine:                           Intel 80386
  Version:                           0x1
  Entry point address:               0x0 //入口地址 可重定位文件一般没有入口地址
  Start of program headers:          0 (bytes into file)
  Start of section headers:          896 (bytes into file)//段表的偏移地址
  Flags:                             0x0
  Size of this header:               52 (bytes)//elf本身文件头大小
  Size of program headers:           0 (bytes)
  Number of program headers:         0
  Size of section headers:           40 (bytes)//段表每个条目大小 
  Number of section headers:         13 //段表条目数量
  Section header string table index: 10 //段表字符串表
```
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-21-08-47-04.png
width=490px>
</center>

```cpp
typedef struct
{
  unsigned char	e_ident[EI_NIDENT];	/* Magic number and other info EI_NIDENT=16 刚好为魔数大小*/
  Elf32_Half	e_type;			/* Object file type */
  Elf32_Half	e_machine;		/* Architecture */
  Elf32_Word	e_version;		/* Object file version */
  Elf32_Addr	e_entry;		/* Entry point virtual address */
  Elf32_Off	e_phoff;		/* Program header table file offset */
  Elf32_Off	e_shoff;		/* Section header table file offset */
  Elf32_Word	e_flags;		/* Processor-specific flags */
  Elf32_Half	e_ehsize;		/* ELF header size in bytes */
  Elf32_Half	e_phentsize;		/* Program header table entry size */
  Elf32_Half	e_phnum;		/* Program header table entry count */
  Elf32_Half	e_shentsize;		/* Section header table entry段表条目大小 size */
  Elf32_Half	e_shnum;		/* Section header table entry count */
  Elf32_Half	e_shstrndx;		/* Section header string table index */
} Elf32_Ehdr;
```

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-21-08-50-33.png
width=450px>
</center>
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=
width=450px>
</center>

- ET_REL, 1 可重定位
- ET_EXEC, 2， 可执行文件
- ET_DYN，3， 共享目标文件



### 段表(section table) 段表实际也是一个段
- `readelf -S SimpleSection.o`
- 段表是除了elf头文件外，最重要的结构,描述了各个段的信息
- 是以Elf32_Shdr结构为元素的数组
- 段表大小根据段表条目大小和数量计算（在elf头文件中）
- 段表和ELF头文件并不相邻！
- .rel.text段 类型为SHT_REL 也就是一个重定位表
```cpp
There are 13 section headers, starting at offset 0x380 (896btye): 表大小13*40=
Section Headers:
  [Nr] Name              Type            Addr     Off    Size   ES Flg Lk Inf Al
  [ 0]                   NULL            00000000 000000 000000 00      0   0  0
  [ 1] .text             PROGBITS        00000000 000034 000062 00  AX  0   0  1
  [ 2] .rel.text         REL             00000000 0002e8 000028 08   I 11   1  4
  [ 3] .data             PROGBITS        00000000 000098 000008 00  WA  0   0  4
  [ 4] .bss              NOBITS          00000000 0000a0 000004 00  WA  0   0  4（2的4次方倍）
  [ 5] .rodata           PROGBITS        00000000 0000a0 00000c 00   A  0   0  4
  [ 6] .comment          PROGBITS        00000000 0000ac 000036 01  MS  0   0  1
  [10] .shstrtab         STRTAB(字符串表)          00000000 000320 00005f 00      0   0  1
  [11] .symtab           SYMTAB  （符号表         00000000 000148 000120 10     12  12  4
  [12] .strtab           STRTAB          00000000 000268 000080 00      0   0  1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings)
  I (info), L (link order), G (group), T (TLS), E (exclude), x (unknown)
  O (extra OS processing required) o (OS specific), p (processor specific)

//段描述符结构
typedef struct
{
  Elf32_Word	sh_name;		/* Section name (string tbl index) */
  Elf32_Word	sh_type;		/* Section type */
  Elf32_Word	sh_flags;		/* Section flags */
  Elf32_Addr	sh_addr;		/* Section virtual addr at execution */
  Elf32_Off	sh_offset;		/* Section file offset */
  Elf32_Word	sh_size;		/* Section size in bytes */
  Elf32_Word	sh_link;		/* Link to another section */
  Elf32_Word	sh_info;		/* Additional section information */
  Elf32_Word	sh_addralign;		/* Section alignment */
  Elf32_Word	sh_entsize;		/* Entry size if section holds table */
} Elf32_Shdr;

Elf32_Shdr elf32shdr[];
```

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-21-09-12-42.png
width=450px>

</center>

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-21-09-25-23.png
width=390px>
</center>

- 段名只在编译过程有意义，不能真正表示段的属性。可以将数据段命名为.text（随意命名）。真正决定段的属性是**段的类型sh_type和段的标志位sh_flags**
  - 相关常量以SHT_开头
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-21-09-32-30.png
  width=450px>
  </center>
  
  - 段的标志位：进程虚拟空间中的属性，比如是否可写，是否可以执行等
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-21-09-35-28.png
  width=450px>
  </center>

  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-21-09-37-31.png
  width=450px>
  </center>
  

  - 段的链接信息（sh_link,sh_info)
    - 如果段的类型是与链接相关的（不论动态链接还是静态链接），如重定位，符号表
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-21-09-44-30.png
  width=490px>
  </center>
  
### 重定位表
- 处理目标文件时，需要对目标文件某些部位进行重定位，代码段和数据段中那些对绝对地址引用的位置。对于需要重定位的数据段或者代码段都有相应的重定位表，例如.rel.text的重定位表针对的额是.text
- .rel.text段 类型为SHT_REL 也就是一个重定位表
- .rel.text的重定位表针对的额是.text，对应.data的重定位文件表示.rel.data段
- sh_info也就是Inf表示它作用于哪个段，例如.rel.text的Inf为1 表示作用于段下标为1的.text段


### 字符串表

- ELF文件中用到了很多字符串，比如段名，变量名等。
- 由于字符串长度不固定，因此难以用固定结构存储。常见做法是将字符串集中存放到一个表。
- 字符串在表中的偏移引用字符串
- **偏移如图：终于明白了'\0'的作用了~~~**，只需要给一个字符的起始偏移就行了，长度都不用给，因为\0结束
- 只有 分析ELF头 可以获得段表和段表字符串表的位置，从而解析整个ELF头文件（字符串表这个段存的有段名等字符串）
  <center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-21-10-01-48.png
  width=490px>
  </center>


- **字符串表.strtab和段表字符串表.shstrtab**:.strtab用来存放普通的字符串，.shstrtab则存放段表中用到的的字符串（例如段名）。
  
- ELF头文件中e_shstrndx含义：Section Header string table index **表示.shstrtab在段中的下标**
  - 因此我们只需要分析ELF文件头，就可以得到段表和段表字串符串表的位置（然后根据段名进一步得到其他段）


## 链接的接口----符号

- 将不同的目标文件粘在一起，像积木一样拼装成一个整体
- 目标文件必须有固定的规则，目标文件之间拼合实际上是目标文件之间对地址（函数或变量地址）的引用
  - 例如目标文件B要用目标文件A中的函数foo（称A**定义**了函数foo，B**引用**了A中函数foo）,变量也一样
  - 因此每个函数和变量需要有自己独特的名字，因此我们在链接中将函数和变量统称为**符号symbol**,**函数名和变量名称为符号名Symbol name**
  - 每一个目标文件都有个一个**符号表（Symbol Table）**
  - 每一个定义的符号有一个对应的值，叫做**符号值 Symbol value**，对于函数和变量：符号值就是他们的地址
- 符号类型分类
  - **定义在本目标文件的全局符号**，可以被其他目标文件引用。例如SimpleSection.o中的func1，main，global_init_val。
  - 在本目标文件引用的全局符号，却没有定义在本目标文件。一般称为**外部符号External Symbol**，如何SimpleSection.o中的printf
  - **段名**，这种符号由编译器产生，值就是该段的起始地址，如SimpleSection.o中的.text,.data等
  - 局部符号：编译单元内部可见，对链接过程没有作用
  - 行号信息：目标文件指令与源代码行的对应关系，可选。
  - **最值得关注的是全局符号**‘
- **注意：没有普通局部变量的符号**

- nm查看符号结果 nm SimpleSection.o
  ```c
  00000000 R const_var
  00000008 r const_var2.1491
  00000000 T func1
  00000000 D globle_init_var
  00000004 C globle_uninit_var
  0000001c T main
          U printf
  00000004 d static_var.1489
  00000000 b static_var2.1490
  ```

### ELF符号表(.symtab,st)结构 

- 符号表是文件中的一个段，段名一般为.symtab
- Elf32_Sym结构：
  ```cpp
  typedef struct
  {
    Elf32_Word	st_name;		/* Symbol name (string tbl index) */
    Elf32_Addr	st_value;		/* Symbol value */
    Elf32_Word	st_size;		/* Symbol size */
    unsigned char	st_info;		/* Symbol type and binding */
    unsigned char	st_other;		/* Symbol visibility */
    Elf32_Section	st_shndx;		/* Section index 符号所在段的下标*/
  } Elf32_Sym;
  ```

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-21-10-37-46.png
width=490px>
</center>

- **符号类型（低四位）和绑定信息（高28位)**
- **绑定信息 宏定义**
  - STB_LOCAL：局部符号，目标文件外部不可见
  - STB_GLOBL 全局符号，外部可见
  - STB_WEAK 若引用
- **符号类型 0-4**
  - STT_NOTYPE 未知类型符号
  - STT_OBJECT 数据对象，变量，数据等
  - STT_FUNC 符号是个函数或者其他可执行代码
  - STT_SECTION 符号表示一个段，符号必须是STB_LOCAL
  - STT_FILE 符号表示一个文件名，目标文件对应的源文件名。STB_LOCAL类型，且**st_shndx**一定是SHN_ABS

- 符号所在段**sh_shndx**：如果符号定义在本目标文件中，那么这个；如果成员不在目标中，或者特殊符号，sh_shndx有些特殊这
  - SHN_ABS:符号包含一个绝对的值，比如文件名
  - SHN_COMMON：未初始化的全局定义的变量，该符号是一个COMMAN块
  - SHN_UNDEF：表示该符号未定义,本目标文件被引用，但是未在本目标文件定义

- **符号名st_name**:注意的是符号名是不能直接存的字符串的。而是**指向对应的字符串表去找对应的字符串**
- 符号值 st_value:如果符号是一个变量或者函数，那么符号值就是他们的地址，以下几种情况区别对待
  - 目标文件中，如果是符号的定义，并且不是COMMAN块，那么st_value表示该符号在段中的偏移,如func1，main，global_init_val
  - 目标文件中，如果符号是COMMAN块，即sh_shndx为SHN_COMMAN,则st_value表示该符号的对齐属性
  - 可执行文件中，st_value的值是符号的虚拟地址，对动态链接器十分有用




- `readelf -s SimpleSection.o`
  ```c
  Symbol table '.symtab' contains 18 entries:
   Num:    Value  Size Type    Bind   Vis      Ndx Name
     0: 00000000     0 NOTYPE  LOCAL  DEFAULT  UND 
     1: 00000000     0 FILE    LOCAL  DEFAULT  ABS SimpleSection.c
     6: 00000004     4 OBJECT  LOCAL  DEFAULT    3 static_var.1489
     7: 00000000     4 OBJECT  LOCAL  DEFAULT    4 static_var2.1490
     8: 00000008     4 OBJECT  LOCAL  DEFAULT    5 const_var2.1491 rodata
    12: 00000000     4 OBJECT  GLOBAL DEFAULT    3 globle_init_var
    13: 00000004     4 OBJECT  GLOBAL DEFAULT  COM globle_uninit_var
    14: 00000000     4 OBJECT  GLOBAL DEFAULT    5 const_var rodata
    15: 00000000    28 FUNC    GLOBAL DEFAULT    1 func1
    16: 00000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
    17: 0000001c    70 FUNC    GLOBAL DEFAULT    1 main


    00000000 l    df *ABS*  00000000 SimpleSection.c
    00000000 l    d  .text  00000000 .text
    00000000 l    d  .data  00000000 .data
    00000000 l    d  .bss   00000000 .bss
    00000000 l    d  .rodata        00000000 .rodata
    00000004 l     O .data  00000004 static_var.1489
    00000000 l     O .bss   00000004 static_var2.1490
    00000008 l     O .rodata        00000004 const_var2.1491
    00000000 g     O .data  00000004 globle_init_var
    00000004       O *COM*  00000004 globle_uninit_var
    00000000 g     O .rodata        00000004 const_var
    00000000 g     F .text  0000001c func1
    00000000         *UND*  00000000 printf
    0000001c g     F .text  00000046 main
  ```


### 特殊符号
- ld链接器来连接产生可执行文件时，会定义很多特殊符号
- 没有在程序中定义，但是可以声明并引用
- 只有使用ld链接器才会存在
  - __executable_start：该符号为程序起始地址（不是入口地址），是程序最开始的地址
  - __etext或_etext,etext代码段结束地址，代码段最末尾的地址
  - _edata,edata,数据段结束地址
  - _end，程序结束地址
  - 以上地址都为程序装载时的虚拟地址
```c
//可以直接使用

  ```

### 符号修饰和函数签名

- 以前的编译器为了防止用户自定义变量名和库中存在的变量或者函数名冲突，因此编译之后加下划线
- c++ 名称空间

#### C++符号修饰：因为C++支持函数重载，即两个函数名可以相同，如参数列表不同，如何实现？
- 符号修饰
- 符号改编
  ```cpp
  int func(int);
  float func(float); //参数列表不同 算重载
  
  class C{
    int func(int);
    class C2{
      int func(int);
    }
  };
  namespace N{
    int func(int);
    class C{
      int func(int);
    }
  }
  ```

<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-06-23-09-54-03.png
width=490px>
</center>

- 函数签名：包含函数信息，函数名，参数类型，所在类和名称空间及其它信息。函数签名使用某种名称修饰
  - C++以 **_Z** 开头，对于嵌套的名字后面紧跟 **”N“**，然后是**各个名称空间和类名**。每个名字前是**名字字符串长度**，以**E**结尾，参数列表紧跟在E后面，对于int来说就是 **”i“**
  - 例如 _ZN+字符串长度+空间名或类名+E
- 变量也遵循以上规则，比如foo空间的全局变量bar，则为_ZN3foo3barE,注意，变量的类型没有加到修饰后的名称中。


### extern ”C“
- 为了与c兼容，C++用来声明或者定义一个C的符号的extern "C"
  ```cpp
  extern "C"{
    int func(int);
    int var;
  }
  //或者
  extern "C" int func(int);
  ```
  - 会将大括号里面的代码当做C语言处理，按前文符号修饰，例如fun会变为_fun，全局变量var会变成_var，Linux的GCC没有这种修饰，extern "C"里面的符号都为修饰后的符号
  - 很多时候头文件中声明了C语言的全局变量和c语言的函数，这时C或者C++都可能会使用，如果说在头文件中直接extern "C" 这样定义，C++可以用，但是c语言不兼容extern "C"用法，因此无法使用
  - 定义两套头文件太麻烦，这时可以使用宏__cplusplus
  - 如果是在C++代码中，会声明extern "C"里面，如果是c会直接使用memset函数
  ```cpp
  #ifdef __cplusplus
  extern "C"{
  #endif
    void *memset(void*, int, size_t);
  #ifdef __cplusplus
  }
  #endif
  ```

### 弱符号和强符号

- 符号重复定义：多个目标文件中含有多个相同名字，如目标文件A和B都定义了全局变量var并且都初始化了，就会报错

- 这种符号定义被称为**强符号**，有些符号被称为**弱符号**，C/C++来说是全局变量未定义是弱符号，全局变量且初始化为强符号,可以利用_`__attribute__((weak))`定义弱符号
```c
extern int ext;
int weak;
int strong = 1;

__attribute__((weak)) weak2 = 2;

int main(){
  return 0;
}
//
```
- ext外部引用，既不是强符号也不是弱符号，strong为强符号，weak和weak2是弱符号
- 规则
  - 规则1：不允许强符号被多次引用
  - 规则2：如果一个符号在某个目标文件中是强符号，在其他文件中是弱符号，则选用强符号
  - 规则3：如果都是弱符号，则选占用空间最大的那个 如文件A中var是int 4字节，文件B中是double，则A和B链接后，var占8字节

- **弱引用和强引用**
  - 如果对外部目标文件的引用在链接时必须要正确决议，如果没有找到该符号定义，则链接器报错。这种称为**强引用**
  - 与之对应有弱引用，如果该符号没有定义，编译器对该引用不会报错。可以将扩展模块定义为弱引用
  - 例如在创建线程时，可以将pthread_create声明成一个弱引用，运行时动态决定是使用单线程还是多线程
  ```c
  //声明弱引用
  __attribute__((weakref)) void foo();
  int main(){
    foo();//运行时报错，foo函数地址为0，类似于纯虚函数
    //正确写法
    if(foo){
      foo();
    }



```c
 //弱引用pthread_create

#include<pthread.h>

  #include<stdio.h>

  int pthread_create( //参数列表
    pthread_t*,
    const pthread_attr_t*,
    void* (*)(void*);
    void*) __attribute__((weak));//ref?
  int main(){
    if(pthread_create){
      //多线程程序
    }
    else{
      //非多线程程序
    }
  }
```