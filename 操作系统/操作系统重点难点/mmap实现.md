## mmap实现分析

`man mmap`

[深入理解计算机系统](/Users/matytan/Desktop/code/基础/操作系统/深入理解计算机系统2个番茄/重点ch10虚拟内存.md)

```c
mmap, munmap - map or unmap files or devices into memory
#include <sys/mman.h>

void *mmap(void *addr, size_t length, 
           int prot, int flags,
           int fd, off_t offset);
int munmap(void *addr, size_t length);
             
```



mmap本质：**当前进程分配（或找到）一个合适的vma（虚拟内存地址），然后为该vma设置对应的缺页处理函数**。

- mmap按照flag可以分为匿名映射和非匿名映射，又可分为shared映射和private映射

  (1) 匿名shared映射：fd为-1，可用于父子进程通信。

  (2) 匿名private映射：例如malloc大块的内存（大于128k）（内容初始化为全0）

  (3) 非匿名shared映射：常见的用于进程通信方式。（共享内存，写同一个文件）

  (4) 非匿名private映射：例如程序在启动时加载so时，就是用的这种方式，相当于“写时拷贝COW”。

- mmap函数只是为进程分配了虚拟内存空间，并没有真的建立虚拟内存和物理内存的映射。这个建立映射的过程是到缺页中断的函数中进行的。

  

- MAP_SHARED  共享，同一区域的其他线程对这块映射内存可见，支持文件映射传递。

   Share  this  mapping.  Updates to the mapping are visible to other processes mapping the same region, and (in the case  of  file-backed  mappings)  are carried through to the underlying file.  (To precisely control when updates are carried through to the underlying  file  requires the use of msync(2).)

- MAP_SHARED_VALIDATE (since Linux 4.15):行为跟MAP_SHARED相同

  - This  flag  provides  the  same  behavior  as  MAP_SHARED  except  that

    MAP_SHARED mappings ignore unknown flags in flags.  By  contrast,  when creating  a  mapping using MAP_SHARED_VALIDATE, the kernel verifies all passed flags are known and fails the mapping with the error  EOPNOTSUPP for  unknown  flags.   This mapping type is also required to be able to use some mapping flags (e.g., MAP_SYNC).

- MAP_ANONYMOUS 匿名映射：任何文件都不支持映射，内容初始化为全0。挂载到zero

   The mapping is not backed by any file; its contents are initialized  to zero. The  fd  argument is ignored; however, some implementations require fd to be -1 if MAP_ANONYMOUS (or MAP_ANON) is specified, and portable  applications should ensure this.  The offset argument should bezero.  The use of MAP_ANONYMOUS in conjunction with MAP_SHARED is  supported on Linux only since kernel 2.4.





（1）非匿名shared映射：调用文件各自文件系统的缺页函数；

（2）非匿名private映射：调用文件各自文件系统的缺页函数；

（3）匿名shared映射：调用tmpfs文件系统的缺页函数；

（4）匿名private映射：do_anonymous_page处理缺页，也是目前唯一支持THP（透明大页）的方式。

