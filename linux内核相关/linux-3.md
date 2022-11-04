## 目录详细

`arch`目录：存放cpu体系结构之间有差异的那些功能模块，会存放到该目录



![image-20211106125910740](linux-3.assets/image-20211106125910740.png)

```javascript
解包：tar xvf FileName.tar
打包：tar cvf FileName.tar DirName
```





<img src="linux-3.assets/image-20211106154929280.png" alt="image-20211106154929280" style="zoom:150%;" />

Docmentation ：文档

- **init** 启动相关代码

- Block 块设备代码

- **driver ** 代码驱动 :占了源码的一半，非常多

  ```bash
  [root@tanqi drivers]# du -sh
  491M    .
  ```

  

- **ipc ** 进程通信

- **fs ** 文件系统

- kernel代码：核心代码 如进程调度

- **mm **：内存管理相关代码

- firmware:固件相关的代码

- include

- scripts：脚本：（gdb）调试

  - https://www.jianshu.com/p/6cdd79ed7dfb

  - 新线程的自动通知

  - **thread thread_id**: 用于在线程之间切换的命令

  - **info threads**: 用于查询当前存在的线程信息

    ```bash
      Id   Target Id         Frame 
    * 2    Thread 0x7ffff75ec700 (LWP 31796) "a.out" func (args=0x0) at hello.c:6
      1    Thread 0x7ffff7ede740 (LWP 31795) "a.out" 0x00007ffff7bc8017 in pthread_join ()
    ```

    

  - **thread apply [thread-id-list] [all] args**: 对一系列的线程应用某一个命令

  - thread-specific breakpoints

  - **set print thread-events**: 控制是否打印线程启动、退出消息

  - **set libthread-db-search-path path**: 假如默认的选择不兼容当前程序的话，让用户选择使用那个`thread-db`

- Lib:通用库





### 简单的字符驱动





- makefile编写

  - 应用程序语法

    ```mak
    all:$(targets)
    $(targets):%:%.o
    	$(CC) $ -o $@ $(CFLAGS)
    %.o:%.cpp
    	$(CC) -c $
    ```

    

- 插入驱动

- 显示所有驱动 lsmod

- remove驱动



- 跟踪wirte过程：从应用层如何调到了驱动的wirte？

  - 1. 系统调用陷入内核

    2. 调用内核的wirte

    3. 调用文件file结构体的wirte函数（callback）==实际上是驱动写的wirte传给file的

       



  ### 编译过程

```bash
find ./ name Makefile
```



### 内核空间和用户空间

TASK_SIZE

PAGE_OFFSET=0xC000000



![image-20211108192059393](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108192059393.png) 





为什么划分？

- 处理器模式不同，权限不同
- 安全考量
- 解耦合（核心代码和业务代码的耦合）

![image-20211108192441930](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211108192441930.png)

`copy_from_user`：用户态拷贝内核态

`copy_to_user`



### 段页式管理 和 页表映射



 

### 逻辑地址到物理地址映射

![image-20211109183508371](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211109183508371.png)

Linux中段选择为0

页目录，页表，偏移量





### 同步操作



