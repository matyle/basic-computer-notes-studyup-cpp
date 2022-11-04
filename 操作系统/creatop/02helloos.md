
####  02 Hello os

- PC机引导流程
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-12-12-22-33.png
  width=300px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;">引导流程图</div>
  </center>
  
    - pc机上电后会运行bios固件中的第一条指令：负责检测和初始化cpu 内存 主板平台，加载引导设备的第一个扇区数据，到0x7c00地址开始的内存空间，接着跳转到0x7c00执行指令
- hello os的引导汇编代码：不能直接用c 即entry.asm中代码
  - db定义字节类型变量，一个字节数据占1个字节单元，读完一个，偏移量加1
  - dw定义字类型变量，一个字数据占2个字节单元，读完一个，偏移量加2
  - dd定义双字类型变量，一个双字数据占4个字节单元，读完一个，偏移量加4
    ```s
    MBT_HDR_FLAGS	EQU 0x00010003
    MBT_HDR_MAGIC	EQU 0x1BADB002 ;多引导协议头魔数
    MBT_HDR2_MAGIC	EQU 0xe85250d6 ;第二版多引导协议头魔数
    global _start ;导出_start符号
    extern main ;导入外部的main函数符号

    [section .start.text] ;定义.start.text代码节
    [bits 32] ;汇编成32位代码
    _start:
        jmp _entry
    ALIGN 8
    mbt_hdr:
        dd MBT_HDR_MAGIC ;dd是表示定义双字节变量
        dd MBT_HDR_FLAGS
        dd -(MBT_HDR_MAGIC+MBT_HDR_FLAGS)
        dd mbt_hdr
        dd _start
        dd 0
        dd 0
        dd _entry

    ;以上是GRUB所需要的头
    ALIGN 8
    mbt2_hdr:
        DD	MBT_HDR2_MAGIC
        DD	0
        DD	mbt2_hdr_end - mbt2_hdr
        DD	-(MBT_HDR2_MAGIC + 0 + (mbt2_hdr_end - mbt2_hdr))
        DW	2, 0
        DD	24
        DD	mbt2_hdr
        DD	_start
        DD	0
        DD	0
        DW	3, 0
        DD	12
        DD	_entry
        DD      0
        DW	0, 0
        DD	8
    mbt2_hdr_end:
    ;以上是GRUB2所需要的头
    ;包含两个头是为了同时兼容GRUB、GRUB2

    ALIGN 8

    _entry:
        ;关中断
        cli
        ;关不可屏蔽中断
        in al, 0x70
        or al, 0x80
        out 0x70,al
        ;重新加载GDT
        lgdt [GDT_PTR]
        jmp dword 0x8 :_32bits_mode

    _32bits_mode:
        ;下面初始化C语言可能会用到的寄存器
        mov ax, 0x10
        mov ds, ax
        mov ss, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        xor eax,eax
        xor ebx,ebx
        xor ecx,ecx
        xor edx,edx
        xor edi,edi
        xor esi,esi
        xor ebp,ebp
        xor esp,esp
        ;初始化栈，C语言需要栈才能工作
        mov esp,0x9000
        ;调用C语言函数main
        call main
        ;让CPU停止执行指令
    halt_step:
        halt
        jmp halt_step


    GDT_START:
    knull_dsc: dq 0
    kcode_dsc: dq 0x00cf9e000000ffff
    kdata_dsc: dq 0x00cf92000000ffff
    k16cd_dsc: dq 0x00009e000000ffff
    k16da_dsc: dq 0x000092000000ffff
    GDT_END:

    GDT_PTR:
    GDTLEN	dw GDT_END-GDT_START-1
    GDTBASE	dd GDT_START
    ```
- 主函数
  - 汇编中外部引入了main函数

- 控制计算机屏幕 
  - 显卡VESA标准，VGABIOS
  - 它把屏幕分成 24 行，每行 80 个字符，把这（24*80）个位置映射到以 0xb8000 地址开始的内存中，每两个字节对应一个字符，其中一个字节是字符的 ASCII 码，另一个字节为字符的颜色值。
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-05-14-22-00-01.png
  width=500px>
      <br>
      <div style="color:orange; border-bottom: 1px solid #d9d9d9;
      display: inline-block;
      color: #999;
      padding: 2px;"></div>
  </center>
  
- make工具 [博客]([https://link](https://blog.csdn.net/u013774102/article/details/79043559?utm_medium=distribute.pc_relevant.none-task-blog-baidujs_title-0&spm=1001.2101.3001.4242))
  - 读取makefile文件，
  - make中的格式
  - $(宏名)
  - makefile中\$^ 、 \$@、\$<分别代表所有依赖(去除重复）、目标文件、第一个依赖



- 启动
```c
menuentry 'HelloOS' { insmod part_msdos #GRUB加载分区模块识别分区 
insmod ext2 #GRUB加载ext文件系统模块识别ext文件系统 
set root='hd0,msdos4' #注意boot目录挂载的分区，这是我机器上的情况 m
ultiboot2 /boot/HelloOS.bin #GRUB以multiboot2协议加载HelloOS.bin 
boot #GRUB启动HelloOS.bin}
```



