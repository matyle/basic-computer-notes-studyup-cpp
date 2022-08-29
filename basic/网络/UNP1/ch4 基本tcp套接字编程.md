



## 4.3 connect函数


## 4.4 bind函数
```c
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr,
        socklen_t addrlen);

struct sockaddr_in servaddr;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//内核自己选择ip地址，一般用于服务器
```




## 4.5 listen函数


- backlog 值不超过 未完成队列数量+已完成队列数量
<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-23-17-14-46.png
width=350px>
</center>



## fork和exce函数 clone

- fork之后 子进程拷贝父线程的文件描述符。同时指向同一个文件表，导致引用计数增加，这时子进程应该关闭listenfd，父进程关闭connfd


## 并发服务器

- fork的文件描述符，引用计数等

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-23-17-19-23.png
width=450px>
</center>
