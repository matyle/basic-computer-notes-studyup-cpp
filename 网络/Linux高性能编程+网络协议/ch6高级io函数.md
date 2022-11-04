
### 高级io函数

#### 创建文件描述符的函数 如和pipe，dup/dup2

- pipe函数
  - pipe函数用于创建一个管道
    ```c
    #include<unistd.h>
    int pipe(int fd[2]);
    ```
  - 文件描述符fd[0]和fd[1]分别构成管道的两端
  - 从fd[1]写入 fd[0] 读出
- dup函数和dup2函数
  - dup返回一个当前系统最小的文件描述符，dup2返回不小于fd2的整数值
  - CGI服务器 61.c

    ```c
    #include<unistd.h>
    int dup(int fd);
    int dup2(int fd1,int fd2);
    ```

- readv和writev函数
  - readv分散读，writev函数集中写
  - 例如web服务器，状态行，头部字段，空行 三个部分和文档可能不在同一块内存。
  - file_stat st_mode 宏 
  - `snprintf()`函数
    ```c
    #include<sys/uio.h>
    ssize_t readv(int fd,const struct iovec* vector,int count);
    ssize_t writev(int fd,const sturct iovec* vector,int count);
    struct iovec
    {
        void *iov_base;	/* Pointer to data.  */
        size_t iov_len;	/* Length of data.  */
    };
    ```
- sendfile函数
  - in_fd参数是待读出内容的文件描述符 out_fd是待写入内容的文件描述符
  ```c
    #include<sys/sendfile.h>
    ssize_t sendfile(int out_fd,int in_fd,off_t* offset,size_t count);
    sendfile(connfd,filefd,NULL,file_stat.st_size);//从fliefd写入connfd
    ```

- mmap和munmap函数
    - mmap函数用于申请一段内存空间，这段内存作为进程通信间的共享内存
    - munmap函数则释放这段内存空间
    ```c
    #include<sys/mman.h>
    void mmap(void *start,size_t length,int prot,int falgs,int fd,int off_set);
    int munmap(void *start,size_t length);
    //start允许用户使用特定地址作为内存的起始地址如为NULL 则自动分配 prot表示权限：
    //取以下几个值按位或
    //PROT_READ RROT_WRITE PROT_EXEC PROT_NONE
    //flags 控制内存短内容被修改后程序的行为
    ```   
- splice函数:在两个文件描述符直接移动数据
    ```c
    #include<fcntl.h>
    ssize_t splice(int fd_in,loff_t* off_in,int fd_out,loff_t* off_out,size_t len,unsigned int flags);
    ```
- tee函数:在两个文件描述符直接复制数据 零拷贝
    ```c
    #include<fcntl.h>
    ssize_t tee(int fd_in,int fd_out,size_t len,unsigned int flags);
    ```

- fcntl函数 :flie control 提供对文件描述符的各种控制操作;另外一个常见的控制文件描述符属性和行为的系统调用是ioctl
```c
#include<fcntl.h>
int fcntl(int fd,int cmd,...); //cmd参数指定执行何种类型的参数
```
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-05-12-14-47-17.png
width=400px>
    <br>
    <div style="color:orange; border-bottom: 1px solid #d9d9d9;
    display: inline-block;
    color: #999;
    padding: 2px;"></div>
</center>


