

## 认识ssh

>[SSH](https://link.zhihu.com/?target=https%3A//zh.wikipedia.org/wiki/Secure_Shell) 叫安全外壳协议（**S**ecure **Sh**ell），是一种加密的网络传输协议，可在不安全的网络中网络服务提供安全的传输环境。它通过在网络中创建安全隧道来实现 SSH 客户端和服务器之间的连接。最早的时候，互联网通信都是明文通信，一旦被截获，内容就会被暴露。1995年，芬兰学者 Tatu Ylonen 设计了 SSH 协议，将登录信息全部加密，成为互联网安全的一个基本解决方案，迅速在全世界获得推广，目前已经成为所有操作系统的标准配置。
>
>SSH 是一种协议， 存在多种实现，既有商业实现，也有开源实现（OSSH，OpenSSH）。

## 安装ssh服务

> centos安装

```bash
sudo yum install sshd
#或者安装
sudo yum install openssh-server
```

> ubuntu or debian

```bash
sudo apt-get install sshd
#或者
sudo apt-get install openssh-server
```



## 启动sshd

> 查看sshd 是否启动

```bash
netstat -nptl || grep 22

#显示如下
[root@VM-4-8-centos ~]# netstat -nptl || grep 22
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      2698/sshd     
```



> 启动sshd

```bash
sudo service sshd start 或者 systemctrl start ssh.service
```



> 重启

```bash
　　// ubuntu系统
　　service ssh restart
　　// debian系统
　　/etc/init.d/ssh restart
```



## ssh免密登录

1.客户端生成公钥

```bash
ssh-keygen #一路回车 生成公钥文件
```

2.生成了公钥文件 在~/.ssh文件目录下

>1. id_rsa （私钥）
>2. id_rsa.pub (公钥)

3.传递公钥到服务器（需要登录的电脑）

- 

```bash
ssh-copy-id -i ~/.ssh/id_rsa.pub username@ip 
```

- 报错 如果命令报错（无法安装）

  ```bash
  (base) matytan@matytandeMacBook-Pro ~ % ssh-copy-id -i ~/.ssh/id_rsa.pub tengxun1/usr/bin/ssh-copy-id: INFO: Source of key(s) to be installed: "/Users/matytan/.ssh/id_rsa.pub"
  /usr/bin/ssh-copy-id: INFO: attempting to log in with the new key(s), to filter out any that are already installed
  ```

  >在服务器上面执行ssh-keygen命令就行了，一般新服务器可能会有这个问题

4.为了不每次输入ip地址 我们可以在~/.ssh/config中配置主机名

```bash

Host tx1 # 你的主机名
  HostName 110.xxx.xxx.xxx #你的ip地址
  User root # 你的用户名

```

 然后使用`ssh tx1`就可以链接上服务器啦

## ssh 传输文件

### `scp`命令
scp -r [要传输的文件的路径] [用户]@[ip]:[接收路径]
- 基本参数
    - -r参数传输文件夹
```bash
scp -r
scp -r linux_amd64_server.tar root@110.42...:/root/ 
linux_amd64_server.tar                                                         100%   14MB   9.5MB/s   00:01
```