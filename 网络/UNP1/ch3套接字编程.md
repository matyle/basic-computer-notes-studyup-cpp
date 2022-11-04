- 阅读方法 首先通读这一章 （目前进度到3.3）
- 记录重点，精读
- 灵活应用man

## 套接字地址结构


```c
//通用
struct sockaddr {
    sa_family_t sa_family;
    char        sa_data[14];
}
```

### ipv4

- <netinet/in.h>中
  ```c
    #define	__SOCKADDR_COMMON(sa_prefix) \
    sa_family_t sa_prefix##family
  struct in_addr
  {
    in_addr_t s_addr;
  };
  struct sockaddr_in
  {
    __SOCKADDR_COMMON (sin_);
    in_port_t sin_port;			/* Port number.  */
    struct in_addr sin_addr;		/* Internet address.  */

    /* Pad to size of `struct sockaddr'.  */
    unsigned char sin_zero[sizeof (struct sockaddr)
			   - __SOCKADDR_COMMON_SIZE
			   - sizeof (in_port_t)
			   - sizeof (struct in_addr)];
  };
  ```

- 通用套接字地址结构与ipv4互转




### 通用套接字地址结构

- 党作为一个参数传进任何套接字函数时，套接字地址结构总是以引用（或者指针）形式传递。然而以这样的指针作为参数之一的任何套接字必须**支持所有协议族的套接字地址结构（IPv4和IPv6）**
  - 在ANSI C之后解决很简单，void*就行了
  - **但是套接字是在ANSI C之前定义的！当时的方法就是用了一个通用套接字地址结构**
- 因此就涉及到了一个强转 ，将特定协议族的地址结构强制转换为通用套接字地址结构的指针
  ```c
  struct sockaddr_in serv;

  bind(sockfd,(struct sockaddr*)&serv,sizeof(serv));
  ```

### IPv6


### 新的通用套接字地址结构

- 
- struct sockaddr_storage 足以容纳系统所支持的所有任何套接字结构

### 套接字地址比较

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-22-11-19-55.png
width=450px>
</center>


## 值-结果参数
- 指针作为参数传递，同时会写值作为结果返回，传回调用函数（因此不能是const）
- 利用accept中的addr和len参数，会填入
  ```c
  
  ```

1. 从进程到内核传递套接字：bind，connect，sendto


2. 从内核到进程传递套接字地址：accept reacform getsockname,getppeername

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-23-16-58-10.png
width=250px>
</center>

- select三个中间参数都是值-结果参数


## 字节排序函数
- 小端存储：低序字节存储在内存的起始地址
- 大端存储：高位字节存储在内存的起始地址
  ```c
    int c[2] = {0x11223344,0x55667788};

    unsigned char *p = c; //实际赋值的为地址 实际存储 低地址 44332211 88776655 高地址
    // p++;//取77
    // p++; //66
    p +=  4;
    printf("c:%x\n",*p);
  ```

- 网络字节序和本地字节序互转
  ```c
  unit16_t htons(unit16_t host16bitvalue);
  unit32_t htonl(unit32_t host32bitvalue);

  unit16_t ntohs(unit16_t host16bitvalue);
  unit32_t ntohl(unit32_t host32bitvalue);


  ```

## 字节操纵函数
```c
#include<string.h>
void bzero(void *dest,size_t nbytes);
void memset(void *dest,int c,size_t len);
void memcpy(void *dest,const void *scr,size_t nbytes)
int memvmp(const void *ptrl,const void *ptr2,size_t nbytes);
```

## inet_aton inet_addr inet_ntoa



## inet_pton（字符串转数值） i_net_ntop

```c
#include<arpa/inet.h>
//表达式转数值 将点分10进制字符串转为数值 
int inet_pton(int family,const char *strptr,void *addrptr); 

int inet_ntop(int family,const void *addrptr,char *strptr);
```


## readn writen readline(复习csapp 健壮类io)
- 不使用缓冲区的话每次进行read write系统调用非常慢
- 使用面向文本行（特别是stdio的缓冲区）又会出问题，因为stdio缓冲区的状态是不可见的。
- 可以依照缓冲区而不是文本行的要求来考虑Bianca。编写从缓冲区中读取数据的代码。当期待一个文本行的时候，就查看缓冲区中是否含有那一行



