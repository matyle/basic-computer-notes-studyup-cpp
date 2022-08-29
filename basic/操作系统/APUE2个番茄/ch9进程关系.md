## 终端登录

- Unix登录：init进程 为每一个可登录的设备生成一个getty进程。
- macOS登录
- Linux终端登录
- Solaris终端登录

## 网络登录

- BSD网络登录：inetd进程
- macOS：执行网络登录的更好方法ssh
- Linux：xinetd


## 进程组
- 每个进程除了有一个进程ID外，还属于一个进程组。
- 进程组是一个或者多个进程的集合，属于同一作业中结合起来的，同一进程组中的各个进程接收同一终端的各种信号
  ```c
  #include<unistd.h>
  pid_t getpgrp(void); //返回调用进程的进程组ID

  pid_t getpgid(pid_t pid);//指定获取进程pid的进程组id

  //若pid等于0 两个函数等价
  ```

- 每个进程组有一个组长进程，组长进程的pid等于进程组ID
  ```c
  #include<unistd.h>
  int setpgid(pid_t pid,pid_t pgid);//将pid进程的进程组ID设置pgid
  ```
  - 若pid==pgid 则pid成为进程组组长。若pid为0，则使用调用者的pid
  - 通常fork之后调用此函数，使父进程设置其子进程的进程组ID。并且也使子进程设置其自己的进程组id


## 会话

- 会话（session）是一个或者多个进程组的集合。
- 通常由管道将几个进程编成一组
  ```c
  #include<unistd.h>
  pid_t setsid(void); //该进程编成会话首进程，该进程称为一个新进程组的组长进程

  pid_t getsid(pid_t pid);
  ```



## 控制终端

<center>
    <img style="border-radius: 1.125em;
    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08);"
    src=img/2021-07-03-10-59-09.png
width=450px>
</center>


