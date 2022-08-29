

### docker的历史

`dotcloud` pass云计算服务

容器化技术

开源思想很重要

官网 docker

看官方文档 docker文档非常详细

dockerhub 仓库地址

### docker能干嘛

虚拟化技术：

容器化技术：

DevOps（开发，运维）

更快递交付和部署

高效的利用硬件资源

### docker 安装



docker基本组成

客户端

服务器：

- 镜像
- 容器
- 仓库

系统内核3.10以上





### 配置阿里云加速





### docker run的流程和docker原理

`docker run hello world`

run的时候发生了什么？



#### 底层原理

docker怎么工作的？
Client-server结构

docker为什么比vm快？

 

> port  	  # 查看映射端口对应的容器内部源端口
> pause	  # 暂停容器
> ps        # 猎户容器列表
> pull      # 从docker镜像源服务器拉取指定镜像或者库镜像
> push      # 推送指定镜像或者库镜像至docker源服务器
> restart   # 重启运行的容器
> rm        # 移除一个或多个容器
> rmi       # 移除一个或多个镜像 （无容器使用该镜像才可删除，否则需要删除相关容器才可继续或 -f 强制删除）
> run       # 创建一个新的容器并运行一个命令
> save      # 保存一个镜像为一个 tar 包【对应 load】
> search    # 在 docker hub 中搜索镜像
> start     # 启动容器
> stop      # 停止容器
> tag       # 给源中镜像打标签
> top       # 查看容器中运行的进程信息
> unpause   # 取消暂停容器
> version   # 查看 docker版本号
> wait      # 截取容器停止时的退出状态值



P13



### docker 安装 centos

官网文档

1. 若安装过 先卸载旧版本

```
 sudo yum remove docker \
                  docker-client \
                  docker-client-latest \
                  docker-common \
                  docker-latest \
                  docker-latest-logrotate \
                  docker-logrotate \
                  docker-engine
```

2. 设置下载源

   安装`yum-utils`包 ，并设置稳定源

   ```sh
    sudo yum install -y yum-utils
   
   #国外源 可能有点慢
   $ sudo yum-config-manager \
       --add-repo \
       https://download.docker.com/linux/centos/docker-ce.repo
   ```

### 镜像基本命令

> `docker images`

参数说明：

- -a
- -q 只显示镜像id
- -f 过滤 只保留符合条件的镜像
- --help

> `docker rmi` + 容器id   #删除镜像
>
> `docker rmi -f $(docker images -aq)`  
>
> ```sh
> $() 表示输入这个结果 到前面
> ```
>
> 

参数：

-f :强制删除

```sh
[root@tanqi ~]# docker rmi -f $(docker images -aq)
Untagged: hello-world:latest
Untagged: hello-world@sha256:9ade9cc2e26189a19c2e8854b9c8f1e14829b51c55a630ee675a5a9540ef6ccf
Deleted: sha256:feb5d9fea6a5e9606aa995e879d862b825965ba48de054caab5ef356dc6b3412
Untagged: centos:latest
Untagged: centos@sha256:a27fd8080b517143cbbbab9dfb7c8571c40d67d534bbdee55bd6c473f432b177
Deleted: sha256:5d0da3dc976460b72c77d94c8a1ad043720b0416bfc16c52c45d4847e53fadb6
```



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
>       --details        Show extra details provided to logs
>   -f, --follow         Follow log output 新log会加
>       --since string   Show logs since timestamp (e.g. 2013-01-02T13:23:37Z) or relative (e.g. 42m for
>                        42 minutes)
>   -n, --tail string    Number of lines to show from the end of the logs (default "all")
>   -t, --timestamps     Show timestamps
>       --until string   Show logs before a timestamp (e.g. 2013-01-02T13:23:37Z) or relative (e.g. 42m
>                        for 42 minutes)

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





- 安装Nginx

  - `docker pull nginx`
  - `docker run  -d --name nginx01 -p 3344:80 nginx`
  - `docker start`
  - `docker exec / attach`

  docker常见错误：

  - `Error response from daemon: driver failed programming external connectivity on endpoint nginx01 (68f0fa06fb21194c14cb9ec041c0e58efa8fccd975b9f7d64f5a4a2ce4e81e87):  (iptables failed: iptables --wait -t nat -A DOCKER -p tcp -d 0/0 --dport 3344 -j DNAT --to-destination 172.17.0.2:80 ! -i docker0: iptables: No chain/target/match by that name.`
    - 重启docker即可 `systemctl restart docker`

- 安装es+kibana

  - `docker run -d --name elasticsearch --net somenetwork -p 9200:9200 -p 9300:9300 -e "discovery.type=single-node" elasticsearch:tag`
  - es配置
  - 限制内存 -e 环境配置

- Portainer

## docker镜像

带代码，运行库，环境变量和配置文件的软件包



- 联合文件系统
- 镜像加载原理
  - bootfs
  - `rootfs`
- 镜像分层原理
  - `docker image inspect`



> `docker commit`
>
> 可选参数：
>
> -m=" 描述信息"
>
> -a="作者"



`cp -r 要拷贝的路径 目标路径`


理解概念，但是一定要实践，最后实践和理论结合







## 容器数据卷

同步机制

> `docker run 可选参数  镜像名`
>
> -d  后台运行
>
> -p 端口映射 主机：容器
>
> --name 容器名
>
> -e 环境配置
>
> 方式一：命令 `-v` 主机目录：容器目录

挂载之后

主机修改自动同步

容器修改也会自动同步到宿主机

绑定的方式

>        "Mounts": [
>            {
>                "Type": "bind",
>                "Source": "/home/centos01/home",
>                "Destination": "/home",
>                "Mode": "",
>                "RW": true,
>                "Propagation": "rprivate"
>            }



于是可以把自己的容器打包为镜像，然后设置文件映射呀！！

### mysql数据同步

mysql 端口3306

利用dockerhub的帮助文档

同时使用-v 主机目录:容器目录 去挂载卷

>`docker run -it --name mysql01 -p 3310:3306 -v mysqlvolume1:/etc/mysql/conf.d -v mysqlvolume02:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=my-secret-pw -d mysql:5.7`
>
>`docker run -it --name mysql02 -p 3311:3306 -e MYSQL_ROOT_PASSWORD=my-secret-pw -d  --volumes-from mysql01 mysql:5.7`

### 具名挂载 匿名挂载

不指定容器外的名字，只指定容器目录 ：匿名挂载

> 匿名挂载
>
> `docker run -v 容器内路径`  # 匿名挂载

>具名挂载 没有指定主机目录
>
>`docker run -v 卷名：/home centos:ro` # 只读
>
>`docker run -v 卷名：/home centos:rw`# 只写

数据卷容器：

> `--volumes-from` 容器间挂载
>
> `docker run -it --name mysql02 -p 3311:3306 -e MYSQL_ROOT_PASSWORD=my-secret-pw -d  --volumes-from mysql01 mysql:5.7`






### docker 登录

> `docker login -u matytan` 
>
> `docker tag +镜像id +版本`
>
> `docker push 镜像id`
>
> ###### 发布到阿里云
>
> 容器镜像服务
>
> 创建命名空间（防止冲突）
>
> 创建容器镜像
>
> 参考官方文档

![image-20211014110447370](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014110447370.png)

## Docker 网络

![image-20211014111241428](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014111241428.png)



> veth-pair技术
>
> 每一个容器都会生成一对网卡
>
> 

![image-20211014144426113](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014144426113.png)



> `--link` #将容器host文件增加一个其他容器的映射



### 自定义网络



```sh
[root@tanqi ~]# docker network ls
NETWORK ID     NAME      DRIVER    SCOPE
b0f6fc63f4fd   bridge    bridge    local
9d4ddd9d6910   host      host      local
21739635bcf7   none      null      local
```



> `docker network create --driver bridge --subnet 192.168.0.0/16  --gateway 192.168.0.1 mynet` 
>
> `docker run -it --name tomcat01 -P --net mynet tomcat`

![image-20211014170814869](https://gitee.com/matytan/tupic/raw/master/uPic/image-20211014170814869.png)

>`docker network connect mynet centos1`
