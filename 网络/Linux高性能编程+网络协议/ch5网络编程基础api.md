### Linux网络编程基础API

#### socket地址API

- 主机字节序和网络字节序
  - 大端字节序：整数的高位字节(23-32)存在内存的低地址处，低位字节存在高地址处
  - 小端字节序：

```c
#include <netinet/in.h>
unsigned long int htonl(unsigned long int hostlong);//主机转网络字节序长整型 用于IP地址
unsigned long int ntohl(unsigned long int netlong);//网络字节序长整型转主机 用于IP地址
unsigned long int htons(unsigned long int hostlong);//主机转网络字节序短整型 用于IP地址
unsigned short int ntohs(unsigned long int netlong);//网络字节序短整型转主机 用于IP地址
```
- 通用socket地址

```c
#include<bits/socket.h>
struct sockaddr{
    sa_family_t sa_family;
    char sa_data[4];
}
//上面的sa_data无法容纳多数地址值
struct sockaddr_storage{
    sa_family_t sa_family;
    unsigned long int __ss_align;
    char _ss_padding[128-sizeof(__ss_align)]
}
//空间大 且对齐
```
- 专用socket地址:实际使用时需要强制转化为通用socket地址 （sockaddr*）&sockaddr socklen_t addrlen
  - sockaddr_in 和sockaddr_in6 分别用于IPv4 和IPv6
  - sin_zero 应该使用函数bzero()或 memset() 来全部置零。它被加入到这个结构，以使struct sockaddr_in的长度和 struct sockaddr 一样
  - 通过sin_zero补 0 ，sockaddr_in结构体 和 sockaddr 结构体等效，一个指向 sockaddr_in 结构体的指针也可以被指向结构体 sockaddr 并且代替它。
  - 结构体 struct sockaddr_in 与 struct sockaddr 等效，只是将每个信息都用一个变量表示，使用时更清晰方便。
- 二者长度一样，都是16个字节，即占用的内存大小是一致的，因此可以互相转化。二者是并列结构，指向sockaddr_in结构的指针也可以指向sockaddr。
- sockaddr常用于bind、connect、recvfrom、sendto等函数的参数，指明地址信息，是一种通用的套接字地址。 
- sockaddr_in 是internet环境下套接字的地址形式。所以在网络编程中我们会对sockaddr_in结构体进行操作，使用sockaddr_in来建立所需的信息，最后使用类型转化就可以了。一般先把sockaddr_in变量赋值后，强制类型转换后传入用sockaddr做参数的函数：sockaddr_in用于socket定义和赋值；sockaddr用于函数参数。

```c
#include<sys/in.h>

struct sockaddr_in{
    sa_family_t sin_family; //IPv4
    u_int16_t sin_port;
    struct in_addr sin_addr;
}
struct in_addr{
    u_int32_t s_addr;
}
```
- IP地址转换函数
  - ntoa函数内部是一个静态变量，因此不可重入
  - inet_addr,inet_aton,inet_ntoa
  - inet_pton，inet_ntop函数
```c
#include <arpa/inet.h>
in_addr_t inet_addr(const char* strptr);//字符串IP地址转网络字节序的整数表示的IP
int inet_aton(const char* cp,struct in_addr* inp); //字符串IP地址转网络字节序的整数表示的IP
char* inet_ntoa(struct in_addr in);//网络字节序的整数表示的IP转字符串

//inet_pton和inet_ntop同时适用于IPv4，与IPv6
int inet_pton(int af,const char* src,void* dst);//af为协议族AF_INET AF_INET6,转换之后的存在dst中
char* inet_ntop(int af, const void* src,char* dst,socklen_t cnt);//cnt指定目标存储单元大小 
//cnt ipv4一般为INET_ADDRSTRLEN 16
//cnt ipv6一般为INET6_ADDRSTRLEN 46
```
#### socket基础API
- 创建socket函数
  - 参数
```c
#include<sys/types.h>
#include<sys/socket.h>
int socket(int domain,int type,int protocol);
//domain协议簇，type表示是使用TCP还是udp，SOCK_STREAM表示tcp，SOCK_DGRAM表示udp
//protocol默认为0
```
- 命名socket 即绑定 
  - 使用bind函数与IP地址绑定,失败返回-1 设置errno：EACCES表示受保护的地址和EADDINUSE表示正在使用
  - 客户端通常不需要显示命名/绑定
```c
#include<sys/types.h>
#include<sys/socket.h>
int bind(int sockfd,const struct sockaddr* myaddr,socklen_t addrlen);
//
```
- 监听socket
  - 参数backlog
```c
#include<sys/socket.h>
int listen(int sockfd,int backlog);
```

- 接受连接
```c
#include<sys/socket.h>
int accept(int sockfd,struct sockaddr *addr,socklen_t *addrlen);
```
- 发起连接
```c
#include<sys/socket.h>
int connect(int sockfd,const struct sockaddr *sever_addr,socklen_t *addrlen);
```
- 关闭连接

##### 数据读写
- TCP数据读写
```c
#include<sys/socket.h>
#include<sys/types.h>
ssize_t recv(int sockfd,void* buf,size_t len,int flags);
ssize_t send(int sockfd,const void* buf,size_t len,int flags);
```

- UDP数据读写
```c
#include<sys/socket.h>
#include<sys/types.h>
ssize_t recvfrom(int sockfd,void* buf,size_t len,int flags,struct sockaddr* addr,socklen_t* addrlen);
ssize_t sendto(int sockfd,const void* buf,size_t len,int flags,struct sockaddr* addr,socklen_t* addrlen);//声明
```
- 通用数据读写
```c
#include<sys/socket.h>
ssize_t recvmsg(int sockfd,struct msghdr* msg,size_t len,int flags);
ssize_t sendmsg(int sockfd,struct msghdr* msg,size_t len,int flags);//声明

struct msghdr{
    void *msg_name;		/* socket地址  */
    socklen_t msg_namelen;	/* Length of address data.  */

    struct iovec *msg_iov;	/* 分散内存块  */
    size_t msg_iovlen;		/* 分散内存块数量 */

    void *msg_control;		/* Ancillary data (eg BSD filedesc passing). */
    size_t msg_controllen;	/* Ancillary data buffer length.
				   !! The type should be socklen_t but the
				   definition of the kernel is incompatible
				   with this.  */
    int msg_flags;		/* Flags on received message.  */
}
```
- 带外标记
```c
#include<sys/socket.h>
#include<sys/types.h>
int sockatmark(int sockfd); //检测fd是否处于带外标记
```
- 地址信息函数
```c
#include<sys/socket.h>
#include<sys/types.h>
int getsockname(int sockfd,struct sockaddr* address,socklen_t* address_len);
//getsockname用于获取本端的socket地址
int getpeername(int sockfd,struct sockaddr* address,socklen_t* address_len);
//getpeername用于获取远端的socket地址
```
- socket选项
```c
#include<sys/socket.h>
#include<sys/types.h>
int getsockopt(int sockfd,int level,int option_name,void* option_value,socklen_t* restrict option_len);


int setsockopt(nt sockfd,int level,int option_name,const void* option_value,socklen_t*  option_len);
```
<center>
  <img style="border-radius: 1.125em;
  box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
  src=img/2021-05-19-16-01-30.png
width=700px>
  <br>
  <div style="color:orange; border-bottom: 1px solid #d9d9d9;
  display: inline-block;
  color: #999;
  padding: 2px;">图片描述</div>
</center>



#### 网络信息API
- gethostbyname和 gethostbyaddr

```c
#include<netdb.h>
struct hostent* gethostbyname(const char* name);
struct hostent* gethostbyaddr(const void* addr,size_t len, int type);

struct hostent{
  char *h_name;			/* Official name of host.  */
  char **h_aliases;		/* Alias list.  */
  int h_addrtype;		/* Host address type.  */
  int h_length;			/* Length of address.  */
  char **h_addr_list;		/* List of addresses from name server.  */
}
```

- getservbyname和 getservbyport 获取服务完整信息
- -getaddinfo 通过主机名获取IP地址，通过服务名获取端口号
- getnameinfo

```c
#include<netdb.h>
struct servent* gethostbyname(const char* name);
struct servent* gethostbyaddr(const void* addr,size_t len, int type);

struct servent{
    char* s_name; //服务名
    char** s_aliases; //服务别名
    int s_port;
    char s_proto//服务类型 udp还是tcp

}
```
