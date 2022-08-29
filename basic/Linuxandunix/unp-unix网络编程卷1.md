## 第一部分：简介和TCP/IP

### 第一章 概述

#### 1.1 

#### 1.2 一个简单的时间获取例子
- unp.h 系统头文件 --定义了常值，函数等
- 
```cpp
#include	"unp.h"

int main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];//MAXLINE
	struct sockaddr_in	servaddr;////定义sockaddr_in的变量servaddr

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");//socket函数创建一个网际字节流（AF_INET）和字节流套接字（SOCKET_STREAM）,第三个参数为协议，对于protocol为0（IPPROTO_IP)的raw socket。用于接收任何的IP数据包。其中的校验和和协议分析由程序自己完成。，返回值

	bzero(&servaddr, sizeof(servaddr));//类似memset，清零结构体
	servaddr.sin_family = AF_INET;//
	servaddr.sin_port   = htons(13);	/*端口13服务器时间 ，htons函数将数值转为二进制，daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	exit(0);
}

```
- socket函数创建一个网际字节流
- 地址族和端口绑定
- sockaddr_in 结构体意义
- 计算机网络
  - 各层对等实体间交换的单位信息成为`协议数据单元（PDU）`
    - 比如传输层TCP的PDU称为segment 段
    - 网络层叫IP数据报
  
#### 1.3 协议无关性
