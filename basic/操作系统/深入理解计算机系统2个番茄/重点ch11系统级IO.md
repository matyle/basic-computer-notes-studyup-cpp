# 系统级I/O

- 输入操作是：拷贝数据到主存
- 输出操作是：把数据从**主存拷贝数据到I/O设备**


## UNIX I/IO

- 一个Unix文件就是一个m字节的序列:$B_0,B_1,...,B_k,...,B_{m-1}$
- 所有IO设备例如网络，磁盘和终端都被模型化为文件，所有输入和输出都被当作是文件的读和写（读入主存，主存写出）
  - 1. **打开文件：** 表明它想访问一个IO设备，内核返回一个小的描述符，后续操作都利用描述符标识这个文件。内核记录的所有打开的文件所有信息，应用程序只需要记住描述符。unix创建的每个进程开始时都打开了三个文件描述符，0，1，2.分别代表标准输入，标准输出，标准错误
  - 2. **改变文件位置：** 内核保持一个文件位置k，对于每个打开文件，初始值为0.能够通过seek函数，显式设置文件的当前位置为k
  - 3. **读写文件**。一个读操作就是从文件拷贝n>0个字节到存储器，从当前位置k开始，然后将k增加到k+n。写操作：就是从存储器拷贝n>0个字节到一个文件，从当前文件位置k开始，然后更新k。//ps:所以有覆盖和追加！！默认从0开始写
  - 4. **关闭文件**

## 打开和关闭文件
```c
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
int open(char *filename,int flags,mode_t mode)
```
- flags参数：表明进程如何访问文件
  - O_RDONLY
  - O_WRONLY
  - O_RDWR:可读可写
  - 可以与下面的或
  - O_CREAT 文件不存在，就创建一个截断的（空）文件
  - O_TRUNC:文件存在，就截断它
  - O_APPEND:写操作前，设置文件位置在末尾出

- mode:每一个权限位设置为 mode&u_mask(每一个进程都有一个u_mask)
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-07-11-27-24.png
  width=490px>
  </center>
  


## 读写文件

```c
#include<unistd.h>
ssize_t read(int fd,void *buf,size_t n);//拷贝fd中的内容到（内存）buf中，也就是内存读
sszie_t write(int fd,const void* buf,size_t n);// 将内存buf中的拷贝到文件（IO设备）中fd中
int main(void){
    char c;
    while(read(STDIN_FILENO,&c,1)!=0){
        Write(STDOUT_FILENO,&c,1); //内存拷贝到IO设备上面

    }
    exit(0);
}
```
- ssize_t有符号整型，size_t 无符号整型
- 通过lseek函数可以显式修改当前文件位置
- 读时遇到EOF。如果某文件只有20字节，而我们以50字节的组块进行读取，这时就会会返回不足值为20，read返回0并发出EOF信号
- 从终端读文本行：cin是把内存读取终端文本，也就是输入，注意输入（read，读）是将文件的内容拷贝到内存！写是从内存拷贝内容到文件
- 读和写网络套接字

## 用Rio包进行健壮读和写 内存是主角
- Rio（Robust I/O)包能够自动处理不足值
- 无缓冲的输入输出函数：直接在存储器和文件中传送数据，没有应用级缓冲。尤其在对将二进制数据读写到网络和从网络中读写二进制文件特别有用
- 带缓冲的输入函数：这些函数可以高效地从文件中读取文本行和二进制数据

### Rio的无缓冲的输入输出函数
```c
#include<csapp.h>
ssize_t rio_readn(int fd,void* usrbuf,size_t n);
ssize_t rio_writen(int fd,void* usrbuf,size_t n);
```
- rio_readn函数从描述符fd的当前位置最多传送n个字符到usrbuf中。
- rio_writen函数从位置usrbuf最多传送n个字符到描述符fd中。
- 遇到EOF rio_readn只能返回一个不足值，而rio_writen则不会返回不足值

- rio_readn和rio_writen实现
  ```c
  ssize_t rio_readn(int fd,void* usrbuf,size_t n){
      size_t nleft = n;
      ssize_t nread;
      char* bufp = usrbuf;
      while(nleft>0){
          if((nread=read(fd,bufp,nleft))<0){
              if(errno == EINTR) nread = 0;
              else return -1;       
          }
          else if(nread==0) break;
          nletf-=nread;
          bufp+= nread; //位置加上已经读取的
        
      }
      return (n-nleft);
  }
    ssize_t rio_writen(int fd,void* usrbuf,size_t n){
      size_t nleft = n;
      ssize_t nwrite;
      char* bufp = usrbuf;
      while(nleft>0){
          if((nwrite=write(fd,bufp,nleft))<0){ //读取失败
              if(errno == EINTR) nwrite = 0; 
              else return -1;       
          }
          else if(nwrite==0) break;
          nletf-=nwrite;
          bufp+= nwrite; //位置加上已经写入的。应该写下一个位置
        
      }
      return n;
  }
   
  ```

### Rio带行缓冲的输入函数
- 一个文本行是一个由换行符结尾的ASCII码字符序列。"\n"和ASCII换行符LF相同。 值为0x0a
- 假如要计算文本行数，我们可以read一次一个字节从文件读取到用户存储器，这种方法效率太低，每次都要陷入内核访问IO设备
- 如何实现更好的方法？使用一个缓冲区！调用一个函数每次从一个缓冲区读取一行，当缓冲区空时，read自动重新填满缓冲区

```c
#include "csapp.h"
void rio_readinitb(rio_t *rp,int fd);
ssize_t rio_readlineb(rio_t *rp,viod *usrbuf,size_T maxlen); //按行冬
ssize_t rio_readnb(rio_t *rp, void *usrbuf,size_t n); //
```
- rio_readinitb函数 将描述符fd和地址rp处一个类型为rio_t读缓冲区联系起来
- rio_readlineb :从rp读出来一个文本行拷贝到内存位置usrbuf中，并用null结束文本行，因此此函数最多读取maxlen-1个字符
- rio_readnb从文件rp中最多读n个字节到usrbuf 不按行只是带了缓冲区
  ```c
    #define RIO_BUFSIZE 8192
    typedef struct {
        int rio_fd;                /* 缓冲区的文件描述符 */
        int rio_cnt;               /* 缓冲区的未读字节 */
        char *rio_bufptr;          /* 内部缓冲区下一个未读字节的地址 */
        char rio_buf[RIO_BUFSIZE]; /* 内部缓存 */
    } rio_t;
    void rio_readinitb(rio_t *rp, int fd) 
    {
        rp->rio_fd = fd;  
        rp->rio_cnt = 0;  
        rp->rio_bufptr = rp->rio_buf;
    }
    static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n)
    {
        int cnt;
  
        while (rp->rio_cnt <= 0) {  /* Refill if buf is empty */
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, 
                sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) /* Interrupted by sig handler return */
            return -1;
        }
        else if (rp->rio_cnt == 0)  /* EOF */
            return 0;
        else 
            rp->rio_bufptr = rp->rio_buf; /* Reset buffer ptr */
        }
  
        /* Copy min(n, rp->rio_cnt) bytes from internal buf to user buf */
        cnt = n;          
        if (rp->rio_cnt < n)   
        cnt = rp->rio_cnt;
        memcpy(usrbuf, rp->rio_bufptr, cnt);
        rp->rio_bufptr += cnt;
        rp->rio_cnt -= cnt;
        return cnt;
    }
  
    ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) 
    {
        int n, rc;
        char c, *bufp = usrbuf;
  
        for (n = 1; n < maxlen; n++) { 
            if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') {
                    n++;
                break;
                }
        } else if (rc == 0) {
            if (n == 1)
            return 0; /* EOF, no data read */
            else
            break;    /* EOF, some data was read */
        } else
            return -1;	  /* Error */
        }
        *bufp = 0;
        return n-1;
    }
    ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) 
    {
        size_t nleft = n;
        ssize_t nread;
        char *bufp = usrbuf;
        
        while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0) 
                return -1;          /* errno set by read() */ 
        else if (nread == 0)
            break;              /* EOF */
        nleft -= nread;
        bufp += nread;
        }
        return (n - nleft);         /* return >= 0 */
    }
  ```


## 读取文件元数据
- 应用程序通过stat和fstat函数检索关于文件的信息(元数据metadata)

```c
#include<unistd.h>
#include<sys/stat.h>
int stat(const char *filename,struct stat *buf); 
int fstat(int fd,struct stat *buf); //使用文件描述符
```

- stat函数以一个文件名作为输入 并填写stat数据结构的各个成员
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-08-11-18-17.png
  width=490px>
  </center>
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-08-11-22-09.png
  width=490px>
  </center>
  
- st_mode成员编码了文件访问的许可位与文件类型,st_mode和下面的标志位与
```c
/* File mode */
/* Read, write, execute/search by owner */
#define S_IRWXU         0000700         /* [XSI] RWX mask for owner */
#define S_IRUSR         0000400         /* [XSI] R for owner */
#define S_IWUSR         0000200         /* [XSI] W for owner */
#define S_IXUSR         0000100         /* [XSI] X for owner */
/* Read, write, execute/search by group */
#define S_IRWXG         0000070         /* [XSI] RWX mask for group */
#define S_IRGRP         0000040         /* [XSI] R for group */
#define S_IWGRP         0000020         /* [XSI] W for group */
#define S_IXGRP         0000010         /* [XSI] X for group */
/* Read, write, execute/search by others */
#define S_IRWXO         0000007         /* [XSI] RWX mask for other */
#define S_IROTH         0000004         /* [XSI] R for other */
#define S_IWOTH         0000002         /* [XSI] W for other */
#define S_IXOTH         0000001         /* [XSI] X for other */

#define S_ISUID         0004000         /* [XSI] set user id on execution */
#define S_ISGID         0002000         /* [XSI] set group id on execution */
#define S_ISVTX         0001000         /* [XSI] directory restrcted delete */
```

## 共享文件
- 描述符表：每个进程都有一个独立的描述符表，表项是由打开的**文件描述符索引**，表项指向**文件表**的一个表项
- 文件表：所有进程共享文件表，文件表的表项组成包括：当前文件位置，引用计数（当前指向该表项的描述符表项数量）
- v-node表：所有进程共享v-node表，每个表项stat结构中的大多数信息，包括st_mode,st_size
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-09-08-31-31.png
width=490px>
</center>

- 多个描述符也可以通过不同的文件表表项引用同一个文件。如果一个同一个文件名调用两次open函数,会生成两个文件描述符，指向不同的文件表，但其实最终是指向同一个文件
    <center>
        <img style="border-radius: 1.125em;
        box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
        src=img/2021-06-09-08-33-36.png
    width=490px>
    </center>

- 子进程有父进程的描述符表的副本，在调用fork()前进程有如图11.11的打开文件。调用fork之后就像下图，因此在内核关闭文件表之前，父子进程都应该要先关闭他们的描述符
- 父子进程虽然是不同的描述符表，但是指向的是同一个文件表！！！
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-09-08-40-44.png
  width=490px>
  </center>



## 11.7 I/O重定向

- I/O重定向符：允许用户将磁盘文件和标准输入输出联系。如Unix> ls > foo.txt shell加载并执行ls程序，将标准输出重定向到磁盘文件foo.txt（即将在终端上面显示的内容，写到了txt文件中）
  ```c
  #include<unistd.h>
  int dup2(int oldfd,int newfd);
  ```
  - dup2函数拷贝**描述符表项oldfd到描述符表项newfd**，
  - 例如dup2(4.1)，。0，1，2分别代表标准输入，输出，错误
    - 在调用之前，描述符（标准输出）对应于文件A比如一个终端shell，描述符4对应一个文件B
    - 调用之后A会被关闭，然后两个描述符都会指向B，此时文件A已经被关闭，文件表和vnode也已经被删除，文件B引用计数增加，任何写到标准输出的数据都会被重定向到B
  <center>
      <img style="border-radius: 1.125em;
      box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
      src=img/2021-06-09-09-10-10.png
  width=490px>
  </center>
  
## 11.8 标准I/O

- 标准I/O库将打开的文件模型化为一个流， 一个流就是一个指向类型为FILE结构的指针！！每一个ANSI C程序开始是都有3个打开的流 stdin,stdout,stderr
```c
#include<stdio.h>
extern FILE *stdin;//标准输入，描述符0
extern FILE *stdout;//标准输出，描述符1
extern FILE *stderr;//标准错误，描述符2
```
- 类型为FILE的流是对文件描述符和流缓冲区的抽象
- 缓冲区可以使开销较高的IO系统调用数量减少，例如有一个程序反复调用标准库中的getc(),每一次getc函数调用相当于调用一次read，第一次getc时会调用read填满缓冲区，然后下一次getc会在流缓冲区中得到服务，不用频繁IO操作

## 11.9 我该使用哪些IO函数

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-06-09-09-23-52.png
width=490px>
</center>

- 尽可能用标准IO，但是对于网络编程，网络的输入输出并不能直接使用标准IO
  - 标准IO从某种意义上来讲是全双工的，程序能在同一个流上执行输入和输出。
  - 两个限定：一个输入函数不能随便跟随在一个输出函数之后；一个输出输出函数不能随便跟在一个输入函数之后，必须要进行flush，fseek，fsetpos或者fwind的调用
  - 由于对套接字使用lseek是非法的
  - 因此尽量rio_函数，UnixIO

