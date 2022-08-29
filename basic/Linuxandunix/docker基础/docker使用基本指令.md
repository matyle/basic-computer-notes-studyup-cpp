---
title: docker基本指令
date: 2021-12-4 20:08:17
tags: docker
categories: 容器
keywords: docker
description: docker images指令，container指令
cover: https://gitee.com/matytan/tupic/raw/master/uPic/69358fdd0b7653e1268a26dd83168483.jpeg
---

### 容器命令

>创建容器：`docker run`

参数

- -it 交互
- -d 后台
- --name 容器名
- -p 主机端口:容器端口  （主机端口映射到docker端口）

```sh
docker run --name centos1 -it -p 33:1234 centos /bin/bash
```



>docker ps  显示容器信息

参数：

- -a
- -q 只显示容器id

```sh
[root@tanqi ~]# docker ps -a
CONTAINER ID   IMAGE          COMMAND           CREATED              STATUS                      PORTS     NAMES
8ad5b2b6854f   centos         "/bin/bash"       About a minute ago   Up About a minute                     centos1
3c45403d051a   centos         "/bin/bash"       14 minutes ago       Exited (0) 14 minutes ago             frosty_cannon

[root@tanqi ~]# docker ps 
CONTAINER ID   IMAGE     COMMAND       CREATED              STATUS              PORTS     NAMES
8ad5b2b6854f   centos    "/bin/bash"   About a minute ago   Up About a minute             centos1
[root@tanqi ~]# docker ps -q
8ad5b2b6854f
[root@tanqi ~]# docker ps -aq
8ad5b2b6854f

```



- 启动容器

> `docker start -i 容器id/容器名

Options:
  -a, --attach               Attach STDOUT/STDERR and forward signals
      --detach-keys string   Override the key sequence for detaching a container
  -i, --interactive          Attach container's STDIN



- 进入容器

>`docker exec -it 容器id/容器名 /bin/bash`  #启动新终端

```sh
[root@tanqi ~]# docker exec -it centos1 /bin/bash

```



> `docker attach 容器id/容器名`  //进入容器直接进入正在执行

```sh
[root@tanqi ~]# docker attach centos1
[root@8ad5b2b6854f /]# 
```

- 退出容器

> exit退出并停止
>
> crtl+P+Q 退出 不停止



- 从容器拷贝到主机

>`docker cp 容器id:容器路径 主机路径`

```sh
[root@tanqi ~]# docker cp centos1:/home/test /root
[root@tanqi ~]# ls
dump.rdb  f1.txt  install.sh  study  test
```

-v 卷的技术





### 其他命令

- 后台创建启动容器：

> `docker run -d centos `# 可能会停止 因为没有前台进程

- 日志

> `docker logs --tail 10 容器id/容器名 ` //看后10条
>
> Options:
>    --details        Show extra details provided to logs
> -f, --follow         Follow log output 新log会加
>    --since string   Show logs since timestamp (e.g. 2013-01-02T13:23:37Z) or relative (e.g. 42m for
>                     42 minutes)
> -n, --tail string    Number of lines to show from the end of the logs (default "all")
> -t, --timestamps     Show timestamps
>    --until string   Show logs before a timestamp (e.g. 2013-01-02T13:23:37Z) or relative (e.g. 42m
>                     for 42 minutes)

```sh
[root@tanqi ~]# docker logs --tail 2 centos1
[root@8ad5b2b6854f home]# ls 
test

[root@tanqi ~]# docker logs -f --tail 2 centos1
[root@8ad5b2b6854f home]# ls 
test
...等待刷新
```



- 运行脚本

> `docker run -d 容器di /bin/sh -c "脚本语言"`

- 进程信息

> `docker top 容器id/容器名`

```sh
[root@tanqi ~]# docker top centos1
UID                 PID                 PPID                C                   STIME               TTY                 TIME                CMD
root                15054               15033               0                   15:30               pts/0               00:00:00            /bin/bash
```

- 元数据

> `docker inspect 容器id/容器名`



### 小结

![image-20211012164355267](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211012164355267.png)